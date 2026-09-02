//************************************************************
//  Copyright (c) 深圳市赛元微电子股份有限公司
//	文件名称	: CRC_Option_EW.h
//	作者		:
//	模块功能	: CRC固件库函数头文件
//	局部函数列表:
//  最后更正日期: 2024/1/12
// 	版本		: V1.0
//  说明    :该文件仅适用于SC95R503系列芯片
//*************************************************************
#ifndef _CRC_OPTION_EW_H_
#define	_CRC_OPTION_EW_H_


/**************************************************
*函数名称：nsigned long CRC_CodeAllOption(void)
*函数功能：计算Code区域的CRC检验值
*入口参数：void
*出口参数：计算出的CRC值
**************************************************/
unsigned long CRC_CodeAllOption(void);

/**************************************************
*函数名称：void CRC_ResetDROption(void)
*函数功能：CRCDR寄存器复位
*入口参数：void  
*出口参数：void
**************************************************/
void CRC_ResetDROption(void);

/**************************************************
*函数名称：void CRC_CalcOption(unsigned char Data)
*函数功能：对指定的CRC数据进行计算
*入口参数：Data：要计算的Byte
*出口参数：void
**************************************************/
void CRC_CalcOption(unsigned char Data);

/**************************************************
*函数名称：unsigned long CRC_GetOption(void)
*函数功能：获取CRC校验值
*入口参数：void
*出口参数：计算出的CRC值
**************************************************/
unsigned long CRC_GetOption(void);

#endif