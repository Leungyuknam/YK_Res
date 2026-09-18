
#ifndef _UART1_H_
#define _UART1_H_

#define 	C_SEND_MAX_1          	8	    //发送最大长度
#define 	C_RECEIVE_MAX_1         10	    //接收最大长度
#define 	C_SEND_QUEUE_MAX_1    	3		//发送函数队列长度
#define 	C_SEND_INTERVAL_TIME_1  10		//两帧发送数据的间隔时间
#define 	C_RESEND_DELAY_TIME_1 	10		//重发间隔时间
#define 	C_RESEND_TIMES_1      	1		//重发次数
#define 	C_NO_RECEIVE_TIME_1   	5		//一段时间内没收到数据，进行一次解码
#define 	C_REC_ERROR_TIME_1      1000    //接收出错时长设定

//typedef void (*voidFan)(void);
//typedef void (*U8Fan)(unsigned char DA);
			 
//typedef struct
//{
//	U8Fan Fan;			  //函数名
//	unsigned char SN;	  //函数输入形参
//}OneParametFunStr;

	

typedef struct
{
	OneParametFunStr Fun[C_SEND_QUEUE_MAX_1];	   //函数队列
	unsigned char Head;						       //队列头
	unsigned char Tail;						       //队列尾
}OneParametQueue1;

#ifndef   RamDataType
#define   RamDataType    xdata
#endif

#define 	Def_USE_UART0_S           0		//95,92系列相同
#define 	Def_USE_USCI0_S           0		//仅95
#define 	Def_USE_USCI1_S           0		//仅95
#define 	Def_USE_USCI2_S           1		//仅95
#define 	Def_USE_SSI_S             0		//仅92


#if Def_USE_UART0_S

#define 	USER_REG_RECEIVE_S        SBUF
#define 	USER_REG_SEND_S           SBUF
#define 	USER1_UART_C51_INT_NUM    4
#define 	_TEST_UART1_RX            RI
#define 	_CLR_UART1_RX             RI = 0
#define 	_TEST_UART1_TX            TI
#define 	_CLR_UART1_TX             TI = 0


#elif Def_USE_USCI0_S

#define 	USER_REG_RECEIVE_S        US0CON3
#define 	USER_REG_SEND_S           US0CON3
#define 	USER1_UART_C51_INT_NUM    7
#define 	_TEST_UART1_RX            (US0CON0 & 0x01)
#define 	_CLR_UART1_RX             US0CON0 &= 0xFE
#define 	_TEST_UART1_TX            (US0CON0 & 0x02)
#define 	_CLR_UART1_TX             US0CON0 &= 0xFD

#elif Def_USE_USCI1_S

#define 	USER_REG_RECEIVE_S        US1CON3
#define 	USER_REG_SEND_S           US1CON3
#define 	USER1_UART_C51_INT_NUM    15
#define 	_TEST_UART1_RX            (US1CON0 & 0x01)
#define 	_CLR_UART1_RX             US1CON0 &= 0xFE
#define 	_TEST_UART1_TX            (US1CON0 & 0x02)
#define 	_CLR_UART1_TX             US1CON0 &= 0xFD

#elif Def_USE_USCI2_S

#define 	USER_REG_RECEIVE_S        US2CON3
#define 	USER_REG_SEND_S           US2CON3
#define 	USER1_UART_C51_INT_NUM    16
#define 	_TEST_UART1_RX            (US2CON0 & 0x01)
#define 	_CLR_UART1_RX             US2CON0 &= 0xFD
#define 	_TEST_UART1_TX            (US2CON0 & 0x02)
#define 	_CLR_UART1_TX             US2CON0 &= 0xFE

#elif Def_USE_SSI_S

#define 	USER_REG_RECEIVE_S        SSDAT
#define 	USER_REG_SEND_S           SSDAT
#define 	USER1_UART_C51_INT_NUM    7
#define 	_TEST_UART1_RX            (SSCON0 & 0x01)
#define 	_CLR_UART1_RX             SSCON0 &= 0xFE
#define 	_TEST_UART1_TX            (SSCON0 & 0x02)
#define 	_CLR_UART1_TX             SSCON0 &= 0xFD

#endif

#define 	_UART_RX_EN             EA = 0;_CLR_UART1_RX;_BS(US2CON0,4);EA = 1
#define 	_UART_RX_DIS            EA = 0;_CLR_UART1_RX;_BC(US2CON0,4);EA = 1

#define C_ONEBitSendOverRigistSet_S()     {}
#define C_ONEFrameSendOverRigistSet_S()   {}
#define C_CheckProtocolFrameFinish_S()    {} 
#define C_ReadOtherProtocolDeal_S()       {}
#define C_SendOtherProtocolDeal_S()       {}
#define F_UART_Send_init_S()	          {}
#define F_UART_Recieve_init_S()	          {}


//外部函数声明
#define 	F_GetReadFrameNData_En_S 	   0
#define 	F_AddActiveSendFun_En_S 	   1
#define 	F_AddFrebackSendFun_En_S 	   1
#define 	F_DecActiveSendFun_En_S 	   0

//外部变量声明
extern unsigned char RamDataType g_u8ReadDataLength1;
extern unsigned char RamDataType g_u8SendBuf1[C_SEND_MAX_1];
extern unsigned char RamDataType g_u8ReadBuf1[C_RECEIVE_MAX_1];
extern unsigned char RamDataType g_u8FrebackInf1;

//选用函数
extern unsigned char F_GetReadFrameNData_S(unsigned char N);
extern void          F_AddActiveSendFun_S(U8Fan Fun,  unsigned char SN);
extern void          F_AddFrebackSendFun_S(U8Fan Fun, unsigned char SN ); 
extern void          F_DecActiveSendFun_S( void );

//必用函数
extern void          F_OneFrameDataReceived_S(void);
extern void          F_UartControl_S(void);
extern void          F_StartSend_S(unsigned char l_u8Length);
extern void          F_UartRecInital_S(void);



#endif