#ifndef UARTHARD_H	
#define UARTHARD_H	
                
#ifdef	EXTR                                      
#undef	EXTR      
#endif                                  			
				                 
#ifdef	UartHard	
#define EXTR                                   
#else	                                           
#define EXTR extern 
#endif  

#define FOSC0 		(CPU_Freq*1000000ul)//12M
#define BAUD0			9600

//#define HIBYTE(v1)	((uchar)((v1)>>8))
//#define LOBYTE(v1)	((uchar)((v1)&0xff))

#define BAUD0_INIT	(uint)(FOSC0/BAUD0/16)
#define BAUD0_SFINE	(uint)(FOSC0/BAUD0-BAUD0_INIT*16)
#define BAUD0_SBRTL	 LOBYTE(32768-BAUD0_INIT)
#define BAUD0_SBRTH	(HIBYTE(32768-BAUD0_INIT))|0x80


//======================================================
/*****************************************************************
	用户定义的硬件接口
******************************************************************/
//---------------------UART0相关设置---------------------------------------------------
//==========================================================================
//#define UART0_PORT_SET(x)	    {REG_ADDR = 0x34;REG_DATA &= ~(0x60);REG_DATA |= (0x60&(x<<5));}//串口功能映射选择0-为选择PC4/5为串口，1选择PE4/5为串口，2/3选择PF4/5为串口
//#define UART0_ON_OFF(x) 		{UART0_CON1 &= ~(0x40);UART0_CON1 |= (0x40&(x<<6));}//UAERT模块开关,1为开，0为关
#define UART0_IE_EN 			IEN0 |= 0x10	//开UART中断使能
#define UART0_IE_DISABLE 		IEN0 &= ~0x10	//关UART中断使能
//#define UART0_IP_SET			IPL2 |= 0x04	//设置UART中断优先级为高
//#define UART0_IP_CLR			IPL2 &= ~0x04	//设置UART中断优先级为低
//#define UART0_INT_FLAG_CLR	    IRCON2 &= ~0x04	//清除UART中断标志位

#define UART0_INT_IF_TF_CLR()	{SCON &= ~(0x02);}//清除UART0_TF标志位
#define UART0_INT_IF_RF_CLR()	{SCON &= ~(0x01);}//清除UART0_RF标志位
//#define UART0_INT_IF_RO_CLR(x)	{UART0_STATE &= ~(0x04);UART0_STATE |= (0x04&(x<<2));}//写0清除UART0_溢出标志位
//#define UART0_INT_IF_F_CLR(x)	{UART0_STATE &= ~(0x02);UART0_STATE |= (0x02&(x<<1));}//写0清除UART0_帧错标志位
//#define UART0_INT_IF_P_CLR(x)	{UART0_STATE &= ~(0x01);UART0_STATE |= (0x01&(x<<0));}//写0清除UART0_寄偶校验错误标志位
#define UART0_INT_CLR_RF 		SCON&0x01
#define UART0_INT_CLR_TF  		SCON&0x02
//==========================================================================
#define GetUART0SBUF(Data)		{(Data) = SBUF;}
#define SetUART0SBUF(Data)		{SBUF = (Data);}


EXTR void EnableRecWifiComm(void);
EXTR void DisableRecWifiComm(void);
EXTR void IniSend_WifiComm(void);
EXTR void Inirec_WifiComm(void);
EXTR void Init_WifiComm(void);
#endif
/******************************************************************
						END
******************************************************************/
