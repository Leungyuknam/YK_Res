//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: sc95f7x1x_adc.c
//	作者		: 
//	模块功能	: ADC固件库函数C文件
//	局部函数列表:
//  最后更正日期: 2019/7/10
// 	版本		: V1.0 
//  说明        ：  
//*************************************************************

#include "sc95f7x1x_adc.h"

/**************************************************
*函数名称：void ADC_DeInit(void)
*函数功能：ADC相关寄存器复位至缺省值
*入口参数：void
*出口参数：void
**************************************************/
void ADC_DeInit(void)
{
	ADCCON = 0x00;
	ADCCFG0 = 0X00;
	ADCCFG1 = 0X00;
	ADCCFG2 = 0X00;
	ADCVL = 0X00;
	ADCVH = 0X00;
	EADC = 0;
	IPADC = 0;
}

/**************************************************
*函数名称：void ADC_Init(ADC_Cycle_TypeDef ADC_Cycle)
*函数功能：ADC初始化配置函数
*入口参数：ADC_Cycle		采样时间选择
*出口参数：void
**************************************************/
void ADC_Init(ADC_Cycle_TypeDef ADC_Cycle)
{
	ADCCFG2 = ADC_Cycle;			
}

/**************************************************
*函数名称：void ADC_ChannelConfig(ADC_Channel_TypeDef ADC_Channel, FunctionalState NewState)
*函数功能：ADC输入口配置函数
*入口参数：ADC_Channel				 ADC输入口选择
		   FunctionalState NewState	 ADCx使能/关闭选择	
*出口参数：void
**************************************************/
void ADC_ChannelConfig(ADC_Channel_TypeDef ADC_Channel, FunctionalState NewState)
{
	uint16_t tmpreg;

	ADCCON = (ADCCON & 0XE0) | ADC_Channel;
	tmpreg = (0x0001 << ADC_Channel);

	if(ADC_Channel < ADC_CHANNEL_VDD_D4)
	{
		if(NewState == DISABLE)
		{
			ADCCFG0 &= (~(uint8_t)tmpreg);
			ADCCFG1 &= (~(uint8_t)(tmpreg>>8)); 	
		}
		else
		{
			ADCCFG0 |= ((uint8_t)tmpreg);
			ADCCFG1 |= ((uint8_t)(tmpreg>>8));
		}
	} 		
}
/**************************************************
*函数名称：void ADC_EAINConfig(ADC_EAIN_TypeDef ADC_Channel, FunctionalState NewState)
*函数功能：将对应的ADC输入口设置为模拟输入模式
*入口参数：ADC_EAIN_Select		     选择需要设置的ADC口
		   FunctionalState NewState	 ADC通道使能/关闭选择	
*出口参数：void
**************************************************/
void ADC_EAINConfig(uint16_t ADC_EAIN_Select, FunctionalState NewState)
{
	if(NewState == DISABLE)
	{
		ADCCFG0 &= (~(uint8_t)ADC_EAIN_Select);
		ADCCFG1 &= (~(uint8_t)(ADC_EAIN_Select>>8)); 	
	}
	else
	{
		ADCCFG0 |= ((uint8_t)ADC_EAIN_Select);
		ADCCFG1 |= ((uint8_t)(ADC_EAIN_Select>>8));
	}
}
/*****************************************************
*函数名称：void ADC_Cmd(FunctionalState NewState)
*函数功能：ADC功能开关函数
*入口参数：FunctionalState NewState	功能启动/关闭选择
*出口参数：void
*****************************************************/
void ADC_Cmd(FunctionalState NewState)
{
	if(NewState == DISABLE)
	{
		ADCCON &= 0X7F;
	}
	else
	{
		ADCCON |= 0x80;
	}	
}

/*****************************************************
*函数名称：void ADC_StartConversion(void)
*函数功能：开始一次AD转换
*入口参数：void
*出口参数：void
*****************************************************/
void ADC_StartConversion(void)
{
	ADCCON |= 0X40;
}

/*****************************************************
*函数名称：uint16_t ADC_GetConversionValue(void)
*函数功能：获得一次AD转换数据
*入口参数：void
*出口参数：uint16_t
*****************************************************/
uint16_t ADC_GetConversionValue(void)
{
	uint16_t temp;
	temp = (uint16_t)(ADCVH<<4) + (ADCVL>>4);
	return temp;
}

/*****************************************************
*函数名称：void ADC_ITConfig(FunctionalState NewState, PriorityStatus Priority)
*函数功能：ADC中断初始化
*入口参数：FunctionalState NewState	中断使能/关闭选择
 		   PriorityStatus Priority	中断优先级选择
*出口参数：void
*****************************************************/
void ADC_ITConfig(FunctionalState NewState, PriorityStatus Priority)
{
    if(NewState == DISABLE)
    {
       EADC = 0;
    }
    else
    {
       EADC = 1;
    }
/************************************************************/
	if(Priority == LOW)
	{
		IPADC = 0;
	}
	else
	{
		IPADC = 1;
	}
}

/*****************************************************
*函数名称：FlagStatus ADC_GetFlagStatus(void)
*函数功能：获得ADC中断标志状态
*入口参数：void
*出口参数：FlagStatus	ADC中断标志状态	
*****************************************************/
FlagStatus ADC_GetFlagStatus(void)
{
	return (FlagStatus)(ADCCON & 0x20);
}

/*****************************************************
*函数名称：void ADC_ClearFlag(void)
*函数功能：清除ADC中断标志状态
*入口参数：void
*出口参数：void	
*****************************************************/
void ADC_ClearFlag(void)
{
   ADCCON &= 0xdf;
}

/******************* (C) COPYRIGHT 2018 SinOne Microelectronics *****END OF FILE****/