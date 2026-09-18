//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: sc95f_wdt.c
//	作者		:
//	模块功能	: WDT固件库函数C文件
//  最后更正日期: 2020/8/13
// 	版本		: V1.0
//  说明        :该文件仅适用于SC95Fxx1x、SC95Fxx2x系列芯片
//*************************************************************
#include "COMMON\AllInclude.h"
#include "sc95f_wdt.h"

/**************************************************
*函数名称:void WDT_DeInit(void)
*函数功能:WDT相关寄存器复位至缺省值
*入口参数:void
*出口参数:void
**************************************************/
void WDT_DeInit(void)
{
    WDTCON = 0X00;
}

/**************************************************
*函数名称:void WDT_Init(WDT_OverflowTime_TypeDef OverflowTime)
*函数功能:WDT初始化配置函数
*入口参数:
WDT_OverflowTime_TypeDef:OverflowTime:WDT溢出时间选择
*出口参数:void
**************************************************/
void WDT_Init(WDT_OverflowTime_TypeDef OverflowTime)
{
    WDTCON = (WDTCON & 0XF8) | OverflowTime;
}

/*****************************************************
*函数名称:void WDT_SetReload(void)
*函数功能:WDT喂狗
*入口参数:void
*出口参数:void
*****************************************************/
void WDT_SetReload(void)
{
    WDTCON |= 0X10;
}
/******************* (C) COPYRIGHT 2020 SinOne Microelectronics *****END OF FILE****/