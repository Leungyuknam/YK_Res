#ifndef UART_Analog_H
#define UART_Analog_H

#ifdef	EXTR
#undef	EXTR                                                                        
#endif

#ifdef	UART_Analog_C
#define EXTR                                                                                                         
#else						                                                                                         
#define EXTR	extern                                                                                               
#endif 

#define		UART_MASTER			0		//主设备
#define		UART_SLAVE			1		//从设备
#define 	DEVICE_TYPE			UART_SLAVE //UART_MASTER//UART_SLAVE	//设备属性（主或从）
#define		BUF_LENGTH			7

//#define		LR_RX_LENGTH		4

#define 	DECODE_PASS			1
#define 	DECODE_FAIL			0

#define		C_REC_NO_DATA_TIME				50			// 500ms收不到中断，认为没有应答
#define		C_REC_INT_DECODE_TIME			2//3			//收到中断后30ms，解码
#define		C_SEND_DELAY_TIME				2//4//10			// 接收到信息后，延迟20ms发送

#define		MAX_DISCONECT_TIME				50

#define		TX_SET_OUTPUT()		    {P1CON |= BIT3;}
#define 	TX_SET_HIGH()			{P13 = 1;}
#define 	TX_SET_LOW()			{P13 = 0;}
#define		TX_PULL_HIGH()		    {P1PH |= BIT3;}	//PxPCR 0：内部上拉电阻关闭 1：内部上拉电阻开启
#define		TX_PULL_HIGH_OFF()		{P1PH &= ~BIT3;}	//PxPCR 0：内部上拉电阻关闭 1：内部上拉电阻开启



#define		RX_SET_INPUT()			{P1CON &= ~BIT2;}   //PxCR 0：输入模式  1：输出模式
#define		RX_PULL_HIGH()		    {P1PH |= BIT2;}	//PxPCR 0：内部上拉电阻关闭 1：内部上拉电阻开启
#define		RX_REC_PIN()			(P12)

extern xdata unsigned char ControlDisConnect_Cnt;


extern xdata unsigned char  TxUartBuf[BUF_LENGTH];
extern xdata unsigned char RxUartBuf[BUF_LENGTH];
extern xdata unsigned char RxUartCmpBuf[BUF_LENGTH];



EXTR	void UartProtocol(void);
EXTR	void Uart_Analog_IRQ(void);


#endif
