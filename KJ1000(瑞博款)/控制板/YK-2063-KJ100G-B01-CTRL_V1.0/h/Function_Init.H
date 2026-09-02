#ifndef _Function_Init_H_
#define _Function_Init_H_

#include "H/SC95R503_C.H"
#include "intrins.H"
#define uchar unsigned char
#define uint unsigned int
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
extern void IO_Init(void);
extern void BTM_Test(void);
extern void EXTI_Test(void);
extern void Timer_Test(void);
extern void PWM_Test(void);
extern void Uart_Test(void);
extern void USCI0_Test(void);
extern void USCI1_Test(void);
extern void USCI2_Test(void);
extern void ADC_Test(void);
extern void IAP_Test(void);
extern void CRC_Test(void);
extern void Multiplier_Divider_Test(void);
extern void Delay(unsigned long time);

/*****************************************
ËµÃ÷£ºÑ¡Ôñ²âÊÔIC
*****************************************/
#define SC95R503P32  0
#define SC95R503X28  1
#define SC95R503M28  2
#define SC95R503X20  3
#define SC95R503M20  4
#define SC95R503M16  5

#define IC_MODEL  SC95R503P32

#endif