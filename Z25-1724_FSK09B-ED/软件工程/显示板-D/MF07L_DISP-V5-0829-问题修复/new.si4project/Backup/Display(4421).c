#define _Native_Display_H
#include "Include\Include.h"
#include <math.h>

//unsigned int xdata PWM31_REG _at_ 0x103a;	//PWM占空比调节寄存器

u16 code SEG_NUM[] ={   
 DISP_0,DISP_1,DISP_2,DISP_3,DISP_4,DISP_5,DISP_6,DISP_7,DISP_8,DISP_9,DISP_A,DISP_b,DISP_C,DISP_d,DISP_E,DISP_F
};


#define	PSEG1	BIT3
#define	PSEG2	BIT4
#define	PSEG3	BIT6
#define	PSEG4	BIT5
#define	PSEG5	BIT0
#define	PSEG6	BIT1
#define	PSEG7	BIT2


#define	PCOM1	0	
#define	PCOM2	1
#define PCOM3	2
#define PCOM4	3
#define	PCOM5	4	
#define	PCOM6	5
#define PCOM7	6
/*
#define	PIN_A			P_SEG_5L	BIT0
#define	PIN_B			P_SEG_6L
#define	PIN_C			P_SEG_7L
#define	PIN_D			P_SEG_1L
#define	PIN_E			P_SEG_2L
#define	PIN_F			P_SEG_4L
#define	PIN_G			P_SEG_3L


#define		P_COM5		P51	//PIN4
#define		P_SEG_1L	P50	//PIN5
#define		P_SEG_7L	P11	//PIN6
#define		P_SEG_6L	P12	//PIN7   tTCK
#define		P_SI		P13	//PIN8	 tDIO
#define		P_SEG_5L	P14	//PIN9
#define		P_SEG_3L	P15	//PIN10
#define		P_SEG_4L	P16	//PIN11
#define		P_COM1		P17	//PIN12		//0
#define		P_COM6		P27	//PIN13
#define		P_COM7		P26	//PIN14
//PIN15~PIN28
#define		P_SEG_2L	P25	//PIN15
#define		P_COM2		P24	//PIN16
#define		P_COM3		P23	//PIN17
#define		P_COM4		P22	//PIN18
*/


#define	DS_LED1()		{g_u8DispSegBuf[PCOM5] |= PSEG1+PSEG2;g_u8DispSegBuf[PCOM4] |= PSEG7;}
#define	DS_LED2()		{g_u8DispSegBuf[PCOM4] |= PSEG4+PSEG5+PSEG6;}
#define	DS_LED3()		{g_u8DispSegBuf[PCOM4] |= PSEG1+PSEG2+PSEG3;}
#define	DS_LED4()		{g_u8DispSegBuf[PCOM3] |= PSEG5+PSEG6+PSEG7;}
#define	DS_LED5()		{g_u8DispSegBuf[PCOM3] |= PSEG1+PSEG3+PSEG4;}
#define	DS_LED6()		{g_u8DispSegBuf[PCOM2] |= PSEG6+PSEG7;g_u8DispSegBuf[PCOM3] |= PSEG2;}
#define	DS_LED7()		{g_u8DispSegBuf[PCOM2] |= PSEG3+PSEG4+PSEG5;}
#define	DS_LED8()		{g_u8DispSegBuf[PCOM2] |= PSEG1+PSEG2;g_u8DispSegBuf[PCOM1] |= PSEG7;}
#define	DS_LED9()		{g_u8DispSegBuf[PCOM1] |= PSEG4+PSEG5+PSEG6;}
#define	DS_LED10()		{g_u8DispSegBuf[PCOM1] |= PSEG1+PSEG2+PSEG3;}


#define	DS_LED1_OFF()	{g_u8DispSegBuf[PCOM5] &= ~(PSEG1+PSEG2);g_u8DispSegBuf[PCOM4] &= ~PSEG7;}
#define	DS_LED2_OFF()	{g_u8DispSegBuf[PCOM4] &= ~(PSEG4+PSEG5+PSEG6);}
#define	DS_LED3_OFF()	{g_u8DispSegBuf[PCOM4] &= ~(PSEG1+PSEG2+PSEG3);}
#define	DS_LED4_OFF()	{g_u8DispSegBuf[PCOM3] &= ~(PSEG5+PSEG6+PSEG7);}
#define	DS_LED5_OFF()	{g_u8DispSegBuf[PCOM3] &= ~(PSEG1+PSEG3+PSEG4);}
#define	DS_LED6_OFF()	{g_u8DispSegBuf[PCOM2] &= ~(PSEG6+PSEG7);g_u8DispSegBuf[PCOM3] &= ~PSEG2;}
#define	DS_LED7_OFF()	{g_u8DispSegBuf[PCOM2] &= ~(PSEG3+PSEG4+PSEG5);}
#define	DS_LED8_OFF()	{g_u8DispSegBuf[PCOM2] &= ~(PSEG1+PSEG2);g_u8DispSegBuf[PCOM1] &= ~PSEG7;}
#define	DS_LED9_OFF()	{g_u8DispSegBuf[PCOM1] &= ~(PSEG4+PSEG5+PSEG6);}
#define	DS_LED10_OFF()	{g_u8DispSegBuf[PCOM1] &= ~(PSEG1+PSEG2+PSEG3);}

#define	DS_SIGN_C()			{g_u8DispSegBuf[PCOM5] |= PSEG3+PSEG6;}
#define	DS_SIGN_H()			{g_u8DispSegBuf[PCOM5] |= PSEG4+PSEG3;}

#define	DS_SIGN_C_OFF()		{g_u8DispSegBuf[PCOM5] &= ~(PSEG3+PSEG6);}
#define	DS_SIGN_H_OFF()		{g_u8DispSegBuf[PCOM5] &= ~(PSEG4+PSEG3);}



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
//		g_u8DispSegBuf[LedTask[temp].Com] |= LedTask[temp].Val;
//	}
//}

/*
static void F_LedOFF(unsigned char temp)
{
	if (temp < _LED_TOTAL_)
	{
		g_u8DispSegBuf[LedTask[temp].Com] &= ~LedTask[temp].Val;
	}
}
*/






static void F_DispLed(void)
{

}

void PowLevelShow(ShowM_Enum ShowControl)
{
//	u16 PowBuf = 0;
//	if(DispFg.Bri || ShowControl)   //(FALSE == 
//	{
//		PowBuf = CookLevelDispPowTab[Sys.CookLevel];
//		if(PowBuf/100%10)
//			g_u8DispSegBuf[DIG1] |= SEG_NUM[PowBuf/100%10];
//		g_u8DispSegBuf[DIG2] |= SEG_NUM[PowBuf/10%10];	
//		g_u8DispSegBuf[DIG3] |= SEG_NUM[PowBuf%10];	
//		g_u8DispSegBuf[DIG4] |= SEG_NUM[0];
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
//			g_u8DispSegBuf[DIG1] |= SEG_NUM[TempBuf/100%10];
//		g_u8DispSegBuf[DIG2] |= SEG_NUM[TempBuf/10%10];	
//		g_u8DispSegBuf[DIG3] |= SEG_NUM[TempBuf%10];	
//		g_u8DispSegBuf[DIG4] |= DISP_C;
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
//			g_u8DispSegBuf[DIG2] |= SEG_NUM[0];	
//			g_u8DispSegBuf[DIG3] |= SEG_NUM[TimeDat/10]; 
//			g_u8DispSegBuf[DIG4] |= SEG_NUM[TimeDat%10]; 

//		}
//		else
//		{
//			u16Dat = TimeDat/3600;
//			if(u16Dat/10)
//				g_u8DispSegBuf[DIG1] |= SEG_NUM[u16Dat/10];
//			g_u8DispSegBuf[DIG2] |= SEG_NUM[u16Dat%10];	
//			
//			u16Dat = TimeDat/60;
//			u16Dat = u16Dat%60;
//			g_u8DispSegBuf[DIG3] |= SEG_NUM[u16Dat/10];
//			g_u8DispSegBuf[DIG4] |= SEG_NUM[u16Dat%10];	
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
			SetStr((uchar *)&g_u8DispSegBuf, 0xff, sizeof(g_u8DispSegBuf));
			if(SysMX_Adjust == SysMode)
				FactoryStep =2;	
		break;

		case 1:
			//发送WIFI产测模式
			if(B_Comm_Error || Sys.Sign.RecSI_Err)
			{
				g_u8DispSegBuf[DIG2] |= DISP_E; 
				g_u8DispSegBuf[DIG3] |= DISP_8; //通信故障	
			}
			else
			{
				g_u8DispSegBuf[DIG1] = SEG_NUM[FactoryKeyNum];
				g_u8DispSegBuf[DIG2] = SEG_NUM[FactoryKeyNum];
				g_u8DispSegBuf[DIG3] = SEG_NUM[FactoryKeyNum];
				g_u8DispSegBuf[DIG4] = SEG_NUM[FactoryKeyNum];
				L_K_ALL();
			}
			break;
		case 2:
			g_u8DispSegBuf[DIG1] = SEG_NUM[Sys.ComSlopeCurr_Adjust/16];
			g_u8DispSegBuf[DIG2] = SEG_NUM[Sys.ComSlopeCurr_Adjust%16];
			g_u8DispSegBuf[DIG3] = SEG_NUM[Sys.ShowEppSlope/16];
			g_u8DispSegBuf[DIG4] = SEG_NUM[Sys.ShowEppSlope%16];
			L_K_INC();
			L_K_DEC();
			break;
		default:

			break;
	}
	#endif
	*/
}

#ifdef _DEMO_TEST_
void DemoTest()
{
	u8 i = 0;
	static u8 DisplayStepTest = 0;
	for (i = 0; i < COM_MAX_CNT; i++)
	{
		g_u8DispSegBuf[i] = 0;
		if(i < DIG_MAX)
			DispBuf[i] = 0;
	}
	if (GetTimeTick(TIME_1S))
		DisplayStepTest++;
	if(DisplayStepTest >= 10)
		DisplayStepTest = 0;
	g_u8DispSegBuf[DIG1] = SEG_NUM[DisplayStepTest%10]; 
	g_u8DispSegBuf[DIG2] = SEG_NUM[DisplayStepTest%10];
	switch(DisplayStepTest)
	{
		case 0:
			DS_LED1();
			DS_SIGN_C();
		break;
		case 1:
			DS_LED2();
			DS_SIGN_H();
		break;
		case 2:
			DS_LED3();
		break;
		case 3:
			DS_LED4();
		break;
		case 4:
			DS_LED5();
		break;
		case 5:
			DS_LED6();
		break;
		case 6:
			DS_LED7();
		break;
		case 7:
			DS_LED8();
		break;
		case 8:
			DS_LED9();
		break;
		case 9:
			DS_LED10();
		break;
	}
	if(g_u32KeyBuf == KEY_CUP)
		DS_LED1();
	if(g_u32KeyBuf == KEY_PLATE)
		DS_LED2();
	if(g_u32KeyBuf == KEY_WARM)
		DS_LED3();
	if(g_u32KeyBuf == KEY_TEMP)
		DS_LED4();
	if(g_u32KeyBuf == KEY_TIME)
		DS_LED5();
	if(g_u32KeyBuf == KEY_DEC)
		DS_LED6();
	if(g_u32KeyBuf == KEY_INC)
		DS_LED7();
	if(g_u32KeyBuf == KEY_START_PAUSE)
		DS_LED8();
	if(g_u32KeyBuf == KEY_POW)
		DS_LED9();
	if(g_u32KeyBuf == KEY_LOCK)
		DS_LED10();
}
#endif


#ifdef _SHOW_TEMP_AD_
void DemoTempAD()
{
	if(B_500MS_BriOn)
	{
		g_u8DispSegBuf[DIG1] = SEG_NUM[DisplayStepTest%10]; 
		g_u8DispSegBuf[DIG2] = SEG_NUM[DisplayStepTest%10];
	}
	/*
	else
	{
		Disp1650Seg[2] =  NumTable[(Fuc.EnvironTemp/10)%10];	//Fuc.EnvironTemp
		Disp1650Seg[1] =  NumTable[Fuc.EnvironTemp%10]; 
		TEMP_ON();
	}
	*/
}
#endif




//100MS
void Display_Value_Set(void)
{

	u8 u8temp0 = 0;
	u16 u16TotalTime = 0;
	u8 i;
	for (i = 0; i < COM_MAX_CNT; i++)
	{
		g_u8DispSegBuf[i] = 0;
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
		SetStr((uchar *)&g_u8DispSegBuf, 0xffff, sizeof(g_u8DispSegBuf));
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


	#ifdef _DEMO_TEST_
		#warning " ------------底层测试--------------" 
		DemoTest();
	#endif
	#ifdef _SHOW_TEMP_AD_
		#warning "----------显示温度与AD----------------"
		DemoTempAD();
	#endif
	for (i = 0; i < 7; i++)
	{
		g_u8DispSeg[i] = g_u8DispSegBuf[i];
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
