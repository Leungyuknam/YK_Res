#include "..\Include\Include.h"
 bit g_bRstPos = 0;		//到达复位位置
 bit g_bRstPosBak = 0;	

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
    
    if(P_HALL == g_bRstPosBak)
    {
        if(++s_u8Filter1 >= 20)
        {
            s_u8Filter1 = 0;
            g_bRstPosBak = !P_HALL;
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
    if(g_bRstPosBak != g_bRstPos)
    {
        g_bRstPos = g_bRstPosBak;
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
		if(g_bRstPos)
		{

		}

	}
} 
