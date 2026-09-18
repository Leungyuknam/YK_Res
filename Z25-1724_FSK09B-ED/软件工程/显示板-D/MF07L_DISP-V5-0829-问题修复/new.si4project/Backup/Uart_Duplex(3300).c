#define UART_DUPLEX_C

#include "Include\Include.h"
 
unsigned int idata g_u16RecTimeOut = 0;
unsigned char idata g_u8TxdBusyTim = 0;
unsigned char idata g_u8TxdNum = TXD1_BUF_LENGTH;
unsigned char idata g_u8SendLength = 0;
unsigned char idata g_u8RecNum = RXD1_BUF_LENGTH;
unsigned char xdata Uart1RXDBuf[RXD1_BUF_LENGTH] = {0};
unsigned char xdata Uart1TXDBuf[TXD1_BUF_LENGTH]= {0};

void F_RXDInit(void)
{
	while(g_u8RecNum)
	{
		g_u8RecNum --;
		Uart1RXDBuf[g_u8RecNum] = 0;
	}
	g_u8RecNum = 0; 
	g_u16RecTimeOut = 60000;
	EN_RXD_INT();
}

void F_RxdDecode(U8 length)
{
	DeCodeProc(length);
}

void F_IRQ_REC(void)
{
	U8 temp;
	temp = REG1_READD;
	if(g_u16RecTimeOut)
	{
		if(g_u8RecNum < RXD1_BUF_LENGTH)
		{
			g_u16RecTimeOut = UART1_DECODE_DELAY_TIME;
			Uart1RXDBuf[g_u8RecNum] = temp;
			g_u8RecNum ++;
		}
	}
}

void F_UartRxdProc(void)
{
	if(GetTimeTick(TIME_10MS)) 
	{
		if(g_u16RecTimeOut)
		{
			g_u16RecTimeOut --;
			if(g_u16RecTimeOut == 0)
			{
				if(g_u8RecNum)
				{
					F_RxdDecode(g_u8RecNum);
				}
				F_RXDInit();
			}
		}
	}
}

void F_InitUartSend(void)
{
	g_u8TxdNum = 1;
	EN_TXD_INT();
	REG1_SENDD = Uart1TXDBuf[0];
	Uart1TXDBuf[0] = 0;
}

void F_IRQ_TXD(void)
{
	if(g_u8TxdNum < g_u8SendLength)
	{
		REG1_SENDD = Uart1TXDBuf[g_u8TxdNum];
		Uart1TXDBuf[g_u8TxdNum] = 0;
		g_u8TxdNum ++;
		g_u8TxdBusyTim = UART1_SEND_DELAY_TIME;
	}
	else
	{
		DIS_TXD_INT();
	}
}

void F_UartTxdProc(void)
{
	if(g_u8TxdBusyTim == 0)
	{
		g_u8SendLength = MakeEnCode();
		if(g_u8SendLength)
		{
			g_u8TxdBusyTim = UART1_SEND_DELAY_TIME;
			F_InitUartSend();
		}
	}
	else
	{
		if(GetTimeTick(TIME_10MS)) g_u8TxdBusyTim--;
	}
}

void F_UartDuplexProc(void)
{
	F_UartRxdProc();
	F_UartTxdProc();
}

/*****************************************************
*函数名称：void Uart0_Init(uint Freq,unsigned long int baud)
*函数功能：Uart0中断初始化
*入口参数：Freq-主频，baud-波特率
*出口参数：void
*****************************************************/
void Uart0_Init(uint Freq,unsigned long int baud)    //选择Timer1作为波特率信号发生器
{
	P1CON &= 0xF3;   //TX/RX设置为输入带上拉
	P1PH  |= 0x0C;
	
	SCON  |= 0X50;   //设置通信方式为模式一，允许接收
	TMCON |= 0X02;
	TMOD  |= 0X20;
	PCON  |= 0X80;	//SMOD=1
	TH1 = (Freq*1000000/baud)>>8;	  //波特率为T1的溢出时间；
	TL1 = Freq*1000000/baud;
	TR1 = 0;
	ET1 = 0;
	EUART = 1;     //开启Uart0中断
	EA = 1;
}

/*****************************************************
*函数名称：void UartInt(void) interrupt 4
*函数功能：Uart0中断函数
*入口参数：void
*出口参数：void
*****************************************************/
void UartInt(void) interrupt 4
{
	if(TI)
	{
		TI = 0;	
		 F_IRQ_TXD();		
	}
	if(RI)
	{
		RI = 0;	
		F_IRQ_REC(); 
	}	
}

void F_SYSUartInit(void)
{
	while(g_u8TxdNum)
	{
		g_u8TxdNum --;
		Uart1TXDBuf[g_u8TxdNum] = 0;
	}
	F_RXDInit();
	Uart0_Init(12,9600);
}