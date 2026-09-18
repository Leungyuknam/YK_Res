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
#ifdef _FAST_TEST_
	#warning "-----当前定时开机、关机为缩时模式------"
	#define	TIME_SECOND		80		//10ms
#else
	#define	TIME_SECOND		8000
#endif
	if(s_u161sTime >= TIME_SECOND)
	{
		s_u161sTime -= TIME_SECOND;
		if(Sys.Time)
		{
			if(++Sys.u16TimeContDown >= 3600)
			{
				Sys.u16TimeContDown = 0;
				Sys.Time --;
				if(!Sys.Time)
				{

				}
			}
		}
		else
		{	
            Sys.u16TimeContDown = 0;
        }
	}
	#ifdef		_RUN_HEAT_TEST_
		#message " ---取消门开计时一段时间自动回到待机----"
	#else
		#message " ---正常功能----"
	if((COOK_RUN_PAUSE == Sys.MState) && !Sys.PowMessage.DoorState)
	{
		#ifdef _FAST_TEST_
		if (GetTimeTick(TIME_100MS))
		#else
		if (GetTimeTick(TIME_1S))
		#endif
		{
			if(++Sys.RunOpenDoorSec >= COOK_DOOR_OPEN_TIME)
			{
				Sys.RunOpenDoorSec = 0;
				WorkModeNewSet(SET_FST_ON);
			}
		}
	}
	else
		Sys.RunOpenDoorSec = 0;	
	#endif

	if((COOK_RUN_PAUSE == Sys.MState) && !Sys.Time)
	{
	#ifdef _FAST_TEST_
		if (GetTimeTick(TIME_100MS))
	#else
		if (GetTimeTick(TIME_1S))
	#endif
		{
			BELL_NOR();
			if(++Sys.CookComplete_Sec >= COOK_COMPLETE_TIME)
			{
				Sys.CookComplete_Sec = 0;
				WorkModeNewSet(SET_FST_ON);
			}
		}
	}
	else
		Sys.CookComplete_Sec = 0;	


	if(COOK_IDLE == Sys.MState)
	{
		#ifdef _FAST_TEST_
		if (GetTimeTick(TIME_100MS))
		#else
		if (GetTimeTick(TIME_1S))
		#endif
		{
			if(++Sys.IDLE_Sec >= 180)
			{
				Sys.IDLE_Sec = 0;
				WorkModeNewSet(SET_STATE_NONE);
				BELL_NOR();
			}
		}
	}
	else
		Sys.IDLE_Sec = 0;	
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
