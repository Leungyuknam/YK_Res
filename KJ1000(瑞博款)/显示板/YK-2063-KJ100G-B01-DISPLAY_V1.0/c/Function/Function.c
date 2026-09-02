#include "h\Allinclude.h"

U8 xdata g_u8FanSpeed = 0;
U8 xdata g_u8AiFanSpeed = 0;        //AI风速

U16 xdata g_u16SenseEnTime = 0;     //传感器使能时间 

U8 xdata g_u8AirQuality = 0;

StrByte LOAD = {0};

//U16 code t_FanSpeed[5] = {0,800,1000,1250,1450};
U16 code t_FanSpeed[5] = {0,500,650,800,1000};
//U16 code t_FanSpeed[5] = {0,400,800,1250,1450};

U8 xdata g_u8VoiceCntTime = 0;
U8 xdata g_u8VoiceState = 0;
U8 xdata g_u8LocalTimeIsNight = 0;		//早晚标志位  0:白天 	1:夜晚

/**************************************************
*函数名称：void  F_LoadDry(void) 
*函数功能：负载输出
*入口参数：void
*出口参数：void  
**************************************************/
void F_LoadDry()
{
    U16 temp = 0;
        
    if(g_bSenseLoad)
    {   P_PM25_EN = 0;}
    else
    {   P_PM25_EN = 1;}

    if(g_bIonLoad)
    {   P_ION = 1;}
    else
    {   P_ION = 0;}
    
//    if(g_bUvLoad)
//    {   P_UV = 1;}
//    else
//    {   P_UV = 0;}
    
    if(g_u8FanSpeed)
    {   InFan.AimSpeed = t_FanSpeed[g_u8FanSpeed];}
    else
    {   InFan.AimSpeed = 0;}
}
/**************************************************
*函数名称：void  F_WorkChenge(void) 
*函数功能：工作切换
*入口参数：void
*出口参数：void  
**************************************************/
void F_WorkChenge()
{
    static bit s_bTurnOnBak = 0;
    static U8 s_u8LightStateBak = 0;
        
    if(s_bTurnOnBak != g_bTurnOn)
    {
        s_bTurnOnBak = g_bTurnOn;
        F_TimeType(0);
        g_bInFanErr = 0;
        if(!g_bTurnOn)
        {   
//						g_u8LightState = LIGHT_HALF;		//待机状态下，开关键半亮
			g_bDLZEn = c_OFF;
			g_bLockEn = 0;
			g_u8PowerOnDispTime = 0;
		}
		else{
			g_bDLZEn = c_ON;
			g_u8WorkMode = MODE_LOW;
			g_u8PowerOnDispTime = C_POWERON_DISP_TIME;
			if(g_bScreenInitFlg)
			{	g_bVoiceFilterEn = 1;}
		}
    }
        
    if(s_u8LightStateBak != g_u8LightState)
    {
        s_u8LightStateBak = g_u8LightState;
        if(LIGHT_OFF == g_u8LightState)
        {   g_u8KeyDispTime = 0;}
        else if(LIGHT_HALF == g_u8LightState)
        {   
            g_u8KeyDispTime = 0;
            g_bHalfDispEn = 1;
        }
    }
//    if(g_u8WorkMode <= MODE_HIGH)
//    {   g_u8SetSpeed = g_u8WorkMode;}
}
/**************************************************
*函数名称：void  F_AiModeEvent(void) 
*函数功能：AI模式工作逻辑
*入口参数：void
*出口参数：void  
**************************************************/
void F_AiModeEvent()
{

		g_u8AirQuality = g_u8PM25AirQuality;
		if(g_u8VocAirQuality > g_u8AirQuality)
				g_u8AirQuality = g_u8VocAirQuality;
		if(g_u8CH2OAirQuality > g_u8AirQuality)
				g_u8AirQuality = g_u8CH2OAirQuality;

    
    if(c_AIR_QUALITY_ACTOR == g_u8AirQuality)
    {   g_u8FanSpeed = FAN_SLEEP;}
    else if(c_AIR_QUALITY_GOOD == g_u8AirQuality)
    {   g_u8FanSpeed = FAN_LOW;}
    else if(c_AIR_QUALITY_MID == g_u8AirQuality)
    {   g_u8FanSpeed = FAN_MID;}
    else
    {   g_u8FanSpeed = FAN_HI;}
    
    g_u8AiFanSpeed = g_u8FanSpeed;
}
/**************************************************
*函数名称：void  F_FanEvent(void) 
*函数功能：风机工作逻辑
*入口参数：void
*出口参数：void  
**************************************************/
void F_FanEvent()
{
    static U8 s_u8FanInitTime = 0;
    
    if(g_bTurnOn && !g_bInFanErr && !g_bQDErr && !g_bDoorOpenLeft & !g_bDoorOpenRight)
    {
        if(s_u8FanInitTime)
        {
            s_u8FanInitTime--;
            g_u8FanSpeed = FAN_MID;
        }
        else if(MODE_AI == g_u8WorkMode)
        {   F_AiModeEvent();}
        else if(MODE_LOW == g_u8WorkMode)
        {   g_u8FanSpeed = FAN_LOW;}
        else if(MODE_MID == g_u8WorkMode)
        {   g_u8FanSpeed = FAN_MID;}
        else if(MODE_HIGH == g_u8WorkMode)
        {   g_u8FanSpeed = FAN_HI;}
//        else if(MODE_PET == g_u8WorkMode)
//        {   g_u8FanSpeed = FAN_PET;}
        else //if(MODE_SLEEP == g_u8WorkMode)
        {   g_u8FanSpeed = FAN_SLEEP;}
    }
    else
    {
        s_u8FanInitTime = 30;
        g_u8FanSpeed = FAN_OFF;
    }
}
/**************************************************
*函数名称：void  F_IonEvent(void) 
*函数功能：负离子逻辑
*入口参数：void
*出口参数：void  
**************************************************/
void F_IonEvent()
{
//		if(MODE_SLEEP == g_u8WorkMode)		//睡眠模式等离子强制关闭
//		{
//			g_bDLZEn = 0;
//		}
//		if(!g_bTurnOn || g_bDoorOpenRight || g_bDoorOpenLeft || g_bQDErr)
//		{	g_bDLZEn = 0;}
		
    if(g_bTurnOn && g_bDLZEn &&
    (MODE_SLEEP != g_u8WorkMode)
		&&(!g_bDoorOpenRight)
		&&(!g_bDoorOpenLeft)
		&&(!g_bQDErr))
    {
        g_bIonLoad = 1;
    }
    else
    {   g_bIonLoad = 0;}
}
///**************************************************
//*函数名称：void  F_UvEvent(void) 
//*函数功能：UV逻辑
//*入口参数：void
//*出口参数：void  
//**************************************************/
//void F_UvEvent()
//{    
//    if(g_bTurnOn && g_u8FanSpeed && (MODE_PET == g_u8WorkMode))
//    {
//        g_bUvLoad = 1;
//    }
//    else
//    {   g_bUvLoad = 0;}
//}
/**************************************************
*函数名称：void  F_SenseEnEvent(void) 
*函数功能：传感器使能5V事件
*调用时间：100ms
*入口参数：void
*出口参数：void  
**************************************************/
void F_SenseEnEvent()
{
//    static U8 s_u8Delay = 0;
    
//    if(g_bTurnOn)
//    {   s_u8Delay = 100;}
//    else if(s_u8Delay)
//    {   
//        s_u8Delay--;
//        if(s_u8Delay > 70)
//        {   g_u16SenseEnTime = 0xffff;}
//        else
//        {   g_u16SenseEnTime = 0;}
//    }
    
//    if(g_bTurnOn || s_u8Delay || (g_u8MCURunTime <= 10))
    if(g_bTurnOn || (g_u8MCURunTime <= 10))
    {   g_bSenseLoad = 1;}
    else
    {   g_bSenseLoad = 0;}
    
    if(g_bSenseLoad)
    {
        if((g_u16SenseEnTime < 0xffff) && g_bTurnOn)
        {   g_u16SenseEnTime++;}
    }
    else
    {   g_u16SenseEnTime = 0;}
}
/**************************************************
*函数名称：void  F_ModeWork(void) 
*函数功能：工作逻辑
*调用时间：100ms
*入口参数：void
*出口参数：void  
**************************************************/
void F_ModeWork(void)
{
    F_FanEvent();
    F_IonEvent();
//    F_UvEvent();
    F_SenseEnEvent();
    F_LoadDry();
}

/**************************************************
*函数名称：void  F_TimeEvent(void) 
*函数功能：定时控制
*调用时间：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_TimeEvent()
{
    if(g_u8TimeData)
    {
        if(g_bShrinkTimeFlg)
        {
            if(++g_u16TimeCountDown >= 60)
            {
                g_u16TimeCountDown = 0;
                g_u8TimeData--;
                if(!g_u8TimeData)
                {
                    g_bTurnOn = !g_bTurnOn;
                    if(!g_bTurnOn)
                    {   g_bLockEn = 0;}
                    F_DispTskChange(DIPS_NORMAL);
                }
            }
        }
        else
        {
            if(++g_u16TimeCountDown >= c_Time_1h)
            {
                g_u16TimeCountDown = 0;
                g_u8TimeData--;
                if(!g_u8TimeData)
                {
                    g_bTurnOn = !g_bTurnOn;
                    if(!g_bTurnOn)
                    {   g_bLockEn = 0;}
                    F_DispTskChange(DIPS_NORMAL);
                }
            }
        }
    }
    else
    {	g_u16TimeCountDown = 0;}
}


/**************************************************
*函数名称：void  F_VoiceEvent(void) 
*函数功能：滤网播报控制
*调用时间：100ms
*入口参数：void
*出口参数：void  
**************************************************/

void F_VoiceEvent(void)
{
    if(g_bVoiceFilterEn)
    {
        g_bVoiceFilterEn = 0;
		if (!g_u8LocalTimeIsNight && g_bTurnOn && !g_u8VoiceState)
		{
		    F_VoiceFliter();
	        g_u8VoiceCntTime = 30;
	        g_u8VoiceState = 1;        
	        return;
		}
    }
    // 处理3秒后的第二次播报
    if(g_u8VoiceState == 1)
    {
        if(--g_u8VoiceCntTime == 0)
        {
            F_VoiceFliter();
            g_u8VoiceState = 0;
        }
    }
}

/**************************************************
*函数名称：void  F_mSystemInital(void) 
*函数功能：逻辑初始
*入口参数：void
*出口参数：void  
**************************************************/
void F_mSystemInital(void)
{
    g_bTurnOn = 0;
    g_bLockEn = 0;
    
    g_u8WorkMode = MODE_AI;
    g_u8LightState = LIGHT_ON;
    g_bDLZEn = c_OFF;
    g_u16ScreenRunTime = 0;	
    g_bEepromEn = 1;
    g_bShrinkTimeFlg = 0;
    
    g_bRgbDispEn = 1;
	
	
	
//	g_bScreenInitFlg = 1;
}
/**************************************************************
//Fuction Name: void F_FunctionSchdule(void)
//Desrition: 系统运行逻辑
//Input: 
//Output:
***************************************************************/
void F_mFunctionSchdule(void)
{       
    F_WorkChenge();
    
    if(F_GetTimeTick(C_TIME_100ms))
	{
        F_ModeWork();
		F_VoiceEvent();
    } 
    
    if(F_GetTimeTick(C_TIME_1s))
	{                        
        F_TimeEvent();
    }
}
