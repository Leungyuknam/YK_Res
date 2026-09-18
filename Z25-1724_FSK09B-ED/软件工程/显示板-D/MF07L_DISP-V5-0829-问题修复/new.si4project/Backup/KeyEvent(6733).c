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
		Sys.MState = COOK_NONE;		
		Sys.Time = 0;
		*((u8 *)&Sys.Sign) = 0;
		break;
	case SET_FST_ON:
		Sys.MState = COOK_IDLE;
		Sys.mMenu = MENU_BAOWEN;
		break;
		
	case SET_RET_IDLE:
		Sys.MState = COOK_IDLE;
		break;
		
	case SET_M_BAOWEN:
		Sys.mMenu = MENU_BAOWEN;
		Sys.mTempLevel = MenuCookTempDefTab[Sys.mMenu];
		Sys.Time = 0;//MenuCookTimeDefTab[Sys.mMenu];
		break;
	
	case SET_M_DIE_ZI:
		Sys.mMenu = MENU_DIE_ZI;
		Sys.mTempLevel = MenuCookTempDefTab[Sys.mMenu];
		Sys.Time = 0;//MenuCookTimeDefTab[Sys.mMenu];
		break;
		
	case SET_M_CUP:
		Sys.mMenu = MENU_CUP;
		Sys.mTempLevel = MenuCookTempDefTab[Sys.mMenu];
		Sys.Time = 0;//MenuCookTimeDefTab[Sys.mMenu];
		break;

	default:

		break;
	}
	F_DispTaskChange(DISP_NORMAL);
	TimeFucRunStart();		//÷ÿ÷√º∆ ±
}


void KeyFactory(void)
{
	if(SysMX_Run == SysMode) 
	{
		if(Sys.Fg.Lock)
		{

		}	
		else if(COOK_NONE == Sys.MState)
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
		else if(COOK_NONE == Sys.MState)
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





