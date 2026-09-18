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
	*((u8 *)&Sys.Sign) = 0;
	Sys.Err = 0;
	Sys.IDLE_Sec = 0; 
	Sys.HeatSec = 0;
	DriveMenuReset();
	switch (mNewDat)
	{
	case SET_STATE_NONE:
		Sys.CookLevel = 0;
		Sys.MState = COOK_NONE;		
		break;
	case SET_FST_ON:
		Sys.MState = COOK_IDLE;
		Sys.mMenu = HUOGUO;
		Sys.CookLevel = MenuLevelDefTab[Sys.mMenu];
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
	TimeFucRunStart();		//重置计时
}





u8 IncDecU32Fuc(Col_Enum Col,SelectSet_Enum Object,u16 StepVal,u32 MaxVal,u32 Minval)
{
	u8 u8BuzzEn = TRUE;
	if(SET_BOOK_TIME == Object)
	{
		if(CODE_DEC == Col)
		{
			if(Minval == Sys.BookRunSec)
				u8BuzzEn = FALSE;	
			if(Sys.BookRunSec >= StepVal)
			{
				Sys.BookRunSec -= StepVal;
			}
			else
			{

				Sys.BookRunSec = Minval; 	
			}
			if(Sys.BookRunSec < Minval)
				Sys.BookRunSec = Minval; 			
		}
		else
		{
			if(MaxVal == Sys.BookRunSec)
				u8BuzzEn = FALSE;	

			Sys.BookRunSec += StepVal;
			if(Sys.BookRunSec > MaxVal)
			{
				Sys.BookRunSec = MaxVal; 
			}	
		}
		if(Sys.BookRunSec != Minval) 
			Sys.BookRunSec -= Sys.BookRunSec%StepVal;
	}
	else
	{
		if(CODE_DEC == Col)
		{
			if(Sys.CookRunSec >= StepVal)
			{
				Sys.CookRunSec -= StepVal;
			}
			else 
			{
				if(Minval == Sys.CookRunSec)
					u8BuzzEn = FALSE;	
				Sys.CookRunSec = Minval; 	
			}
			if(Sys.CookRunSec < Minval)
				Sys.CookRunSec = Minval;				
		}
		else
		{
			if(MaxVal == Sys.CookRunSec)
				u8BuzzEn = FALSE;	

			Sys.CookRunSec += StepVal;
			if(Sys.CookRunSec > MaxVal)
			{
				Sys.CookRunSec = MaxVal; 
			}	
		}
	}
	return u8BuzzEn;
}

u8 IncDecU16Fuc(Col_Enum Col,u16 MaxVal,u16 Minval)
{
	u8 xdata u8Buf = FALSE;
	if(CODE_DEC == Col)
	{
		if(Sys.CookLevel > Minval)
		{
			Sys.CookLevel --;
			u8Buf = TRUE;
		}
	}
	else
	{
		if(Sys.CookLevel < MaxVal)
		{
			Sys.CookLevel ++;
			u8Buf = TRUE;
		}
		else
			Sys.CookLevel = MaxVal; 
	}
	return u8Buf;
}


void KeyInc(void)
{
	u8 SetEn = TRUE;
	u8 u8BuzzEn = FALSE;
	u32 u32Buf = 0;
	FactoryKeyNumFuc(4);
	if(SysMX_Run == SysMode)
	{
		if((COOK_IDLE == Sys.MState) || (COOK_RUN_PAUSE == Sys.MState))
		{
			if((COOK_RUN_PAUSE == Sys.MState) && (!Sys.Fg.HeatEn) && (Sys.Fg.BooktimeEn))
				return;
			if(((MENGZHU == Sys.mMenu) || (BAOTANG == Sys.mMenu) || (MANDUN == Sys.mMenu)) && GetDispType() != DISP_SET_BOOK_TIME)
			{
				SetEn = FALSE;
				if(!Sys.Fg.KeepWarmEn) //(COOK_RUN_PAUSE == Sys.MState) && 
				{
					Sys.Fg.CooktimeEn = TRUE;
					F_DispTaskChange(DISP_SET_COOK_TIME);
				}
			}

			if(DISP_SET_BOOK_TIME == GetDispType())
			{	
				if(Sys.Fg.CooktimeEn)
				{
					u32Buf = Sys.CookRunSec;	//有定时,预约最小值按当前定时的时间
					if(Sys.CookRunSec < MenuBookTimeMinTab[Sys.mMenu])
						u32Buf = MenuBookTimeMinTab[Sys.mMenu]; 	
				}
				else
					u32Buf = MenuBookTimeMinTab[Sys.mMenu];
				u8BuzzEn = IncDecU32Fuc(CODE_INC,SET_BOOK_TIME,STEP_BT_S,MenuBookTimeMaxTab[Sys.mMenu],u32Buf);
				F_DispTaskChange(DISP_SET_BOOK_TIME);
				//u8BuzzEn = TRUE;
			}
			else if(DISP_SET_COOK_TIME == GetDispType())
			{
				if(Sys.Fg.BooktimeEn)
				{
					u32Buf = Sys.BookRunSec;	//有预约,定时最大值按当前预约的时间
					if(Sys.BookRunSec > MenuCookTimeMaxTab[Sys.mMenu])
						u32Buf = MenuCookTimeMaxTab[Sys.mMenu]; 	
				}
				else
					u32Buf = MenuCookTimeMaxTab[Sys.mMenu];
				u8BuzzEn = IncDecU32Fuc(CODE_INC,SET_COOK_TIME,STEP_CT_S,u32Buf,MenuCookTimeMinTab[Sys.mMenu]);
				F_DispTaskChange(DISP_SET_COOK_TIME);
				//u8BuzzEn = TRUE;
			}
			else if(SetEn)
			{	
				u8BuzzEn = IncDecU16Fuc(CODE_INC,MenuLevelMaxTab[Sys.mMenu],POW_P1);
				//if(Sys.Fg.TimekeepingEn || Sys.Fg.BooktimeEn || Sys.Fg.CooktimeEn || (COOK_RUN_PAUSE == Sys.MState))
					F_DispTaskChange(DISP_SET_POW);
			}
			Sys.IDLE_Sec = 0; 
			if(u8BuzzEn)
			{
				BUZZ();
			}
		}
	}
	else if(SysMX_Adjust == SysMode)
	{
		BUZZ();
		Sys.ComSlopeCurr_Adjust++;			//电流斜率调整值-1（功率减小）
	}
}

void KeyLongInc(void)
{
	u8 SetEn = TRUE;
	u32 u32Buf = 0;
	if(SysMX_Run == SysMode)
	{
		if((COOK_IDLE == Sys.MState) || (COOK_RUN_PAUSE == Sys.MState))
		{
			if((COOK_RUN_PAUSE == Sys.MState) && (!Sys.Fg.HeatEn) && (Sys.Fg.BooktimeEn))
				return;
			if(((MENGZHU == Sys.mMenu) || (BAOTANG == Sys.mMenu) || (MANDUN == Sys.mMenu)) && GetDispType() != DISP_SET_BOOK_TIME)
			{
				SetEn = FALSE;
				if( !Sys.Fg.KeepWarmEn) //(COOK_RUN_PAUSE == Sys.MState) &&
				{
					Sys.Fg.CooktimeEn = TRUE;
					F_DispTaskChange(DISP_SET_COOK_TIME);
				}
			}

			if(DISP_SET_BOOK_TIME == GetDispType())
			{
				if(Sys.Fg.CooktimeEn)
				{
					u32Buf = Sys.CookRunSec;	//有定时,预约最小值按当前定时的时间
					if(Sys.CookRunSec < MenuBookTimeMinTab[Sys.mMenu])
						u32Buf = MenuBookTimeMinTab[Sys.mMenu]; 	
				}
				else
					u32Buf = MenuBookTimeMinTab[Sys.mMenu];
				IncDecU32Fuc(CODE_INC,SET_BOOK_TIME,STEP_BT_L,MenuBookTimeMaxTab[Sys.mMenu],u32Buf);
				F_DispTaskChange(DISP_SET_BOOK_TIME);
			}
			else if(DISP_SET_COOK_TIME == GetDispType())
			{
				if(Sys.Fg.BooktimeEn)
				{
					u32Buf = Sys.BookRunSec;	//有预约,定时最大值按当前预约的时间
					if(Sys.BookRunSec > MenuCookTimeMaxTab[Sys.mMenu])
						u32Buf = MenuCookTimeMaxTab[Sys.mMenu]; 	
				}
				else
					u32Buf = MenuCookTimeMaxTab[Sys.mMenu];

				IncDecU32Fuc(CODE_INC,SET_COOK_TIME,STEP_CT_L,u32Buf,MenuCookTimeMinTab[Sys.mMenu]);
				F_DispTaskChange(DISP_SET_COOK_TIME);
			}
			else if(SetEn)
			{	
				IncDecU16Fuc(CODE_INC,MenuLevelMaxTab[Sys.mMenu],POW_P1);
				//if(Sys.Fg.TimekeepingEn || Sys.Fg.BooktimeEn || Sys.Fg.CooktimeEn || (COOK_RUN_PAUSE == Sys.MState))
					F_DispTaskChange(DISP_SET_POW);

			}
			Sys.IDLE_Sec = 0; 
			//BUZZ();
		}
	}
}


void KeyDec(void)
{
	u8 SetEn = TRUE;
	u8 u8BuzzEn = FALSE;
	u32 u32Buf = 0;
	FactoryKeyNumFuc(3);
	if((SysMX_Run == SysMode))
	{
		if((COOK_IDLE == Sys.MState) || (COOK_RUN_PAUSE == Sys.MState))
		{
			if((COOK_RUN_PAUSE == Sys.MState) && (!Sys.Fg.HeatEn) && (Sys.Fg.BooktimeEn))
				return;
			if(((MENGZHU == Sys.mMenu) || (BAOTANG == Sys.mMenu) || (MANDUN == Sys.mMenu)) && GetDispType() != DISP_SET_BOOK_TIME)
			{
				SetEn = FALSE;
				if( !Sys.Fg.KeepWarmEn) //(COOK_RUN_PAUSE == Sys.MState) &&
				{
					Sys.Fg.CooktimeEn = TRUE;
					F_DispTaskChange(DISP_SET_COOK_TIME);
				}
			}
			if(DISP_SET_BOOK_TIME == GetDispType())
			{	
				if(Sys.Fg.CooktimeEn)
				{
					u32Buf = Sys.CookRunSec;	//有定时,预约最小值按当前定时的时间
					if(Sys.CookRunSec < MenuBookTimeMinTab[Sys.mMenu])
						u32Buf = MenuBookTimeMinTab[Sys.mMenu]; 	
				}
				else
					u32Buf = MenuBookTimeMinTab[Sys.mMenu];
				u8BuzzEn = IncDecU32Fuc(CODE_DEC,SET_BOOK_TIME,STEP_BT_S,MenuBookTimeMaxTab[Sys.mMenu],u32Buf);
				F_DispTaskChange(DISP_SET_BOOK_TIME);
				//u8BuzzEn = TRUE;
			}
			else if(DISP_SET_COOK_TIME == GetDispType())
			{
				if(Sys.Fg.BooktimeEn)
				{
					u32Buf = Sys.BookRunSec;	//有预约,定时最大值按当前预约的时间
					if(Sys.BookRunSec > MenuCookTimeMaxTab[Sys.mMenu])
						u32Buf = MenuCookTimeMaxTab[Sys.mMenu]; 	
				}
				else
					u32Buf = MenuCookTimeMaxTab[Sys.mMenu];

				u8BuzzEn = IncDecU32Fuc(CODE_DEC,SET_COOK_TIME,STEP_CT_S,u32Buf,MenuCookTimeMinTab[Sys.mMenu]);
				F_DispTaskChange(DISP_SET_COOK_TIME);
				//u8BuzzEn = TRUE;
			}
			else if(SetEn)
			{	
				u8BuzzEn = IncDecU16Fuc(CODE_DEC,MenuLevelMaxTab[Sys.mMenu],POW_P1);
				//if(Sys.Fg.TimekeepingEn || Sys.Fg.BooktimeEn || Sys.Fg.CooktimeEn || (COOK_RUN_PAUSE == Sys.MState))
					F_DispTaskChange(DISP_SET_POW);
			}
			Sys.IDLE_Sec = 0; 
			if(u8BuzzEn)
			{
				BUZZ();
			}
		}
	}
	else if(SysMX_Adjust == SysMode)
	{
		BUZZ();
		Sys.ComSlopeCurr_Adjust--;			//电流斜率调整值-1（功率增加）
	}

}

void KeyLongDec(void)
{
	u8 SetEn = TRUE;
	u32 u32Buf = 0;
	if((SysMX_Run == SysMode))
	{
		if((COOK_IDLE == Sys.MState) || (COOK_RUN_PAUSE == Sys.MState))
		{
			if((COOK_RUN_PAUSE == Sys.MState) && (!Sys.Fg.HeatEn) && (Sys.Fg.BooktimeEn))
				return;
			if(((MENGZHU == Sys.mMenu) || (BAOTANG == Sys.mMenu) || (MANDUN == Sys.mMenu)) && GetDispType() != DISP_SET_BOOK_TIME)
			{
				SetEn = FALSE;
				if( !Sys.Fg.KeepWarmEn) //(COOK_RUN_PAUSE == Sys.MState) &&
				{
					Sys.Fg.CooktimeEn = TRUE;
					F_DispTaskChange(DISP_SET_COOK_TIME);
				}
			}
			if(DISP_SET_BOOK_TIME == GetDispType())
			{
				if(Sys.Fg.CooktimeEn)
				{
					u32Buf = Sys.CookRunSec;	//有定时,预约最小值按当前定时的时间
					if(Sys.CookRunSec < MenuBookTimeMinTab[Sys.mMenu])
						u32Buf = MenuBookTimeMinTab[Sys.mMenu]; 	
				}
				else
					u32Buf = MenuBookTimeMinTab[Sys.mMenu];
				IncDecU32Fuc(CODE_DEC,SET_BOOK_TIME,STEP_BT_L,MenuBookTimeMaxTab[Sys.mMenu],u32Buf);
				F_DispTaskChange(DISP_SET_BOOK_TIME);
			}
			else if(DISP_SET_COOK_TIME == GetDispType())
			{
				if(Sys.Fg.BooktimeEn)
				{
					u32Buf = Sys.BookRunSec;	//有预约,定时最大值按当前预约的时间
					if(Sys.BookRunSec > MenuCookTimeMaxTab[Sys.mMenu])
						u32Buf = MenuCookTimeMaxTab[Sys.mMenu]; 	
				}
				else
					u32Buf = MenuCookTimeMaxTab[Sys.mMenu];

				IncDecU32Fuc(CODE_DEC,SET_COOK_TIME,STEP_CT_L,u32Buf,MenuCookTimeMinTab[Sys.mMenu]);
				F_DispTaskChange(DISP_SET_COOK_TIME);	
			}
			else if(SetEn)
			{	
				IncDecU16Fuc(CODE_DEC,MenuLevelMaxTab[Sys.mMenu],POW_P1);
				//if(Sys.Fg.TimekeepingEn || Sys.Fg.BooktimeEn || Sys.Fg.CooktimeEn || (COOK_RUN_PAUSE == Sys.MState))
					F_DispTaskChange(DISP_SET_POW);

			}
			Sys.IDLE_Sec = 0; 
			//BUZZ();
		}
	}
}


void KeyTimekeeping(void)
{
	FactoryKeyNumFuc(1);

	if((SysMX_Run == SysMode) && (Sys.MState == COOK_RUN_PAUSE) && (HUOGUO == Sys.mMenu || JIANKAO == Sys.mMenu || ZHENGZHU == Sys.mMenu))
	{
		if((COOK_RUN_PAUSE == Sys.MState) && (!Sys.Fg.HeatEn) ) //&& (Sys.Fg.BooktimeEn)
			return;

		if(Sys.Fg.TimekeepingEn)
		{
			Sys.Fg.TimekeepingEn = FALSE;
			F_DispTaskChange(DISP_NORMAL);
			
		}
		else 
		{
			Sys.Fg.TimekeepingEn = TRUE;
			Sys.TKeepSec = 0;
			DispRunSec = 0;
			F_DispTaskChange(DISP_NORMAL);
			TimeFucRunStart();
		}
		BUZZ();
		Sys.IDLE_Sec = 0; 
	}	
}

void KeyTimekeepingOFF(void)
{
	if((SysMX_Run == SysMode) && (Sys.MState != COOK_NONE) && (HUOGUO == Sys.mMenu || JIANKAO == Sys.mMenu))
	{
		Sys.Fg.TimekeepingEn = FALSE;
		F_DispTaskChange(DISP_NORMAL);
		TimeFucRunStart();
		BUZZ();
		Sys.IDLE_Sec = 0; 
	}	
}




void KeyBooktime(void)
{
	u32 u32Buf;
	if((SysMX_Run == SysMode) && (Sys.MState == COOK_IDLE) && (Sys.mMenu != HUOGUO) && (Sys.mMenu != JIANKAO))
	{
		if((GetDispType() != DISP_SET_BOOK_TIME))
		{
			if(FALSE == Sys.Fg.BooktimeEn)
			{
				u32Buf = MenuBookTimeDefTab[Sys.mMenu]; 		
				if(Sys.Fg.CooktimeEn)
				{
					if(Sys.CookRunSec > MenuBookTimeDefTab[Sys.mMenu])
						u32Buf = Sys.CookRunSec;	//有定时,定时较大时，预约按这个最小时间	
				}
				Sys.BookRunSec = u32Buf;
			}
			F_DispTaskChange(DISP_SET_BOOK_TIME);
			//Sys.Fg.BooktimeEn = TRUE;
		}
		else 
		{
			Sys.Fg.BooktimeEn = TRUE;
			F_DispTaskChange(DISP_NORMAL);
		}
		BUZZ();
		Sys.IDLE_Sec = 0; 
	}	
}


void KeyCooktime(void)
{
	u8 u8MenuCookEn = FALSE;
	u32 u32Buf;
	FactoryKeyNumFuc(2);
	if((HUOGUO == Sys.mMenu) || (ZHENGZHU == Sys.mMenu) || (JIANKAO == Sys.mMenu)) 
		u8MenuCookEn = TRUE;
	if(GetDispType() == DISP_SET_BOOK_TIME) 
	{
		Sys.Fg.BooktimeEn = TRUE;
		F_DispTaskChange(DISP_NORMAL);
		BUZZ();
	}
	else if(GetDispType() == DISP_SET_COOK_TIME) 
	{
		Sys.Fg.CooktimeEn = TRUE;
		if(0 == Sys.CookRunSec)
			Sys.Fg.CooktimeEn = FALSE;

		F_DispTaskChange(DISP_NORMAL);
		if((MENGZHU != Sys.mMenu) && (BAOTANG != Sys.mMenu) && (MANDUN != Sys.mMenu))
			BUZZ();
	}
	else if((SysMX_Run == SysMode) && ((COOK_IDLE == Sys.MState) || (COOK_RUN_PAUSE == Sys.MState))   && u8MenuCookEn && !Sys.Fg.TimekeepingEn)
	{
		if((COOK_RUN_PAUSE == Sys.MState) && (!Sys.Fg.HeatEn) ) //&& (Sys.Fg.BooktimeEn) 暂停也不能设置时间
			return;
		
		if((GetDispType() != DISP_SET_COOK_TIME))
		{
			if(FALSE == Sys.Fg.CooktimeEn)
			{
				u32Buf = MenuCookTimeDefTab[Sys.mMenu]; 		
				if(Sys.Fg.BooktimeEn)
				{
					if(Sys.BookRunSec < MenuCookTimeDefTab[Sys.mMenu])
						u32Buf = Sys.BookRunSec;	//有定时,定时较大时，预约按这个最小时间	
				}
				Sys.CookRunSec = u32Buf;
			}
			F_DispTaskChange(DISP_SET_COOK_TIME);
			Sys.EppromCookRunMin = Sys.CookRunSec / 60;
			if(Sys.CookRunSec % 60)
				Sys.EppromCookRunMin ++;
			//Sys.Fg.CooktimeEn = TRUE;
		}
		else
		{
			if(Sys.CookRunSec == 0)
				Sys.Fg.CooktimeEn = FALSE;
			F_DispTaskChange(DISP_NORMAL);
		}
		BUZZ();
		
		Sys.IDLE_Sec = 0; 
	}	
}




void KeyMenuSelect(void)
{
	FactoryKeyNumFuc(5);
	if((SysMX_Run == SysMode) && (COOK_IDLE == Sys.MState)) // && (FALSE == Sys.Fg.MenuSure)
	{
		if(++Sys.mMenu > MANDUN)
			Sys.mMenu = HUOGUO;
		Sys.Fg.BooktimeEn = FALSE;
		Sys.Fg.CooktimeEn = FALSE;
		Sys.Fg.TimekeepingEn = FALSE;
		if((MENGZHU == Sys.mMenu) || (BAOTANG == Sys.mMenu) || (MANDUN == Sys.mMenu))
		{
			Sys.Fg.CooktimeEn = TRUE;
			Sys.CookRunSec = MenuCookTimeDefTab[Sys.mMenu];
		}
		Sys.CookLevel=  MenuLevelDefTab[Sys.mMenu];
		BUZZ();
		F_DispTaskChange(DISP_NORMAL);
		Sys.IDLE_Sec = 0; 
		DriveMenuReset();
	}	
}


void KeyPauseStart(void)
{
	

	FactoryKeyNumFuc(6);
	if(SysMX_Run == SysMode)
	{
		if(COOK_NONE == Sys.MState)
		{
			#ifdef _TO_CUSTOM_LIMIT_
			#message "----------------------测试用程序，限制开关次数-------------------------"
			KeyOnCnt++;
			if(KeyOnCnt < KEY_LIMIT)
			#endif
				WorkModeNewSet(SET_FST_ON);
		}
		else if(COOK_IDLE == Sys.MState)
		{
			//Sys.Fg.MenuSure = TRUE;
			Sys.MState = COOK_RUN_PAUSE;
			if(DISP_SET_COOK_TIME == GetDispType())
			{
				 Sys.Fg.CooktimeEn = TRUE;
				if(0 == Sys.CookRunSec)
					Sys.Fg.CooktimeEn = FALSE;
			}
			if(FALSE == Sys.Fg.CooktimeEn)
			{
				if((HUOGUO == Sys.mMenu) || (ZHENGZHU == Sys.mMenu) || (JIANKAO == Sys.mMenu))
				{
					Sys.Fg.CooktimeEn = TRUE;
					Sys.CookRunSec = MenuCookTimeDefTab[Sys.mMenu];
				}	
			}
			if(DISP_SET_BOOK_TIME == GetDispType())
			{
				Sys.Fg.BooktimeEn = TRUE;
			}
			if(Sys.Fg.BooktimeEn && Sys.Fg.CooktimeEn)
			{
				if(Sys.CookRunSec >= Sys.BookRunSec)
				{
					Sys.Fg.BooktimeEn = FALSE;
				}
			}
			if(FALSE == Sys.Fg.BooktimeEn)
				Sys.Fg.HeatEn = TRUE;
			TimeFucRunStart();
			F_DispTaskChange(DISP_NORMAL);
		}
		else if(COOK_RUN_PAUSE == Sys.MState)
		{
			if(Sys.Fg.BooktimeEn)
				return;			//只能长按退出
			/*
			if(Sys.Fg.BooktimeEn)
			{
				Sys.Fg.BooktimeEn = FALSE;
				Sys.Fg.HeatEn = TRUE;
			}
			else
			*/
				Sys.Fg.HeatEn = !Sys.Fg.HeatEn;
			Sys.Fg.TimekeepingEn = FALSE;
			F_DispTaskChange(DISP_NORMAL);
		}
		Sys.IDLE_Sec = 0; 
		BUZZ();
	}
}

void KeyPowOFF(void)
{
	if((SysMX_Run == SysMode) && (Sys.MState != COOK_NONE))
	{
		if(COOK_IDLE == Sys.MState)
		{
			if(Sys.Fg.CooktimeEn || Sys.Fg.BooktimeEn)
			{
				if((MENGZHU != Sys.mMenu) && (BAOTANG != Sys.mMenu) && (MANDUN != Sys.mMenu))
					Sys.Fg.CooktimeEn = FALSE;
				Sys.Fg.BooktimeEn = FALSE;
				BUZZ();
			}
			return;	//无反应
		}
		else if(COOK_RUN_PAUSE == Sys.MState)
		{
			//当前模式返回设置状态
			WorkModeNewSet(SET_NOW_MENU_RESET);
		}
		BUZZ();
	}

}

void KeyAdjust(void)
{
	if(g_u8PowerON_TCnt)
	{
		FactoryStep = 0;
		FactoryMode_Sec = 0;
		SysMode = SysMX_Adjust;
		BUZZ();
	}
}


void KeyFactory(void)
{
	//if((SYS_IDLE == SysInfo.SysMode) && (FALSE == Up.BoxStatus.OpenClose)) //门打开的状态 
	{
	//	if(COOK_IDLE == Up.MState)
		{
	//		if((SET_NONE == Clock.SetM) && (MODE_NONE == Up.Mode))
			{
				if(g_u8PowerON_TCnt)
				{
					SetFactoryMode();
					BUZZ();
				}
			}
		}
	}
}



