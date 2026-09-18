/*********************************************************
文 件 名 : GPIO_Output.H
文件说明 : GPIO驱动输出头文件
编写作者 : thm
修订作者 : 
编写时间 : 2021-12-27
版 本 号 : V1.0
==========================================================*/
#ifndef _GPIO_OUTPUT_H_
#define _GPIO_OUTPUT_H_ 

/*************************配置开始***********************/
//负载个数（0-16）
#define     C_LOAD_NUMBERS           0  

//GPIO管脚配置
#define     _GPIO_PIN0               P10
#define     _GPIO_PIN1               P11
#define     _GPIO_PIN2               P12
#define     _GPIO_PIN3               P13
#define     _GPIO_PIN4               P14
#define     _GPIO_PIN5               P15
#define     _GPIO_PIN6               P16
#define     _GPIO_PIN7               P17
#define     _GPIO_PIN8               P20
#define     _GPIO_PIN9               P21
#define     _GPIO_PIN10              P22
#define     _GPIO_PIN11              P23
#define     _GPIO_PIN12              P24
#define     _GPIO_PIN13              P25
#define     _GPIO_PIN14              P26
#define     _GPIO_PIN15              P27

//g_u16OutputBuff
#define     Buffer_B0_               0
#define     Buffer_B1_               1
#define     Buffer_B2_               2
#define     Buffer_B3_               3
#define     Buffer_B4_               4
#define     Buffer_B5_               5
#define     Buffer_B6_               6
#define     Buffer_B7_               7
#define     Buffer_B8_               8
#define     Buffer_B9_               9
#define     Buffer_B10_              10
#define     Buffer_B11_              11
#define     Buffer_B12_              12
#define     Buffer_B13_              13
#define     Buffer_B14_              14
#define     Buffer_B15_              15

/*************************配置结束***********************/

extern unsigned int xdata g_u16OutputBuff;

extern void F_GPIO_Output(void);

#endif    