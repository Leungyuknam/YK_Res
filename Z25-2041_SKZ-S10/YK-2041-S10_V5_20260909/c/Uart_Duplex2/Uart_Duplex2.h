/*--------------------------------------------------------------------
模块名称：全双工UART驱动模块
模块作者：陈首敏
编制时间：2020-4-30
功能描述：收发同时进行，发送buf一边发一边清零；接收多少个清除多少个
版本号: 		 V2.0
---------------------------------------------------------------------*/
#ifndef UART_DUPLEX_H
#define UART_DUPLEX_H

#ifdef	EXTR
#undef	EXTR                                                                        
#endif
						                                                                                         
#ifdef	UART_DUPLEX_C
#define EXTR                                                                                                         
#else						                                                                                         
#define EXTR	extern                                                                                               
#endif 

#define RXD_BUF_LENGTH		20
#define TXD_BUF_LENGTH		9

#define UART_SEND_DELAY_TIME	3
#define UART_DECODE_DELAY_TIME	2

#define REG_READD (SBUF1)
#define REG_SENDD (SBUF1)

#define EN_RXD_INT()		{;}
#define DIS_RXD_INT()		{;}
#define EN_TXD_INT()		{;}
#define DIS_TXD_INT()		{;}

extern unsigned char LONG_REG UartRXDBuf[RXD_BUF_LENGTH];
extern unsigned char LONG_REG UartTXDBuf[TXD_BUF_LENGTH];

EXTR void F_SysUartInit(void);
EXTR void F_UartDuplexProc(void);

#endif

