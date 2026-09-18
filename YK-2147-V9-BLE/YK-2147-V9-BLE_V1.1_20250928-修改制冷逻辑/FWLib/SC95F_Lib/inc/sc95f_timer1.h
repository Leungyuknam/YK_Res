//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: sc95f_timer1.h
//	作者		:
//	模块功能	: TIMER1固件库函数头文件
//  最后更正日期: 2020/8/14
// 	版本		: V1.0
//  说明        :该文件仅适用于SC95Fxx1x、SC95Fxx2x系列芯片
//*************************************************************

#ifndef _sc95f_TIMER1_H_
#define _sc95f_TIMER1_H_

#include "sc95f.h"

typedef enum
{
  TIM1_PRESSEL_FSYS_D12 = ((uint8_t)0x00), //TIMER1计数源来自系统时钟12分频
  TIM1_PRESSEL_FSYS_D1 = ((uint8_t)0x01)   //TIMER1计数源来自系统时钟
} TIM1_PresSel_TypeDef;

typedef enum
{
  TIM1_MODE_TIMER = ((uint8_t)0x01),  //TIMER1做定时器
  TIM1_MODE_COUNTER = ((uint8_t)0x02) //TIMER1做计数器
} TIM1_CountMode_TypeDef;

typedef enum
{
  TIM1_WORK_MODE0 = ((uint8_t)0x00), //TIMER1选择工作模式0
  TIM1_WORK_MODE1 = ((uint8_t)0x01), //TIMER1选择工作模式1
  TIM1_WORK_MODE2 = ((uint8_t)0x02), //TIMER1选择工作模式2
} TIM1_WorkMode_TypeDef;

void TIM1_DeInit(void);
void TIM1_TimeBaseInit(TIM1_PresSel_TypeDef TIM1_PrescalerSelection,
                       TIM1_CountMode_TypeDef TIM1_CountMode);
void TIM1_WorkMode0Config(uint16_t TIM1_SetCounter);
void TIM1_WorkMode1Config(uint16_t TIM1_SetCounter);
void TIM1_WorkMode2Config(uint8_t TIM1_SetCounter);
void TIM1_Mode0SetReloadCounter(uint16_t TIM1_SetCounter);
void TIM1_Mode1SetReloadCounter(uint16_t TIM1_SetCounter);
void TIM1_Cmd(FunctionalState NewState);
void TIM1_ITConfig(FunctionalState NewState, PriorityStatus Priority);
FlagStatus TIM1_GetFlagStatus(void);
void TIM1_ClearFlag(void);
void TIM1_WorkModeConfig(TIM1_WorkMode_TypeDef TIM1_WorkMode, uint16_t TIM1_SetCounter);

#endif

/******************* (C) COPYRIGHT 2018 SinOne Microelectronics *****END OF FILE****/