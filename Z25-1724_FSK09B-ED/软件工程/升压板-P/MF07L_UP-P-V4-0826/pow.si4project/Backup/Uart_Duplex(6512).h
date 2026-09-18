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

#define RXD1_BUF_LENGTH		12
#define	SAME_BUF_LENGTH		(RXD1_BUF_LENGTH - 1) //去掉校验码比较

#define TXD1_BUF_LENGTH		6

#define UART1_SEND_DELAY_TIME	    6
#define UART1_DECODE_DELAY_TIME	    2

#define REG1_READD (SBUF)
#define REG1_SENDD (SBUF)
//#define REG1_READD (SSDAT)
//#define REG1_SENDD (SSDAT)

#define EN_RXD_INT()		{;}
#define DIS_RXD_INT()		{;}
#define EN_TXD_INT()		{;}
#define DIS_TXD_INT()		{;}

extern unsigned char xdata Uart1RXDBuf[RXD1_BUF_LENGTH];
extern unsigned char xdata Uart1TXDBuf[TXD1_BUF_LENGTH];

EXTR void F_SYSUartInit(void);
EXTR void F_UartDuplexProc(void);
	
//0824
EXTR void F_SYSUart2Init(void);
EXTR void F_Uart2DuplexProc(void);
	
#define RXD2_BUF_LENGTH		6//12
#define	SAME2_BUF_LENGTH		(RXD2_BUF_LENGTH - 1) //去掉校验码比较

#define TXD2_BUF_LENGTH		12//6

#define UART2_SEND_DELAY_TIME	    6
#define UART2_DECODE_DELAY_TIME	    2	
	
	
#define REG2_READD (S2BUF)
#define REG2_SENDD (S2BUF)	
	
#define EN_RXD2_INT()		{;}
#define DIS_RXD2_INT()		{;}
#define EN_TXD2_INT()		{;}
#define DIS_TXD2_INT()		{;}	
	
extern unsigned char xdata Uart2RXDBuf[RXD2_BUF_LENGTH];
extern unsigned char xdata Uart2TXDBuf[TXD2_BUF_LENGTH];	
	

#endif

