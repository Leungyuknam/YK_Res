/*********************************************************
文 件 名 : SystemTime.c
文件说明 : 系统时间计算，各种时间标记建立所在地
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/
//头文件
#include "COMMON\AllInclude.h"

//全局变量声明
static	u8		g_u8BaseTimeFlagBuff = 0;
static  u8		g_u8Sys50msCnt = 0;
static  u8		g_u8Sys100msCnt = 0;
static  u8		g_u8Sys500msCnt = 0;
static  u8		g_u8Sys1sCnt = 0;
static  u8		g_u8Sys3sCnt = 0;
static  u8		g_u8Sys5sCnt = 0;
bit g_bTimeFlag_2ms = 0;

#if def_Flag1ms
static	u8		g_u8Base1msCnt = 0;
#endif

#if def_Flag2ms
static	u8		g_u8Base2msCnt = 0;
#endif

#if def_Flag3ms
static	u8		g_u8Base3msCnt = 0;
#endif

#if def_Flag4ms
static	u8		g_u8Base4msCnt = 0;
#endif

#if def_Flag5ms
static	u8		g_u8Base5msCnt = 0;
#endif

#if def_Flag10ms
static	u8		g_u8Base10msCnt = 0;
#endif

		bit		 g_bFlash1HzFlag = 0;
		bit		 g_bFlash2HzFlag = 0;
		u8		 g_u8SysTimeBuff = 0;
		u8 xdata g_u8RunTime100mS = 0;

/*****************************************************
*函数名称：void F_Delay_us(u8 u8InputDelayTime)
*函数功能：微秒级延时函数
*入口参数：u8InputDelayTime
*出口参数：void
*说明：仅做粗略延时用
*****************************************************/
void F_Delay_Us(u8 u8InputDelayTime)
{
    u8  l_u8UsCnt = 0;
    u8  l_u8Ns125Cnt = 0;
    
    for (l_u8UsCnt = 0; l_u8UsCnt < u8InputDelayTime; l_u8UsCnt++)
    {
        for (l_u8Ns125Cnt = 0; l_u8Ns125Cnt < 200; l_u8Ns125Cnt++)
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

		if (++g_u8Sys100msCnt >= C_SET_100MS)
		{
			//100ms标志位
			g_u8Sys100msCnt = 0;
			_BS(g_u8SysTimeBuff,C_SYS_100MS_);

			//500ms标志位
			if (++g_u8Sys500msCnt >= C_SET_500MS)
			{
				g_u8Sys500msCnt = 0;
				_BS(g_u8SysTimeBuff,C_SYS_500MS_);

				//1Hz标志位
				g_bFlash1HzFlag = ~g_bFlash1HzFlag;
			}

			//1s标志位
			if (++g_u8Sys1sCnt >= C_SET_1S)
			{	
				g_u8Sys1sCnt = 0;
				_BS(g_u8SysTimeBuff,C_SYS_1S_);

				//2Hz标志位
				g_bFlash2HzFlag = ~g_bFlash2HzFlag;
			}
			
			//3s标志位
			if (++g_u8Sys3sCnt >= C_SET_3S)
			{
				g_u8Sys3sCnt = 0;
				_BS(g_u8SysTimeBuff,C_SYS_3S_);
			}

			//5s标志位
			if (++g_u8Sys5sCnt >= C_SET_5S)
			{
				g_u8Sys5sCnt = 0;
				_BS(g_u8SysTimeBuff,C_SYS_5S_);
			}
		}							
	}	
}


/*****************************************************
*函数名称：void F_RunTime_Calc(void)
*函数功能：系统运行时间
*输入参数：无
*输出参数：无
*调用周期：100ms
*说明：
*****************************************************/
void F_RunTime_Calc(void)
{
	if (++g_u8RunTime100mS >= 251)
	{
		g_u8RunTime100mS = 251;
	}
}



/*****************************************************
*函数名称：void F_TIMER0_Interrupt(void)
*函数功能：定时器0中断处理
*说明：	   中断时间125uS
*****************************************************/
void F_TIMER0_Interrupt(void)		interrupt 1				
{
//	TCON &= 0XDF;		//clear Interrupt flag

	static u8 xdata s_u8Time1msCnt = 0;

	TF0 = 0;

//	F_CompError_Test();    单通讯获取报错结果
	F_DRIVE_Buzzer();
	F_AnalogPwm_Drive(&g_sDefrost);

/**********时基处理**********/	

#if def_Flag1ms
	if (++g_u8Base1msCnt >= 8)
	{
		g_u8Base1msCnt = 0;
		_BS(g_u8BaseTimeFlagBuff,C_BASETIME_1MS_);
	}
#endif

#if def_Flag2ms
	if (++g_u8Base2msCnt >= 16)
	{
		g_u8Base2msCnt = 0;
		_BS(g_u8BaseTimeFlagBuff,C_BASETIME_2MS_);
	}
#endif

#if def_Flag3ms
	if (++g_u8Base3msCnt >= 24)
	{
		g_u8Base3msCnt = 0;
		_BS(g_u8BaseTimeFlagBuff,C_BASETIME_3MS_);
	}
#endif

#if def_Flag4ms
	if (++g_u8Base4msCnt >= 32)
	{
		g_u8Base4msCnt = 0;
		_BS(g_u8BaseTimeFlagBuff,C_BASETIME_4MS_);
	}
#endif

#if def_Flag5ms
	if (++g_u8Base5msCnt >= 40)
	{
		g_u8Base5msCnt = 0;
		_BS(g_u8BaseTimeFlagBuff,C_BASETIME_5MS_);
	}
#endif

#if def_Flag10ms
	if (++g_u8Base10msCnt >= 80)
	{
		g_u8Base10msCnt = 0;
		_BS(g_u8BaseTimeFlagBuff,C_BASETIME_10MS_);
	}
#endif

	if (++s_u8Time1msCnt >= 16)
	{
		s_u8Time1msCnt = 0;
		g_bTimeFlag_2ms = 1;
	}
}