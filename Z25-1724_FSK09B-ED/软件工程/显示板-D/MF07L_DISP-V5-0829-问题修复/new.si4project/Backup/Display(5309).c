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

#define	PCOM1	4	
#define	PCOM2	5
#define PCOM3	6
#define PCOM4	3
#define	PCOM5	2	
#define	PCOM6	0
#define PCOM7	1


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
//	u16 PowBuf = 0;
//	if(DispFg.Bri || ShowControl)   //(FALSE == 
//	{
//		PowBuf = CookLevelDispPowTab[Sys.CookLevel];
//		if(PowBuf/100%10)
//			g_u16DispSegBuf[DIG1] |= SEG_NUM[PowBuf/100%10];
//		g_u16DispSegBuf[DIG2] |= SEG_NUM[PowBuf/10%10];	
//		g_u16DispSegBuf[DIG3] |= SEG_NUM[PowBuf%10];	
//		g_u16DispSegBuf[DIG4] |= SEG_NUM[0];
//		L_W_SIGN();	
//	}
	
}

void TempShow(ShowM_Enum ShowControl)
{
//	u8 TempBuf = 0;
//	if(DispFg.Bri || ShowControl)   //(FALSE == 
//	{	
//		TempBuf = TempShowTab[Sys.CookLevel%(TEMP_L10 + 1)];
//		if(TempBuf/100%10)
//			g_u16DispSegBuf[DIG1] |= SEG_NUM[TempBuf/100%10];
//		g_u16DispSegBuf[DIG2] |= SEG_NUM[TempBuf/10%10];	
//		g_u16DispSegBuf[DIG3] |= SEG_NUM[TempBuf%10];	
//		g_u16DispSegBuf[DIG4] |= DISP_C;
//	}
}


void ShowTime(u32 TimeDat,ShowM_Enum ShowControl)
{
//	u16 u16Dat = 0;
//	u8 u8Buf = 0;
//	if(DispFg.Bri || ShowControl)   //(FALSE == 
//	{
//		u8Buf = 1;		//冒号要跟着显示
//		if(TimeDat < 60) 
//		{
//			g_u16DispSegBuf[DIG2] |= SEG_NUM[0];	
//			g_u16DispSegBuf[DIG3] |= SEG_NUM[TimeDat/10]; 
//			g_u16DispSegBuf[DIG4] |= SEG_NUM[TimeDat%10]; 

//		}
//		else
//		{
//			u16Dat = TimeDat/3600;
//			if(u16Dat/10)
//				g_u16DispSegBuf[DIG1] |= SEG_NUM[u16Dat/10];
//			g_u16DispSegBuf[DIG2] |= SEG_NUM[u16Dat%10];	
//			
//			u16Dat = TimeDat/60;
//			u16Dat = u16Dat%60;
//			g_u16DispSegBuf[DIG3] |= SEG_NUM[u16Dat/10];
//			g_u16DispSegBuf[DIG4] |= SEG_NUM[u16Dat%10];	
//		}
//	}
//	if(TimeDat >= 60 && (TimeDat < 60*60))
//	{
//		if(SHOW_BRI_EN == ShowControl)
//		{
//			if(DispFg.Bri)
//			{
//				L_MIN_SIGN();;
//			}	
//		}
//		else
//		L_MIN_SIGN();;	
//	}

//	if(SHOW_T_RUN == ShowControl)
//	{
//		if(B_500MS_BriOn)
//		{
//			L_COLON_SIGN();;
//		}	
//	}
//	else
//	{
//		if(u8Buf)
//			L_COLON_SIGN();;
//	}
}



static void F_DispNormal(void)
{

}


static void F_DispSetPow(void)
{

}

static void F_DispSetBookTime(void)
{
	
	if(DispFg.Bri)
	{

		L_BOOK_SIGN();	
	}
}

static void F_DispSetTimeCook(void)
{

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

}

void FactoryKeyNumFuc(u8 Dat)
{
	#ifndef _TEST_DEBUG_
//	if(SysMX_Factory == SysMode)
//	{
//		FactoryKeyNum = Dat;
//		FactoryMode_Sec = 0;
//		FactoryStep = 1;
//		BUZZ();
//	}
	#endif
}


//100ms
//输出为进入产测，自动轮流输出动作
void FactoryDisplay_Value_Set(void)
{
	/*
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
			if(B_Comm_Error || Sys.Sign.RecSI_Err)
			{
				g_u16DispSegBuf[DIG2] |= DISP_E; 
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
	*/
}


//100MS
void Display_Value_Set(void)
{
	/*
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
			g_u8PowerON_TCnt = FST_POW_INIT_TIME;
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

	//if(sHeat.State == HEAT_NORMAL)
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
//	g_u16DispSegBuf[DIG1] = 0x00;
//	g_u16DispSegBuf[DIG2] = 0x00;
//	g_u16DispSegBuf[DIG3] = 0x00;
//	g_u16DispSegBuf[DIG4] = 0x00;


//	g_u16DispSegBuf[DIG1] = SEG_NUM[PowADValue/1000%10]; 
//	g_u16DispSegBuf[DIG2] = SEG_NUM[PowADValue/100%10];
//	g_u16DispSegBuf[DIG3] = SEG_NUM[PowADValue/10%10];
//	g_u16DispSegBuf[DIG4] = SEG_NUM[PowADValue%10];

	
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

	*/
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
		/*
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
		*/
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
