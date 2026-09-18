/*********************************************************
文 件 名 : KeyEvent.C
文件说明 : 按键事件处理
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/
//头文件
#include "COMMON\AllInclude.h"

u8 g_u8DispKeyBuff = 0;
unsigned char g_u8UpKeyRepeat = 0;
signed int g_s16Input = (-40);
signed int g_s16Output = (-40);

code s8 C_s8XdataNum_0 = 0;
code s8 C_s8XdataNum_P1 = 1;
code s8 C_s8XdataNum_P2 = 2;
code s8 C_s8XdataNum_P10 = 10;
code s8 C_s8XdataNum_P18 = 18;
code s8 C_s8XdataNum_P40 = 40;
code s8 C_s8XdataNum_P60 = 60;
code s8 C_s8XdataNum_P104 = 104;
code s8 C_s8XdataNum_N10 = (-10);
code s8 C_s8XdataNum_N18 = (-18);
code s8 C_s8XdataNum_N40 = (-40);

code TemperSet_Typedef t_sTempSet[] = 
{
//   设置对象              设置变量                  变量上限值C	                 变量下限值C	               变量上限值F	                 变量下限值F
{C_MS_OBJECT_LBOX, &g_CarRfgrt.s8SetTemp_L,      &g_CarRfgrt.s8SetTemp_Top,   &g_CarRfgrt.s8SetTemp_Bot,   &g_CarRfgrt.s8SetTemp_Top,   &g_CarRfgrt.s8SetTemp_Bot,  },
{C_MS_OBJECT_E2,   &g_CarRfgrt.s8SetTemp_Top,    &C_s8XdataNum_P40,           &g_CarRfgrt.s8SetTemp_L,     &C_s8XdataNum_P104,          &g_CarRfgrt.s8SetTemp_L,    },
{C_MS_OBJECT_E1,   &g_CarRfgrt.s8SetTemp_Bot,    &g_CarRfgrt.s8SetTemp_L,     &C_s8XdataNum_N40,           &g_CarRfgrt.s8SetTemp_L,     &C_s8XdataNum_N40,          },
{C_MS_OBJECT_E6,   &g_CarRfgrt.s8TempComp_N6_L,  &C_s8XdataNum_P10,           &C_s8XdataNum_N10,           &C_s8XdataNum_P18,           &C_s8XdataNum_N18,          },
{C_MS_OBJECT_E7,   &g_CarRfgrt.s8TempComp_N12_L, &C_s8XdataNum_P10,           &C_s8XdataNum_N10,           &C_s8XdataNum_P18,           &C_s8XdataNum_N18,          },
{C_MS_OBJECT_E8,   &g_CarRfgrt.s8TempComp_N13_L, &C_s8XdataNum_P10,           &C_s8XdataNum_N10,           &C_s8XdataNum_P18,           &C_s8XdataNum_N18,          },
{C_MS_OBJECT_E3,   &g_CarRfgrt.s8StartBias_L,    &C_s8XdataNum_P10,           &C_s8XdataNum_P1,            &C_s8XdataNum_P18,           &C_s8XdataNum_P2,           },
{C_MS_OBJECT_E9,   &g_CarRfgrt.s8StopBias_L,     &C_s8XdataNum_0,             &C_s8XdataNum_N10,           &C_s8XdataNum_0,             &C_s8XdataNum_N18,          },

//{C_MS_OBJECT_RBOX, &g_CarRfgrt.s8SetTemp_R,      &g_CarRfgrt.s8SetTemp_Top,   &g_CarRfgrt.s8SetTemp_Bot,   &g_CarRfgrt.s8SetTemp_Top,   &g_CarRfgrt.s8SetTemp_Bot,  },
{C_MS_OBJECT_Eb,   &g_CarRfgrt.u8DefrostOffTime,  &C_s8XdataNum_P60,           &C_s8XdataNum_0,           &C_s8XdataNum_P60,           &C_s8XdataNum_0,          },
//{C_MS_OBJECT_EC,   &g_CarRfgrt.s8TempComp_N12_R, &C_s8XdataNum_P10,           &C_s8XdataNum_N10,           &C_s8XdataNum_P18,           &C_s8XdataNum_N18,          },
//{C_MS_OBJECT_Ed,   &g_CarRfgrt.s8TempComp_N13_R, &C_s8XdataNum_P10,           &C_s8XdataNum_N10,           &C_s8XdataNum_P18,           &C_s8XdataNum_N18,          },
{C_MS_OBJECT_EA,   &g_CarRfgrt.u8DefrostOnTime,    &C_s8XdataNum_P60,           &C_s8XdataNum_0,           &C_s8XdataNum_P60,           &C_s8XdataNum_0,           },
//{C_MS_OBJECT_EE,   &g_CarRfgrt.s8StopBias_R,     &C_s8XdataNum_0,             &C_s8XdataNum_N10,           &C_s8XdataNum_0,             &C_s8XdataNum_N18,          },

{C_MS_OBJECT_E4,   &g_CarRfgrt.u8ComProtectTime, &C_s8XdataNum_P10,           &C_s8XdataNum_0,             &C_s8XdataNum_P10,           &C_s8XdataNum_0,            },
};


void F_DispKey_Scan(void)
{
	u16 l_u8Delay = 200;
	u8 l_u8Key = 0;

	_SET_PIN_COM1_PP;
	_BS(GPIO_COM1,PIN_COM1);
	_SET_PIN_COM2_PP;
	_BS(GPIO_COM2,PIN_COM2);
	_SET_PIN_COM3_PP;
	_BS(GPIO_COM3,PIN_COM3);
	
	_SET_PIN_SEGA_PP;
	_IO_SEG_A_OFF;
	_IO_SEG_B_OFF;
	_IO_SEG_C_OFF;
	_IO_SEG_D_OFF;
	_IO_SEG_E_OFF;
    _IO_SEG_F_OFF;
	_IO_SEG_G_OFF;

	while(l_u8Delay)
	{l_u8Delay--;}

	_SET_PIN_COM1_IN_PU;
	_SET_PIN_COM2_IN_PU;
	_SET_PIN_COM3_IN_PU;

	l_u8Delay = 100;
	while(l_u8Delay)
	{l_u8Delay--;}

	if (!_TEST_KEY_UP)
	{l_u8Key |= C_KEY_UP_L;}

	if (!_TEST_KEY_DOWN)
	{l_u8Key |= C_KEY_DOWN_L;}

	if (!_TEST_KEY_ONOFF)
	{l_u8Key |= C_KEY_ONOFF;}

	_SET_PIN_COM1_IN_HI;
	_SET_PIN_COM2_IN_HI;
	_SET_PIN_COM3_IN_HI;
	
	l_u8Delay = 100;
	while(l_u8Delay)
	{l_u8Delay--;}

	if ((l_u8Key == 0) || (l_u8Key >= g_u8DispKeyBuff))
	{g_u8DispKeyBuff = l_u8Key;}
}

/*****************************************************
*函数名称：void F_TemperSet(u8 l_u8SetType)
*函数功能：温度设置
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
static void F_TemperSet(u8 l_u8SetType)
{
	u8  i;
	s8  l_s8TempTopC,l_s8TempBotC,l_s8TempTopF,l_s8TempBotF;
	u8  l_u8Length = (sizeof(t_sTempSet)/sizeof(t_sTempSet[0]));
	s8  **pps8SetVariate;

	for (i = 0; i < l_u8Length; i++)
	{
		if (g_sModeSet.u8Object == t_sTempSet[i].u8Object)
		{
#if (Def_Machine_Type == C_Machine_Type_DoubleBox_LeftMaster)
			if ((g_CarRfgrt.u8BoxEn == C_BOX_ONLY_LEFT_EN) && (g_sModeSet.u8Object == C_MS_OBJECT_LBOX))
			{
				l_s8TempBotF = g_CarRfgrt.s8SetTemp_Bot;
				l_s8TempBotC = g_CarRfgrt.s8SetTemp_Bot;
			}
			else
			{
				pps8SetVariate = &(t_sTempSet[i].s8BottomC);
				l_s8TempBotC = (**pps8SetVariate);
				pps8SetVariate = &(t_sTempSet[i].s8BottomF);
				l_s8TempBotF = (**pps8SetVariate);
			}

			if ((g_CarRfgrt.u8BoxEn == C_BOX_ONLY_LEFT_EN) && (g_sModeSet.u8Object == C_MS_OBJECT_E1))
			{
				l_s8TempTopC = g_CarRfgrt.s8SetTemp_L;
				l_s8TempTopF = g_CarRfgrt.s8SetTemp_L;
			}
			else
			{
				pps8SetVariate = &(t_sTempSet[i].s8TopC);
				l_s8TempTopC = (**pps8SetVariate);
				pps8SetVariate = &(t_sTempSet[i].s8TopF);
				l_s8TempTopF = (**pps8SetVariate);
			}
#else

			pps8SetVariate = &(t_sTempSet[i].s8BottomC);
			l_s8TempBotC = (**pps8SetVariate);
			pps8SetVariate = &(t_sTempSet[i].s8BottomF);
			l_s8TempBotF = (**pps8SetVariate);

			pps8SetVariate = &(t_sTempSet[i].s8TopC);
			l_s8TempTopC = (**pps8SetVariate);
			pps8SetVariate = &(t_sTempSet[i].s8TopF);
			l_s8TempTopF = (**pps8SetVariate);

#endif

			pps8SetVariate = &(t_sTempSet[i].s8Variate);

			if (l_u8SetType == C_SET_INCREASE)
			{
				(**pps8SetVariate)++;

				if (g_CarRfgrt.u8TempUnit == C_Unit_Celsius)
				{
					if ((**pps8SetVariate) > l_s8TempTopC)
					{(**pps8SetVariate) = l_s8TempTopC;}
				}
				else
				{
					if ((**pps8SetVariate) > l_s8TempTopF)
					{(**pps8SetVariate) = l_s8TempTopF;}
				}
			}
			else //if (l_u8SetType == C_SET_DECREASE)
			{
				if (g_CarRfgrt.u8TempUnit == C_Unit_Celsius)
				{
					if ((**pps8SetVariate) > l_s8TempBotC)
					{(**pps8SetVariate)--;}
				}
				else
				{
					if ((**pps8SetVariate) > l_s8TempBotF)
					{(**pps8SetVariate)--;}
				}			
			}	
		}
	}

#if (Def_Machine_Type == C_Machine_Type_DoubleBox_LeftMaster)
	if (g_sModeSet.u8Object == C_MS_OBJECT_RBOX)
	{
		if (g_CarRfgrt.s8SetTemp_L < g_CarRfgrt.s8SetTemp_R)
		{
			g_CarRfgrt.s8SetTemp_L = g_CarRfgrt.s8SetTemp_R;
		}
	}
#elif (Def_Machine_Type == C_Machine_Type_DoubleBox_RightMaster)
	if (g_sModeSet.u8Object == C_MS_OBJECT_RBOX)
	{
		if (g_CarRfgrt.s8SetTemp_R < g_CarRfgrt.s8SetTemp_L)
		{
			g_CarRfgrt.s8SetTemp_R = g_CarRfgrt.s8SetTemp_L;
		}
	}
#endif

}



/*****************************************************
*函数名称：void F_CharToggle(unsigned char *pu8SetVariate, unsigned char l_u8Status1, unsigned char l_u8Status2)
*函数功能：Char变量值翻转
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
static void F_CharToggle(unsigned char *pu8SetVariate, unsigned char l_u8Status1, unsigned char l_u8Status2)
{
	if (*pu8SetVariate == l_u8Status1)
	{
		*pu8SetVariate = l_u8Status2;	
	}
	else
	{
		*pu8SetVariate = l_u8Status1;	
	}
}


/*****************************************************
*函数名称：void F_AdvancedSetup(u8 l_u8HandleType)
*函数功能：高级设置
*输入参数：l_u8HandleType（设置类型）
*返回值：  无
*说明：	   
*****************************************************/
static void F_AdvancedSetup(u8 l_u8HandleType)
{

	if ((g_sModeSet.u8Status >= C_MS_STATUS_E1) && (g_sModeSet.u8Status <= C_MS_STATUS_END))
	{
		g_sModeSet.u8Time  = C_MS_TIME_5S;

		if ((l_u8HandleType == C_ADVSET_UP) || (l_u8HandleType == C_ADVSET_DOWN))
		{
			if (g_sModeSet.u8Object < C_MS_OBJECT_BIAS)
			{
				g_sModeSet.u8Object = g_sModeSet.u8Status + C_MS_OBJECT_BIAS;
			}	
		}

		else if (l_u8HandleType == C_ADVSET_SET)
		{
//			if (g_sModeSet.u8Object < C_MS_OBJECT_BIAS)
			{
				if (++g_sModeSet.u8Status > C_MS_STATUS_END)
				{g_sModeSet.u8Status = C_MS_STATUS_E1;}

			#if Def_AdvancedSetDisp
				g_sModeSet.u8Object = g_sModeSet.u8Status + C_MS_OBJECT_BIAS;
			#else
			    g_sModeSet.u8Object = C_MS_OBJECT_NONE;
			#endif

			}
//			else
//			{
//			#if Def_AdvancedSet
//				g_sModeSet.u8Object = C_MS_OBJECT_NONE;
//				g_sModeSet.u8Status++;
//			#else
//				g_sModeSet.u8Object = g_sModeSet.u8Status - C_MS_OBJECT_BIAS;
//			#endif	
//			}	
		}

		else 
		{
		
		}
	}


#if (Def_Machine_Type > 0)

	else if ((g_sModeSet.u8Status == C_MS_STATUS_TpCh) || (g_sModeSet.u8Status == C_MS_STATUS_ValveCh))
	{
		if (l_u8HandleType == C_ADVSET_SET)
		{
			if (g_sModeSet.u8Status == C_MS_STATUS_TpCh)
			{
				if (l_u8HandleType == C_ADVSET_SET)
				{
					g_sModeSet.u8Time   = C_MS_TIME_5S;
					g_sModeSet.u8Status = C_MS_STATUS_ValveCh;
				}
			}
		
			else if (g_sModeSet.u8Status == C_MS_STATUS_ValveCh)
			{
				if (l_u8HandleType == C_ADVSET_SET)
				{
					g_sModeSet.u8Time   = C_MS_TIME_5S;
					g_sModeSet.u8Status = C_MS_STATUS_TpCh;
				}
			}	
		}

		else if (l_u8HandleType == C_ADVSET_UP)
		{
			g_sModeSet.u8Time   = C_MS_TIME_5S;	
		}
	}

#endif

}

/*****************************************************
*函数名称：void F_FactoryDataReset(u8 l_u8PowerStatus, u8 l_u8AllReset)
*函数功能：恢复出厂设置
*调用周期：无
*输入参数：无
*返回值：  无
*说明：	   
*****************************************************/
void F_FactoryDataReset(u8 l_u8PowerStatus, u8 l_u8AllReset)
{
	g_CarRfgrt.u8OnOff           =  l_u8PowerStatus;

	g_CarRfgrt.u8ComProtectTime  =  0;
	g_CarRfgrt.u8BatProtectLv    =  C_Protect_Mid;
	g_CarRfgrt.u8SpeedLv         =  C_Speed_MAX;
	g_CarRfgrt.u8BoxEn           =  C_BOX_ALL_EN;

#if (Def_TempUnit == 0)
	#warning "带电量显示，摄氏度出厂"
	g_CarRfgrt.u8TempUnit        =  C_Unit_Celsius;
	g_CarRfgrt.s8SetTemp_L       =  (-6);
	g_CarRfgrt.s8SetTemp_Top     =  (20);
	g_CarRfgrt.s8SetTemp_Bot     =  (-6);
	g_CarRfgrt.s8TempComp_N6_L   =  (1);
	g_CarRfgrt.s8TempComp_N12_L  =  (0);
	g_CarRfgrt.s8TempComp_N13_L  =  (0);
	g_CarRfgrt.s8StartBias_L     =  (2);
	g_CarRfgrt.s8StopBias_L      =  (0);

	g_CarRfgrt.s8SetTemp_R       =  (-20);
	g_CarRfgrt.s8TempComp_N6_R   =  (0);
	g_CarRfgrt.s8TempComp_N12_R  =  (0);
	g_CarRfgrt.s8TempComp_N13_R  =  (0);
	g_CarRfgrt.s8StartBias_R     =  (2);
	g_CarRfgrt.s8StopBias_R      =  (0);
	
	g_CarRfgrt.u8DefrostOnTime   =  (5);
	g_CarRfgrt.u8DefrostOffTime  =  (10);
#else

	#warning "无电量显示，华氏度出厂"
	g_CarRfgrt.u8TempUnit        =  C_Unit_Fahrenheit;
	g_CarRfgrt.s8SetTemp_L       =  (-4);
	g_CarRfgrt.s8SetTemp_Top     =  (68);
	g_CarRfgrt.s8SetTemp_Bot     =  (-4);
	g_CarRfgrt.s8TempComp_N6_L   =  (0);
	g_CarRfgrt.s8TempComp_N12_L  =  (0);
	g_CarRfgrt.s8TempComp_N13_L  =  (0);
	g_CarRfgrt.s8StartBias_L     =  (4);
	g_CarRfgrt.s8StopBias_L      =  (0);

	g_CarRfgrt.s8SetTemp_R       =  (-4);
	g_CarRfgrt.s8TempComp_N6_R   =  (0);
	g_CarRfgrt.s8TempComp_N12_R  =  (0);
	g_CarRfgrt.s8TempComp_N13_R  =  (0);
	g_CarRfgrt.s8StartBias_R     =  (4);
	g_CarRfgrt.s8StopBias_R      =  (0);
#endif
	
	g_CarRfgrt.u8CloseFlag_L     =  0;

	if (l_u8AllReset)
	{
		g_CarRfgrt.u8CompError   = C_ERRO_NULL;
		g_CarRfgrt.u8PowerSource = C_Power_Adaptor;
		g_CarRfgrt.u8SpeedDrv    = C_Speed_STOP;
		g_CarRfgrt.u8AppLock     = C_APP_LOCK_DISABLE;
		g_CarRfgrt.u8BleStatus   = C_BLE_MISS;
		g_CarRfgrt.u8Charge      = C_Disable;
		g_CarRfgrt.u8BleBindTime = 0;

		g_CarRfgrt.u8NtcToward       =  C_Toward_Forward;
		g_CarRfgrt.u8ValveToward     =  C_Toward_Forward;
		g_CarRfgrt.u8ValveStatus     =  C_VALVE_TURN_NONE;

		g_CarRfgrt.s8RoomTemp_L      =  (-40);
		g_CarRfgrt.s8RoomTemp_R      =  (-40);
		g_CarRfgrt.u16FullDisp = 22500;
		
		g_CarRfgrt.s8DispTemp_L      =  (-40);
		g_CarRfgrt.s8DispTemp_R      =  (-40);
	}
	
	g_CarRfgrt.u8LastComProtectTime = g_CarRfgrt.u8ComProtectTime;
													
}

/*****************************************************
*函数名称：void F_TemperTranslate_C2F(void)
*函数功能：摄氏转华氏
*调用周期：无
*输入参数：无
*返回值：  无
*说明：	   
*****************************************************/
void F_TemperTranslate_C2F(void)
{
	g_CarRfgrt.s8DispTemp_L       =   F_C2F(g_CarRfgrt.s8DispTemp_L);
	g_CarRfgrt.s8RoomTemp_L       =   F_C2F(g_CarRfgrt.s8RoomTemp_L);
	g_CarRfgrt.s8SetTemp_L        =   F_C2F(g_CarRfgrt.s8SetTemp_L);
	g_CarRfgrt.s8SetTemp_Top      =   F_C2F(g_CarRfgrt.s8SetTemp_Top);
	g_CarRfgrt.s8SetTemp_Bot      =   F_C2F(g_CarRfgrt.s8SetTemp_Bot);
	g_CarRfgrt.s8TempComp_N6_L    =   F_Relative_C2F(g_CarRfgrt.s8TempComp_N6_L);
	g_CarRfgrt.s8TempComp_N12_L   =   F_Relative_C2F(g_CarRfgrt.s8TempComp_N12_L);
	g_CarRfgrt.s8TempComp_N13_L   =   F_Relative_C2F(g_CarRfgrt.s8TempComp_N13_L);
	g_CarRfgrt.s8StartBias_L      =   F_Relative_C2F(g_CarRfgrt.s8StartBias_L);
	g_CarRfgrt.s8StopBias_L       =   F_Relative_C2F(g_CarRfgrt.s8StopBias_L);

	g_CarRfgrt.s8DispTemp_R       =   F_C2F(g_CarRfgrt.s8DispTemp_R);
	g_CarRfgrt.s8RoomTemp_R       =   F_C2F(g_CarRfgrt.s8RoomTemp_R);
	g_CarRfgrt.s8SetTemp_R        =   F_C2F(g_CarRfgrt.s8SetTemp_R);
	g_CarRfgrt.s8TempComp_N6_R    =   F_Relative_C2F(g_CarRfgrt.s8TempComp_N6_R);
	g_CarRfgrt.s8TempComp_N12_R   =   F_Relative_C2F(g_CarRfgrt.s8TempComp_N12_R);
	g_CarRfgrt.s8TempComp_N13_R   =   F_Relative_C2F(g_CarRfgrt.s8TempComp_N13_R);
	g_CarRfgrt.s8StartBias_R      =   F_Relative_C2F(g_CarRfgrt.s8StartBias_R);
	g_CarRfgrt.s8StopBias_R       =   F_Relative_C2F(g_CarRfgrt.s8StopBias_R);
}


/*****************************************************
*函数名称：void F_TemperTranslate_F2C(void)
*函数功能：华氏转摄氏
*调用周期：无
*输入参数：无
*返回值：  无
*说明：	   
*****************************************************/
void F_TemperTranslate_F2C(void)
{
	g_CarRfgrt.s8DispTemp_L       =   F_F2C(g_CarRfgrt.s8DispTemp_L);	
	g_CarRfgrt.s8RoomTemp_L       =   F_F2C(g_CarRfgrt.s8RoomTemp_L);	
	g_CarRfgrt.s8SetTemp_L        =   F_F2C(g_CarRfgrt.s8SetTemp_L);
	g_CarRfgrt.s8SetTemp_Top      =   F_F2C(g_CarRfgrt.s8SetTemp_Top);
	g_CarRfgrt.s8SetTemp_Bot      =   F_F2C(g_CarRfgrt.s8SetTemp_Bot);
	g_CarRfgrt.s8TempComp_N6_L    =   F_Relative_F2C(g_CarRfgrt.s8TempComp_N6_L);
	g_CarRfgrt.s8TempComp_N12_L   =   F_Relative_F2C(g_CarRfgrt.s8TempComp_N12_L);
	g_CarRfgrt.s8TempComp_N13_L   =   F_Relative_F2C(g_CarRfgrt.s8TempComp_N13_L);
	g_CarRfgrt.s8StartBias_L      =   F_Relative_F2C(g_CarRfgrt.s8StartBias_L);
	g_CarRfgrt.s8StopBias_L       =   F_Relative_F2C(g_CarRfgrt.s8StopBias_L);

	g_CarRfgrt.s8DispTemp_R       =   F_F2C(g_CarRfgrt.s8DispTemp_R);
	g_CarRfgrt.s8RoomTemp_R       =   F_F2C(g_CarRfgrt.s8RoomTemp_R);
	g_CarRfgrt.s8SetTemp_R        =   F_F2C(g_CarRfgrt.s8SetTemp_R);
	g_CarRfgrt.s8TempComp_N6_R    =   F_Relative_F2C(g_CarRfgrt.s8TempComp_N6_R);
	g_CarRfgrt.s8TempComp_N12_R   =   F_Relative_F2C(g_CarRfgrt.s8TempComp_N12_R);
	g_CarRfgrt.s8TempComp_N13_R   =   F_Relative_F2C(g_CarRfgrt.s8TempComp_N13_R);
	g_CarRfgrt.s8StartBias_R      =   F_Relative_F2C(g_CarRfgrt.s8StartBias_R);
	g_CarRfgrt.s8StopBias_R       =   F_Relative_F2C(g_CarRfgrt.s8StopBias_R);
}


/*****************************************************
*函数名称：F_KeyScan(void)
*函数功能：键值扫描
*调用周期：10ms
*输入参数：无
*返回值：  键值
*说明：
*****************************************************/
#if (KEY_NUMBERS > 16)		
u32 F_KeyScan(void)
#elif (KEY_NUMBERS > 8)
u16 F_KeyScan(void)
#else
u8 F_KeyScan(void)
#endif
{

	#if (KEY_NUMBERS > 16)
	u32  l_uReadKey = 0;
	static u32 s_uReadKey = 0;
	#elif (KEY_NUMBERS > 8)
	u16  l_uReadKey = 0;
	static u16 s_uReadKey = 0;
	#else
    u8   l_uReadKey = 0;
	static u8 s_uReadKey = 0;
	#endif

#if Def_SocTouchKey
	 
    static u32 xdata s_u32ReadKey = 0;
		   u32 xdata l_u32KeyIoNum = 0x01;

	if(SOCAPI_TouchKeyStatus & 0x80)	    //重要步骤2:  触摸键扫描一轮标志，是否调用TouchKeyScan()一定要根据此标志位置起后
	{	 		  																	
		SOCAPI_TouchKeyStatus &= 0x7f;	//重要步骤3: 清除标志位， 需要外部清除。													    
		s_u32ReadKey = TouchKeyScan();  //按键数据处理函数    
		TouchKeyRestart();				//启动下一轮转换 																																 			
	}

    if (s_u32ReadKey & (l_u32KeyIoNum << C_KEY_ONOFF_NUMBER))
    {l_uReadKey |= C_KEY_ONOFF;}

    if (s_u32ReadKey & (l_u32KeyIoNum << C_KEY_SET_NUMBER))
    {l_uReadKey |= C_KEY_SET;}

    if (s_u32ReadKey & (l_u32KeyIoNum << C_KEY_UP_L_NUMBER))
    {l_uReadKey |= C_KEY_UP_L;}

    if (s_u32ReadKey & (l_u32KeyIoNum << C_KEY_DOWN_L_NUMBER))
    {l_uReadKey |= C_KEY_DOWN_L;}

    if (s_u32ReadKey & (l_u32KeyIoNum << C_KEY_UP_R_NUMBER))
    {l_uReadKey |= C_KEY_UP_R;}

    if (s_u32ReadKey & (l_u32KeyIoNum << C_KEY_DOWN_R_NUMBER))
    {l_uReadKey |= C_KEY_DOWN_R;}

#else  
    
	if (!_TEST_KEY_UP)
	{l_uReadKey |= C_KEY_UP_L;}

	if (!_TEST_KEY_DOWN)
	{l_uReadKey |= C_KEY_DOWN_L;}
    
//	if (!_TEST_KEY_SET)
//	{l_uReadKey |= C_KEY_SET;}

	if (!_TEST_KEY_ONOFF)
	{l_uReadKey |= C_KEY_ONOFF;}

#endif 

   if ((l_uReadKey == 0) || (l_uReadKey >= s_uReadKey))
   {
   		s_uReadKey = l_uReadKey;
   }

   return s_uReadKey;
}


/*****************************************************
*函数名称：void F_PowerOnOff(void)
*函数功能：开关机事件
*输入参数：无
*输出参数：无
*说明：短按开关键触发
*****************************************************/
void F_PowerOn(void)
{
	_BUZZER_SHORT_BEEP;

//	if (g_u8SelfCheckFlag)
//	{
//		F_BackOutSelfCheck();
//		return ;
//	}

	if (g_CarRfgrt.u8BleBindTime)
	{
//		g_CarRfgrt.u8BleBindTime = 0;
//		g_CarRfgrt.u8BleStatus = C_BLE_IDLE;
//		F_AddActiveSendFun(F_UpLoad_Bind,++g_u8AppSn);
		return ;	
	}

	if (g_CarRfgrt.u8AppLock == C_APP_LOCK_ENABLE)
	{return ;}

//	if (g_CarRfgrt.u8OnOff == C_Power_ON)
//	{return ;}

	if (g_CarRfgrt.u8OnOff == C_Power_ON)
	{
		g_sModeSet.u8Status = C_MS_STATUS_IDLE;
		g_sModeSet.u8Object = C_MS_OBJECT_NONE;
		g_sModeSet.u8Time   = C_MS_TIME_0S;	
		g_CarRfgrt.u8OnOff = C_Power_OFF;
	}
//	else
//	{
//		if ((g_sModeSet.u8Status >= C_MS_STATUS_E1) && (g_sModeSet.u8Status <= C_MS_STATUS_END))
//		{
//			F_AdvancedSetup(C_ADVSET_SET);
//		}
//		else
//		{
//			g_sModeSet.u8Status = C_MS_STATUS_IDLE;
//			g_sModeSet.u8Object = C_MS_OBJECT_NONE;
//			g_sModeSet.u8Time   = C_MS_TIME_0S;
//			g_CarRfgrt.u8OnOff = C_Power_ON;		
//		}
//	}

}

void F_PowerOff(void)
{
	_BUZZER_SHORT_BEEP;

	if (g_u8SelfCheckFlag)
	{
		F_BackOutSelfCheck();
		return ;
	}

	if (g_CarRfgrt.u8BleBindTime)
	{
		g_CarRfgrt.u8BleBindTime = 0;
		g_CarRfgrt.u8BleStatus = C_BLE_IDLE;
		F_AddActiveSendFun(F_UpLoad_Bind,++g_u8AppSn);
		return ;	
	}
	
	if (g_CarRfgrt.u8BleBindTime)
	{return ;}

	if (g_CarRfgrt.u8AppLock == C_APP_LOCK_ENABLE)
	{return ;}

//	if (g_CarRfgrt.u8OnOff == C_Power_ON)
//	{return ;}

	if ((g_sModeSet.u8Status >= C_MS_STATUS_E1) && (g_sModeSet.u8Status <= C_MS_STATUS_END))
	{
		F_AdvancedSetup(C_ADVSET_SET);
	}
	else
	{
		g_sModeSet.u8Status = C_MS_STATUS_IDLE;
		g_sModeSet.u8Object = C_MS_OBJECT_NONE;
		g_sModeSet.u8Time   = C_MS_TIME_0S;
		
		if (g_CarRfgrt.u8OnOff == C_Power_ON)
		{
			g_CarRfgrt.u8OnOff = C_Power_OFF;
		}
		else
		{
			g_CarRfgrt.u8OnOff = C_Power_ON;
		}			
	}
	
}

static void F_TemperUnit_Toggle(void)
{
	if (g_sModeSet.u8Object == C_MS_OBJECT_E5)
	{
		if (g_CarRfgrt.u8TempUnit == C_Unit_Celsius)
		{
			g_CarRfgrt.u8TempUnit = C_Unit_Fahrenheit;
			F_TemperTranslate_C2F();
		}
		else
		{
			g_CarRfgrt.u8TempUnit = C_Unit_Celsius;
			F_TemperTranslate_F2C();
		}
	}
}

/*****************************************************
*函数名称：void F_DownSet(void)
*函数功能：下调节事件
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_DownSet(void)
{
//	_BUZZER_SHORT_BEEP;

	if (g_u8SelfCheckFlag)
	{
		F_DispToggle();
		return ;
	}

	if (g_CarRfgrt.u8BleBindTime)
	{return ;}

	if (g_CarRfgrt.u8AppLock == C_APP_LOCK_ENABLE)
	{return ;}

	F_SetStaticDispTime();

	if ((g_sModeSet.u8Object) && (g_sModeSet.u8Object != C_MS_STATUS_CTRL_RBOX))
	{
		F_TemperSet(C_SET_DECREASE);
		F_TemperUnit_Toggle();
	}

	if (g_CarRfgrt.u8OnOff == C_Power_ON)
	{
		if ((g_sModeSet.u8Status == C_MS_STATUS_IDLE) || (g_sModeSet.u8Object == C_MS_STATUS_CTRL_RBOX))
		{
//			if (g_CarRfgrt.u8BoxEn == C_BOX_ONLY_RIGHT_EN)
//			{
//				g_sModeSet.u8Object = C_MS_OBJECT_RBOX;
//				g_sModeSet.u8Status = C_MS_STATUS_CTRL_RBOX;			
//			}
//			else
//			if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_RIGHT_EN)
			{
				g_sModeSet.u8Object = C_MS_OBJECT_LBOX;
				g_sModeSet.u8Status = C_MS_STATUS_CTRL_LBOX;
			}
		}
		else if (g_sModeSet.u8Status == C_MS_STATUS_PROTECT)
		{
			if (g_CarRfgrt.u8BatProtectLv == C_Protect_Low)
			{g_CarRfgrt.u8BatProtectLv = C_Protect_Mid;}
			else if (g_CarRfgrt.u8BatProtectLv == C_Protect_Mid)
			{g_CarRfgrt.u8BatProtectLv = C_Protect_High;}
			else
			{g_CarRfgrt.u8BatProtectLv = C_Protect_Low;}				
		}
		else if (g_sModeSet.u8Status == C_MS_STATUS_SPEED)
		{
			if (g_CarRfgrt.u8SpeedLv == C_Speed_MAX)
			{g_CarRfgrt.u8SpeedLv = C_Speed_ECO;}
			else
			{g_CarRfgrt.u8SpeedLv = C_Speed_MAX;}				
		}		
		g_sModeSet.u8Time   = C_MS_TIME_3S;
	}
	else
	{
		if (g_sModeSet.u8Object == C_MS_OBJECT_NONE)
		{
			F_AdvancedSetup(C_ADVSET_DOWN);       //菜单选择
		}
		else
		{
			F_AdvancedSetup(C_ADVSET_RELOAD);     //重载设置时间
		}
	}	
}

/*****************************************************
*函数名称：void F_LongDownSet(void)
*函数功能：左箱长减
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_LongDownSet(void)
{
	if (g_u8SelfCheckFlag)
	{return ;}

	if (g_CarRfgrt.u8BleBindTime)
	{return ;}

	if (g_CarRfgrt.u8AppLock == C_APP_LOCK_ENABLE)
	{return ;}

	F_SetStaticDispTime();

	if ((g_sModeSet.u8Object) && (g_sModeSet.u8Object != C_MS_STATUS_CTRL_RBOX))
	{
		F_TemperSet(C_SET_DECREASE);
	}

	if (g_CarRfgrt.u8OnOff == C_Power_ON)
	{
		if (g_sModeSet.u8Status == C_MS_STATUS_IDLE)
		{
//			if (g_CarRfgrt.u8BoxEn == C_BOX_ONLY_RIGHT_EN)
//			{
//				g_sModeSet.u8Object = C_MS_OBJECT_RBOX;
//				g_sModeSet.u8Status = C_MS_STATUS_CTRL_RBOX;			
//			}
//			else
//			if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_RIGHT_EN)
			{
				g_sModeSet.u8Object = C_MS_OBJECT_LBOX;
				g_sModeSet.u8Status = C_MS_STATUS_CTRL_LBOX;
			}
		}
		g_sModeSet.u8Time   = C_MS_TIME_3S;
	}
	else
	{
		if (g_sModeSet.u8Object == C_MS_OBJECT_NONE)
		{
			F_AdvancedSetup(C_ADVSET_DOWN);       //菜单选择
		}
		else
		{
			F_AdvancedSetup(C_ADVSET_RELOAD);     //重载设置时间
		}
	}
}

/*****************************************************
*函数名称：void F_DownSet_R(void)
*函数功能：下调节事件
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_DownSet_R(void)
{
	_BUZZER_SHORT_BEEP;

	if (g_u8SelfCheckFlag)
	{
		F_DispToggle();
		return ;
	}

	if (g_CarRfgrt.u8BleBindTime)
	{return ;}

	if (g_CarRfgrt.u8BoxEn == C_BOX_ONLY_LEFT_EN)
	{return ;}

	if (g_CarRfgrt.u8AppLock == C_APP_LOCK_ENABLE)
	{return ;}

	F_SetStaticDispTime();

	if ((g_sModeSet.u8Object) && (g_sModeSet.u8Object != C_MS_STATUS_CTRL_LBOX))
	{
		F_TemperSet(C_SET_DECREASE);
		F_TemperUnit_Toggle();
	}

	if (g_CarRfgrt.u8OnOff == C_Power_ON)
	{
		if ((g_sModeSet.u8Status == C_MS_STATUS_IDLE) || (g_sModeSet.u8Object == C_MS_STATUS_CTRL_LBOX))
		{
			g_sModeSet.u8Object = C_MS_OBJECT_RBOX;
			g_sModeSet.u8Status = C_MS_STATUS_CTRL_RBOX;			
		}
		g_sModeSet.u8Time   = C_MS_TIME_3S;
	}
	else
	{
		if (g_sModeSet.u8Object == C_MS_OBJECT_NONE)
		{
			F_AdvancedSetup(C_ADVSET_DOWN);       //菜单选择
		}
		else
		{
			F_AdvancedSetup(C_ADVSET_RELOAD);     //重载设置时间
		}
	}	
}

/*****************************************************
*函数名称：void F_LongDownSet_R(void)
*函数功能：右箱长减
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_LongDownSet_R(void)
{
	if (g_u8SelfCheckFlag)
	{return ;}

	if (g_CarRfgrt.u8BleBindTime)
	{return ;}

	if (g_CarRfgrt.u8BoxEn == C_BOX_ONLY_LEFT_EN)
	{return ;}

	if (g_CarRfgrt.u8AppLock == C_APP_LOCK_ENABLE)
	{return ;}

	F_SetStaticDispTime();

	if ((g_sModeSet.u8Object) && (g_sModeSet.u8Object != C_MS_STATUS_CTRL_LBOX))
	{
		F_TemperSet(C_SET_DECREASE);
	}

	if (g_CarRfgrt.u8OnOff == C_Power_ON)
	{
		if (g_sModeSet.u8Status == C_MS_STATUS_IDLE)
		{
			g_sModeSet.u8Object = C_MS_OBJECT_RBOX;
			g_sModeSet.u8Status = C_MS_STATUS_CTRL_RBOX;			
		}
		g_sModeSet.u8Time   = C_MS_TIME_3S;
	}
	else
	{
		if (g_sModeSet.u8Object == C_MS_OBJECT_NONE)
		{
			F_AdvancedSetup(C_ADVSET_DOWN);       //菜单选择
		}
		else
		{
			F_AdvancedSetup(C_ADVSET_RELOAD);     //重载设置时间
		}
	}
}

/*****************************************************
*函数名称：void F_UpSet(void)
*函数功能：上调事件
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_UpSet(void)
{
//	_BUZZER_SHORT_BEEP;

	if (g_u8SelfCheckFlag)
	{
		F_ManualCheck();
		return ;
	}

	if (g_CarRfgrt.u8BleBindTime)
	{
		g_CarRfgrt.u8BleBindTime = 0;
		g_CarRfgrt.u8BleStatus = C_BLE_BIND;
		F_AddActiveSendFun(F_UpLoad_Bind,++g_u8AppSn);
		return ;	
	}
	
	if (g_CarRfgrt.u8BleBindTime)
	{return ;}

	if (g_CarRfgrt.u8AppLock == C_APP_LOCK_ENABLE)
	{return ;}

	F_SetStaticDispTime();

	if ((g_sModeSet.u8Object) && (g_sModeSet.u8Object != C_MS_STATUS_CTRL_RBOX))
	{
		F_TemperSet(C_SET_INCREASE);
		F_TemperUnit_Toggle();
	}

	if (g_CarRfgrt.u8OnOff == C_Power_ON)
	{
		if ((g_sModeSet.u8Status == C_MS_STATUS_IDLE) || (g_sModeSet.u8Object == C_MS_STATUS_CTRL_RBOX))
		{
//			if (g_CarRfgrt.u8BoxEn == C_BOX_ONLY_RIGHT_EN)
//			{
//				g_sModeSet.u8Object = C_MS_OBJECT_RBOX;
//				g_sModeSet.u8Status = C_MS_STATUS_CTRL_RBOX;			
//			}
//			else
//			if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_RIGHT_EN)
			{
				g_sModeSet.u8Object = C_MS_OBJECT_LBOX;
				g_sModeSet.u8Status = C_MS_STATUS_CTRL_LBOX;
			}
		}
		else if (g_sModeSet.u8Status == C_MS_STATUS_PROTECT)
		{
			if (g_CarRfgrt.u8BatProtectLv == C_Protect_Low)
			{g_CarRfgrt.u8BatProtectLv = C_Protect_Mid;}
			else if (g_CarRfgrt.u8BatProtectLv == C_Protect_Mid)
			{g_CarRfgrt.u8BatProtectLv = C_Protect_High;}
			else
			{g_CarRfgrt.u8BatProtectLv = C_Protect_Low;}				
		}
		else if (g_sModeSet.u8Status == C_MS_STATUS_SPEED)
		{
			if (g_CarRfgrt.u8SpeedLv == C_Speed_MAX)
			{g_CarRfgrt.u8SpeedLv = C_Speed_ECO;}
			else
			{g_CarRfgrt.u8SpeedLv = C_Speed_MAX;}				
		}
		g_sModeSet.u8Time   = C_MS_TIME_3S;
	}

	else
	{
		if (g_sModeSet.u8Object == C_MS_OBJECT_NONE)
		{
			F_AdvancedSetup(C_ADVSET_UP);       //菜单选择
		}
		else
		{
			F_AdvancedSetup(C_ADVSET_RELOAD);   //重载设置时间
		}
	}	
}

/*****************************************************
*函数名称：void F_LongUpSet(void)
*函数功能：长加事件
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_LongUpSet(void)
{
	if (g_u8SelfCheckFlag)
	{return ;}

	if (g_CarRfgrt.u8BleBindTime)
	{return ;}

	if (g_CarRfgrt.u8AppLock == C_APP_LOCK_ENABLE)
	{return ;}

	F_SetStaticDispTime();

	if ((g_sModeSet.u8Object) && (g_sModeSet.u8Object != C_MS_STATUS_CTRL_RBOX))
	{
		F_TemperSet(C_SET_INCREASE);
	}

	if (g_CarRfgrt.u8OnOff == C_Power_ON)
	{
		if (g_sModeSet.u8Status == C_MS_STATUS_IDLE)
		{
//			if (g_CarRfgrt.u8BoxEn == C_BOX_ONLY_RIGHT_EN)
//			{
//				g_sModeSet.u8Object = C_MS_OBJECT_RBOX;
//				g_sModeSet.u8Status = C_MS_STATUS_CTRL_RBOX;			
//			}
//			else
//			if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_RIGHT_EN)
			{
				g_sModeSet.u8Object = C_MS_OBJECT_LBOX;
				g_sModeSet.u8Status = C_MS_STATUS_CTRL_LBOX;
			}
		}
		g_sModeSet.u8Time   = C_MS_TIME_3S;
	}
	else
	{
		if (g_sModeSet.u8Object == C_MS_OBJECT_NONE)
		{
			F_AdvancedSetup(C_ADVSET_UP);       //菜单选择
		}
		else
		{
			F_AdvancedSetup(C_ADVSET_RELOAD);   //重载设置时间
		}

		F_SensorDirectionSet();
	}	
}

/*****************************************************
*函数名称：void F_UpSet_R(void)
*函数功能：上调事件
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_UpSet_R(void)
{
	_BUZZER_SHORT_BEEP;

	if (g_u8SelfCheckFlag)
	{
		F_DispToggle();
		return ;
	}

	if (g_CarRfgrt.u8BleBindTime)
	{return ;}

	if (g_CarRfgrt.u8BoxEn == C_BOX_ONLY_LEFT_EN)
	{return ;}

	if (g_CarRfgrt.u8AppLock == C_APP_LOCK_ENABLE)
	{return ;}

	F_SetStaticDispTime();

	if ((g_sModeSet.u8Object) && (g_sModeSet.u8Object != C_MS_STATUS_CTRL_LBOX))
	{
		F_TemperSet(C_SET_INCREASE);
		F_TemperUnit_Toggle();
	}

	if (g_CarRfgrt.u8OnOff == C_Power_ON)
	{
		if ((g_sModeSet.u8Status == C_MS_STATUS_IDLE) || (g_sModeSet.u8Object == C_MS_STATUS_CTRL_LBOX))
		{
			g_sModeSet.u8Object = C_MS_OBJECT_RBOX;
			g_sModeSet.u8Status = C_MS_STATUS_CTRL_RBOX;			
		}
		g_sModeSet.u8Time   = C_MS_TIME_3S;
	}

	else
	{
		if (g_sModeSet.u8Object == C_MS_OBJECT_NONE)
		{
			F_AdvancedSetup(C_ADVSET_UP);       //菜单选择
		}
		else
		{
			F_AdvancedSetup(C_ADVSET_RELOAD);   //重载设置时间
		}
	}	
}

/*****************************************************
*函数名称：void F_LongUpSet_R(void)
*函数功能：长加事件
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_LongUpSet_R(void)
{
	if (g_u8SelfCheckFlag)
	{return ;}

	if (g_CarRfgrt.u8BleBindTime)
	{return ;}

	if (g_CarRfgrt.u8BoxEn == C_BOX_ONLY_LEFT_EN)
	{return ;}

	if (g_CarRfgrt.u8AppLock == C_APP_LOCK_ENABLE)
	{return ;}

	F_SetStaticDispTime();

	if ((g_sModeSet.u8Object) && (g_sModeSet.u8Object != C_MS_STATUS_CTRL_LBOX))
	{
		F_TemperSet(C_SET_INCREASE);
	}

	if (g_CarRfgrt.u8OnOff == C_Power_ON)
	{
		if (g_sModeSet.u8Status == C_MS_STATUS_IDLE)
		{
			g_sModeSet.u8Object = C_MS_OBJECT_RBOX;
			g_sModeSet.u8Status = C_MS_STATUS_CTRL_RBOX;
		}
		g_sModeSet.u8Time   = C_MS_TIME_3S;
	}
	else
	{
		if (g_sModeSet.u8Object == C_MS_OBJECT_NONE)
		{
			F_AdvancedSetup(C_ADVSET_UP);       //菜单选择
		}
		else
		{
			F_AdvancedSetup(C_ADVSET_RELOAD);   //重载设置时间
		}
	}	
}

void F_SetBeep(void)
{
	_BUZZER_SHORT_BEEP;	
}

void F_KeyEvent_Unlock(void)
{
	if (g_CarRfgrt.u8AppLock == C_APP_LOCK_ENABLE)
	{
		_BUZZER_SHORT_BEEP;
		g_CarRfgrt.u8AppLock = C_APP_LOCK_DISABLE;
	}
}

/*****************************************************
*函数名称：void F_LevelSelect(void)
*函数功能：档位选择事件
*输入参数：无
*输出参数：无
*说明：短按SET键触发
*****************************************************/
void F_LevelSelect(void)
{

//	if (g_u8SelfCheckFlag)
//	{
//		F_ManualCheck();
//		return ;
//	}

//	if (g_CarRfgrt.u8BleBindTime)
//	{
//		g_CarRfgrt.u8BleBindTime = 0;
//		g_CarRfgrt.u8BleStatus = C_BLE_BIND;
//		F_AddActiveSendFun(F_UpLoad_Bind,++g_u8AppSn);
//		return ;	
//	}

	if (g_CarRfgrt.u8AppLock == C_APP_LOCK_ENABLE)
	{return ;}

	if (g_CarRfgrt.u8OnOff == C_Power_ON)
	{
//		if (g_sModeSet.u8Status == C_MS_STATUS_SPEED)
//		{
//			F_CharToggle(&g_CarRfgrt.u8SpeedLv,C_Speed_MAX,C_Speed_ECO);					
//		}
//		else if (g_sModeSet.u8Status == C_MS_STATUS_PROTECT)
//		{
//			if (g_CarRfgrt.u8BatProtectLv == C_Protect_High)
//			{g_CarRfgrt.u8BatProtectLv = C_Protect_Low;}
//			else if (g_CarRfgrt.u8BatProtectLv == C_Protect_Low)
//			{g_CarRfgrt.u8BatProtectLv = C_Protect_Mid;}
//			else
//			{g_CarRfgrt.u8BatProtectLv = C_Protect_High;}		
//		}

//	#if Def_SetKeyToggleTempSetEn

//		else if (g_sModeSet.u8Status == C_MS_STATUS_CTRL_LBOX)
//		{
//			if (g_CarRfgrt.u8BoxEn == C_BOX_ALL_EN)
//			{
//				g_sModeSet.u8Status = C_MS_STATUS_CTRL_RBOX;
//				g_sModeSet.u8Object = C_MS_OBJECT_RBOX;
//			}	
//		}

//		else if (g_sModeSet.u8Status == C_MS_STATUS_CTRL_RBOX)
//		{
//			if (g_CarRfgrt.u8BoxEn == C_BOX_ALL_EN)
//			{
//				g_sModeSet.u8Status = C_MS_STATUS_CTRL_LBOX;
//				g_sModeSet.u8Object = C_MS_OBJECT_LBOX;
//			}	
//		}

//	#endif

//		else
		{
			_BUZZER_SHORT_BEEP;
			if (g_sModeSet.u8Status == C_MS_STATUS_IDLE)
			{
				g_sModeSet.u8Status = C_MS_STATUS_SPEED;
			}
		}

		g_sModeSet.u8Time   = C_MS_TIME_3S;
	}
		
//	else
//	{

//		F_AdvancedSetup(C_ADVSET_SET);
//					
//	}	
}


void F_KeyEvent_SetProtect(void)
{
	if (g_CarRfgrt.u8AppLock == C_APP_LOCK_ENABLE)
	{return ;}

	if (g_CarRfgrt.u8OnOff == C_Power_ON)
	{
		_BUZZER_SHORT_BEEP;
		if (g_sModeSet.u8Status == C_MS_STATUS_IDLE)
		{
			g_sModeSet.u8Status = C_MS_STATUS_PROTECT;
		}
	}

	g_sModeSet.u8Time   = C_MS_TIME_3S;
}


/*****************************************************
*函数名称：void F_ModeSetChange(void)
*函数功能：模式切换事件
*输入参数：无
*输出参数：无
*说明：长按SET键3秒触发
*****************************************************/
void F_ModeSetChange(void)
{
	

	if (g_u8SelfCheckFlag)
	{return ;}

//	if (g_CarRfgrt.u8BleBindTime)
//	{return ;}
//
//	if (g_CarRfgrt.u8AppLock == C_APP_LOCK_ENABLE)
//	{return ;}

	if (g_CarRfgrt.u8OnOff == C_Power_ON)
	{
//		g_sModeSet.u8Status = C_MS_STATUS_PROTECT;
//		g_sModeSet.u8Object = C_MS_OBJECT_NONE;
//		g_sModeSet.u8Time   = C_MS_TIME_3S;	
	}
	else
	{
		_BUZZER_SHORT_BEEP;
		
		if (g_sModeSet.u8Status == C_MS_STATUS_IDLE)
		{
			g_sModeSet.u8Status = C_MS_STATUS_E1;	
			g_sModeSet.u8Time   = C_MS_TIME_5S;
			
		#if Def_AdvancedSetDisp
			g_sModeSet.u8Object = C_MS_OBJECT_E1;
		#else
			g_sModeSet.u8Object = C_MS_OBJECT_NONE;
		#endif		
		}
		else if (g_sModeSet.u8Status == C_MS_STATUS_E1)
		{
			F_FactoryDataReset(0,0);
			g_sModeSet.u8Status = C_MS_SYS_RESET;
			g_sModeSet.u8Time   = C_MS_TIME_5S;
		}			
	}	
}


/*****************************************************
*函数名称：void F_SystemReset(void)
*函数功能：恢复出厂设置和关左箱
*输入参数：无
*输出参数：无
*说明：	   E1状态下长按+和-键
*****************************************************/
void F_SystemReset(void)
{
				
	if (g_u8SelfCheckFlag)
	{return ;}

	if (g_CarRfgrt.u8OnOff == C_Power_ON)
	{
	#if Def_CloseLBox_En
		_BUZZER_SHORT_BEEP;

		if (g_CarRfgrt.u8CloseFlag_L)
		{g_CarRfgrt.u8CloseFlag_L = 0;}
		else
		{g_CarRfgrt.u8CloseFlag_L = 1;}
	#endif
	}
	else
	{
		if (g_sModeSet.u8Status == C_MS_STATUS_E1)
		{
			_BUZZER_SHORT_BEEP;
			F_FactoryDataReset(0,0);
			g_sModeSet.u8Status = C_MS_SYS_RESET;
			g_sModeSet.u8Time   = C_MS_TIME_5S;
		}
	}	
}


/*****************************************************
*函数名称：void F_CloseLbox(void)
*函数功能：关闭左箱设置
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_CloseLbox(void)
{
	_BUZZER_SHORT_BEEP;

	if (g_u8SelfCheckFlag)
	{return ;}

	if (g_CarRfgrt.u8AppLock == C_APP_LOCK_ENABLE)
	{return ;}

	if (g_CarRfgrt.u8OnOff == C_Power_ON)
	{
		if (g_CarRfgrt.u8BoxEn == C_BOX_ALL_EN)
		{
			g_CarRfgrt.u8BoxEn = C_BOX_ONLY_RIGHT_EN;		
		}
		else if (g_CarRfgrt.u8BoxEn == C_BOX_ONLY_LEFT_EN)
		{
			g_CarRfgrt.u8BoxEn = C_BOX_ONLY_RIGHT_EN;		
		}
		else
		{
			g_CarRfgrt.u8BoxEn = C_BOX_ALL_EN;
		}
	}	
}


/*****************************************************
*函数名称：void F_CloseRbox(void)
*函数功能：关闭右箱设置
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_CloseRbox(void)
{
	_BUZZER_SHORT_BEEP;

	if (g_u8SelfCheckFlag)
	{return ;}

	if (g_CarRfgrt.u8AppLock == C_APP_LOCK_ENABLE)
	{return ;}

	if (g_CarRfgrt.u8OnOff == C_Power_ON)
	{
		if (g_CarRfgrt.u8BoxEn == C_BOX_ALL_EN)
		{
			g_CarRfgrt.u8BoxEn = C_BOX_ONLY_LEFT_EN;		
		}
		else if (g_CarRfgrt.u8BoxEn == C_BOX_ONLY_RIGHT_EN)
		{
			g_CarRfgrt.u8BoxEn = C_BOX_ONLY_LEFT_EN;		
		}
		else
		{
			g_CarRfgrt.u8BoxEn = C_BOX_ALL_EN;
		}
	}	
}


/*****************************************************
*函数名称：void F_IntoSensorChannelSet(void)
*函数功能：进入电磁阀和NTC通道切换
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_IntoSensorChannelSet(void)
{
   	if (g_CarRfgrt.u8OnOff == C_Power_OFF)
	{
		_BUZZER_SHORT_BEEP;
		g_sModeSet.u8Status = C_MS_STATUS_ValveCh;
		g_sModeSet.u8Time   = C_MS_TIME_5S;
	}
}


/*****************************************************
*函数名称：void F_SensorDirectionSet(void)
*函数功能：电磁阀和NTC方向切换
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_SensorDirectionSet(void)
{

	if (g_u8UpKeyRepeat < 5)
	{
		g_u8UpKeyRepeat++;
	}

   	else if (g_u8UpKeyRepeat == 5)
	{
		g_u8UpKeyRepeat++;

		if (g_sModeSet.u8Status == C_MS_STATUS_ValveCh)
		{
			_BUZZER_SHORT_BEEP;
			F_CharToggle(&g_CarRfgrt.u8ValveToward,C_Toward_Forward,C_Toward_Backward);
			g_sModeSet.u8Time   = C_MS_TIME_5S;
		}
		else if (g_sModeSet.u8Status == C_MS_STATUS_TpCh)
		{
			_BUZZER_SHORT_BEEP;
			F_CharToggle(&g_CarRfgrt.u8NtcToward,C_Toward_Forward,C_Toward_Backward);
			g_sModeSet.u8Time   = C_MS_TIME_5S;
		}
	}
}



void F_IntoVersion(void)
{
	if (g_u8RunTime100mS < 100)
	{
		_BUZZER_SHORT_BEEP;
		
		if (g_CarRfgrt.u8OnOff != C_Power_ON)
		{
			g_u8VersionDispTime = 20;
		}
		else
		{
			F_IntoSelfCheck();
		}
	}
}
