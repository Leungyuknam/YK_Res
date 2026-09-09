/*--------------------------------------------------------------------
模块名称：全双工UART驱动模块
模块作者：陈首敏
编制时间：2020-4-30
功能描述：收发同时进行，发送buf一边发一边清零；接收多少个清除多少个
版本号: 		 V2.0
---------------------------------------------------------------------*/

#define UART_DUPLEX_C

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
	F_CH2ORecProc(length);
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
    SET_BANK_1();
	REG_SENDD = UartTXDBuf[0];
    SET_BANK_0();
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
//		SendLength = CallArrayFunc();
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

void Uart_Init(uint Freq,unsigned long int baud)   
{	
    SET_BANK_1();
    
    UART1CR = B00110010;//TX映射到P15 RX映射到P14
    
    PCON1 = 0x00;
	SCON1 = 0x50; 	// 配置Uart工作在模式1，UART0接收器允许
	/*配置波特率参数，波特率9600*/
	/* 计算公式：(int)X=FSY/(16*波特率) ;  SBRT=32768-X  ;   SFINE=（FSY/波特率）-16*X   FSY=12M*/
	SBRTH1 = ( (32768-(Freq*1000000/16/baud) & 0x7f00) >> 8) | 0x80; //高位为波特率使能位
	SBRTL1 = ( 32768-(Freq*1000000/16/baud) )& 0x00ff;
	SFINE1 = (Freq*1000000/baud) - (16*(Freq*1000000/16/baud));
    
    SET_BANK_0();
    
	IEN2 |= 0x01;  
}
void UART1_IRQHandler(void)  interrupt 15 
{
	//-----------------------------------------------------------
	// 压栈保护
    _push_(INSCON);
	INSCON = 0;
    _push_(FLASHCON);
    FLASHCON = 0;
	
	//-----------------------------------------------------------
    SET_BANK_1();
    
	if(SCON1 & 0x02)
	{	
		SCON1 &= ~0x02;
        IRQ_TXD();
        
	}
	if(SCON1 & 0x01) 
	{	
		SCON1 &= ~0x01;
        IRQ_REC();    
	}
    
    SET_BANK_0();
    //-----------------------------------------------------------
    
	// 出栈
    INSCON = 0;
	_pop_(FLASHCON);
    _pop_(INSCON);
}
void F_SysUartInit(void)
{
	while(TxdNum)
	{
		TxdNum --;
		UartTXDBuf[TxdNum] = 0;
	}
	RXDInit();
	Uart_Init(XTAL_FREQUENCY,9600);
}

