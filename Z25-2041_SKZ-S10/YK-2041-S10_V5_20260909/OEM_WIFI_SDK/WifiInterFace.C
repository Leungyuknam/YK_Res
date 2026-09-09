#define  WifiInterFace        
//#include "Define.h"
#include "h\Allinclude.h"

// =======================================================
// 全局备份变量声明 (用于隔离上下发，防止状态回环反杀)
// =======================================================
uchar ucWifiHumFanSpeedSetBak;
uchar ucScreenSaverGradeBak;
uchar ucWifiHumiditySetBak;
uchar ucCheckWaterBak;
uint ucFanDryTimBak;
uchar ucFanDryTim;
uchar ucSelfCleanTimBak;
uchar ucSelfCleanTim;
uchar ucWifiHumModeSetBak;                  
uchar ucWifiScreenSaverTimBak;

uchar fgTimeToClose;

uchar ucR_Buz_cnt;
uint  uiR_Buz_off_time;
uint  uiR_Buz_on_time;

uint ucWfiTimeToCloseBak2 = 0;

U16 u16LocalTargetSec;
U16 u16Diff;
U16 u16NewCnt;
bit g_bSnCodeError = 0; //SN故障

#define C_WIFI_INVALID_MANUAL_MODE  0xFF
static U8 s_u8LastManualMode = MODE_MANUAL_4;

static U8 F_WifiSpeedToManualMode(U8 l_u8Speed)
{
    switch(l_u8Speed)
    {
        case U8_S_SPEED: return MODE_MANUAL_1;
        case U8_L_SPEED: return MODE_MANUAL_2;
        case U8_M_SPEED: return MODE_MANUAL_3;
        case U8_H_SPEED: return MODE_MANUAL_4;
        default:         return C_WIFI_INVALID_MANUAL_MODE;
    }
}

static U8 F_WifiManualModeToSpeed(U8 l_u8Mode)
{
    switch(l_u8Mode)
    {
        case MODE_MANUAL_1: return U8_S_SPEED;
        case MODE_MANUAL_2: return U8_L_SPEED;
        case MODE_MANUAL_3: return U8_M_SPEED;
        case MODE_MANUAL_4: return U8_H_SPEED;
        default:            return U8_H_SPEED;
    }
}
/***************************************************************
Function Name : S_API_Buz(uchar cnt,uint on_time,uchar off_time)
Description : 蜂鸣器输出驱动接口 
Input       : cnt鸣叫次数,on_time单次鸣叫时长,off_time间隔时间
Return      : null
***************************************************************/
void S_API_Buz(uchar cnt, uint on_time, uint off_time)
{                  
    ucR_Buz_cnt = cnt;          
    uiR_Buz_on_time = on_time;
    uiR_Buz_off_time = off_time;
    if(uiR_Buz_on_time)
        F_SetBuz(ucR_Buz_cnt, uiR_Buz_on_time, uiR_Buz_off_time);
}

/***************************************************************
Function Name : UserInterFace()
Description   : 【接收】处理 WiFi 下发的数据 (按最新宏定义与规则对齐)
***************************************************************/
void UserInterFace(void)
{
    U8 l_u8WorkModeBak;
    U8 l_u8ManualMode;

    // =========================================================
    // 1. 开关机同步
    // =========================================================
    if(unS_API_WIFI_COM.fg.OnOff != fgWifiOnOffBak)
    {
        fgWifiOnOffBak = unS_API_WIFI_COM.fg.OnOff;
        if(unS_API_WIFI_COM.fg.OnOff != g_stSys.bTurnOn)
        {
            g_stSys.bTurnOn = unS_API_WIFI_COM.fg.OnOff;
            F_DispTskChange(DIPS_NORMAL);
            if(!g_stSys.bTurnOn)
            {   
                fgSnCodeError = 0; // 关机清异常
            }
        }
    }
    
    // =========================================================
    // 2. 风速同步 (优先级最高：只要APP下发风速，必定强切为手动模式)
    // =========================================================
    if(unS_API_WIFI_COM.HumFanSpeedSet != ucWifiHumFanSpeedSetBak)
    {
        l_u8WorkModeBak = g_stSys.u8WorkMode;
        l_u8ManualMode = F_WifiSpeedToManualMode(unS_API_WIFI_COM.HumFanSpeedSet);

        if(l_u8ManualMode == C_WIFI_INVALID_MANUAL_MODE)
        {
            l_u8ManualMode = s_u8LastManualMode;
            unS_API_WIFI_COM.HumFanSpeedSet = F_WifiManualModeToSpeed(l_u8ManualMode);
        }

        // 风速命令代表手动模式；校验完成后再提交上下发备份，避免非法值污染状态
        ucWifiHumFanSpeedSetBak = unS_API_WIFI_COM.HumFanSpeedSet;
        unS_API_WIFI_COM.HumModeSet = U8_NOR_MODE;
        ucWifiHumModeSetBak = U8_NOR_MODE;
        g_stSys.u8WorkMode = l_u8ManualMode;
        s_u8LastManualMode = l_u8ManualMode;

        if((l_u8WorkModeBak != g_stSys.u8WorkMode) && g_stSys.bTurnOn
            && (g_stSys.bScreenAlwaysOn || g_stSys.u16Cnt_WakeDisp > 0))
        {
            F_DispModeSwitched();
        }
    }

    // =========================================================
    // 3. 模式同步 (自动 / 睡眠 / 手动)
    // =========================================================
    if(unS_API_WIFI_COM.HumModeSet != ucWifiHumModeSetBak)
    {
        l_u8WorkModeBak = g_stSys.u8WorkMode;
        ucWifiHumModeSetBak = unS_API_WIFI_COM.HumModeSet;

        if(unS_API_WIFI_COM.HumModeSet == U8_AI_MODE) // 0x08
        {
            g_stSys.u8WorkMode = MODE_SMART;
        }
        else if(unS_API_WIFI_COM.HumModeSet == U8_SLP_MODE) // 0x07
        {
            g_stSys.u8WorkMode = MODE_SLEEP;
        }
        else if(unS_API_WIFI_COM.HumModeSet == U8_NOR_MODE) // 0x01
        {
            l_u8ManualMode = F_WifiSpeedToManualMode(unS_API_WIFI_COM.HumFanSpeedSet);
            if(l_u8ManualMode == C_WIFI_INVALID_MANUAL_MODE)
            {
                l_u8ManualMode = s_u8LastManualMode;
                unS_API_WIFI_COM.HumFanSpeedSet = F_WifiManualModeToSpeed(l_u8ManualMode);
            }

            ucWifiHumFanSpeedSetBak = unS_API_WIFI_COM.HumFanSpeedSet;
            g_stSys.u8WorkMode = l_u8ManualMode;
            s_u8LastManualMode = l_u8ManualMode;
        }

        if((l_u8WorkModeBak != g_stSys.u8WorkMode) && g_stSys.bTurnOn
            && (g_stSys.bScreenAlwaysOn || g_stSys.u16Cnt_WakeDisp > 0))
        {
            F_DispModeSwitched();
        }
    }
	if(g_stSys.bTurnOn)
	{
		if(unS_API_WIFI_COM.fg.TimeToClose != fgWifiTimCloseBak)
		{
			fgWifiTimCloseBak = unS_API_WIFI_COM.fg.TimeToClose;
			if(0 == unS_API_WIFI_COM.fg.TimeToClose)
			{   
				g_stSys.u16TimeCnt    = 0;
				g_stSys.u8TimeSetData = 0;
			}
		}
		if((unS_API_WIFI_COM.T_TimeToClose != ucWfiTimeToCloseBak2)
			&& (1 == unS_API_WIFI_COM.fg.TimeToClose))
		{
			ucWfiTimeToCloseBak2 = unS_API_WIFI_COM.T_TimeToClose;
			g_stSys.u16TimeCnt = (U16)unS_API_WIFI_COM.T_TimeToClose * 10;
			g_stSys.u8TimeSetData = (g_stSys.u16TimeCnt > 0) ? 
				((g_stSys.u16TimeCnt + 3599) / 3600) : 0;
			if(g_stSys.bTurnOn) { F_DispTskChange(DIPS_TIME_SET); F_DispSetTimeModified(); }
		}
	}

	// =========================================================
	// 5. 风干功能同步
	// =========================================================
	if(unS_API_WIFI_COM.fg.FanDry != fgWifiFanDryBak)
	{
		fgWifiFanDryBak = unS_API_WIFI_COM.fg.FanDry;
//		if(unS_API_WIFI_COM.fg.FanDry && !g_stSys.bIsAirDrying)
//		{
//			if(g_stSys.bTurnOn)
//			{
//				F_EnterAirDry();
//				F_DispTskChange(DIPS_AIR_DRY);
//			}
//		}
//		else if(!unS_API_WIFI_COM.fg.FanDry && g_stSys.bIsAirDrying)
//		{
//			g_stSys.bIsAirDrying = 0;
//			g_stSys.u16Cnt_AirDry = 0;
//			if(g_stSys.u8WorkMode == MODE_SMART) g_stSys.bAiJustEntered = 1;
//			F_DispTskChange(DIPS_NORMAL);
//		}
	}
	// 风干剩余时间同步（仅在风干进行中才接受下发）
	if(unS_API_WIFI_COM.FanDryTim != ucFanDryTimBak)
	{
		ucFanDryTimBak = unS_API_WIFI_COM.FanDryTim;

//		if(unS_API_WIFI_COM.fg.FanDry && g_stSys.bIsAirDrying)
//		{
//			// 1秒单位 → 分钟，向上取整
////			g_stSys.u16Cnt_AirDry = (unS_API_WIFI_COM.FanDryTim + 59) / 60;
//			if(unS_API_WIFI_COM.FanDryTim != g_stSys.u16Cnt_AirDry)
//			{
//				g_stSys.u16Cnt_AirDry = unS_API_WIFI_COM.FanDryTim;
//			}
//		}
	}
	
    // =========================================================
    // 6. 氛围灯同步
    // =========================================================
	if(unS_API_WIFI_COM.fg.BrightStren != fgBrightStrenBak)
	{
		fgBrightStrenBak = unS_API_WIFI_COM.fg.BrightStren;
		if(1 == unS_API_WIFI_COM.fg.BrightStren)
			g_stSys.u8LightState = LIGHT_ON;
		else
			g_stSys.u8LightState = LIGHT_OFF;   // 直接else，0就是关
	}
    
    // =========================================================
    // 7. 息屏控制同步 (0=全亮, 7=息屏)
    // =========================================================
    if(unS_API_WIFI_COM.ScreenSaverGrade != ucScreenSaverGradeBak)
    {
        ucScreenSaverGradeBak = unS_API_WIFI_COM.ScreenSaverGrade;
        
        // 0为常亮，非0视为息屏
        g_stSys.bScreenAlwaysOn = (unS_API_WIFI_COM.ScreenSaverGrade == 0) ? 1 : 0;
        
        // 当 APP 下发息屏时，立刻清零唤醒倒计时，让屏幕立马黑掉
        if(g_stSys.bScreenAlwaysOn == 0) g_stSys.u16Cnt_WakeDisp = 0; 
    }

    // =========================================================
    // 8. 蜂鸣器声音同步 (如果产品支持APP静音控制，可在这里赋值)
    // =========================================================
	if(unS_API_WIFI_COM.fg.Buz_En != fgWifiBuzEnBak)
	{
		fgWifiBuzEnBak = unS_API_WIFI_COM.fg.Buz_En;
		if(1 == unS_API_WIFI_COM.fg.Buz_En)
		{   g_bMuteOn = 0;}
		else
		{   g_bMuteOn = 1;}
	}

	 if(unS_API_WIFI_COM.fg.SelfClean != fgWifiSelfCleanBak)
    {
        fgWifiSelfCleanBak = unS_API_WIFI_COM.fg.SelfClean;
//		if(1 == unS_API_WIFI_COM.fg.SelfClean)
//		{   g_stSys.bCleanRemind = 1;}
//		else
//		{   g_stSys.bCleanRemind = 0;}
    }
}

/***************************************************************
Function Name : S_API_MachineStateChange
Description   : 【发送】将本地状态全量上报给 WiFi 模块 (按最新宏定义与规则)
***************************************************************/
void S_API_MachineStateChange(void)
{
    // =========================================================
    // 1. 基础温湿度与开关机状态
    // =========================================================
    S_API_Room_Temp((U8)g_stSys.s8DispTemp);
    S_API_Hum_Temp(g_stSys.u8DispHumi);
    S_API_S_API_OnOff_Set(g_stSys.bTurnOn);

    // =========================================================
    // 2. 模式与风速映射
    // 规则：风速映射仅针对手动模式，其他模式不需要更新风速
    // =========================================================
    switch(g_stSys.u8WorkMode)
    {
        case MODE_SMART:   
            S_API_HumMode_Set(U8_AI_MODE);
            break; 
            
        case MODE_SLEEP:   
            S_API_HumMode_Set(U8_SLP_MODE);
            break; 
            
        case MODE_MANUAL_1: 
            s_u8LastManualMode = MODE_MANUAL_1;
            S_API_HumMode_Set(U8_NOR_MODE);
            S_API_HumFanSpeed_Set(U8_S_SPEED); // 仅手动模式上报风速
            break; 
            
        case MODE_MANUAL_2: 
            s_u8LastManualMode = MODE_MANUAL_2;
            S_API_HumMode_Set(U8_NOR_MODE);
            S_API_HumFanSpeed_Set(U8_L_SPEED);
            break; 
            
        case MODE_MANUAL_3: 
            s_u8LastManualMode = MODE_MANUAL_3;
            S_API_HumMode_Set(U8_NOR_MODE);
            S_API_HumFanSpeed_Set(U8_M_SPEED);
            break; 
            
        case MODE_MANUAL_4: 
            s_u8LastManualMode = MODE_MANUAL_4;
            S_API_HumMode_Set(U8_NOR_MODE);
            S_API_HumFanSpeed_Set(U8_H_SPEED);
            break; 
    }


	if(g_stSys.u16TimeCnt > 0)
	{
		S_API_TimetoClose_Set(1);
		S_API_TimetoClose_TimSet(g_stSys.u16TimeCnt / 10); 
	}
	else
	{
		S_API_TimetoClose_Set(0);
		S_API_TimetoClose_TimSet(0);
	}
	fgWifiTimCloseBak = unS_API_WIFI_COM.fg.TimeToClose;
	ucWfiTimeToCloseBak2 = unS_API_WIFI_COM.T_TimeToClose;
    // =========================================================
    // 4. 水箱与故障状态上报
    // 规则：缺水根据水箱不在位和缺水检测综合判断。满水100，缺水0。
    // =========================================================
//    if(g_stSys.bWaterLack)// || g_stSys.bTankRemoved)
//    {
////        S_API_CheckWater(U8_NO_WATER);       // 上报水箱缺水 (0)
//		unS_API_WIFI_COM.CheckWater = U8_NO_WATER;
//        S_API_Err_Report(U8_WATERLOW_ERROR); // 上报故障代码 36
//    }
//	if(g_stSys.bHumiErr) 
//	{
//		S_API_Err_Report(U8_FH_ERROR);   // 上报温湿度故障 10
//	}
//    else
//    {
////        S_API_CheckWater(U8_FULL_WATER);     // 上报水箱满水 (100)
//        

////        else 
////        {
//			unS_API_WIFI_COM.CheckWater = U8_FULL_WATER;
//            S_API_Err_Report(U8_NO_ERROR);   // 无故障 0
////        }
//    }
	
	// 水位状态（独立）
	if(g_stSys.bWaterLack)
		unS_API_WIFI_COM.CheckWater = U8_NO_WATER;
	else
		unS_API_WIFI_COM.CheckWater = U8_FULL_WATER;

	// 故障码（可同时存在，按优先级上报）
	if(g_stSys.bTankRemoved)
		S_API_Err_Report(54);
	else if(g_stSys.bWaterLack)
		S_API_Err_Report(U8_WATERLOW_ERROR);
	else if(g_stSys.bHumiErr)
		S_API_Err_Report(U8_FH_ERROR);
	else
		S_API_Err_Report(U8_NO_ERROR);

    // =========================================================
    // 5. 状态标识与特色功能
    // =========================================================
    // 屏显（0全亮、7熄灭）
    S_API_ScreenSaver_Set(g_stSys.bScreenAlwaysOn ? 0 : 2);
	ucScreenSaverGradeBak = unS_API_WIFI_COM.ScreenSaverGrade; 
	
    // 氛围灯开关（0x01开启、0x02关闭）
    // 注：由于不是位操作，直接赋给结构体变量
	unS_API_WIFI_COM.fg.BrightStren = (g_stSys.u8LightState == LIGHT_ON) ? 0x01 : 0x00;
	fgBrightStrenBak = unS_API_WIFI_COM.fg.BrightStren;
    // 余水清洁提示
//    unS_API_WIFI_COM.fg.AddWater = g_stSys.bCleanRemind;
	unS_API_WIFI_COM.fg.SelfClean = g_stSys.bCleanRemind;
	fgWifiSelfCleanBak = unS_API_WIFI_COM.fg.SelfClean;
//	S_API_SelfClean_Set(g_stSys.bCleanRemind);
	
    // 自清洁剩余时间（上传0即可）
    unS_API_WIFI_COM.SelfCleanTim = 0;

    // 负离子开关
    S_API_Anion_Set(g_stSys.bAnionOn);
	fgWifiAnionEnBak = unS_API_WIFI_COM.fg.Anion_En;
	// 风干状态上报
	if(g_stSys.bIsAirDrying)
	{
		S_API_FanDry_Set(1);
		unS_API_WIFI_COM.FanDryTim = g_stSys.u16Cnt_AirDry;
	}
	else
	{
		S_API_FanDry_Set(0);
		unS_API_WIFI_COM.FanDryTim = 0;
	}

    // 蜂鸣器开关 (默认1表示开启提示音)
    if(g_bMuteOn)
    {   S_API_Buz_Set(0);}
    else
    {   S_API_Buz_Set(1);}
	fgWifiBuzEnBak = unS_API_WIFI_COM.fg.Buz_En;
    // =========================================================
    // 6. 其他未使用的功能 (填 0)
    // =========================================================
    S_API_TimetoOpen_Set(0);           
    S_API_TimetoOpen_TimSet(0);
    S_API_Humidity_Set(0);             
    S_API_ScreenSaver_TimSet(0);
}