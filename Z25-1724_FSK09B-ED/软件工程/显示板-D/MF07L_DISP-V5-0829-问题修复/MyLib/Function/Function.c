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

#define _FAST_TEST_
/********************************************************************************************************
* @ name   : void F_TimeEvent(void)
* @ msg    : 时间变量处理
* @ param   (*)
* @ return  (*)
 ********************************************************************************************************/
void F_TimeEvent(void)
{
//	#ifdef _FAST_TEST_
//	#warning "-----当前定时开机、关机为缩时模式------"
//	#define	TIME_SECOND		2000		//10ms
//	#else
//	#define	TIME_SECOND		8000
//	#endif
//	if(s_u161sTime >= TIME_SECOND)
//	{
//		s_u161sTime -= TIME_SECOND;
//		if((Sys.TimeOnCount) && (STATE_NONE == Sys.MState))
//		{
//			if(++Sys.TimeContOn >= 3600)
//			{
//				Sys.TimeContOn = 0;
//				Sys.TimeOnCount--;
//				if(3 == Sys.TimeOnCount)
//					Sys.mFanOnTime = T_ON_THREE;
//				else if(2 == Sys.TimeOnCount)
//					Sys.mFanOnTime = T_ON_TWO;
//				else if(1 == Sys.TimeOnCount)
//					Sys.mFanOnTime = T_ON_ONE;
//				if(!Sys.TimeOnCount)
//				{
//					Sys.mFanOnTime = T_ON_OFF;
//					BELL_NOR();
//					
//				  //new0829
//					WorkModeNewSet(SET_FST_ON);						
//				}
//			}
//		}
//		else
//		{	
//			Sys.TimeContOn = 0;
//		}

//		if(Sys.Time  && (STATE_WORK == Sys.MState))
//		{
//			if(++Sys.TimeContDown >= 3600)
//			{
//				Sys.TimeContDown = 0;
//				Sys.Time--;
//				if(3 == Sys.Time)
//					Sys.mFanOFF_Time = T_OFF_THREE;
//				if(2 == Sys.Time)
//					Sys.mFanOFF_Time = T_OFF_TWO;
//				else if(1 == Sys.Time)
//					Sys.mFanOFF_Time = T_OFF_ONE;
//				if(!Sys.Time)
//				{
//					Sys.mFanOFF_Time = T_OFF_OFF;
//					WorkModeNewSet(SET_STATE_NONE);
//					BELL_NOR();
//				}
//			}
//		}
//		else
//		{	
//			Sys.TimeContDown = 0;
//		}


//		if(FAN_SLEEP == Sys.Mode && (STATE_WORK == Sys.MState))
//		{
//			if(++Sys.SleepSec >= 1800)	//30min
//			{
//				Sys.SleepSec = 0;
//	
//				if(++Sys.SleepHalfHour >= 16)
//				{
//					Sys.SleepHalfHour = 0;
//					Sys.SleepSec = 0;
//					Sys.Mode = FAN_NORMAL;
//					WorkModeNewSet(SET_STATE_NONE);
//					BELL_NOR();
//				}
//			}
//		}
//		else
//		{	
//			Sys.SleepHalfHour = 0;
//			Sys.SleepSec = 0;
//		}

//		
//	}



	
    if (GetTimeTick(TIME_100MS))
    {
		
    }
    if (GetTimeTick(TIME_1S))
    {
		if((Sys.TimeOnCount) && (STATE_NONE == Sys.MState))
		{
			if(++Sys.TimeContOn >= 3600)
			{
				Sys.TimeContOn = 0;
				Sys.TimeOnCount--;
				if(3 == Sys.TimeOnCount)
					Sys.mFanOnTime = T_ON_THREE;
				else if(2 == Sys.TimeOnCount)
					Sys.mFanOnTime = T_ON_TWO;
				else if(1 == Sys.TimeOnCount)
					Sys.mFanOnTime = T_ON_ONE;
				if(!Sys.TimeOnCount)
				{
					Sys.mFanOnTime = T_ON_OFF;
					BELL_NOR();
					
				  //new0829
					WorkModeNewSet(SET_FST_ON);						
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
				if(3 == Sys.Time)
					Sys.mFanOFF_Time = T_OFF_THREE;
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


		if(FAN_SLEEP == Sys.Mode && (STATE_WORK == Sys.MState))
		{
			if(++Sys.SleepSec >= 1800)	//30min
			{
				Sys.SleepSec = 0;
	
				if(++Sys.SleepHalfHour >= 16)
				{
					Sys.SleepHalfHour = 0;
					Sys.SleepSec = 0;
					Sys.Mode = FAN_NORMAL;
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

/**************************************************
*函数名称：void  F_mSystemInital(void) 
*函数功能：逻辑初始
*入口参数：void
*出口参数：void  
**************************************************/
void F_mSystemInital(void)
{
	Sys.mFan = FAN_ONE;
	Sys.TimeOnCount = 0;
	Sys.TimeContDown = 0;
	Sys.IDLE_Sec = 0;
	Sys.Fg.LightSleep = 0;
	Sys.mFanOnTime = T_ON_OFF;
	Sys.mFanOFF_Time = T_OFF_OFF;
}

//10ms


//安全开关检测
u8 CheckFgChange(u8 DatBit,u8 u8NowBit,u16 Setcnt,u16 ClrCnt)
{
	u8 X_REG Buf = 0;
	static X_REG u16 Sw_Cnt = 0;
	if(0 == DatBit)
	{
		if(u8NowBit)  
		{
			if(++Sw_Cnt >= Setcnt)
			{
				Sw_Cnt = 0;
				DatBit = u8NowBit;  
				Sys.Err.StoreBit = u8NowBit;
				Buf = 1;
			}	
		}
		else
		{
			Sw_Cnt = 0;	
		}		
	}
	else
	{
		if(0 == u8NowBit)
		{
			if(++Sw_Cnt >= ClrCnt)
			{
				Sw_Cnt = 0;
				DatBit = u8NowBit;	
				Sys.Err.StoreBit = u8NowBit;
				Buf = 1;
			}	
		}
		else
		{
			Sw_Cnt = 0;	
		}	
	}	
	return Buf;
}


//10ms
void PowErrCheck()
{
	if(g_u16Bat_Vol >= 8500)
	{
		Sys.Err.BitBuf = 0;
	}
	else if(g_u16Bat_Vol <= 8000)
	{
		Sys.Err.BitBuf = 1;
	}
	
	
	if(CheckFgChange(Sys.Err.PowVolLow,Sys.Err.BitBuf,50,20))
		Sys.Err.PowVolLow = Sys.Err.StoreBit;
}
