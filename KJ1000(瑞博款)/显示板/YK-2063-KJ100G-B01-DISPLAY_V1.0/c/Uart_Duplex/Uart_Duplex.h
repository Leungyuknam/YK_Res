#ifndef _UART_DUPLEX_H_
#define _UART_DUPLEX_H_

// 语音模块固定为 8 字节收发
#define RXD1_VOICE_BUF_LENGTH    8
#define TXD1_VOICE_BUF_LENGTH    8

#define UART1_VOICE_SEND_DELAY_TIME    5
#define UART1_VOICE_DECODE_DELAY_TIME  2

#define REG1_READD_VOICE (SSDAT)
#define REG1_SENDD_VOICE (SSDAT)

// 中断开关宏 (如果你原来用的是别的，请替换大括号内的内容)
#define EN_RXD1_VOICE_INT()       {;}
#define DIS_RXD1_VOICE_INT()      {;}
#define EN_TXD1_VOICE_INT()       {;}
#define DIS_TXD1_VOICE_INT()      {;}

extern unsigned char xdata Uart1RXDBuf[RXD1_VOICE_BUF_LENGTH];
extern unsigned char xdata Uart1TXDBuf[TXD1_VOICE_BUF_LENGTH];

extern bit g_bVoiceTxReady;
extern unsigned char g_u8PendingVoiceBuf[];
extern bit g_bHasNewVoiceData;

	
extern void F_SysUart1Init(void);
extern void F_Uart1DuplexProc(void);
extern void F_SendToVoiceModule(unsigned char *pData); // 供外部调用

#endif