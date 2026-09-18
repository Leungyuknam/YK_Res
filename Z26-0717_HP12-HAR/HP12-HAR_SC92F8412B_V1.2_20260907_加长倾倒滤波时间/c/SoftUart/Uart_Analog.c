/*************************************************************
说明:
(1)使用定时器模拟UART通信，一般125us作为定时中断。
C_BIT_SAMPLE_CNT作为一个位抽样次数，一般8次。
(2)接收管脚无需外部中断，完全使用定时器采样替代中断。
可以1根线通信，也可以2根线通信。
(3)采用应答方式，主机先发，从机应答。


//作者：陈首敏
*************************************************************/
	
#include "h\Allinclude.h"

#define		C_UART_C_REC_INIT		0
#define 	C_UART_RECEIVE_ING		1
#define 	C_UART_C_REC_END		2
#define 	C_UART_SEND_INIT		3
#define 	C_UART_SEND_ING			4
#define 	C_UART_SEND_END			5

#define		C_BIT_SAMPLE_CNT		0x80
#define		C_SUM_HIGH				2

#define 	C_START_BIT				0
#define 	C_RE_START_BIT			1
#define 	C_DATA_BIT				2
#define 	C_STOP_BIT				3
#define 	C_DOUBLE_STOP_BIT		4

#define		r_u8BoundRateCalFunEn		1	

unsigned char xdata g_u8UartBuf[C_BUF_LENGTH];


#if(C_UART_SLAVE == C_DEVICE_TYPE)
unsigned char xdata r_u8UartMode = C_UART_C_REC_INIT;	// 从机
#else					
unsigned char xdata r_u8UartMode = C_UART_SEND_INIT;  // 主机
#endif
unsigned char xdata r_u8UartTimeOut = C_SEND_DELAY_TIME;				//应答超时时间
unsigned char xdata r_u8SampleCnt = C_BIT_SAMPLE_CNT;
unsigned char xdata r_u8BitSection = C_START_BIT;
unsigned char xdata r_u8BitPhase = 0x01;
unsigned char xdata r_u8ByteNum = 0;
unsigned char xdata r_u8CurByte = 0;
unsigned char xdata r_u8BufLength = 0;
unsigned char xdata r_u8PinHighSum = 0;
unsigned char xdata r_u8SamplePhase = 0x38;//b0011_1000;

#if r_u8BoundRateCalFunEn
unsigned char xdata r_u8BoundRate = 0;
unsigned char xdata r_u8StatBitLength = 0;
#endif

unsigned char F_DecCode(void)
{
	unsigned char Result;
	Result = DeCodeProc(r_u8ByteNum);
	return Result;

}

unsigned char F_EnCode(void)
{
	unsigned char i;
	for(i = 0; i < C_BUF_LENGTH; i ++)
	{
		g_u8UartBuf[i] = 0;
	}
	i = MakeEnCode();
	return i;
}

void F_UartSendInit(void)
{
	r_u8BitSection = C_START_BIT;
	r_u8ByteNum = 0;
	r_u8SampleCnt = C_BIT_SAMPLE_CNT;
	Pin_Set_High();
	Pin_Set_Output();
}

void F_SendDrv(void)
{
	r_u8SampleCnt >>= 1;
	if(r_u8SampleCnt == 0)
	{
		r_u8SampleCnt = C_BIT_SAMPLE_CNT;
		if(r_u8BitSection == C_START_BIT)
		{//起始位
			Pin_Set_Low();
			r_u8BitSection = C_DATA_BIT;
			r_u8BitPhase = 0x01;
			r_u8CurByte = g_u8UartBuf[r_u8ByteNum];
		}
		else if(r_u8BitPhase != 0)
		{// 8位数据
			if(r_u8CurByte & r_u8BitPhase)
			{
				Pin_Set_High();
			}
			else
			{
				Pin_Set_Low();
			}
			r_u8BitPhase <<= 1;
		}
		else
		{//结束位
			Pin_Set_High();
			if(r_u8BitSection == C_DATA_BIT)
			{
				r_u8ByteNum ++;
				if(r_u8ByteNum >= r_u8BufLength)
				{// 发送结束
					r_u8UartMode = C_UART_SEND_END;
					Pin_Set_Input();
					Pin_PullL_High();
				}
				else
				{
					r_u8BitSection = C_DOUBLE_STOP_BIT;
				}
			}
			else
			{// 发送结束位
				r_u8BitSection = C_START_BIT;
			}
		}
		
	}
}

void F_UartRecInit(void)
{
	unsigned char i;

	Pin_Set_Input();
	Pin_PullL_High();
	r_u8BitSection = C_START_BIT;
	r_u8ByteNum = 0;
	r_u8SampleCnt = C_BIT_SAMPLE_CNT;
	for(i = 0; i < C_BUF_LENGTH; i ++)
	{
		g_u8UartBuf[i] = 0;
	}
}

void F_RecDrv(void)
{
#if r_u8BoundRateCalFunEn
	r_u8StatBitLength ++;
#endif 
	if(r_u8BitSection == C_START_BIT)
	{
		if(0 == GetRecPin())
		{
			r_u8BitSection = C_RE_START_BIT;
			r_u8PinHighSum = 0;
			r_u8SampleCnt = C_BIT_SAMPLE_CNT >> 1;
#if r_u8BoundRateCalFunEn
			r_u8BoundRate = r_u8StatBitLength;
			r_u8StatBitLength = 0;
#endif			
		}
	}
	else
	{
		if(GetRecPin())
		{
			if(r_u8SampleCnt & r_u8SamplePhase)
			{
				r_u8PinHighSum ++;
			}
		}	
		r_u8SampleCnt >>= 1;
		if(r_u8SampleCnt == 0)
		{
			r_u8SampleCnt = C_BIT_SAMPLE_CNT;
			if(r_u8BitSection == C_RE_START_BIT)
			{// 起始位
				if(r_u8PinHighSum < C_SUM_HIGH)// 低电平
				{// 正常
					r_u8BitSection = C_DATA_BIT;
					r_u8CurByte = 0;
					r_u8BitPhase = 0x01;
					r_u8UartTimeOut = C_REC_INT_C_DECODE_TIME;
				}
				else
				{// 失败
					r_u8BitSection = C_START_BIT;
				}
			}
			else if(r_u8BitPhase != 0)
			{// 接收 8 位数据
				if(r_u8PinHighSum >= C_SUM_HIGH)
				{// 高电平
					r_u8CurByte |= r_u8BitPhase; 
				}
				r_u8BitPhase <<= 1;
			}
			else 
			{// 停止位
				r_u8BitSection = C_START_BIT;
				g_u8UartBuf[r_u8ByteNum] = r_u8CurByte;
				r_u8ByteNum ++;
				if(r_u8ByteNum >= C_BUF_LENGTH)
				{// 接收失败并结束
					r_u8UartMode = C_UART_C_REC_END;
				}
			}
			r_u8PinHighSum = 0;
		}
	}
}

void F_Uart_Analog_IRQ(void)
{
	if(C_UART_SEND_ING == r_u8UartMode)
	{
		F_SendDrv();
	}
	else if(C_UART_RECEIVE_ING == r_u8UartMode)
	{
		F_RecDrv();
	}	
}

#if r_u8BoundRateCalFunEn
void F_Calr_u8BoundRate(void)
{
	if(r_u8BoundRate >= 88 + 7)
	{
		r_u8SamplePhase = 0x38;//b0011_1000;
	}
	else if(r_u8BoundRate >= 88 + 3)
	{
		r_u8SamplePhase = 0x07;//b0000_0111;
	}
	else if(r_u8BoundRate >= 88 - 3)
	{
		r_u8SamplePhase = 0x38;//b0011_1000;
	}
	else if(r_u8BoundRate >= 88 - 7)
	{
		r_u8SamplePhase = 0xe0;//b1110_0000;
	}
	else
	{
		r_u8SamplePhase = 0x38;//b0011_1000;
	}
	r_u8StatBitLength = 0;
}
#endif
/*********************************************************************
//UART接收和发送协议处理 10MS
*********************************************************************/
void F_UartProtocol(void)
{	

		if(r_u8UartTimeOut)
		{
			r_u8UartTimeOut --;
		}


	if(C_UART_C_REC_INIT == r_u8UartMode)// 接收初始化				
	{
#if r_u8BoundRateCalFunEn
		F_Calr_u8BoundRate(); 	
#endif
		F_UartRecInit();
		r_u8UartTimeOut = C_REC_NO_DATA_TIME;
		r_u8UartMode = C_UART_RECEIVE_ING;//r_u8UartMode 都必须最后赋值，一旦改变，发送就开始。
	}
	else if(C_UART_RECEIVE_ING == r_u8UartMode)//接收中
	{
		if( r_u8UartTimeOut == 0)	
		{
			r_u8UartMode = C_UART_C_REC_END;
		}
	}
	else if(C_UART_C_REC_END == r_u8UartMode)// 接收完成
	{		
		if(C_DECODE_FAIL == F_DecCode())
		{// 解码错误
		#if(C_UART_SLAVE == C_DEVICE_TYPE)// 从机
			r_u8UartMode = C_UART_C_REC_INIT;	
		#else// 主机
			r_u8UartTimeOut = C_SEND_DELAY_TIME;						
			r_u8UartMode = C_UART_SEND_INIT;
		#endif
		}
		else
		{// 解码成功
			r_u8UartTimeOut = C_SEND_DELAY_TIME;						
			r_u8UartMode = C_UART_SEND_INIT;			
		}
	}
	else if(C_UART_SEND_INIT == r_u8UartMode)//发送初始化
	{		
		if(0 == r_u8UartTimeOut)
		{
			r_u8BufLength = F_EnCode();
			if(r_u8BufLength != 0)
			{
				F_UartSendInit();
				r_u8UartMode = C_UART_SEND_ING;
			}
		}
	}
	else if(C_UART_SEND_END == r_u8UartMode) //发送结束
	{
		r_u8UartMode = C_UART_C_REC_INIT;
	}
	else //if(C_UART_SEND_ING == r_u8UartMode)
	{// 发送中
		;
	}
}

void F_UartInit(void)
{
#if(C_UART_SLAVE == C_DEVICE_TYPE)// 从机
		r_u8UartMode = C_UART_C_REC_INIT;	
#else// 主机
		r_u8UartTimeOut = C_SEND_DELAY_TIME;						
		r_u8UartMode = C_UART_SEND_INIT;
#endif
}

