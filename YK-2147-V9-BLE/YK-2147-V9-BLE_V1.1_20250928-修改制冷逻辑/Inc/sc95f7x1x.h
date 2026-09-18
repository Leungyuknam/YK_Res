//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: sc95f7x1x.h
//	作者		: 
//	模块功能	: 赛元SC854x系列系统头文件
//	局部函数列表:
//  最后更正日期: 2019/7/10
// 	版本		: V1.0 
//  说明        ：  
//*************************************************************

#ifndef _sc95f7x1x_H
#define _sc95f7x1x_H

#include "sc95f7x1x_C.H"

#define enableInterrupts()    EA=1	 /** 开启总中断	 **/
#define disableInterrupts()   EA=0	 /** 关闭总中断	 **/
 
#define     __I     volatile const   /*!< defines 'read only' permissions     */
#define     __O     volatile         /*!< defines 'write only' permissions    */
#define     __IO    volatile         /*!< defines 'read / write' permissions  */

/*!< Signed integer types  */
typedef   signed char     int8_t;
typedef   signed short    int16_t;
typedef   signed long     int32_t;

/*!< Unsigned integer types  */
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned long     uint32_t;

typedef int32_t  s32;
typedef int16_t  s16;
typedef int8_t   s8;

typedef uint32_t  u32;
typedef uint16_t  u16;
typedef uint8_t   u8;


typedef enum {FALSE = 0, TRUE = !FALSE} bool;

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus, BitStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

typedef enum {LOW = 0, HIGH = !LOW} PriorityStatus;

#endif