#include "h\Allinclude.h"

#define C_Time4ms				2
#define C_Time10ms				5
#define C_Time100ms				10		
/*#define C_Time1s				10*/
#define C_Time1s				8000
//#define C_Time1min				60	
//#define C_Time1Hour				60	

unsigned char xdata g_u84msCnt = C_Time4ms;
unsigned char xdata g_u810msCnt = C_Time10ms;
unsigned char xdata g_u8100msCnt = C_Time100ms;
/*unsigned char xdata g_u81sCnt = C_Time1s;*/
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
*函数功能：2ms时间标记
*入口参数：void
*出口参数：void  
**************************************************/
void F_TimeBaseTick_IRQ(void)
{
	g_u8TimeBaseCnt ++;
	if( g_u8TimeBaseCnt >= 16 )
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

		g_u8TimeFlag |= C_TIME_2ms;
		
        g_u84msCnt --;
		if(g_u84msCnt == 0)
		{
			g_u8TimeFlag |= C_TIME_4ms;// 4ms
			g_u84msCnt = C_Time4ms;
		}
        
		g_u810msCnt --;
		if(g_u810msCnt == 0)
		{
			g_u8TimeFlag |= C_TIME_10ms;// 10ms
			g_u810msCnt = C_Time10ms;
			
			g_u8100msCnt--;
			if(g_u8100msCnt == 0)
			{
				g_u8TimeFlag |= C_TIME_100ms;// 100ms	
				g_u8100msCnt = C_Time100ms;
				/*g_u81sCnt--;
				if(g_u81sCnt == 0)				
				{
					g_u8TimeFlag |= C_TIME_1s;//	1 s		
					g_u81sCnt = C_Time1s;
				}*/
			}
		}
	}
    if(g_b1sTimeBase)
    {
        g_b1sTimeBase = 0;
        g_u8TimeFlag |= C_TIME_1s;
        if(g_u8MCURunTime < 0xff)
        {   g_u8MCURunTime++;}
    }
}