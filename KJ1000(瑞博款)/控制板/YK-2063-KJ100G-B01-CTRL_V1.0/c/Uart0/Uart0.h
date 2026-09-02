/*--------------------------------------------------------------------
模块名称：全双工UART驱动模块
模块作者：陈首敏
编制时间：2020-4-30
功能描述：收发同时进行，发送buf一边发一边清零；接收多少个清除多少个
版本号: 		 V2.0
---------------------------------------------------------------------*/


#ifndef _UART0_H
#define _UART0_H

#ifdef	EXTR
#undef	EXTR                                                                        
#endif
						                                                                                         
#ifdef	_UART0_C
#define EXTR                                                                                                         
#else						                                                                                         
#define EXTR	extern                                                                                               
#endif 

#define C_UART_RXD_BUF_LENGTH		    8
#define C_UART_TXD_BUF_LENGTH		    8

#define C_UART_SEND_DELAY_TIME	      6				//发送间隔时间
#define C_UART_DECODE_DELAY_TIME	    2

#define REG_READD_UART (SBUF)
#define REG_SENDD_UART (SBUF)
//#define REG_READD_UART (US0CON3)
//#define REG_SENDD_UART (US0CON3)
//#define REG_READD_UART (US2CON3)
//#define REG_SENDD_UART (US2CON3)

#define UART_EN_RXD_INT()		{;}
#define UART_DIS_RXD_INT()		{;}
#define UART_EN_TXD_INT()		{;}
#define UART_DIS_TXD_INT()		{;}

extern unsigned char LONG_REG Uart0RXDBuf[C_UART_RXD_BUF_LENGTH];
extern unsigned char LONG_REG Uart0TXDBuf[C_UART_TXD_BUF_LENGTH];

extern void F_SysUartInit(void);
extern void F_Uart0Proc(void);

#endif

