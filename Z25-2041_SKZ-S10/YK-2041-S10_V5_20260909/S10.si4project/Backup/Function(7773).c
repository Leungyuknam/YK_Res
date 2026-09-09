#include "h\Allinclude.h"

U8 xdata g_u8FanSpeed = 0;

U16 xdata g_u16Time_1h = c_Time_1h;//1h时间

U8 xdata g_u8AiHumStage = 0;
U8 xdata g_u8AiTime = 0;

bit g_bNetIonLoadBak = 0;           //净离子状态
U16 xdata g_u16NetIonRunTime = 0;   //净离子运行时间

U16 xdata g_u16SenseEnTime = 0;     //传感器使能时间 

bit g_bAiModeStop = 0;              //智慧模式风机停止标记
U8 xdata g_u8PM25UpLimit = 35;      //智能模式，PM25上限值
U8 xdata g_u8PM25DownLimit = 10;    //智能模式，PM25下限值
U16 xdata g_u16AiModeRunTime = 0;   //智能模式，PM25小于下限值时间

StrByte LOAD = {0};

U16 code t_FanSpeed[] = {0,475,800,1160,1450,1540};

/**************************************************
*函数名称：void  F_LoadDry(void) 
*函数功能：负载输出
*入口参数：void
*出口参数：void  
**************************************************/
void F_LoadDry()
{
    U16 temp = 0;
    
    if(g_bNetIonLoad)
    {   P_NETION = 1;}
    else
    {   P_NETION = 0;}
    
    if(g_bSenseLoad)
    {   P_PM25_EN = 0;}
    else
    {   P_PM25_EN = 1;}

    if(g_bIonLoad)
    {   P_ION = 1;}
    else
    {   P_ION = 0;}
    
    if(g_bUvLoad)
    {   P_LED_PWM1 = 1;}
    else
    {   P_LED_PWM1 = 0;}
    
    if(g_u8FanSpeed)
    {
        if((MODE_AI == g_u8WorkMode) && !g_bAirDryingOn && (ucExpdModeSet > 1))
        {
            temp = ucExpdModeOffset & 0x7f;
            if(ucExpdModeOffset & 0x80)
            {//减
                temp = (t_FanSpeed[FAN_MAX] - t_FanSpeed[FAN_SLEEP]) * temp / 100;
                if(t_FanSpeed[g_u8FanSpeed] > temp)
                {
                    temp = t_FanSpeed[g_u8FanSpeed] - temp;
                    if(temp > EXPD_FAN_SPEED_MIN)
                    {   InFan.AimSpeed = temp;}
                    else
                    {   InFan.AimSpeed = EXPD_FAN_SPEED_MIN;}
                }
                else
                {   InFan.AimSpeed = t_FanSpeed[g_u8FanSpeed];}
            }
            else
            {//加
                temp = (t_FanSpeed[FAN_MAX] - t_FanSpeed[FAN_SLEEP]) * temp / 100;
                temp = t_FanSpeed[g_u8FanSpeed] + temp;
                if(temp > EXPD_FAN_SPEED_MAX)
                {   temp = EXPD_FAN_SPEED_MAX;}
                InFan.AimSpeed = temp;
            }
        }
        else
        {   InFan.AimSpeed = t_FanSpeed[g_u8FanSpeed];}
    }
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
    static U8 s_u8WorkModeBak = 0;
    static bit s_bTurnOnBak = 0;
    static bit s_bAirDryingOnBak = 0;
    static U8 s_u8LightStateBak = 0;
    
    if(s_u8WorkModeBak != g_u8WorkMode)
    {
        if(MODE_AI == s_u8WorkModeBak)
        {   ucExpdModeSet = 1;}
        s_u8WorkModeBak = g_u8WorkMode;
        g_bAiModeStop = 0;
        g_u16AiModeRunTime = 0;
    }
    
    if(s_bTurnOnBak != g_bTurnOn)
    {
        s_bTurnOnBak = g_bTurnOn;
        if(!g_bTurnOn)
        {   
            g_u8LightState = LIGHT_ON;
            g_bAirDryingOn = 0;
        }
        g_u16AirDryingTime = 0;
        g_bAiModeStop = 0;
        g_u16AiModeRunTime = 0;
        g_bInFanErr = 0;
        g_bSnCodeError = 0;
    }
    
    if(s_bAirDryingOnBak != g_bAirDryingOn)
    {   
        s_bAirDryingOnBak = g_bAirDryingOn;
        g_u16AirDryingTime = 0;
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
    
    if(g_bShrinkTimeFlg)
    {   g_u16Time_1h = 36;}
    else
    {   g_u16Time_1h = c_Time_1h;}
}
/**************************************************
*函数名称：void  F_AirDryingEvent(void) 
*函数功能：风干工作逻辑
*入口参数：void
*出口参数：void  
**************************************************/
void F_AirDryingEvent()
{
    if(g_u16AirDryingTime >= c_Air_Time)//30min
    {   g_bAirDryingOn = 0;}
    if(g_u8NowHumi < 40)
    {   g_u8FanSpeed = FAN_SLEEP;}
    else if(g_u8NowHumi <= 55)
    {   g_u8FanSpeed = FAN_L;}
    else if(g_u8NowHumi <= 70)
    {   g_u8FanSpeed = FAN_M;}
    else
    {   g_u8FanSpeed = FAN_H;}
}
/**************************************************
*函数名称：void  F_AiModeEvent(void) 
*函数功能：AI模式工作逻辑
*入口参数：void
*出口参数：void  
**************************************************/
void F_AiModeEvent()
{
    U8 l_u8AirQuality = 0;
    
    if(g_u8PM25AirQuality >= g_u8VocAirQuality)
    {
        if(g_u8PM25AirQuality >= g_u8CH2OAirQuality)
        {   l_u8AirQuality = g_u8PM25AirQuality;}
        else
        {   l_u8AirQuality = g_u8CH2OAirQuality;}
    }
    else
    {
        if(g_u8VocAirQuality >= g_u8CH2OAirQuality)
        {   l_u8AirQuality = g_u8VocAirQuality;}
        else
        {   l_u8AirQuality = g_u8CH2OAirQuality;}
    }
    
    if(g_bAiModeStop)
    {   g_u8FanSpeed = 0;}
    else if(c_AIR_QUALITY_ACTOR == l_u8AirQuality)
    {   g_u8FanSpeed = FAN_SLEEP;}
    else if(c_AIR_QUALITY_GOOD == l_u8AirQuality)
    {   g_u8FanSpeed = FAN_L;}
    else if(c_AIR_QUALITY_MID == l_u8AirQuality)
    {   g_u8FanSpeed = FAN_M;}
    else
    {   g_u8FanSpeed = FAN_H;}
    
    if(!g_bAiModeStop)
    {
        if(g_u16Pm25Data < g_u8PM25DownLimit)
        {
            if(g_u16AiModeRunTime >= c_AI_MODE_RUN_TIME)
            {
                g_u16AiModeRunTime = 0;
                g_bAiModeStop = 1;
            }
        }
        else
        {   g_u16AiModeRunTime = 0;}
    }
    else
    {
        g_u16AiModeRunTime = 0;
        if((g_u16Pm25Data > g_u8PM25UpLimit) || (g_u8VocAirQuality >= c_AIR_QUALITY_GOOD) || (g_u8CH2OAirQuality >= c_AIR_QUALITY_GOOD))
        {   g_bAiModeStop = 0;}
    }
}


/**************************************************
*函数名称：void  F_AiModeEvent(void) 
*函数功能：AI模式工作逻辑
*入口参数：void
*出口参数：void  
**************************************************/
void F_AiModeEvent()
{
	if(g_bSwtich2Ai && (g_u8HumiRange != c_HUMI_RANGE_4))//进入智能模式且未达到区间4
	{
		g_bSwtich2Ai = 0;
		g_u16AiModeRunTime = 600;
		g_u8FanSpeed = FAN_M;
	}
	else if(!g_u16AiModeRunTime)
	{
		if(c_HUMI_RANGE_1 == g_u8HumiRange)
		{
			g_u8FanSpeed = FAN_M;
		}
		else if(c_HUMI_RANGE_2 == g_u8HumiRange)
		{
			g_u8FanSpeed = FAN_L;
		}
		else
		{
			g_u8FanSpeed = FAN_SLEEP;
		}
	}
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
    
    if(g_bTurnOn && !g_bInFanErr && !g_bUartErrFlg && !g_bDispUartErr)
    {
        if(s_u8FanInitTime)
        {
            s_u8FanInitTime--;
            g_u8FanSpeed = FAN_L;
        }
        else if(g_bAirDryingOn)
        {   
            F_AirDryingEvent();
        }
        else if(MODE_AI == g_u8WorkMode)
        {   F_AiModeEvent();}
        else 
        {
            if(MODE_LOW == g_u8WorkMode)
            {   g_u8FanSpeed = FAN_L;}
            else if(MODE_MID == g_u8WorkMode)
            {   g_u8FanSpeed = FAN_M;}
            else if(MODE_HIGH == g_u8WorkMode)
            {   g_u8FanSpeed = FAN_MAX;}
            else if(MODE_SLEEP == g_u8WorkMode)
            {   g_u8FanSpeed = FAN_SLEEP;}
        }
    }
    else
    {
        s_u8FanInitTime = 30;
        g_u8FanSpeed = FAN_OFF;
    }
}
/**************************************************
*函数名称：void  F_NetIonEvent(void) 
*函数功能：净离子逻辑
*入口参数：void
*出口参数：void  
**************************************************/
void F_NetIonEvent()
{
    if(g_bTurnOn && g_bNetIonEn && g_u8FanSpeed)
    {
        g_bNetIonLoadBak = 1;
    }
    else
    {   
        g_bNetIonLoadBak = 0;
        g_u16NetIonRunTime = 0;
    }
    
    if(g_u16NetIonRunTime >= c_NETION_RUN_TIME)
    {
        g_bNetIonLoad = 0;
        if(g_u16NetIonRunTime >= (c_NETION_RUN_TIME+c_NETION_STOP_TIME))
        {   g_u16NetIonRunTime = 0;}
    }
    else
    {   g_bNetIonLoad = g_bNetIonLoadBak;}
}
/**************************************************
*函数名称：void  F_SenseEnEvent(void) 
*函数功能：传感器使能5V事件
*调用时间：100ms
*入口参数：void
*出口参数：void  
**************************************************/
void F_SenseEnEvent()
{
    static U8 s_u8Delay = 0;
    
    if(g_bTurnOn)
    {   s_u8Delay = 100;}
    else if(s_u8Delay)
    {   
        s_u8Delay--;
        if(s_u8Delay > 70)
        {   g_u16SenseEnTime = 0xffff;}
        else
        {   g_u16SenseEnTime = 0;}
    }
    
    if(g_bTurnOn || s_u8Delay || (g_u8MCURunTime <= 10))
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
    F_NetIonEvent();
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
        if(++g_u16TimeCountDown >= g_u16Time_1h)
        {
            g_u16TimeCountDown = 0;			
            g_u8TimeData--;
            if(!g_u8TimeData)
            {
                g_bTurnOn = !g_bTurnOn;
                F_DispTskChange(DIPS_NORMAL);
            }
        }
    }
    else
    {	g_u16TimeCountDown = 0;}
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
    g_bNetIonEn = 1;
    
    g_bAirDryingOn = 0;
    g_u8WorkMode = MODE_AI;
    g_u8LightState = LIGHT_ON;
    
    g_u16ScreenRunTime = 0;
    g_bEepromEn = 1;
    g_bShrinkTimeFlg = 0;
    
    g_u8PM25UpLimit = 35;
    g_u8PM25DownLimit = 10;
}
/**************************************************************
//Fuction Name: void F_FunctionSchdule(void)
//Desrition: 系统运行逻辑
//Input: 
//Output:
***************************************************************/
void F_mFunctionSchdule(void)
{   
    static U8 s_u8QuickTestTime = 0;
    
    F_WorkChenge();
    
    if(F_GetTimeTick(C_TIME_100ms))
	{
        F_ModeWork();
    } 
    
    if(F_GetTimeTick(C_TIME_1s))
	{
        if(g_bNetIonLoadBak)
        {
            if(g_u16NetIonRunTime < 0xffff)g_u16NetIonRunTime++;
        }
        
        if(g_u16AiModeRunTime < 0xffff)g_u16AiModeRunTime++;
        
        if(g_u16AirDryingTime < 0xffff)g_u16AirDryingTime++;

        if(g_bQuickTest)
        {
            if(++s_u8QuickTestTime >= 180)//3min自动退出快检
            {
                s_u8QuickTestTime = 0;
                g_bQuickTest = 0;
                g_bScreenInitFlg = 0;
                g_u16ScreenRunTime = 0;
            }
        }
        else
        {   s_u8QuickTestTime = 0;}
        
        F_TimeEvent();
    }
}