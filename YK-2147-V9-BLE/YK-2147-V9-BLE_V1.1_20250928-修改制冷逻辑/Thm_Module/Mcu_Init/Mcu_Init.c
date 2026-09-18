//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: SC_Init.c
//	模块功能	: MCU初始化函数C文件
//  说明        :  
//*************************************************************
#include "COMMON\AllInclude.h"

void F_MCU_Init(void)
{
	//IO口配置
	P0 = 0x00;
	P2 = 0x00;
	P3 = 0x00;

	GPIO_SET_MUX_MODE(P01CFG,GPIO_MUX_GPIO);
	GPIO_ENABLE_OUTPUT(P0TRIS,GPIO_PIN_1);
//	GPIO_ENABLE_UP(P0UP,GPIO_PIN_1);

	GPIO_SET_MUX_MODE(P02CFG,GPIO_MUX_GPIO);
	GPIO_ENABLE_OUTPUT(P0TRIS,GPIO_PIN_2);
//	GPIO_ENABLE_UP(P0UP,GPIO_PIN_2);

	GPIO_SET_MUX_MODE(P03CFG,GPIO_MUX_GPIO);
	GPIO_ENABLE_OUTPUT(P0TRIS,GPIO_PIN_3);

	GPIO_SET_MUX_MODE(P04CFG,GPIO_MUX_GPIO);
	GPIO_ENABLE_OUTPUT(P0TRIS,GPIO_PIN_4);
//	GPIO_ENABLE_UP(P0UP,GPIO_PIN_4);

	GPIO_SET_MUX_MODE(P05CFG,GPIO_MUX_GPIO);
	GPIO_ENABLE_OUTPUT(P0TRIS,GPIO_PIN_5);		   
//	GPIO_ENABLE_UP(P0UP,GPIO_PIN_5);

	GPIO_SET_MUX_MODE(P21CFG,GPIO_MUX_GPIO);
	GPIO_ENABLE_OUTPUT(P2TRIS,GPIO_PIN_1);

	GPIO_SET_MUX_MODE(P22CFG,GPIO_MUX_GPIO);
	GPIO_ENABLE_OUTPUT(P2TRIS,GPIO_PIN_2);
//	GPIO_ENABLE_UP(P2UP,GPIO_PIN_2);

	GPIO_SET_MUX_MODE(P23CFG,GPIO_MUX_GPIO);	 
	GPIO_ENABLE_OUTPUT(P2TRIS,GPIO_PIN_3);

	GPIO_SET_MUX_MODE(P24CFG,GPIO_MUX_ANALOG);
	GPIO_ENABLE_INPUT(P2TRIS,GPIO_PIN_4);

	GPIO_SET_MUX_MODE(P26CFG,GPIO_MUX_GPIO);
	GPIO_ENABLE_OUTPUT(P2TRIS,GPIO_PIN_6);

	GPIO_SET_MUX_MODE(P30CFG,GPIO_MUX_GPIO);
	GPIO_ENABLE_OUTPUT(P3TRIS,GPIO_PIN_0);

	GPIO_SET_MUX_MODE(P31CFG,GPIO_MUX_GPIO);
	GPIO_ENABLE_OUTPUT(P3TRIS,GPIO_PIN_1);

	GPIO_SET_MUX_MODE(P32CFG,GPIO_MUX_GPIO);
	GPIO_ENABLE_INPUT(P3TRIS,GPIO_PIN_2);

	GPIO_SET_MUX_MODE(P35CFG,GPIO_MUX_GPIO);
	GPIO_ENABLE_OUTPUT(P3TRIS,GPIO_PIN_5);

	P0 = 0x00;
	P2 = 0x00;
	P3 = 0x00;


/////////////////////////////////////////////////////

	/*系统时钟架构可参考手册4.1章节"系统时钟结构"*/
	/*(1)通过选项Options->Debug->Seting中设置时钟Fosc时钟
	    Fosc 可选择:
		(1) HSI(48Mhz)(可选分频：1、2、3、6)
		(2) HSE(高速晶振:8Mhz、16Mhz)
		(3) LSE(低速晶振:32.768Khz)	
		(4) LSI(125Khz)	
	*/
	// Fosc = HSI/2 = 24Mhz;
	
	/*(2)通过选项Options->Debug->Seting中设置时钟Fsys_pre时钟
	   Fsys_pre = Fosc/SYS_PRESCALE(系统时钟预分频: 1、2、4、8)*/
	
	// Fsys_pre = Fosc/1 = 24Mhz;
	
	/*(3)通过寄存器CLKDIV配置Fsys(系统时钟): Fsys = Fsys_pre/分频*/
		
	SYS_SET_SYSTEM_CLK(SYS_CLK_DIV_1);

/////////////////////////////////////////////////////

	//ADC配置
	//设置ADC的运行模式
	ADC_ConfigRunMode(ADC_CLK_DIV_256, ADC_RESULT_LEFT);	//设置ADC时钟为系统时钟的128分频，ADC结果为左对齐，如有对ADC时钟有操作请参考"中微8051芯片ADC模块应用笔记"
	
	//设置ADC转换通道
//	ADC_EnableChannel(ADC_CH_16);	
//	GPIO_SET_MUX_MODE(P35CFG, GPIO_MUX_ANALOG);
//
//	ADC_EnableChannel(ADC_CH_14);	
//	GPIO_SET_MUX_MODE(P32CFG, GPIO_MUX_ANALOG);
//
//	ADC_EnableChannel(ADC_CH_22);	
//	GPIO_SET_MUX_MODE(P30CFG, GPIO_MUX_ANALOG);
//
//	ADC_EnableChannel(ADC_CH_9);	
//	GPIO_SET_MUX_MODE(P23CFG, GPIO_MUX_ANALOG);

	ADC_EnableChannel(ADC_CH_10);	
	GPIO_SET_MUX_MODE(P24CFG, GPIO_MUX_ANALOG);

	ADC_Start();	//使能ADC模块

/////////////////////////////////////////////////////

	/*
	(1)设置Timer的运行模式
	*/
	TMR_ConfigRunMode(TMR0, TMR_MODE_TIMING,TMR_TIM_AUTO_8BIT);	
	/*
	(2)设置Timer 运行时钟
	*/
	TMR_ConfigTimerClk(TMR0, TMR_CLK_DIV_12);						/*Fsys = 24Mhz，Ftimer = 2Mhz,Ttmr=0.5us*/
	/*
	(3)设置Timer周期
	*/	
	TMR_ConfigTimerPeriod(TMR0, 256-250, 256-250); 				// 200*0.5us = 100us,递增计数
		
	/*
	(4)开启中断
	*/
	TMR_EnableOverflowInt(TMR0);

	/*
	(5)设置Timer中断优先级
	*/	
	IRQ_SET_PRIORITY(IRQ_TMR0,IRQ_PRIORITY_LOW);
//	IRQ_ALL_ENABLE();	

	/*
	(6)开启Timer
	*/
	TMR_Start(TMR0);

/////////////////////////////////////////////////////

//	//设置EPWM运行模式
//	EPWM_ConfigRunMode(EPWM_OC_INDEPENDENT|EPWM_OCU_SYMMETRIC|EPWM_COUNT_DOWN); //互补模式+输出独立模式+对称计数模式+向下计数(边沿对齐)模式
//	//设置EPWM运行时钟	
//	EPWM_ConfigChannelClk(EPWM0, EPWM_CLK_DIV_2);		
//	EPWM_ConfigChannelClk(EPWM2, EPWM_CLK_DIV_2);
//	EPWM_ConfigChannelClk(EPWM4, EPWM_CLK_DIV_2);
//    EPWM_ConfigChannelClk(EPWM1, EPWM_CLK_DIV_2);
//
//	//设置EPWM 周期以及占空比	
//	EPWM_ConfigChannelPeriod(EPWM0, C_PWM_PERIOD_100);
//	EPWM_ConfigChannelPeriod(EPWM2, C_PWM_PERIOD_100);
//	EPWM_ConfigChannelPeriod(EPWM4, C_PWM_PERIOD_100);
//	EPWM_ConfigChannelPeriod(EPWM1, C_PWM_PERIOD_100);
//
//	EPWM_ConfigChannelSymDuty(EPWM0, 0x00);
//	EPWM_ConfigChannelSymDuty(EPWM2, 0x00);
//	EPWM_ConfigChannelSymDuty(EPWM4, 0x00);
//    EPWM_ConfigChannelSymDuty(EPWM1, C_PWM_PERIOD_100);
//
//	//设置EPWM的加载方式为自动加载
//	EPWM_EnableAutoLoadMode(EPWM_CH_4_MSK|EPWM_CH_2_MSK|EPWM_CH_0_MSK|EPWM_CH_1_MSK);
//	//设置EPWM的输出极性
//	EPWM_DisableReverseOutput(EPWM_CH_0_MSK|EPWM_CH_1_MSK|EPWM_CH_2_MSK|EPWM_CH_3_MSK|EPWM_CH_4_MSK|EPWM_CH_5_MSK); //关闭反向输出
////	EPWM_EnableReverseOutput(EPWM_CH_0_MSK);
//
//	//设置EPWM输出
//	EPWM_EnableOutput(EPWM_CH_0_MSK|EPWM_CH_2_MSK|EPWM_CH_4_MSK|EPWM_CH_1_MSK);		//开启输出
//	//设置EPWM死区
//	EPWM_DisableDeadZone(EPWM0);
//	EPWM_DisableDeadZone(EPWM2);
//	EPWM_DisableDeadZone(EPWM4);
//	EPWM_DisableDeadZone(EPWM1);
//
//	//设置EPWM中断
////	EPWM_EnableZeroInt(EPWM_CH_0_MSK);				//开启EPWM零点中断
////	EPWM_AllIntEnable();							//开启EPWM总中断
////	IRQ_SET_PRIORITY(IRQ_PWM,IRQ_PRIORITY_HIGH);
////	IRQ_ALL_ENABLE();								//开启总中断
//
//	//设置EPWM输出IO复用
//	GPIO_SET_MUX_MODE(P21CFG, GPIO_MUX_PG0);
//	GPIO_SET_MUX_MODE(P01CFG, GPIO_MUX_PG4);
//	GPIO_SET_MUX_MODE(P02CFG, GPIO_MUX_PG2);
//	GPIO_SET_MUX_MODE(P05CFG, GPIO_MUX_PG1);
//
//	//开启EPWM
//	EPWM_Start(EPWM_CH_4_MSK|EPWM_CH_2_MSK|EPWM_CH_0_MSK|EPWM_CH_1_MSK);

/////////////////////////////////////////////////////	

	/*
	(1)设置LVD 监测电压
	*/
	SYS_ConfigLVD(SYS_LVD_4_3V);
	/*
	(2)设置LVD中断
	*/	
	SYS_EnableLVDInt();
	IRQ_SET_PRIORITY(IRQ_LVD,IRQ_PRIORITY_HIGH);
	IRQ_ALL_ENABLE();
	/*
	(3)开启LVD
	*/		
	SYS_EnableLVD();
	
/////////////////////////////////////////////////////			
	/*
	(1)喂狗，避免WDT时间太短，进入WDT复位
	*/
	WDT_ClearWDT();
	/*
	(2)开启WDT溢出时间
	*/
	WDT_ConfigOverflowTime(WDT_CLK_16777216);	// Fsys=24Mhz -> Twdt = 4194304/24 = 174.76ms
	/*
	(3)设置WDT溢出中断
	*/
	WDT_EnableOverflowInt();

	IRQ_SET_PRIORITY(IRQ_WDT, IRQ_PRIORITY_HIGH);
	IRQ_ALL_ENABLE();




}