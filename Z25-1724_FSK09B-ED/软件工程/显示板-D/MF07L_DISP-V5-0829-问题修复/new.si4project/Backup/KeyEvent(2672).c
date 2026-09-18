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
	switch (mNewDat)
	{
	case SET_STATE_NONE:
		Sys.mFan = 0;
		Sys.Fg.UDOSC = 0;
		Sys.Fg.RLOSC = 0;
		Sys.Mode = FAN_NORMAL; 
		Sys.Time = 0;
		Sys.TimeOnCount = 0;
		g_u8TimeSet = 0;
		Sys.UpDnSMRstState = Rst_Need;
		Sys.LR_SMRstState = Rst_Need;
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
	g_u8TimeSet = 0;
	F_DispTaskChange(DISP_NORMAL);
	TimeFucRunStart();		//重置计时
}

void KeyOFFON(void)
{
	if(SysMX_Factory == SysMode || SysMX_CustomerTest == SysMode) 
	{
		FactoryKeyNumFuc(5);
		BELL_NOR();
	}
	else if(SysMX_Run == SysMode)
	{
		if(Sys.Fg.LightSleep)
		{
			Sys.Fg.LightSleep = 0;
			Sys.IDLE_Sec = 0;
		}
		else if(SysMX_Run == SysMode)
		{
			if(STATE_NONE == Sys.MState)
			{
				WorkModeNewSet(SET_STATE_NONE);
			}
			else
			{
				WorkModeNewSet(SET_FST_ON);
			}
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
	if(SysMX_Factory == SysMode || SysMX_CustomerTest == SysMode) 
	{
		FactoryKeyNumFuc(6);
		BELL_NOR();
	}
	else if(SysMX_Run == SysMode)
	{
		if(Sys.Fg.LightSleep)
		{
			Sys.Fg.LightSleep = 0;
			Sys.IDLE_Sec = 0;
			BELL_NOR();
		}
		else if(Sys.PowMessage.PowRecSIErr || (GetDispType() == DISP_TIME_ON_SET) || Sys.TimeOnCount)
		{	
			return;
		}
		else 
		{
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
	if(SysMX_Factory == SysMode || SysMX_CustomerTest == SysMode) 
	{
		FactoryKeyNumFuc(4);
		BELL_NOR();
	}
	else if(SysMX_Run == SysMode)
	{
		if(Sys.Fg.LightSleep)
		{
			Sys.Fg.LightSleep = 0;
			Sys.IDLE_Sec = 0;
			BELL_NOR();
		}
		else if(Sys.PowMessage.PowRecSIErr || (GetDispType() == DISP_TIME_ON_SET) || Sys.TimeOnCount)
		{	
			return;
		}
		else 
		{
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
	if(SysMX_Factory == SysMode || SysMX_CustomerTest == SysMode) 
	{
		FactoryKeyNumFuc(8);
		BELL_NOR();
	}
	else if(SysMX_Run == SysMode)
	{
		if(Sys.Mode != FAN_NATURE)
		{	
			Sys.Mode = FAN_NATURE; 

			BELL_NOR();
		}
		else if(Sys.Mode != FAN_NORMAL)
		{	
			Sys.Mode = FAN_NATURE; 
		
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





