//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: sc95f7x1x_pwm.h
//	作者		: 
//	模块功能	: PWM固件库函数头文件
//	局部函数列表:
//  最后更正日期: 2019/7/10
// 	版本		: V1.0 
//  说明        ：  
//*************************************************************

#ifndef _sc95f7x1x_PWM_H_
#define	_sc95f7x1x_PWM_H_

#include "sc95f7x1x.h"

typedef enum {
  PWM40 = (uint8_t)0x01,   //PWM输出通道选择：PWM40
  PWM41 = (uint8_t)0x02,   //PWM输出通道选择：PWM41
  PWM42 = (uint8_t)0x04,   //PWM输出通道选择：PWM42
  PWM43 = (uint8_t)0x08,   //PWM输出通道选择：PWM43
  PWM50 = (uint8_t)0x10,   //PWM输出通道选择：PWM50
  PWM51 = (uint8_t)0x20,   //PWM输出通道选择：PWM51
  PWM52 = (uint8_t)0x40,   //PWM输出通道选择：PWM52
  PWM53 = (uint8_t)0x80	   //PWM输出通道选择：PWM53
}PWM_OutputPin_TypeDef;

typedef enum {
  PWM_PRESSEL_FHRC_D1  = (uint8_t)0x00,	   //PWM预分频为Fhrc/1
  PWM_PRESSEL_FHRC_D2  = (uint8_t)0x10,	   //PWM预分频为Fhrc/2
  PWM_PRESSEL_FHRC_D4  = (uint8_t)0x20,	   //PWM预分频为Fhrc/4
  PWM_PRESSEL_FHRC_D8  = (uint8_t)0x30	   //PWM预分频为Fhrc/8
}PWM_PresSel_TypeDef;

typedef enum {
	PWM40PWM41 = (uint8_t)0x00,					           //PWM互补模式通道选择：PWM40、PWM41 
	PWM42PWM43 = (uint8_t)0x02,					           //PWM互补模式通道选择：PWM42、PWM43
	PWM50PWM51 = (uint8_t)0x04,					           //PWM互补模式通道选择：PWM50、PWM51
	PWM52PWM53 = (uint8_t)0x06					           //PWM互补模式通道选择：PWM52、PWM53
} PWM_ComplementaryOutputPin_TypeDef;

typedef enum{
  PWM_OUTPUTSTATE_DISABLE            = ((uint8_t)0x00),	   //该PIN脚作为GPIO
  PWM_OUTPUTSTATE_ENABLE	         = ((uint8_t)0x01)	   //该PIN脚作为PWM
}PWM_OutputState_TypeDef;

typedef enum{
  PWM_POLARITY_NON_INVERT            = ((uint8_t)0x00),		//PWM输出不反向
  PWM_POLARITY_INVERT                = ((uint8_t)0x01)		//PWM输出反向
}PWM_Polarity_TypeDef;

typedef enum{
  PWM_Latch_Mode                     = ((uint8_t)0x00),		//PWM故障检测模式：锁存模式
  PWM_Immediate_Mode                 = ((uint8_t)0x20)		//PWM故障检测模式：立即模式
}PWM_FaultDetectionMode_TypeDef;

typedef enum{
  PWM_FaultDetectionVoltage_Low            = ((uint8_t)0x00),		//PWM故障检测低电平选择
  PWM_FaultDetectionVoltage_high           = ((uint8_t)0x10)		//PWM故障检测高电平选择
}PWM_FaultDetectionVoltageSelect_TypeDef;

typedef enum{
  PWM_WaveFilteringTime_0us                = ((uint8_t)0x00),		//PWM故障检测输入信号滤波时间0us
  PWM_WaveFilteringTime_1us                = ((uint8_t)0x01),		//PWM故障检测输入信号滤波时间1us
  PWM_WaveFilteringTime_4us                = ((uint8_t)0x02),		//PWM故障检测输入信号滤波时间4us
  PWM_WaveFilteringTime_16us               = ((uint8_t)0x03)		//PWM故障检测输入信号滤波时间16us
}PWM_FaultDetectionWaveFilteringTime_TypeDef;

typedef enum {
	PWM_Edge_Aligned_Mode  	   = (uint8_t)0x00,	//PWM边沿对齐模式
	PWM_Center_Alignment_Mode  = (uint8_t)0x01	//PWM中间对齐模式
} PWM_Aligned_Mode_TypeDef;

void PWM_DeInit(void);
void PWM_Init(PWM_PresSel_TypeDef PWM_PresSel, uint16_t PWM_Period);
void PWM_OutputStateConfig(uint8_t PWM_OutputPin, PWM_OutputState_TypeDef PWM_OutputState);
void PWM_PolarityConfig(PWM_OutputPin_TypeDef PWM_OutputPin, PWM_Polarity_TypeDef PWM_Polarity);
void PWM_IndependentModeConfig(PWM_OutputPin_TypeDef PWM_OutputPin, uint16_t PWM_DutyCycle);
void PWM_ComplementaryModeConfig(PWM_ComplementaryOutputPin_TypeDef PWM_ComplementaryOutputPin, uint16_t PWM_DutyCycle);
void PWM_DeadTimeConfig(uint8_t PWM_RisingDeadTime, uint8_t PWM_fallingDeadTime);
void PWM_Cmd(FunctionalState NewState);
void PWM_ITConfig(FunctionalState NewState, PriorityStatus Priority);
FlagStatus PWM_GetFlagStatus(void);
FlagStatus PWM_GetFaultDetectionFlagStatus(void);
void PWM_ClearFlag(void);
void PWM_ClearFaultDetectionFlag(void);
void PWM_FaultDetectionConfig(FunctionalState NewState);
void PWM_FaultDetectionModeConfig(PWM_FaultDetectionMode_TypeDef FaultDetectionMode, PWM_FaultDetectionVoltageSelect_TypeDef FaultDetectionVoltageSelect, PWM_FaultDetectionWaveFilteringTime_TypeDef FaultDetectionWaveFilteringTime);
void PWM_Aligned_Mode_Select(PWM_Aligned_Mode_TypeDef PWM_Aligned_Mode);

#endif

/******************* (C) COPYRIGHT 2018 SinOne Microelectronics *****END OF FILE****/