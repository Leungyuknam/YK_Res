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
		break;
	case SET_FST_ON:
		Sys.MState = COOK_IDLE;
		Sys.mMenu = MENU_NONE;
		//Sys.Fg.HadHeated = FALSE;
		break;
	case SET_NOW_MENU_RESET:

		Sys.MState = COOK_IDLE;
		*((u8 *)&Sys.Fg) = 0;
		if((MENGZHU == Sys.mMenu) || (BAOTANG == Sys.mMenu) || (MANDUN == Sys.mMenu))
		{
			Sys.Fg.CooktimeEn = TRUE;
			Sys.CookRunSec = MenuCookTimeDefTab[Sys.mMenu];			
		}

		break;
	default:

		break;
	}
	F_DispTaskChange(DISP_NORMAL);
	TimeFucRunStart();		//÷ÿ÷√º∆ ±
}




