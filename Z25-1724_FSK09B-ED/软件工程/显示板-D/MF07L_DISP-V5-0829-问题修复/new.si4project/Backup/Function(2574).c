#define	_Native_FUN_H
#include "Include\Include.h"

static U16 s_u161sTime = 0;



/**************************************************
*函数名称：void    	 TimeCountFrq(void) 
*函数功能：定时控制
*调用时间：125us
*入口参数：void
*出口参数：void  
**************************************************/
void TimeCountFrq()
{
	if(s_u161sTime < 65535)
		s_u161sTime ++;
	#ifdef _DEBUG_IO_
	#warning "-----------屏蔽功能，调用IO测试用-----------"
	if(s_u161sTime == 8000)
	{
		PIN1 ^= 1;
		PIN2 ^= 1;
	}
	#endif

}


void F_TimeCount()
{


}




/********************************************************************************************************
* @ name   : void F_TimeEvent(void)
* @ msg    : 时间变量处理
* @ param   (*)
* @ return  (*)
 ********************************************************************************************************/
void F_TimeEvent(void)
{
	F_TimeCount();
    if (GetTimeTick(TIME_100MS))
    {
		
    }
    if (GetTimeTick(TIME_1S))
    {
		
    }
}
