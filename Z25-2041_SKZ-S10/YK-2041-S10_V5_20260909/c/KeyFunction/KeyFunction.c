#include "h\Allinclude.h"


ST_SYSTEM_DATA xdata g_stSys;

// 专门为了填补 WiFi 库的外部引用而定义的空变量
unsigned char unFlagKey1 = 0;

// =================================================================================
// 内部辅助函数
// =================================================================================

/**************************************************
* 函数名称：F_KeyWakeUpCheck
* 入口参数：u8KeyType
* 0: 定时/模式短按 (可拦截，息屏仅唤醒)
* 1: 开关键 (已开机息屏时仅唤醒)
* 2: 定时/模式长按组合 (可拦截)
* 3: 灯光键短按 (不拦截，息屏仅唤醒)
* 4: 灯光键长按 (不拦截)
**************************************************/
U8 F_KeyWakeUpCheck(U8 u8KeyType)
{
    U8 bExecute = 1; 
    U8 bIsScreenPhysicalOff = 0; 
    
    if((DIPS_POWERUP == F_GetDispType()) && u8KeyType != 1) return 0;
    if(C_SelfTest == g_u8TestState) { BUZ_KEY(); return 0; }
    
    S_API_Key_Tuch();
    
    if(g_stSys.bScreenAlwaysOn == 0 && g_stSys.u16Cnt_WakeDisp == 0)
    {
        bIsScreenPhysicalOff = 1;
    }

    g_stSys.u16Cnt_WakeDisp = C_DISP_WAKE_TIME;

    // ========================================================
    // 【异常状态拦截】：水箱/缺水/风干时限制可操作按键
    // ========================================================
    if(g_stSys.bTurnOn && (g_stSys.bTankRemoved || g_stSys.bWaterLack || g_stSys.bIsAirDrying))
    {
        if(u8KeyType == 0 || u8KeyType == 2) 
        {
            BUZ_KEYINVAL(); // 滴滴两声无效音
            g_stSys.u8AirDryWarnTick = 20; // 注入 2 秒生命周期，通知 UI 强闪风干灯
            return 0; // 彻底拦截
        }
        if(g_stSys.bTankRemoved && (u8KeyType == 3 || u8KeyType == 4))
        {
            BUZ_KEYINVAL();
            return 0;
        }
    }

    // ========================================================
    // 【息屏唤醒拦截】：黑屏下，短按只唤醒屏幕不执行功能
    // ========================================================
    if(bIsScreenPhysicalOff && g_stSys.bTurnOn)
    {
        // 0=模式/定时短按，1=开关键短按，3=灯光短按
        if(u8KeyType == 0 || u8KeyType == 1 || u8KeyType == 3) 
        {
            return 0; 
        }
    }
    
    return bExecute;
}



///**************************************************
//* 函数名称：F_KeyWakeUpCheck
//* 函数功能：按键唤醒检查与拦截机制 (全局门神)
//* 入口参数：u8KeyType (0:普通短按, 1:开关键短按, 2:长按或组合键)
//* 返回值：  1=放行执行按键功能, 0=拦截(仅唤醒屏幕或报警)
//**************************************************/
//U8 F_KeyWakeUpCheck(U8 u8KeyType)
//{
//    U8 bExecute = 1; 
//    U8 bIsScreenPhysicalOff = 0; // 屏幕物理上是否处于黑屏状态
//	
//	if((DIPS_POWERUP == F_GetDispType()) && u8KeyType != 1)
//    {   return 0;}
//	
//	if(C_SelfTest == g_u8TestState)
//	{
//		BUZ_KEY();
//		return 0;
//	}
//	
//    S_API_Key_Tuch();
//	
//    // 【智能判断】：当前屏幕到底是不是黑的？
//    if( (g_stSys.bScreenAlwaysOn == 0 || g_stSys.u8WorkMode == MODE_SLEEP) && g_stSys.u16Cnt_WakeDisp == 0)
//    {
//        bIsScreenPhysicalOff = 1;
//    }

//    // 1. 无条件重置唤醒倒计时 (只要有按键，屏幕必须亮)
//    if(g_stSys.u8WorkMode == MODE_SLEEP) g_stSys.u16Cnt_WakeDisp = C_DISP_WAKE_TIME;
//    else g_stSys.u16Cnt_WakeDisp = C_DISP_WAKE_TIME;

//    // 2. 如果屏幕是黑的，拦截普通按键 (仅负责唤醒，不执行功能)
//    if(bIsScreenPhysicalOff && g_stSys.bTurnOn)
//    {
//        if(u8KeyType == 0) 
//        {
//            return 0; // 成功拦截，画面会因为倒计时重置而亮起
//        }
//    }

//    // 3. 【核心新增】：缺水或风干的全局霸道拦截！
//    // 只要是开机状态，并且处于缺水或风干或无水箱
//    if(g_stSys.bTurnOn && (g_stSys.bTankRemoved || g_stSys.bWaterLack || g_stSys.bIsAirDrying))
//    {
//        // 开关键(1)拥有最高特权，不拦截。其他所有键统统拦截！
//        if(u8KeyType != 1) 
//        {
//            BUZ_KEYINVAL(); // 蜂鸣器提示两声无效音
//			if(g_stSys.bIsAirDrying)
//			{
//				g_stSys.u8AirDryWarnTick = 20; // 设定闪烁寿命为 20 个 100ms = 2秒
//			}
//            return 0;       // 彻底拦截
//        }
//    }
//    
//    return bExecute;
//}

// =================================================================================
// 外部按键接口 (短按)
// =================================================================================

/**************************************************
* 函数名称：void F_PowerKey(void) 
* 函数功能：开关机按键
* 修改点：只反转 bTurnOn，负载控制全部移交 Function.c
**************************************************/
void F_PowerKey(void)
{
    if(F_KeyWakeUpCheck(1)) 
    {
        g_stSys.bTurnOn = !g_stSys.bTurnOn;
        
        BUZ_KEY(); 
        F_DispTskChange(DIPS_NORMAL); 
    }
}

/**************************************************
* 函数名称：void F_ModeKey(void)
* 函数功能：模式切换 
* 修改点：只修改 u8WorkMode，不设置 u8FanSpeed
**************************************************/
void F_ModeKey(void)
{

    if(F_KeyWakeUpCheck(0)) 
    {     
        if(!g_stSys.bTurnOn) return;

        if(F_GetDispType() != DIPS_MODE_SET)
        {
            // first press: enter mode/gear setting state, do NOT change mode yet
            F_DispEnterModeSet();
        }
        else
        {
            // already in setting: cycle to next mode/gear
            g_stSys.u8WorkMode++;
            if(g_stSys.u8WorkMode >= MODE_MAX_COUNT)
            {
                g_stSys.u8WorkMode = 0;
            }
            F_DispModeSwitched();
        }

        BUZ_KEY();
    }
}

// =================================================================================
// 接口函数：设置定时时间
// 入口参数：u8Hour (0, 2, 4... 12)
// =================================================================================
void F_TimeType(U8 u8Hour)
{
    // 1. 边界保护
    if(u8Hour > 12) u8Hour = 0;

    // 2. 更新状态变量
    g_stSys.u8TimeSetData = u8Hour;

    // 3. 核心计算：将小时转换为秒 (1小时 = 3600秒)
    // 这样倒计时逻辑就可以统一按秒递减，显示更精准
    if(u8Hour > 0)
    {
        g_stSys.u16TimeCnt = (U16)u8Hour * 3600;
    }
    else
    {
        g_stSys.u16TimeCnt = 0;
    }
}


/**************************************************
* 函数名称：void F_TimeKey(void)
* 函数功能：定时设置键
* 交互规则：首次查看，再次循环 (0 -> 2 -> 4 -> ... 12 -> 0)
**************************************************/
void F_TimeKey(void)
{
    // 传入 0，遇到息屏/暗屏时仅唤醒屏幕，拦截本次按键功能
    if(F_KeyWakeUpCheck(0))
    {
        if(!g_stSys.bTurnOn) return; // 关机状态下不响应

        // 1. 首次按下：进入定时设置界面
        if(F_GetDispType() != DIPS_TIME_SET)
        {
            // 安全检查：如果倒计时已经自然结束(u16TimeCnt==0)，务必重置显示档位
            // 防止上次设了4H结束了，这次一按进来直接显示4H的假象
            if(g_stSys.u16TimeCnt == 0) 
            {
                g_stSys.u8TimeSetData = 0;
            }
            
            // 切换到定时闪烁任务 (寿命30Ticks=3秒)
            F_DispTskChange(DIPS_TIME_SET);
        }
        // 2. 再次按下：循环切换定时时间
        else
        {
            U8 u8NextTime = g_stSys.u8TimeSetData + 2;
            if(u8NextTime > 12) u8NextTime = 0;

            // 统一调用转换函数 (内部已自动将 u8TimeSetData 转为 u16TimeCnt 秒数倒计时)
            F_TimeType(u8NextTime);
            
            // 重置一下 DIPS_TIME_SET 任务，让闪烁倒计时重新开始(3秒)
            F_DispTskChange(DIPS_TIME_SET);
        }
        
        BUZ_KEY(); // 每次有效按下都给单声音反馈
    }
}
///**************************************************
//* 函数名称：void F_FWLightKey(void)
//* 函数功能：灯光键 (短按：开关氛围灯)
//**************************************************/
//void F_FWLightKey(void)
//{
//    if(F_KeyWakeUpCheck(0))
//    {
//        if(!g_stSys.bTurnOn) return;
//        if(F_GetDispType() == DIPS_TIME_SET)
//        {
//			F_DispTskChange(DIPS_NORMAL);
//		}
//        // 简单切换：亮 <-> 灭
//        if(g_stSys.u8LightState == LIGHT_ON)
//        {
//            g_stSys.u8LightState = LIGHT_OFF;
//        }
//        else
//        {
//            g_stSys.u8LightState = LIGHT_ON;
//        }
//        
//        BUZ_KEY();
//    }
//}


// ========================================================
// 【同步修改按键传入的 Type 标识】
// ========================================================
void F_FWLightKey(void)
{
    if(F_KeyWakeUpCheck(3)) // 【修改为 3：灯光短按】
    {
        if(!g_stSys.bTurnOn) return;
        if(F_GetDispType() == DIPS_TIME_SET) F_DispTskChange(DIPS_NORMAL);
        
        g_stSys.u8LightState = (g_stSys.u8LightState == LIGHT_ON) ? LIGHT_OFF : LIGHT_ON;
        BUZ_KEY();
    }
}

void F_ScreenLightKey(void)
{
    if(!g_stSys.bTurnOn) return;
    if(DIPS_POWERUP == F_GetDispType()) return;
    
    if(F_KeyWakeUpCheck(4)) // 【修改为 4：灯光长按】
    {
        g_stSys.bScreenAlwaysOn = (g_stSys.bScreenAlwaysOn == 1) ? 0 : 1;
        g_stSys.u16Cnt_WakeDisp = g_stSys.bScreenAlwaysOn ? C_DISP_WAKE_TIME : 0; 
        BUZ_KEY();
    }
}


// =================================================================================
// 长按 / 组合按键接口
// =================================================================================

/**************************************************
* 函数名称：void F_WifiKey(void)
* 函数功能：触发AP配网 (长按模式键)
**************************************************/
void F_WifiKey(void)
{
	if(DIPS_POWERUP == F_GetDispType())
    {   return;}

    if(!F_KeyWakeUpCheck(2)) return; // 唤醒屏幕
	S_API_AP_Tuch();
}

/**************************************************
* 函数名称：void F_WifiCancelKey(void)
* 函数功能：取消配网
**************************************************/
void F_WifiCancelKey(void)
{
	if(DIPS_POWERUP == F_GetDispType())
    {   return;}
    if(g_u16MCURunTime >= 300) return;
    if(!F_KeyWakeUpCheck(2)) return; // 唤醒屏幕

    S_API_CANCEL_WIFI_Tuch();
}

/**************************************************
* 函数名称：void F_WifiResetKey(void)
* 函数功能：重置模块
**************************************************/
void F_WifiResetKey(void)
{
	if(DIPS_POWERUP == F_GetDispType())
    {   return;}
    if(g_u16MCURunTime >= 300) return;
    if(!F_KeyWakeUpCheck(2)) return; // 唤醒屏幕

	S_API_WIFI_RST_Tuch();
}


/**************************************************
* 函数名称：void F_QuickTestKey(void)
* 函数功能：进入自检 (上电15秒内长按开关键)
**************************************************/
void F_QuickTestKey(void)
{
	if(DIPS_POWERUP == F_GetDispType())
    {   return;}
    if(g_u16MCURunTime > 15) return;
    if(g_stSys.bQuickTest || g_stSys.bTurnOn) return;
	if(!F_KeyWakeUpCheck(2)) return; // 唤醒屏幕

	F_mSystemInital();
	
	g_stSys.bQuickTest = 1;
	g_stSys.bEepromWriteBan = 1;
	if(g_stSys.bEepromEn)
    {   F_EepromWriteCtrl();}
	g_stSys.bTurnOn = 1;
    S_API_WIFI_FastCheck_Tuch();
    BUZ_POWERON();
    F_DispTskChange(DIPS_QUICK_TEST);
    BUZ_KEY();
}


