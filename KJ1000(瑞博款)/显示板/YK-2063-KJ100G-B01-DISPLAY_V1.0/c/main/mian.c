 //************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: main.c
//	作者		: 
//	模块功能	: 触控DEMO
//  最后更正日期:
// 	版本		: 
//              :  
//*************************************************************
/*   
通用框架版本V1.0 2022.10.24
提供的服务：
1.1	定时中断模块，固定为125US定时中断，产生系统基准时间标志。
1.2	系统时钟信号产生模块，文件名：SystemTime.c  提供服务F_mSystemTimeDeal();
1.3	AD检测服务提供模块。文件名：AD.c  提供服务：unsigned char F_GetAD(unsigned char ch,AD_Structure *AD_Str,unsigned int MaxValue,unsigned int MinValue)
1.4	按键扫描和处理服务模块。文件名: Key.c 提供服务：void F_mKeyControl(void); 需要配置keyCfg.h表格，并实现按键处理函数在KeyFunction.c文件中
提供的接口：
2.1    F_mCpuInital(); CPU初始化，需要你添加代码在此函数里面
2.2    F_mAllADSenseDeal(); AD 处理，需要你添加代码在此函数里面
2.2    F_mFunctionSchdule(); 项目功能处理总入口，执行时间可调。
        此接口为程序总控制逻辑的入口，负责程序总体逻辑的控制最终输出对应负载的输出信号。一般情况建议此接口内的代码执行周期为100MS，有特殊功能要求的可做调整。
2.3    F_mSelfTestSchdule(); 自检执行进入接口。
2.4    F_mSystemInital();逻辑层面的初始化 
*/

#include "h\Allinclude.h"

/**************************************************
*函数名称：void main(void)								  
*函数功能：主函数
*入口参数：void
*出口参数：void  
**************************************************/
void main(void)
{					
	cli();
    F_mCpuInital();//CPU初始化操作
//    InitArraryFunc();
//    F_mSystemInital();//项目逻辑层数据、状态初始化操作
//    F_mDispInital();
	//触控按键初始化
	TouchKeyInit();
//    F_BuzzerSet(BUZ_POWER);
	sei();
	while(1)
	{
		//---固有接口可以根据项目选择需要哪些接口-----------------------------
		WDT_clear();
		F_mSystemTimeDeal();
		F_mDisplayControl();//显示处理，包括数据更新和底层扫描驱动
		F_mAllSenseDeal();//所有传感器检测处理 
		F_mKeyControl();//按键系统处理包括扫描和处理函数
		F_mTransferDataDeal();//通讯或者红外数据处理接口
//        if(g_u8TestState)
//        {   F_mSelfTestSchdule();}
//        else
//        {   F_mFunctionSchdule();}//功能逻辑处理，包括负载的输出控制
//		//---其他接口用于特定项目需要的特定处理模块接口----------------------
//        F_mBuzProc();
//        DCFanProc();
//        F_EepromReadCtrl();
//        F_IapWriteCtrl();
	} 
}