#include "h\Allinclude.h"

/* ========================================== */
/* 传感器数据位掩码 (MCU_A 解析 MCU_B 传回)   */
/* ========================================== */
#define BIT_SENSOR_WATER_SHORTAGE   0x01  
#define BIT_SENSOR_WATER_BOX        0x02  
#define BIT_SENSOR_DOOR_LEFT        0x04  
#define BIT_SENSOR_DOOR_RIGHT       0x08  

/* ========================================== */
/* 变量定义                                   */
/* ========================================== */
S8 xdata g_s8NowTemp = 24;
U8 xdata g_u8NowHumi = 60;

U8 xdata g_u8PM25AirQuality = 0;
U8 xdata g_u8VocAirQuality  = 0; 
U8 xdata g_u8CH2OAirQuality = 0;

U16 g_u16ScreenRunTime = 0;
U8  g_u8RunPercent = 0;
U8  g_u8FhFilterPer = 100;
bit g_bScreenInitFlg = 0;

StrByte SENSE = {0};
bit g_bWaterBox    = 1;
bit g_bWaterBoxBak = 1;

U8 code t_ScreenRunData[] = {5,7,9,9,10,15,20,25,30};//不同PM25，一小时换算滤网使用时间，扩大10倍

U8 code t_ScreenRunDataNew[] = {5,6,8,10};//不同PM25和档位，一小时换算滤网使用时间，扩大10倍
/**************************************************
*函数名称：void  F_ScreenLifeEvent(void) 
*函数功能：滤网寿命计算
*调用时间：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_ScreenLifeEvent()
{
    static U16 s_u16ScreenTime = 0;
		U8 l_u8Pm25Index = 0;
		U8 l_u8FanSpdIndex = 0;
    if(g_bTurnOn && g_u8FanSpeed)
    {
        if(g_u16ScreenRunTime < DUST_TOTAL_TIM)
        {
            if(g_bShrinkTimeFlg)
            {
                if(++s_u16ScreenTime >= 1)
                {
                    s_u16ScreenTime = 0;
					if(!g_sPm25.u16DispData)
					{   g_u16ScreenRunTime += t_ScreenRunData[0];}
					else if(g_sPm25.u16DispData > 450)
					{   g_u16ScreenRunTime += t_ScreenRunData[8];}
					else
					{   g_u16ScreenRunTime += t_ScreenRunData[(g_sPm25.u16DispData-1) / 50];}		
                }
            }
            else
            {
                if(++s_u16ScreenTime >= c_Time_1h)
                {
                    s_u16ScreenTime = 0;
					if(!g_sPm25.u16DispData)
					{   g_u16ScreenRunTime += t_ScreenRunData[0];}
					else if(g_sPm25.u16DispData > 450)
					{   g_u16ScreenRunTime += t_ScreenRunData[8];}
					else
					{   g_u16ScreenRunTime += t_ScreenRunData[(g_sPm25.u16DispData-1) / 50];}	
                }
            }
        }
    }
    else
    {   s_u16ScreenTime = 0;}
    
    if(g_u16ScreenRunTime >= DUST_TOTAL_TIM)
    {   g_u8RunPercent = 100;}
    else
    {
        g_u8RunPercent = (U32)g_u16ScreenRunTime * 100 / DUST_TOTAL_TIM;
    }
    g_u8FhFilterPer = 100 - g_u8RunPercent;
    
    if(g_u8FhFilterPer <= 2)
    {   g_bScreenInitFlg = 1;}
}


/**************************************************
*函数名称：void  F_SenseEvent(void) 
*函数功能：传感器事件
*调用时间：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_SenseEvent()
{
    static U8 s_u8TempCnt = 0;
    static U8 s_u8HumiCnt = 0;
        
	if(g_u8MCURunTime <= 5)
	{
        g_s8NowTemp = g_s8TempData;
		g_u8NowHumi = g_u8HumiData;
	}
	else
	{        
        if(g_s8NowTemp != g_s8TempData)
        {
            if(++s_u8TempCnt >= 5)
            {
                s_u8TempCnt = 0;
                g_s8NowTemp = g_s8TempData;
            }
        }
        else
        {   s_u8TempCnt = 0;}
        
        if(g_u8NowHumi != g_u8HumiData)
        {
            if(++s_u8HumiCnt >= 5)
            {
                s_u8HumiCnt = 0;
                g_u8NowHumi = g_u8HumiData;
            }
        }
        else
        {   s_u8HumiCnt = 0;}
	}
}
/**************************************************
*函数名称：void F_StiveAirQualityJudge(void) 
*函数功能：粉尘空气质量判定
*调用时间：1s
*入口参数：l_u16DispData:粉尘数据 l_u8StiveAirQuality:空气质量
*出口参数：void  
**************************************************/
void F_StiveAirQualityJudge(U16 l_u16StiveData,U8 *l_u8StiveAirQuality)
{    
    if(AIR_QUALITY_ACTOR == *l_u8StiveAirQuality)
    {
        if(l_u16StiveData > c_STIVE_GOOD)
        {   *l_u8StiveAirQuality = AIR_QUALITY_GOOD;}
    }
    else if(AIR_QUALITY_GOOD == *l_u8StiveAirQuality)
    {
        if(l_u16StiveData < c_STIVE_GOOD)
        {   *l_u8StiveAirQuality = AIR_QUALITY_ACTOR;}
        else if(l_u16StiveData > c_STIVE_MID)
        {   *l_u8StiveAirQuality = AIR_QUALITY_MID;}
    }
    else if(AIR_QUALITY_MID == *l_u8StiveAirQuality)
    {
        if(l_u16StiveData < c_STIVE_MID)
        {   *l_u8StiveAirQuality = AIR_QUALITY_GOOD;}
        else if(l_u16StiveData > c_STIVE_BAD)
        {   *l_u8StiveAirQuality = AIR_QUALITY_BAD;}
    }
    else
    {
        if(l_u16StiveData < c_STIVE_BAD)
        {   *l_u8StiveAirQuality = AIR_QUALITY_MID;}
    }
}
/**************************************************
*函数名称：void F_StiveAirQualityEvent(void) 
*函数功能：粉尘空气质量事件
*调用时间：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_StiveAirQualityEvent()
{    
    F_StiveAirQualityJudge(g_sPm25.u16DispData,&g_u8PM25AirQuality);
//    F_StiveAirQualityJudge(g_sPm10.u16DispData,&g_u8PM10AirQuality);
//    F_StiveAirQualityJudge(g_sPm1.u16DispData,&g_u8PM1AirQuality);    
}


/*****************************************************************************
*函数名称：void F_VOCAirQualityEvent(void) 
*函数功能：VOC空气质量事件 (纯净版：严格按分界值实时评定等级)
*调用时间：1s
******************************************************************************/
void F_VOCAirQualityEvent()
{
    if(g_u16DispVocData > c_VOC_BAD)
    {
        g_u8VocAirQuality = AIR_QUALITY_BAD;
    }
    else if(g_u16DispVocData > c_VOC_MID)
    {
        g_u8VocAirQuality = AIR_QUALITY_MID;
    }
    else if(g_u16DispVocData > c_VOC_GOOD)
    {
        g_u8VocAirQuality = AIR_QUALITY_GOOD;
    }
    else
    {
        g_u8VocAirQuality = AIR_QUALITY_ACTOR;
    }
}


/**************************************************
*函数名称：void F_CH2OAirQualityEvent(void) 
*函数功能：甲醛空气质量事件
*调用时间：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_CH2OAirQualityEvent()
{    
    if(AIR_QUALITY_ACTOR == g_u8CH2OAirQuality)
    {
        if(g_u16DispCH2OData > c_CH2O_GOOD)
        {   g_u8CH2OAirQuality = AIR_QUALITY_GOOD;}
    }
    else if(AIR_QUALITY_GOOD == g_u8CH2OAirQuality)
    {
        if(g_u16DispCH2OData < c_CH2O_GOOD)
        {   g_u8CH2OAirQuality = AIR_QUALITY_ACTOR;}
        else if(g_u16DispCH2OData > c_CH2O_MID)
        {   g_u8CH2OAirQuality = AIR_QUALITY_MID;}
    }
    else if(AIR_QUALITY_MID == g_u8CH2OAirQuality)
    {
        if(g_u16DispCH2OData < c_CH2O_MID)
        {   g_u8CH2OAirQuality = AIR_QUALITY_GOOD;}
        else if(g_u16DispCH2OData > c_CH2O_BAD)
        {   g_u8CH2OAirQuality = AIR_QUALITY_BAD;}
    }
    else
    {
        if(g_u16DispCH2OData < c_CH2O_BAD)
        {   g_u8CH2OAirQuality = AIR_QUALITY_MID;}
    }
}


/**************************************************
*函数名称：void  F_UnpackSensorData(void) 
*函数功能：解析 MCU_B 发来的传感器字节（不含倾倒）
*调用时间：10ms
**************************************************/
void F_UnpackSensorData(void)
{
    g_bWaterShortageBak = (g_u8SensorData & BIT_SENSOR_WATER_SHORTAGE) ? 1 : 0;
    g_bWaterBoxBak      = (g_u8SensorData & BIT_SENSOR_WATER_BOX)      ? 1 : 0;
    g_bDoorOpenLeftBak  = (g_u8SensorData & BIT_SENSOR_DOOR_LEFT)      ? 1 : 0;
    g_bDoorOpenRightBak = (g_u8SensorData & BIT_SENSOR_DOOR_RIGHT)     ? 1 : 0;
}



/**************************************************
*函数名称：void  F_QDScan(void) 
*函数功能：倾倒扫描
*调用时间：10ms
*入口参数：void
*出口参数：void  
**************************************************/
void F_QDScan()
{
    static U8 s_u8Filter1 = 0;
    
    if(P_QD == g_bQDErrBak)
    {
        if(++s_u8Filter1 >= 20)
        {
            s_u8Filter1 = 0;
            g_bQDErrBak = !P_QD;
        }
    }
    else
    {   s_u8Filter1 = 0;}
}
/**************************************************
*函数名称：void  F_QDEvent(void) 
*函数功能：倾倒事件
*调用时间：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_QDEvent()
{
	static U8 g_u8Cnt = 10;
    if(g_bQDErrBak != g_bQDErr)
    {
        g_bQDErr = g_bQDErrBak;
    }
		if(g_bQDErr && (g_u8Cnt))
    {   
		F_BuzzerSet(BUZ_LOCK);
		g_u8Cnt--;
	}
	else if(!g_bQDErr)
	{
		g_u8Cnt = 10;
	}
}



/**************************************************
*函数名称：void  F_QDEvent(void) 
*函数功能：缺水事件
*调用时间：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_WaterShortageEvent()
{
    if(g_bWaterShortageBak != g_bWaterShortage)
    {
        g_bWaterShortage = g_bWaterShortageBak;
    }
}


/**************************************************
*函数名称：void  F_QDEvent(void) 
*函数功能：水箱在位事件
*调用时间：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_WaterBoxEvent()
{
    if(g_bWaterBoxBak != g_bWaterBox)
    {
        g_bWaterBox = g_bWaterBoxBak;
    }
}

/**************************************************
*函数名称：void  F_DoorOpenEvent(void) 
*函数功能：门盖事件
*调用时间：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_DoorOpenEvent()
{
	static U8 g_u8Cnt = 10;
    if(g_bDoorOpenLeftBak != g_bDoorOpenLeft)
    {
        g_bDoorOpenLeft = g_bDoorOpenLeftBak;
    }
		if(g_bDoorOpenLeft && (g_u8Cnt))
    {   
		F_BuzzerSet(BUZ_LOCK);
		g_u8Cnt--;
	}
	else if(!g_bDoorOpenLeft)
	{
		g_u8Cnt = 10;
	}
}


/**************************************************
*函数名称：void  F_DoorOpenEvent(void) 
*函数功能：门盖事件
*调用时间：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_DoorOpenRightEvent()
{
	static U8 g_u8Cnt = 10;
    if(g_bDoorOpenRightBak != g_bDoorOpenRight)
    {
        g_bDoorOpenRight = g_bDoorOpenRightBak;
    }
		if(g_bDoorOpenRight && (g_u8Cnt))
    {   
		F_BuzzerSet(BUZ_LOCK);
		g_u8Cnt--;
	}
	else if(!g_bDoorOpenRight)
	{
		g_u8Cnt = 10;
	}
}


void F_mAllSenseDeal(void)
{  
	if(F_GetTimeTick(C_TIME_10ms))
	{
        F_QDScan();
		F_UnpackSensorData();
    }        
    
	if(F_GetTimeTick(C_TIME_1s))
	{
        if(g_bSenseLoad)
        {
            F_QueryVocData();
            F_VOCAirQualityEvent();
            F_StiveAirQualityEvent();
        }
		F_QueryCH2OData();
        F_CH2OAirQualityEvent();
		F_SenseEvent();
		
		F_DoorOpenRightEvent();
        F_ScreenLifeEvent();
        F_QDEvent();
		F_WaterShortageEvent();
		F_DoorOpenEvent();
		F_WaterBoxEvent();
	}
} 