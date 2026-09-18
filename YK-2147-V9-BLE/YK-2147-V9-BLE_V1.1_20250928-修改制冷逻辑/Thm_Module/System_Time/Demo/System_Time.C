/*********************************************************
文 件 名 : SystemTime.c
文件说明 : 系统时间计算，各时间标记建立所在地
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/
//头文件
#include "ALLInclude.h"

//全局变量声明
static  u8      g_u8BaseTimeCnt = 0;


static	u8		g_u8BaseTimeFlagBuff = 0;
static  u8		g_u8Sys50msCnt = 0;
static  u8		g_Sys100msCnt = 0;
static  u8		g_Sys500msCnt = 0;
static  u8		g_Sys1sCnt = 0;
static  u8		g_Sys3sCnt = 0;
static  u8		g_Sys5sCnt = 0;
    	
/*****************************************************
*函数名称：void F_Delay_us(u8 l_u8InputDelayTime)
*函数功能：微秒级阻塞延时函数
*入口参数：l_u8InputDelayTime
*出口参数：void
*说明：仅做粗略延时用，需根据不同单片机做调整
*****************************************************/
void F_Delay_Us(u8 l_u8InputDelayTime)
{
    u8  l_u8UsCnt = 0;
    u8  l_u8Ns125Cnt = 0;
    
    for (l_u8UsCnt = 0; l_u8UsCnt < l_u8InputDelayTime; l_u8UsCnt++)
    {
        for (l_u8Ns125Cnt = 0; l_u8Ns125Cnt < 2; l_u8Ns125Cnt++)
        {}
    }
}


/*****************************************************
*函数名称：void F_SYSTEM_Time(void)
*函数功能：系统时间标志处理
*输入参数：无
*输出参数：无
*调用周期：
*说明：
*****************************************************/
void F_SYSTEM_Time(void)
{
	g_u8SysTimeBuff = 0;

	if (_BTS(g_u8BaseTimeFlagBuff,C_BASETIME_10MS_))
	{
		g_u8BaseTimeFlagBuff = 0;

		//10ms标志位
		_BS(g_u8SysTimeBuff,C_SYS_10MS_);

		//50ms标志位
		if (++g_u8Sys50msCnt >= C_SET_50MS)
		{
			g_u8Sys50msCnt = 0;
			_BS(g_u8SysTimeBuff,C_SYS_50MS_);
		}

		if (++g_Sys100msCnt >= C_SET_100MS)
		{
			//100ms标志位
			g_Sys100msCnt = 0;
			_BS(g_u8SysTimeBuff,C_SYS_100MS_);

			//500ms标志位
			if (++g_Sys500msCnt >= C_SET_500MS)
			{
				g_Sys500msCnt = 0;
				_BS(g_u8SysTimeBuff,C_SYS_500MS_);

				//1Hz标志位
				g_bFlash1HzFlag = ~g_bFlash1HzFlag;
			}

			//1s标志位
			if (++g_Sys1sCnt >= C_SET_1S)
			{	
				g_Sys1sCnt = 0;
				_BS(g_u8SysTimeBuff,C_SYS_1S_);

				//2Hz标志位
				g_bFlash2HzFlag = ~g_bFlash2HzFlag;
			}
			
			//3s标志位
			if (++g_Sys3sCnt >= C_SET_3S)
			{
				g_Sys3sCnt = 0;
				_BS(g_u8SysTimeBuff,C_SYS_3S_);
			}

			//5s标志位
			if (++g_Sys5sCnt >= C_SET_5S)
			{
				g_Sys5sCnt = 0;
				_BS(g_u8SysTimeBuff,C_SYS_5S_);
			}
		}							
	}	
}

/*****************************************************
*函数名称：void F_INT0_Interrupt(void)
*函数功能：定时器0中断处理
*说明：	   中断时间125uS
*****************************************************/
void F_TIMER0_Interrupt(void)	interrupt 1				
{

	TCON &= 0XDF;		//clear Interrupt flag

/**********系统时基**********/	

	g_u16SystemClick++;
	
}


/*****************************************************
*函数名称：u8 F_CLOG_FREE_DELAY(u16 l_u16DelayMs, DELAY_T l_sTypeIndex)
*函数功能：无阻塞式延时函数
*输入参数：l_u16DelayMs（需延时时间，范围：1-8000ms）
		   l_sTypeIndex（延时类型）
*输出参数：0(延时未完成)
		   1(延时完成)
*调用周期：无
*说明：
*****************************************************/
u8 F_CLOG_FREE_DELAY(u16 l_u16DelayMs, DELAY_T l_sTypeIndex)
{

	if (l_sTypeIndex.u8DelayState == C_DELAY_DONE)
	{
		l_sTypeIndex.u8DelayState = C_DELAY_ING;
		l_sTypeIndex.u16LastSystemClick = g_u16SystemClick;
		l_sTypeIndex.u16CompareValue = ((l_u16DelayMs*1000)/125);	
	}
	else
	{
		if ((g_u16SystemClick - l_sTypeIndex.u16LastSystemClick) >= l_sTypeIndex.u16CompareValue)
		{
			l_sTypeIndex.u8DelayState = C_DELAY_DONE;
			return 1;		
		}
	}
	return 0;
		
}


/*****************************************************
*函数名称：void F_CLOG_DELAY(u16 l_u16DelayMs)
*函数功能：阻塞式延时函数
*输入参数：l_u16DelayMs（需延时时间，范围：1-8000ms）
*输出参数：0(延时未完成)
		   1(延时完成)
*调用周期：无
*说明：
*****************************************************/
void F_CLOG_DELAY(u16 l_u16DelayMs)
{
	u16  s_u16lastSystemClick = g_u16SystemClick;
	u16  s_u16CompareValue = ((l_u16DelayMs*1000)/125);

	while ((g_u16SystemClick - s_u16lastSystemClick) < s_u16CompareValue);		
}

/*****************************************************
*函数名称：void F_SYSTEM_Time(void)
*函数功能：系统时间标志处理
*输入参数：无
*输出参数：无
*调用周期：
*说明：
*****************************************************/
void F_SYSTEM_Time(void)
{

#if def_Flag1ms
	

#endif


}