/*********************************************************
文 件 名 : API.C
文件说明 : 应用接口处理
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/
//头文件
#include "COMMON\AllInclude.h"

static u16 xdata  g_u16LimitTime = 0;
static u8  xdata  g_u8SelfCheckState = 0;
	   u8  xdata  g_u8StepCnt = 0;
	   u8  xdata  g_u8SelfCheckFlag  = 0;
	   u8  xdata  g_u8ForbidSaveData  = 0;

static bit s_bDispToggle = 1;

/*****************************************************
*函数名称：void F_IntoSelfCheck(void)
*函数功能：进入自检事件
*输入参数：无
*输出参数：无
*说明：	   仅前8秒可进入自检
*****************************************************/
void F_IntoSelfCheck(void)
{	
	if (g_u8RunTime100mS < 80)
	{
		g_u8SelfCheckFlag = 1;
		g_u8ForbidSaveData = 1;
		_BUZZER_SHORT_BEEP;				
	}
}

/*****************************************************
*函数名称：void F_BackOutSelfCheck(void)
*函数功能：退出自检事件
*输入参数：无
*输出参数：无
*说明：	   
*****************************************************/
void F_BackOutSelfCheck(void)
{	
	g_u8SelfCheckFlag = 0;
//	_BUZZER_SHORT_BEEP;
}


/*****************************************************
*函数名称：void F_DispToggle(void)
*函数功能：显示开关反转
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_DispToggle(void)
{
	s_bDispToggle = ~s_bDispToggle;
}

/*****************************************************
*函数名称：void F_ManualCheck(void)
*函数功能：手动检测
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_ManualCheck(void)
{
	if (g_u8SelfCheckFlag)
	{
		if (++g_u8SelfCheckState > C_MAX_STATE)
		{
			g_u8SelfCheckState = 0;
		}
//		_BUZZER_SHORT_BEEP;	
	}
	g_u16LimitTime = 0;
}

/*****************************************************
*函数名称：void F_SELFCHECK_Display(void)
*函数功能：自检显示
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_SELFCHECK_Display(void)
{
	static u8 xdata s_u8Time100msCnt = 0;

	if (s_bDispToggle == 0)
	{
	
	}
	else if (g_u8SelfCheckState == 0)
	{
		if (++s_u8Time100msCnt > 3)
		{
			s_u8Time100msCnt = 0;
			if (++g_u8StepCnt > 11)
			{g_u8StepCnt = 0;}
		}

//		if (g_u8StepCnt > 0)  {_L_UNIT_F_DISP;}
//		if (g_u8StepCnt > 1)  {_L_UNIT_C_DISP;}
//		if (g_u8StepCnt > 2)  {_ICON_H_DISP;}
//		if (g_u8StepCnt > 3)  {_ICON_M_DISP;}
//		if (g_u8StepCnt > 4)  {_ICON_L_DISP;}
//		if (g_u8StepCnt > 5)  {_R_UNIT_F_DISP;}
//		if (g_u8StepCnt > 6)  {_R_UNIT_C_DISP;}
//		if (g_u8StepCnt > 7)  {g_tu8Tm1650Buff[C_SLAVE1_FIRST_NUM] |= 0x02;}
//		if (g_u8StepCnt > 8)  {g_tu8Tm1650Buff[C_SLAVE1_FIRST_NUM] |= 0x04;}
//		if (g_u8StepCnt > 9)  {g_tu8Tm1650Buff[C_SLAVE1_FIRST_NUM] |= 0x40;}
//		if (g_u8StepCnt > 5) {_ICON_ECO_DISP;}
//		if (g_u8StepCnt > 6) {_ICON_MAX_DISP;}
//		if (g_u8StepCnt > 7) {_POWER_SHAPE_DISP;}
//		if (g_u8StepCnt > 8) {_POWER_LV1_DISP;}
//		if (g_u8StepCnt > 9) {_POWER_LV2_DISP;}
//		if (g_u8StepCnt > 10) {_POWER_LV3_DISP;}
//		if (g_u8StepCnt > 11) {_POWER_LV4_DISP;}
//		if (g_u8StepCnt > 12) {_POWER_LV5_DISP;}
//		if (g_u8StepCnt > 13) {_BLUE_DISP;}

//		_POWER_DOT_DISP;
//		_POWER_V_DISP;

		if (g_u8StepCnt < 10)
		{
			g_u16Digital_COM1_Data |= C_NUMBER_TABLE[g_u8StepCnt];
			g_u16Digital_COM2_Data |= C_NUMBER_TABLE[g_u8StepCnt];
			g_u16Digital_COM3_Data |= C_NUMBER_TABLE[g_u8StepCnt];
		}
		else //if (g_u8StepCnt == 10)
		{
			g_u16DisPrimaryBuff_TABLE[C_FIRST_NUM] |= C_LABLE_A;
			F_NUMBER_Discretize(C_SOFT_VER);								
		}
	}

	else if (g_u8SelfCheckState == 1)
	{
		F_DISP_All();
		s_u8Time100msCnt = 0;
		g_u8StepCnt = 0;
	}
	else if (g_u8SelfCheckState == 2)
	{
		if (++s_u8Time100msCnt > 20)
		{
			s_u8Time100msCnt = 0;
			if (++g_u8StepCnt > 7)
			{g_u8StepCnt = 0;}
		}

//		if (g_u8StepCnt == 0)
//		{
//			_ICON_ECO_DISP;
//			g_u16DisPrimaryBuff_TABLE[C_SLAVE1_SECOND_NUM] |= C_NUMBER_TABLE[0];
//			g_u16DisPrimaryBuff_TABLE[C_SLAVE1_THIRD_NUM] |= C_NUMBER_TABLE[0];
//		    _POWER_DOT_DISP;
//			_POWER_V_DISP;								
//		}
//		else 
		if (g_u8StepCnt == 0)
		{
			_ICON_MAX_DISP;
//			g_u16DisPrimaryBuff_TABLE[C_SLAVE1_SECOND_NUM] |= C_NUMBER_TABLE[2];
//			g_u16DisPrimaryBuff_TABLE[C_SLAVE1_THIRD_NUM] |= C_NUMBER_TABLE[0];
//		    _POWER_DOT_DISP;
//			_POWER_V_DISP;								
		}
		else if (g_u8StepCnt == 1)
		{
			_ICON_ECO_DISP;
		}
		else if (g_u8StepCnt == 2)
		{
			_ICON_H_DISP;
		}
		else if (g_u8StepCnt == 3)
		{
			_ICON_M_DISP;
		}
		else if (g_u8StepCnt == 4)
		{
			_ICON_L_DISP;
		}		
//		else if (g_u8StepCnt == 2)
//		{
//			_ICON_H_DISP;
//			g_u16DisPrimaryBuff_TABLE[C_SLAVE1_SECOND_NUM] |= C_NUMBER_TABLE[4];
//			g_u16DisPrimaryBuff_TABLE[C_SLAVE1_THIRD_NUM] |= C_NUMBER_TABLE[2];
//		    _POWER_DOT_DISP;
//			_POWER_V_DISP;								
//		}
		else if (g_u8StepCnt == 5)
		{
			if (g_CarRfgrt.u8BleStatus != C_BLE_MISS)
			{
				g_u16DisPrimaryBuff_TABLE[C_FIRST_NUM] |= C_LABLE_A;
				g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_P;
				g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_P;
			}
			else
			{
				g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_n;
				g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM6;			
			}
//		    _POWER_DOT_DISP;
//			_POWER_V_DISP;								
		}
		else if (g_u8StepCnt == 6)
		{
//			_ICON_L_DISP;
			F_TempValue_Display(g_CarRfgrt.s8RoomTemp_L,C_SET_LEFT);
//		    _POWER_DOT_DISP;
//			_POWER_V_DISP;								
		}
		else if (g_u8StepCnt == 7)
		{
			g_u16DisPrimaryBuff_TABLE[C_FIRST_NUM] |= C_NUMBER_TABLE[g_sPower.u16Value/100];
			g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_NUMBER_TABLE[g_sPower.u16Value%100/10];
			g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_NUMBER_TABLE[g_sPower.u16Value%10];							
		}	
//		else if (g_u8StepCnt == 5)
//		{
//			F_TempValue_Display(g_CarRfgrt.s8RoomTemp_L,C_SET_LEFT);
//			_L_UNIT_C_DISP;		
//		}
//		else if (g_u8StepCnt == 6)
//		{
//			F_TempValue_Display(g_CarRfgrt.s8RoomTemp_R,C_SET_RIGHT);
//			_R_UNIT_C_DISP;		
//		}
//		else if (g_u8StepCnt == 6)
//		{
//		    F_PowerValue_Display();	
//			_POWER_SHAPE_DISP;
//			_POWER_LV1_DISP;
//		    _POWER_LV2_DISP;
//			_POWER_LV3_DISP;
//			_POWER_LV4_DISP;
//			_POWER_LV5_DISP;
//		}
//		else if (g_u8StepCnt == 7)
//		{
//			_BLUE_DISP;
//			if (g_CarRfgrt.u8BleStatus != C_BLE_MISS)
//			{
//				g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_P;
//				g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_A;
//			}
//			else
//			{
//				g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_n;
//				g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM6;			
//			}
//		}
	}


	if (++g_u16LimitTime > 800)
	{
		g_u8SelfCheckFlag = 0;	
	}
}


/*****************************************************
*函数名称：void F_SELFCHECK_Output(void)
*函数功能：自检输出
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_SELFCHECK_Output(void)
{
	if (g_u8SelfCheckState == 0)
	{
//		if (g_u8StepCnt & 0x02)
//		{_IO_DCF_A_ON;_IO_DCF_B_OFF;}
//		else
//		{_IO_DCF_A_OFF;_IO_DCF_B_ON;}

//		_IO_RPM2000_OFF;
//	    _IO_RPM3000_OFF;
//		_IO_MIDPRT_OFF;
//		_IO_LOWPRT_OFF;
		
		if (g_u8StepCnt < 5)
		{g_sDefrost.u16Duty = 100;}
		else if (g_u8StepCnt < 8)
		{g_sDefrost.u16Duty = 30;}
		else
		{g_sDefrost.u16Duty = 0;}			
	}

	else if (g_u8SelfCheckState == 1)
	{
//		_IO_DCF_A_OFF;
//		_IO_DCF_B_OFF;
	}

	else if (g_u8SelfCheckState == 2)
	{
		if (g_u8StepCnt == 1)
		{_IO_RPM2000_ON;}
		else
		{_IO_RPM2000_OFF;}

		if (g_u8StepCnt == 0)
		{_IO_RPM3000_ON;}
		else
		{_IO_RPM3000_OFF;}

		if (g_u8StepCnt == 3)
		{_IO_MIDPRT_ON;}
		else
		{_IO_MIDPRT_OFF;}

		if (g_u8StepCnt == 4)
		{_IO_LOWPRT_ON;}
		else
		{_IO_LOWPRT_OFF;}
	}
}