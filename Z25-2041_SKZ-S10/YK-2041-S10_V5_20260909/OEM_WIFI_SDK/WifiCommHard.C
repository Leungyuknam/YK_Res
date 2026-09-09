#define UartHard
//#include "Define.h"
#include "h\Allinclude.h"
/***************************************************************
NAME    : EnableRecWifiComm()                         
FUNCTION: 打开中断使能
INPUT   : null
RETURN  : null			                             
***************************************************************/
void EnableRecWifiComm(void)
{
	IEN0 |= 0x10;	//开UART中断使能	 	
}
/***************************************************************
NAME    : DisableRecWifiComm()                   
FUNCTION: 关闭中断使能
INPUT   : null
RETURN  : null			                             
***************************************************************/
void DisableRecWifiComm(void)
{
	IEN0 &= ~0x10;	//关UART中断使能	 
}
/***************************************************************
NAME	: Init_WifiComm()                         
FUNCTION: Wifi UART0初始化
INPUT	: null
RETURN	: null			                             
***************************************************************/
void Init_WifiComm(void)
{
	Uart0_Init(24,9600);
//	SETBANK1();  
//	UART0CR = 0x40 + 0x05;//TXD0-P2.6 RXD0-P2.7
//	SETBANK0();
//	/*---EUART0初始化---*/
//	/*P_WIFI_TXD_OUTMODE();
//	P_WIFI_TXD_ENPULL();
//	P_WIFI_TXD_SETB();

//	P_WIFI_RXD_INMODE();
//	P_WIFI_RXD_ENPULL();
//	P_WIFI_RXD_SETB();*/
//	
//	SBRTL = BAUD0_SBRTL;//UART0配置自带波特率发生器	
//	SBRTH = BAUD0_SBRTH;
//	SFINE = BAUD0_SFINE;//UART0波特率发生器微调数据寄存器
//	SBRTH |= BIT7;//UART0波特率发生器使能控制位

//	SCON = 0x40;//UART0设置串口通讯方式1 波特率可变。
//	PCON = 0x00;
//	REN  = 1;//UART0允许接收	
//	
//	IEN0 |= BIT4;//允许UART0中断
}
/***************************************************************
NAME	: Inirec_SUART()                          
FUNCTION: 设置UART接收波特率
INPUT	: null
RETURN	: null			                             
***************************************************************/
void Inirec_WifiComm(void)
{		
     if (UART_SEND_HALT == ucSendState)	//正在发送空闲区时候，执行所有串口初始化
     {
		Init_WifiComm();		
     }	
     //WifisUartGetByte(ucTemp);	//打开接收中断前清除接收请求标识位
     EnableRecWifiComm();	
     ucReceState  = UART_RECING;  
     ucRecCountWifi = 0;
}
/***************************************************************
NAME	: IniSend_WifiComm()                          
FUNCTION: 设置UART发送波特率
INPUT	: null
RETURN	: null			                             
***************************************************************/
void IniSend_WifiComm(void) //
{
	if(ucWifiRecFinishTime == 0xff)	//判断接收是否已经接收到数据
	{		
		Init_WifiComm();	
	}
	ucSendState = UART_SENDING;
	ucSendCountWifi = 1;	        		//one command send finshed
	ucT10msSendTimeOut = U8_SENDTIMR300MS;	//300MS还没有发送完成的时候，直接转换为发送空闲	
	WifiUartSendByte(ucSendBufWifi[0]);
}
/*******************************************************************
			END	
*******************************************************************/
