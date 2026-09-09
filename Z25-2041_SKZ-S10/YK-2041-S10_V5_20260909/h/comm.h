#ifndef __COMM_H__
#define __COMM_H__

#include <intrins.h>
#include <STRING.H>
#include <absacc.h>
#include "SH79F9463A.h"

/* 类型定义 ------------------------------------------------------------------ */
typedef unsigned char		uint8_t;
typedef signed char 		int8_t;
typedef unsigned short 		uint16_t;
typedef signed short 		int16_t;
typedef unsigned long 		uint32_t;
typedef signed long 		int32_t;


/* 标志定义 ------------------------------------------------------------------- */
typedef enum
{
    DISABLE = 0,
    ENABLE = !DISABLE,
}FunctionState;

typedef enum
{
    OFF = 0,
    ON = !OFF,
}CmdState;


/* 通用宏定义 ------------------------------------------------------------------ */

#define BIN(b7,b6,b5,b4,b3,b2,b1,b0)    (b0 + b1*2 + b2*4 + b3*8 + b4*16 + b5*32 + b6*64 + b7*128)

#define	BANK1					(INSCON	|= 0x40)
#define	BANK0					(INSCON	&= 0xBF)
#define __SET_BANK_1()			BANK1
#define __SET_BANK_0()			BANK0
#define SET_BANK1()				BANK1
#define SET_BANK0()				BANK0

#define NOP() 					_nop_()
#define CLEAR_WDT()				RSTSTAT=0x00
#define disable_interrupt()     EA=0
#define enable_interrupt()      EA=1
#define entry_interrupt()       \
		do{						\
			_push_(INSCON);		\
			INSCON = 0;			\
			_push_(FLASHCON);	\
			FLASHCON = 0;		\
		}while(0)
#define	exit_interrupt()		\
		do{						\
			INSCON = 0;			\
			_pop_(FLASHCON);	\
			_pop_(INSCON);		\
		}while(0)

#define	HIBYTE(v1)				((unsigned char)((v1)>>8))
#define	LOBYTE(v1)				((unsigned char)((v1)&0xFF))
#define SETB(r,n)				(r |= (1<<n))
#define CLRB(r,n)				(r &= ~(1<<n))
#define	GETB(r,n)				(r &( 1<<n)) 
#define SWAP(r,n)				(r ^= (1<<n))
/* 中断向量 ------------------------------------------------------------------ */
#define     INT_VECTOR_EXTI0            0                   // External interrupt 0
#define     INT_VECTOR_TK               1                   // Touch Key
#define     INT_VECTOR_EXTI1            2                   // External interrupt 1
#define     INT_VECTOR_TIM5             3                   // Timer5
    #define INT_VECTOR_T5               INT_VECTOR_TIM5     // Timer5
    #define INT_VECTOR_TMR5             INT_VECTOR_TIM5     // Timer5
    #define INT_VECTOR_TIMER5           INT_VECTOR_TIM5     // Timer5
#define 	INT_VECTOR_UART0            4			        // Uart0
#define     INT_VECTOR_TIM3             5                   // Timer3
    #define INT_VECTOR_T3               INT_VECTOR_TIM3     // Timer3
    #define INT_VECTOR_TMR3             INT_VECTOR_TIM3     // Timer3
    #define INT_VECTOR_TIMER3           INT_VECTOR_TIM3     // Timer3
#define     INT_VECTOR_ADC              6                   // ADC
#define     INT_VECTOR_TWI              7                   // TWI
#define     INT_VECTOR_LED              8                   // LED
#define     INT_VECTOR_TIM4             9                   // Timer4
    #define INT_VECTOR_T4               INT_VECTOR_TIM4     // Timer4
    #define INT_VECTOR_TMR4             INT_VECTOR_TIM4     // Timer4
    #define INT_VECTOR_TIMER4           INT_VECTOR_TIM4     // Timer4
#define     INT_VECTOR_EXTI4            10                  // External interrupt 4
#define     INT_VECTOR_IENC             INT_VECTOR_EXTI4    // IENC    
#define     INT_VECTOR_EXTI3            11                  // External interrupt 3
#define     INT_VECTOR_EXTI2            12                  // External interrupt 2
#define 	INT_VECTOR_LPD 				13			        // LPD
#define 	INT_VECTOR_UART1 			15			        // UART1
#define 	INT_VECTOR_UART2 			16			        // UART2
#define 	INT_VECTOR_CRC	 			17			        // CRC
#define 	INT_VECTOR_PWM0	 			18			        // PWM0
#define 	INT_VECTOR_SPI	 			19			        // SPI
#define 	INT_VECTOR_PCA0 			20			        // PCA0
#define 	INT_VECTOR_PWM1	 			21			        // PWM1
#endif /* __COMM_H__ */