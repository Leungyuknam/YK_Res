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
		Sys.mMenu = MENU_BAOWEN;
		break;
	case SET_NOW_MENU_RESET:
		Sys.MState = COOK_IDLE;
		*((u8 *)&Sys.Fg) = 0;
		break;
	case SET_RET_IDLE:

		break;
		
	case SET_M_BAOWEN:
		Sys.mMenu = MENU_BAOWEN;
		Sys.mTempLevel = TEMP_50C;
		break;
	
	case SET_M_DIE_ZI:
		Sys.mMenu = MENU_DIE_ZI;
		Sys.mTempLevel = TEMP_50C;
		break;
		
	case SET_M_CUP:
		Sys.mMenu = MENU_CUP;
		Sys.mTempLevel = TEMP_40C;
		break;

	default:

		break;
	}
	F_DispTaskChange(DISP_NORMAL);
	TimeFucRunStart();		//ÖØÖÃ¼ÆÊ±
}

void KeyPow(void)
{
	if(SysMX_Run == SysMode) 
	{
		if(COOK_NONE == Sys.MState)
		{
			WorkModeNewSet(SET_FST_ON);
			BELL_NOR();
		}
		else if(COOK_RUN_PAUSE == Sys.MState)
		{
			WorkModeNewSet(SET_RET_IDLE);
			BELL_NOR();
		}

	}
}

void KEY_Lock(void)
{
	if(SysMX_Run == SysMode) 
	{


	}
}


void KEY_StartPause(void)
{
	if(SysMX_Run == SysMode) 
	{


		if(COOK_IDLE == Sys.MState)
		{
			Sys.MState = COOK_RUN_PAUSE;
			Sys.Fg.HeatEn = TRUE;
			F_DispTaskChange(DISP_NORMAL);
		}
		if(COOK_RUN_PAUSE == Sys.MState)
		{
			Sys.Fg.HeatEn ^= 1;
			BELL_NOR();
			F_DispTaskChange(DISP_NORMAL);
		}
	}
}


void KEY_Dec(void)
{
	if((SysMX_Run == SysMode) && (COOK_IDLE == Sys.MState))
	{
		if(Sys.Fg.Lock)
		{	return;}

		if(DISP_TIME_OFF_SET == GetDispType())
		{
			if(Sys.TimeBuf > MenuCookTimeMinTab[Sys.mMenu])
			{
				Sys.TimeBuf  --;
		
			}
			F_DispTaskChange(DISP_TIME_OFF_SET);
			BELL_NOR();
		}
		else if(DISP_TEMP_SET == GetDispType())
		{
			if(++Sys.mTempLevel > MenuCookTempMaxTab[Sys.mMenu])
			{	Sys.mTempLevel = MenuCookTempMaxTab[Sys.mMenu];}
			F_DispTaskChange(DISP_TEMP_SET);
			BELL_NOR();
		}
	}
}


void KEY_Inc(void)
{
	if((SysMX_Run == SysMode) && (COOK_IDLE == Sys.MState))
	{
		if(Sys.Fg.Lock)
		{	return;}

		if(DISP_TIME_OFF_SET == GetDispType())
		{
			if(++Sys.TimeBuf > MenuCookTimeMaxTab[Sys.mMenu])
			{
				Sys.TimeBuf  = MenuCookTimeMaxTab[Sys.mMenu];

			}
			F_DispTaskChange(DISP_TIME_OFF_SET);
			BELL_NOR();
		}
		else if(DISP_TEMP_SET == GetDispType())
		{
			if(++Sys.mTempLevel > MenuCookTempMaxTab[Sys.mMenu])
			{	Sys.mTempLevel = MenuCookTempMaxTab[Sys.mMenu];}
			F_DispTaskChange(DISP_TEMP_SET);
			BELL_NOR();
		}
	}
}


void KEY_Time(void)
{
	if(SysMX_Run == SysMode) 
	{
		if(COOK_IDLE == Sys.MState)
		{
		
			if((GetDispType() != DISP_TIME_OFF_SET))
			{
				if(0 == Sys.Time)
					Sys.TimeBuf = MenuCookTimeDefTab[Sys.mMenu];
				else
					Sys.TimeBuf = Sys.Time;	
				Sys.u16TimeContDown = 0;
				F_DispTaskChange(DISP_TIME_OFF_SET);
				BELL_NOR();
			}
			else if(GetDispType() == DISP_TIME_OFF_SET)
			{
				F_DispTaskChange(DISP_TIME_OFF_SET);
				BELL_NOR();
			}

		}
		else if(COOK_RUN_PAUSE == Sys.MState)
		{
			F_DispTaskChange(DISP_TIME_OFF_SET);
		}
	}
}



void KEY_Temp(void)
{
	if(SysMX_Run == SysMode) 
	{
		if(COOK_IDLE == Sys.MState)
		{
			F_DispTaskChange(DISP_TEMP_SET);
			BELL_NOR();
		}
		else if(COOK_RUN_PAUSE == Sys.MState)
		{
			F_DispTaskChange(DISP_TEMP_SET);
		}
	}
}

void KeyBaoWen(void)
{
	if(SysMX_Run == SysMode) 
	{
		if(COOK_IDLE == Sys.MState)
		{
			WorkModeNewSet(SET_M_BAOWEN);
		}
	}
}

void KeyDieZi(void)
{
	if(SysMX_Run == SysMode) 
	{
		if(COOK_IDLE == Sys.MState)
		{
			WorkModeNewSet(SET_M_DIE_ZI);
		}
	}
}


void KeyCup(void)
{
	if(SysMX_Run == SysMode) 
	{
		if(COOK_IDLE == Sys.MState)
		{
			WorkModeNewSet(SET_M_CUP);
		}
	}
}




