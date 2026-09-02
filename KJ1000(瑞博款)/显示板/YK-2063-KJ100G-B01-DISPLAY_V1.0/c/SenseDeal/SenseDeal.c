#include "h\AllInclude.h"

unsigned char g_u8SensorData = 0;

StrByte SENSE = {0};
bit g_bWaterBox = 1;		//水箱在位
bit g_bWaterBoxBak = 1;	//水箱在位

// ==========================================
// 传感器数据位定义 (传输给 MCU_A)
// ==========================================
#define BIT_SENSOR_WATER_SHORTAGE  0x01  // BIT0: 缺水标志
#define BIT_SENSOR_WATER_BOX       0x02  // BIT1: 水箱在位
#define BIT_SENSOR_DOOR_LEFT       0x04  // BIT2: 左门盖打开
#define BIT_SENSOR_DOOR_RIGHT      0x08  // BIT3: 右门盖打开


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
    if(g_bDoorOpenLeftBak != g_bDoorOpenLeft)
    {
        g_bDoorOpenLeft = g_bDoorOpenLeftBak;
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
    if(g_bDoorOpenRightBak != g_bDoorOpenRight)
    {
        g_bDoorOpenRight = g_bDoorOpenRightBak;
    }
}

/**************************************************
*函数名称：void  F_UpdateSensorDataToSend(void) 
*函数功能：将分离的 bit 状态打包进 8 位的发送字节
*调用时间：跟随 1s 事件更新后调用
**************************************************/
void F_UpdateSensorDataToSend(void)
{
    unsigned char l_u8TempSensor = 0;

    // 逐个判断状态并置位
    if(g_bWaterShortage)
    {
        l_u8TempSensor |= BIT_SENSOR_WATER_SHORTAGE;
    }
    
    if(g_bWaterBox)
    {
        l_u8TempSensor |= BIT_SENSOR_WATER_BOX;
    }
    
    if(g_bDoorOpenLeft)
    {
        l_u8TempSensor |= BIT_SENSOR_DOOR_LEFT;
    }
    
    if(g_bDoorOpenRight)
    {
        l_u8TempSensor |= BIT_SENSOR_DOOR_RIGHT;
    }

    // 一次性赋值给全局通信变量，保证数据原子性
    g_u8SensorData = l_u8TempSensor;
}




void F_mAllSenseDeal(void)
{  
	if(F_GetTimeTick(C_TIME_10ms))
	{
		F_WaterShortageScan();
		F_DoorOpenScan();
		F_DoorOpenRightScan();
		F_WaterBoxScan();
    }        
    
	if(F_GetTimeTick(C_TIME_1s))
	{
		F_DoorOpenRightEvent();
		F_WaterShortageEvent();
		F_DoorOpenEvent();
		F_WaterBoxEvent();
		F_UpdateSensorDataToSend();
	}
} 