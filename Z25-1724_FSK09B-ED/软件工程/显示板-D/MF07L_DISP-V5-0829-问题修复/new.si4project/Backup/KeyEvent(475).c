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
	TimeFucRunStart();		//жижУМЦЪБ
}

void KeyPowFstOn(void)
{
	if(SysMX_Run == SysMode) 
	{
		if(Sys.MState != COOK_NONE)
		{
			WorkModeNewSet(SET_STATE_NONE);
			BELL_OFF();
		}
		else if(Sys.Sign.Err || Sys.PowMessage.DoorState)
		{
			//Пе
		}
		else if(Sys.Fg.Lock)
		{
			BELL_WARN();
		}
		else if(COOK_NONE == Sys.MState)
		{
			WorkModeNewSet(SET_FST_ON);
			BELL_ON();
		}



	}
}

void KeyPowToIDLE(void)
{
	if(SysMX_Factory == SysMode || SysMX_CustomerTest == SysMode) 
	{
		FactoryKeyNumFuc(2);
		BELL_NOR();
	}
	else if(Sys.Sign.Err || Sys.PowMessage.DoorState)
	{
		//Пе
	}
	else if(SysMX_Run == SysMode) 
	{
		if(COOK_RUN_PAUSE == Sys.MState)
		{
			WorkModeNewSet(SET_RET_IDLE);
			BELL_PAUSE();
		}
	}
}


void KEY_Lock(void)
{
	if(SysMX_Factory == SysMode || SysMX_CustomerTest == SysMode) 
	{
		FactoryKeyNumFuc(1);
		BELL_NOR();
	}
	else if(Sys.Sign.Err || Sys.PowMessage.DoorState)
	{
		//Пе
	}
	else if(SysMX_Run == SysMode) 
	{
		Sys.Fg.Lock ^= 1;
		BELL_NOR();	
	}
}

void KEY_FacTest(void)
{
	if(SysMX_Factory == SysMode || SysMX_CustomerTest == SysMode) 
	{
		FactoryKeyNumFuc(1);
		BELL_NOR();
	}
}






void KEY_StartPause(void)
{
	if(SysMX_Factory == SysMode || SysMX_CustomerTest == SysMode) 
	{
		FactoryKeyNumFuc(3);
		BELL_NOR();
	}
	else if(Sys.Sign.Err || Sys.PowMessage.DoorState)
	{
		//Пе
	}
	else if(SysMX_Run == SysMode) 
	{

		if(COOK_RUN_PAUSE == Sys.MState)
		{
			WorkModeNewSet(SET_RET_IDLE);
			BELL_PAUSE();
		}
		else if(Sys.Fg.Lock)
		{
			BELL_WARN();
		}
		else if(COOK_IDLE == Sys.MState)
		{
			if(DISP_TIME_OFF_SET == GetDispType())
			{
				Sys.Time = Sys.TimeBuf;
			}
			else if(0 == Sys.Time)
			{
				Sys.Time = MenuCookTimeDefTab[Sys.mMenu];
			}
			Sys.MState = COOK_RUN_PAUSE;
			Sys.Fg.HeatEn = TRUE;
			Sys.Sign.HadWork = TRUE;
			Sys.mLastWorkTempLevel = Sys.mTempLevel;
			KEY_EFF();
			F_DispTaskChange(DISP_NORMAL);
			BELL_START();
		}
	}
}


void KEY_Dec(void)
{
	if(SysMX_Factory == SysMode || SysMX_CustomerTest == SysMode) 
	{
		FactoryKeyNumFuc(5);
		BELL_NOR();
	}
	else if(Sys.Sign.Err || Sys.PowMessage.DoorState)
	{
		//Пе
	}
	else if((SysMX_Run == SysMode) && (COOK_IDLE == Sys.MState))
	{
		if(Sys.Fg.Lock)
		{
			BELL_WARN();
		}
		else if(DISP_TIME_OFF_SET == GetDispType())
		{
			if(Sys.TimeBuf > MenuCookTimeMinTab[Sys.mMenu])
			{
				Sys.TimeBuf  --;
			}
			Sys.u16TimeContDown = 0;
			KEY_EFF();
			F_DispTaskChange(DISP_TIME_OFF_SET);
			BELL_NOR();
		}
		else if(DISP_TEMP_SET == GetDispType())
		{
			if(Sys.mTempLevel > MenuCookTempMinTab[Sys.mMenu])
			{	Sys.mTempLevel --;}
			KEY_EFF();
			F_DispTaskChange(DISP_TEMP_SET);
			BELL_NOR();
		}
	}
}


void KEY_Inc(void)
{
	if(SysMX_Factory == SysMode || SysMX_CustomerTest == SysMode) 
	{
		FactoryKeyNumFuc(4);
		BELL_NOR();
	}
	else if(Sys.Sign.Err || Sys.PowMessage.DoorState)
	{
		//Пе
	}
	else if((SysMX_Run == SysMode) && (COOK_IDLE == Sys.MState))
	{
		if(Sys.Fg.Lock)
		{
			BELL_WARN();
		}
		else if(DISP_TIME_OFF_SET == GetDispType())
		{
			if(++Sys.TimeBuf > MenuCookTimeMaxTab[Sys.mMenu])
			{
				Sys.TimeBuf  = MenuCookTimeMaxTab[Sys.mMenu];

			}
			Sys.u16TimeContDown = 0;
			KEY_EFF();
			F_DispTaskChange(DISP_TIME_OFF_SET);
			BELL_NOR();
		}
		else if(DISP_TEMP_SET == GetDispType())
		{
			if(++Sys.mTempLevel > MenuCookTempMaxTab[Sys.mMenu])
			{	Sys.mTempLevel = MenuCookTempMaxTab[Sys.mMenu];}
			KEY_EFF(); 
			F_DispTaskChange(DISP_TEMP_SET);
			BELL_NOR();
		}
	}
}


void KEY_Time(void)
{
	if(SysMX_Factory == SysMode || SysMX_CustomerTest == SysMode) 
	{
		FactoryKeyNumFuc(6);
		BELL_NOR();
	}
	else if(Sys.Sign.Err || Sys.PowMessage.DoorState)
	{
		//Пе
	}
	else if(SysMX_Run == SysMode) 
	{
		if(Sys.Fg.Lock)
		{
			BELL_WARN();
		}	
		else if(COOK_IDLE == Sys.MState)
		{
			if((GetDispType() != DISP_TIME_OFF_SET))
			{
				if(0 == Sys.Time)
					Sys.TimeBuf = MenuCookTimeDefTab[Sys.mMenu];
				else
					Sys.TimeBuf = Sys.Time;	
				Sys.u16TimeContDown = 0;
				KEY_EFF();
				F_DispTaskChange(DISP_TIME_OFF_SET);
				BELL_NOR();
			}
			else if(GetDispType() == DISP_TIME_OFF_SET)
			{
				KEY_EFF();
				F_DispTaskChange(DISP_TIME_OFF_SET);
				BELL_NOR();
			}

		}
		else if(COOK_RUN_PAUSE == Sys.MState)
		{
			KEY_EFF();
			Sys.TimeBuf = Sys.Time;	
			F_DispTaskChange(DISP_TIME);
			BELL_NOR();
		}
	}
}



void KEY_Temp(void)
{
	if(SysMX_Factory == SysMode || SysMX_CustomerTest == SysMode) 
	{
		FactoryKeyNumFuc(7);
		BELL_NOR();
	}
	else if(Sys.Sign.Err || Sys.PowMessage.DoorState)
	{
		//Пе
	}
	else if(SysMX_Run == SysMode) 
	{
		if(Sys.Fg.Lock)
		{
			BELL_WARN();
		}	
		else if(COOK_IDLE == Sys.MState)
		{
			KEY_EFF();
			F_DispTaskChange(DISP_TEMP_SET);
			BELL_NOR();
		}
		else if(COOK_RUN_PAUSE == Sys.MState)
		{
			KEY_EFF();
			F_DispTaskChange(DISP_TEMP_SET);
			BELL_NOR();
		}
	}
}

void KeyBaoWen(void)
{
	if(SysMX_Factory == SysMode || SysMX_CustomerTest == SysMode) 
	{
		FactoryKeyNumFuc(8);
		BELL_NOR();
	}
	else if(Sys.Sign.Err || Sys.PowMessage.DoorState)
	{
		//Пе
	}
	else if(SysMX_Run == SysMode) 
	{
		if(Sys.Fg.Lock)
		{
			BELL_WARN();
		}	
		else if(COOK_IDLE == Sys.MState)
		{
			WorkModeNewSet(SET_M_BAOWEN);
			BELL_NOR();
		}
	}
}

void KeyDieZi(void)
{
	if(SysMX_Factory == SysMode || SysMX_CustomerTest == SysMode) 
	{
		FactoryKeyNumFuc(9);
		BELL_NOR();
	}
	else if(Sys.Sign.Err || Sys.PowMessage.DoorState)
	{
		//Пе
	}
	else if(SysMX_Run == SysMode) 
	{
		if(Sys.Fg.Lock)
		{
			BELL_WARN();
		}	
		else if(COOK_IDLE == Sys.MState)
		{
			WorkModeNewSet(SET_M_DIE_ZI);
			BELL_NOR();
		}
	}
}


void KeyCup(void)
{
	if(SysMX_Factory == SysMode || SysMX_CustomerTest == SysMode) 
	{
		SysMode = SysMX_Run;
		WorkModeNewSet(SET_STATE_NONE);
		BELL_NOR();
	}
	else if(Sys.Sign.Err || Sys.PowMessage.DoorState)
	{
		//Пе
	}
	else if(SysMX_Run == SysMode) 
	{
		if(Sys.Fg.Lock)
		{
			BELL_WARN();
		}	
		else if(COOK_IDLE == Sys.MState)
		{
			WorkModeNewSet(SET_M_CUP);
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





