//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: sc95f_uart0.c
//	作者		:
//	模块功能	: UART0固件库函数C文件
//  最后更正日期: 2020/8/13
// 	版本		: V1.0
//  说明        :该文件仅适用于SC95Fxx1x、SC95Fxx2x系列芯片
//*************************************************************

#include "sc95f_uart0.h"

/**************************************************
*函数名称:void UART0_DeInit(void)
*函数功能:UART0相关寄存器复位至缺省值
*入口参数:void
*出口参数:void
**************************************************/
void UART0_DeInit(void)
{
  SCON = 0X00;
  SBUF = 0X00;
  PCON &= 0X7F;
  IE &= 0XEF;
  IP &= 0XEF;
}

/**************************************************
*函数名称:void UART0_Init(uint32_t Uart0Fsys, uint32_t BaudRate, UART0_Mode_Typedef Mode,
						   UART0_Clock_Typedef ClockMode, UART0_RX_Typedef RxMode)
*函数功能:UART0初始化配置函数
*入口参数:
uint32_t:Uart0Fsys:系统时钟频率
uint32_t:BaudRate:波特率
UART0_Mode_Typedef:Mode:UART0工作模式
UART0_Clock_Typedef:ClockMode:波特率时钟源（TIMER1/TIMER2）
UART0_RX_Typedef:RxMode:接收允许选择
*出口参数:void
**************************************************/
void UART0_Init(uint32_t Uart0Fsys, uint32_t BaudRate, UART0_Mode_Typedef Mode,
                UART0_Clock_Typedef ClockMode, UART0_RX_Typedef RxMode)
{
  SCON = SCON & 0X2F | Mode | RxMode; //设置UART工作模式,设置接收允许位

  if (Mode == UART0_Mode_8B)
  {
    if (BaudRate == UART0_BaudRate_FsysDIV12)
    {
      SCON &= 0XDF;
    }
    else if (BaudRate == UART0_BaudRate_FsysDIV4)
    {
      SCON |= 0X20;
    }
  }
  else
  {
#if defined(SC95F8x2x) || defined(SC95F7x2x)
    TXINX = 0X02;
#endif
    TXCON = (TXCON & 0xCF) | ClockMode; //设置波特率时钟源

    if (ClockMode == UART0_CLOCK_TIMER1)
    {
      TH1 = (Uart0Fsys / BaudRate) / 256;
      TL1 = (Uart0Fsys / BaudRate) % 256;
      TR1 = 0;
    }
    else if (ClockMode == UART0_CLOCK_TIMER2)
    {
      RCAPXH = (Uart0Fsys / BaudRate) / 256;
      RCAPXL = (Uart0Fsys / BaudRate) % 256;
#if defined(SC95F8x2x) || defined(SC95F7x2x)
      TRX = 1;
#endif
    }
  }
}

/*****************************************************
*函数名称:void UART0_SendData8(uint8_t Data)
*函数功能:UART0发送8位数据
*入口参数:
uint8_t:Data:发送的数据
*出口参数:void
*****************************************************/
void UART0_SendData8(uint8_t Data)
{
  SBUF = Data;
}

/**************************************************
*函数名称:uint8_t UART0_ReceiveData8(void)
*函数功能:获得SBUF中的值
*入口参数:void
*出口参数:
uint8_t:接收的数据
**************************************************/
uint8_t UART0_ReceiveData8(void)
{
  return SBUF;
}

/*****************************************************
*函数名称:void UART0_SendData9(uint16_t Data)
*函数功能:UART0发送9位数据
*入口参数:
uint16_t:Data:发送的数据
*出口参数:void
*****************************************************/
void UART0_SendData9(uint16_t Data)
{
  uint8_t Data_9Bit;
  Data_9Bit = (Data >> 8);

  if (Data_9Bit)
  {
    SCON |= 0X08;
  }
  else
  {
    SCON &= 0XF7;
  }

  SBUF = (uint8_t)Data;
}

/**************************************************
*函数名称:uint16_t UART0_ReceiveData9(void)
*函数功能:获得SBUF中的值及第九位的值
*入口参数:void
*出口参数:
uint16_t:SBUF中的值及第九位的值
**************************************************/
uint16_t UART0_ReceiveData9(void)
{
  uint16_t Data9;
  Data9 = SBUF + ((uint16_t)(SCON & 0X04) << 6);
  SCON &= 0XFB;
  return Data9;
}

/*****************************************************
*函数名称:void UART0_ITConfig(FunctionalState NewState, PriorityStatus Priority)
*函数功能:UART0中断初始化
*入口参数:
FunctionalState:NewState:中断使能/关闭选择
PriorityStatus:Priority:中断优先级选择
*出口参数:void
*****************************************************/
void UART0_ITConfig(FunctionalState NewState, PriorityStatus Priority)
{
  if (NewState == DISABLE)
  {
    EUART = 0;
  }
  else
  {
    EUART = 1;
  }

  //设置中断优先级
  if (Priority == LOW)
  {
    IPUART = 0;
  }
  else
  {
    IPUART = 1;
  }
}

/*****************************************************
*函数名称:FlagStatus UART0_GetFlagStatus(UART0_Flag_Typedef UART0_Flag)
*函数功能:获得UART0中断标志状态
*入口参数:
UART0_Flag_Typedef:UART0_Flag:中断标志位选择
*出口参数:
FlagStatus:UART0中断标志状态
*****************************************************/
FlagStatus UART0_GetFlagStatus(UART0_Flag_Typedef UART0_Flag)
{
  return (bool)(SCON & UART0_Flag);
}

/*****************************************************
*函数名称:void UART0_ClearFlag(UART0_Flag_Typedef UART0_Flag)
*函数功能:清除UART0中断标志状态
*入口参数:
UART0_Flag_Typedef:UART0_Flag:中断标志位选择
*出口参数:void
*****************************************************/
void UART0_ClearFlag(UART0_Flag_Typedef UART0_Flag)
{
  SCON &= (~UART0_Flag);
}

/******************* (C) COPYRIGHT 2020 SinOne Microelectronics *****END OF FILE****/