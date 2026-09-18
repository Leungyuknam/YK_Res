#define _Native_OutDeal_H
#include "Include\Include.h"




//100ms
void OutPutDeal(void)
{
	SysLoadBuf.Byte = 0;
	if(SysMX_Run == SysMode) 
	{

	}
}

void FactoryOutPutDeal(void)
{
    static X_REG u8 FstFactory_TCnt = 0;
	SysLoadBuf.Byte = 0;
	if(GetTimeTick(TIME_100MS))
	{
		if(FstFactory_TCnt < 255)
			FstFactory_TCnt ++;
		if(FstFactory_TCnt >= 70)
			FstFactory_TCnt = 0;
	}

    if(FstFactory_TCnt < 10)
    {
//		SysLoadBuf.Bits.ValveA = TRUE;
    }




	SysLoad.Byte = SysLoadBuf.Byte;
	
//	if(SysLoad.Bits.ValveA)
//		mValveA_OutEn = EN_ON;
//	else
//		mValveA_OutEn = EN_OFF;	


}
