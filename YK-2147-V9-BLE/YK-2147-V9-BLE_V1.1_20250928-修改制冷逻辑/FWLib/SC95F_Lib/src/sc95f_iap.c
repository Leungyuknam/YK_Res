//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: sc95f_iap.c
//	作者		:
//	模块功能	: IAP固件库函数C文件
//  最后更正日期: 2021/7/23
// 	版本		: V1.10001
//  说明        :该文件仅适用于SC95F系列芯片
//*************************************************************
#include "COMMON\AllInclude.h"
#include "sc95f_iap.h"

/**************************************************
*函数名称:void IAP_DeInit(void)
*函数功能:IAP相关寄存器复位至缺省值
*入口参数:void
*出口参数:void
**************************************************/
void IAP_DeInit(void)
{
  IAPKEY = 0X00;
  IAPADL = 0X00;
  IAPADH = 0X00;
  IAPADE = 0X00;
  IAPDAT = 0X00;
  IAPCTL = 0X00;
}

#if defined(SC95F8x3x) || defined(SC95F7x3x)

/**************************************************
*函数名称:void PhysicsAddrToMCUAddr(unsigned long *IAP_IapAddr)
*函数功能:逻辑地址转MCU地址 
*入口参数:
unsigned long*:IAP_IapAddr:地址
*出口参数:void 
**************************************************/
void PhysicsAddrToMCUAddr(unsigned long *IAP_IapAddr)
{
  if (*IAP_IapAddr >= 0x10000) //如果超过64K则需要操作Bank寄存器指向目标地址
  {
    if (*IAP_IapAddr < 0x18000)
    {
      ROMBNK = (ROMBNK & 0xCF) | 0x20;
      *IAP_IapAddr = (*IAP_IapAddr - 0x8000);
    }
    else if (*IAP_IapAddr < 0x20000)
    {

      ROMBNK = (ROMBNK & 0xCF) | 0x30;
      *IAP_IapAddr = (*IAP_IapAddr - 0x10000);
    }
  }
  else
  {
    ROMBNK = (ROMBNK & 0xCF) | 0x10;
  }
}

/**************************************************
*函数名称:void IAP_ProgramByte(uint16_t Address, uint8_t Data, IAP_MemType_TypeDef IAP_MemType, uint8_t WriteTimeLimit)
*函数功能:IAP写入一个字节
*入口参数:
uint32_t:Address:IAP操作地址
uint8_t:Data:写入的数据
IAP_MemType_TypeDef:IAP_MemType:IAP操作对象（ROM、IFB）
uint8_t:WriteTimeLimitIAP操作时限（值需大于等于0x40）
*出口参数:void
**************************************************/
void IAP_ProgramByte(uint32_t Address, uint8_t Data, IAP_MemType_TypeDef IAP_MemType,
                     uint8_t WriteTimeLimit)
{
  BitStatus tmpbit;
  uint8_t tmpROMBNK;
  uint8_t tmpIAPADE;

  tmpbit = (BitStatus)EA;
  tmpIAPADE = IAPADE;
  tmpROMBNK = ROMBNK;
  if (tmpbit != RESET)
  {
    disableInterrupts();
  }

  PhysicsAddrToMCUAddr(&Address);
  IAPADE = IAP_MemType;
  IAPDAT = Data;
  IAPADH = (unsigned char)(Address >> 8);
  IAPADL = (unsigned char)(Address);

  IAPKEY = WriteTimeLimit;
  IAPCTL = 0x10;
  IAPCTL |= 0x02;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  
  #warning "IAP需16个NOP"
  
  ROMBNK = tmpROMBNK;
  IAPADE = tmpIAPADE;
  EA = tmpbit;
}

/**************************************************
*函数名称:uint8_t IAP_ReadByte(uint16_t Address, IAP_MemType_TypeDef IAP_MemType)
*函数功能:IAP读一个字节
*入口参数:
uint32_t:Address:IAP操作地址
IAP_MemType_TypeDef:IAP_MemType:IAP操作对象（ROM、IFB）
*出口参数:
uint8_t:读到的字节数据
**************************************************/
uint8_t IAP_ReadByte(uint32_t Address, IAP_MemType_TypeDef IAP_MemType)
{
  uint8_t tmpbyte;
  BitStatus tmpbit;
  uint8_t tmpROMBNK;
  uint8_t tmpIAPADE;

  tmpbit = (BitStatus)EA;
  tmpIAPADE = IAPADE;
  tmpROMBNK = ROMBNK;

  if (tmpbit != RESET)
  {
    disableInterrupts();
  }

  PhysicsAddrToMCUAddr(&Address);
  IAPADE = IAP_MemType;
  tmpbyte = *((uint8_t code *)Address);

  ROMBNK = tmpROMBNK;
  IAPADE = tmpIAPADE;

  if (tmpbit != RESET)
  {
    enableInterrupts();
  }

  return tmpbyte;
}

/**************************************************
*函数名称:void IAP_SectorErase(IAP_MemType_TypeDef IAP_MemType, uint16_t IAP_SectorEraseAddress)
*函数功能:IAP扇区擦除
*入口参数:
IAP_MemType_TypeDef:IAP_MemType:IAP操作对象（ROM、IFB）
uint32_t:IAP_SectorEraseAddress:IAP扇区擦除目标地址
uint8_t:WriteTimeLimit:IAP操作时限(值需大于等于0x40)
*出口参数:void
**************************************************/
void IAP_SectorErase(IAP_MemType_TypeDef IAP_MemType, uint32_t IAP_SectorEraseAddress,
                     uint8_t WriteTimeLimit)
{
  uint8_t tmpbit;
  uint8_t tmpROMBNK;
  uint8_t tmpIAPADE;

  tmpbit = (BitStatus)EA;
  tmpIAPADE = IAPADE;
  tmpROMBNK = ROMBNK;

  if (tmpbit != RESET)
  {
    disableInterrupts();
  }

  PhysicsAddrToMCUAddr(&IAP_SectorEraseAddress);

  IAPADE = IAP_MemType;
  IAPADH = (uint8_t)(IAP_SectorEraseAddress >> 8); //擦除IAP目标地址高位值
  IAPADL = (uint8_t)IAP_SectorEraseAddress;        //擦除IAP目标地址低位值
  IAPKEY = WriteTimeLimit;
  IAPCTL = 0x20;
  IAPCTL |= 0x02;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();

  tmpIAPADE = IAPADE;
  tmpROMBNK = ROMBNK;
  if (tmpbit != RESET)
  {
    enableInterrupts();
  }
}
#else
/**************************************************
*函数名称:void IAP_ProgramByte(uint16_t Address, uint8_t Data, IAP_MemType_TypeDef IAP_MemType, uint8_t WriteTimeLimit)
*函数功能:IAP写入一个字节
*入口参数:
uint32_t:Address:IAP操作地址
uint8_t:Data:写入的数据
IAP_MemType_TypeDef:IAP_MemType:IAP操作对象（ROM、IFB）
uint8_t:WriteTimeLimit:IAP操作时限(值需大于等于0x40)
*出口参数:void
**************************************************/
void IAP_ProgramByte(uint32_t Address, uint8_t Data, IAP_MemType_TypeDef IAP_MemType,
                     uint8_t WriteTimeLimit)
{
  BitStatus tmpbit;
  tmpbit = (BitStatus)EA;

  if (tmpbit != RESET)
  {
    disableInterrupts();
  }

  IAPADE = IAP_MemType;
  IAPDAT = Data;
  IAPADH = (uint8_t)(Address >> 8);
  IAPADL = (uint8_t)Address;
  IAPKEY = WriteTimeLimit;
  IAPCTL = 0x10;
  IAPCTL |= 0x02;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  IAPADE = IAP_MEMTYPE_ROM;

  if (tmpbit != RESET)
  {
    enableInterrupts();
  }
}

/**************************************************
*函数名称:uint8_t IAP_ReadByte(uint16_t Address, IAP_MemType_TypeDef IAP_MemType)
*函数功能:IAP读一个字节
*入口参数:
uint32_t:Address:IAP操作地址
IAP_MemType_TypeDef:IAP_MemType:IAP操作对象（ROM、IFB）
*出口参数:
uint8_t:读到的字节数据
**************************************************/
uint8_t IAP_ReadByte(uint32_t Address, IAP_MemType_TypeDef IAP_MemType)
{
  uint8_t tmpbyte;
  BitStatus tmpbit;
  tmpbit = (BitStatus)EA;

  if (tmpbit != RESET)
  {
    disableInterrupts();
  }

  IAPADE = IAP_MemType;
  tmpbyte = *((uint8_t code *)Address);
  IAPADE = IAP_MEMTYPE_ROM;

  if (tmpbit != RESET)
  {
    enableInterrupts();
  }

  return tmpbyte;
}

/**************************************************
*函数名称:void IAP_SectorErase(IAP_MemType_TypeDef IAP_MemType, uint16_t IAP_SectorEraseAddress)
*函数功能:IAP扇区擦除
*入口参数:
IAP_MemType_TypeDef:IAP_MemType:IAP操作对象（ROM、IFB）
uint32_t:IAP_SectorEraseAddress:IAP扇区擦除目标地址
uint8_t:WriteTimeLimit:IAP操作时限(值需大于等于0x40)
*出口参数:void
**************************************************/
void IAP_SectorErase(IAP_MemType_TypeDef IAP_MemType, uint32_t IAP_SectorEraseAddress,
                     uint8_t WriteTimeLimit)
{
  IAPADE = IAP_MemType;
  IAPADH = (uint8_t)(IAP_SectorEraseAddress >> 8); //擦除IAP目标地址高位值
  IAPADL = (uint8_t)IAP_SectorEraseAddress;        //擦除IAP目标地址低位值
  IAPKEY = WriteTimeLimit;
  IAPCTL = 0x20;
  IAPCTL |= 0x02;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  IAPADE = IAP_MEMTYPE_ROM;
}

#endif

/**************************************************
*函数名称:void IAP_BootLoaderControl(IAP_BTLDType_TypeDef IAP_BTLDType)
*函数功能:MCU复位后从那个区域启动
*入口参数:
IAP_BTLDType_TypeDef:IAP_BTLDType:选择启动区域
*出口参数:void
**************************************************/
void IAP_BootLoaderControl(IAP_BTLDType_TypeDef IAP_BTLDType)
{
  IAPCTL = (IAPCTL & 0xFB) | IAP_BTLDType;
}

/******************* (C) COPYRIGHT 2020 SinOne Microelectronics *****END OF FILE****/