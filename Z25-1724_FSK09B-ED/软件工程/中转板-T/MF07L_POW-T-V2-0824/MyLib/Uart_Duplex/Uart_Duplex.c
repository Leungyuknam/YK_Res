#define UART_DUPLEX_C

#include "..\Include\Include.h"
 
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
	F_CheckCommucate();
	F_UartRxdProc();
	F_UartTxdProc();
}


 //PIN1~PIN8
//GND  //PIN1
//#define P_LR4		P1_0 //PIN2
//#define P_PWM		P1_1 //PIN3  //风扇驱动
//#define P_LR1		P2_7 //PIN4
//#define P_LR2		P2_6 //PIN5
//烧录口SCK	 //PIN6  P2_5
//烧录口SDA	 //PIN7	 P2_4	
//#define P_LR3		P2_3 //PIN8

//PIN9~PIN16
//#define P_VOL_AD	P0_6 //AN6 //PIN9
//#define P_UPDN1		P0_5 //PIN10
//#define P_UPDN2		P0_4 //PIN11
//#define P_UPDN3		P0_3 //PIN12
//#define P_UPDN4		P0_2 //PIN13
//#define P_TX		P0_1 //PIN14
//#define P_RX		P0_0 //PIN15
//VDD  //PIN16
/*****************************************************
*函数名称：void Uart0_Init(uint Freq,unsigned long int baud)
*函数功能：Uart0中断初始化
*入口参数：Freq-主频，baud-波特率
*出口参数：void
*****************************************************/
void Uart0_Init()    //选择Timer1作为波特率信号发生器  //uint Freq,unsigned long int baud
{
	P0M2 = P0M2&0x0F|0x80;				          //P05设置为推挽输出
	P0M3 = P0M3&0xF0|0x02;				          //P06设置为上拉输入
	TXD_MAP = 0x05;						          //TXD映射P05
	RXD_MAP = 0x06;						          //RXD映射P06
	BRTSEL = 0X00;                                //UART1的波特率:00 T4
	
	T4CON = 0x06;						          //T4工作模式：UART1波特率发生器
	
	//波特率计算
	//波特率 = 1/16 * (T4时钟源频率 / 定时器4预分频比) / (65536 - 0xFF98)
	//       = 1/16 * ((16000000 / 1) / 104)
	//		 = 9615.38(误差0.16%)

	//波特率9600
	//反推初值 = (65536 - ((T4时钟源频率 / 定时器4预分频比) * (1 / 16)) / 波特率)
	//		   = (65536 - (16000000 * (1 / 16) / 9600))
	//		   = (65536 - 104.167)
	//         = FF98
	
    TH4 = 0xFF;
	TL4 = 0x98;							             //波特率9600
	SCON2 = 0x02;						             //8位UART，波特率可变
	SCON = 0x10;					                 //允许串行接收
	IE |= 0X10;							             //使能串口中断
}

/*****************************************************
*函数名称：void UartInt(void) interrupt 4
*函数功能：Uart0中断函数
*入口参数：void
*出口参数：void
*****************************************************/
void UartInt(void) interrupt 4
{
	if(SCON & 0x02)
	{
		SCON &=~ 0x02;			             //清除发送中断标志位	
		F_IRQ_TXD();		
	}
	
	if(SCON & 0x01)						        //判断接收中断标志位
	{
		SCON &=~ 0x01;					        //清除接收中断标志位
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
	Uart0_Init(); //12,9600
}