#define _Native_OutDeal_H
#include "Include\Include.h"



//100ms
void OutPutDeal(void)
{
	u8 SetLevel = 0;
	SysLoadBuf.Byte = 0;
	if(SysMX_Run == SysMode) 
	{
		if((Sys.MState == COOK_RUN_PAUSE) && Sys.Fg.HeatEn && Sys.Time && Sys.PowMessage.DoorState)
		{
			SetLevel = DriveControDeal(0xff - Sys.TempAD);
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
		BELL_NOR();
	else if(FstFactory_TCnt == 3*TIME_DIV)
		BELL_NOR();
	
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
