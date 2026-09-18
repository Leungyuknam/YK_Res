//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: SC_Init.c
//	模块功能	: MCU初始化函数C文件
//  说明        :  
//*************************************************************

#include "SC_Init.h"	//MCU初始化头文件，包含固件库所有头文件
#include "..\Drivers\SCDriver_list.h"
#include "HeadFiles\SC_itExtern.h"
//*************************************************************

/***********************user_code_area*********************/
/*<UserCodeStart>*/
/*<UserCodeEnd>*/
/**********************.user_code_area.********************/

/*****************************************************
*函数名称: SC_Init
*函数功能: MCU初始化函数
*入口参数：void
*出口参数：void
*****************************************************/
void SC_Init(void)
{
	
	SC_GPIO_Init();
	SC_OPTION_Init();
	SC_TIM0_Init();
	SC_ADC_Init();
	SC_WDT_Init();
	SC_IAP_Init();
	/*write initial function here*/	
	EA = 1;	
}

/*****************************************************
*函数名称: SC_OPTION_Init
*函数功能: OPTION配置初始化函数
*入口参数：void
*出口参数：void
*****************************************************/
void SC_OPTION_Init(void)
{
	OPTION_SYSCLK_Init(SYSCLK_PRESSEL_FOSC_D2);
	OPTION_ADC_VrefConfig(ADC_VREF_VDD);
	OPTION_LVR_Init(LVR_1_9V);
	OPTION_IAP_SetOperateRange(IAP_OPERATERANGE__LAST_1K_CODEREGION);
	OPTION_RST_PIN_Cmd(DISABLE);
	OPTION_XTIPLL_Cmd(DISABLE);
	OPTION_JTG_Cmd(DISABLE);
	/*OPTION_Init write here*/		
}

/*****************************************************
*函数名称: SC_GPIO_Init
*函数功能: GPIO初始化函数
*入口参数：void
*出口参数：void
*****************************************************/
void SC_GPIO_Init(void)
{
	GPIO_Init(GPIO1, GPIO_PIN_0,GPIO_MODE_OUT_PP);
	GPIO_Init(GPIO5, GPIO_PIN_1,GPIO_MODE_IN_PU);
	GPIO_Init(GPIO5, GPIO_PIN_0,GPIO_MODE_OUT_PP);
	GPIO_Init(GPIO1, GPIO_PIN_1,GPIO_MODE_OUT_PP);
	GPIO_Init(GPIO1, GPIO_PIN_2,GPIO_MODE_OUT_PP);
	GPIO_Init(GPIO1, GPIO_PIN_3,GPIO_MODE_OUT_PP);
	GPIO_Init(GPIO1, GPIO_PIN_4,GPIO_MODE_IN_HI);
	GPIO_Init(GPIO1, GPIO_PIN_5,GPIO_MODE_OUT_PP);
	GPIO_Init(GPIO1, GPIO_PIN_6,GPIO_MODE_OUT_PP);
	GPIO_Init(GPIO1, GPIO_PIN_7,GPIO_MODE_IN_HI);
	GPIO_Init(GPIO2, GPIO_PIN_7,GPIO_MODE_IN_PU);
	GPIO_Init(GPIO2, GPIO_PIN_6,GPIO_MODE_IN_PU);
	GPIO_Init(GPIO2, GPIO_PIN_5,GPIO_MODE_IN_PU);
	GPIO_Init(GPIO2, GPIO_PIN_3,GPIO_MODE_OUT_PP);
	GPIO_Init(GPIO2, GPIO_PIN_2,GPIO_MODE_OUT_PP);
	GPIO_Init(GPIO2, GPIO_PIN_1,GPIO_MODE_OUT_PP);
	GPIO_Init(GPIO2, GPIO_PIN_0,GPIO_MODE_OUT_PP);
	GPIO_Init(GPIO0, GPIO_PIN_7,GPIO_MODE_OUT_PP);
	GPIO_Init(GPIO0, GPIO_PIN_6,GPIO_MODE_OUT_PP);
	GPIO_Init(GPIO0, GPIO_PIN_5,GPIO_MODE_OUT_PP);
	GPIO_Init(GPIO0, GPIO_PIN_4,GPIO_MODE_OUT_PP);
	GPIO_Init(GPIO0, GPIO_PIN_3,GPIO_MODE_OUT_PP);
	GPIO_Init(GPIO0, GPIO_PIN_2,GPIO_MODE_OUT_PP);
	GPIO_Init(GPIO0, GPIO_PIN_1,GPIO_MODE_OUT_PP);
	GPIO_Init(GPIO0, GPIO_PIN_0,GPIO_MODE_OUT_PP);
	/*GPIO_Init write here*/		
}

/*****************************************************
*函数名称: SC_UART0_Init
*函数功能: UART0初始化函数
*入口参数：void
*出口参数：void
*****************************************************/
void SC_UART0_Init(void)
{
	/*UART0_Init write here*/		
}

/*****************************************************
*函数名称: SC_TIM0_Init
*函数功能: TIMER0初始化函数
*入口参数：void
*出口参数：void 
*****************************************************/
void SC_TIM0_Init(void)
{
	TIM0_TimeBaseInit(TIM0_PRESSEL_FSYS_D12,TIM0_MODE_TIMER);
	TIM0_WorkModeConfig(TIM0_WORK_MODE2,89, 0);
	TIM0_ITConfig(ENABLE,HIGH);
	TIM0_Cmd(ENABLE);
	/*TIM0_Init write here*/		
}

/*****************************************************
*函数名称: SC_TIM1_Init
*函数功能: TIMER1初始化函数
*入口参数：void
*出口参数：void
*****************************************************/
void SC_TIM1_Init(void)
{
	/*TIM1_Init write here*/	
}

/*****************************************************
*函数名称: SC_TIM2_Init
*函数功能: TIMER2初始化函数
*入口参数：void
*出口参数：void
*****************************************************/
void SC_TIM2_Init(void)
{
	/*TIM2_Init write here*/	
}
/*****************************************************
*函数名称: SC_TIM3_Init
*函数功能: TIMER3初始化函数
*入口参数：void
*出口参数：void
*****************************************************/
void SC_TIM3_Init(void)
{
	/*TIM3_Init write here*/	
}
/*****************************************************
*函数名称: SC_TIM4_Init
*函数功能: TIMER4初始化函数
*入口参数：void
*出口参数：void
*****************************************************/
void SC_TIM4_Init(void)
{
	/*TIM4_Init write here*/	
}
/*****************************************************
*函数名称: SC_PWM_Init
*函数功能: PWM初始化函数
*入口参数：void
*出口参数：void 
*****************************************************/
void SC_PWM_Init(void)
{
	/*PWM_Init write here*/	
}
/*****************************************************
*函数名称: SC_PWM0_Init
*函数功能: PWM0初始化函数
*入口参数：void
*出口参数：void 
*****************************************************/
void SC_PWM0_Init(void)
{
	/*PWM0_Init write here*/	
}
/*****************************************************
*函数名称: SC_PWM1_Init
*函数功能: PWM1初始化函数
*入口参数：void
*出口参数：void 
*****************************************************/
void SC_PWM1_Init(void)
{
	/*PWM1_Init write here*/	
}
/*****************************************************
*函数名称: SC_PWM2_Init
*函数功能: PWM1初始化函数
*入口参数：void
*出口参数：void 
*****************************************************/
void SC_PWM2_Init(void)
{
	/*PWM2_Init write here*/	
}
/*****************************************************
*函数名称: SC_PWM3_Init
*函数功能: PWM1初始化函数
*入口参数：void
*出口参数：void 
*****************************************************/
void SC_PWM3_Init(void)
{
	/*PWM3_Init write here*/	
}
/*****************************************************
*函数名称: SC_PWM4_Init
*函数功能: PWM1初始化函数
*入口参数：void
*出口参数：void 
*****************************************************/
void SC_PWM4_Init(void)
{
	/*PWM4_Init write here*/	
}
/*****************************************************
*函数名称: SC_INT_Init
*函数功能: INT初始化函数
*入口参数：void
*出口参数：void
*****************************************************/
void SC_INT_Init(void)
{
	/*INT_Init write here*/	
}

/*****************************************************
*函数名称: SC_ADC_Init
*函数功能: ADC初始化函数
*入口参数：void
*出口参数：void 
*****************************************************/
void SC_ADC_Init(void)
{
	ADC_Init(ADC_PRESSEL_6CLOCK,ADC_Cycle_Null);
	/*AIN2口模式设置*/ADC_EAINConfig(ADC_EAIN_2,ENABLE);
	/*AIN9口模式设置*/ADC_EAINConfig(ADC_EAIN_9,ENABLE);
	ADC_ChannelConfig(ADC_CHANNEL_2,ENABLE);
	ADC_ITConfig(DISABLE,LOW);
	ADC_Cmd(ENABLE);
	/*ADC_Init write here*/
}

/*****************************************************
*函数名称: SC_IAP_Init
*函数功能: IAP初始化函数
*入口参数：void
*出口参数：void 
*****************************************************/
void SC_IAP_Init(void)
{
	/*IAP_Init write here*/		
}

/*****************************************************
*函数名称: SC_USCI0_Init
*函数功能: USCI0初始化函数
*入口参数：void
*出口参数：void
*****************************************************/
void SC_USCI0_Init(void)
{
	/*USCI0_Init write here*/		
}
/*****************************************************
*函数名称: SC_USCI1_Init
*函数功能: USCI1初始化函数
*入口参数：void
*出口参数：void
*****************************************************/
void SC_USCI1_Init(void)
{
	/*USCI1_Init write here*/		
}
/*****************************************************
*函数名称: SC_USCI2_Init
*函数功能: USCI2初始化函数
*入口参数：void
*出口参数：void
*****************************************************/
void SC_USCI2_Init(void)
{
	/*USCI2_Init write here*/		
}
/*****************************************************
*函数名称: SC_USCI3_Init
*函数功能: USCI3初始化函数
*入口参数：void
*出口参数：void
*****************************************************/
void SC_USCI3_Init(void)
{
	/*USCI3_Init write here*/		
}
/*****************************************************
*函数名称: SC_USCI4_Init
*函数功能: USCI4初始化函数
*入口参数：void
*出口参数：void
*****************************************************/
void SC_USCI4_Init(void)
{
	/*USCI4_Init write here*/		
}
/*****************************************************
*函数名称: SC_BTM_Init
*函数功能: 低频时钟定时器初始化函数
*入口参数：void
*出口参数：void 
*****************************************************/
void SC_BTM_Init(void)
{
	/*BTM_Init write here*/		
}

/*****************************************************
*函数名称: SC_CRC_Init
*函数功能: check sum 初始化函数
*入口参数：void
*出口参数：void
*****************************************************/
void SC_CRC_Init(void)
{
	/*CRC_Init write here*/	
}

/*****************************************************
*函数名称: SC_WDT_Init
*函数功能: 看门狗初始化函数
*入口参数：void
*出口参数：void 
*****************************************************/
void SC_WDT_Init(void)
{
	WDT_Init(WDT_OverflowTime_500MS);
	OPTION_WDT_Cmd(ENABLE);
	/*WDT_Init write here*/		
}

/*****************************************************
*函数名称: SC_PWR_Init
*函数功能: 电源/功耗控制初始化函数
*入口参数：void
*出口参数：void 
*****************************************************/
void SC_PWR_Init(void)
{
	/*PWR_Init write here*/		
}
/*****************************************************
*函数名称: SC_DDIC_Init
*函数功能: 显示控制初始化函数
*入口参数：void
*出口参数：void 
*****************************************************/
void SC_DDIC_Init(void)
{
	/*DDIC_Init write here*/	
}
/*****************************************************
*函数名称: SC_ACMP_Init
*函数功能: 运放初始化函数
*入口参数：void
*出口参数：void 
*****************************************************/
void SC_ACMP_Init(void)
{
	/*ACMP_Init write here*/	
}
/*****************************************************
*函数名称: SC_MDU_Init
*函数功能: 乘除法器初始化函数
*入口参数：void
*出口参数：void 
*****************************************************/
void SC_MDU_Init(void)
{
	/*MDU_Init write here*/	
}
