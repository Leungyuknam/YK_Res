#include "h\AllInclude.h"

S8 xdata g_s8NowTemp = 24;//当前温度

U8 xdata g_u8NowHumi = 60;//当前湿度

U8 xdata g_u8PM25AirQuality = 0;//PM25空气质量
U8 xdata g_u8VocAirQuality = 0; //VOC空气质量
U8 xdata g_u8CH2OAirQuality = 0;//甲醛空气质量

U16 g_u16ScreenRunTime = 0;//滤网使用时间 单位0.1小时
U8 g_u8RunPercent = 0;//滤网使用百分比
U8 g_u8FhFilterPer = 100;//滤网剩余百分比
bit g_bScreenInitFlg = 0;//滤网复位标记

StrByte SENSE = {0};
bit g_bWaterBox = 1;		//水箱在位
bit g_bWaterBoxBak = 1;	//水箱在位

U8 code t_ScreenRunData[] = {5,7,9,9,10,15,20,25,30};//不同PM25，一小时换算滤网使用时间，扩大10倍

U8 code t_ScreenRunDataNew[] = {5,6,8,10};//不同PM25和档位，一小时换算滤网使用时间，扩大10倍
//  U8 code t_ScreenRunDataNew[] = {100,200,300,400};//不同PM25和档位，一小时换算滤网使用时间，扩大10倍
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
										// --- 风速对应小时 ---
										switch (g_u8FanSpeed)
										{
												case FAN_SLEEP: l_u8FanSpdIndex = 0; break;   // 0.5h
												case FAN_LOW:   l_u8FanSpdIndex = 1; break;   // 0.6h
												case FAN_MID:   l_u8FanSpdIndex = 2; break;   // 0.8h
												case FAN_HI:    l_u8FanSpdIndex = 3; break;  // 1.0h
												default:        l_u8FanSpdIndex = 0; break;
										}
										// --- PM2.5 对应小时 ---
										if (g_u16DispPm25Data < 50)        l_u8Pm25Index = 0;
										else if (g_u16DispPm25Data < 100)  l_u8Pm25Index = 1;
										else if (g_u16DispPm25Data < 150)  l_u8Pm25Index = 2;
										else                               l_u8Pm25Index = 3;
										
										g_u16ScreenRunTime += (t_ScreenRunDataNew[l_u8FanSpdIndex] + t_ScreenRunDataNew[l_u8Pm25Index]);	
										
//                    if(!g_u16DispPm25Data)
//                    {   g_u16ScreenRunTime += t_ScreenRunData[0];}
//                    else if(g_u16DispPm25Data > 450)
//                    {   g_u16ScreenRunTime += t_ScreenRunData[8];}
//                    else
//                    {   g_u16ScreenRunTime += t_ScreenRunData[(g_u16DispPm25Data-1) / 50];}
                }
            }
            else
            {
                if(++s_u16ScreenTime >= c_Time_1h)
                {
                    s_u16ScreenTime = 0;
										// --- 风速对应小时 ---
										switch (g_u8FanSpeed)
										{
												case FAN_SLEEP: l_u8FanSpdIndex = 0; break;   // 0.5h
												case FAN_LOW:   l_u8FanSpdIndex = 1; break;   // 0.6h
												case FAN_MID:   l_u8FanSpdIndex = 2; break;   // 0.8h
												case FAN_HI:    l_u8FanSpdIndex = 3; break;  // 1.0h
												default:        l_u8FanSpdIndex = 0; break;
										}
										// --- PM2.5 对应小时 ---
										if (g_u16DispPm25Data < 50)        l_u8Pm25Index = 0;
										else if (g_u16DispPm25Data < 100)  l_u8Pm25Index = 1;
										else if (g_u16DispPm25Data < 150)  l_u8Pm25Index = 2;
										else                               l_u8Pm25Index = 3;
										
										g_u16ScreenRunTime += (t_ScreenRunDataNew[l_u8FanSpdIndex] + t_ScreenRunDataNew[l_u8Pm25Index]);	
										
//                    if(!g_u16DispPm25Data)
//                    {   g_u16ScreenRunTime += t_ScreenRunData[0];}
//                    else if(g_u16DispPm25Data > 450)
//                    {   g_u16ScreenRunTime += t_ScreenRunData[8];}
//                    else
//                    {   g_u16ScreenRunTime += t_ScreenRunData[(g_u16DispPm25Data-1) / 50];}
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
//void F_SenseEvent()
//{
//    static U8 s_u8TempCnt = 0;
//    static U8 s_u8HumiCnt = 0;
//        
//	if(g_u8MCURunTime <= 5)
//	{
//        g_s8NowTemp = g_s8TempData;
//		g_u8NowHumi = g_u8HumiData;
//	}
//	else
//	{        
//        if(g_s8NowTemp != g_s8TempData)
//        {
//            if(++s_u8TempCnt >= 5)
//            {
//                s_u8TempCnt = 0;
//                g_s8NowTemp = g_s8TempData;
//            }
//        }
//        else
//        {   s_u8TempCnt = 0;}
//        
//        if(g_u8NowHumi != g_u8HumiData)
//        {
//            if(++s_u8HumiCnt >= 5)
//            {
//                s_u8HumiCnt = 0;
//                g_u8NowHumi = g_u8HumiData;
//            }
//        }
//        else
//        {   s_u8HumiCnt = 0;}
//	}
//}
/**************************************************
*函数名称：void  F_PM25Event(void) 
*函数功能：PM25传感器事件
*调用时间：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_PM25Event()
{    
    if(c_AIR_QUALITY_ACTOR == g_u8PM25AirQuality)
    {
        if(g_u16DispPm25Data > c_PM25_GOOD)
        {   g_u8PM25AirQuality = c_AIR_QUALITY_GOOD;}
    }
    else if(c_AIR_QUALITY_GOOD == g_u8PM25AirQuality)
    {
        if(g_u16DispPm25Data < c_PM25_GOOD)
        {   g_u8PM25AirQuality = c_AIR_QUALITY_ACTOR;}
        else if(g_u16DispPm25Data > c_PM25_MID)
        {   g_u8PM25AirQuality = c_AIR_QUALITY_MID;}
    }
    else if(c_AIR_QUALITY_MID == g_u8PM25AirQuality)
    {
        if(g_u16DispPm25Data < c_PM25_MID)
        {   g_u8PM25AirQuality = c_AIR_QUALITY_GOOD;}
        else if(g_u16DispPm25Data > c_PM25_BAD)
        {   g_u8PM25AirQuality = c_AIR_QUALITY_BAD;}
    }
    else
    {
        if(g_u16DispPm25Data < c_PM25_BAD)
        {   g_u8PM25AirQuality = c_AIR_QUALITY_MID;}
    }
}

/**************************************************
*函数名称：void  F_CH2OEvent(void) 
*函数功能：甲醛传感器事件
*调用时间：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_CH2OEvent()
{
    if(c_AIR_QUALITY_ACTOR == g_u8CH2OAirQuality)
    {
        if(g_u16CH2OData > c_CH2O_GOOD)
        {   g_u8CH2OAirQuality = c_AIR_QUALITY_GOOD;}
    }
    else if(c_AIR_QUALITY_GOOD == g_u8CH2OAirQuality)
    {
        if(g_u16CH2OData < c_CH2O_GOOD)
        {   g_u8CH2OAirQuality = c_AIR_QUALITY_ACTOR;}
        else if(g_u16CH2OData > c_CH2O_MID)
        {   g_u8CH2OAirQuality = c_AIR_QUALITY_MID;}
    }
    else if(c_AIR_QUALITY_MID == g_u8CH2OAirQuality)
    {
        if(g_u16CH2OData < c_CH2O_MID)
        {   g_u8CH2OAirQuality = c_AIR_QUALITY_GOOD;}
        else if(g_u16CH2OData > c_CH2O_BAD)
        {   g_u8CH2OAirQuality = c_AIR_QUALITY_BAD;}
    }
    else
    {
        if(g_u8CH2OAirQuality < c_CH2O_BAD)
        {   g_u8CH2OAirQuality = c_AIR_QUALITY_MID;}
    }		
}

/**************************************************
*函数名称：void  F_VOCEvent(void) 
*函数功能：VOC传感器事件
*调用时间：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_VOCEvent()
{
    static U16 s_u16UpDelay = 0;
    static U8 s_u8VocAirQuality = 0;
    static U8 s_u8VocAirQualityBak = 0;
    static U16 s_u16VocDataBak = 0;    
    
    if(c_AIR_QUALITY_ACTOR == s_u8VocAirQuality)
    {
        if(g_u16DispVocData >= c_VOC_GOOD)
        {   s_u8VocAirQuality = c_AIR_QUALITY_GOOD;}
    }
    else if(c_AIR_QUALITY_GOOD == s_u8VocAirQuality)
    {
        if(g_u16DispVocData <= (c_VOC_GOOD - c_VOC_BIAS))
        {   s_u8VocAirQuality = c_AIR_QUALITY_ACTOR;}
        else if(g_u16DispVocData >= c_VOC_MID)
        {   s_u8VocAirQuality = c_AIR_QUALITY_MID;}
    }
    else if(c_AIR_QUALITY_MID == s_u8VocAirQuality)
    {
        if(g_u16DispVocData <= (c_VOC_MID - c_VOC_BIAS))
        {   s_u8VocAirQuality = c_AIR_QUALITY_GOOD;}
        else if(g_u16DispVocData >= c_VOC_BAD)
        {   s_u8VocAirQuality = c_AIR_QUALITY_BAD;}
    }
    else
    {
        if(g_u16DispVocData <= (c_VOC_BAD - c_VOC_BIAS))
        {   s_u8VocAirQuality = c_AIR_QUALITY_MID;}
    }
    
    if(s_u8VocAirQualityBak != s_u8VocAirQuality)
    {   
        s_u8VocAirQualityBak = s_u8VocAirQuality;
        g_u8VocAirQuality = s_u8VocAirQuality;
        s_u16UpDelay = 0;
    }
    
    
    if(g_u8VocAirQuality != s_u8VocAirQuality)
    {
        if(s_u16VocDataBak > g_u16DispVocData)
        {
            if((s_u16VocDataBak - g_u16DispVocData) >= 15)
            {
                g_u8VocAirQuality = s_u8VocAirQuality;
                s_u16UpDelay = 0;
            }
        }
        else if(s_u16VocDataBak < g_u16DispVocData)
        {
            if((g_u16DispVocData - s_u16VocDataBak) >= 15)
            {
                g_u8VocAirQuality = s_u8VocAirQuality;
                s_u16UpDelay = 0;
            }
        }
    }
		
		 if(g_u8VocAirQuality >= c_AIR_QUALITY_MID)
    {
        if(s_u16UpDelay < 0xffff)s_u16UpDelay++;
				if(s_u16UpDelay >= 3600)
				{   
						//预留重新预热接口
						s_u16UpDelay = 0;
				}
    }
		else
		{
			s_u16UpDelay = 0;
		}
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
*函数名称：void  F_WaterShortageScan(void) 
*函数功能：缺水扫描
*调用时间：10ms
*入口参数：void
*出口参数：void  
**************************************************/
void F_WaterShortageScan()
{
    static U8 s_u8Filter1 = 0;
    
    if(P_WATER_SHORTAGE == g_bWaterShortageBak)
    {
        if(++s_u8Filter1 >= 30)
        {
            s_u8Filter1 = 0;
            g_bWaterShortageBak = !P_WATER_SHORTAGE;
        }
    }
    else
    {   s_u8Filter1 = 0;}
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
*函数名称：void  F_WaterBoxScan(void) 
*函数功能：水箱在位扫描
*调用时间：10ms
*入口参数：void
*出口参数：void  
**************************************************/
void F_WaterBoxScan()
{
    static U8 s_u8Filter1 = 0;
    
    if(P_WATER_BOX == g_bWaterBoxBak)
    {
        if(++s_u8Filter1 >= 30)
        {
            s_u8Filter1 = 0;
            g_bWaterBoxBak = !P_WATER_BOX;
        }
    }
    else
    {   s_u8Filter1 = 0;}
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
*函数名称：void  F_DoorOpenScan(void) 
*函数功能：门盖扫描
*调用时间：10ms
*入口参数：void
*出口参数：void  
**************************************************/
void F_DoorOpenScan()
{
    static U8 s_u8Filter1 = 0;
    
    if(!P_DOOR_LEFT == g_bDoorOpenLeftBak)
    {
        if(++s_u8Filter1 >= 30)
        {
            s_u8Filter1 = 0;
            g_bDoorOpenLeftBak = P_DOOR_LEFT;
        }
    }
    else
    {   s_u8Filter1 = 0;}
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
*函数名称：void  F_DoorOpenScan(void) 
*函数功能：门盖扫描
*调用时间：10ms
*入口参数：void
*出口参数：void  
**************************************************/
void F_DoorOpenRightScan()
{
    static U8 s_u8Filter1 = 0;
    
    if(!P_DOOR_RIGHT == g_bDoorOpenRightBak)
    {
        if(++s_u8Filter1 >= 30)
        {
            s_u8Filter1 = 0;
            g_bDoorOpenRightBak = P_DOOR_RIGHT;
        }
    }
    else
    {   s_u8Filter1 = 0;}
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
		else if(!g_bDoorOpenLeft)
		{
				g_u8Cnt = 10;
		}
}


/**************************************************
*函数名称：void  F_UnpackScan(void) 
*函数功能：拆机保护微动开关扫描
*调用时间：10ms
*入口参数：void
*出口参数：void  
**************************************************/
//void F_UnpackScan()
//{
//    static U8 s_u8Filter1 = 0;
//    
//    if(P_SWITCH != g_bUnpackErr)
//    {
//        if(++s_u8Filter1 >= 50)
//        {
//            s_u8Filter1 = 0;
//            g_bUnpackErr = P_SWITCH;
//        }
//    }
//    else
//    {   s_u8Filter1 = 0;}
//}

void F_mAllSenseDeal(void)
{  
	if(F_GetTimeTick(C_TIME_10ms))
	{
        F_QDScan();
		F_WaterShortageScan();
		F_DoorOpenScan();
		F_DoorOpenRightScan();
		F_WaterBoxScan();
//        F_UnpackScan();
    }        
    
	if(F_GetTimeTick(C_TIME_1s))
	{
        if(g_bSenseLoad)
        {
            F_QueryVocData();
			F_QueryCH2OData();
            F_PM25Event();
            F_VOCEvent();
			F_CH2OEvent();
        }
		F_DoorOpenRightEvent();
//        F_SenseEvent();
        F_ScreenLifeEvent();
        F_QDEvent();
		F_WaterShortageEvent();
		F_DoorOpenEvent();
		F_WaterBoxEvent();
	}
} 