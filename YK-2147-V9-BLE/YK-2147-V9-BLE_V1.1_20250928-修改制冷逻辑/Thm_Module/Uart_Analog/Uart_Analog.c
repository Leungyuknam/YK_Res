/*************************************************************
说明:
(1)使用定时器模拟UART通信，一般125us作为定时中断。
C_BIT_SAMPLE_CNT作为一个位抽样次数，一般8次。
(2)接收管脚无需外部中断，完全使用定时器采样替代中断。
可以1根线通信，也可以2根线通信。
(3)采用应答方式，主机先发，从机应答。
*************************************************************/
#include "COMMON\AllInclude.h"

//变量声明
       u8   xdata g_u8UartAnalogErro = 0;

static u8 	xdata g_u8UartBuf[C_BUFF_LENGTH];
static u8 	xdata r_u8UartTimeOut = C_SEND_DELAY_TIME;				//应答超时时间
static u8 	xdata r_u8SampleCnt = C_BIT_SAMPLE_CNT;
static u8 	xdata r_u8BitSection = C_START_BIT;
static u8 	xdata r_u8BitPhase = 0x01;
static u8 	xdata r_u8ByteNum = 0;
static u8 	xdata r_u8CurByte = 0;
static u8 	xdata r_u8BufLength = 0;
static u8 	xdata r_u8PinHighSum = 0;
static u8 	xdata r_u8SamplePhase = b0001_1100;
static u16 	xdata g_u16DisconnectCnt = 0;

#if(C_UART_SLAVE == C_DEVICE_TYPE)
static u8 	xdata r_u8UartMode = C_UART_C_REC_INIT;	//slave
#else					
static u8 	xdata r_u8UartMode = C_UART_SEND_INIT;    //master
#endif

#if Def_BoundRateCalFunEn
static u8 xdata r_u8BoundRate = 0;
static u8 xdata r_u8StatBitLength = 0;
#endif


unsigned char F_MathCheckSum(unsigned char *l_pu8Buf, unsigned char l_u8BufSize)
{
	unsigned char l_u8CheckSum = 0;
	unsigned char i = 0;
			   
	if (l_pu8Buf[0] != 101)
	{return 0;}
						
	if (l_pu8Buf[1] != 9)
	{return 0;}

	if (l_pu8Buf[6] != 0)
	{return 0;}

	if (l_pu8Buf[7] != 0)
	{return 0;}

	if (l_pu8Buf[8] != 0)
	{return 0;}
				
	for(i=0; i<l_u8BufSize; i++)
	{
		l_u8CheckSum += l_pu8Buf[i]; 
	}
	l_u8CheckSum += 0xAA;
	
    if (l_u8CheckSum == l_pu8Buf[9])
	{
		return 1;
	}

    return 0;
}

/*****************************************************
*函数名称：u8 F_DecCode(void)
*函数功能：串口接收数据校验解码
*输入参数：无
*输出参数：无
*调用周期：无
*说明：	   解码成功返回1，否则返回0
*****************************************************/
static u8 F_DecCode(void)
{
	static FILTER16_T xdata POWER_LITHIUM = {0,0,0};
	static u8 xdata g_u8LastUartBuf[C_BUFF_LENGTH];
	u8 i;

	if (F_MathCheckSum(&g_u8UartBuf,C_REC_LENGTH - 1))
	{
		g_u16DisconnectCnt = 0;

		//连续收到两帧相同的数据才更新
		for (i = 0; i < C_BUFF_LENGTH; i++)
		{
			if (g_u8LastUartBuf[i] != g_u8UartBuf[i])
			{
				for (i = 0; i < C_BUFF_LENGTH; i++)
				{g_u8LastUartBuf[i] = g_u8UartBuf[i];}
				return 1;
			}
		}

		for (i = 0; i < C_BUFF_LENGTH; i++)
		{g_u8LastUartBuf[i] = g_u8UartBuf[i];}		

		g_u16PowerValue = ((_MAKEWORD(g_u8UartBuf[2],g_u8UartBuf[3])) / 10);
		g_u8PowerMainValue  = g_u16PowerValue / 10;
		g_u8PowerSlaveValue = g_u16PowerValue % 10;

		if (F_FILTER_16Bit(g_u16PowerValue,1,&POWER_LITHIUM))
		{
			if (POWER_LITHIUM.u16OutValue >= LITHIUM_INCRESE_TABLE[g_u8BatteryLevel])
			{
				if (g_u8BatteryLevel < 5)
				{g_u8BatteryLevel++;}
			}
			else if (POWER_LITHIUM.u16OutValue <= LITHIUM_DECRESE_TABLE[g_u8BatteryLevel])
			{
				if (g_u8BatteryLevel > 0)
				{g_u8BatteryLevel--;}
			}
		}


		if ((_BTS(g_u8UartBuf[5],3)) || (_BTS(g_u8UartBuf[5],4)))
		{
			g_bChargeStatus = C_IN_CHARGE;	
		}
		else
		{
			g_bChargeStatus = C_NO_CHARGE;		
		}

		if (_BTS(g_u8UartBuf[5],5))
		{
			g_bBatteryType = C_BATTERY_LITHIUM;
		}
		else
		{
			g_bBatteryType = C_BATTERY_LEAD;
		}
		return 1;
	}
	else
	{
//		if (++g_u8ReceiveFailCnt > 250)
//		{
//			g_u8ReceiveFailCnt = 250;
//		}

		return 0;
	}
}

/*****************************************************
*函数名称：u8 F_EnCode(void)
*函数功能：串口发送数据填充
*输入参数：无
*输出参数：无
*调用周期：无
*说明：	   返回发送的字节数
*****************************************************/
static u8 F_EnCode(void)
{
//	u8 		i = 0;
//	u16		l_u16TempValue = 0;
//
//	g_u16DisconnectCnt = 0;
//
//	//清缓存
//	for(i = 0; i < C_BUFF_LENGTH; i ++)
//	{
//		g_u8UartBuf[i] = 0;
//	}
//
//	g_u8UartBuf[0]	=	g_u8PowerMainValue;	 
//	g_u8UartBuf[1]	=	(g_u8IntervalStatus << 7);
//	g_u8UartBuf[1]  |=  g_u8PowerSlaveValue;
//	g_u8UartBuf[2]	=	_LO8BIT(TEMPER_LBox.AD_Value);
//	g_u8UartBuf[3]	=	_LO8BIT(TEMPER_RBox.AD_Value);
//	g_u8UartBuf[4]	=	(_HI8BIT(TEMPER_LBox.AD_Value) << 4);
//	g_u8UartBuf[4]	|=	(_HI8BIT(TEMPER_RBox.AD_Value));
//	g_u8UartBuf[5]	=	(g_u8BatteryLevel);
//	g_u8UartBuf[6]	=	g_u8SysErroStatus;
//
//	l_u16TempValue  =   F_CHECKSUM_MemoryCalculate(&g_u8UartBuf,C_SEND_LENGTH);
//	g_u8UartBuf[7]	=	_HI8BIT(l_u16TempValue);
//	g_u8UartBuf[8]	=	_LO8BIT(l_u16TempValue);
	
	return C_SEND_LENGTH;
}


/*****************************************************
*函数名称：void F_UartSendInit(void)
*函数功能：串口发送初始化
*输入参数：无
*输出参数：无
*调用周期：无
*说明：	   
*****************************************************/
static void F_UartSendInit(void)
{
	r_u8BitSection = C_START_BIT;
	r_u8ByteNum = 0;
	r_u8SampleCnt = C_BIT_SAMPLE_CNT;
	Tx_Set_High();
	Tx_Set_Output();
}


/*****************************************************
*函数名称：void F_SendDrv(void)
*函数功能：串口发送驱动
*输入参数：无
*输出参数：无
*调用周期：125us
*说明：	   
*****************************************************/
static void F_SendDrv(void)
{
	r_u8SampleCnt >>= 1;
	if(r_u8SampleCnt == 0)
	{
		r_u8SampleCnt = C_BIT_SAMPLE_CNT;
		if(r_u8BitSection == C_START_BIT)
		{//起始位
			Tx_Set_Low();
			r_u8BitSection = C_DATA_BIT;
			r_u8BitPhase = 0x01;
			r_u8CurByte = g_u8UartBuf[r_u8ByteNum];
		}
		else if(r_u8BitPhase != 0)
		{// 8位数据
			if(r_u8CurByte & r_u8BitPhase)
			{
				Tx_Set_High();
			}
			else
			{
				Tx_Set_Low();
			}
			r_u8BitPhase <<= 1;
		}
		else
		{//结束位
			Tx_Set_High();
			if(r_u8BitSection == C_DATA_BIT)
			{
				r_u8ByteNum ++;
				if(r_u8ByteNum >= r_u8BufLength)
				{// 发送结束
					r_u8UartMode = C_UART_SEND_END;
					Rx_Set_Input();
					Rx_Pull_High();
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


/*****************************************************
*函数名称：void F_UartRecInit(void)
*函数功能：串口接收初始化
*输入参数：无
*输出参数：无
*调用周期：
*说明：	   
*****************************************************/
void F_UartRecInit(void)
{
	unsigned char i;

	Rx_Set_Input();
	Rx_Pull_High();
	r_u8BitSection = C_START_BIT;
	r_u8ByteNum = 0;
	r_u8SampleCnt = C_BIT_SAMPLE_CNT;

	for(i = 0; i < C_BUFF_LENGTH; i ++)
	{
		g_u8UartBuf[i] = 0;
	}
}


/*****************************************************
*函数名称：void F_RecDrv(void)
*函数功能：串口接收驱动
*输入参数：无
*输出参数：无
*调用周期：125us
*说明：	   
*****************************************************/
void F_RecDrv(void)
{
#if Def_BoundRateCalFunEn
	r_u8StatBitLength ++;
#endif 
	if(r_u8BitSection == C_START_BIT)
	{
		if(0 == GetRxPin())
		{
			r_u8BitSection = C_RE_START_BIT;
			r_u8PinHighSum = 0;
			r_u8SampleCnt = C_BIT_SAMPLE_CNT >> 1;
#if Def_BoundRateCalFunEn
			r_u8BoundRate = r_u8StatBitLength;
			r_u8StatBitLength = 0;
#endif		
			#if Def_TEST_BAOUNDRATE
			_TEST_BAOUNDRATE;
			#endif	
		}
	}
	else
	{
		if(GetRxPin())
		{
			if(r_u8SampleCnt & r_u8SamplePhase)
			{
				r_u8PinHighSum ++;

				#if Def_TEST_BAOUNDRATE
				_TEST_BAOUNDRATE;
				#endif
			}
		}	
		r_u8SampleCnt >>= 1;
		if(r_u8SampleCnt == 0)
		{
			r_u8SampleCnt = C_BIT_SAMPLE_CNT;
			if(r_u8BitSection == C_RE_START_BIT)
			{//start bit
				if(r_u8PinHighSum < C_SUM_HIGH)// low
				{// OK
					r_u8BitSection = C_DATA_BIT;	  
					r_u8CurByte = 0;
					r_u8BitPhase = 0x01;
					r_u8UartTimeOut = C_REC_INT_C_DECODE_TIME;
				}
				else
				{// FAIL
					r_u8BitSection = C_START_BIT;
				}
			}
			else if(r_u8BitPhase != 0)
			{// receice 8 bit
				if(r_u8PinHighSum >= C_SUM_HIGH)
				{// High
					r_u8CurByte |= r_u8BitPhase; 
				}
				r_u8BitPhase <<= 1;
			}
			else 
			{//stop bit
				r_u8BitSection = C_START_BIT;
				g_u8UartBuf[r_u8ByteNum] = r_u8CurByte;
				r_u8ByteNum ++;
				if(r_u8ByteNum >= C_BUFF_LENGTH)
				{//Fail finhish
					r_u8UartMode = C_UART_C_REC_END;
					
				}

			#if Def_TEST_BAOUNDRATE
				_CLEAR_BAOUNDRATE;
			#endif

			}
			r_u8PinHighSum = 0;
		}
	}

}


/*****************************************************
*函数名称：void F_UART_Analog_IRQ(void)
*函数功能：串口中断处理程序
*输入参数：无
*输出参数：无
*调用周期：125us
*说明：	   
*****************************************************/
void F_UART_Analog_IRQ(void)
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


/*****************************************************
*函数名称：void F_Calr_u8BoundRate(void)
*函数功能：串口波特率校准
*输入参数：无
*输出参数：无
*调用周期：无
*说明：	   
*****************************************************/
#if Def_BoundRateCalFunEn
static void F_Calr_u8BoundRate(void)
{
	if(r_u8BoundRate >= 88 + 7)
	{
		r_u8SamplePhase = b0011_1000;
	}
	else if(r_u8BoundRate >= 88 + 3)
	{
		r_u8SamplePhase = b0000_0111;
	}
	else if(r_u8BoundRate >= 88 - 3)
	{
		r_u8SamplePhase = b0011_1000;
	}
	else if(r_u8BoundRate >= 88 - 7)
	{
		r_u8SamplePhase = b1110_0000;
	}
	else
	{
		r_u8SamplePhase = b0011_1000;
	}
	r_u8StatBitLength = 0;
}
#endif


/*****************************************************
*函数名称：void F_UART_Analog_Protocol(void)
*函数功能：串口协议处理
*输入参数：无
*输出参数：无
*调用周期：无
*说明：	   
*****************************************************/
void F_UART_Analog_Protocol(void)
{	
	if(C_UART_C_REC_INIT == r_u8UartMode)// 接收初始化				
	{
#if Def_BoundRateCalFunEn
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
#if Def_ONLY_RX
		F_DecCode();
		r_u8UartMode = C_UART_C_REC_INIT;
#else
		if(C_DECODE_FAIL == F_DecCode())
		{// decode error
		#if(C_UART_SLAVE == C_DEVICE_TYPE)//slave
			r_u8UartMode = C_UART_C_REC_INIT;	
		#else//master
			r_u8UartTimeOut = C_SEND_DELAY_TIME;						
			r_u8UartMode = C_UART_SEND_INIT;
		#endif
		}
		else
		{// decode OK
			r_u8UartTimeOut = C_SEND_DELAY_TIME;						
			r_u8UartMode = C_UART_SEND_INIT;			
		}
#endif
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
#if Def_ONLY_TX
		r_u8UartMode = C_UART_SEND_INIT;
		r_u8UartTimeOut = 10;
#else
		r_u8UartMode = C_UART_C_REC_INIT;
#endif
	}
	else //if(C_UART_SEND_ING == r_u8UartMode)
	{// 发送中
		;
	}
}


/*****************************************************
*函数名称：void F_UART_Analog_Init(void)
*函数功能：串口初始化
*输入参数：无
*输出参数：无
*调用周期：无
*说明：	   
*****************************************************/
void F_UART_Analog_Init(void)
{
#if(C_UART_SLAVE == C_DEVICE_TYPE)//slave
		r_u8UartMode = C_UART_C_REC_INIT;	
#else//master
		r_u8UartTimeOut = C_SEND_DELAY_TIME;						
		r_u8UartMode = C_UART_SEND_INIT;
#endif
}


/*****************************************************
*函数名称：void F_UART_Analog_TimeOut(void)
*函数功能：串口超时计时
*输入参数：无
*输出参数：无
*调用周期：125us
*说明：	   
*****************************************************/
void F_UART_Analog_TimeOut(void)
{
	if(r_u8UartTimeOut)
	{
		r_u8UartTimeOut--;
	}

//	if (++g_u16DisconnectCnt > 120)
//	{
//		if (g_u16DisconnectCnt > 65500)
//		{g_u16DisconnectCnt = 65500;}
//
//		g_u8UartAnalogErro = 1;
//		g_bBatteryType = C_BATTERY_LEAD;	
//	}
//	else
//	{
//		g_u8UartAnalogErro = 0;
//	}

	if (++g_u16DisconnectCnt > 9600)
	{
		if (g_u16DisconnectCnt > 65500)
		{g_u16DisconnectCnt = 65500;}

		g_u8UartAnalogErro = 1;
		g_bBatteryType = C_BATTERY_LEAD;	
	}
	else
	{
		g_u8UartAnalogErro = 0;
	}

}

