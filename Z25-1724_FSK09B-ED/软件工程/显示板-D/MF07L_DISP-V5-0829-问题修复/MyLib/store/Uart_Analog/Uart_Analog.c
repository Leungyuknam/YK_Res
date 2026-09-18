/*************************************************************
说明:
(1)使用定时器模拟UART通信，一般125us作为定时中断。
(2)接收管脚无需外部中断，完全使用定时器采样替代中断。
可以1根线通信，也可以2根线通信。
(3)采用应答方式，主机先发，从机应答。


//Author:陈首敏
*************************************************************/

#define		UART_Analog_C
#include "Include\Include.h"


#define		UART_REC_INIT			0
#define 	C_UART_RECEIVE_ING		1
#define 	C_UART_C_REC_END		2
#define 	C_UART_SEND_INIT		3
#define 	C_UART_SEND_ING			4
#define 	C_UART_SEND_END			5

#define		C_BIT_SAMPLE_CNT		b0001_0000
#define		C_SUM_HIGH				2

#define 	C_START_BIT				0
#define 	C_RE_START_BIT			1
#define 	C_DATA_BIT				2
#define 	C_STOP_BIT				3
#define 	C_DOUBLE_STOP_BIT		4
#define 	C_WAIT_H_BIT			5


xdata unsigned char  RxUartMode = UART_REC_INIT;			
xdata unsigned char  TxUartMode = C_UART_SEND_INIT;  
xdata unsigned char  TxUartTimeOut = C_SEND_DELAY_TIME;				//应答超时时间
xdata unsigned char  RxUartTimeOut = C_SEND_DELAY_TIME;				//应答超时时间

xdata unsigned char  TxSampleCnt = C_BIT_SAMPLE_CNT;
xdata unsigned char  RxSampleCnt = C_BIT_SAMPLE_CNT;
xdata unsigned char  TxBitSection = C_START_BIT;
xdata unsigned char  RxBitSection = C_START_BIT;
xdata unsigned char  TxBitPhase = 0x01;
xdata unsigned char  RxBitPhase = 0x01;
xdata unsigned char  TxByteNum = 0;
xdata unsigned char  RxByteNum = 0;
xdata unsigned char  TxCurByte = 0;
xdata unsigned char  RxCurByte = 0;
xdata unsigned char  TxBufLength = 0;
xdata unsigned char  RxBufLength = 0;

xdata unsigned char  TxPinHighSum = 0;
xdata unsigned char  RxPinHighSum = 0;
xdata unsigned char  TxSamplePhase = b0000_1110;
xdata unsigned char  RxSamplePhase = b0000_1110;

xdata unsigned char  TxUartBuf[BUF_LENGTH] = {0};
xdata unsigned char  RxUartBuf[BUF_LENGTH] = {0};
xdata unsigned char  RxUartCmpBuf[BUF_LENGTH];
xdata unsigned char  RxCalRate = 0xff;


xdata unsigned char ControlDisConnect_Cnt = 0;
xdata unsigned char g_u8RxSI_Cnt = 0;

void CalBoundRate(void)
{
	//if (CalRate != 0xff)
	{
		//if ((CalRate > 55 + 1) && (CalRate < 55 + 5))
		{
		//	SamplePhase = b0000_0111;
		}
		//else if ((CalRate < 55 - 1) && (CalRate > 55 - 5))
		{
		//	SamplePhase = b0001_1100;
		}
		//else
		{
			RxSamplePhase = b0000_1110;
		}
	}
	RxCalRate = 0;
}

unsigned char DecCode(void)
{
	unsigned char Result;
	if (RxByteNum)
	{
		Result = DeCodeProc(&RxUartBuf, RxByteNum);
	}
	else
	{
		Result = DECODE_FAIL;
	}
	return Result;
}

unsigned char EnCode(void)
{
	unsigned char i;
	for (i = 0; i < BUF_LENGTH; i++)
	{
		TxUartBuf[i] = 0;
	}
	i = MakeEnCode(&TxUartBuf);
	return i;
}

void F_UartSendInit(void)
{
	TxBitSection = C_START_BIT;
	TxByteNum = 0;
	TxSampleCnt = C_BIT_SAMPLE_CNT;
	TX_SET_HIGH();
	TX_SET_OUTPUT();
	TX_PULL_HIGH();
}

void F_SendDrv(void)
{
	TxSampleCnt >>= 1;
	if (TxSampleCnt == 0)
	{
		TxSampleCnt = C_BIT_SAMPLE_CNT;
		if (TxBitSection == C_START_BIT)
		{ //起始位
			TX_SET_LOW();
			TxBitSection = C_DATA_BIT;
			TxBitPhase = 0x01;
			TxCurByte = TxUartBuf[TxByteNum];
		}
		else if (TxBitPhase != 0)
		{ // 8位数据
			if (TxCurByte & TxBitPhase)
			{
				TX_SET_HIGH();
			}
			else
			{
				TX_SET_LOW();
			}
			TxBitPhase <<= 1;
		}
		else
		{ //结束位
			TX_SET_HIGH();
			if (TxBitSection == C_DATA_BIT)
			{
				TxByteNum++;
				if (TxByteNum >= TxBufLength)
				{ // 发送结束
					TxUartMode = C_UART_SEND_END;
				}
				else
				{
					TxBitSection = C_DOUBLE_STOP_BIT;
				}
			}
			else
			{ // 发送结束位
				TxBitSection = C_START_BIT;
			}
		}
	}
}

void RxUartRecInit(void)
{
	unsigned char i;
	RX_SET_INPUT();
	RX_PULL_HIGH();
	RxBitSection = C_START_BIT;
	RxByteNum = 0;
	RxSampleCnt = C_BIT_SAMPLE_CNT;
	for (i = 0; i < BUF_LENGTH; i++)
	{
		RxUartBuf[i] = 0;
	}
}

void F_RecDrv(void)
{
	if (RxCalRate != 0xff)
		RxCalRate++;
	if (RxBitSection == C_START_BIT)
	{
		if (0 == RX_REC_PIN())
		{
			RxBitSection = C_RE_START_BIT;
			RxPinHighSum = 0;
			RxSampleCnt = C_BIT_SAMPLE_CNT >> 1;
			CalBoundRate();
		}
	}
	else if (RxBitSection == C_WAIT_H_BIT)
	{
		if (RX_REC_PIN())
			RxBitSection = C_START_BIT;
	}
	else
	{
		if (RX_REC_PIN())
		{
			if (RxSampleCnt & RxSamplePhase)
			{
				RxPinHighSum++;
			}
		}
		RxSampleCnt >>= 1;
		if (RxSampleCnt == 0)
		{
			RxSampleCnt = C_BIT_SAMPLE_CNT;
			if (RxBitSection == C_RE_START_BIT)
			{								 // start bit
				if (RxPinHighSum < C_SUM_HIGH) // low
				{							 // OK
					RxBitSection = C_DATA_BIT;
					RxCurByte = 0;
					RxBitPhase = 0x01;
					RxUartTimeOut = C_REC_INT_DECODE_TIME;
					g_u8RxSI_Cnt = 0;
				}
				else
				{ // FAIL
					RxBitSection = C_START_BIT;
				}
			}
			else if (RxBitPhase)
			{ // receice 8 bit
				if (RxPinHighSum >= C_SUM_HIGH)
				{ // High
					RxCurByte |= RxBitPhase;
				}
				RxBitPhase <<= 1;
			}
			else
			{ // stop bit

				if (RxPinHighSum >= C_SUM_HIGH)
				{
					RxUartBuf[RxByteNum] = RxCurByte;
					RxByteNum++;
					RxBitSection = C_START_BIT;
				}
				else
				{
					RxBitSection = C_WAIT_H_BIT;
				}
				if (RxByteNum >= BUF_LENGTH)
				{ // Fail finhish
					RxUartMode = C_UART_C_REC_END;
				}
			}
			RxPinHighSum = 0;
		}
	}
}

void Uart_Analog_IRQ(void)
{
	if (C_UART_SEND_ING == TxUartMode)
	{
		F_SendDrv();
	}

	if (C_UART_RECEIVE_ING == RxUartMode)
	{
		F_RecDrv();
	}
}

/*********************************************************************
//UART接收和发送协议处理
*********************************************************************/
void UartProtocol(void)
{
	if (GetTimeTick(TIME_10MS))
	{

		if (TxUartTimeOut)
			TxUartTimeOut--;
	}

	if (C_UART_SEND_INIT == TxUartMode) //发送初始化
	{
		if (0 == TxUartTimeOut)
		{
			TxBufLength = EnCode();
			if (TxBufLength)
			{
				
				F_UartSendInit();
				TxUartMode = C_UART_SEND_ING;
			}
		}
	}
	else if (C_UART_SEND_END == TxUartMode) //发送结束
	{
		TxUartTimeOut = C_SEND_DELAY_TIME;
		TxUartMode = C_UART_SEND_INIT;
	}
	else if(C_UART_SEND_ING == TxUartMode)
	{
		//发送中不做处理

	}
	//else
	//	TxUartMode = C_UART_SEND_INIT;			


	if (g_u8RxSI_Cnt >= MS10_CNT)  
	{
		g_u8RxSI_Cnt -= MS10_CNT;
		if (RxUartTimeOut)
			RxUartTimeOut--;
	}
	
	if (UART_REC_INIT == RxUartMode) // 接收初始化
	{
		RxUartRecInit();
		RxCalRate = 0xff;
		RxUartTimeOut = 0xff;
		if (RX_REC_PIN())
			RxUartMode = C_UART_RECEIVE_ING; // RxUartMode 都必须最后赋值，一旦改变，发送就开始。
	}
	else if (C_UART_RECEIVE_ING == RxUartMode) //接收中
	{
		if (RxUartTimeOut == 0)
		{
			RxUartMode = C_UART_C_REC_END;
		}
	}
	else if (C_UART_C_REC_END == RxUartMode) // 接收完成
	{
	
		if (DECODE_FAIL == DecCode())
		{						// decode error
			RxUartMode = UART_REC_INIT;
		}
		else
		{ 
			RxUartMode = UART_REC_INIT;
		}
	}
	else
		RxUartMode = UART_REC_INIT;	

}


