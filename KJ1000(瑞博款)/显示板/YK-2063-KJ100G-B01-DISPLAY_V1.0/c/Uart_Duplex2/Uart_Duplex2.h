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
#define TXD_BUF_LENGTH		6

#define UART_SEND_DELAY_TIME	3
#define UART_DECODE_DELAY_TIME	2

// --- 修改配置：指向 Uart2 的收发寄存器 ---
#define REG_READD (US2CON3)
#define REG_SENDD (US2CON3)

#define EN_RXD_INT()		{;}
#define DIS_RXD_INT()		{;}
#define EN_TXD_INT()		{;}
#define DIS_TXD_INT()		{;}

extern unsigned char LONG_REG UartRXDBuf[RXD_BUF_LENGTH];
extern unsigned char LONG_REG UartTXDBuf[TXD_BUF_LENGTH];

EXTR void F_DustUartInit(void);
EXTR void F_UartDuplexProc(void);

#endif