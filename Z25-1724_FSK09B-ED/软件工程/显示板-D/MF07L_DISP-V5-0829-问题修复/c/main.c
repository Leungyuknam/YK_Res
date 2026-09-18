 //************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: main.c
//	作者		: Yanni
//	模块功能	: SC91F8311触控DEMO
//  最后更正日期:
// 	版本		: 2015-12-25:
//              :  
//*************************************************************
#include "Include\Include.h"
#include "lib\SensorMethod.h"

#ifdef __Uart_TOUCH_DEBUG__
//	#include "..\SOC_DebugTouchKey_Lib\SOC_DebugTouchKey.h"
#endif


/**************************************************
*函数名称：void main(void)								  
*函数功能：主函数
*入口参数：void
*出口参数：void  
**************************************************/
void main(void)
{					                                                                                      
	F_McuInit();
	#ifndef __Uart_TOUCH_DEBUG__
	F_SYSUartInit();
	#endif
//	TouchKeyInit();
	F_mSystemInital();
	BELL_NOR();
	#ifdef __Uart_TOUCH_DEBUG__
		#warning "----调试触摸，获取动态触摸数据------"
		SOCAPI_DeBugTouchKey_Init();
	#endif
	while(1)
	{
		WDTCON  = 0x10;	   	  
		TimeProc();
		if(GetTimeTick(TIME_10MS))
		{
			F_KeyDealControl();
			BeepDeal();
			PowErrCheck();
		}
		F_TimeEvent();
		F_mTransferDataDeal();//通讯或者红外数据处理接口
		F_mAllSenseDeal();
		if (GetTimeTick(TIME_100MS))
		{
			Display_Value_Set();
			if(SysMode == SysMX_Factory)
				FactoryOutPutDeal();
			else
				OutPutDeal();
		}
		#ifndef __Uart_TOUCH_DEBUG__
		F_UartDuplexProc();
		#endif
	} 
}
