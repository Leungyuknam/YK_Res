#define _Native_KeyEvent_H
#include "Include\Include.h"
#include <intrins.h>




u32 F_KeyScan()
{

	return g_u32KeyBuf;
}


void WorkModeNewSet(SetMode_Enum mNewDat)
{
	*((u8 *)&Sys.Fg) = 0;
	Sys.Err = 0;
	Sys.IDLE_Sec = 0; 
	Sys.SleepHalfHour = 0;
	Sys.SleepSec = 0;
	switch (mNewDat)
	{
	case SET_STATE_NONE:
		Sys.mFan = 0;
		Sys.Mode = FAN_NORMAL; 

		Sys.mFanOFF_Time = T_OFF_OFF;
		Sys.Time = 0;

		Sys.mFanOnTime = T_ON_OFF;
		Sys.TimeOnCount = 0;
		

		Sys.MState = STATE_NONE;	
		*((u8 *)&Sys.Sign) = 0;
		break;
		
	case SET_FST_ON:
		Sys.Mode = FAN_NORMAL; 
		Sys.mFan = FAN_ONE;
		Sys.MState = STATE_WORK;
		break;
	

	default:

		break;
	}
	F_DispTaskChange(DISP_NORMAL);
	TimeFucRunStart();		//重置计时
}

void KeyOFFON(void)
{
	if(SysMX_Factory == SysMode) 
	{
		FactoryKeyNumFuc(5);
		BELL_NOR();
	}
	else 
	{
		if(Sys.Fg.LightSleep)
		{
			Sys.Fg.LightSleep = 0;
			Sys.IDLE_Sec = 0;
			BELL_NOR();
		}
		else if(SysMX_Run == SysMode)
		{
			if(STATE_NONE == Sys.MState)
			{
				WorkModeNewSet(SET_FST_ON);
				
			}
			else
			{
				WorkModeNewSet(SET_STATE_NONE);
			}
			BELL_NOR();
		}
	}
}


/**************************************************
*函数名称：void      F_WingLevelInc(void) 
*函数功能：
*入口参数：void
*出口参数：void  
**************************************************/
void F_WingLevelInc(void)
{	 
	if(SysMX_Factory == SysMode ) 
	{
		FactoryKeyNumFuc(6);
		BELL_NOR();
	}
	else if((SysMX_Run == SysMode) && (STATE_WORK == Sys.MState))
	{
		if(Sys.Fg.LightSleep)
		{
			Sys.Fg.LightSleep = 0;
			Sys.IDLE_Sec = 0;
			BELL_NOR();
		}
		else if(Sys.PowMessage.PowRecSIErr || (GetDispType() == DISP_TIME_ON_SET) || Sys.TimeOnCount)
		{	
			Sys.IDLE_Sec = 0;
			return;
		}
		else 
		{
			Sys.IDLE_Sec = 0;
			if(FAN_NATURE == Sys.Mode || Sys.Fg.FanSleep) 
			{
				Sys.Mode = FAN_NORMAL; 
				Sys.Fg.FanSleep = 0;
				BELL_NOR();
				if(Sys.mFan > FAN_SEVEN)
					Sys.mFan = FAN_SEVEN;
				else if(Sys.mFan < FAN_ONE)
					Sys.mFan = FAN_ONE;	
			}
			else
			{
				if(Sys.mFan < FAN_SEVEN)
				{	
					Sys.mFan ++;
				}
				BELL_NOR();
			}
		}
	}
}

/**************************************************
*函数名称：void      F_WingLevelDec(void) 
*函数功能：
*入口参数：void
*出口参数：void  
**************************************************/
void F_WingLevelDec(void)
{	 
	if(SysMX_Factory == SysMode ) 
	{
		FactoryKeyNumFuc(4);
		BELL_NOR();
	}
	else if((SysMX_Run == SysMode) && (STATE_WORK == Sys.MState))
	{
		if(Sys.Fg.LightSleep)
		{
			Sys.Fg.LightSleep = 0;
			Sys.IDLE_Sec = 0;
			BELL_NOR();
		}
		else if(Sys.PowMessage.PowRecSIErr || (GetDispType() == DISP_TIME_ON_SET) || Sys.TimeOnCount)
		{	
			Sys.IDLE_Sec = 0;
			return;
		}
		else 
		{
			Sys.IDLE_Sec = 0;
			if(FAN_NATURE == Sys.Mode || Sys.Fg.FanSleep) 
			{
				Sys.Mode = FAN_NORMAL; 
				Sys.Fg.FanSleep = 0;
				BELL_NOR();
				if(Sys.mFan > FAN_SEVEN)
					Sys.mFan = FAN_SEVEN;
				else if(Sys.mFan < FAN_ONE)
					Sys.mFan = FAN_ONE;	
			}
			else
			{
				if(Sys.mFan > FAN_ONE)
				{	
					Sys.mFan --;
				}
				BELL_NOR();
			}
		}
	}
}

/**************************************************
*函数名称：void      F_WingNature(void) 
*函数功能：
*入口参数：void
*出口参数：void  
**************************************************/
void F_WingNature(void)
{	 
	if(SysMX_Factory == SysMode ) 
	{
		FactoryKeyNumFuc(8);
		BELL_NOR();
	}
	else if((SysMX_Run == SysMode) && (STATE_WORK == Sys.MState))
	{
		if(Sys.Fg.LightSleep)
		{
			Sys.Fg.LightSleep = 0;
			Sys.IDLE_Sec = 0;
			BELL_NOR();
		}
		else if(Sys.PowMessage.PowRecSIErr || (GetDispType() == DISP_TIME_ON_SET) || Sys.TimeOnCount)
		{	
			Sys.IDLE_Sec = 0;
			return;
		}
		else
		{
			Sys.IDLE_Sec = 0;
			if(Sys.Mode != FAN_NATURE || Sys.Fg.FanSleep)
			{	
				Sys.Mode = FAN_NATURE; 
				Sys.Fg.FanSleep = 0;
				BELL_NOR();
			}
			else 
			{	
				Sys.Mode = FAN_NORMAL; 
			
				BELL_NOR();
			}
		}
	}
}

/**************************************************
*函数名称：void      F_WingSleep(void) 
*函数功能：
*入口参数：void
*出口参数：void  
**************************************************/
void F_WingSleep(void)
{	 
	if(SysMX_Factory == SysMode ) 
	{
		FactoryKeyNumFuc(8);
		BELL_NOR();
	}
	else if((SysMX_Run == SysMode) && (STATE_WORK == Sys.MState))
	{
		if(Sys.Fg.LightSleep)
		{
			Sys.Fg.LightSleep = 0;
			Sys.IDLE_Sec = 0;
			BELL_NOR();
		}
		else if(Sys.PowMessage.PowRecSIErr || (GetDispType() == DISP_TIME_ON_SET) || Sys.TimeOnCount)
		{	
			Sys.IDLE_Sec = 0;
			return;
		}
		else
		{
			Sys.IDLE_Sec = 0;
			if(Sys.Fg.FanSleep)
			{	
				Sys.Fg.FanSleep = 0; 
				BELL_NOR();
			}
			else
			{	
				Sys.Fg.FanSleep = 1;
				BELL_NOR();
			}
		}
	}
}


/**************************************************
*函数名称：void      F_SetTimeOff(void) 
*函数功能：
*入口参数：void
*出口参数：void  
**************************************************/
void F_SetTimeOff(void)
{	 
	if(SysMX_Factory == SysMode ) 
	{
		FactoryKeyNumFuc(3);
		BELL_NOR();
	}
	else if((SysMX_Run == SysMode) && (STATE_WORK == Sys.MState))
	{
		if(Sys.Fg.LightSleep)
		{
			Sys.Fg.LightSleep = 0;
			Sys.IDLE_Sec = 0;
			BELL_NOR();
		}
		else if(Sys.PowMessage.PowRecSIErr || (GetDispType() == DISP_TIME_ON_SET) || Sys.TimeOnCount)
		{	
			Sys.IDLE_Sec = 0;
			return;
		}
		else
		{
			Sys.IDLE_Sec = 0;
			if(GetDispType() == DISP_TIME_OFF_SET)
			{
				if(++Sys.mFanOFF_Time > T_OFF_FOR)
					Sys.mFanOFF_Time = T_OFF_OFF;
				if(T_OFF_OFF == Sys.mFanOFF_Time)
					Sys.Time = 0;
				else if(T_OFF_ONE == Sys.mFanOFF_Time)
					Sys.Time = 1;
				else if(T_OFF_TWO == Sys.mFanOFF_Time)
					Sys.Time = 2;
				else 
					Sys.Time = 4;
				Sys.TimeContDown = 0;
				F_DispTaskChange(DISP_TIME_OFF_SET);
				BELL_NOR();
			}
			else if((GetDispType() != DISP_TIME_ON_SET) && !Sys.TimeOnCount)
			{
				if(T_OFF_OFF == Sys.mFanOFF_Time)
				{
					Sys.mFanOFF_Time = T_OFF_ONE;	
					Sys.Time = 1;
					Sys.TimeContDown = 0;
				}
				F_DispTaskChange(DISP_TIME_OFF_SET);
				BELL_NOR();
			}
		}
	}
}

/**************************************************
*函数名称：void      F_SetTimeOn(void) 
*函数功能：
*入口参数：void
*出口参数：void  
**************************************************/
void F_SetTimeOn(void)
{	 
	if(SysMX_Factory == SysMode ) 
	{
		FactoryKeyNumFuc(7);
		BELL_NOR();
	}
	else if((SysMX_Run == SysMode) && (STATE_WORK == Sys.MState))
	{
		if(Sys.Fg.LightSleep)
		{
			Sys.Fg.LightSleep = 0;
			Sys.IDLE_Sec = 0;
			BELL_NOR();
		}
		else if(Sys.PowMessage.PowRecSIErr || (GetDispType() == DISP_TIME_ON_SET) || Sys.TimeOnCount)
		{	
			Sys.IDLE_Sec = 0;
			return;
		}
		else
		{
			Sys.IDLE_Sec = 0;
			if(GetDispType() == DISP_TIME_ON_SET)
			{
				if(++Sys.mFanOnTime > T_ON_SIX)
					Sys.mFanOnTime = T_ON_OFF;
				
				if(T_ON_OFF == Sys.mFanOnTime)
					Sys.TimeOnCount = 0;
				else if(T_ON_TWO == Sys.mFanOnTime)
					Sys.TimeOnCount = 2;
				else if(T_ON_FOUR == Sys.mFanOnTime)
					Sys.TimeOnCount = 4;
				else 
					Sys.TimeOnCount = 6;
				Sys.TimeContOn = 0;
				if(T_ON_OFF == Sys.mFanOnTime)
					F_DispTaskChange(DISP_NORMAL);
				else
					F_DispTaskChange(DISP_TIME_ON_SET);
				BELL_NOR();
			}
			else if((GetDispType() != DISP_TIME_OFF_SET) && !Sys.Time)
			{
				if(T_ON_OFF == Sys.mFanOnTime)
				{
					Sys.mFanOnTime = T_ON_TWO;
					//Sys.TimeBuf = 2;
					Sys.TimeOnCount = 2;
					Sys.TimeContOn = 0;
				}
				F_DispTaskChange(DISP_TIME_ON_SET);
				BELL_NOR();
			}
		}
	}
}



/**************************************************
*函数名称：void  F_RLOSCKey(void) 
*函数功能：左右摇头控制
*入口参数：void
*出口参数：void  
**************************************************/
void F_RLOSC_AngleKey()
{	
	if(SysMX_Factory == SysMode ) 
	{
		FactoryKeyNumFuc(1);
		BELL_NOR();
	}
	else if((SysMX_Run == SysMode) && (STATE_WORK == Sys.MState))
	{
		if(Sys.Fg.LightSleep)
		{
			Sys.Fg.LightSleep = 0;
			Sys.IDLE_Sec = 0;
			BELL_NOR();
		}
		else if(Sys.PowMessage.PowRecSIErr || (GetDispType() == DISP_TIME_ON_SET) || Sys.TimeOnCount)
		{	
			Sys.IDLE_Sec = 0;
			return;
		}
		else
		{
			Sys.IDLE_Sec = 0;
			if(!Sys.Fg.RLOSC)
			{	
				Sys.Fg.RLOSC = 1;
				Sys.mAngle = ANGLE_30;
				BELL_NOR();
			}
			else
			{
				Sys.Fg.RLOSC = 0;
				BELL_NOR();
			}
			/*
			else if(GetDispType() == DISP_ANGLE_SET)
			{
				if(++Sys.mAngle > ANGLE_80)
				{
					Sys.Fg.RLOSC = 0;
					F_DispTaskChange(DISP_NORMAL);
				}
				else
				{
					Sys.Fg.RLOSC = 1;
					F_DispTaskChange(DISP_ANGLE_SET); 
				}
				BELL_NOR();
			}
			else if(Sys.Fg.RLOSC || (GetDispType() != DISP_ANGLE_SET))
			{
				F_DispTaskChange(DISP_ANGLE_SET); 
				BELL_NOR();
			}
			*/
		}
	}
}


/**************************************************
*函数名称：void     	 F_UpDnOscKey(void) 
*函数功能：左右摇头控制
*入口参数：void
*出口参数：void  
**************************************************/
void F_UpDnOscKey()
{	
	if(SysMX_Factory == SysMode ) 
	{
		FactoryKeyNumFuc(1);
		BELL_NOR();
	}
	else if((SysMX_Run == SysMode) && (STATE_WORK == Sys.MState))
	{
		if(Sys.Fg.LightSleep)
		{
			Sys.Fg.LightSleep = 0;
			Sys.IDLE_Sec = 0;
			BELL_NOR();
		}
		else if(Sys.PowMessage.PowRecSIErr || (GetDispType() == DISP_TIME_ON_SET) || Sys.TimeOnCount)
		{	
			Sys.IDLE_Sec = 0;
			return;
		}
		else
		{
			Sys.IDLE_Sec = 0;
			if(!Sys.Fg.UDOSC)
			{	
				Sys.Fg.UDOSC = 1;
				F_DispTaskChange(DISP_NORMAL);
			}
			else
			{
				Sys.Fg.UDOSC = 0;
				F_DispTaskChange(DISP_NORMAL);
			}
	        BELL_NOR();
		}
	}
}

void KeyFactory(void)
{
	if(SysMX_Run == SysMode) 
	{
		if(Sys.Fg.Lock)
		{

		}	
		else if(STATE_NONE == Sys.MState)
		{
			if(g_u8PowerON_TCnt < 10)
			{
				SetFactoryMode();
				BELL_NOR();
			}
		}
	}
}

#if 0
void KeyCustomerTest(void)
{
	if(SysMX_Run == SysMode) 
	{
		if(Sys.Fg.Lock)
		{

		}	
		else if(STATE_NONE == Sys.MState)
		{
			if(g_u8PowerON_TCnt < 10)
			{
				SetFactoryMode();
				SysMode = SysMX_CustomerTest;
				BELL_NOR();
			}
		}
	}
}
#endif




