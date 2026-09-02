/*--------------------------------------------------------------------
模块名称：全双工UART驱动模块
模块作者：陈首敏
编制时间：2020-4-30
功能描述：收发同时进行，发送buf一边发一边清零；接收多少个清除多少个
版本号: 		 V2.0
---------------------------------------------------------------------*/

#define UART_DUPLEX_C
//粉尘传感器串口
#include "h\Allinclude.h"

unsigned int SHORT_REG RecTimeOut = 0;
unsigned char SHORT_REG TxdBusyTim = 0;
unsigned char SHORT_REG TxdNum = TXD_BUF_LENGTH;
unsigned char SHORT_REG SendLength = 0;
unsigned char SHORT_REG RecNum = RXD_BUF_LENGTH;
unsigned char LONG_REG UartRXDBuf[RXD_BUF_LENGTH] = {0};
unsigned char LONG_REG UartTXDBuf[TXD_BUF_LENGTH]= {0};

void RXDInit(void)
{
	while(RecNum)
	{
		RecNum --;
		UartRXDBuf[RecNum] = 0;
	}
	RecNum = 0; 
	RecTimeOut = 60000;
	EN_RXD_INT();
}

void RxdDecode(U8 length)
{
	F_StiveRecProc(length);
}

void IRQ_REC(void)
{
	uchar temp;
	temp = REG_READD;
	if(RecTimeOut)
	{
		if(RecNum < RXD_BUF_LENGTH)
		{
			RecTimeOut = UART_DECODE_DELAY_TIME;
			UartRXDBuf[RecNum] = temp;
			RecNum ++;
		}
	}
}

void UartRxdProc(void)
{
	if(F_GetTimeTick(C_TIME_10ms))
	{
		if(RecTimeOut)
		{
			RecTimeOut --;
			if(RecTimeOut == 0)
			{
				if(RecNum)
				{
					RxdDecode(RecNum);
				}
				RXDInit();
			}
		}
	}
}

void InitUartSend(void)
{
	TxdNum = 1;
	EN_TXD_INT();
	REG_SENDD = UartTXDBuf[0];
	UartTXDBuf[0] = 0;
}

void IRQ_TXD(void)
{
	if(TxdNum < SendLength)
	{
		REG_SENDD = UartTXDBuf[TxdNum];
		UartTXDBuf[TxdNum] = 0;
		TxdNum ++;
		TxdBusyTim = UART_SEND_DELAY_TIME;
	}
	else
	{
		DIS_TXD_INT();
	}
}

void UartTxdProc(void)
{
	if(TxdBusyTim == 0)
	{
		SendLength = F_StiveSendCode();
		if(SendLength)
		{
			TxdBusyTim = UART_SEND_DELAY_TIME;
			InitUartSend();
		}
	}
	else
	{
		if(F_GetTimeTick(C_TIME_10ms))TxdBusyTim--;
	}
}

void F_UartDuplexProc(void)
{
	UartRxdProc();
	UartTxdProc();
}

void Uart_Init(unsigned int Freq, unsigned long int baud)  
{       
	// --- 修改配置：直接配置 US2CON 相关寄存器 ---
    TMCON |= 0xC0;                           //串行接口USCI2选择Uart通信
    US2CON0 = 0x50;                          //设置通信方式为模式一，允许接收
    US2CON1 = Freq * 1000000 / baud;         //波特率低位控制
    US2CON2 = (Freq * 1000000 / baud) >> 8;  //波特率高位控制
    IE2 |= 0x02;                             //开启Uart中断 (IE2 bit1)
}

// --- 修改配置：中断号16，使用 US2CON0 寄存器 ---
void UART2_IRQHandler(void) interrupt 16
{
	if(US2CON0&0x02)
	{	
		US2CON0 &= 0xFE;	
		IRQ_TXD();
	}	

	if(US2CON0&0x01) 
	{	
		US2CON0 &= 0xFD;
		IRQ_REC();
	}
}

void F_DustUartInit(void)
{
	while(TxdNum)
	{
		TxdNum --;
		UartTXDBuf[TxdNum] = 0;
	}
	RXDInit();
	Uart_Init(XTAL_FREQUENCY,9600);
}