
#include  "h\AllInclude.h"

xdata U8	g_u8BellOnTime = 0,g_u8BellOffTime = 0,g_u8BellTimes = 0;
xdata U8	g_u8SetBellOnTime = 0;
xdata U8    g_u8SetBellOffTime = 0;
/******************************************************************
 函数名称					：	void F_BellControl()
 功能描述					：	蜂鸣器控制函数
 被本函数调用的函数列表		：	NULL
 调用本函数列表				：	main()
 调用本函数的时钟周期		：	20ms
 本函数访问的数据列表		：	r_BellTimes 鸣叫次数,r_BellOffTime 暂停时间,r_BellOnTime 鸣叫时间,
 输入参数					：	NULL
 输出参数					：	NULL
 其他说明					：	NULL
*******************************************************************/
void F_BellControl()
{
	if(F_GetTimeTick(C_TIME_10ms))
	{
		if( g_u8BellOnTime > 0 )			//叫的时间
		{
			g_u8BellOnTime--;
		}																	
		else if( g_u8BellOffTime > 0 )	//停的时间
		{
			g_u8BellOffTime--;
		}
		else if( g_u8BellTimes > 0 )		//次数
		{
			g_u8BellTimes--;
			g_u8BellOnTime = g_u8SetBellOnTime;
			g_u8BellOffTime = g_u8SetBellOffTime;			//间隔时间定1S
		}
		else
		{	//P_BELL = 0;
		}
	}
}
/******************************************************************
 函数名称：						F_BellIrq()
 功能描述：						
 调用本函数的时钟周期：			125us
 输入参数：						
 输出参数：						无
 其他说明：						无
*******************************************************************/
void F_BellIrq()
{
	if(g_u8BellOnTime > 0)
	{
		P_BUZZ = ~P_BUZZ;
	}
	else 
	{
		P_BUZZ = 0;
	}

}

void F_SetBuz(U8 l_u8Times,U8 l_u8SetBellOnTime,U8 l_u8SetBellOffTime)
{
	g_u8BellOnTime = 0;
	g_u8BellOffTime = 0;	
	g_u8BellTimes = l_u8Times;
	g_u8SetBellOnTime = l_u8SetBellOnTime;
	g_u8SetBellOffTime = l_u8SetBellOffTime;
}
