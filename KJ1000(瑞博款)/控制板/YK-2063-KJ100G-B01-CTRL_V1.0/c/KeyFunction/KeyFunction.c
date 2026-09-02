#include "h\Allinclude.h"

U8 xdata g_u8KeyData = 0;
U8 xdata g_u8TimeData = 0;          //定时控制，单位H
U16 xdata g_u16TimeCountDown = 0;   //定时倒计时
U8 xdata g_u8WorkMode = MODE_LOW;    //模式
U8 xdata g_u8SetSpeed = 0;          //风速,模式低中高档对应速度，用于wifi控制
U8 xdata g_u8LightState = LIGHT_ON; //灯状态

bit g_bTurnOn = 0;     //开关机标志
bit g_bLockEn = 0;     //童锁使能
bit g_bQuickTest = 0;  //快检标记
bit g_bDLZEn = 0;			 //等离子使能

/*****************************************************************************
* 函数名称：void F_TriggerFilterVoiceOnce(void) 
* 函数功能：全局单次滤网语音触发器
******************************************************************************/
void F_TriggerFilterVoiceOnce(void)
{
    // 只有在开机状态，且滤网寿命已到时，才装载 1 次播报任务
    if(g_bTurnOn && g_bScreenInitFlg)
    {
        // 如果当前没有正在播报的任务，才触发（防止用户狂按导致语音重叠）
        if(g_u8VoiceFilterBeepCnt == 0) 
        {
            g_u8VoiceFilterBeepCnt = 1;
            g_u8VoiceCntTime = 0; // 立即播报
        }
    }
}


/**************************************************
*函数名称：void  F_ModeSleepReturnKey(void) 
*函数功能：睡眠模式按键逻辑控制
*入口参数：void
*出口参数：0为跳过按键 1为执行
*备    注：F_LockLongKey,F_LightKey()不受此控制
**************************************************/
U8 F_ModeSleepReturnKey()
{ 
		if((g_u8WorkMode == MODE_SLEEP)&&(!g_bLockEn)&&(g_bTurnOn))
		{
			g_u8WorkMode = MODE_LOW;
			F_BuzzerSet(BUZ_SHORT);
			return 0;
		}
		else {
			return 1;
		}
}


/**************************************************
*函数名称：void  F_RetuenKey(void) 
*函数功能：短按键跳过控制,童锁跳过处理及睡眠唤醒处理
*入口参数：void
*出口参数：0为跳过按键 1为执行
*备    注：F_LockLongKey,F_LightKey()不受此控制
**************************************************/
U8 F_RetuenKey()
{    
    if(g_u8TestState)
	{	
		F_BuzzerSet(BUZ_SHORT);
		return 0;
    }
	 
	if(g_bQDErr || g_bDoorOpenLeft || g_bDoorOpenRight)
	{
		return 0;
	}
    
    if((DIPS_POWERUP == F_GetDispType()) || g_u8TestState)
    {   return 0;}	
    
    if(g_bLockEn)
    {
        g_u8LockBlinkTime = C_LOCK_BLINK_TIME;
        F_BuzzerSet(BUZ_LOCK);
        return 0;
    }    
	F_TriggerFilterVoiceOnce();
    
    return 1;
}
/**************************************************
*函数名称：void  F_RetuenKey(void) 
*函数功能：长按键跳过控制
*入口参数：void
*出口参数：0为跳过按键 1为执行
*备    注：F_PowerKey(),F_LockLongKey,F_LightKey()不受此控制
**************************************************/
U8 F_RetuenLongKey()
{    
    if(g_u8TestState)
	{	
//        g_u8TestState = C_TestOff;
        F_BuzzerSet(BUZ_SHORT);
        return 0;
    }
		
	if(g_bQDErr || g_bDoorOpenLeft || g_bDoorOpenRight)
	{
		return 0;
	}
    
    if((DIPS_POWERUP == F_GetDispType()) || g_u8TestState)
    {   return 0;}
		
    if(g_bLockEn)
    {
        g_u8LockBlinkTime = C_LOCK_BLINK_TIME;
        F_BuzzerSet(BUZ_LOCK);
        return 0;
    }    
    F_TriggerFilterVoiceOnce();
    return 1;
}
/**************************************************
*函数名称：void  F_PowerKey(void) 
*函数功能：开关控制
*入口参数：void
*出口参数：void  
**************************************************/
void F_PowerKey()
{	

	if(DIPS_POWERUP == F_GetDispType())
    {   return ;}
    if(g_u8TestState)
	{	
        g_u8TestState = C_TestOff;
		g_bHalfDispEn = 1;
        F_BuzzerSet(BUZ_SHORT);
        return;
    }
		g_bTurnOn = !g_bTurnOn;
		if(g_bTurnOn)
		{   F_BuzzerSet(BUZ_UP);}
		else
		{   F_BuzzerSet(BUZ_DOWN);}
		g_u8TimeHBlinkTime = 0;
		F_DispTskChange(DIPS_NORMAL);
}
/**************************************************
*函数名称：void  F_LockLongKey(void) 
*函数功能：童锁控制
*入口参数：void
*出口参数：void  
**************************************************/
void F_LockKey()
{
	if(F_RetuenKey())
	{
		if((g_u8WorkMode == MODE_SLEEP)&&(!g_bLockEn)&&(g_bTurnOn))
		{
			g_u8WorkMode = MODE_LOW;
			F_BuzzerSet(BUZ_SHORT);
			return;
		}	
	}
}
/**************************************************
*函数名称：void  F_LockLongKey(void) 
*函数功能：长按童锁控制
*入口参数：void
*出口参数：void  
**************************************************/
void F_LockLongKey()
{	
    if(g_u8TestState)
	{	
        F_BuzzerSet(BUZ_SHORT);
        return;
    }
		
    if((DIPS_POWERUP == F_GetDispType()) || g_u8TestState)
    {   return;}
	
	if(g_bQDErr || g_bDoorOpenLeft || g_bDoorOpenRight)
	{
		return;
	}
    
    if(g_bTurnOn)
    {
        g_bLockEn = !g_bLockEn;
        F_BuzzerSet(BUZ_SHORT);
		F_TriggerFilterVoiceOnce();
    }
}

/**************************************************
*函数名称：void  F_FilterKey(void) 
*函数功能：滤网按键控制
*入口参数：void
*出口参数：void  
**************************************************/
void F_FilterKey()
{
	F_RetuenKey();
	if((g_u8WorkMode == MODE_SLEEP)&&(!g_bLockEn)&&(g_bTurnOn))
	{
		g_u8WorkMode = MODE_LOW;
		F_BuzzerSet(BUZ_SHORT);
		return;
	}	
}
/**************************************************
*函数名称：void  F_FilterLongKey(void) 
*函数功能：长按滤网键3S
*入口参数：void
*出口参数：void  
**************************************************/
void F_FilterLongKey()
{
    if(g_bScreenInitFlg)
    {
        if(F_RetuenLongKey())
        {   
            if(g_bTurnOn)
            {
                g_bScreenInitFlg = 0;
                g_u8RunPercent = 0;
                g_u8FhFilterPer = 100;
                g_u16ScreenRunTime = 0;
                F_BuzzerSet(BUZ_SHORT_3);
				g_u8VoiceFilterBeepCnt = 0;
            }
        }
    }
}
/**************************************************
*函数名称：void  F_FilterLong20sKey(void) 
*函数功能：长按滤网键20S
*入口参数：void
*出口参数：void  
**************************************************/
void F_FilterLong20sKey()
{	
    if(F_RetuenLongKey())
    {			
        if(g_bTurnOn)
        {
            g_bScreenInitFlg = 0;
            g_u8RunPercent = 0;
            g_u8FhFilterPer = 100;
            g_u16ScreenRunTime = 0;
            F_BuzzerSet(BUZ_SHORT_3);
			g_u8VoiceFilterBeepCnt = 0;
        }
    }
}

/**************************************************
*函数名称：void  F_ModeKey(void) 
*函数功能：模式控制
*入口参数：void
*出口参数：void  
**************************************************/
void F_ModeKey()
{	
    if(F_RetuenKey())
    {   
        if(g_bTurnOn)
        {
			if(++g_u8WorkMode > MODE_AI)
            {   g_u8WorkMode = MODE_SLEEP;}
            F_BuzzerSet(BUZ_SHORT);
        }
    }
}
/**************************************************
*函数名称：void  F_TimeType(U8 time) 
*函数功能：定时切换控制
*入口参数：time
*出口参数：void  
**************************************************/
void F_TimeType(S8 time)
{
    if(time > TIME_MAX)
    {   time = 0;}
    if(time < 0)
    {   time = TIME_MAX;}
    g_u8TimeData = time;
    g_u16TimeCountDown = 0;
}
/**************************************************
*函数名称：void  F_TimeKey(void) 
*函数功能：定时控制
*入口参数：void
*出口参数：void  
**************************************************/
void F_TimeKey()
{
    if(F_RetuenKey())
    {
		if(g_bTurnOn)
		{
			if((g_u8WorkMode == MODE_SLEEP)&&(!g_u8TimeHBlinkTime))				//睡眠模式且已关闭指示灯
			{
				g_u8TimeHBlinkTime = C_TIMEH_BLINK_TIME;
				F_BuzzerSet(BUZ_SHORT);
				return;
			}
			else if((g_u8WorkMode == MODE_SLEEP)&&(!g_u8TimeHBlinkTime))	//睡眠模式且正在闪烁
			{
				g_u8TimeHBlinkTime = C_TIMEH_BLINK_TIME;
				F_TimeType(g_u8TimeData+1);
				F_BuzzerSet(BUZ_SHORT);
				return;	
			}
			else			//普通定时
			{
				g_u8TimeHBlinkTime = C_TIMEH_BLINK_TIME;
				F_TimeType(g_u8TimeData+1);
				F_BuzzerSet(BUZ_SHORT);
			}
		}
		else					//关机定时
		{
			g_u8TimeHBlinkTime = C_TIMEH_BLINK_TIME;
			F_TimeType(g_u8TimeData+1);
			F_BuzzerSet(BUZ_SHORT);
		}
    }	
}
/**************************************************
*函数名称：void  F_TimeLongKey(void) 
*函数功能：定时控制
*入口参数：void
*出口参数：void  
**************************************************/
void F_TimeLongKey()
{
    if(F_RetuenLongKey())
    {   
        if(g_u8TimeData)
        {   
            F_TimeType(0);
			g_u8TimeHBlinkTime = C_TIMEH_BLINK_TIME;
            F_BuzzerSet(BUZ_SHORT);
        }
    }	
}
/**************************************************
*函数名称：void  F_WifiKey(void) 
*函数功能：WIFI配网按键
*入口参数：void
*出口参数：void  
**************************************************/
void F_WifiKey()
{	    
	if(F_RetuenKey())
	{
		if((g_u8WorkMode == MODE_SLEEP)&&(!g_bLockEn)&&(g_bTurnOn))
		{
			g_u8WorkMode = MODE_LOW;
			F_BuzzerSet(BUZ_SHORT);
			return;
		}
	}
}
/**************************************************
*函数名称：void F_WifiLongKey(void) 
*函数功能：WIFI配网按键
*入口参数：void
*出口参数：void  
**************************************************/
void F_WifiLongKey()
{	    
    if(F_RetuenLongKey())
    {
		if((g_u8WorkMode == MODE_SLEEP)&&(!g_bLockEn)&&(g_bTurnOn))
		{
			g_u8WorkMode = MODE_LOW;
			F_BuzzerSet(BUZ_SHORT);
			return;
		}			
        if(g_bTurnOn)
        {
            TuyaWifiTask(WIFI_CMD_RESET);
			g_bWifiWorkStart = 1;
            F_BuzzerSet(BUZ_SHORT);
        }
    }	
}
		
/**************************************************
*函数名称：void  F_IonKey(void) 
*函数功能：等离子控制
*入口参数：void
*出口参数：void  
**************************************************/
void F_IonKey()
{
    if(F_RetuenKey())
    {
		if((g_u8WorkMode == MODE_SLEEP)&&(!g_bLockEn)&&(g_bTurnOn))
		{
				g_u8WorkMode = MODE_LOW;
				F_BuzzerSet(BUZ_SHORT);
				return;
		}
		if(g_bTurnOn)
        {
            if(g_u8WorkMode == MODE_SLEEP)
            {
				g_u8WorkMode = MODE_LOW;
            }
			else if(g_u8WorkMode != MODE_SLEEP)
			{
				g_bDLZEn =!g_bDLZEn;
			}
            F_BuzzerSet(BUZ_SHORT);
        }	
    }	
}


/**************************************************
*函数名称：void  F_RgbChangeKey(void) 
*函数功能：RGB显示切换控制
*入口参数：void
*出口参数：void  
**************************************************/
void F_RgbChangeKey()
{
    if(F_RetuenKey())
    {
		if((g_u8WorkMode == MODE_SLEEP)&&(!g_bLockEn)&&(g_bTurnOn))
		{
			g_u8WorkMode = MODE_LOW;
			F_BuzzerSet(BUZ_SHORT);
			return;
		}	
	   if(g_bTurnOn)
        {					
			g_u8RGBDispMode = C_RGB_DISMODE_MANUAL;
			if((!g_u8RGBManualDispTime)||(++g_u8RGBDispType > C_DISPTYPE_VOC))
			{
				g_u8RGBDispType = C_DISPTYPE_CH2O;
			}
			g_u8RGBManualDispTime = C_RGB_MANUAL_DISP_TIME;
            F_BuzzerSet(BUZ_SHORT);
        }	
    }	
}
		