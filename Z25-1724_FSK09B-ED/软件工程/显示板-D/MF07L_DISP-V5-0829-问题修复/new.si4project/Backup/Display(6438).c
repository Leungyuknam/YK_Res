#define _Native_Display_H
#include "Include\Include.h"
#include <math.h>

//unsigned int xdata PWM31_REG _at_ 0x103a;	//PWM占空比调节寄存器

u16 code SEG_NUM[] ={   
 DISP_0,DISP_1,DISP_2,DISP_3,DISP_4,DISP_5,DISP_6,DISP_7,DISP_8,DISP_9,DISP_A,DISP_b,DISP_C,DISP_d,DISP_E,DISP_F
};


#define	PSEG1	BIT0
#define	PSEG2	BIT1
#define	PSEG3	BIT2
#define	PSEG4	BIT3
#define	PSEG5	BIT4
#define	PSEG6	BIT5
//#define	PSEG7	BIT6


#define	PCOM1	0	
#define	PCOM2	1
#define PCOM3	2
#define PCOM4	3
//#define	PCOM5	4	
//#define	PCOM6	5
//#define 	PCOM7	6

//#define	DS_ALL_KEY_LED()	{g_DispSegBuf[KEY_DISP] |= 0x3ff;}

extern code DispTask_t DispTask[_DISP_TOTAL_];

/********************************************************************************************************
* @ name   : 
* @ msg    : LED显示映射
* @ param   (*)
* @ return  (*)
 ********************************************************************************************************/
enum LED_TASK_NUM
{
  L_LR,           
  L_UP_DN,
  L_TOFF_ONE,
	L_TOFF_TWO,
	L_TOFF_FOUR,
	L_FAN1,
	L_FAN2,
	L_FAN3,
	L_FAN4,
	L_FAN5,
	L_FAN6,
	L_FAN7,
	L_TON_2H,
	L_TON_4H,
	L_TON_6H,
	L_NATURE,
	L_SLEEP,
	L_LR2,
	L_LR3,
	L_UP_DN2,
	L_UP_DN3,
		_LED_TOTAL_
};

//42
code LedTask_t LedTask[_LED_TOTAL_] =
{
	//Index,			Com,			Val
	{L_LR,				PCOM3,			PSEG5},		
	{L_UP_DN,			PCOM1,			PSEG1},		 
	{L_TOFF_ONE,	PCOM4,			PSEG2},	  
	
	{L_TOFF_TWO,	PCOM3,			PSEG2},	   
	{L_TOFF_FOUR,	PCOM2,			PSEG2},	  
	{L_FAN1,			PCOM1,			PSEG2},		
	{L_FAN2,			PCOM1,			PSEG3},		
	{L_FAN3,			PCOM2,			PSEG3},		
	{L_FAN4,			PCOM3,			PSEG3},	
	{L_FAN5,			PCOM4,			PSEG3},		
	{L_FAN6,			PCOM4,			PSEG4},		 
	{L_FAN7,			PCOM3,			PSEG4},		

	
	{L_TON_2H,			PCOM2,			PSEG4},	
	{L_TON_4H,			PCOM1,			PSEG4},		
	{L_TON_6H,			PCOM1,			PSEG5},
	{L_NATURE,			PCOM4,			PSEG1},		
	{L_SLEEP,			  PCOM2,			PSEG5},		
	
	
	{L_LR2,				  PCOM4,			PSEG5},		
	{L_LR3,				  PCOM4,			PSEG6},	
	{L_UP_DN2,			PCOM2,			PSEG1},		 
	{L_UP_DN3,			PCOM3,			PSEG1},		 
};

///********************************************************************************************************
//* @ name   : 
//* @ msg    : LED显示映射
//* @ param   (*)
//* @ return  (*)
// ********************************************************************************************************/
//enum LED_TASK_NUM
//{
//    L_LR_30,
//	L_LR_60,
//	L_LR_90,
//	L_LR_180,
//    L_UD_45,
//    L_UD_90,
//    L_UD_180,
//    L_UD_360,
//    L_TIME_1H,
//    L_TIME_2H,
//    L_TIME_3H,
//    L_TIME_8H,
//	L_FAN1,
//	L_FAN2,
//	L_FAN3,
//	L_FAN4,
//	L_FAN5,
//	L_FAN6,
//	L_FAN7,	
//	L_FAN8,
//	L_NATURE,
//	L_MAX,
//	L_SLEEP,
//    _LED_TOTAL_
//};

////42
//code LedTask_t LedTask[_LED_TOTAL_] =
//{
//	//Index,			Com,			Val
//	{L_LR,				PCOM2,			PSEG1},		// {L_LR,				COM2,			PSEG1},
//	{L_UP_DN,			PCOM3,			PSEG1},		// 
//	{L_TOFF_ONE,		PCOM4,			PSEG1},		//
	
//	{L_TOFF_TWO,		PCOM1,			PSEG1},		// 
//	{L_TOFF_FOUR,		PCOM1,			PSEG2},		// 
//	{L_FAN1,			PCOM4,			PSEG2},		//
//	{L_FAN2,			PCOM3,			PSEG2},		// 
//	{L_FAN3,			PCOM2,			PSEG2},		// 
//	{L_FAN4,			PCOM2,			PSEG3},		//
//	{L_FAN5,			PCOM3,			PSEG3},		// 
//	{L_FAN6,			PCOM1,			PSEG3},		// 
//	{L_FAN7,			PCOM1,			PSEG4},		//

	
//	{L_TON_2H,			PCOM3,			PSEG4},		//
//	{L_TON_4H,			PCOM2,			PSEG4},		// 
//	{L_TON_6H,			PCOM2,			PSEG5},		// 
//	{L_NATURE,			PCOM3,			PSEG5},		//
//	{L_SLEEP,			PCOM1,			PSEG5},		//
//};

/********************************************************************************************************
* @ name   : static void F_LedEnable(unsigned char temp)
* @ msg    : LED显示
* @ param   (unsigned char) temp
* @ return  (*)
 ********************************************************************************************************/
static void F_LedEnable(unsigned char temp)
{
	if (temp < _LED_TOTAL_)
	{
		g_DispSegBuf[LedTask[temp].Com] |= LedTask[temp].Val;
	}
}

/*
static void F_LedOFF(unsigned char temp)
{
	if (temp < _LED_TOTAL_)
	{
		g_DispSegBuf[LedTask[temp].Com] &= ~LedTask[temp].Val;
	}
}
*/

static void F_DispLed(void)
{
	u8 xdata i = 0;
	if(Sys.MState == STATE_NONE)
	{
		if(T_ON_TWO == Sys.mFanOnTime)
			F_LedEnable(L_TON_2H);	
		else if(T_ON_FOUR == Sys.mFanOnTime)
			F_LedEnable(L_TON_4H);	
		else if(T_ON_SIX == Sys.mFanOnTime)
			F_LedEnable(L_TON_6H);
	}
	else
	{
		if(Sys.Fg.RLOSC)
		{
			//F_LedEnable(L_LR);
			if(Sys.mAngle == ANGLE_30)
			{
				F_LedEnable(L_LR);
			}
			else if(Sys.mAngle == ANGLE_50)
			{
				F_LedEnable(L_LR2);
			}
			else if(Sys.mAngle == ANGLE_80)
			{
				F_LedEnable(L_LR3);
			}
		}
		
		if(Sys.Fg.UDOSC)
		{
			//F_LedEnable(L_UP_DN);	
			if(Sys.mAngleUpDn == ANGLE_30)
			{
				F_LedEnable(L_UP_DN);
			}
			else if(Sys.mAngleUpDn == ANGLE_50)
			{
				F_LedEnable(L_UP_DN2);
			}
			else if(Sys.mAngleUpDn == ANGLE_80)
			{
				F_LedEnable(L_UP_DN3);
			}
		}
		
		if(T_OFF_ONE == Sys.mFanOFF_Time)
			F_LedEnable(L_TOFF_ONE);	
		else if(T_OFF_TWO == Sys.mFanOFF_Time)
			F_LedEnable(L_TOFF_TWO);	
		else if(T_OFF_FOR == Sys.mFanOFF_Time)
			F_LedEnable(L_TOFF_FOUR);	

//		if((FAN_NORMAL == Sys.Mode) && !Sys.Fg.FanSleep)
		if(FAN_NORMAL == Sys.Mode)
		{
			for(i = 1;i <= Sys.mFan; i++)
			{	
				F_LedEnable(L_TOFF_FOUR+i);
			}
		}

		#if 0
		if(T_ON_TWO == Sys.mFanOnTime)
			F_LedEnable(L_TON_2H);	
		else if(T_ON_FOUR == Sys.mFanOnTime)
			F_LedEnable(L_TON_4H);	
		else if(T_ON_SIX == Sys.mFanOnTime)
			F_LedEnable(L_TON_6H);
		#endif

//		if((FAN_NATURE == Sys.Mode) && !Sys.Fg.FanSleep)
//			F_LedEnable(L_NATURE);

//		if(Sys.Fg.FanSleep)
//			F_LedEnable(L_SLEEP);
		
		if(FAN_NATURE == Sys.Mode)
			F_LedEnable(L_NATURE);
		else if(FAN_SLEEP == Sys.Mode)
			F_LedEnable(L_SLEEP);
		else if(FAN_MAX == Sys.Mode)
			F_LedEnable(L_SLEEP);
	}
}
static void F_DispNormal(void)
{
	F_DispLed();
}

static void F_DispPowLow(void)
{
	u8 xdata i = 0;
	if(DispFg.Bri)
	{
		for (i = 0; i < DISP_LENGTH; i++)
		{
			g_DispSegBuf[i] = 0xff;
		}

	}

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
	FactoryKeyNum = Dat;
	FactoryMode_Sec = 0;
	FactoryStep = 1;
	BELL_NOR();
	FactoryStep = 1;
}

//100ms
//输出为进入产测，自动轮流输出动作
void FactoryDisplay_Value_Set(void)
{
	u8 xdata Dat = 0;
	u8 xdata i = 0;
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
			SetStr((uchar *)&g_DispSegBuf, 0xff, sizeof(g_DispSegBuf));
		break;

		case 1:
			//发送WIFI产测模式
//			if(Sys.PowMessage.PowRecSIErr || ControlDisConnect_Cnt >= 3) //|| ControlDisConnect_Cnt > 20
//			{
//				if(B_500MS_BriOn)
//				{
//					SetStr((uchar *)&g_DispSegBuf, 0xff, sizeof(g_DispSegBuf));
//				}
//			}
			if(1 == FactoryKeyNum)
			{
				Dat = g_u16Bat_Vol/1000;
				for(i = 1;i <= Dat; i++)
				{	
					F_LedEnable(L_TOFF_FOUR+i);
				}
			}
			else if(2 == FactoryKeyNum)
			{
				F_LedEnable(L_LR);
				F_LedEnable(L_UP_DN);
			}
			else if(3 == FactoryKeyNum)
			{
				F_LedEnable(L_TOFF_ONE);
				F_LedEnable(L_TOFF_TWO);
				F_LedEnable(L_TOFF_FOUR);
			}
			else if(4 == FactoryKeyNum)
			{
				F_LedEnable(L_FAN1);
				F_LedEnable(L_FAN2);
				F_LedEnable(L_FAN3);
			}
			else if(5 == FactoryKeyNum)
			{
				F_LedEnable(L_FAN4);
				F_LedEnable(L_FAN5);
			}
			else if(6 == FactoryKeyNum)
			{
				F_LedEnable(L_FAN6);
				F_LedEnable(L_FAN7);
			}
			else if(7 == FactoryKeyNum)
			{
				F_LedEnable(L_TON_2H);
				F_LedEnable(L_TON_4H);
				F_LedEnable(L_TON_6H);

			}
			else if(8 == FactoryKeyNum)
			{
				F_LedEnable(L_NATURE);
			}
			else if(9 == FactoryKeyNum)
			{
				F_LedEnable(L_SLEEP);
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
	for (i = 0; i < DISP_LENGTH; i++)
	{
		g_DispSegBuf[i] = 0;
	}
	if(g_u8PowerON_TCnt < 10)
	{
		SetStr((uchar *)&g_DispSegBuf, 0xff, sizeof(g_DispSegBuf));
	}
	else
	{
		if (GetTimeTick(TIME_1S))
			DisplayStepTest++;
		if(DisplayStepTest >= _LED_TOTAL_)
			DisplayStepTest = 0;

		F_LedEnable(DisplayStepTest);

		if(g_u32KeyBuf)
		{
			F_LedEnable(L_TON_2H);
			F_LedEnable(L_TON_4H);
			F_LedEnable(L_TON_6H);
		}
		
		if(g_u32KeyBuf == KEY_LR)
			F_LedEnable(0);
		if(g_u32KeyBuf == KEY_UP_DN)
			F_LedEnable(1);
		if(g_u32KeyBuf == KEY_T_OFF)
			F_LedEnable(2);
		if(g_u32KeyBuf == KEY_INC)
			F_LedEnable(3);
		if(g_u32KeyBuf == KEY_ONOFF)
			F_LedEnable(4);
		if(g_u32KeyBuf == KEY_DEC)
			F_LedEnable(5);
		if(g_u32KeyBuf == KEY_T_ON)
			F_LedEnable(6);
		if(g_u32KeyBuf == KEY_NATURE)
			F_LedEnable(7);
		if(g_u32KeyBuf == KEY_SLEEP_WIND)
			F_LedEnable(8);

	}


}
#endif


#ifdef _SHOW_TEMP_AD_
void DemoTempAD()
{
	if(B_500MS_BriOn)
	{
		//g_DispSegBuf[DIG1] = SEG_NUM[Sys.TempAD/16%16]; 
		//g_DispSegBuf[DIG2] = SEG_NUM[Sys.TempAD%16];
		//DS_SIGN_H();

	}
	else
	{
//		g_DispSegBuf[DIG1] = 0;
//		g_DispSegBuf[DIG2] = SEG_NUM[Sys.TempAD/256%16];
		//g_DispSegBuf[DIG1] =  SEG_NUM[(Sys.EnvironTemp/10)%10];	//Fuc.EnvironTemp
		//g_DispSegBuf[DIG2] =  SEG_NUM[Sys.EnvironTemp%10]; 
		//DS_SIGN_C();
	}
}
#endif




//100MS
void Display_Value_Set(void)
{

	u8 u8temp0 = 0;
	u16 u16TotalTime = 0;
	u8 i;
	for (i = 0; i < DISP_LENGTH; i++)
	{
		g_DispSegBuf[i] = 0;
		g_DispSegHalfBuf[i] = 0;
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
		SetStr((uchar *)&g_DispSegBuf, 0xff, sizeof(g_DispSegBuf));
		break;

	case SysMX_Run:
		
		if (DispLastTime)
		{
			DispLastTime--;
			if (DispLastTime == 0)
			{
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
		
		if(Sys.Fg.LightSleep)
		{
			for (i = 0; i < COM_MAX_CNT; i++)
				g_DispSegHalfBuf[i]  = g_DispSegBuf[i];
		}
		break;

	case SysMX_Factory:
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
	
	//显示面板的
	for (i = 0; i < COM_MAX_CNT; i++)
	{
		g_DispSegHalfBuf[i]  = ~g_DispSegHalfBuf[i];
		g_DispSegHalfBuf[i] = g_DispSegBuf[i] & g_DispSegHalfBuf[i];
	}
	DispLedBufSet(g_DispSegHalfBuf,g_DispSegBuf);
}


/********************************************************************************************************
 * @brief  获取当前显示状态
 * @param  {None}
 * @return 当前显示状态
 ********************************************************************************************************/
//unsigned char GetDispType(void)
//{
//    return DispTask[DispIndex].Index;
//}

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
			//if((DispTask[i].Index != DISP_TIME_OFF_SET) && (DispIndex == DISP_TIME_OFF_SET))
			//{
			//	Sys.Time = Sys.TimeBuf;
			//}

			
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
		//Index,				LastTime,	Blink *100ms,	Function
		{DISP_NORMAL,				0,			5,				&F_DispNormal},
		{DISP_POW_LOW,			50, 		5,				&F_DispPowLow},
		//{DISP_TIME_ON_SET,			30,			5,				&F_DispTempSet},
		//{DISP_ANGLE_SET,			30,			5,				&F_DispTime},

	};
