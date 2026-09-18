#define _Native_OutDeal_H
#include "Include\Include.h"



//100ms
void OutPutDeal(void)
{
	static u8 KeepFanRunSec = 0;
	u8 SetLevel = 0;
	SysLoadBuf.Byte = 0;
	if(SysMX_Run == SysMode) 
	{
		if((Sys.MState == COOK_RUN_PAUSE) && Sys.Fg.HeatEn && Sys.Time && Sys.PowMessage.DoorState)
		{
			SetLevel = DriveControDeal(0x3ff - Sys.TempAD);
			if(SetLevel)
				SysLoadBuf.Bits.Heat = TRUE;
		}
		else
		{
			sHeat.HeatStep = 0;
		}
			
		if(Sys.MState == COOK_RUN_PAUSE)
		{
			SysLoadBuf.Bits.Light = TRUE;
			SysLoadBuf.Bits.Fan = TRUE;
			KeepFanRunSec = 240;
		}
		else if(Sys.MState == COOK_IDLE)
		{
			if(Sys.Fg.HadWork)
			{
				if(Sys.mLastWorkTempLevel >= TEMP_50C)
				{
					if(Sys.EnvironTemp >= 45)
						SysLoadBuf.Bits.Fan = TRUE;
					else
						Sys.Fg.HadWork = 0;
				}
				else
				{
					if(KeepFanRunSec && GetTimeTick(TIME_1S))
						KeepFanRunSec --;
					if (KeepFanRunSec)
						SysLoadBuf.Bits.Fan = TRUE;
				}
			}
		}

		//π ’œ”≈œ»
		if((g_sTempAD.AD_Staut != c_AD_NORMAL) && (Sys.MState != COOK_NONE))
		{
			SysLoadBuf.Bits.Heat = FALSE;
			SysLoadBuf.Bits.Fan = TRUE;
		}
	}
	SysLoad.Byte = SysLoadBuf.Byte;
}

#define TIME_DIV		10
#define	DRV_NUM			4
void FactoryOutPutDeal(void)
{
    static X_REG u8 FstFactory_TCnt = 0;
	SysLoadBuf.Byte = 0;
	if(GetTimeTick(TIME_100MS))
	{
		if(FstFactory_TCnt < 255)
			FstFactory_TCnt ++;
		if(FstFactory_TCnt >= TIME_DIV*DRV_NUM)
			FstFactory_TCnt = 0;
	}
	
	if(FstFactory_TCnt == TIME_DIV)
	{	BELL_NOR();}
	else if(FstFactory_TCnt == 3*TIME_DIV)
	{	BELL_NOR();}
	
    if(FstFactory_TCnt < TIME_DIV)
    {
		SysLoadBuf.Bits.Heat = TRUE;
    }
	else if(FstFactory_TCnt < 2*TIME_DIV)
	{
		SysLoadBuf.Bits.Fan = TRUE;
	}
	else if(FstFactory_TCnt < 3*TIME_DIV)
	{
		SysLoadBuf.Bits.WarmHeat = TRUE;
	}
	else 
	{
		SysLoadBuf.Bits.Light = TRUE;
	}
	SysLoad.Byte = SysLoadBuf.Byte;
}

void CustomerTestOutPutDeal(void)
{
	SysLoadBuf.Byte = 0;
    if(1 == FactoryKeyNum)
    {
    	SysLoadBuf.Bits.Fan = TRUE;
		
    }
	else if(2 == FactoryKeyNum)
	{
		SysLoadBuf.Bits.Light = TRUE;
	}
	else if(4 == FactoryKeyNum)
	{
		SysLoadBuf.Bits.Heat = TRUE;
	}
	SysLoad.Byte = SysLoadBuf.Byte;
}



