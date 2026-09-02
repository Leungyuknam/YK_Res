/*--------------------------------------------------------------------
模块名称：全双工UART驱动模块
模块作者：陈首敏
编制时间：2020-4-30
功能描述：收发同时进行，发送buf一边发一边清零；接收多少个清除多少个
版本号: 		 V2.0
---------------------------------------------------------------------*/

#define _UART_DUPLEX_WIFI_C

#include "h\Allinclude.h"

unsigned int SHORT_REG g_u16WifiRecTimeOut = 0;
unsigned char SHORT_REG g_u8WifiTxdBusyTim = 0;
unsigned int SHORT_REG g_u16WifiTxdNum = C_WIFI_TXD_BUF_LENGTH;
unsigned char SHORT_REG g_u8WifiSendLength = 0;
unsigned int SHORT_REG g_u16WifiRecNum = C_WIFI_RXD_BUF_LENGTH;
unsigned char LONG_REG g_u8WifiUartRXDBuf[C_WIFI_RXD_BUF_LENGTH] = {0};
unsigned char LONG_REG g_u8WifiUartTXDBuf[C_WIFI_TXD_BUF_LENGTH]= {0};

void F_WifiRXDInit(void)
{
	while(g_u16WifiRecNum)
	{
		g_u16WifiRecNum --;
		g_u8WifiUartRXDBuf[g_u16WifiRecNum] = 0;
	}
	g_u16WifiRecNum = 0; 
	g_u16WifiRecTimeOut = 60000;
	WIFI_EN_RXD_INT();
}

void F_WifiRxdDecode(void)
{
	TuyaWifiRecProc();
}

void F_Wifi_IRQ_REC(void)
{
	uchar temp;
	temp = REG_READD_WIFI;
	if(g_u16WifiRecTimeOut)
	{
		if(g_u16WifiRecNum < C_WIFI_RXD_BUF_LENGTH)
		{
			g_u16WifiRecTimeOut = C_WIFI_UART_DECODE_DELAY_TIME;
			g_u8WifiUartRXDBuf[g_u16WifiRecNum] = temp;
			g_u16WifiRecNum ++;
		}
	}
}

void F_WifiUartRxdProc(void)
{
	if(F_GetTimeTick(C_TIME_10ms))
	{
		if(g_u16WifiRecTimeOut)
		{
			g_u16WifiRecTimeOut --;
			if(g_u16WifiRecTimeOut == 0)
			{
				if(g_u16WifiRecNum)
				{
					F_WifiRxdDecode();
				}
				F_WifiRXDInit();
			}
		}
	}
}

void F_WifiInitUartSend(void)
{
	g_u16WifiTxdNum = 1;
	WIFI_EN_TXD_INT();
	REG_SENDD_WIFI = g_u8WifiUartTXDBuf[0];
	g_u8WifiUartTXDBuf[0] = 0;
}

void F_Wifi_IRQ_TXD(void)
{
	if(g_u16WifiTxdNum < g_u8WifiSendLength)
	{
		REG_SENDD_WIFI = g_u8WifiUartTXDBuf[g_u16WifiTxdNum];
		g_u8WifiUartTXDBuf[g_u16WifiTxdNum] = 0;
		g_u16WifiTxdNum ++;
		g_u8WifiTxdBusyTim = C_WIFI_UART_SEND_DELAY_TIME;
	}
	else
	{
		WIFI_DIS_TXD_INT();
	}
}

void F_WifiUartTxdProc(void)
{
	if(g_u8WifiTxdBusyTim == 0)
	{
		g_u8WifiSendLength = CallArrayFunc();
		if(g_u8WifiSendLength)
		{
			g_u8WifiTxdBusyTim = C_WIFI_UART_SEND_DELAY_TIME;
			F_WifiInitUartSend();
		}
	}
	else
	{
		if(F_GetTimeTick(C_TIME_10ms))g_u8WifiTxdBusyTim--;
	}
}

void F_WifiUartDuplexProc(void)
{
	F_WifiUartRxdProc();
	F_WifiUartTxdProc();
}

void F_WifiUart_Init(uint Freq, unsigned long int baud)  
{
    // --- 配置：USCI0 选择 Uart0 通信 ---
    OTCON |= 0x30;                           // 串行接口SSI0选择Uart0通信
    US0CON0 = 0x50;                          // 设置通信方式为模式一，允许接收
    US0CON1 = Freq * 1000000 / baud;         // 波特率低位控制
    US0CON2 = (Freq * 1000000 / baud) >> 8;  // 波特率高位控制
    IE1 |= 0x01;                             // 开启USCI0中断 (IE1 bit0)
}

// --- 配置：中断号7，使用 US0CON0 ---
void USRT0_IRQHandler(void) interrupt 7
{
	if(US0CON0&0x02)
	{	
		US0CON0 &= 0xFE;	
		F_Wifi_IRQ_TXD();
	}	

	if(US0CON0&0x01) 
	{	
		US0CON0 &= 0xFD;
		F_Wifi_IRQ_REC();
	}
}

void F_WifiSysUartInit(void)
{
	while(g_u16WifiTxdNum)
	{
		g_u16WifiTxdNum --;
		g_u8WifiUartTXDBuf[g_u16WifiTxdNum] = 0;
	}
	F_WifiRXDInit();
	F_WifiUart_Init(XTAL_FREQUENCY,9600);    // wifi	
}