//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: sc95f_iap.h
//	作者		:
//	模块功能	: IAP固件库函数头文件
//	局部函数列表:
//  最后更正日期: 2021/6/11
// 	版本		: V1.10000
//  说明        :该文件仅适用于SC95F系列芯片
//*************************************************************

#ifndef _sc95f_IAP_H_
#define _sc95f_IAP_H_

#include "sc95f.h"
#include "intrins.h"

//IAP操作区域选择枚举
typedef enum
{
  IAP_MEMTYPE_ROM = (uint8_t)0x00, //IAP操作区域为ROM
  IAP_MEMTYPE_IFB = (uint8_t)0x01, //IAP操作区域为IFB
} IAP_MemType_TypeDef;

///MCU复位启动区域选择枚举
typedef enum
{
  IAP_BTLDType_APPROM = (uint8_t)0x00, //MCU复位后从APROM复位
  IAP_BTLDType_LDROM = (uint8_t)0x01,  //MCU复位后从LDROM复位
} IAP_BTLDType_TypeDef;

void IAP_DeInit(void);
void IAP_ProgramByte(uint32_t Address, uint8_t Data, IAP_MemType_TypeDef IAP_MemType,
                     uint8_t WriteTimeLimit);
uint8_t IAP_ReadByte(uint32_t Address, IAP_MemType_TypeDef IAP_MemType);
void IAP_SectorErase(IAP_MemType_TypeDef IAP_MemType, uint32_t IAP_SectorEraseAddress,
                     uint8_t WriteTimeLimit);
void IAP_BootLoaderControl(IAP_BTLDType_TypeDef IAP_BTLDType);

#endif

/******************* (C) COPYRIGHT 2020 SinOne Microelectronics *****END OF FILE****/