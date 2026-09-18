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

/********************************************************************************************************
* @ name   : void F_TimeEvent(void)
* @ msg    : 时间变量处理
* @ param   (*)
* @ return  (*)
 ********************************************************************************************************/
void F_TimeEvent(void)
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
		if((Sys.TimeOnCount) && (STATE_WORK == Sys.MState))
		{
			if(++Sys.TimeContOn >= 3600)
			{
				Sys.TimeContOn = 0;
				Sys.TimeOnCount--;
				if(4 == Sys.TimeOnCount)
					Sys.mFanOnTime = T_ON_FOUR;
				else if(2 == Sys.TimeOnCount)
					Sys.mFanOnTime = T_ON_TWO;
				if(!Sys.TimeOnCount)
				{
					Sys.mFanOnTime = T_ON_OFF;
					BELL_NOR();
				}
			}
		}
		else
		{	
			Sys.TimeContOn = 0;
		}

		if(Sys.Time  && (STATE_WORK == Sys.MState))
		{
			if(++Sys.TimeContDown >= 3600)
			{
				Sys.TimeContDown = 0;
				Sys.Time--;
				if(2 == Sys.Time)
					Sys.mFanOFF_Time = T_OFF_TWO;
				else if(1 == Sys.Time)
					Sys.mFanOFF_Time = T_OFF_ONE;
				if(!Sys.Time)
				{
					Sys.mFanOFF_Time = T_OFF_OFF;
					WorkModeNewSet(SET_STATE_NONE);
					BELL_NOR();
				}
			}
		}
		else
		{	
			Sys.TimeContDown = 0;
		}


		if(Sys.Fg.FanSleep  && (STATE_WORK == Sys.MState))
		{
			if(++Sys.SleepSec >= 1800)	//30min
			{
				Sys.SleepSec = 0;
	
				if(++Sys.SleepHalfHour >= 16)
				{
					Sys.SleepHalfHour = 0;
					Sys.SleepSec = 0;
					Sys.Fg.FanSleep  = 0;
					WorkModeNewSet(SET_STATE_NONE);
					BELL_NOR();
				}
			}
		}
		else
		{	
			Sys.SleepHalfHour = 0;
			Sys.SleepSec = 0;
		}

		
	}



	
    if (GetTimeTick(TIME_100MS))
    {
		
    }
    if (GetTimeTick(TIME_1S))
    {
		if((STATE_WORK == Sys.MState) && !Sys.Fg.LightSleep)
		{
			if(++Sys.IDLE_Sec >= 30)
			{
				Sys.IDLE_Sec = 0;
				Sys.Fg.LightSleep = 1;
			}
		}
    }
	if(STATE_NONE == Sys.MState) 
	{
		Sys.IDLE_Sec = 0;
		Sys.Fg.LightSleep = 0;
	}
}
