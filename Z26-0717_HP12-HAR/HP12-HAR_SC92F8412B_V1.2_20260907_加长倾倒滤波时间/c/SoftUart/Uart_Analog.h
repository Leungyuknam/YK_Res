#ifndef __UART_Analog_H__
#define __UART_Analog_H__
		                                                                                                                                                                                 
 
#define		C_UART_MASTER			0		//主设备
#define		C_UART_SLAVE			1		//从设备

#define 	C_DECODE_PASS			1
#define 	C_DECODE_FAIL			0

#define		C_REC_NO_DATA_TIME				10			// 100 ms；500 ms 收不到中断则认为没有应答
#define		C_REC_INT_C_DECODE_TIME			3			//收到中断后30ms，解码
#define		C_SEND_DELAY_TIME				2			// 接收到信息后，延迟20ms发送


//------------配置部分-------------------
#define 	C_DEVICE_TYPE			C_UART_MASTER	//设备属性（主或从）
#define		C_BUF_LENGTH			7

#define 	Pin_Set_High()			{P11 = 1;}
#define 	Pin_Set_Low()			{P11 = 0;}
#define		Pin_Set_Input()			{P1CON &= ~BIT1;}
#define		Pin_Set_Output()		{P1CON |= BIT1;}
#define		Pin_PullL_High()		{P1PH |= BIT1;}
#define		GetRecPin()				P11

//----------调用的函数和数据-----------------------

void F_UartProtocol();			// 每 10 ms 调用
void F_Uart_Analog_IRQ();		// 在 125 us 中断中调用
void F_UartInit();				// 初始化
extern unsigned char xdata g_u8UartBuf[C_BUF_LENGTH];	//存放发送数据

//------------需要自己定义的函数---------------------
extern U8 MakeEnCode();						//放发送数据至g_u8UartBuf，返回字节数触发发送，否则返回0
extern U8 DeCodeProc(U8 l_u8ByteNum);		// l_u8ByteNum 为接收字节数；解码成功返回 C_DECODE_PASS，否则返回 C_DECODE_FAIL

#endif
