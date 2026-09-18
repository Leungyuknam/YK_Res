
#ifndef _UART_APP1_H_
#define _UART_APP1_H_


extern u8  g_u8RecDelay;
extern bit g_bCompCmnError;


//±ØÓÃº¯Êý
extern void F_Uart1App_Handle(void);
extern u8 F_CheckSumCalc(u8 *puchMsg, u8 usDataLen);

#endif