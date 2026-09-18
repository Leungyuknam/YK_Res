#include "Include\Include.h"
 U8 g_u8RstPosBak = 0;	

/**************************************************
*函数名称：void  F_WaterShortageScan(void) 
*函数功能：缺水扫描
*调用时间：10ms
*入口参数：void
*出口参数：void  
**************************************************/
void F_RstPosScan()
{
    static U8 s_u8Filter1 = 0;
    
    if(P_HALL_OUT == g_u8RstPosBak)
    {
        if(++s_u8Filter1 >= 20)
        {
            s_u8Filter1 = 0;
            g_u8RstPosBak = !P_HALL_OUT;
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
void F_RstPosEvent()
{
    if(g_u8RstPosBak != Out.LRHallState)
    {
        Out.LRHallState = g_u8RstPosBak;
    }
}



void F_mAllSenseDeal(void)
{  
	if(GetTimeTick(TIME_10MS))
	{
		F_RstPosScan();
	}       
	if(GetTimeTick(TIME_100MS))
	{
		F_RstPosEvent();
	}
} 
