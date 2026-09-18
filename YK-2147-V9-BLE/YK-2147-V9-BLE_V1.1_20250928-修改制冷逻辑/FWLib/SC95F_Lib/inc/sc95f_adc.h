//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: sc95f_adc.h
//	作者		:
//	模块功能	: ADC固件库函数头文件
//  最后更正日期: 2021/5/20
// 	版本		: V1.10001
//  说明        : 该文件仅适用于SC95F系列芯片
//*************************************************************

#ifndef _sc95f_ADC_H_
#define _sc95f_ADC_H_

#include "sc95f.h"

//ADC采样时钟分频枚举
typedef enum
{
  ADC_PRESSEL_3CLOCK = (uint8_t)0x10,  //采样时间为3个系统时钟
  ADC_PRESSEL_6CLOCK = (uint8_t)0x14,  //采样时间为6个系统时钟
  ADC_PRESSEL_16CLOCK = (uint8_t)0x18, //采样时间为16个系统时钟
  ADC_PRESSEL_32CLOCK = (uint8_t)0x1c  //采样时间为32个系统时钟
} ADC_PresSel_TypeDef;

//ADC采样周期选择枚举
typedef enum
{
  ADC_Cycle_Null = 0x00 //95系列该功能无效
} ADC_Cycle_TypeDef;

//ADC采样通道选择
#if defined(SC95F8x1x) || defined(SC95F7x1x) || defined(SC95FWxx) || defined(SC95F8x3x) || defined(SC95F7x3x)
typedef enum
{
  ADC_CHANNEL_0 = (uint8_t)0x00,     //选择AIN0做AD输入口
  ADC_CHANNEL_1 = (uint8_t)0x01,     //选择AIN1做AD输入口
  ADC_CHANNEL_2 = (uint8_t)0x02,     //选择AIN2做AD输入口
  ADC_CHANNEL_3 = (uint8_t)0x03,     //选择AIN3做AD输入口
  ADC_CHANNEL_4 = (uint8_t)0x04,     //选择AIN4做AD输入口
  ADC_CHANNEL_5 = (uint8_t)0x05,     //选择AIN5做AD输入口
  ADC_CHANNEL_6 = (uint8_t)0x06,     //选择AIN6做AD输入口
  ADC_CHANNEL_7 = (uint8_t)0x07,     //选择AIN7做AD输入口
  ADC_CHANNEL_8 = (uint8_t)0x08,     //选择AIN8做AD输入口
  ADC_CHANNEL_9 = (uint8_t)0x09,     //选择AIN9做AD输入口
  ADC_CHANNEL_10 = (uint8_t)0x0A,    //选择AIN10做AD输入口
  ADC_CHANNEL_11 = (uint8_t)0x0B,    //选择AIN11做AD输入口
  ADC_CHANNEL_12 = (uint8_t)0x0C,    //选择AIN12做AD输入口
  ADC_CHANNEL_13 = (uint8_t)0x0D,    //选择AIN13做AD输入口
  ADC_CHANNEL_14 = (uint8_t)0x0E,    //选择AIN14做AD输入口
  ADC_CHANNEL_15 = (uint8_t)0x0F,    //选择AIN15做AD输入口
  ADC_CHANNEL_VDD_D4 = (uint8_t)0x1f //选择内部1/4VDD做AD输入口
} ADC_Channel_TypeDef;

//ADC采样通道使能枚举
typedef enum
{
  ADC_EAIN_0 = (uint16_t)0x0001,  //选择AIN0
  ADC_EAIN_1 = (uint16_t)0x0002,  //选择AIN1
  ADC_EAIN_2 = (uint16_t)0x0004,  //选择AIN2
  ADC_EAIN_3 = (uint16_t)0x0008,  //选择AIN3
  ADC_EAIN_4 = (uint16_t)0x0010,  //选择AIN4
  ADC_EAIN_5 = (uint16_t)0x0020,  //选择AIN5
  ADC_EAIN_6 = (uint16_t)0x0040,  //选择AIN6
  ADC_EAIN_7 = (uint16_t)0x0080,  //选择AIN7
  ADC_EAIN_8 = (uint16_t)0x0100,  //选择AIN8
  ADC_EAIN_9 = (uint16_t)0x0200,  //选择AIN9
  ADC_EAIN_10 = (uint16_t)0x0400, //选择AIN10
  ADC_EAIN_11 = (uint16_t)0x0800, //选择AIN11
  ADC_EAIN_12 = (uint16_t)0x1000, //选择AIN12
  ADC_EAIN_13 = (uint16_t)0x2000, //选择AIN13
  ADC_EAIN_14 = (uint16_t)0x4000, //选择AIN14
  ADC_EAIN_15 = (uint16_t)0x8000  //选择AIN15
} ADC_EAIN_TypeDef;
#endif

#if defined(SC95F8x2x) || defined(SC95F7x2x)
//ADC采样通道选择
typedef enum
{
  ADC_CHANNEL_0 = (uint8_t)0x00,     //选择AIN0做AD输入口
  ADC_CHANNEL_1 = (uint8_t)0x01,     //选择AIN1做AD输入口
  ADC_CHANNEL_2 = (uint8_t)0x02,     //选择AIN2做AD输入口
  ADC_CHANNEL_3 = (uint8_t)0x03,     //选择AIN3做AD输入口
  ADC_CHANNEL_4 = (uint8_t)0x04,     //选择AIN4做AD输入口
  ADC_CHANNEL_5 = (uint8_t)0x05,     //选择AIN5做AD输入口
  ADC_CHANNEL_6 = (uint8_t)0x06,     //选择AIN6做AD输入口
  ADC_CHANNEL_7 = (uint8_t)0x07,     //选择AIN7做AD输入口
  ADC_CHANNEL_8 = (uint8_t)0x08,     //选择AIN8做AD输入口
  ADC_CHANNEL_9 = (uint8_t)0x09,     //选择AIN9做AD输入口
  ADC_CHANNEL_VDD_D4 = (uint8_t)0x1f //选择内部1/4VDD做AD输入口
} ADC_Channel_TypeDef;

//ADC采样通道使能枚举
typedef enum
{
  ADC_EAIN_0 = (uint16_t)0x0001, //选择AIN0
  ADC_EAIN_1 = (uint16_t)0x0002, //选择AIN1
  ADC_EAIN_2 = (uint16_t)0x0004, //选择AIN2
  ADC_EAIN_3 = (uint16_t)0x0008, //选择AIN3
  ADC_EAIN_4 = (uint16_t)0x0010, //选择AIN4
  ADC_EAIN_5 = (uint16_t)0x0020, //选择AIN5
  ADC_EAIN_6 = (uint16_t)0x0040, //选择AIN6
  ADC_EAIN_7 = (uint16_t)0x0080, //选择AIN7
  ADC_EAIN_8 = (uint16_t)0x0100, //选择AIN8
  ADC_EAIN_9 = (uint16_t)0x0200, //选择AIN9
} ADC_EAIN_TypeDef;
#endif

void ADC_DeInit(void);
void ADC_Init(ADC_PresSel_TypeDef ADC_PrescalerSelection, ADC_Cycle_TypeDef ADC_Cycle);
void ADC_ChannelConfig(ADC_Channel_TypeDef ADC_Channel, FunctionalState NewState);
void ADC_EAINConfig(uint16_t ADC_EAIN_Select, FunctionalState NewState);
void ADC_Cmd(FunctionalState NewState);
void ADC_StartConversion(void);
uint16_t ADC_GetConversionValue(void);
void ADC_ITConfig(FunctionalState NewState, PriorityStatus Priority);
FlagStatus ADC_GetFlagStatus(void);
void ADC_ClearFlag(void);

#endif

/******************* (C) COPYRIGHT 2021 SinOne Microelectronics *****END OF FILE****/