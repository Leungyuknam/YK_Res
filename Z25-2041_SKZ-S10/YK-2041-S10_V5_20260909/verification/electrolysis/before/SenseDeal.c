#include "h\Allinclude.h"

// =================================================================================
// 全局/静态变量
// =================================================================================



// 计算出的湿度区间 (Function.c 需要读取它)
U8 xdata g_u8HumiRange = c_HUMI_RANGE_1;

// 湿度补偿值
U8 xdata g_u8HumiComp = 8;

#define C_TANK_WATER_CHECK_IDLE   0
#define C_TANK_WATER_CHECK_WAIT   1
#define C_TANK_WATER_CHECK_READY  2
static U8 xdata s_u8TankWaterCheckState = C_TANK_WATER_CHECK_IDLE;

// =================================================================================
// 内部函数声明
// =================================================================================
static void F_AiHumiRangeEvent(void);
static void F_SenseEvent(void);
static S8 F_SafeConvertTemp(S16 s16RawData);
static U8 F_SafeConvertHumi(U16 u16RawData);

// =================================================================================
// 1. 智能模式区间计算 (严谨匹配规格书 5.4.2)
//    输入: g_stSys.s8DispTemp, g_stSys.u8DispHumi
//    输出: g_u8HumiRange
// =================================================================================
//static void F_AiHumiRangeEvent(void)
//{
//    U8 s_u8RefHumi_High, s_u8RefHumi_Mid, s_u8RefHumi_Low;
//    U8 u8TempIndex = 0; 
//    U8 u8FinalHumi = 0;

//    if(g_stSys.bTurnOn)
//    {
//        // 1. 计算带补偿的内部判定湿度
//        u8FinalHumi = g_stSys.u8DispHumi;
//        if(u8FinalHumi > 99) u8FinalHumi = 99;

//        // 2. 查表获取阈值 (严格按照规格书 5.4.2 温度区间)
//        if(g_stSys.s8DispTemp < 13)       u8TempIndex = 0; // Temp < 13
//        else if(g_stSys.s8DispTemp < 19)  u8TempIndex = 1; // 13 ≤ Temp ≤ 18
//        else if(g_stSys.s8DispTemp < 25)  u8TempIndex = 2; // 18 < Temp < 25
//        else if(g_stSys.s8DispTemp <= 30) u8TempIndex = 3; // 25 ≤ Temp ≤ 30
//        else                              u8TempIndex = 4; // Temp > 30

//        s_u8RefHumi_High = t_AiModeHumiSetData[u8TempIndex][0]; // MAX
//        s_u8RefHumi_Mid  = t_AiModeHumiSetData[u8TempIndex][1]; // KEEP
//        s_u8RefHumi_Low  = t_AiModeHumiSetData[u8TempIndex][2]; // MIN

//        // 3. 施密特触发器状态机：精准实现上升无延迟，下降 -2%RH 回差
//        switch(g_u8HumiRange)
//        {
//            case c_HUMI_RANGE_1: // 当前在区间1 (最低湿)
//                if(u8FinalHumi > s_u8RefHumi_Low) g_u8HumiRange = c_HUMI_RANGE_2; // 上升跨越 MIN
//                break;
//                
//            case c_HUMI_RANGE_2: // 当前在区间2
//                if(u8FinalHumi > s_u8RefHumi_Mid) g_u8HumiRange = c_HUMI_RANGE_3; // 上升跨越 KEEP
//                else if(u8FinalHumi <= (s_u8RefHumi_Low - 2)) g_u8HumiRange = c_HUMI_RANGE_1; // 下降跌破 MIN-2
//                break;
//                
//            case c_HUMI_RANGE_3: // 当前在区间3
//                if(u8FinalHumi > s_u8RefHumi_High) g_u8HumiRange = c_HUMI_RANGE_4; // 上升跨越 MAX
//                else if(u8FinalHumi <= (s_u8RefHumi_Mid - 2)) g_u8HumiRange = c_HUMI_RANGE_2; // 下降跌破 KEEP-2
//                break;
//                
//            case c_HUMI_RANGE_4: // 当前在区间4 (最高湿)
//                if(u8FinalHumi <= (s_u8RefHumi_High - 2)) g_u8HumiRange = c_HUMI_RANGE_3; // 下降跌破 MAX-2
//                break;
//        }
//    }
//}


// --- 温度安全转换：带四舍五入与限幅 ---
static S8 F_SafeConvertTemp(S16 s16RawData)
{
    S16 s16Result;
    // 原始数据为 x10 精度 (例如 255 代表 25.5℃)
    if (s16RawData >= 0) s16Result = (s16RawData + 5) / 10;
    else s16Result = (s16RawData - 5) / 10;
    
    // 限幅处理 (规格书范围 5℃～40℃，此处按显示定义限幅)
    if (s16Result > TEMP_DISP_MAX) s16Result = TEMP_DISP_MAX;
    else if (s16Result < TEMP_DISP_MIN) s16Result = TEMP_DISP_MIN;
    
    return (S8)s16Result;
}

// --- 湿度补偿逻辑：环境湿度 - 8 ---
static U8 F_SafeConvertHumi(U16 u16RawData)
{
    S16 s16Result;
    s16Result = (S16)((u16RawData + 5) / 10); // 原始数据四舍五入
	if(g_u16MCURunTime < 3600)
		s16Result -= 8; 
	else
		s16Result -= 10; 
    
    if (s16Result > HUMI_DISP_MAX) s16Result = HUMI_DISP_MAX;
    else if (s16Result < HUMI_DISP_MIN) s16Result = HUMI_DISP_MIN;
	
    return (U8)s16Result;
}


// --- 物理缺水/提箱采样：10ms 调用一次 ---
void F_CheckWaterLevel(void)
{
    static U8 s_u8TankRemoveDebounce = 0; // 提箱消抖计数器
    static U8 s_u8TankPlaceDebounce  = 0; // 放回消抖计数器
    static U8 s_u8WaterLackCnt = 0;       // 缺水消抖计数器
    static U8 s_u8WaterFullCnt = 0;     // 恢复有水消抖计数器
    
    U8 u8ActiveHalls = 0; // 统计当前被触发的霍尔数量

    // 1. 统计触发数量 (假设低电平 0 为触发)
    if(!P_HALL1) u8ActiveHalls++;
    if(!P_HALL2) u8ActiveHalls++;
    if(!P_HALL3) u8ActiveHalls++;
    if(!P_HALL4) u8ActiveHalls++;

    // =========================================================
    // 2. 提箱逻辑判定 (0个霍尔触发)
    // =========================================================
    if(u8ActiveHalls == 0)
    {
        s_u8TankPlaceDebounce = 0;
        if(s_u8TankRemoveDebounce < 50)
        {
            s_u8TankRemoveDebounce++;
            if(s_u8TankRemoveDebounce >= 50) g_stSys.bTankRemoved = 1; // 连续500ms确认提箱
        }
    }
    else
    {
        s_u8TankRemoveDebounce = 0;
        if(s_u8TankPlaceDebounce < 50)
        {
            s_u8TankPlaceDebounce++;
            if(s_u8TankPlaceDebounce >= 50) g_stSys.bTankRemoved = 0; // 连续500ms确认放回
        }
    }

    // =========================================================
    // 3. 缺水逻辑判定 (前提：必须在没有提箱的情况下)
    // =========================================================
    if(!g_stSys.bTankRemoved)
    {
        if(u8ActiveHalls >= 2) 
        {
            // 触发了 2 个或以上霍尔 -> 缺水
            s_u8WaterFullCnt = 0; 
            if(s_u8WaterLackCnt < 200) s_u8WaterLackCnt++;   /* point6: 2s confirm (200 x 10ms) */
            else
            {
                g_stSys.bWaterLack = 1; // 确认缺水
                if(s_u8TankWaterCheckState == C_TANK_WATER_CHECK_WAIT)
                {
                    s_u8TankWaterCheckState = C_TANK_WATER_CHECK_READY;
                }
            }
        }
        else // u8ActiveHalls == 1
        {
            // 只触发了 1 个霍尔 -> 有水
            s_u8WaterLackCnt = 0;
            if(s_u8WaterFullCnt < 200) s_u8WaterFullCnt++;   /* 2s confirm (200 x 10ms) */
            else
            {
                g_stSys.bWaterLack = 0; // 确认有水
                if(s_u8TankWaterCheckState == C_TANK_WATER_CHECK_WAIT)
                {
                    s_u8TankWaterCheckState = C_TANK_WATER_CHECK_READY;
                }
            }
        }
    }
    else
    {
        // 无水箱时不对外上报缺水；放回后重新完成水位防抖
        s_u8WaterLackCnt = 0;
        s_u8WaterFullCnt = 0;
        g_stSys.bWaterLack = 0;
        s_u8TankWaterCheckState = C_TANK_WATER_CHECK_IDLE;
    }
}


static void F_WaterStatusObserver(void)
{
    static U8 s_bWaterLackBak = 0;
    static U8 s_bTankRemovedBak = 0;

    // 1. 监听提箱状态跳变
    if(s_bTankRemovedBak != g_stSys.bTankRemoved)
    {
        s_bTankRemovedBak = g_stSys.bTankRemoved;

        if(g_stSys.bTankRemoved)
        {
            s_u8TankWaterCheckState = C_TANK_WATER_CHECK_IDLE;
            g_stSys.bCleanRemind = 0;
            g_stSys.u16StandbyTimerMin = 0;
            if(g_stSys.bTurnOn)
            {
                g_stSys.u16Cnt_WakeDisp = C_DISP_WAKE_TIME;
                F_DispTskChange(DIPS_TANK_REMOVED);
            }
        }
        else
        {
            g_stSys.bCleanRemind = 0;
            g_stSys.u16StandbyTimerMin = 0;
            if(g_stSys.bTurnOn)
            {
                // 等水位防抖完成后再决定是否退出风干
                s_u8TankWaterCheckState = C_TANK_WATER_CHECK_WAIT;
                if(g_stSys.bIsAirDrying) F_DispTskChange(DIPS_AIR_DRY);
                else F_DispTskChange(DIPS_NORMAL);
            }
            else
            {
                s_u8TankWaterCheckState = C_TANK_WATER_CHECK_IDLE;
            }
        }
    }

    // 2. 放回水箱后的水位检测完成事件
    if(s_u8TankWaterCheckState == C_TANK_WATER_CHECK_READY)
    {
        s_u8TankWaterCheckState = C_TANK_WATER_CHECK_IDLE;
        s_bWaterLackBak = g_stSys.bWaterLack;

        if(g_stSys.bTurnOn && !g_stSys.bTankRemoved)
        {
            if(g_stSys.bWaterLack)
            {
                // 仍缺水：保留原风干状态、档位和剩余倒计时
                if(g_stSys.bIsAirDrying) F_DispTskChange(DIPS_AIR_DRY);
                else F_DispTskChange(DIPS_WATER_LACK);
            }
            else
            {
                // 已加水：退出风干并恢复正常工作
                if(g_stSys.bIsAirDrying)
                {
                    g_stSys.bIsAirDrying = 0;
                    g_stSys.u16Cnt_AirDry = 0;
                }
                if(g_stSys.u8WorkMode == MODE_SMART) g_stSys.bAiJustEntered = 1;
                F_DispTskChange(DIPS_NORMAL);
            }
        }
        return;
    }

    // 3. 监听正常运行中的缺水/加水跳变
    if(s_bWaterLackBak != g_stSys.bWaterLack)
    {
        s_bWaterLackBak = g_stSys.bWaterLack;

        if(g_stSys.bTurnOn)
        {
            if(g_stSys.bWaterLack)
            {
                if(g_stSys.bIsAirDrying) F_DispTskChange(DIPS_AIR_DRY);
                else F_DispTskChange(DIPS_WATER_LACK);
            }
            else if(!g_stSys.bTankRemoved)
            {
                if(g_stSys.bIsAirDrying)
                {
                    g_stSys.bIsAirDrying = 0;
                    g_stSys.u16Cnt_AirDry = 0;
                }
                if(g_stSys.u8WorkMode == MODE_SMART) g_stSys.bAiJustEntered = 1;
                F_DispTskChange(DIPS_NORMAL);
            }
        }
    }
}
// --- 温湿度事件处理：1s 调用一次 ---
// 处理显示平滑消抖及 FH 故障走字
void F_SenseEvent(void)
{
    static U8  s_u8TempDebounce = 0;
    static U8  s_u8HumiDebounce = 0;
    static U16 s_u16FHStepTimer = 0; 
    static U8  s_bFHDir = 1; 

    // --- A. 传感器通讯正常 ---
    if(!g_stSys.bHumiErr)
    {
        S8 s8TargetTemp;
        U8 u8TargetHumi;
        if(!g_stSys.bHumiDataValid)
        {
            s_u16FHStepTimer = 0;
            return;
        }
        s8TargetTemp = F_SafeConvertTemp(g_stSys.s16TempRaw10);
        u8TargetHumi = F_SafeConvertHumi(g_stSys.u16HumiRaw10);
        if(g_u16MCURunTime <= 5)
        {
            g_stSys.s8DispTemp = s8TargetTemp;
            g_stSys.u8DispHumi = u8TargetHumi;
            
            // 同步清零滤波计数，防止第 6 秒产生误判
            s_u8TempDebounce = 0;
            s_u8HumiDebounce = 0;
        }
        else
        {
			// 温度平滑 (3秒稳定显示)
			if(g_stSys.s8DispTemp != s8TargetTemp) {
				if(++s_u8TempDebounce >= 3) { s_u8TempDebounce = 0; g_stSys.s8DispTemp = s8TargetTemp; }
			} else s_u8TempDebounce = 0;

			// 湿度平滑 (3秒稳定显示)
			if(g_stSys.u8DispHumi != u8TargetHumi) {
				if(++s_u8HumiDebounce >= 3) { s_u8HumiDebounce = 0; g_stSys.u8DispHumi = u8TargetHumi; }
			} else s_u8HumiDebounce = 0;
		}

        s_u16FHStepTimer = 0; 
    }
    // --- B. FH 故障模式 (规格书：上电10秒没读取到数值触发)  ---
    else
    {
        // 故障初始显示 50% 
        if(s_u16FHStepTimer == 0) g_stSys.u8DispHumi = 50; 

        // 每 10 分钟变化 1%，在 55%-60% 之间循环 
        if(++s_u16FHStepTimer >= 600) 
        {
            s_u16FHStepTimer = 1;
            if(s_bFHDir) { if(++g_stSys.u8DispHumi >= 60) s_bFHDir = 0; }
            else { if(--g_stSys.u8DispHumi <= 55) s_bFHDir = 1; }
        }
    }
}



// --- 任务总入口 ---
void F_mAllSenseDeal(void)
{
    // 物理采样放在 10ms 周期
    if(F_GetTimeTick(C_TIME_10ms))
    {
        F_CheckWaterLevel(); 		
    }
	if(F_GetTimeTick(C_TIME_100ms))
	{
		F_WaterStatusObserver();
	}
    
    // 上电500ms提前初始化并触发首帧测量
    if(F_GetTimeTick(C_TIME_500ms))
    {
        F_Zs13PowerOnPrepare();
    }
    
    // 数据处理放在 1s 周期
    if(F_GetTimeTick(C_TIME_1s))
    {
        F_QueryHumiData(); // 驱动 I2C 读取
        F_SenseEvent();    // 计算与走字逻辑
//		F_AiHumiRangeEvent();
    }
}