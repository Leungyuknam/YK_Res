#include "h\Allinclude.h"

// =================================================================================
// 全局/静态变量
// =================================================================================

// =================================================================================
// 静态变量与映射表
// =================================================================================

// 智能模式湿度区间分界值 (TempIndex: <13, <19, <25, <=30, >30)
U8 code t_AiModeHumiSetData[5][3] = {
    {48,45,40},
    {58,55,50},
    {68,65,60}, // 18~25度区间 (最舒适)
    {48,45,40},
    {43,40,35}
};

// 模式 -> 风速等级初始映射表
// 索引: 1档(0), 2档(1), 3档(2), 4档(3), 睡眠(4), 智能(5)
U8 code t_u8ModeSpeedMap[MODE_MAX_COUNT] = {
    SPEED_GEAR_1,   // 0: 手动模式1档 (40%)
    SPEED_GEAR_2,   // 1: 手动模式2档 (60%)
    SPEED_GEAR_3,   // 2: 手动模式3档 (80%)
    SPEED_GEAR_4,   // 3: 手动模式4档 (100%)
    SPEED_SLEEP,    // 4: 睡眠模式 (使用独立的 30% 最低档) 
    SPEED_GEAR_3    // 5: 智能模式 (初始默认中高档，具体由逻辑覆盖)
};

// 风速等级 -> PWM占空比映射表 (最大值 1600)
// 分别对应: 0%, 30%, 40%, 60%, 80%, 100% 
U16 code t_FanDutyMap[6] = {
    0,      // 0: SPEED_STOP
    480,    // 1: SPEED_SLEEP  (30%)
    640,    // 2: SPEED_GEAR_1 (40%)
    960,    // 3: SPEED_GEAR_2 (60%)
    1280,   // 4: SPEED_GEAR_3 (80%)
    1600    // 5: SPEED_GEAR_4 (100%)
};

// --- 时间控制变量 ---
#define C_ANION_ON_TIME_100MS   1200
#define C_ANION_OFF_TIME_100MS  4800
static U8  xdata s_u8WaterLackDelay = 0; // 缺水确认延时
static U8  xdata s_u8AirDryMinCnt = 0;   // 风干分钟计数
static U16 xdata s_u16AnionCycleTick = 0; // 负离子当前阶段累计100ms次数
static U8  xdata s_bAnionCycleOn = 1;    // 1:开2分钟阶段，0:停8分钟阶段
static U8  xdata s_bAnionCycleEnabled = 0; // 总开关条件已生效，防止切入时提前计时

/**************************************************
* 函数：F_CalcAutoFanSpeed
* 功能：AI 智能模式 (湿随温变) 核心状态机与风速计算
* 说明：完美复刻你的迟滞防抖与单向锁死架构
**************************************************/
static U8 F_CalcAutoFanSpeed(void)
{
    S8 t = g_stSys.s8DispTemp; 
    U8 h = g_stSys.u8DispHumi;
    U8 min_h, keep_h, max_h;
    U8 u8TempIndex = 0;
    
    // 历史档位记忆与锁定状态
    static U8 s_u8AiPrevGear = SPEED_GEAR_2; 
    static U8 s_u8AiState = 0;

    // === 异常处理 ===
    if(g_stSys.bHumiErr) return SPEED_GEAR_4; // 规格书：故障强制最高档

    // === 第一步：环境温度与目标湿度映射 ===
    if(t < 13)                u8TempIndex = 0;
    else if(t >= 13 && t <= 18) u8TempIndex = 1;
    else if(t > 18 && t < 25)   u8TempIndex = 2;
    else if(t >= 25 && t <= 30) u8TempIndex = 3;
    else /* t > 30 */         u8TempIndex = 4;

    max_h  = t_AiModeHumiSetData[u8TempIndex][0];
    keep_h = t_AiModeHumiSetData[u8TempIndex][1];
    min_h  = t_AiModeHumiSetData[u8TempIndex][2];

    // === 第二步：AI 模式初次切入逻辑 ===
    if(g_stSys.bAiJustEntered)
    {
        g_stSys.bAiJustEntered = 0;
        s_u8AiPrevGear = SPEED_GEAR_2; // 初始切入时给定中档
        s_u8AiState = 0;               // 重置为上升加湿阶段去重新评估当前环境
    }
    
    // === 第三步：AI 核心控制状态机 ===
    if(s_u8AiState == 1)
    {
        return SPEED_SLEEP; // 单向状态锁死在睡眠档 (直至再次切入 AI)
    }
    else
    {
        // 状态 0：上升加湿阶段
        if(h <= min_h - 2) 
        {
            // 极度干燥：中档加湿
            s_u8AiPrevGear = SPEED_GEAR_2; 
        }
        else if(h <= min_h) 
        {
            // 迟滞防抖区 (MIN-2) ~ MIN 
            if(s_u8AiPrevGear != SPEED_GEAR_2) s_u8AiPrevGear = SPEED_GEAR_1; 
        }
        else if(h <= keep_h - 2) 
        {
            // 接近舒适线：降低为低档
            s_u8AiPrevGear = SPEED_GEAR_1; 
        }
        else if(h <= keep_h)
        {
            // 迟滞防抖区 (KEEP-2) ~ KEEP 
            if(s_u8AiPrevGear != SPEED_GEAR_1) s_u8AiPrevGear = SPEED_SLEEP; 
        }
        else if(h < max_h)
        {
            // 达标缓冲期
            s_u8AiPrevGear = SPEED_SLEEP;
        }
        else
        {
            // 突破上限：单向状态锁死
            s_u8AiState = 1; 
            return SPEED_SLEEP; 
        }
        
        return s_u8AiPrevGear;
    }
}

/**************************************************
* 函数：F_FanSpeedDecision
* 功能：计算风机档位（AI 模式 5 秒防抖延时，其他瞬间生效）
**************************************************/
static void F_FanSpeedDecision(void)
{
    static U8 s_u8DelayTimer = 0; 
    U8 u8TargetSpeed = SPEED_STOP;

    // === 1. 计算逻辑目标档位 ===
    // (优先判定停机和高优打断条件)
    if(!g_stSys.bTurnOn) 
    {
        u8TargetSpeed = SPEED_STOP;
    }
    else if(g_stSys.bIsAirDrying) 
    {
        return; // 保留风干档位，水箱移除时由输出层临时停机
    }
    else if(g_stSys.bTankRemoved)
    {
        u8TargetSpeed = SPEED_STOP;
    }
    else
    {
        switch(g_stSys.u8WorkMode)
        {
            case MODE_SMART:   u8TargetSpeed = F_CalcAutoFanSpeed(); break; 
            case MODE_SLEEP:   u8TargetSpeed = SPEED_SLEEP;          break;
            case MODE_MANUAL_1: u8TargetSpeed = SPEED_GEAR_1;         break;
            case MODE_MANUAL_2: u8TargetSpeed = SPEED_GEAR_2;         break;
            case MODE_MANUAL_3: u8TargetSpeed = SPEED_GEAR_3;         break;
            case MODE_MANUAL_4: u8TargetSpeed = SPEED_GEAR_4;         break;
            default:           u8TargetSpeed = SPEED_GEAR_3;         break;
        }
    }

    // === 2. 执行延时或瞬发逻辑 ===
    // 【核心修正】：仅当处于 AI 模式且正常加湿时，才执行 5 秒防抖倒计时
    if (g_stSys.u8WorkMode == MODE_SMART && g_stSys.bTurnOn && !g_stSys.bTankRemoved && !g_stSys.bIsAirDrying)
    {
        if(u8TargetSpeed != g_stSys.u8FanSpeed)
        {
            if(++s_u8DelayTimer >= 50) // 100ms * 50 = 5秒
            {
                g_stSys.u8FanSpeed = u8TargetSpeed; 
                s_u8DelayTimer = 0;
            }
        }
        else
        {
            s_u8DelayTimer = 0; // 目标和实际一致，清零防抖
        }
    }
    else
    {
        // 非 AI 模式（手动、睡眠、关机等），立刻生效！
        if(!g_stSys.bIsAirDrying) 
        {
            g_stSys.u8FanSpeed = u8TargetSpeed;
        }
        s_u8DelayTimer = 0; // 同步清零倒计时，防止下次切回AI时带有旧数据
    }
}



//static void F_FanSpeedDecision(void)
//{
//    static U8 s_u8DelayCnt = 0; // 用于智能模式切档的 5 秒延迟计时器
//    U8 u8TargetSpeed = SPEED_STOP;

//    // 1. 强制停机条件 (关机)
//    if(!g_stSys.bTurnOn || g_stSys.bTankRemoved)
//    {
//        g_stSys.u8FanSpeed = SPEED_STOP;
//        s_u8DelayCnt = 0; 
//        return;
//    }
//    // 2. 风干模式：优先级最高，维持原设定不变
//    if(g_stSys.bIsAirDrying)
//    {
//        return; 
//    }

//    // 3. 正常运行模式决策
//    switch(g_stSys.u8WorkMode)
//    {
//		case MODE_SMART:
//            // A. 故障拦截：温湿度传感器故障时，智能模式强制高档加湿 [cite: 5]
//            if(g_stSys.bHumiErr)
//            {
//                u8TargetSpeed = SPEED_GEAR_4; 
//            }
//            // B. 预运行：启动前1分钟强制中档 
//            else if(s_u8AiPreRunCnt > 0)
//            {
//                u8TargetSpeed = SPEED_GEAR_2; 
//            }
//            // C. 稳态：根据计算出的湿度区间决定目标风速 
//            else
//            {
//                switch(g_u8HumiRange)
//                {
//                    case c_HUMI_RANGE_1: u8TargetSpeed = SPEED_GEAR_2;   break; // 中档
//                    case c_HUMI_RANGE_2: u8TargetSpeed = SPEED_GEAR_1;   break; // 低档
//                    case c_HUMI_RANGE_3: u8TargetSpeed = SPEED_SLEEP;   break; // 睡眠档
//                    case c_HUMI_RANGE_4: u8TargetSpeed = SPEED_SLEEP;   break; // 睡眠档
//                    default:             u8TargetSpeed = SPEED_GEAR_3;   break;
//                }
//            }
//            
//            // D. 风速档位切换延迟 5 秒机制
//            if(u8TargetSpeed != g_stSys.u8FanSpeed)
//            {
//                if(++s_u8DelayCnt >= 50) // 100ms * 50 = 5秒
//                {
//                    g_stSys.u8FanSpeed = u8TargetSpeed; 
//                    s_u8DelayCnt = 0;
//                }
//            }
//            else
//            {
//                s_u8DelayCnt = 0; // 只要目标和实际一致，清零防抖
//            }
//            break;

//        default:
//            // 其他固定模式：直接查表，且无须延迟 5 秒（保证按键秒响应）
//            if(g_stSys.u8WorkMode < MODE_MAX_COUNT)
//            {
//                g_stSys.u8FanSpeed = t_u8ModeSpeedMap[g_stSys.u8WorkMode];
//            }
//            s_u8DelayCnt = 0; 
//            break;
//    }
//}

// =================================================================================
// 2. 硬件执行函数 (驱动层)
//    功能：将逻辑风速 (Speed Level) 转换为 PWM 占空比并输出
// =================================================================================
void F_FanUpdate(void)
{
    U16 u16TargetDuty = 0;

    // 水箱移除时物理风机停转，但保留风干档位供放回后恢复
    if(!g_stSys.bTankRemoved && g_stSys.u8FanSpeed <= SPEED_GEAR_4)
    {
        u16TargetDuty = t_FanDutyMap[g_stSys.u8FanSpeed];
    }
    
    // 下发给电机驱动
    InFan.AimDuty = u16TargetDuty;
}

// =================================================================================
// 3. 模式变化监听 (边缘检测与初始化)
//    功能：处理开关机、模式切换瞬间的“一次性”动作
// =================================================================================
static void F_ModeChange(void)
{
    static U8 s_bTurnOnBak = 0;
    static U8 s_u8WorkModeBak = 0xFF;

    // --- A. 开关机边缘检测 ---
    if(s_bTurnOnBak != g_stSys.bTurnOn)
    {
        s_bTurnOnBak = g_stSys.bTurnOn;
        
        if(g_stSys.bTurnOn) // 关 -> 开 (上升沿)
        {
            // 1. 负载初始化
            g_stSys.bUvOn = 1;
            g_stSys.bAnionOn = 1;
            // 2. 状态清除
            g_stSys.bIsAirDrying = 0; // 开机打断风干
            g_stSys.u16Cnt_AirDry  = 0;
            s_u8WaterLackDelay = 30;  // 重置缺水延时
            
			g_stSys.bScreenAlwaysOn = 1; // 【新增】每次开机，默认为常亮模式
			if(g_stSys.u8WorkMode == MODE_SLEEP) 
			{
				g_stSys.u16Cnt_WakeDisp = C_DISP_WAKE_TIME;
				g_stSys.bScreenAlwaysOn = 0;
			}
			if(g_stSys.u8WorkMode == MODE_SMART) 
            {
				g_stSys.bAiJustEntered = 1; // 开机时如果是智能模式，触发初次状态判定
            }
			
			// =========================================================
            // 【核心迁移】：开机瞬间的 72H 提醒与 UI 路由
            // =========================================================
			
			if(F_GetDispType() == DIPS_QUICK_TEST)
            {
                // 如果是快检开机，不改变 UI 状态
            }
            // A. 判断刚才这次待机，有没有达标 72H (4320分钟)
            else if(g_stSys.u16StandbyTimerMin >= C_WATER_REMIND_MIN)
            {
                g_stSys.bCleanRemind = 1; // 激活标志
                F_DispTskChange(DIPS_CLEAN_REMIND); // 劫持为清洁动画
            }
			else if(g_stSys.bCleanRemind)
			{
				F_DispTskChange(DIPS_CLEAN_REMIND); // 劫持为清洁动画
			}
			else if(g_stSys.bTankRemoved)
            {
                F_DispTskChange(DIPS_TANK_REMOVED); // 缺水箱，立刻进入提箱报警动画
            }
            else if(g_stSys.bWaterLack)
            {
                F_DispTskChange(DIPS_WATER_LACK);   // 不用清洁，但缺水
            }
            else
            {
                F_DispResetAnim();
                F_DispTskChange(DIPS_NORMAL); // 一切正常
            }
            
            // B. 只要执行了开机，立刻清零待机计时器！下一次关机必定从0算起。
            g_stSys.u16StandbyTimerMin = 0;
            
            // 4. 计算一次初始风速 (防止第一帧风速为0)
            F_FanSpeedDecision(); 
			F_DispResetAnim();
        }
        else // 开 -> 关 (下降沿)
        {    
            // 2. 清除运行状态
            g_stSys.bIsAirDrying = 0;
            g_stSys.u16Cnt_AirDry  = 0;
            g_stSys.u8TimeSetData = 0;
            g_stSys.u16TimeCnt = 0;
			
            // 3. 强制风速归零
            g_stSys.u8FanSpeed = SPEED_STOP;
            InFan.AimDuty = 0; 
        }
		g_bSnCodeError = 0;
    }

    // --- B. 模式切换边缘检测 ---
    if(g_stSys.bTurnOn && (s_u8WorkModeBak != g_stSys.u8WorkMode))
    {
		if(g_stSys.u8WorkMode == MODE_SLEEP)
		{
			if(g_stSys.bScreenAlwaysOn || g_stSys.u16Cnt_WakeDisp > 0)
			{
				g_stSys.u16Cnt_WakeDisp = C_DISP_WAKE_TIME;
			}

			g_stSys.bScreenAlwaysOn = 0;
		}
		else if(s_u8WorkModeBak == MODE_SLEEP)
		{
			g_stSys.bScreenAlwaysOn = 1;
		}
		
		if(g_stSys.u8WorkMode == MODE_SMART) 
		{
			g_stSys.bAiJustEntered = 1; // 开机时如果是智能模式，触发初次状态判定
		}
        
        // 2. 更新历史
        s_u8WorkModeBak = g_stSys.u8WorkMode;
        
        // 3. 立即触发一次风速计算，响应按键
        F_FanSpeedDecision();
		
		F_DispResetAnim();
    }
}



//// =================================================================================
//// 1. 进入风干模式处理 (辅助函数)
////    功能：根据进入风干前的【实际风速档位】设置风干时长和风干档位
////    规格：5.7 风干功能 -> 三、工作过程
//// =================================================================================
//void F_EnterAirDry(void)
//{
//    // 1. 标记状态，正式进入风干
//    g_stSys.bIsAirDrying = 1;
//    
//    // 2. 清除定时状态 (规格书要求：进入风干默认取消定时关机时间) 
//    g_stSys.u8TimeSetData = 0; 
//    g_stSys.u16TimeCnt = 0;
//    
//    // 3. 根据风干前当前的实际运行档位决定风干参数 
//    // 不再区分工作模式(智能/睡眠/手动)，统一按实际风速档位映射：
//    switch(g_stSys.u8FanSpeed)
//    {
//        case SPEED_GEAR_4: 
//            // 风干前档位为4档时，风干时间默认为45分钟 
//            g_stSys.u16Cnt_AirDry  = 45 * 60; 
//            break;
//            
//        case SPEED_GEAR_3:  
//            // 风干前档位为3档时，风干时间默认为65分钟 
//            g_stSys.u16Cnt_AirDry  = 65 * 60; 
//            break;
//            
//        case SPEED_GEAR_2:  
//            // 风干前档位为2档时，风干时间默认为75分钟 
//            g_stSys.u16Cnt_AirDry  = 75 * 60; 
//            break;
//            
//        case SPEED_GEAR_1:
//		case SPEED_SLEEP:
//        default:         
//            // 风干前档位为1档(或停机/异常等)时，风干时间默认为99分钟 
//            g_stSys.u8FanSpeed = SPEED_GEAR_1; // 强制定为1档保底
//            g_stSys.u16Cnt_AirDry  = 99 * 60; 
//            break;
//    }
//}

// =================================================================================
// 进入风干模式处理
// 风干档位：根据进入风干前的工作模式决定
//   MODE_MANUAL_4 / MODE_MANUAL_3       → 3档
//   MODE_MANUAL_2                       → 2档
//   MODE_MANUAL_1 / MODE_SLEEP / MODE_SMART → 1档
// 风干时长：根据【转换后档位】和【环境湿度区间】共同决定
//                  <40%        40%~60%      >60%
//   3档          [短]         [中]         [长]
//   2档          [短]         [中]         [长]
//   1档          [短]         [中]         [长]
// =================================================================================
void F_EnterAirDry(void)
{
    // 1. 标记状态
    g_stSys.bIsAirDrying = 1;

    // 2. 清除定时状态
    g_stSys.u8TimeSetData = 0;
    g_stSys.u16TimeCnt    = 0;

    // 3. 根据进入风干前的工作模式决定风干档位
    switch(g_stSys.u8WorkMode)
    {
        case MODE_MANUAL_4:
        case MODE_MANUAL_3:
            g_stSys.u8FanSpeed = SPEED_GEAR_3;
            break;

        case MODE_MANUAL_2:
            g_stSys.u8FanSpeed = SPEED_GEAR_2;
            break;

        case MODE_MANUAL_1:
        case MODE_SLEEP:
        case MODE_SMART:
        default:
            g_stSys.u8FanSpeed = SPEED_GEAR_1;
            break;
    }

    // 4. 根据【转换后档位】+【环境湿度】共同决定风干时长（单位：秒）
    switch(g_stSys.u8FanSpeed)
    {
        case SPEED_GEAR_3:
            if(g_stSys.u8DispHumi < 40)
            {
                g_stSys.u16Cnt_AirDry = 240 * 60;   // TODO: 3档 + 湿度<40%
            }
            else if(g_stSys.u8DispHumi <= 60)
            {
                g_stSys.u16Cnt_AirDry = 330 * 60;   // TODO: 3档 + 湿度40~60%
            }
            else
            {
                g_stSys.u16Cnt_AirDry = 420 * 60;   // TODO: 3档 + 湿度>60%
            }
            break;

        case SPEED_GEAR_2:
            if(g_stSys.u8DispHumi < 40)
            {
                g_stSys.u16Cnt_AirDry = 300 * 60;   // TODO: 2档 + 湿度<40%
            }
            else if(g_stSys.u8DispHumi <= 60)
            {
                g_stSys.u16Cnt_AirDry = 390 * 60;   // TODO: 2档 + 湿度40~60%
            }
            else
            {
                g_stSys.u16Cnt_AirDry = 480 * 60;   // TODO: 2档 + 湿度>60%
            }
            break;

        case SPEED_GEAR_1:
        default:
            if(g_stSys.u8DispHumi < 40)
            {
                g_stSys.u16Cnt_AirDry = 330 * 60;   // TODO: 1档 + 湿度<40%
            }
            else if(g_stSys.u8DispHumi <= 60)
            {
                g_stSys.u16Cnt_AirDry = 420 * 60;   // TODO: 1档 + 湿度40~60%
            }
            else
            {
                g_stSys.u16Cnt_AirDry = 540 * 60;   // TODO: 1档 + 湿度>60%
            }
            break;
    }
}
// =================================================================================
// 2. 1秒周期时间逻辑 (核心状态流转)
//    功能：处理所有的倒计时、延时启动、状态自动跳转
// =================================================================================
static void F_TimeEvent_Logic(void)
{
	static U8 s_u8StandbySecCnt = 0;
	static U8 s_u8TankRemoveAutoOffSec = 0;
    // --- 缺水风干启动逻辑  缺水60秒后进入风干 ---
    if(g_stSys.bTurnOn && g_stSys.bWaterLack && !g_stSys.bIsAirDrying)
    {
        if(s_u8WaterLackDelay > 0)
        {
            s_u8WaterLackDelay--;
            if(s_u8WaterLackDelay == 0)
            {
                /* V2.6 point5: air-dry entry moved to water-lack animation end (see F_DispWaterLack) */
            }
        }
    }
    else
    {
        // 如果水恢复了，或者已经进风干了，重置延时计数
        s_u8WaterLackDelay = 30; 
    }

    // --- 风干过程倒计时 (分钟级) ---
    if(g_stSys.bTurnOn && g_stSys.bIsAirDrying)
    {
//        if(++s_u8AirDryMinCnt >= 60) // 60秒 = 1分钟
//        {
//            s_u8AirDryMinCnt = 0;
		if(g_stSys.u16Cnt_AirDry > 0)
		{
			g_stSys.u16Cnt_AirDry--;
			if(g_stSys.u16Cnt_AirDry == 0)
			{
				// 风干时间到，关机 (规格书 5.7)
				g_stSys.bTurnOn = 0;
				g_stSys.bIsAirDrying = 0;
				F_DispTskChange(DIPS_NORMAL);
			}
		}
//        }
    }
    else
    {
        s_u8AirDryMinCnt = 0;
    }

	// --- 12H 定时关机逻辑 ---
    // 条件：开机 + 非风干 + 倒计时大于0
    if(g_stSys.bTurnOn && !g_stSys.bIsAirDrying && g_stSys.u16TimeCnt > 0)
    {
        g_stSys.u16TimeCnt--; 

        // 更新显示用的 u8TimeSetData (向上取整算法)
        // 剩 3601秒 -> 显示 2H
        // 剩 3600秒 -> 显示 1H
        // 剩 1秒    -> 显示 1H
        if(g_stSys.u16TimeCnt > 0)
        {
            g_stSys.u8TimeSetData = (g_stSys.u16TimeCnt + 3599) / 3600;
        }
        else
        {
            // 时间到 -> 关机
            g_stSys.u8TimeSetData = 0;
            g_stSys.bTurnOn = 0;
            F_DispTskChange(DIPS_NORMAL);
        }
    }
    else
    {
        // 没开定时，确保同步
        if(g_stSys.u8TimeSetData == 0) g_stSys.u16TimeCnt = 0;
    }
	
	if(!g_stSys.bTurnOn && !g_stSys.bTankRemoved) 
    {
        if(++s_u8StandbySecCnt >= 60) // 累计 60 秒 = 1 分钟
        {
            s_u8StandbySecCnt = 0;
            // 累加待机分钟数 (上限卡在 5000 左右防溢出，72H = 4320 分钟)
            if(g_stSys.u16StandbyTimerMin < 5000) 
            {
                g_stSys.u16StandbyTimerMin++;
            }
        }
    }
    else
    {
		g_stSys.u16StandbyTimerMin = 0;
		s_u8StandbySecCnt = 0;
    }
	
	if(g_stSys.bTurnOn && g_stSys.bTankRemoved)
    {
        if(++s_u8TankRemoveAutoOffSec >= 30) 
        {
            s_u8TankRemoveAutoOffSec = 0; // 清空计时器
            
            g_stSys.bTurnOn = 0;          // 执行关机
            g_stSys.bIsAirDrying = 0;
            g_stSys.u16Cnt_AirDry = 0;
            
            F_DispTskChange(DIPS_NORMAL); // 强制 UI 回落到关机背景
            
            BUZ_POWERON();
        }
    }
    else
    {
        // 如果水箱放回（bTankRemoved 变为 0），或者处于关机状态
        s_u8TankRemoveAutoOffSec = 0; 
    }
}

// =================================================================================
// 4. 负载与指示灯控制（100ms调用）
// =================================================================================
static void F_LoadControl(void)
{
    if(g_stSys.bTurnOn)
    {
        // 1. 负离子控制：四档且水箱正常时，总开关开启，负载开2分钟、停8分钟循环
        if(g_stSys.u8WorkMode == MODE_MANUAL_4 && !g_stSys.bWaterLack && !g_stSys.bTankRemoved)
        {
            // 总开关由关变开时刷新周期；首次只初始化，从负载真正开启后再开始计时
            if(!s_bAnionCycleEnabled)
            {
                s_u16AnionCycleTick = 0;
                s_bAnionCycleOn = 1;
                s_bAnionCycleEnabled = 1;
            }
            else
            {
                s_u16AnionCycleTick++;
                if(s_bAnionCycleOn)
                {
                    if(s_u16AnionCycleTick >= C_ANION_ON_TIME_100MS)
                    {
                        s_u16AnionCycleTick = 0;
                        s_bAnionCycleOn = 0;
                    }
                }
                else if(s_u16AnionCycleTick >= C_ANION_OFF_TIME_100MS)
                {
                    s_u16AnionCycleTick = 0;
                    s_bAnionCycleOn = 1;
                }
            }

            g_stSys.bAnionOn = 1;
            P_FLZ = s_bAnionCycleOn ? 1 : 0;
        }
        else
        {
            s_u16AnionCycleTick = 0;
            s_bAnionCycleOn = 1;
            s_bAnionCycleEnabled = 0;
            g_stSys.bAnionOn = 0;
            P_FLZ = 0;
        }

        // 氛围灯变色 (0-39%橘, 40-65%白, 66%黄)
        if(g_stSys.u8LightState == LIGHT_ON)
        {
            if(g_stSys.u8DispHumi < 40)       g_stSys.u8ColorIdx = 1; 
            else if(g_stSys.u8DispHumi <= 65) g_stSys.u8ColorIdx = 2; 
            else                              g_stSys.u8ColorIdx = 3; 

        }
        else
        {
            g_stSys.u8ColorIdx = 0; 
        }
    }
    else
    {
        s_u16AnionCycleTick = 0;
        s_bAnionCycleOn = 1;
        s_bAnionCycleEnabled = 0;
        g_stSys.bAnionOn = 0;
        P_FLZ = 0;
        g_stSys.u8ColorIdx = 0;
    }
}


// =================================================================================
// 6. 系统初始化 (严谨全量版)
// =================================================================================
void F_mSystemInital(void)
{
    // ==========================================
    // [1] 核心状态与模式 (Core & Mode)
    // ==========================================
    g_stSys.bTurnOn      = 0;               // 默认关机状态
    g_stSys.u8WorkMode   = MODE_MANUAL_4;   // 规格书要求：默认4档 (EEPROM若有记录会在后续覆盖)
    g_stSys.u8FanSpeed   = SPEED_STOP;      // 关机默认风机停止

    // ==========================================
    // [2] 附加功能与负载 (Loads & Features)
    // ==========================================
    g_stSys.bIsAirDrying = 0;               // 严禁上电意外进入风干
    g_stSys.bUvOn        = 0;
    g_stSys.bAnionOn     = 0;

    // ==========================================
    // [3] 定时器与倒计时 (Timers & Counters)
    // ==========================================
    g_stSys.u8TimeSetData = 0;
    g_stSys.u16TimeCnt    = 0;
    g_stSys.u16Cnt_AirDry  = 0;

    // ==========================================
    // [4] 显示与 UI 交互 (Display & UI)
    // ==========================================
    g_stSys.bScreenAlwaysOn = 1;                 // 开机默认允许常亮
    g_stSys.u16Cnt_WakeDisp = C_DISP_WAKE_TIME;  // 默认倒计时备用
    g_stSys.u8Cnt_TimeSet   = 0;                 // 闪烁倒计时清零
    g_stSys.u8DispBright    = 10;                // 默认屏幕全亮 (配合 Gamma 表)
    g_stSys.u8LightState    = LIGHT_ON;          // 氛围灯系统默认开启
    g_stSys.u8ColorIdx      = 0;

    // ==========================================
    // [5] 保护与报警标志 (Protections & Alarms)
    // ==========================================
    g_stSys.bWaterLack         = 0;
    g_stSys.bHumiErr           = 0;
    g_stSys.bTankRemoved       = 0;
    g_stSys.bCleanRemind       = 0;
    g_stSys.u16StandbyTimerMin = 0;              // 待机计时器清零

    // ==========================================
    // [6] 温湿度传感器数据 (Sensor Data)
    // ==========================================
    g_stSys.u8HumiState    = 0;
    g_stSys.u8HumiLinkTime = c_HUMI_LINK_ERR_TIME;
    g_stSys.u16HumiRaw10   = 0;
    g_stSys.bHumiDataValid = 0;
    g_stSys.s16TempRaw10   = 0;
    g_stSys.u8DispHumi     = 50;  // 给定适中初值，防通讯前屏幕闪现 0%
    g_stSys.s8DispTemp     = 25;  // 给定适中初值，防通讯前屏幕闪现 0℃

    // ==========================================
    // [7] EEPROM & 快检配置
    // ==========================================
    g_stSys.bEepromEn       = 1;  // 掉电记忆默认使能
    g_stSys.bEepromWriteBan = 0;  // 允许写入
    g_stSys.bQuickTest    = 0;  // 快检标志位清零，防意外触发产测

	g_stSys.bAiJustEntered = 0;  // 开机默认清零
    // =======================================================
    // C. 静态内部时间轴/延时器复位
    // =======================================================
    s_u8WaterLackDelay  = 30;     // 缺水确认倒数 30 秒 (规格书要求)
    s_u8AirDryMinCnt    = 0;
    s_u16AnionCycleTick  = 0;
    s_bAnionCycleOn     = 1;
    s_bAnionCycleEnabled = 0;
    
    // =======================================================
    // D. 底层硬件驱动复位
    // =======================================================
    InFan.AimDuty = 0;            // 强制清空风机底层占空比
    
    F_TMS6644_Init();             // 屏幕硬件初始化
}


void F_mFunctionSchdule(void)
{
    static U8 s_u8QuickTestTime = 0;    
	
	// 1. 先处理状态变化 (开关机、模式切换的初始化)
    F_ModeChange();
    
    // 2. 100ms 任务：实时控制
    if(F_GetTimeTick(C_TIME_100ms))
    {
        // A. 计算当前应该是什么风速 (逻辑层)
        F_FanSpeedDecision();
        
        // B. 执行风速输出 (驱动层)
        F_FanUpdate();    
        
        // C. 执行其他负载 (UV、灯光)
        F_LoadControl();
    }
    
    // 3. 1s 任务：时间逻辑
    if(F_GetTimeTick(C_TIME_1s))
    {
        F_TimeEvent_Logic();
		
		if(g_stSys.bQuickTest)
        {
            if(++s_u8QuickTestTime >= 180)//3min自动退出快检
            {
                s_u8QuickTestTime = 0;
                g_stSys.bQuickTest = 0;
				BUZ_POWERON();
            }
        }
        else
        {   s_u8QuickTestTime = 0;}
    }
}