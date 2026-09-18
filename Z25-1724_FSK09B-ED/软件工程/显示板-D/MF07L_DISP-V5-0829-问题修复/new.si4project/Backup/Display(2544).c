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

#define	DS_ALL_KEY_LED()	{g_u8DispSegBuf[PCOM1] |= 0xfe;\
							 g_u8DispSegBuf[PCOM2] |= 0xfe;\
							 g_u8DispSegBuf[PCOM3] |= 0xfe;\
							 g_u8DispSegBuf[PCOM4] |= 0xfe;\
							 g_u8DispSegBuf[PCOM5] |= PSEG1+PSEG2;\
							}

#define	DS_CUP_LED1()		{g_u8DispSegBuf[PCOM5] |= PSEG1+PSEG2;g_u8DispSegBuf[PCOM4] |= PSEG7;}
#define	DS_DIE_LED2()		{g_u8DispSegBuf[PCOM4] |= PSEG4+PSEG5+PSEG6;}
#define	DS_WARM_LED3()		{g_u8DispSegBuf[PCOM4] |= PSEG1+PSEG2+PSEG3;}
#define	DS_TEMP_LED4()		{g_u8DispSegBuf[PCOM3] |= PSEG5+PSEG6+PSEG7;}
#define	DS_TIME_LED5()		{g_u8DispSegBuf[PCOM3] |= PSEG1+PSEG3+PSEG4;}
#define	DS_DEC_LED6()		{g_u8DispSegBuf[PCOM2] |= PSEG6+PSEG7;g_u8DispSegBuf[PCOM3] |= PSEG2;}
#define	DS_INC_LED7()		{g_u8DispSegBuf[PCOM2] |= PSEG3+PSEG4+PSEG5;}
#define	DS_PAUSE_LED8()		{g_u8DispSegBuf[PCOM2] |= PSEG1+PSEG2;g_u8DispSegBuf[PCOM1] |= PSEG7;}
#define	DS_POW_LED9()		{g_u8DispSegBuf[PCOM1] |= PSEG4+PSEG5+PSEG6;}
#define	DS_LOCK_LED10()		{g_u8DispSegBuf[PCOM1] |= PSEG1+PSEG2+PSEG3;}


#define	DS_CUP_LED1_OFF()	{g_u8DispSegBuf[PCOM5] &= ~(PSEG1+PSEG2);g_u8DispSegBuf[PCOM4] &= ~PSEG7;}
#define	DS_DIE_LED2_OFF()	{g_u8DispSegBuf[PCOM4] &= ~(PSEG4+PSEG5+PSEG6);}
#define	DS_WARM_LED3_OFF()	{g_u8DispSegBuf[PCOM4] &= ~(PSEG1+PSEG2+PSEG3);}
#define	DS_TEMP_LED4_OFF()	{g_u8DispSegBuf[PCOM3] &= ~(PSEG5+PSEG6+PSEG7);}
#define	DS_TIME_LED5_OFF()	{g_u8DispSegBuf[PCOM3] &= ~(PSEG1+PSEG3+PSEG4);}
#define	DS_DEC_LED6_OFF()	{g_u8DispSegBuf[PCOM2] &= ~(PSEG6+PSEG7);g_u8DispSegBuf[PCOM3] &= ~PSEG2;}
#define	DS_INC_LED7_OFF()	{g_u8DispSegBuf[PCOM2] &= ~(PSEG3+PSEG4+PSEG5);}
#define	DS_PAUSE_LED8_OFF()	{g_u8DispSegBuf[PCOM2] &= ~(PSEG1+PSEG2);g_u8DispSegBuf[PCOM1] &= ~PSEG7;}
#define	DS_POW_LED9_OFF()	{g_u8DispSegBuf[PCOM1] &= ~(PSEG4+PSEG5+PSEG6);}
#define	DS_LOCK_LED10_OFF()	{g_u8DispSegBuf[PCOM1] &= ~(PSEG1+PSEG2+PSEG3);}

#define	DS_SIGN_C()			{g_u8DispSegBuf[PCOM5] |= PSEG3+PSEG6;}
#define	DS_SIGN_H()			{g_u8DispSegBuf[PCOM5] |= PSEG4+PSEG5;}

#define	DS_SIGN_C_OFF()		{g_u8DispSegBuf[PCOM5] &= ~(PSEG3+PSEG6);}
#define	DS_SIGN_H_OFF()		{g_u8DispSegBuf[PCOM5] &= ~(PSEG4+PSEG5);}



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
	if(Sys.PowMessage.DoorState)
		return;			//门闭合不显示
	if(COOK_NONE == Sys.MState)
	{
		if(DispFg.Bri && Sys.Fg.Lock)
			DS_LOCK_LED10();	
		DS_POW_LED9();
	}
	else if(COOK_IDLE == Sys.MState)
	{
		DS_ALL_KEY_LED();
		if(!DispFg.Bri)
		{
			if(MENU_BAOWEN == Sys.mMenu)
			{
				DS_WARM_LED3_OFF();
			}
			else if(MENU_DIE_ZI == Sys.mMenu)
			{
				DS_DIE_LED2_OFF();
			}
			else if(MENU_CUP == Sys.mMenu)
			{
				DS_CUP_LED1_OFF();
			}
			DS_PAUSE_LED8_OFF();
			//if(DISP_TIME_OFF_SET == DispTask[DispIndex].Index)
			//	DS_TIME_LED5_OFF();
			//if(DISP_TEMP_SET == DispTask[DispIndex].Index)
			//	DS_TEMP_LED4_OFF();
			if(Sys.Fg.Lock)
				DS_LOCK_LED10_OFF();	
		}
	}
	else if(COOK_RUN_PAUSE == Sys.MState)
	{
		DS_ALL_KEY_LED();
		if(!DispFg.Bri && Sys.Fg.Lock)
		{
			DS_LOCK_LED10_OFF();
		}
		//没有选中的要熄灭

		
		if(MENU_BAOWEN == Sys.mMenu)
		{
			DS_CUP_LED1_OFF();
			DS_DIE_LED2_OFF();
		}
		else if(MENU_DIE_ZI == Sys.mMenu)
		{
			DS_CUP_LED1_OFF();
			DS_WARM_LED3_OFF();
		}
		else if(MENU_CUP == Sys.mMenu)
		{
			DS_DIE_LED2_OFF();
			DS_WARM_LED3_OFF();
		}
		if(DISP_TIME == DispTask[DispIndex].Index)
			DS_TIME_LED5_OFF();
		if(DISP_TEMP_SET == DispTask[DispIndex].Index)
			DS_TEMP_LED4_OFF();
		
		DS_DEC_LED6_OFF();
		DS_INC_LED7_OFF();

		
	}
}



void TempShow(ShowM_Enum ShowControl)
{
	if(DispFg.Bri || ShowControl)   //(FALSE == 
	{	
		if(TEMP_40C == Sys.mTempLevel)
		{
			g_u8DispSegBuf[DIG1] = DISP_4;
			g_u8DispSegBuf[DIG2] = DISP_0;
		}
		else if(TEMP_50C == Sys.mTempLevel)
		{
			g_u8DispSegBuf[DIG1] = DISP_5;
			g_u8DispSegBuf[DIG2] = DISP_0;
		}
		else if(TEMP_60C == Sys.mTempLevel)
		{
			g_u8DispSegBuf[DIG1] = DISP_6;
			g_u8DispSegBuf[DIG2] = DISP_0;
		}
		else if(TEMP_70C == Sys.mTempLevel)
		{
			g_u8DispSegBuf[DIG1] = DISP_7;
			g_u8DispSegBuf[DIG2] = DISP_0;
		}
		DS_SIGN_C();
	}
}


void ShowTime(u8 TimeDat,ShowM_Enum ShowControl)
{
	if(DispFg.Bri || ShowControl)   //(FALSE == 
	{	
		g_u8DispSegBuf[DIG1] |= SEG_NUM[TimeDat/10%10]; 
		g_u8DispSegBuf[DIG2] |= SEG_NUM[TimeDat%10]; 
		DS_SIGN_H();
	}
}



static void F_DispNormal(void)
{
	if(Sys.PowMessage.DoorState)
		return;			//门闭合不显示
	if(Sys.MState == COOK_NONE)
	{

	}
	else
	{
		if(Sys.Time)
		{
			if(Sys.MState == COOK_RUN_PAUSE)
			{
				ShowTime(Sys.Time,SHOW_ALL_ON);
			}
			else
				TempShow(SHOW_ALL_ON);	
		}
		else
		{
			if(Sys.MState == COOK_RUN_PAUSE)
			{
				ShowTime(Sys.Time,SHOW_ALL_ON);
			}
			else //if(Sys.MState != COOK_NONE)
			{
				TempShow(SHOW_ALL_ON);
			}
		}
	}
}


void F_DispTimeOFFSet(void)
{
	if(Sys.PowMessage.DoorState)
		return;			//门闭合不显示
	ShowTime(Sys.TimeBuf,SHOW_BRI_EN);

}

void F_DispTime(void)
{
	if(Sys.PowMessage.DoorState)
		return;			//门闭合不显示
	ShowTime(Sys.Time,SHOW_BRI_EN);

}


void F_DispTempSet(void)
{
	if(Sys.PowMessage.DoorState)
		return;			//门闭合不显示
	TempShow(SHOW_BRI_EN);
}


void SetFactoryMode()
{
	FactoryKeyNum = 0;
	FactoryMode_Sec = 0;
	FactoryStep = 0;
	SysMode = SysMX_Factory;
}

void FactoryKeyNumFuc(u8 Dat)
{
	if(SysMX_Factory == SysMode)
	{
		FactoryKeyNum = Dat;
		FactoryMode_Sec = 0;
		FactoryStep = 1;
		BELL_NOR();
	}
}

//100ms
//输出为进入产测，自动轮流输出动作
void FactoryDisplay_Value_Set(void)
{
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
		break;

		case 1:
			//发送WIFI产测模式
			if(Sys.PowMessage.PowRecSIErr || ControlDisConnect_Cnt > 50)
			{
				g_u8DispSegBuf[DIG1] |= DISP_E; 
				g_u8DispSegBuf[DIG2] |= DISP_8; //通信故障	
			}
			else
			{
				g_u8DispSegBuf[DIG1] = SEG_NUM[FactoryKeyNum];
				g_u8DispSegBuf[DIG2] = SEG_NUM[FactoryKeyNum];
			}
			break;
		case 2:
			g_u8DispSegBuf[DIG1] = SEG_NUM[Sys.TempAD/16];
			g_u8DispSegBuf[DIG2] = SEG_NUM[Sys.TempAD%16];
			break;
		default:

			break;
	}
}


//100ms
//输出为进入产测，自动轮流输出动作
void CustomerDispSet(void)
{
	if(GetTimeTick(TIME_1S))
	{
		if(++FactoryMode_Sec >= 20)
		{
			FactoryMode_Sec = 0; 
			SysMode = SysMX_Run;
			WorkModeNewSet(SET_STATE_NONE);
		}
	}

	switch (FactoryKeyNum) 
	{
		case 0:
			if(B_500MS_BriOn)
			{
				g_u8DispSegBuf[DIG1] = DISP_C;
				g_u8DispSegBuf[DIG2] = DISP_1;
			}
			else
			{
				g_u8DispSegBuf[DIG1] = DISP_1;
				g_u8DispSegBuf[DIG2] = DISP_5;
			}
		break;

		case 3:
			if(Sys.PowMessage.DoorState)
			{
				g_u8DispSegBuf[DIG1] = DISP_0;
				g_u8DispSegBuf[DIG2] = DISP_3;
			}
			else
			{
				g_u8DispSegBuf[DIG1] = DISP_1;
				g_u8DispSegBuf[DIG2] = DISP_1;
			}
		break;

		case 5:

		break;

		case 1:
		case 2:
		case 4:
		case 6:
		case 7:
		case 8:
			g_u8DispSegBuf[DIG1] = SEG_NUM[FactoryKeyNum/16];
			g_u8DispSegBuf[DIG2] = SEG_NUM[FactoryKeyNum%16];
			break;
		case 9:
			if(B_500MS_BriOn)
			{
				g_u8DispSegBuf[DIG1] = DISP_U;
				g_u8DispSegBuf[DIG2] = DISP_E;
			}
			else
			{
				g_u8DispSegBuf[DIG1] = DISP_1;
				g_u8DispSegBuf[DIG2] = DISP_4;
			}
			break;
			
		default:

			break;
	}
}





#ifdef _DEMO_TEST_
#warning "--------调试测试用---------"
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
	if(++g_u8PowerON_TCnt < 10)
	{
		SetStr((uchar *)&g_u8DispSegBuf, 0xff, sizeof(g_u8DispSegBuf));
	}
	else
	{
		if (GetTimeTick(TIME_1S))
			DisplayStepTest++;
		if(DisplayStepTest >= 10)
			DisplayStepTest = 0;
		g_u8DispSegBuf[DIG1] = SEG_NUM[DisplayStepTest%10]; 
		g_u8DispSegBuf[DIG2] = SEG_NUM[DisplayStepTest%10];
		if(B_500MS_BriOn)
		{
			DS_SIGN_C();
			DS_SIGN_H();
		}
		if(Sys.PowMessage.DoorState)
		{
			DS_SIGN_C();
			DS_SIGN_H();
		}
		switch(DisplayStepTest)
		{
			case 0:
				DS_CUP_LED1();

			break;
			case 1:
				DS_DIE_LED2();
				
			break;
			case 2:
				DS_WARM_LED3();
			break;
			case 3:
				DS_TEMP_LED4();
			break;
			case 4:
				DS_TIME_LED5();
			break;
			case 5:
				DS_DEC_LED6();
			break;
			case 6:
				DS_INC_LED7();
			break;
			case 7:
				DS_PAUSE_LED8();
			break;
			case 8:
				DS_POW_LED9();
			break;
			case 9:
				DS_LOCK_LED10();
			break;
		}
		if(g_u32KeyBuf == KEY_CUP)
			DS_CUP_LED1();
		if(g_u32KeyBuf == KEY_PLATE)
			DS_DIE_LED2();
		if(g_u32KeyBuf == KEY_WARM)
			DS_WARM_LED3();
		if(g_u32KeyBuf == KEY_TEMP)
			DS_TEMP_LED4();
		if(g_u32KeyBuf == KEY_TIME)
			DS_TIME_LED5();
		if(g_u32KeyBuf == KEY_DEC)
			DS_DEC_LED6();
		if(g_u32KeyBuf == KEY_INC)
			DS_INC_LED7();
		if(g_u32KeyBuf == KEY_START_PAUSE)
			DS_PAUSE_LED8();
		if(g_u32KeyBuf == KEY_POW)
			DS_POW_LED9();
		if(g_u32KeyBuf == KEY_LOCK)
			DS_LOCK_LED10();
	}
}
#endif


#ifdef _SHOW_TEMP_AD_
void DemoTempAD()
{
	g_u8DispSegBuf[DIG1] = SEG_NUM[Sys.TempAD/16%16]; 
	g_u8DispSegBuf[DIG2] = SEG_NUM[Sys.TempAD%16];
	if(Sys.PowMessage.DoorState)
	{
		DS_SIGN_C();
		DS_SIGN_H();
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
			g_u8PowerON_TCnt = 0;
			SysMode = SysMX_Run;
			#ifdef _DEMO_TEST_
				SysMode = SysMX_Factory;
		    #endif
			WorkModeNewSet(SET_STATE_NONE);
		}
		SetStr((uchar *)&g_u8DispSegBuf, 0xff, sizeof(g_u8DispSegBuf));
		g_u8DispSegBuf[DIG1] = SEG_NUM[0]; 
		g_u8DispSegBuf[DIG2] = SEG_NUM[0];
		break;

	case SysMX_Run:
		if (DispLastTime)
		{
			DispLastTime--;
			if (DispLastTime == 0)
			{
				if(Sys.MState != COOK_IDLE)
					F_DispTaskChange(DISP_NORMAL);	
			}
		}

		if (DispTask[DispIndex].Blink)
		{
			if (++DispBlink >= DispTask[DispIndex].Blink)
			{
				DispBlink =0;
				DispFg.Bri = !DispFg.Bri;
			}
		}
		(DispTask[DispIndex].DispFunc)(); //
		F_DispLed();
			
		break;

	case SysMX_Factory:
		FactoryDisplay_Value_Set();
		break;

	case SysMX_CustomerTest:
		CustomerDispSet();
		break;

	default:
		SysMode = SysMX_FstPOW_ON;   //异常处理
		break;
	}


	#ifdef _DEMO_TEST_
		#warning " ------------底层测试--------------" 
		DemoTest();
		if(Sys.PowMessage.DoorState)
		{
			DS_SIGN_C();
			DS_SIGN_H();
		}

	#endif
	#ifdef _SHOW_TEMP_AD_
		#warning "----------显示温度与AD----------------"
		DemoTempAD();


		
	#endif
	DispLedBufSet(g_u8DispSegBuf);
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
			/*
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
			*/
			if((DispTask[i].Index != DISP_TIME_OFF_SET) && (DispIndex == DISP_TIME_OFF_SET))
			{
				Sys.Time = Sys.TimeBuf;
			}

			
			DispIndex = i;
			DispLastTime = DispTask[i].LastTime;
			DispBlink = DispTask[i].Blink;
			DispFg.Bri = TRUE;
			DispBlink = 0;
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
		{DISP_TIME_OFF_SET,			30, 		5,				&F_DispTimeOFFSet},
		{DISP_TEMP_SET,				30,			5,				&F_DispTempSet},
		{DISP_TIME,					30,			5,				&F_DispTime},

	};
