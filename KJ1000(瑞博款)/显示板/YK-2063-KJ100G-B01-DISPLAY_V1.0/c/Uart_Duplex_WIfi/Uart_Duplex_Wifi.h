/*--------------------------------------------------------------------
模块名称：全双工UART驱动模块
模块作者：陈首敏
编制时间：2020-4-30
功能描述：收发同时进行，发送buf一边发一边清零；接收多少个清除多少个
版本号: 		 V2.0
---------------------------------------------------------------------*/

#ifndef _UART_DUPLEX_WIFI_H
#define _UART_DUPLEX_WIFI_H

#ifdef	EXTR
#undef	EXTR                                                                
#endif
                                                                                                    
#ifdef	_UART_DUPLEX_WIFI_C
#define EXTR                                                                                                
#else                                                                                                       
#define EXTR	extern                                                                                      
#endif 

#define C_WIFI_RXD_BUF_LENGTH		    60
#define C_WIFI_TXD_BUF_LENGTH		    60

#define C_WIFI_UART_SEND_DELAY_TIME	    3
#define C_WIFI_UART_DECODE_DELAY_TIME	2

// --- 配置：指向 US0CON3 寄存器 ---
#define REG_READD_WIFI (US0CON3)
#define REG_SENDD_WIFI (US0CON3)

#define WIFI_EN_RXD_INT()		{;}
#define WIFI_DIS_RXD_INT()		{;}
#define WIFI_EN_TXD_INT()		{;}
#define WIFI_DIS_TXD_INT()		{;}

extern unsigned char LONG_REG g_u8WifiUartRXDBuf[C_WIFI_RXD_BUF_LENGTH];
extern unsigned char LONG_REG g_u8WifiUartTXDBuf[C_WIFI_TXD_BUF_LENGTH];

EXTR void F_WifiSysUartInit(void);
EXTR void F_WifiUartDuplexProc(void);

#endif