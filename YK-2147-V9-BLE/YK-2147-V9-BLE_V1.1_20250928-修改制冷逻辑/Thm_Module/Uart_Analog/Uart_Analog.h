#ifndef C_UART_Analog_H
#define C_UART_Analog_H

/*************************需配置部分***********************/
//串口属性配置
#define 	C_DEVICE_TYPE					C_UART_SLAVE	//设备属性（主或从）	
#define		C_BUFF_LENGTH					10
#define		C_SEND_LENGTH					9
#define		C_REC_LENGTH					10
#define		C_REC_NO_DATA_TIME				50				// 500ms收不到中断，认为没有应答
//#define		C_REC_INT_C_DECODE_TIME			3				//收到中断后30ms，解码
#define		C_REC_INT_C_DECODE_TIME			255
#define		C_SEND_DELAY_TIME				1				// 接收到信息后，延迟20ms发送

#define		Def_ONLY_TX     	            0
#define		Def_ONLY_RX     	            1
#define		Def_TEST_BAOUNDRATE     	    0

//IO配置
#define 	Tx_Set_High()			{P11 = 1;}
#define 	Tx_Set_Low()			{P11 = 0;}
#define		Tx_Set_Output()			{Set_P11_pp;}
#define		Rx_Set_Input()			{Set_P11_in;}
#define		Rx_Pull_High()			{Set_P11_pu;}
#define		GetRxPin()				(P11)

#define		_TEST_BAOUNDRATE		 {P13 = ~P13;_NOP;_NOP;_NOP;P13 = ~P13;}
#define		_CLEAR_BAOUNDRATE		 {P13 = 0;}

//#define 	Pin_Set_High()			{Set_P13;}
//#define 	Pin_Set_Low()			{Clr_P13;}
//#define		Pin_Set_Input()			{Set_P13_in;}
//#define		Pin_Set_Output()		{Set_P13_pp;}
//#define		Pin_PullL_High()		{Set_P13_pu;}
//#define		GetRecPin()				(Test_P13)

/*************************配置结束*************************/

#define		Def_BoundRateCalFunEn			0

#define		C_UART_MASTER					0		//主设备
#define		C_UART_SLAVE					1		//从设备

#define 	C_DECODE_PASS					1
#define 	C_DECODE_FAIL					0

#define		C_UART_C_REC_INIT				0
#define 	C_UART_RECEIVE_ING				1
#define 	C_UART_C_REC_END				2
#define 	C_UART_SEND_INIT				3
#define 	C_UART_SEND_ING					4
#define 	C_UART_SEND_END					5

#define		C_BIT_SAMPLE_CNT				0x80
#define		C_SUM_HIGH						2

#define 	C_START_BIT						0
#define 	C_RE_START_BIT					1
#define 	C_DATA_BIT						2
#define 	C_STOP_BIT						3
#define 	C_DOUBLE_STOP_BIT				4	

//r_u8SamplePhase
#define 	b0011_1000 						0x38
#define 	b0000_0111 						0x07
#define 	b1110_0000 						0xE0
#define 	b0001_1100 						0x1C

//g_u16DisconnectCnt
#define 	C_UART_DISCONNECT_1S 			100
#define 	C_UART_DISCONNECT_2S 			200
#define 	C_UART_DISCONNECT_3S 			300
#define 	C_UART_DISCONNECT_4S 			400
#define 	C_UART_DISCONNECT_5S 			500
#define 	C_UART_DISCONNECT_6S 			600
#define 	C_UART_DISCONNECT_7S 			700
#define 	C_UART_DISCONNECT_8S 			800
#define 	C_UART_DISCONNECT_9S 			900
#define 	C_UART_DISCONNECT_10S 			1000

//外部声明
extern	u8   xdata g_u8UartAnalogErro;

extern	void F_UART_Analog_Protocol(void);
extern	void F_UART_Analog_IRQ(void);
extern	void F_UART_Analog_Init(void);
extern	void F_UART_Analog_TimeOut(void);

#endif
