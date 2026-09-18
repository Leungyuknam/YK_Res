
#ifndef _UART_H_
#define _UART_H_

#define 	C_SEND_MAX          	36	    //发送最大长度
#define 	C_RECEIVE_MAX          	36	    //接收最大长度
#define 	C_SEND_QUEUE_MAX    	20		//发送函数队列长度
#define 	C_SEND_INTERVAL_TIME   	10		//两帧发送数据的间隔时间
#define 	C_RESEND_DELAY_TIME 	5		//重发间隔时间
#define 	C_RESEND_TIMES      	1		//重发次数
#define 	C_NO_RECEIVE_TIME   	6		//一段时间内没收到数据，进行一次解码


typedef void (*voidFan)(void);
typedef void (*U8Fan)(unsigned char DA);
			 
typedef struct
{
	U8Fan Fan;			  //函数名
	unsigned char SN;	  //函数输入形参
}OneParametFunStr;

typedef struct
{
	OneParametFunStr Fun[C_SEND_QUEUE_MAX];	   //函数队列
	unsigned char Head;						   //队列头
	unsigned char Tail;						   //队列尾
}OneParametQueue;

#ifndef   RamDataType
#define   RamDataType    xdata
#endif

#define 	Def_USE_UART0           0		//95,92系列相同
#define 	Def_USE_USCI0           1		//仅95
#define 	Def_USE_USCI1           0		//仅95
#define 	Def_USE_USCI2           0		//仅95
#define 	Def_USE_SSI             0		//仅92


#if Def_USE_UART0

#define 	USER_REG_RECEIVE        SBUF
#define 	USER_REG_SEND           SBUF
#define 	USER_UART_C51_INT_NUM   4
#define 	_TEST_UART_RX           RI
#define 	_CLR_UART_RX            RI = 0
#define 	_TEST_UART_TX           TI
#define 	_CLR_UART_TX            TI = 0


#elif Def_USE_USCI0

//#define 	USER_REG_RECEIVE        US0CON3
//#define 	USER_REG_SEND           US0CON3
//#define 	USER_UART_C51_INT_NUM   7
//#define 	_TEST_UART_RX           (US0CON0 & 0x01)
//#define 	_CLR_UART_RX            US0CON0 &= 0xFE
//#define 	_TEST_UART_TX           (US0CON0 & 0x02)
//#define 	_CLR_UART_TX            US0CON0 &= 0xFD

#define 	USER_REG_RECEIVE        US0CON3
#define 	USER_REG_SEND           US0CON3
#define 	USER_UART_C51_INT_NUM   7
#define 	_TEST_UART_RX           (US0CON0 & 0x01)
#define 	_CLR_UART_RX            US0CON0 &= 0xFD
#define 	_TEST_UART_TX           (US0CON0 & 0x02)
#define 	_CLR_UART_TX            US0CON0 &= 0xFE


#elif Def_USE_USCI1

#define 	USER_REG_RECEIVE        US1CON3
#define 	USER_REG_SEND           US1CON3
#define 	USER_UART_C51_INT_NUM   15
#define 	_TEST_UART_RX           (US1CON0 & 0x01)
#define 	_CLR_UART_RX            US1CON0 &= 0xFE
#define 	_TEST_UART_TX           (US1CON0 & 0x02)
#define 	_CLR_UART_TX            US1CON0 &= 0xFD

#elif Def_USE_USCI2

#define 	USER_REG_RECEIVE        US2CON3
#define 	USER_REG_SEND           US2CON3
#define 	USER_UART_C51_INT_NUM   16
#define 	_TEST_UART_RX           (US2CON0 & 0x01)
#define 	_CLR_UART_RX            US2CON0 &= 0xFE
#define 	_TEST_UART_TX           (US2CON0 & 0x02)
#define 	_CLR_UART_TX            US2CON0 &= 0xFD

#elif Def_USE_SSI

#define 	USER_REG_RECEIVE        SSDAT
#define 	USER_REG_SEND           SSDAT
#define 	USER_UART_C51_INT_NUM   7
#define 	_TEST_UART_RX           (SSCON0 & 0x01)
#define 	_CLR_UART_RX            SSCON0 &= 0xFE
#define 	_TEST_UART_TX           (SSCON0 & 0x02)
#define 	_CLR_UART_TX            SSCON0 &= 0xFD

#endif


#define C_ONEBitSendOverRigistSet()     {}
#define C_ONEFrameSendOverRigistSet()   {}
#define C_CheckProtocolFrameFinish()    {} 
#define C_ReadOtherProtocolDeal()       {}
#define C_SendOtherProtocolDeal()       {}
#define F_UART_Send_init()	            {}
#define F_UART_Recieve_init()	        {}


//外部函数声明
#define 	F_GetReadFrameNData_En 	       0
#define 	F_AddActiveSendFun_En 	       1
#define 	F_AddFrebackSendFun_En 	       1
#define 	F_DecActiveSendFun_En 	       0

//外部变量声明
extern unsigned char RamDataType g_u8ReadDataLength;
extern unsigned char RamDataType g_u8SendBuf[C_SEND_MAX];
extern unsigned char RamDataType g_u8ReadBuf[C_RECEIVE_MAX];
extern unsigned char RamDataType g_u8FrebackInf;

//选用函数
extern unsigned char F_GetReadFrameNData(unsigned char N);
extern void          F_AddActiveSendFun(U8Fan Fun,  unsigned char SN);
extern void          F_AddFrebackSendFun(U8Fan Fun, unsigned char SN ); 
extern void          F_DecActiveSendFun( void );

//必用函数
extern void          F_OneFrameDataReceived(void);
extern void          F_UartControl(void);
extern void          F_StartSend(unsigned char l_u8Length);
extern void          F_UartRecInital(void);



#endif