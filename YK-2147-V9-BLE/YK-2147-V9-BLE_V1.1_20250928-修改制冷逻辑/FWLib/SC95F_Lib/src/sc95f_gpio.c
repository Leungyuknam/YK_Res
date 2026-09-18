//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称: sc95f_gpio.c
//	作者:
//	模块功能: GPIO固件库函数C文件
//  最后更正日期: 2021/05/19
// 	版本:	V1.10003
//  说明:	该文件仅适用于SC95F系列芯片
//*************************************************************
#include "COMMON\AllInclude.h"
#include "sc95f_gpio.h"

/**************************************************
*函数名称:void GPIO_DeInit(void)
*函数功能:GPIO相关寄存器复位至缺省值
*入口参数:void
*出口参数:void
**************************************************/
void GPIO_DeInit(void)
{
#if defined(SC95F8x1x) || defined(SC95F7x1x) || defined(SC92FWxx) || defined(SC95F8x3x) || defined(SC95F7x3x)
  P0CON = 0x00;
  P1CON = 0x00;
  P2CON = 0x00;
  P3CON = 0x00;
  P4CON = 0x00;
  P5CON = 0x00;
  P0PH = 0x00;
  P1PH = 0x00;
  P2PH = 0x00;
  P3PH = 0x00;
  P4PH = 0x00;
  P5PH = 0x00;
  P0 = P1 = P2 = P3 = P4 = P5 = 0;
#endif
#if defined(SC95F8x2x) || defined(SC95F7x2x)
  P0CON = 0x00;
  P1CON = 0x00;
  P2CON = 0x00;
  P5CON = 0x00;
  P0PH = 0x00;
  P1PH = 0x00;
  P2PH = 0x00;
  P5PH = 0x00;
  P0 = P1 = P2 = P5 = 0;
#endif
}

/**************************************************
*函数名称:void GPIO_Init(GPIO_TypeDef GPIOx, GPIO_Pin_TypeDef PortPins, GPIO_Mode_TypeDef GPIO_Mode)
*函数功能:GPIO口模式配置初始化
*入口参数:
GPIO_TypeDef:GPIOx:选择操作的GPIO口
GPIO_Pin_TypeDef:PortPins:选择GPIO管脚Pxy（uint8_t作为入参，方便进行位与运算）
GPIO_Mode_TypeDef:GPIO_Mode:选择GPIO口模式（高阻输入、上拉输入、推挽输出）
*出口参数:void
**************************************************/
void GPIO_Init(GPIO_TypeDef GPIOx, uint8_t PortPins, GPIO_Mode_TypeDef GPIO_Mode)
{
  if (GPIOx == GPIO0)
  {
    if (GPIO_Mode == GPIO_MODE_IN_HI)
    {
      P0CON &= ~PortPins;
      P0PH &= ~PortPins;
    }

    if (GPIO_Mode == GPIO_MODE_IN_PU)
    {
      P0CON &= ~PortPins;
      P0PH |= PortPins;
    }

    if (GPIO_Mode == GPIO_MODE_OUT_PP)
    {
      P0CON |= PortPins;
    }
  }
  else if (GPIOx == GPIO1)
  {
    if (GPIO_Mode == GPIO_MODE_IN_HI)
    {
      P1CON &= ~PortPins;
      P1PH &= ~PortPins;
    }

    if (GPIO_Mode == GPIO_MODE_IN_PU)
    {
      P1CON &= ~PortPins;
      P1PH |= PortPins;
    }

    if (GPIO_Mode == GPIO_MODE_OUT_PP)
    {
      P1CON |= PortPins;
    }
  }
  else if (GPIOx == GPIO2)
  {
    if (GPIO_Mode == GPIO_MODE_IN_HI)
    {
      P2CON &= ~PortPins;
      P2PH &= ~PortPins;
    }

    if (GPIO_Mode == GPIO_MODE_IN_PU)
    {
      P2CON &= ~PortPins;
      P2PH |= PortPins;
    }

    if (GPIO_Mode == GPIO_MODE_OUT_PP)
    {
      P2CON |= PortPins;
    }
  }

#if defined(SC95F8x1x) || defined(SC95F7x1x) || defined(SC95FWxx) || defined(SC95F8x3x) || defined(SC95F7x3x)
  else if (GPIOx == GPIO3)
  {
    if (GPIO_Mode == GPIO_MODE_IN_HI)
    {
      P3CON &= ~PortPins;
      P3PH &= ~PortPins;
    }

    if (GPIO_Mode == GPIO_MODE_IN_PU)
    {
      P3CON &= ~PortPins;
      P3PH |= PortPins;
    }

    if (GPIO_Mode == GPIO_MODE_OUT_PP)
    {
      P3CON |= PortPins;
    }
  }
  else if (GPIOx == GPIO4)
  {
    if (GPIO_Mode == GPIO_MODE_IN_HI)
    {
      P4CON &= ~PortPins;
      P4PH &= ~PortPins;
    }

    if (GPIO_Mode == GPIO_MODE_IN_PU)
    {
      P4CON &= ~PortPins;
      P4PH |= PortPins;
    }

    if (GPIO_Mode == GPIO_MODE_OUT_PP)
    {
      P4CON |= PortPins;
    }
  }

#endif
  else if (GPIOx == GPIO5)
  {
    if (GPIO_Mode == GPIO_MODE_IN_HI)
    {
      P5CON &= ~PortPins;
      P5PH &= ~PortPins;
    }

    if (GPIO_Mode == GPIO_MODE_IN_PU)
    {
      P5CON &= ~PortPins;
      P5PH |= PortPins;
    }

    if (GPIO_Mode == GPIO_MODE_OUT_PP)
    {
      P5CON |= PortPins;
    }
  }
}

/**************************************************
*函数名称:void GPIO_Write(GPIO_TypeDef GPIOx, uint8_t PortVal)
*函数功能:GPIO口赋值
*入口参数:
GPIO_TypeDef:GPIOx:选择操作的GPIO口
uint8_t:PortVal:GPIO口的值
*出口参数:void
**************************************************/
void GPIO_Write(GPIO_TypeDef GPIOx, uint8_t PortVal)
{
  if (GPIOx == GPIO0)
  {
    P0 = PortVal;
  }

  if (GPIOx == GPIO1)
  {
    P1 = PortVal;
  }

  if (GPIOx == GPIO2)
  {
    P2 = PortVal;
  }

#if defined(SC95F8x1x) || defined(SC95F7x1x) || defined(SC95FWxx) || defined(SC95F8x3x) || defined(SC95F7x3x)

  if (GPIOx == GPIO3)
  {
    P3 = PortVal;
  }

  if (GPIOx == GPIO4)
  {
    P4 = PortVal;
  }

#endif

  if (GPIOx == GPIO5)
  {
    P5 = PortVal;
  }
}

/**************************************************
*函数名称:void GPIO_WriteHigh(GPIO_TypeDef GPIOx, GPIO_Pin_TypeDef PortPins)
*函数功能:GPIO口管脚Pxy置位
*入口参数:
GPIO_TypeDef:GPIOx:选择操作的GPIO口
GPIO_Pin_TypeDef:PortPins:选择GPIO口管脚Pxy（uint8_t作为入参，方便进行位与运算）
*出口参数:void
**************************************************/
void GPIO_WriteHigh(GPIO_TypeDef GPIOx, uint8_t PortPins)
{
  if (GPIOx == GPIO0)
  {
    P0 |= PortPins;
  }

  if (GPIOx == GPIO1)
  {
    P1 |= PortPins;
  }

  if (GPIOx == GPIO2)
  {
    P2 |= PortPins;
  }

#if defined(SC95F8x1x) || defined(SC95F7x1x) || defined(SC95FWxx) || defined(SC95F8x3x) || defined(SC95F7x3x)

  if (GPIOx == GPIO3)
  {
    P3 |= PortPins;
  }

  if (GPIOx == GPIO4)
  {
    P4 |= PortPins;
  }

#endif

  if (GPIOx == GPIO5)
  {
    P5 |= PortPins;
  }
}

/**************************************************
*函数名称:void GPIO_WriteLow(GPIO_TypeDef GPIOx, GPIO_Pin_TypeDef PortPins)
*函数功能:GPIO口管脚Pxy复位
*入口参数:
GPIO_TypeDef:GPIOx:选择操作的GPIO口
GPIO_Pin_TypeDef:PortPins:选择GPIO口管脚Pxy（uint8_t作为入参，方便进行位与运算）
*出口参数:void
**************************************************/
void GPIO_WriteLow(GPIO_TypeDef GPIOx, uint8_t PortPins)
{
  if (GPIOx == GPIO0)
  {
    P0 &= ~PortPins;
  }

  if (GPIOx == GPIO1)
  {
    P1 &= ~PortPins;
  }

  if (GPIOx == GPIO2)
  {
    P2 &= ~PortPins;
  }

#if defined(SC95F8x1x) || defined(SC95F7x1x) || defined(SC95FWxx) || defined(SC95F8x3x) || defined(SC95F7x3x)

  if (GPIOx == GPIO3)
  {
    P3 &= ~PortPins;
  }

  if (GPIOx == GPIO4)
  {
    P4 &= ~PortPins;
  }

#endif

  if (GPIOx == GPIO5)
  {
    P5 &= ~PortPins;
  }
}

/**************************************************
*函数名称:uint8_t GPIO_ReadPort(GPIO_TypeDef GPIOx)
*函数功能:读GPIO口Px的值
*入口参数:
GPIO_TypeDef:GPIOx:选择操作的GPIO口
*出口参数:
uint8_t:返回Px的值
**************************************************/
uint8_t GPIO_ReadPort(GPIO_TypeDef GPIOx)
{
  if (GPIOx == GPIO0)
  {
    return P0;
  }
  else if (GPIOx == GPIO1)
  {
    return P1;
  }
  else if (GPIOx == GPIO2)
  {
    return P2;
  }

#if defined(SC95F8x1x) || defined(SC95F7x1x) || defined(SC95FWxx) || defined(SC95F8x3x) || defined(SC95F7x3x)
  else if (GPIOx == GPIO3)
  {
    return P3;
  }
  else if (GPIOx == GPIO4)
  {
    return P4;
  }

#endif
  else
  {
    return P5;
  }
}

/**************************************************
*函数名称:BitStatus GPIO_ReadPin(GPIO_TypeDef GPIOx, uint8_t PortPins)
*函数功能:读GPIO口管脚Pxy的值
*入口参数:
GPIO_TypeDef:GPIOx:选择操作的GPIO口
GPIO_Pin_TypeDef:PortPins:选择GPIO口管脚Pxy（uint8_t作为入参，方便进行位与运算）
*出口参数:
BitStatus:返回Pxy的值
**************************************************/
BitStatus GPIO_ReadPin(GPIO_TypeDef GPIOx, uint8_t PortPins)
{
  if (GPIOx == GPIO0)
  {
    return (P0 & PortPins);
  }
  else if (GPIOx == GPIO1)
  {
    return (P1 & PortPins);
  }
  else if (GPIOx == GPIO2)
  {
    return (P2 & PortPins);
  }

#if defined(SC95F8x1x) || defined(SC95F7x1x) || defined(SC95FWxx) || defined(SC95F8x3x) || defined(SC95F7x3x)
  else if (GPIOx == GPIO3)
  {
    return (P3 & PortPins);
  }
  else if (GPIOx == GPIO4)
  {
    return (P4 & PortPins);
  }

#endif
  else if (GPIOx == GPIO5)
  {
    return (P5 & PortPins);
  }

  return 0;
}

/**************************************************
*函数名称:void GPIO_IOH_Config(GPIO_TypeDef GPIOx, uint8_t PortPins,GPIO_IOH_Grade_TypeDef GPIO_IOH_Grade)
*函数功能:设置GPIO口管脚IOH输出能力
*入口参数:
GPIO_TypeDef:GPIOx:选择操作的GPIO口
GPIO_Pin_TypeDef:PortPins:选择GPIO口管脚Pxy
GPIO_IOH_Grade_TypeDef:GPIO_IOH_Grade:选择IOH的输出等级
*出口参数:void
**************************************************/
void GPIO_IOH_Config(GPIO_TypeDef GPIOx, GPIO_Pin_TypeDef PortPins, GPIO_IOH_Grade_TypeDef GPIO_IOH_Grade)
{
  switch (GPIOx)
  {
  case GPIO0:
    if (PortPins == GPIO_PIN_LNIB)
    {
      IOHCON0 &= 0xFC;
      IOHCON0 |= GPIO_IOH_Grade;
    }
    else if (PortPins == GPIO_PIN_HNIB)
    {
      IOHCON0 &= 0xF3;
      IOHCON0 |= GPIO_IOH_Grade << 2;
    }
    break;
  case GPIO1:
    if (PortPins == GPIO_PIN_LNIB)
    {
      IOHCON0 &= 0xCF;
      IOHCON0 |= GPIO_IOH_Grade << 4;
    }
    else if (PortPins == GPIO_PIN_HNIB)
    {
      IOHCON0 &= 0x3F;
      IOHCON0 |= GPIO_IOH_Grade << 6;
    }
    break;
  case GPIO2:
    if (PortPins == GPIO_PIN_LNIB)
    {
      IOHCON1 &= 0xFC;
      IOHCON1 |= GPIO_IOH_Grade;
    }
    else if (PortPins == GPIO_PIN_HNIB)
    {
      IOHCON1 &= 0xF3;
      IOHCON1 |= GPIO_IOH_Grade << 2;
    }
    break;
#if defined(SC95F8x1x) || defined(SC95F7x1x) || defined(SC95FWxx) || defined(SC95F8x3x) || defined(SC95F7x3x)
  case GPIO3:
    if (PortPins == GPIO_PIN_LNIB)
    {
      IOHCON1 &= 0xCF;
      IOHCON1 |= GPIO_IOH_Grade << 4;
    }
    break;
#elif defined(SC95F8x2x) || defined(SC95F7x2x)
  case GPIO5:
    if (PortPins == GPIO_PIN_LNIB)
    {
      IOHCON1 &= 0xCF;
      IOHCON1 |= GPIO_IOH_Grade << 4;
    }
    break;
#endif
  default:
    break;
  }
}

/******************* (C) COPYRIGHT 2020 SinOne Microelectronics *****END OF FILE****/