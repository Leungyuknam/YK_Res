/*********************************************************
文 件 名 : Display.C
文件说明 : 显示处理文件
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-6
版 本 号 : V1.0
==========================================================*/
//头文件
#include "COMMON\AllInclude.h"

u8 xdata g_u8VersionDispTime = 0;
u8 xdata g_tu8Tm1650Buff[3] = {0};
static u8 xdata sg_u8StaticDispTime  =   0;
bit g_bUnit = 1;

u8 xdata g_u8DispLv = 0;
Temp_TypeDef xdata g_sInput = {-40,0,0};
Temp_TypeDef xdata g_sOutput = {-40,0,0};


/*****************************************************
*函数名称：void F_SetStaticDispTime(void)
*函数功能：静显处理
*输入参数：无
*输出参数：无
*调用周期：无
*说明：
*****************************************************/
void F_SetStaticDispTime(void)
{
	sg_u8StaticDispTime  =   10;	
}


/*****************************************************
*函数名称：void F_SetTime_Calc(void)
*函数功能：设置时间处理
*输入参数：无
*输出参数：无
*调用周期：100mS
*说明：
*****************************************************/
static void F_SetTime_Calc(void)	
{
	if (g_sModeSet.u8Time > 0)		//控制那个显示状态的时间的
	{
		g_sModeSet.u8Time--;
	}
	else
	{
		g_sModeSet.u8Status = C_MS_STATUS_IDLE;
		g_sModeSet.u8Object = C_MS_OBJECT_NONE;	
	}

	if (sg_u8StaticDispTime > 0)
	{
		sg_u8StaticDispTime--;
	}
}



void F_AllDisplay_1650(void)
{
	g_tu8Tm1650Buff[0] = 0xFF;
	g_tu8Tm1650Buff[1] = 0xFF;
	g_tu8Tm1650Buff[2] = 0xFF;	
}

static void F_AllClear_1650(void)
{
	g_tu8Tm1650Buff[0] = 0x00;
	g_tu8Tm1650Buff[1] = 0x00;
	g_tu8Tm1650Buff[2] = 0x00;	
}

static void F_Tm1650_Drive(void)
{
	u8 l_tu8DispBuff[4] = {0,0,0,0};

	if (_BTS(g_tu8Tm1650Buff[0],0))
	{_BS(l_tu8DispBuff[2],7);}

	if (_BTS(g_tu8Tm1650Buff[0],1))
	{_BS(l_tu8DispBuff[3],1);}

	if (_BTS(g_tu8Tm1650Buff[0],2))
	{_BS(l_tu8DispBuff[3],2);}

	if (_BTS(g_tu8Tm1650Buff[0],3))
	{_BS(l_tu8DispBuff[3],6);}

	if (_BTS(g_tu8Tm1650Buff[0],6))
	{_BS(l_tu8DispBuff[3],3);}



	if (_BTS(g_tu8Tm1650Buff[1],0))
	{_BS(l_tu8DispBuff[0],1);}

	if (_BTS(g_tu8Tm1650Buff[1],1))
	{_BS(l_tu8DispBuff[0],2);}

	if (_BTS(g_tu8Tm1650Buff[1],2))
	{_BS(l_tu8DispBuff[0],3);}

	if (_BTS(g_tu8Tm1650Buff[1],3))
	{_BS(l_tu8DispBuff[0],6);}

	if (_BTS(g_tu8Tm1650Buff[1],4))
	{_BS(l_tu8DispBuff[0],7);}

	if (_BTS(g_tu8Tm1650Buff[1],5))
	{_BS(l_tu8DispBuff[2],1);}

	if (_BTS(g_tu8Tm1650Buff[1],6))
	{_BS(l_tu8DispBuff[2],2);}



	if (_BTS(g_tu8Tm1650Buff[2],0))
	{_BS(l_tu8DispBuff[1],1);}

	if (_BTS(g_tu8Tm1650Buff[2],1))
	{_BS(l_tu8DispBuff[1],2);}

	if (_BTS(g_tu8Tm1650Buff[2],2))
	{_BS(l_tu8DispBuff[1],3);}

	if (_BTS(g_tu8Tm1650Buff[2],3))
	{_BS(l_tu8DispBuff[1],6);}

	if (_BTS(g_tu8Tm1650Buff[2],4))
	{_BS(l_tu8DispBuff[1],7);}

	if (_BTS(g_tu8Tm1650Buff[2],5))
	{_BS(l_tu8DispBuff[2],3);}

	if (_BTS(g_tu8Tm1650Buff[2],6))
	{_BS(l_tu8DispBuff[2],6);}

//	F_1650Driver_1(&l_tu8DispBuff,3);
}

static void F_NUMBER_Discretize_1650(unsigned int l_u16InputValue)
{       
	if (l_u16InputValue > 999)
	{l_u16InputValue = 999;}

    if (l_u16InputValue >= 100)
    {g_tu8Tm1650Buff[C_SLAVE1_FIRST_NUM] |= C_NUMBER_TABLE[((l_u16InputValue / 100))];}
    else
    {g_tu8Tm1650Buff[C_SLAVE1_FIRST_NUM] |= C_LABLE_BLANK;}
    
    if (l_u16InputValue >= 10)
    {
        if (l_u16InputValue >= 100)
        {g_tu8Tm1650Buff[C_SLAVE1_SECOND_NUM] |= C_NUMBER_TABLE[((l_u16InputValue % 100) / 10)];}
        else
        {g_tu8Tm1650Buff[C_SLAVE1_SECOND_NUM] |= C_NUMBER_TABLE[(l_u16InputValue / 10)];}
    }
    else
    {g_tu8Tm1650Buff[C_SLAVE1_SECOND_NUM] |= C_LABLE_BLANK;}
    
    if (l_u16InputValue >= 10)
    {g_tu8Tm1650Buff[C_SLAVE1_THIRD_NUM] |= C_NUMBER_TABLE[(l_u16InputValue % 10)];}
    else
    {g_tu8Tm1650Buff[C_SLAVE1_THIRD_NUM] |= C_NUMBER_TABLE[l_u16InputValue];}      
}

/*****************************************************
*函数名称: void F_BlueTooth_Display(void)
*函数功能: 蓝牙显示缓存处理
*入口参数：void
*出口参数：void 
*****************************************************/
static void F_BlueTooth_Display(void)
{

#if Def_TestMode

	if (g_CarRfgrt.u8ValveStatus != C_VALVE_TURN_NONE)
	{
		if (g_CarRfgrt.u8SpeedDrv == C_Speed_ECO)
		{
			if (g_bFlash1HzFlag)
			{_BLUE_DISP;}
		}
		else
		{_BLUE_DISP;}
	}

#else

	if (g_CarRfgrt.u8BleStatus != C_BLE_MISS)
	{
		if (g_CarRfgrt.u8OnOff == C_Power_ON)
		{
			_BLUE_DISP;
		}	
	}

#endif

}

/*****************************************************
*函数名称: void F_TempUnit_Display(void)
*函数功能: 温度单位显示缓存处理
*入口参数：void
*出口参数：void 
*****************************************************/
static void F_TempUnit_Display(void)
{
	bit l_bUnit_L = 0;

#if (Def_Machine_Type != C_Machine_Type_SingleBox)
	bit l_bUnit_R = 0;
#endif

//	if (g_CarRfgrt.u8OnOff == C_Power_ON)
//	{
//
//	#if (Def_Machine_Type == C_Machine_Type_SingleBox)
//
//		if ((g_CarRfgrt.u8CompError == C_ERRO_NULL) && (TEMPER_LBox.AD_Staut == c_AD_NORMAL))	
//		{l_bUnit_L = 1;}
//
//		if (g_sModeSet.u8Status == C_MS_STATUS_CTRL_LBOX)
//		{l_bUnit_L = 1;}
//
//	#else
//
//		if (g_CarRfgrt.u8BoxEn == C_BOX_ONLY_LEFT_EN)
//		{
//			if ((g_CarRfgrt.u8CompError == C_ERRO_NULL) && (TEMPER_LBox.AD_Staut == c_AD_NORMAL))	
//			{l_bUnit_L = 1;}
//	
//			if (g_sModeSet.u8Status == C_MS_STATUS_CTRL_LBOX)
//			{l_bUnit_L = 1;}
//		}
//		else if (g_CarRfgrt.u8BoxEn == C_BOX_ONLY_RIGHT_EN)
//		{
//			if ((g_CarRfgrt.u8CompError == C_ERRO_NULL) && (TEMPER_RBox.AD_Staut == c_AD_NORMAL))	
//			{l_bUnit_R = 1;}
//	
//			if (g_sModeSet.u8Status == C_MS_STATUS_CTRL_RBOX)
//			{l_bUnit_R = 1;}
//		}		
//		else //if (g_CarRfgrt.u8BoxEn == C_BOX_ALL_EN)
//		{
//			if ((g_CarRfgrt.u8CompError == C_ERRO_NULL) && (TEMPER_LBox.AD_Staut == c_AD_NORMAL))	
//			{l_bUnit_L = 1;}
//
//			if (g_sModeSet.u8Status == C_MS_STATUS_CTRL_LBOX)
//			{l_bUnit_L = 1;}
//
//			if (TEMPER_RBox.AD_Staut == c_AD_NORMAL)	
//			{l_bUnit_R = 1;}
//	
//			if (g_sModeSet.u8Status == C_MS_STATUS_CTRL_RBOX)
//			{l_bUnit_R = 1;}
//
//			if ((g_sModeSet.u8Status == C_MS_STATUS_CTRL_LBOX) || (g_sModeSet.u8Status == C_MS_STATUS_CTRL_RBOX))
//			{
//				if (g_CarRfgrt.s8SetTemp_L == g_CarRfgrt.s8SetTemp_R)
//				{
//					l_bUnit_R = 1;
//					l_bUnit_L = 1;
//				}
//			}
//		}
//
//	#endif
//			
//	}
//
//	//关机
//	else
	{
//		if ((g_sModeSet.u8Object >= C_MS_OBJECT_E1) &&
//		    (g_sModeSet.u8Object <= C_MS_OBJECT_END))
		{
			if (g_sModeSet.u8Object == C_MS_OBJECT_E5)
			{
#if ((Def_Machine_Type == C_Machine_Type_SingleBox) || (Def_AdvancedSetDisp == 0))
				l_bUnit_L = 1;	
#else
				l_bUnit_R = 1;
#endif
			}	
		}
	}

#if Def_TestMode

	if (g_bFlash1HzFlag)
	{
		if (g_CarRfgrt.u8ValveStatus == C_VALVE_TURN_LEFT)
		{l_bUnit_L = ~l_bUnit_L;l_bUnit_R = ~l_bUnit_R;}
		else if (g_CarRfgrt.u8ValveStatus == C_VALVE_TURN_RIGHT)
		{l_bUnit_R = ~l_bUnit_R;}
	}
		
#endif

	if (l_bUnit_L)
	{
		if (g_CarRfgrt.u8TempUnit == C_Unit_Fahrenheit)
		{_L_UNIT_F_DISP;}
		else
		{_L_UNIT_C_DISP;}	
	}

#if (Def_Machine_Type != C_Machine_Type_SingleBox)
	if (l_bUnit_R)
	{
		if (g_CarRfgrt.u8TempUnit == C_Unit_Fahrenheit)
		{_R_UNIT_F_DISP;}
		else
		{_R_UNIT_C_DISP;}	
	}
#endif

}

/*****************************************************
*函数名称: void F_BatProtectLv_Display(void)
*函数功能: 保护等级显示缓存处理
*入口参数：void
*出口参数：void 
*****************************************************/
static void F_BatProtectLv_Display(void)
{

	if (g_sModeSet.u8Status == C_MS_STATUS_PROTECT)
	{
		if (g_bFlash1HzFlag)	
		{
			if (g_CarRfgrt.u8BatProtectLv == C_Protect_Low)
			{_ICON_L_DISP;}
			else //if (g_CarRfgrt.u8BatProtectLv == C_Protect_Mid)
			{_ICON_M_DISP;}
//			else
//			{_ICON_H_DISP;}
		}
	}

}

/*****************************************************
*函数名称: void F_CompSpeed_Display(void)
*函数功能: 转速显示缓存处理
*入口参数：void
*出口参数：void 
*****************************************************/
static void F_CompSpeed_Display(void)
{
	bit	s_bDisFlag = 0;	

	if (g_sModeSet.u8Status == C_MS_STATUS_SPEED)
	{
		if (g_bFlash1HzFlag)
		{s_bDisFlag = 1;}	
	}
	else
	{s_bDisFlag = 1;}

	if (s_bDisFlag)
	{
		if (g_CarRfgrt.u8SpeedLv == C_Speed_ECO)
		{_ICON_ECO_DISP;}
		else
		{_ICON_MAX_DISP;}
	}
}


/*****************************************************
*函数名称: void F_PowerLv_Display(void)
*函数功能: 电量等级显示缓存处理
*入口参数：void
*出口参数：void 
*****************************************************/
static void F_PowerLv_Display(void)
{
#if (Def_TempUnit == 0)

	u8  l_u8DispBuff;

#if Def_BatCommunicateEn
	static u8 xdata s_u8TimeCnt = 0;
	static u8 xdata s_u8State = 1;
	       
	if ((g_CarRfgrt.u8PowerSource == C_Power_Battery) && (g_CarRfgrt.u8Charge == C_Enable))
	{
		if (++s_u8TimeCnt > 5)
		{
			s_u8TimeCnt = 0;
			if (++s_u8State > 5)
			{
				s_u8State = 0;
			}
		}
		l_u8DispBuff = s_u8State;
	}
	else
	{
		s_u8State = 0;
		if (g_CarRfgrt.u8OnOff == C_Power_OFF)
		{return ;}
		l_u8DispBuff = g_sPower.u8Level;
	}

#else

	l_u8DispBuff = g_sPower.u8Level;

#endif

//	if (l_u8DispBuff == C_POWER_LEVEL1_LOWEST)
//	{
//		if (g_bFlash1HzFlag)
//		{_POWER_SHAPE_DISP;} 			
//	}
//	else
	{
		_POWER_SHAPE_DISP;	
	}

	if (l_u8DispBuff == C_POWER_LEVEL2_LOWER)
	{
		_POWER_LV1_DISP;
	}
	else if (l_u8DispBuff == C_POWER_LEVEL3_LOW)
	{
		_POWER_LV1_DISP;
		_POWER_LV2_DISP;
	}
	else if (l_u8DispBuff == C_POWER_LEVEL4_HIGH)
	{
		_POWER_LV1_DISP;
		_POWER_LV2_DISP;
		_POWER_LV3_DISP;
	}
	else if (l_u8DispBuff == C_POWER_LEVEL5_HIGHER)
	{
		_POWER_LV1_DISP;
		_POWER_LV2_DISP;
		_POWER_LV3_DISP;
		_POWER_LV4_DISP;	
	}
	else if (l_u8DispBuff == C_POWER_LEVEL6_HIGHEST)
	{
		_POWER_LV1_DISP;
		_POWER_LV2_DISP;
		_POWER_LV3_DISP;
		_POWER_LV4_DISP;
		_POWER_LV5_DISP;			
	}  
#endif
}


/*****************************************************
*函数名称: void F_PowerValue_Display(void)
*函数功能: 电量值显示缓存处理
*入口参数：void
*出口参数：void 
*****************************************************/
void F_PowerValue_Display(void)
{
	F_NUMBER_Discretize_Slave1(g_sPower.u16Value);
	_POWER_DOT_DISP;
    _POWER_V_DISP;

	if (g_sPower.u16Value < 10)
	{_POWER_MAIN_0_DISP;}	
}

/*****************************************************
*函数名称: void F_TempValue_Display(s8 l_s8InputValue,u8 l_u8RightOrLeft)
*函数功能: 温度显示缓存处理
*入口参数：void
*出口参数：void 
*****************************************************/
void F_TempValue_Display(s8 l_s8InputValue,u8 l_u8RightOrLeft)
{

	if (l_s8InputValue < 0)
	{
		l_s8InputValue = -l_s8InputValue;

//		if (l_u8RightOrLeft == C_SET_RIGHT)
//		{
//			if (l_s8InputValue >= 10)
//			{_R_DIG1_G_DISP;}
//			else
//			{_R_DIG2_G_DISP;}
//		}
//		else
		{
			if (l_s8InputValue >= 10)
			{_L_DIG1_G_DISP;}
			else
			{_L_DIG2_G_DISP;}
		}
	}

	if (l_u8RightOrLeft == C_SET_RIGHT)
	{
		F_NUMBER_Discretize(l_s8InputValue);	
	}
	else 
	{
		F_NUMBER_Discretize(l_s8InputValue);
	}
}




/*****************************************************
*函数名称: void F_SystemOff_Display(u8 l_u8RightOrLeft)
*函数功能: 关闭温区显示缓存处理
*入口参数：void
*出口参数：void 
*****************************************************/
static void F_SystemOff_Display(u8 l_u8RightOrLeft)
{
	if (l_u8RightOrLeft == C_SET_RIGHT)
	{
		_R_DIG1_G_DISP;
		_R_DIG2_G_DISP;
		_R_DIG3_G_DISP;		
	}
	else
	{
		_L_DIG1_G_DISP;
		_L_DIG2_G_DISP;
		_L_DIG3_G_DISP;	
	}
}

/*****************************************************
*函数名称: void F_SystemError_Display(void)
*函数功能: 报错显示缓存处理
*入口参数：void
*出口参数：void 
*****************************************************/
static void F_SystemError_Display(void)
{
	switch (g_CarRfgrt.u8CompError)
	{
		case C_ERRO_F1:
		{
//			if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_RIGHT_EN)
			{_L_F1_DISP;}
//			else
//			{_R_F1_DISP;}
			break;	
		}
		case C_ERRO_F2:
		{
//			if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_RIGHT_EN)
			{_L_F2_DISP;}
//			else
//			{_R_F2_DISP;}
			break;	
		}
		case C_ERRO_F3:
		{
//			if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_RIGHT_EN)
			{_L_F3_DISP;}
//			else
//			{_R_F3_DISP;}
			break;	
		}
		case C_ERRO_F4:
		{
//			if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_RIGHT_EN)
			{_L_F4_DISP;}
//			else
//			{_R_F4_DISP;}
			break;	
		}
		case C_ERRO_F5:
		{
//			if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_RIGHT_EN)
			{_L_F5_DISP;}
//			else
//			{_R_F5_DISP;}
			break;	
		}
		case C_ERRO_F6:
		{
//			if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_RIGHT_EN)
			{_L_F6_DISP;}
//			else
//			{_R_F6_DISP;}
			break;	
		}
		case C_ERRO_F7:
		{
//			if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_RIGHT_EN)
			{_L_FF_DISP;}
//			else
//			{_R_F6_DISP;}
			break;	
		}
		default: 
		{
//			if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_RIGHT_EN)
			{
				if (TEMPER_LBox.AD_Staut == c_AD_LESS_THAN)
				{_L_F7_DISP;}
				else if (TEMPER_LBox.AD_Staut == c_AD_OVERSIZE)
				{_L_F8_DISP;}
			}

//		#if Def_NtcNumbers
//			if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_LEFT_EN)
//			{
//				if (TEMPER_RBox.AD_Staut == c_AD_LESS_THAN)
//				{_R_F7_DISP;}
//				else if (TEMPER_RBox.AD_Staut == c_AD_OVERSIZE)
//				{_R_F8_DISP;}
//			}
//		#endif

			break;
		}			 
	}
}


/*****************************************************
*函数名称: void F_Left_Window_Display(void)
*函数功能: 左窗口显示缓存处理
*入口参数：void
*出口参数：void 
*****************************************************/
static void F_Left_Window_Display(void)
{

	//显示设置温度
////	if ((g_sModeSet.u8Status == C_MS_STATUS_CTRL_LBOX) || 
////	   ((g_CarRfgrt.s8SetTemp_L == g_CarRfgrt.s8SetTemp_R)) && (g_sModeSet.u8Status == C_MS_STATUS_CTRL_RBOX))
//	if (g_sModeSet.u8Status == C_MS_STATUS_PROTECT)
//	{
//		if (g_bFlash1HzFlag)	
//		{
//			if (g_CarRfgrt.u8BatProtectLv == C_Protect_Low)
//			{_ICON_L_DISP;}
//			else //if (g_CarRfgrt.u8BatProtectLv == C_Protect_Mid)
//			{_ICON_M_DISP;}
////			else
////			{_ICON_H_DISP;}
//		}
//	}
//	else 
	if (g_sModeSet.u8Status == C_MS_STATUS_CTRL_LBOX)
	{
//		if (g_sModeSet.u8Status == C_MS_STATUS_CTRL_LBOX)
		{
			if ((g_bFlash1HzFlag) || (sg_u8StaticDispTime))
			{
				F_TempValue_Display(g_CarRfgrt.s8SetTemp_L,C_SET_LEFT);				//设定温度
			}		
		}
//		else
//		{
//			F_TempValue_Display(g_CarRfgrt.s8SetTemp_L,C_SET_LEFT);
//		}		
	}

	else if (g_sModeSet.u8Status == C_MS_STATUS_SPEED)
	{
		if ((g_bFlash1HzFlag) || (sg_u8StaticDispTime))
		{
			if (g_CarRfgrt.u8SpeedLv == C_Speed_MAX)
			{_ICON_MAX_DISP;}
			else
			{_ICON_ECO_DISP;}
		}	
	}
	
	else if (g_sModeSet.u8Status == C_MS_STATUS_PROTECT)
	{
		if ((g_bFlash1HzFlag) || (sg_u8StaticDispTime))
		{
			if (g_CarRfgrt.u8BatProtectLv == C_Protect_High)
			{_ICON_H_DISP;}
			else if (g_CarRfgrt.u8BatProtectLv == C_Protect_Mid)
			{_ICON_M_DISP;}			
			else
			{_ICON_L_DISP;}
		}	
	}
	
	//显示报错代码
//	else if ((g_CarRfgrt.u8CompError != C_ERRO_NULL) || 
//			((g_CarRfgrt.u8BoxEn != C_BOX_ONLY_RIGHT_EN) && (TEMPER_LBox.AD_Staut != c_AD_NORMAL)))

		//通讯异常
	else if (g_bCompCmnError)
	{
		_L_FF_DISP;
	}
	
	else if ((g_CarRfgrt.u8CompError != C_ERRO_NULL) || (TEMPER_LBox.AD_Staut != c_AD_NORMAL))
	{
		F_SystemError_Display();
	}

	//显示实时温度
    else //if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_RIGHT_EN)
	{
		F_TempValue_Display(g_CarRfgrt.s8DispTemp_L,C_SET_LEFT);			//显示温度
	}

	//显示中间三横
//	else
//	{
//		F_SystemOff_Display(C_SET_LEFT);	
//	}	
}

/*****************************************************
*函数名称: void F_Right_Window_Display(void)
*函数功能: 右窗口显示缓存处理
*入口参数：void
*出口参数：void 
*****************************************************/
static void F_Right_Window_Display(void)
{

	if (g_CarRfgrt.u8BoxEn == C_BOX_ONLY_LEFT_EN)
	{
	
	}

	//显示设置温度
	else if ((g_sModeSet.u8Status == C_MS_STATUS_CTRL_RBOX) || 
			((g_sModeSet.u8Status == C_MS_STATUS_CTRL_LBOX)) && (g_CarRfgrt.s8SetTemp_L == g_CarRfgrt.s8SetTemp_R))
	{
		if (g_sModeSet.u8Status == C_MS_STATUS_CTRL_RBOX)
		{
			if ((g_bFlash1HzFlag) || (sg_u8StaticDispTime))
			{
				F_TempValue_Display(g_CarRfgrt.s8SetTemp_R,C_SET_RIGHT);		
			}		
		}
		else
		{
			F_TempValue_Display(g_CarRfgrt.s8SetTemp_R,C_SET_RIGHT);
		}		
	}

	//显示报错代码
//	else if ((g_CarRfgrt.u8BoxEn == C_BOX_ONLY_RIGHT_EN) && 
//			((g_CarRfgrt.u8CompError != C_ERRO_NULL) || (TEMPER_RBox.AD_Staut != c_AD_NORMAL)))
//	{
//		F_SystemError_Display();
//	}
	else if (TEMPER_RBox.AD_Staut != c_AD_NORMAL)
	{
		if (TEMPER_RBox.AD_Staut == c_AD_LESS_THAN)
		{_R_F7_DISP;}
		else if (TEMPER_RBox.AD_Staut == c_AD_OVERSIZE)
		{_R_F8_DISP;}
	}

	//显示实时温度
    else if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_LEFT_EN)
	{
		F_TempValue_Display(g_CarRfgrt.s8RoomTemp_R,C_SET_RIGHT);
	}

	//显示中间三横
	else
	{
//		F_SystemOff_Display(C_SET_RIGHT);	
	}	
}


/*****************************************************
*函数名称: void F_AdvanceSetup_Display(void)
*函数功能: 参数设置显示缓存处理
*入口参数：void
*出口参数：void 
*****************************************************/
static void F_AdvanceSetup_Display(void)
{
	if ((g_sModeSet.u8Status >= C_MS_STATUS_E1) && (g_sModeSet.u8Status <= C_MS_STATUS_END))
	{

#if (Def_AdvancedSetDisp == 0)
		if (g_sModeSet.u8Object < C_MS_OBJECT_BIAS)
#endif

		{
			if (g_bFlash1HzFlag)
			{
				if (g_sModeSet.u8Status == C_MS_STATUS_E1)
				{_E1_DISP;}
				else if (g_sModeSet.u8Status == C_MS_STATUS_E2)
				{_E2_DISP;}
				else if (g_sModeSet.u8Status == C_MS_STATUS_E3)
				{_E3_DISP;}
				else if (g_sModeSet.u8Status == C_MS_STATUS_E4)
				{_E4_DISP;}
				else if (g_sModeSet.u8Status == C_MS_STATUS_E5)
				{_E5_DISP;}
				else if (g_sModeSet.u8Status == C_MS_STATUS_E6)
				{_E6_DISP;}
				else if (g_sModeSet.u8Status == C_MS_STATUS_E7)
				{_E7_DISP;}
				else if (g_sModeSet.u8Status == C_MS_STATUS_E8)
				{_E8_DISP;}
				else if (g_sModeSet.u8Status == C_MS_STATUS_E9)
				{_E9_DISP;}
				else if (g_sModeSet.u8Status == C_MS_STATUS_EA)
				{_EA_DISP;}
				else if (g_sModeSet.u8Status == C_MS_STATUS_Eb)
				{_Eb_DISP;}
				else if (g_sModeSet.u8Status == C_MS_STATUS_EC)
				{_EC_DISP;}
				else if (g_sModeSet.u8Status == C_MS_STATUS_Ed)
				{_Ed_DISP;}
				else if (g_sModeSet.u8Status == C_MS_STATUS_EE)
				{_EE_DISP;}
			}
		}

#if (Def_AdvancedSetDisp == 0)
		else
#endif

		{
			if ((g_bFlash1HzFlag) || (sg_u8StaticDispTime))
			{
				if (g_sModeSet.u8Object == C_MS_OBJECT_E1)
				{F_TempValue_Display(g_CarRfgrt.s8SetTemp_Bot,C_SET_RIGHT);}
				else if (g_sModeSet.u8Object == C_MS_OBJECT_E2)
				{F_TempValue_Display(g_CarRfgrt.s8SetTemp_Top,C_SET_RIGHT);}
				else if (g_sModeSet.u8Object == C_MS_OBJECT_E3)
				{F_TempValue_Display(g_CarRfgrt.s8StartBias_L,C_SET_RIGHT);}
				else if (g_sModeSet.u8Object == C_MS_OBJECT_E4)
				{F_TempValue_Display(g_CarRfgrt.u8ComProtectTime,C_SET_RIGHT);}
				else if (g_sModeSet.u8Object == C_MS_OBJECT_E6)
				{F_TempValue_Display(g_CarRfgrt.s8TempComp_N6_L,C_SET_RIGHT);}
				else if (g_sModeSet.u8Object == C_MS_OBJECT_E7)
				{F_TempValue_Display(g_CarRfgrt.s8TempComp_N12_L,C_SET_RIGHT);}
				else if (g_sModeSet.u8Object == C_MS_OBJECT_E8)
				{F_TempValue_Display(g_CarRfgrt.s8TempComp_N13_L,C_SET_RIGHT);}
				else if (g_sModeSet.u8Object == C_MS_OBJECT_E9)
				{F_TempValue_Display(g_CarRfgrt.s8StopBias_L,C_SET_RIGHT);}
				else if (g_sModeSet.u8Object == C_MS_OBJECT_EA)
				{F_TempValue_Display(g_CarRfgrt.u8DefrostOnTime,C_SET_RIGHT);}
				else if (g_sModeSet.u8Object == C_MS_OBJECT_Eb)
				{F_TempValue_Display(g_CarRfgrt.u8DefrostOffTime,C_SET_RIGHT);}
				else if (g_sModeSet.u8Object == C_MS_OBJECT_EC)
				{F_TempValue_Display(g_CarRfgrt.s8TempComp_N12_R,C_SET_RIGHT);}
				else if (g_sModeSet.u8Object == C_MS_OBJECT_Ed)
				{F_TempValue_Display(g_CarRfgrt.s8TempComp_N13_R,C_SET_RIGHT);}
				else if (g_sModeSet.u8Object == C_MS_OBJECT_EE)
				{F_TempValue_Display(g_CarRfgrt.s8StopBias_R,C_SET_RIGHT);}

				
			}

			F_TempUnit_Display();
		}
	}
    else if (g_sModeSet.u8Status == C_MS_SYS_RESET)
	{
		F_DISP_All();
//		F_AllDisplay_1650();
	}
//	else if (g_sModeSet.u8Status == C_MS_STATUS_ValveCh)
//	{											   
//		g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_U;
//		g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_L;
//		F_TempValue_Display(g_CarRfgrt.u8ValveToward,C_SET_RIGHT);
//	}
//	else if (g_sModeSet.u8Status == C_MS_STATUS_TpCh)
//	{
//		g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_T;
//		g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_P;
//		F_TempValue_Display(g_CarRfgrt.u8NtcToward,C_SET_RIGHT);
//	}
	else 
	{
//		F_PowerLv_Display();	
	}
}

static void F_PowerOn_Disp(void)
{
	g_u16DisPrimaryBuff_TABLE[0] = 0xFF;
	g_u16DisPrimaryBuff_TABLE[1] = 0xFF;
	g_u16DisPrimaryBuff_TABLE[2] = 0xFF;
}


void F_Disp_LowBrighness(void)
{
	static u16 xdata s_u16Filter = 0;
		
	if (g_u8DispKeyBuff == 0)
	{
		if (s_u16Filter < 450)
		{
			s_u16Filter++;
		}
		else
		{
			g_u8DispLv = 8;
		}
	}
	else
	{
		s_u16Filter = 0;
	}
	
}

/*****************************************************
*函数名称: void F_Display_Handle(void)
*函数功能: 显示处理
*入口参数：void
*出口参数：void
*调用周期: 100ms 
*****************************************************/
void F_Display_Handle(void)
{	
	F_SetTime_Calc();
//	F_Disp_LowBrighness();
	
	//清缓存
	F_DISP_Clear();

	//缓存输入
	if (g_u8RunTime100mS < 30)
	{
		F_PowerOn_Disp();					//前3秒全亮
	}
	else if (g_u8VersionDispTime)
	{
		g_u8VersionDispTime--;
		#message "当前软件版本V10"
		g_u16DisPrimaryBuff_TABLE[C_FIRST_NUM] |= C_LABLE_A;
		F_NUMBER_Discretize(C_SOFT_VER);
	}
	else if (g_u8SelfCheckFlag)
	{
		F_SELFCHECK_Display();	
	}
	else if (g_CarRfgrt.u8BleBindTime)
	{
		_APP_DISP;
		g_CarRfgrt.u8BleBindTime--;
		if (g_CarRfgrt.u8BleBindTime == 0)
		{
			g_CarRfgrt.u8BleStatus = C_BLE_IDLE;
			F_AddActiveSendFun(F_UpLoad_Bind,++g_u8AppSn);
		}
	}
	else if (g_CarRfgrt.u8OnOff == C_Power_ON)
	{

//	#if Def_BlueToothEn
//		F_BlueTooth_Display();
//	#endif

//		F_TempUnit_Display();
//		F_BatProtectLv_Display();
//		F_CompSpeed_Display();
//		F_PowerLv_Display();
//		F_PowerValue_Display();

		F_Left_Window_Display();

	}
	else
	{
		F_AdvanceSetup_Display();
	}

	//缓存输出
    F_DISPLAY_DataRedirect();
}
