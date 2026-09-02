#include "h\Allinclude.h"

#define C_Time4ms				2
#define C_Time10ms				5
#define C_Time100ms				10
#define C_Time500ms				5
#define C_Time1s				8000

unsigned char xdata g_u84msCnt = C_Time4ms;
unsigned char xdata g_u810msCnt = C_Time10ms;
unsigned char xdata g_u8100msCnt = C_Time100ms;
unsigned char xdata g_u8500msCnt = C_Time500ms;
unsigned char xdata g_u8TimeFlagBck = 0;
unsigned char xdata g_u8TimeFlag = 0;
unsigned char g_u8TimeBaseCnt = 0;
bit g_bTimeBase = 0;

unsigned int xdata g_u161sCnt = 0;
bit g_b1sTimeBase = 0;

U8 xdata g_u8MCURunTime = 0;    //上电时间

/**************************************************
*函数名称：unsigned char GetTimeTick(unsigned char temp) 
*函数功能：时间段判断
*入口参数：unsigned char
*出口参数：temp  
**************************************************/
unsigned char F_GetTimeTick(unsigned char temp)
{	
	if(temp & C_TIME_NEXT)
	{
		return (temp & g_u8TimeFlagBck);
	}
	else
	{
		return (temp & g_u8TimeFlag);
	}
}

/**************************************************
*函数名称：void F_TimeBaseTick_IRQ(void) 
*函数功能：10ms时间标记
*入口参数：void
*出口参数：void  
**************************************************/
void F_TimeBaseTick_IRQ(void)
{
	g_u8TimeBaseCnt ++;
	if( g_u8TimeBaseCnt >= 80 )
	{			
		g_u8TimeBaseCnt = 0;
		g_bTimeBase = 1;
	}
    
	g_u161sCnt ++;
	if( g_u161sCnt >= C_Time1s )
	{			
		g_u161sCnt = 0;
		g_b1sTimeBase = 1;
	}
}

/**************************************************
*函数名称：void F_TimeProc(void) 
*函数功能：各阶段时间标记生成
*入口参数：void
*出口参数：void  
**************************************************/
void F_mSystemTimeDeal(void)
{
	g_u8TimeFlagBck = g_u8TimeFlag;
	g_u8TimeFlag = 0;
	if(g_bTimeBase)
	{
		g_bTimeBase = 0;

		g_u8TimeFlag |= C_TIME_10ms;
//		P51 = ~P51;
//		g_u810msCnt --;
//		if(g_u810msCnt == 0)
//		{
//			g_u8TimeFlag |= C_TIME_10ms;// 10ms
//			g_u810msCnt = C_Time10ms;
			
			g_u8100msCnt--;
			if(g_u8100msCnt == 0)
			{
				g_u8TimeFlag |= C_TIME_100ms;// 100ms	
				g_u8100msCnt = C_Time100ms;
                
                g_u8500msCnt--;
                if(g_u8500msCnt == 0)
                {
                    g_u8TimeFlag |= C_TIME_500ms;// 500ms	
                    g_u8500msCnt = C_Time500ms;
                }
			}
//		}
	}
    if(g_b1sTimeBase)
    {
        g_b1sTimeBase = 0;
        g_u8TimeFlag |= C_TIME_1s;
        if(g_u8MCURunTime < 0xff)
        {   g_u8MCURunTime++;}
    }
}
/**************************************************************
//Fuction Name: void F_SetStr(void)
//Desrition: 字符串赋值
//Input: 
//Output:
***************************************************************/
void F_SetStr(unsigned char *AimStr,unsigned char Value,unsigned char Count)
{
	while(Count)
	{
		Count --;
		*AimStr = Value;
		AimStr ++;
	}
}
/**************************************************************
//Fuction Name: void F_CopyStr(void)
//Desrition: 字符串复制
//Input: 
//Output:
***************************************************************/
void F_CopyStr(unsigned char *AimStr,unsigned char *CopyStr,unsigned char Count)
{
	while(Count)
	{
		Count --;
		*AimStr = *CopyStr;
		AimStr ++;
        CopyStr ++;
	}
}