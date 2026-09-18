#define _Native_Out_H
#include "Include\Include.h"




void HeatX_Output(Out_Enum mOutputEn)
{
	static u8 u8RelayOn_Cnt = 0;
	if(mOutputEn)
	{
		if(u8RelayOn_Cnt < 255)
			u8RelayOn_Cnt ++;
		if(u8RelayOn_Cnt >= OUTPUT_ON_TIME)
		{
			HeatX_ON();
		}
	}
	else
	{
		HeatX_OFF();	
		u8RelayOn_Cnt = 0;
	}
}

void HeatY_Output(Out_Enum mOutputEn)
{
	static u8 u8RelayOn_Cnt = 0;
	if(mOutputEn)
	{
		if(u8RelayOn_Cnt < 255)
			u8RelayOn_Cnt ++;
		if(u8RelayOn_Cnt >= OUTPUT_ON_TIME)
		{
			HeatY_ON();
		}
	}
	else
	{
		HeatY_OFF();	
		u8RelayOn_Cnt = 0;
	}
}








