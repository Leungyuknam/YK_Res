#define _Native_Display_H
#include "Include\Include.h"
#include <math.h>

//unsigned int xdata PWM31_REG _at_ 0x103a;	//PWM占空比调节寄存器

u16 code SEG_NUM[] ={   
 DISP_0,DISP_1,DISP_2,DISP_3,DISP_4,DISP_5,DISP_6,DISP_7,DISP_8,DISP_9,DISP_A,DISP_b,DISP_C,DISP_d,DISP_E,DISP_F
};


#define	PSEG1	0x20
#define	PSEG2	0x10
#define	PSEG3	0x08
#define	PSEG4	0x04
#define	PSEG5	0x100
#define	PSEG6	0x80
#define	PSEG7	0x40
#define	PSEG8	0x02
#define	PSEG9	0x01

	//g_u16DispSegBuf[0] = 0xffff;  //C6
	//g_u16DispSegBuf[1] = 0xffff;  //C7
	//g_u16DispSegBuf[2] = 0xffff;  //符号
	//g_u16DispSegBuf[3] = 0xffff;  //第四个8
	//g_u16DispSegBuf[4] = 0xffff; //第一个八
	//g_u16DispSegBuf[5] = 0xffff; //第二个八
	//g_u16DispSegBuf[6] = 0xffff; //第三个八

#define	PCOM1	4	
#define	PCOM2	5
#define PCOM3	6
#define PCOM4	3
#define	PCOM5	2	
#define	PCOM6	0
#define PCOM7	1


//enum LED_TASK_NUM
//{
//	L_K_SHORTCUT_TIME,
//	L_K_BOOK_TIME,
//	L_K_DEC,
//	L_K_INC,
//	L_K_MENU,
//	L_K_START_PAUSE,
//	
//	L_BOOK_SIGN,
//	L_W_SIGN,
//	L_RESIDUE_SIGN,
//	L_MIN_SIGN,
//	L_COLON_SIGN,
//	_LED_TOTAL_,
//};

//L_BOOK_SIGN,		5
//L_W_SIGN,			2	
//L_RESIDUE_SIGN, 	6
//L_MIN_SIGN, 		3
//L_COLON_SIGN,		1

//code LedTask_t LedTask[_LED_TOTAL_] =
//{
//	//Com,		Val
//	{PCOM6,		PSEG2}, 			//L_K_SHORTCUT_TIME
//	{PCOM7,		PSEG2}, 			//L_K_BOOK_TIME
//	{PCOM7,		PSEG3}, 			//L_K_DEC
//	{PCOM7,		PSEG1}, 			//L_K_INC
//	{PCOM6,		PSEG1}, 			//L_K_MENU
//	{PCOM7,		PSEG9}, 			//L_K_START_PAUSE   <<
//	
//	{PCOM5,		BIT5}, 				//L_BOOK_SIGN
//	{PCOM5,		BIT2}, 				//L_W_SIGN
//	{PCOM5,		BIT4}, 				//L_RESIDUE_SIGN
//	{PCOM5,		PSEG5}, 			//L_MIN_SIGN
//	{PCOM5,		BIT3}, 				//L_COLON_SIGN

//};

#define	L_K_SHORTCUT_TIME()	{g_u16DispSegBuf[PCOM6] |= PSEG2;}
#define	L_K_BOOK_TIME()		{g_u16DispSegBuf[PCOM7] |= PSEG2;}
#define	L_K_DEC()			{g_u16DispSegBuf[PCOM7] |= PSEG3;}
#define	L_K_INC()			{g_u16DispSegBuf[PCOM7] |= PSEG1;}
#define	L_K_MENU()			{g_u16DispSegBuf[PCOM6] |= PSEG1;}
#define	L_K_START_PAUSE()	{g_u16DispSegBuf[PCOM7] |= PSEG9;}

#define	L_K_ALL()			{g_u16DispSegBuf[PCOM7] |= PSEG2+PSEG3+PSEG1+PSEG9;g_u16DispSegBuf[PCOM6] |= PSEG2+PSEG1;}


#define	L_BOOK_SIGN()		{g_u16DispSegBuf[PCOM5] |= BIT5;}
#define	L_W_SIGN()			{g_u16DispSegBuf[PCOM5] |= BIT2;}
#define	L_RESIDUE_SIGN()	{g_u16DispSegBuf[PCOM5] |= BIT4;}
#define	L_MIN_SIGN()		{g_u16DispSegBuf[PCOM5] |= PSEG5;}
#define	L_COLON_SIGN()		{g_u16DispSegBuf[PCOM5] |= BIT3;}


#define	L_K_SHORTCUT_TIME_OFF()	{g_u16DispSegBuf[PCOM6] &= ~PSEG2;}
#define	L_K_BOOK_TIME_OFF()		{g_u16DispSegBuf[PCOM7] &= ~PSEG2;}
#define	L_K_DEC_OFF()			{g_u16DispSegBuf[PCOM7] &= ~PSEG3;}
#define	L_K_INC_OFF()			{g_u16DispSegBuf[PCOM7] &= ~PSEG1;}
#define	L_START_ELSE_OFF()	{g_u16DispSegBuf[PCOM6] &= ~(PSEG1 + PSEG2);g_u16DispSegBuf[PCOM7] &= ~(PSEG1 + PSEG2 + PSEG3);}



#define	DS_MENU1()	{g_u16DispSegBuf[PCOM6] |= PSEG4+PSEG3;}
#define	DS_MENU2()	{g_u16DispSegBuf[PCOM6] |= PSEG8;g_u16DispSegBuf[PCOM7] |= PSEG4;}
#define	DS_MENU3()	{g_u16DispSegBuf[PCOM6] |= PSEG9;g_u16DispSegBuf[PCOM7] |= PSEG8;}  
#define	DS_MENU4()	{g_u16DispSegBuf[PCOM6] |= PSEG7;g_u16DispSegBuf[PCOM7] |= PSEG7;}  
#define	DS_MENU5()	{g_u16DispSegBuf[PCOM6] |= PSEG6;g_u16DispSegBuf[PCOM7] |= PSEG6;}  
#define	DS_MENU6()	{g_u16DispSegBuf[PCOM6] |= PSEG5;g_u16DispSegBuf[PCOM7] |= PSEG5;} 

#define	DS_ALL_MENU_AND_KEY()	{g_u16DispSegBuf[PCOM6] |= PSEG4+PSEG3+PSEG5+PSEG6+PSEG7+PSEG8+PSEG9;\
						 g_u16DispSegBuf[PCOM7] |= PSEG4+PSEG5+PSEG6+PSEG7+PSEG8;}


#define	DS_MENU1_OFF()	{g_u16DispSegBuf[PCOM6] &= ~(PSEG4+PSEG3);}
#define	DS_MENU2_OFF()	{g_u16DispSegBuf[PCOM6] &= ~PSEG8;g_u16DispSegBuf[PCOM7] &= ~PSEG4;}
#define	DS_MENU3_OFF()	{g_u16DispSegBuf[PCOM6] &= ~PSEG9;g_u16DispSegBuf[PCOM7] &= ~PSEG8;} 
#define	DS_MENU4_OFF()	{g_u16DispSegBuf[PCOM6] &= ~PSEG7;g_u16DispSegBuf[PCOM7] &= ~PSEG7;}
#define	DS_MENU5_OFF()	{g_u16DispSegBuf[PCOM6] &= ~PSEG6;g_u16DispSegBuf[PCOM7] &= ~PSEG6;}
#define	DS_MENU6_OFF()	{g_u16DispSegBuf[PCOM6] &= ~PSEG5;g_u16DispSegBuf[PCOM7] &= ~PSEG5;}


extern code DispTask_t DispTask[_DISP_TOTAL_];

/*******************************************************************************************************/



/********************************************************************************************************
* @ name   : static void F_LedEnable(unsigned char temp)
* @ msg    : LED显示
* @ param   (unsigned char) temp
* @ return  (*)
 ********************************************************************************************************/
//static void F_LedEnable(unsigned char temp)
//{
//	if (temp < _LED_TOTAL_)
//	{
//		g_u16DispSegBuf[LedTask[temp].Com] |= LedTask[temp].Val;
//	}
//}

/*
static void F_LedOFF(unsigned char temp)
{
	if (temp < _LED_TOTAL_)
	{
		g_u16DispSegBuf[LedTask[temp].Com] &= ~LedTask[temp].Val;
	}
}
*/






static void F_DispLed(void)
{
	if(COOK_NONE == Sys.MState)
	{
		if(DispFg.Bri)
			L_K_START_PAUSE();	
	}
	else if(COOK_IDLE == Sys.MState)
	{
		if(MENU_NONE == Sys.mMenu)
		{
			if(DispFg.Bri)
				L_K_MENU();
			return;
			
		}	

		L_K_START_PAUSE();	
		//if(FALSE == Sys.Fg.MenuSure)
		{
			L_K_MENU();	
			L_K_INC();	
			L_K_DEC();	
			DS_MENU1();
			DS_MENU2();
			DS_MENU3();
			DS_MENU4();
			DS_MENU5();
			DS_MENU6();
			L_K_BOOK_TIME(); 
			switch(Sys.mMenu)
			{
				case HUOGUO:
					if(FALSE == DispFg.Bri)
						DS_MENU1_OFF();
					//L_K_SHORTCUT_TIME(); 
					//不可预约
				break;
				case ZHENGZHU:
					if(FALSE == DispFg.Bri)
						DS_MENU2_OFF();
					//L_K_BOOK_TIME(); 
					//不可一键计时
				break;
				case MENGZHU:
					if(FALSE == DispFg.Bri)
						DS_MENU3_OFF();
					//L_K_BOOK_TIME(); 
					//不可一键计时
				break;
				case JIANKAO:
					//L_K_BOOK_TIME_OFF();
					if(FALSE == DispFg.Bri)
						DS_MENU4_OFF();
					//L_K_SHORTCUT_TIME(); 
					//不可预约
				break;
				case BAOTANG:
					if(FALSE == DispFg.Bri)
						DS_MENU5_OFF();
					//L_K_BOOK_TIME(); 
					//不可一键计时
				break;
				case MANDUN:
					if(FALSE == DispFg.Bri)
						DS_MENU6_OFF();
					//L_K_BOOK_TIME(); 
					//不可一键计时
				break;
				default:

				break;
			}
		}
		/*
		else
		{
			//功能选择按键不用显示
			L_K_INC();	
			L_K_DEC();
			//L_K_BOOK_TIME();
			switch(Sys.mMenu)
			{
				case HUOGUO:
					DS_MENU1();
					//L_K_SHORTCUT_TIME(); 
				break;
				case ZHENGZHU:
					DS_MENU2();
				break;
				case MENGZHU:
					DS_MENU3();
				break;
				case JIANKAO:
					L_K_BOOK_TIME_OFF();
					DS_MENU4();
					//L_K_SHORTCUT_TIME(); 
				break;
				case BAOTANG:
					DS_MENU5();
				break;
				case MANDUN:
					DS_MENU6();
				break;
				default:
			
				break;
			}
			
		}
		*/
	}
	else if(COOK_RUN_PAUSE == Sys.MState)
	{
		//功能选择按键不用显示
		L_K_INC();	
		L_K_DEC();
		
		switch(Sys.mMenu)
		{
			case HUOGUO:
				DS_MENU1();
				if(Sys.Fg.HeatEn)
					L_K_SHORTCUT_TIME();
				//调定时 一键计时

				L_K_BOOK_TIME();
			break;
			case ZHENGZHU:
				DS_MENU2();
				if(Sys.Fg.HeatEn)
					L_K_SHORTCUT_TIME();
				L_K_BOOK_TIME();
				//L_K_BOOK_TIME_OFF();
			break;
			case MENGZHU:
				DS_MENU3();
				//L_K_BOOK_TIME_OFF();
			break;
			case JIANKAO:
				DS_MENU4();
				if(Sys.Fg.HeatEn)
					L_K_SHORTCUT_TIME();
				L_K_BOOK_TIME();
				//L_K_BOOK_TIME_OFF();
			break;
			case BAOTANG:
				DS_MENU5();
				//L_K_BOOK_TIME_OFF();

			break;
			case MANDUN:
				DS_MENU6();
				//L_K_BOOK_TIME_OFF();

			break;
			default:
		
			break;
		}

		if(Sys.Fg.KeepWarmEn)
		{
			L_K_INC_OFF();	
			L_K_DEC_OFF();
		}
		

		
		if(Sys.Fg.HeatEn && !Sys.Sign.NoPot)
		{
			L_K_START_PAUSE();
			if(TRUE == Sys.Fg.TimekeepingEn)
				L_K_BOOK_TIME_OFF();
		}
		else
		{
			
			
			if(Sys.Fg.BooktimeEn)
			{
				L_K_START_PAUSE();
				L_START_ELSE_OFF();
			}
			else
			{
				if(DispFg.Bri)
				{
					L_K_START_PAUSE();
				}
				else
				{
					switch(Sys.mMenu)
					{
						case HUOGUO:
							DS_MENU1_OFF();
						break;
						case ZHENGZHU:
							DS_MENU2_OFF();
						break;
						case MENGZHU:
							DS_MENU3_OFF();
						break;
						case JIANKAO:
							DS_MENU4_OFF();
						break;
						case BAOTANG:
							DS_MENU5_OFF();
						break;
						case MANDUN:
							DS_MENU6_OFF();
						break;
						default:
					
						break;
					}
				}

			
				L_K_SHORTCUT_TIME_OFF();	//不显示一键计时
				L_K_BOOK_TIME_OFF();  		//暂停(包含预约状态)都不能设置定时	
			}
			
		}
	}
	
}

void DispPowKey()
{
	if(g_u16DispSegBuf[PCOM7]&PSEG9)
	{
		LED_ON();
	}
	else
	{
		LED_OFF();	
	}
	
	#ifdef _TO_CUSTOM_LIMIT_
		if(KeyOnCnt >= KEY_LIMIT)
			LED_OFF();	
	#endif
}


void PowLevelShow(ShowM_Enum ShowControl)
{
	u16 PowBuf = 0;
	if(DispFg.Bri || ShowControl)   //(FALSE == 
	{
		PowBuf = CookLevelDispPowTab[Sys.CookLevel];
		if(PowBuf/100%10)
			g_u16DispSegBuf[DIG1] |= SEG_NUM[PowBuf/100%10];
		g_u16DispSegBuf[DIG2] |= SEG_NUM[PowBuf/10%10];	
		g_u16DispSegBuf[DIG3] |= SEG_NUM[PowBuf%10];	
		g_u16DispSegBuf[DIG4] |= SEG_NUM[0];
		L_W_SIGN();	
	}
	
}

void TempShow(ShowM_Enum ShowControl)
{
	u8 TempBuf = 0;
	if(DispFg.Bri || ShowControl)   //(FALSE == 
	{	
		TempBuf = TempShowTab[Sys.CookLevel%(TEMP_L10 + 1)];
		if(TempBuf/100%10)
			g_u16DispSegBuf[DIG1] |= SEG_NUM[TempBuf/100%10];
		g_u16DispSegBuf[DIG2] |= SEG_NUM[TempBuf/10%10];	
		g_u16DispSegBuf[DIG3] |= SEG_NUM[TempBuf%10];	
		g_u16DispSegBuf[DIG4] |= DISP_C;
	}
}


void ShowTime(u32 TimeDat,ShowM_Enum ShowControl)
{
	u16 u16Dat = 0;
	u8 u8Buf = 0;
	if(DispFg.Bri || ShowControl)   //(FALSE == 
	{
		u8Buf = 1;		//冒号要跟着显示
		if(TimeDat < 60) 
		{
			g_u16DispSegBuf[DIG2] |= SEG_NUM[0];	
			g_u16DispSegBuf[DIG3] |= SEG_NUM[TimeDat/10]; 
			g_u16DispSegBuf[DIG4] |= SEG_NUM[TimeDat%10]; 

		}
		else
		{
			u16Dat = TimeDat/3600;
			if(u16Dat/10)
				g_u16DispSegBuf[DIG1] |= SEG_NUM[u16Dat/10];
			g_u16DispSegBuf[DIG2] |= SEG_NUM[u16Dat%10];	
			
			u16Dat = TimeDat/60;
			u16Dat = u16Dat%60;
			g_u16DispSegBuf[DIG3] |= SEG_NUM[u16Dat/10];
			g_u16DispSegBuf[DIG4] |= SEG_NUM[u16Dat%10];	
		}
	}
	if(TimeDat >= 60 && (TimeDat < 60*60))
	{
		if(SHOW_BRI_EN == ShowControl)
		{
			if(DispFg.Bri)
			{
				L_MIN_SIGN();;
			}	
		}
		else
		L_MIN_SIGN();;	
	}

	if(SHOW_T_RUN == ShowControl)
	{
		if(B_500MS_BriOn)
		{
			L_COLON_SIGN();;
		}	
	}
	else
	{
		if(u8Buf)
			L_COLON_SIGN();;
	}
}

void ShowTimekeeping(u16 TimeDat,ShowM_Enum ShowControl)
{
	u16 u16Dat = 0;
	u8 u8Buf = 0;
	if(TimeDat < 60) 
	{
		g_u16DispSegBuf[DIG2] |= SEG_NUM[0];	
		g_u16DispSegBuf[DIG3] |= SEG_NUM[TimeDat/10]; 
		g_u16DispSegBuf[DIG4] |= SEG_NUM[TimeDat%10]; 

	}
	else
	{
		u16Dat = TimeDat/60;
		if(u16Dat/10)
			g_u16DispSegBuf[DIG1] |= SEG_NUM[u16Dat/10];
		g_u16DispSegBuf[DIG2] |= SEG_NUM[u16Dat%10];	
		
		u16Dat = TimeDat%60;
		g_u16DispSegBuf[DIG3] |= SEG_NUM[u16Dat/10];
		g_u16DispSegBuf[DIG4] |= SEG_NUM[u16Dat%10];	
	}
	
	if(TimeDat >= 60)
	{
		L_MIN_SIGN();	
	}
	
	if(SHOW_T_RUN == ShowControl)
	{
		if(B_500MS_BriOn)
		{
			L_COLON_SIGN();;
		}	
	}
	else
	{
		//if(u8Buf)
			L_COLON_SIGN();;
	}


}



static void F_DispNormal(void)
{
	u8 u8temp0 = 0;
	u8 u8Buf = 0;
	ShowM_Enum mShowStateBuf = SHOW_BRI_EN;

	
	if(DispIndex != DISP_NORMAL)
			DispRunSec = 0;
	
	u8temp0 = 0;
	if(Sys.Fg.BooktimeEn)
		u8temp0 ++;
	if(Sys.Fg.CooktimeEn)
		u8temp0 ++;
	if (GetTimeTick(TIME_1S))
	{
		//if((MENGZHU == Sys.mMenu) || (BAOTANG == Sys.mMenu) || (MANDUN == Sys.mMenu))
		//{
		//	if(Sys.Fg.BooktimeEn)
		//		u8Buf = 10;
		//}
		//else
		{
			u8Buf = 10;
			if(u8temp0 >= 2)
			{
				if(COOK_IDLE == Sys.MState)
				{
					if((MENGZHU == Sys.mMenu) || (BAOTANG == Sys.mMenu) || (MANDUN == Sys.mMenu))
						u8Buf = 10;
					else
						u8Buf = 9;
				}
					
			}
		}
		if(++DispRunSec >= u8Buf)
			DispRunSec = 0;

		
	}
	

	if(COOK_IDLE == Sys.MState)
	{
		if(MENU_NONE == Sys.mMenu)
		{
			DS_ALL_MENU_AND_KEY();
			g_u16DispSegBuf[DIG1] |= BSEG_G;
			g_u16DispSegBuf[DIG2] |= BSEG_G;	
			g_u16DispSegBuf[DIG3] |= BSEG_G;	
			g_u16DispSegBuf[DIG4] |= BSEG_G;

			
		}	
		else if((MENGZHU == Sys.mMenu) || (BAOTANG == Sys.mMenu) || (MANDUN == Sys.mMenu))
		{
			if(2 == u8temp0)
			{
				if(DispRunSec < 5)
				{
					L_BOOK_SIGN();
					ShowTime(Sys.BookRunSec,SHOW_ALL_ON);
				}
				else
				{
					L_RESIDUE_SIGN();
					ShowTime(Sys.CookRunSec,SHOW_ALL_ON);
				}
			}
			else
			{
				if(DispFg.Bri)
					L_RESIDUE_SIGN();
				ShowTime(Sys.CookRunSec,SHOW_BRI_EN);
			}
		}
		else if(2 == u8temp0) 		//if(DispRunSec < 5)
		{
			if(DispRunSec < 3)		//if(2 == u8temp0)
			{
				L_BOOK_SIGN();
				ShowTime(Sys.BookRunSec,SHOW_ALL_ON);
			}
			else if(DispRunSec < 6)
			{
				L_RESIDUE_SIGN();
				ShowTime(Sys.CookRunSec,SHOW_ALL_ON);
			}
			else
			{
				if(JIANKAO == Sys.mMenu)
					TempShow(SHOW_ALL_ON);	
				else
					PowLevelShow(SHOW_ALL_ON);
			}
		}
		else if(1 == u8temp0)
		{
			if(DispRunSec < 5)
			{
				if(Sys.Fg.BooktimeEn)
				{
					L_BOOK_SIGN();
					ShowTime(Sys.BookRunSec,SHOW_ALL_ON);
				}
				else
				{
					L_RESIDUE_SIGN();
					ShowTime(Sys.CookRunSec,SHOW_ALL_ON);
				}
			}
			else
			{
				if(JIANKAO == Sys.mMenu)
					TempShow(SHOW_ALL_ON);	
				else
					PowLevelShow(SHOW_ALL_ON);
			}
		}
		else
		{
			if(JIANKAO == Sys.mMenu)
				TempShow(SHOW_BRI_EN);	
			else
				PowLevelShow(SHOW_BRI_EN);
		}
	}
	else if(COOK_RUN_PAUSE == Sys.MState)
	{
		if(Sys.Fg.HeatEn && !Sys.Sign.NoPot)
		{
			if(Sys.Fg.TimekeepingEn)
			{
				//if(DispRunSec < 5)
				{
					ShowTimekeeping(Sys.TKeepSec,SHOW_T_RUN);
				}
				/*
				else
				{
					if(JIANKAO == Sys.mMenu)
						TempShow(SHOW_ALL_ON);	
					else
						PowLevelShow(SHOW_ALL_ON);
				}*/
			}
			else if((MENGZHU == Sys.mMenu) || (BAOTANG == Sys.mMenu) || (MANDUN == Sys.mMenu))
			{
				if(Sys.Fg.KeepWarmEn)
				{
					g_u16DispSegBuf[DIG2] |= DISP_b;	
					g_u16DispSegBuf[DIG3] |= DISP_b; 
				}
				else
				{
					L_RESIDUE_SIGN();
					ShowTime(Sys.CookRunSec,SHOW_T_RUN);
				}
			}
			else if(Sys.Fg.CooktimeEn)
			{
				if(DispRunSec < 5)
				{
					L_RESIDUE_SIGN();
					ShowTime(Sys.CookRunSec,SHOW_T_RUN);
				}
				else
				{
					if(JIANKAO == Sys.mMenu)
						TempShow(SHOW_ALL_ON);	
					else
						PowLevelShow(SHOW_ALL_ON);
				}	
			}
			else
			{
				if(JIANKAO == Sys.mMenu)
					TempShow(SHOW_ALL_ON);	
				else
					PowLevelShow(SHOW_ALL_ON);
			}	
		}
		else
		{	
			if(Sys.Fg.BooktimeEn)
			{
				L_BOOK_SIGN();
				ShowTime(Sys.BookRunSec,SHOW_T_RUN);
			}
			else if((MENGZHU == Sys.mMenu) || (BAOTANG == Sys.mMenu) || (MANDUN == Sys.mMenu))
			{
				if(DispFg.Bri)
				{
					if(Sys.Fg.KeepWarmEn)
					{
						g_u16DispSegBuf[DIG2] |= DISP_b;	
						g_u16DispSegBuf[DIG3] |= DISP_b; 
					}
					else
					{
						if(DispFg.Bri)
							L_RESIDUE_SIGN();
						ShowTime(Sys.CookRunSec,SHOW_BRI_EN);
					}
				}
			}
			else if(1 == u8temp0)		//if(DispRunSec < 5)
			{
				if(DispRunSec < 5)
				{	
					if(DispFg.Bri)
					{
						L_RESIDUE_SIGN();
						ShowTime(Sys.CookRunSec,SHOW_BRI_EN);
					}
				}
				else
				{	
					if(JIANKAO == Sys.mMenu)
						TempShow(SHOW_BRI_EN);	
					else
						PowLevelShow(SHOW_BRI_EN);
				}
			}
			else
			{
				if(JIANKAO == Sys.mMenu)
					TempShow(SHOW_BRI_EN);	
				else
					PowLevelShow(SHOW_BRI_EN);
			}
		}
	}
	#ifndef _ERR_IGNORE_



	if((Sys.MState != COOK_NONE) && ((Flag1_Error || Flag2_Error) || (HEAT_PRO_COOK == sHeat.State)))
	{

			
		SetStr((uchar *)&g_u16DispSegBuf, 0, sizeof(g_u16DispSegBuf));
		if(!B_500MS_BriOn)	 //故障显示 
		{
			g_u16DispSegBuf[DIG2] |= DISP_E; 			
				 if(B_EPanOpen)   g_u16DispSegBuf[DIG3] |= DISP_0; //锅底NTC开路故障 
			else if(B_EPanClose)  g_u16DispSegBuf[DIG3] |= DISP_1; //锅底NTC短路故障 	
			else if(B_EPanOver || (HEAT_PRO_COOK == sHeat.State))   g_u16DispSegBuf[DIG3] |= DISP_2; //锅底NTC超温故障 	
			else if(B_EVolHigh)   g_u16DispSegBuf[DIG3] |= DISP_3; //市电高压故障			
			else if(B_EVolLow)	  g_u16DispSegBuf[DIG3] |= DISP_4; //市电低压故障 	
			else if(B_EIgbtOver)  g_u16DispSegBuf[DIG3] |= DISP_5; //IGBT-NTC超温故障	
			else if(B_EIgbtOpen)  g_u16DispSegBuf[DIG3] |= DISP_6; //IGBT-NTC开路故障 
			else if(B_EIgbtClose) g_u16DispSegBuf[DIG3] |= DISP_7; //IGBT-NTC短路故障					
			else if(B_Comm_Error) g_u16DispSegBuf[DIG3] |= DISP_8; //通信故障	
			//else if(!(Status2_Work & 0x20) || Sys.Sign.NoPot)   g_u16DispSegBuf[DIG3] |= DISP_9; //锅的开关故障或者未检测到有金属锅
		}
	}
	#else 
		#message "====================故障屏蔽======================"
		SetStr((uchar *)&g_u16DispSegBuf, 0, sizeof(g_u16DispSegBuf));
		u8Buf = 0;

//#define KEY_SHORTCUT_T				BIT12
//#define KEY_BOOK_T					BIT13
//#define KEY_DEC 					BIT14
//#define KEY_INC 					BIT22
//#define KEY_MENU 					BIT18
//#define KEY_START_PAUSE				BIT9

		
			if((g_u32KeyBuf & BIT12) == BIT12)  u8Buf|= 0x01;
			if((g_u32KeyBuf & BIT13) == BIT13)  u8Buf|= 0x02;
			if((g_u32KeyBuf & BIT14) == BIT14)  u8Buf|= 0x04;
			if((g_u32KeyBuf & BIT22) == BIT22)  u8Buf|= 0x08;
			if((g_u32KeyBuf & BIT18) == BIT18)  u8Buf|= 0x10;
			if((g_u32KeyBuf & BIT9) == BIT9)  u8Buf|= 0x20;
			g_u16DispSegBuf[DIG2] |= SEG_NUM[u8Buf/16];
			g_u16DispSegBuf[DIG3] |= SEG_NUM[u8Buf%16];
	#endif
	//		g_u16DispSegBuf[DIG2] = 0;
	//	g_u16DispSegBuf[DIG3] = 0;
	//	if(Status2_Work & 0x20) g_u16DispSegBuf[DIG3] |= DISP_C;

//	#message "====================待处理检锅方式======================"
	//无锅
//	if(Status1_Work&0x02)
//	{
//		L_RESIDUE_SIGN();
//	}
	//有锅
//	if(Status1_Work&0x04)
//	{
//		L_MIN_SIGN();
//	}

}


static void F_DispSetPow(void)
{
	if(JIANKAO == Sys.mMenu)
	{
		if(COOK_RUN_PAUSE == Sys.MState)
			TempShow(SHOW_ALL_ON);	 
		else
			TempShow(SHOW_BRI_EN);	 	
	}
	else
	{
		if(COOK_RUN_PAUSE == Sys.MState)
			PowLevelShow(SHOW_ALL_ON); 
		else
			PowLevelShow(SHOW_BRI_EN);		
	}
}

static void F_DispSetBookTime(void)
{
	
	if(DispFg.Bri)
	{
		ShowTime(Sys.BookRunSec,SHOW_BRI_EN);
		L_BOOK_SIGN();	
	}
}

static void F_DispSetTimeCook(void)
{
	ShowTime(Sys.CookRunSec,SHOW_BRI_EN);
	if(DispFg.Bri)
	L_RESIDUE_SIGN();
}

/*
static void F_DispSetTimeKeeping(void)
{
	ShowTime(Sys.TKeepSec,SHOW_BRI_EN);
		
}
*/










void SetFactoryMode()
{
	#ifndef _TEST_DEBUG_
//	FactoryStep = 0;
//	FactoryMode_Sec = 0;
//	SysMode = SysMX_Factory;
	#endif
}

void FactoryKeyNumFuc(u8 Dat)
{
	#ifndef _TEST_DEBUG_
	if(SysMX_Factory == SysMode)
	{
//		FactoryKeyNum = Dat;
//		FactoryMode_Sec = 0;
//		FactoryStep = 1;
//		BUZZ();
	}
	#endif
}


//100ms
//输出为进入产测，自动轮流输出动作
void FactoryDisplay_Value_Set(void)
{
	#ifndef _TEST_DEBUG_
	if(GetTimeTick(TIME_1S))
	{
		if(++FactoryMode_Sec >= 60)
		{
			FactoryMode_Sec = 0; 
			SysMode = SysMX_Run;
			WorkModeNewSet(SET_STATE_NONE);
		}
	}

	switch (FactoryStep) 
	{
		case 0:
			SetStr((uchar *)&g_u16DispSegBuf, 0xff, sizeof(g_u16DispSegBuf));
			if(SysMX_Adjust == SysMode)
				FactoryStep =2;	
		break;

		case 1:
			//发送WIFI产测模式
			if(B_Comm_Error)
			{
				g_u16DispSegBuf[DIG3] |= DISP_8; //通信故障	
			}
			else
			{
				g_u16DispSegBuf[DIG1] = SEG_NUM[FactoryKeyNum];
				g_u16DispSegBuf[DIG2] = SEG_NUM[FactoryKeyNum];
				g_u16DispSegBuf[DIG3] = SEG_NUM[FactoryKeyNum];
				g_u16DispSegBuf[DIG4] = SEG_NUM[FactoryKeyNum];
				L_K_ALL();
			}
			break;
		case 2:
			g_u16DispSegBuf[DIG1] = SEG_NUM[Sys.ComSlopeCurr_Adjust/16];
			g_u16DispSegBuf[DIG2] = SEG_NUM[Sys.ComSlopeCurr_Adjust%16];
			g_u16DispSegBuf[DIG3] = SEG_NUM[Sys.ShowEppSlope/16];
			g_u16DispSegBuf[DIG4] = SEG_NUM[Sys.ShowEppSlope%16];
			L_K_INC();
			L_K_DEC();
			break;
		default:

			break;
	}
	#endif
	
}


//100MS
void Display_Value_Set(void)
{
//	static u8 DisplayStepTest = 0;
	u8 u8temp0 = 0;
	u16 u16TotalTime = 0;
	u8 i;
	for (i = 0; i < DISP_COM_LENGTH; i++)
	{
		g_u16DispSegBuf[i] = 0;
		if(i < DIG_MAX)
			DispBuf[i] = 0;
	}

	switch (SysMode)
	{
	case SysMX_FstPOW_ON:
		if(++g_u8PowerON_TCnt > POW_ON_TIME)
		{
			g_u8PowerON_TCnt = 10;
			SysMode = SysMX_Run;
			WorkModeNewSet(SET_FST_ON);
		}
		SetStr((uchar *)&g_u16DispSegBuf, 0xffff, sizeof(g_u16DispSegBuf));
		break;

	case SysMX_Run:
		if (DispLastTime)
		{
			DispLastTime--;
			if (DispLastTime == 0)
			{
				if(DISP_SET_COOK_TIME == DispIndex)
				{
					if(0 == Sys.CookRunSec)
						Sys.Fg.CooktimeEn = FALSE;
				}
				F_DispTaskChange(DISP_NORMAL);
			}
		}

		if (DispBlink)
		{
			DispBlink--;
			if (DispBlink == 0)
			{
				DispBlink = DispTask[DispIndex].Blink;
				DispFg.Bri = !DispFg.Bri;
			}
		}

		(DispTask[DispIndex].DispFunc)(); //
		F_DispLed();
		break;

	case SysMX_Factory:
	case SysMX_Adjust:
		FactoryDisplay_Value_Set();
		break;

	default:
		SysMode = SysMX_FstPOW_ON;   //异常处理
		break;
	}

	#ifdef	 _TEST_DEBUG_
	#message "====================调试模式烹饪显示======================"

	if(sHeat.State == HEAT_NORMAL)
	{
		if((COOK_RUN_PAUSE == Sys.MState) && !Flag1_Error && !Flag2_Error && !Sys.Sign.NoPot)
		{ 
			g_u16DispSegBuf[DIG1] = 0x00;
			g_u16DispSegBuf[DIG2] = 0x00;
			g_u16DispSegBuf[DIG3] = 0x00;
			g_u16DispSegBuf[DIG4] = 0x00;
			g_u16DispSegBuf[PCOM5] = 0;


		
			if(B_500MS_BriOn)
			{
				g_u16DispSegBuf[DIG2] = SEG_NUM[AD_TmprPan/100%10];
				g_u16DispSegBuf[DIG3] = SEG_NUM[AD_TmprPan/10%10];
				g_u16DispSegBuf[DIG4] = SEG_NUM[AD_TmprPan%10];

				L_BOOK_SIGN();	
			}
			else
			{
				
				g_u16DispSegBuf[DIG1] = SEG_NUM[sHeat.ShowLevel%10];
				if(sHeat.s8TempSlope < 0)
				{
					u8temp0 = abs(sHeat.s8TempSlope);
					g_u16DispSegBuf[DIG2] = BSEG_G;
					//g_u16DispSegBuf[DIG2] = SEG_NUM[u8temp0/100%10];
					g_u16DispSegBuf[DIG3] = SEG_NUM[u8temp0/10%16];
					g_u16DispSegBuf[DIG4] = SEG_NUM[u8temp0%10];
				}
				else
				{
					u8temp0 = abs(sHeat.s8TempSlope);
					//g_u16DispSegBuf[DIG2] = SEG_NUM[u8temp0/100%10];
					g_u16DispSegBuf[DIG3] = SEG_NUM[u8temp0/10%16];
					g_u16DispSegBuf[DIG4] = SEG_NUM[u8temp0%10];

				}
			}
		}
		else
		{
			if(DISP_NORMAL == DispTask[i].Index)
			{
				if(B_500MS_BriOn)
				{
					g_u16DispSegBuf[DIG1] = 0x00;
					g_u16DispSegBuf[DIG2] = 0x00;
					g_u16DispSegBuf[DIG3] = 0x00;
					g_u16DispSegBuf[DIG4] = 0x00;

					g_u16DispSegBuf[DIG2] = SEG_NUM[AD_TmprPan/100%10];
					g_u16DispSegBuf[DIG3] = SEG_NUM[AD_TmprPan/10%10];
					g_u16DispSegBuf[DIG4] = SEG_NUM[AD_TmprPan%10];
					
					L_BOOK_SIGN();	
				}
			}

		}

	}
	#endif
	#ifdef	 _TEST_CAP_AD_
		#message "====================电容AD显示======================"
	g_u16DispSegBuf[DIG1] = 0x00;
	g_u16DispSegBuf[DIG2] = 0x00;
	g_u16DispSegBuf[DIG3] = 0x00;
	g_u16DispSegBuf[DIG4] = 0x00;

	
	g_u16DispSegBuf[DIG1] = SEG_NUM[AD_Value/1000%10];
	g_u16DispSegBuf[DIG2] = SEG_NUM[AD_Value/100%10];
	g_u16DispSegBuf[DIG3] = SEG_NUM[AD_Value/10%10];
	g_u16DispSegBuf[DIG4] = SEG_NUM[AD_Value%10];

	


	#endif
	for (i = 0; i < 7; i++)
	{
		g_u8DispSeg[2*i+1] = g_u16DispSegBuf[i]>>8;
		g_u8DispSeg[2*i+0] = g_u16DispSegBuf[i]&0xff;
	}


}


/********************************************************************************************************
 * @brief  获取当前显示状态
 * @param  {None}
 * @return 当前显示状态
 ********************************************************************************************************/
unsigned char GetDispType(void)
{
    return DispTask[DispIndex].Index;
}

/********************************************************************************************************
 * @ name   : void F_DispTaskChange(unsigned char temp)
 * @ msg    : 显示时间切换函数
 * @ param   (unsigned char) temp        显示事件序号
 * @ return  (*)
 ********************************************************************************************************/
void F_DispTaskChange(unsigned char temp)
{
	unsigned char i;

	for (i = 0; i < _DISP_TOTAL_; i++)
	{
		if(DispTask[i].Index == temp)
		{
			if((DispTask[i].Index == DISP_SET_POW) && (DISP_SET_COOK_TIME == DispIndex))
			{
				if(0 == Sys.CookRunSec)
					Sys.Fg.CooktimeEn = FALSE;
			}
			else if((DispTask[i].Index == DISP_SET_BOOK_TIME) && (DISP_SET_COOK_TIME == DispIndex))
			{
				if(0 == Sys.CookRunSec)
					Sys.Fg.CooktimeEn = FALSE;
			}
			else if(DispTask[i].Index == DISP_NORMAL)
			{
				//if((DISP_SET_BOOK_TIME == DispIndex) && Sys.Fg.BooktimeEn)
					DispRunSec = 0;
				//else 
				if((DISP_SET_COOK_TIME == DispIndex)  && Sys.Fg.CooktimeEn && Sys.Fg.BooktimeEn)
				{
					if((MENGZHU == Sys.mMenu) || (BAOTANG == Sys.mMenu) || (MANDUN == Sys.mMenu))	
					{
						//空
					}
					else
						DispRunSec = 3;
						
				}
				//else
				//	DispRunSec = 0;		
			}
			DispIndex = i;
			DispLastTime = DispTask[i].LastTime;
			DispBlink = DispTask[i].Blink;
			DispFg.Bri = TRUE;
			DispBlink = 8; //先给变化常亮0.5S 
			return;
		}
	}
}
/********************************************************************************************************
* @ name   : DispTask[_DISP_TOTAL_]
* @ msg    : 显示模块事件表
* @ param   (*)
* @ return  (*)
 ********************************************************************************************************/
	code DispTask_t DispTask[_DISP_TOTAL_] = 
	{
		//Index,					LastTime,	Blink *100ms,	Function
		{DISP_NORMAL,				0,			5,				&F_DispNormal},
		{DISP_SET_POW,				30, 		5,				&F_DispSetPow},
		{DISP_SET_BOOK_TIME,		50,			5,				&F_DispSetBookTime},
		{DISP_SET_COOK_TIME,		50, 		5,				&F_DispSetTimeCook},
	};
