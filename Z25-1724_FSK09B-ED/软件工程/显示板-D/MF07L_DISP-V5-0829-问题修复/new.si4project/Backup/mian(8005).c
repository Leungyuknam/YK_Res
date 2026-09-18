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

//*****************全局变量区***************************************************
//u32 g_u32KeyValueFlag = 0;		//当前轮按键标志


/**************************************************
*函数名称：void  Sys_Scan(void) 
*函数功能：扫描TK和显示
*入口参数：void
*出口参数：void  
**************************************************/
void Sys_Scan(void)
{      				
	if(SOCAPI_TouchKeyStatus&0x80)	    //重要步骤2:  触摸键扫描一轮标志，是否调用TouchKeyScan()一定要根据此标志位置起后
	 {	   																	
		SOCAPI_TouchKeyStatus &= 0x7f;	//重要步骤3: 清除标志位， 需要外部清除。													    
		g_u32KeyBuf = TouchKeyScan();//按键数据处理函数    
		TouchKeyRestart();				//启动下一轮转换																														 			
	}		  	   
}
/**************************************************
*函数名称：void main(void)								  
*函数功能：主函数
*入口参数：void
*出口参数：void  
**************************************************/
void main(void)
{					                                                                                      
	F_McuInit();
	F_GetCapAD();
	//10min后 153  150
	//触控按键初始化
	TouchKeyInit();
	if(AD_Value >= 310)
	{
		F_ReadFlashData();
	}
	while(1)
	{
		WDTCON  = 0x10;	   	  
		TimeProc();
		if(g_u8BaseTime >= 40)
		{
			g_u8BaseTime = 0;  //5MS
			Deal_CommData();
			Sys_Scan();
		}
		if(GetTimeTick(TIME_10MS))
		{
			F_KeyDealControl();
			BeepDeal();
			OutPut_Buzz();
//			EppromProc();
			Set_Work();
		}
		F_TimeEvent();
		if (GetTimeTick(TIME_100MS))
		{
			Display_Value_Set();
			DispPowKey();
			TM1668_Set(g_u8DispSeg,2);
			CAP_SetInPULL_ON();
		}
		#ifndef _TEST_DEBUG_
		ADProc();
		#endif
	} 
}
