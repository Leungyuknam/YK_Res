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

#define RXD1_BUF_LENGTH		11
#define TXD1_BUF_LENGTH		5

#define UART1_SEND_DELAY_TIME	    6
#define UART1_DECODE_DELAY_TIME	    2

//#define REG1_READD (SBUF)
//#define REG1_SENDD (SBUF)
#define REG1_READD (SSDAT)
#define REG1_SENDD (SSDAT)

#define EN_RXD_INT()		{;}
#define DIS_RXD_INT()		{;}
#define EN_TXD_INT()		{;}
#define DIS_TXD_INT()		{;}

extern unsigned char xdata Uart1RXDBuf[RXD1_BUF_LENGTH];
extern unsigned char xdata Uart1TXDBuf[TXD1_BUF_LENGTH];

EXTR void F_SYSUartInit(void);
EXTR void F_UartDuplexProc(void);

#endif

