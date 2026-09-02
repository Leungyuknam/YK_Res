#ifndef _UART_DUPLEX_H_
#define _UART_DUPLEX_H_

#define RXD1_BUF_LENGTH     11
#define TXD1_BUF_LENGTH     23

#define UART1_SEND_DELAY_TIME      5
#define UART1_DECODE_DELAY_TIME    2
#define UART1_HEARTBEAT_TIME       6 // 100ms ·¢ËÍÖÜÆÚ

#define REG1_READD (US1CON3)
#define REG1_SENDD (US1CON3)

#define EN_RXD_INT()        {;}
#define DIS_RXD_INT()       {;}
#define EN_TXD_INT()        {;}
#define DIS_TXD_INT()       {;}

extern unsigned char xdata Uart1RXDBuf[RXD1_BUF_LENGTH];
extern unsigned char xdata Uart1TXDBuf[TXD1_BUF_LENGTH];

extern void F_SysUart1Init(void);
extern void F_Uart1DuplexProc(void);

#endif