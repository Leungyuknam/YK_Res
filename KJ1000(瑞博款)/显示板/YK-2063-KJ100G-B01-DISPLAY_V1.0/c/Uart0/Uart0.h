/*--------------------------------------------------------------------
模块名称：全双工UART驱动模块 (从机端)
---------------------------------------------------------------------*/
#ifndef UART0_H
#define UART0_H

#ifdef  EXTR
#undef  EXTR                                                                
#endif
                                                                                                    
#ifdef  UART_DUPLEX_C
#define EXTR                                                                                                
#else                                                                                                       
#define EXTR    extern                                                                                      
#endif 

// ==========================================
// 缓冲区大小 (适配主机协议)
// ==========================================
#define RXD0_BUF_LENGTH         23  // 从机接收：23字节 (8语音 + 12显示 + 1亮度 + 1RGB + 1校验)
#define TXD0_BUF_LENGTH         11  // 从机发送：11字节 (8语音 + 1触摸 + 1传感器 + 1校验)

#define UART0_SEND_DELAY_TIME      5   // 发送延时
#define UART0_DECODE_DELAY_TIME    2   // 解码延时

// ==========================================
// 寄存器映射 (UART0)
// ==========================================
#define REG0_READD (SBUF)
#define REG0_SENDD (SBUF)

#define EN_RXD_INT()        {;}
#define DIS_RXD_INT()       {;}
#define EN_TXD_INT()        {;}
#define DIS_TXD_INT()       {;}

extern unsigned char xdata Uart0RXDBuf[RXD0_BUF_LENGTH];
extern unsigned char xdata Uart0TXDBuf[TXD0_BUF_LENGTH];

EXTR void F_SYSUartInit(void);
EXTR void F_UartDuplexProc(void);

#endif