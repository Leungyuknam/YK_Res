//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: sc95f_btm.h
//	作者		:
//	模块功能	: BTM固件库函数头文件
//  最后更正日期: 2020/8/14
// 	版本		: V1.0
//  说明        :该文件仅适用于SC95F系列芯片
//*************************************************************
#ifndef _sc95f_BTM_H_
#define _sc95f_BTM_H_

#include "sc95f.h"

//BTM中断时间选择枚举
typedef enum
{
  BTM_TIMEBASE_15_625MS = (uint8_t)0x00, //BTM每15.625MS产生一次中断
  BTM_TIMEBASE_31_25MS = (uint8_t)0x01,  //BTM每31.25MS产生一次中断
  BTM_TIMEBASE_62_5MS = (uint8_t)0x02,   //BTM每62.5MS产生一次中断
  BTM_TIMEBASE_125MS = (uint8_t)0x03,    //BTM每125MS产生一次中断
  BTM_TIMEBASE_250MS = (uint8_t)0x04,    //BTM每0.25S产生一次中断
  BTM_TIMEBASE_500MS = (uint8_t)0x05,    //BTM每0.5S产生一次中断
  BTM_TIMEBASE_1S = (uint8_t)0x06,       //BTM每1S产生一次中断
  BTM_TIMEBASE_2S = (uint8_t)0x07,       //BTM每2S产生一次中断
  BTM_TIMEBASE_4S = (uint8_t)0x08,       //BTM每4S产生一次中断
  BTM_TIMEBASE_8S = (uint8_t)0x09,       //BTM每8S产生一次中断
  BTM_TIMEBASE_16S = (uint8_t)0x0a,      //BTM每16S产生一次中断
  BTM_TIMEBASE_32S = (uint8_t)0x0b       //BTM每32S产生一次中断
} BTM_Timebase_TypeDef;

void BTM_DeInit(void);
void BTM_Init(BTM_Timebase_TypeDef BTM_Timebase);
void BTM_Cmd(FunctionalState NewState);
void BTM_ITConfig(FunctionalState NewState, PriorityStatus Priority);
FlagStatus BTM_GetFlagStatus(void);
void BTM_ClearFlag(void);

#endif

/******************* (C) COPYRIGHT 2018 SinOne Microelectronics *****END OF FILE****/