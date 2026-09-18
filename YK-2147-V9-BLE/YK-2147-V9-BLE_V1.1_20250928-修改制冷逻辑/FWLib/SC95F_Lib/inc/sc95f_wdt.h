//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: sc95f_wdt.h
//	作者		:
//	模块功能	: WDT固件库函数头文件
//  最后更正日期: 2020/8/14
// 	版本		: V1.0
//  说明        :该文件仅适用于SC95F系列芯片
//*************************************************************

#ifndef _sc95f_WDT_H_
#define _sc95f_WDT_H_

#include "sc95f.h"

typedef enum
{
  WDT_OverflowTime_500MS = (uint8_t)0x00,   //看门狗溢出时间为500MS
  WDT_OverflowTime_250MS = (uint8_t)0x01,   //看门狗溢出时间为250MS
  WDT_OverflowTime_125MS = (uint8_t)0x02,   //看门狗溢出时间为125MS
  WDT_OverflowTime_62_5MS = (uint8_t)0x03,  //看门狗溢出时间为62.5MS
  WDT_OverflowTime_31_5MS = (uint8_t)0x04,  //看门狗溢出时间为31.5MS
  WDT_OverflowTime_15_75MS = (uint8_t)0x05, //看门狗溢出时间为15.75MS
  WDT_OverflowTime_7_88MS = (uint8_t)0x06,  //看门狗溢出时间为7.88MS
  WDT_OverflowTime_3_94MS = (uint8_t)0x07   //看门狗溢出时间为3.94MS
} WDT_OverflowTime_TypeDef;

void WDT_DeInit(void);
void WDT_Init(WDT_OverflowTime_TypeDef OverflowTime);
void WDT_SetReload(void);

#endif

/******************* (C) COPYRIGHT 2020 SinOne Microelectronics *****END OF FILE****/