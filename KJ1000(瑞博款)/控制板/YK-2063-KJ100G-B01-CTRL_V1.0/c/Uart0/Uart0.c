/*--------------------------------------------------------------------
模块名称：全双工UART驱动模块
模块作者：陈首敏
编制时间：2020-4-30
功能描述：收发同时进行，发送buf一边发一边清零；接收多少个清除多少个
版本号: 		 V2.0
---------------------------------------------------------------------*/

#define _UART0_C

#include "h\Allinclude.h"
//语音控制板串口
unsigned int SHORT_REG g_u16UartRecTimeOut = 0;
unsigned char SHORT_REG g_u8UartTxdBusyTim = 0;
unsigned char SHORT_REG g_u8UartTxdNum = C_UART_TXD_BUF_LENGTH;
unsigned char SHORT_REG g_u8UartSendLength = 0;
unsigned char SHORT_REG g_u8UartRecNum = C_UART_RXD_BUF_LENGTH;
unsigned char LONG_REG Uart0RXDBuf[C_UART_RXD_BUF_LENGTH] = {0};
unsigned char LONG_REG Uart0TXDBuf[C_UART_TXD_BUF_LENGTH]= {0};

void F_UartRXDInit(void)
{
	while(g_u8UartRecNum)
	{
		g_u8UartRecNum --;
		Uart0RXDBuf[g_u8UartRecNum] = 0;
	}
	g_u8UartRecNum = 0; 
	g_u16UartRecTimeOut = 60000;
	UART_EN_RXD_INT();
}

void F_UartRxdDecode()
{
//	DeCodeProc(length);
		F_VoiceRecProc();
}


void F_Uart0_IRQ_REC(void)
{
	uchar temp;
	temp = REG_READD_UART;
	if(g_u16UartRecTimeOut)
	{
		if(g_u8UartRecNum < C_UART_RXD_BUF_LENGTH)
		{
			g_u16UartRecTimeOut = C_UART_DECODE_DELAY_TIME;
			Uart0RXDBuf[g_u8UartRecNum] = temp;
			g_u8UartRecNum ++;
		}
	}
}

void F_Uart0RxdProc(void)
{
	if(F_GetTimeTick(C_TIME_10ms))
	{
		if(g_u16UartRecTimeOut)
		{
			g_u16UartRecTimeOut --;
			if(g_u16UartRecTimeOut == 0)
			{
				if(g_u8UartRecNum)
				{
					F_UartRxdDecode();
				}
				F_UartRXDInit();
			}
		}
	}
}

void F_InitUart0Send(void)
{
	g_u8UartTxdNum = 1;
	UART_EN_TXD_INT();
	REG_SENDD_UART = Uart0TXDBuf[0];
	Uart0TXDBuf[0] = 0;
}

void F_Uart0_IRQ_TXD(void)
{
	if(g_u8UartTxdNum < g_u8UartSendLength)
	{
		REG_SENDD_UART = Uart0TXDBuf[g_u8UartTxdNum];
		Uart0TXDBuf[g_u8UartTxdNum] = 0;
		g_u8UartTxdNum ++;
		g_u8UartTxdBusyTim = C_UART_SEND_DELAY_TIME;
	}
	else
	{
		UART_DIS_TXD_INT();
	}
}

void F_Uart0TxdProc(void)
{
	if(g_u8UartTxdBusyTim == 0)
	{
//		g_u8UartSendLength = MakeEnCode();
//		if(g_u8UartSendLength)
//		{
//			g_u8UartTxdBusyTim = C_UART_SEND_DELAY_TIME;
//			F_InitUart0Send();
//		}
		
		g_u8UartSendLength = CallArrayFuncB();
		if(g_u8UartSendLength)
		{
				if(g_u8UartSendLength > C_UART_TXD_BUF_LENGTH)
				{   g_u8UartSendLength = C_UART_TXD_BUF_LENGTH;}
				g_u8UartTxdBusyTim = C_UART_SEND_DELAY_TIME;
				F_InitUart0Send();
		}
	}
	else
	{
		if(F_GetTimeTick(C_TIME_10ms))g_u8UartTxdBusyTim--;
	}
}

void F_Uart0Proc(void)
{
	F_Uart0RxdProc();
	F_Uart0TxdProc();
}

void F_Uart0_Init(uint Freq, unsigned long int baud)  
{
	
	SCON  |= 0x50;   //设置通信方式为模式一，允许接收
	TMCON |= 0x02;
	TH1 = (Freq*1000000/baud)>>8;	  //波特率为T1的溢出时间；
	TL1 = Freq*1000000/baud;
	TR1 = 0;
	ET1 = 0;
	EUART = 1;     //开启Uart中断
	EA = 1;
    
//    OTCON |= 0x30;                           //串行接口SSI0选择Uart0通信
//    US0CON0 = 0x50;                          //设置通信方式为模式一，允许接收
//    US0CON1 = Freq * 1000000 / baud;         //波特率低位控制
//    US0CON2 = (Freq * 1000000 / baud) >> 8;  //波特率高位控制
//    IE1 |= 0x01;                             //开启SSI1中断
    
//    TMCON |= 0xC0;                           //串行接口USCI2选择Uart通信
//    US2CON0 = 0x50;                          //设置通信方式为模式一，允许接收
//    US2CON1 = Freq * 1000000 / baud;         //波特率低位控制
//    US2CON2 = (Freq * 1000000 / baud) >> 8;  //波特率高位控制
//    IE2 |= 0x02;                             //开启Uart中断
}

void UART0_IRQHandler(void)  interrupt 4 
{
	if(TI)
	{	
		TI = 0;
//		F_SendINT();
		F_Uart0_IRQ_TXD();
        
	}
	if(RI) 
	{	
		RI = 0;
//		g_bTurnOn = 1;
		F_Uart0_IRQ_REC();
//		F_ReadINT();    
	}
}

//void USRT0_IRQHandler(void) interrupt 7
//{
//	if(US0CON0&0x02)
//	{	
//		US0CON0 &= 0xFE;	
//		F_Wifi_IRQ_TXD();
//	}	

//	if(US0CON0&0x01) 
//	{	
//		US0CON0 &= 0xFD;
//		F_Uart0_IRQ_REC();
//	}
//}

//void UART2_IRQHandler(void) interrupt 16
//{
//	if(US2CON0&0x02)
//	{	
//		US2CON0 &= 0xFE;	
//		F_Wifi_IRQ_TXD();
//	}	

//	if(US2CON0&0x01) 
//	{	
//		US2CON0 &= 0xFD;
//		F_Uart0_IRQ_REC();
//	}
//}

void F_SysUartInit(void)
{
	while(g_u8UartTxdNum)
	{
		g_u8UartTxdNum --;
		Uart0TXDBuf[g_u8UartTxdNum] = 0;
	}
	F_UartRXDInit();
	F_Uart0_Init(XTAL_FREQUENCY,9600);// wifi	
//  g_bTurnOn = 1;
}

