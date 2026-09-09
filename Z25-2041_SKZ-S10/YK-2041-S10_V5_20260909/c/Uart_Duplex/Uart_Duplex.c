#include "h\Allinclude.h"
 
unsigned int g_u16RecTimeOut = 0;
unsigned char g_u8TxdBusyTim = 0;
unsigned char g_u8TxdNum = TXD1_BUF_LENGTH;
unsigned char g_u8SendLength = 0;
unsigned char g_u8RecNum = RXD1_BUF_LENGTH;
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
	if(F_GetTimeTick(C_TIME_10ms))
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
    SET_BANK_1();
	REG1_SENDD = Uart1TXDBuf[0];
    SET_BANK_0();
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
		if(F_GetTimeTick(C_TIME_10ms))g_u8TxdBusyTim--;
	}
}

void F_Uart1DuplexProc(void)
{
	F_UartRxdProc();
	F_UartTxdProc();
}
void F_Uart_Init(unsigned int Freq, unsigned long int baud)  
{    	    
    SET_BANK_1();
    
    PCON2 = 0x00;
	SCON2 = 0x50; 	// 配置Uart工作在模式1，UART0接收器允许
	/*配置波特率参数，波特率9600*/
	/* 计算公式：(int)X=FSY/(16*波特率) ;  SBRT=32768-X  ;   SFINE=（FSY/波特率）-16*X   FSY=12M*/
	SBRTH2 = ( (32768-(Freq*1000000/16/baud) & 0x7f00) >> 8) | 0x80; //高位为波特率使能位
	SBRTL2 = ( 32768-(Freq*1000000/16/baud) )& 0x00ff;
	SFINE2 = (Freq*1000000/baud) - (16*(Freq*1000000/16/baud));
    
    SET_BANK_0();
    
	IEN2 |= 0x02;  
}
void UART2_IRQHandler(void)  interrupt 16 
{
	//-----------------------------------------------------------
	// 压栈保护
    _push_(INSCON);
	INSCON = 0;
    _push_(FLASHCON);
    FLASHCON = 0;
	
	//-----------------------------------------------------------
    SET_BANK_1();
    
	if(SCON2 & 0x02)
	{	
		SCON2 &= ~0x02;
        F_IRQ_TXD();
        
	}
	if(SCON2 & 0x01) 
	{	
		SCON2 &= ~0x01;
        F_IRQ_REC();    
	}
    
    SET_BANK_0();
    //-----------------------------------------------------------
    
	// 出栈
    INSCON = 0;
	_pop_(FLASHCON);
    _pop_(INSCON);
}
void F_SysUart1Init(void)
{
	while(g_u8TxdNum)
	{
		g_u8TxdNum --;
		Uart1TXDBuf[g_u8TxdNum] = 0;
	}
	F_RXDInit();
	F_Uart_Init(XTAL_FREQUENCY,9600);
}