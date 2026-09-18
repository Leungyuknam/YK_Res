#ifndef SOC_DEBUG_TOUCHKEY_H
#define	SOC_DEBUG_TOUCHKEY_H

//=====================================
//		声明可供外部调用的函数
//=====================================
/*=============================================
*函数名称：void SOCAPI_DeBugTouchKey_Init(void) 
*函数功能：触控调试初始化
*入口参数：unsigned char ChipType  IC类型 
		   0：表示SC91F8301 SC91F8311 SC91F8302  SC91F8322；
		   1：表示SC91F842 SC91F843 SC91F844；
*出口参数：void
================================================*/	
extern void SOCAPI_DeBugTouchKey_Init(void);
#endif