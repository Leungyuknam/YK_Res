/*********************************************************
文 件 名 : EUSART.C
文件说明 : 串口处理文件
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-6
版 本 号 : V1.0
==========================================================*/
#include "COMMON\AllInclude.h"
#include "EUSART\EUSART_C.h"

//变量声明
//		 u8  g_u8SendFlag = 0;
static u8  g_u8SendStatus = C_SEND_DONE;
static u8  g_u8ReceiveTimeOut = 0;
static volatile u8  g_u8ReceiveCnt = 0;

static u8    g_u8ReceiveStatus = C_REC_ENABLE;
static u8    g_u8SendCnt = 1;
static u8    g_u8NeedSendLength = 0;
static u8    g_u8SendTimeOut = 0;
static u8    g_u8AskTime = 0;

static u8 xdata SEND_TABLE[C_SEND_MAX_NUM] = {0};
static u8 xdata RECEIVE_TABLE[C_RECEIVE_MAX_NUM] = {0};

/*****************************************************
*函数名称：void F_TestAtDataSend(void)
*函数功能：利用AT指令检测蓝牙通讯是否正常
*输入参数：无
*输出参数：无
*调用周期：
*说明：
*****************************************************/
void F_TestAtDataSend(void)
{
	SEND_TABLE[0] = 'A';
 	SEND_TABLE[1] = 'T';
	SEND_TABLE[2] = '+';
	SEND_TABLE[3] = 'I'; 
	SEND_TABLE[4] = 'N';
	SEND_TABLE[5] = 'F';
	SEND_TABLE[6] = 'O';
	SEND_TABLE[7] = '=';
	SEND_TABLE[8] = '?';

	g_u8NeedSendLength = 9;
	REGISTER_UART = SEND_TABLE[0];
}

/*****************************************************
*函数名称：void F_CheckAtReceiveData(void)
*函数功能：校验蓝牙自检数据
*输入参数：无
*输出参数：无
*调用周期：
*说明：
*****************************************************/
void F_CheckAtReceiveData(void)
{
	if(RECEIVE_TABLE[0] != 'A')	{	return;	} 	
	if(RECEIVE_TABLE[1] != 'T')	{	return;	}
	if(RECEIVE_TABLE[2] != '+')	{	return;	}	
	if(RECEIVE_TABLE[3] != 'O')	{	return;	}	
	if(RECEIVE_TABLE[4] != 'K')	{	return;	}

	g_bBlueStatus = 1;
}

/*****************************************************
*函数名称：void F_DATA_DownLoad(void)
*函数功能：下载APP数据
*输入参数：无
*输出参数：无
*调用周期：
*说明：
*****************************************************/
void F_DATA_DownLoad(void)
{
							
}

/*****************************************************
*函数名称：u16 F_CHECKSUM_Calculate(u8 *pTable,u8 l_u8TableLength)
*函数功能：校验和计算
*输入参数：无
*输出参数：无
*调用周期：
*说明：	   校验和额外值0xAA
*****************************************************/
u16 F_CHECKSUM_Calculate(u8 *pTable,u8 l_u8TableLength)
{
	u8	 i = 0;
	u16  l_u16CheckSum = 0;


	for (i = 0; i < (l_u8TableLength - 2); i++)
	{
		l_u16CheckSum += pTable[i];
	}

#if Def_ExtraCal
	l_u16CheckSum += 0xAA;
#endif

	return l_u16CheckSum;
}

/*****************************************************
*函数名称：u8 F_CHECKOUT_ReadData(u8 *pTable,u8 l_u8TableLength)
*函数功能：校验读取的数据
*输入参数：无
*输出参数：校验成功返回1，否则返回0
*调用周期：
*说明：
*****************************************************/
u8 F_CHECKOUT_ReadData(u8 *pTable,u8 l_u8TableLength)
{
	u16  l_u16CheckSum = F_CHECKSUM_Calculate(pTable,l_u8TableLength);

	if ((pTable[l_u8TableLength - 2] == (_HI8BIT(l_u16CheckSum))) &&
	    (pTable[l_u8TableLength - 1] == (_LO8BIT(l_u16CheckSum))))
    {
   		return 1;
    }

	return 0;
}

/*****************************************************
*函数名称：void UART0Interrupt()  interrupt 4
*函数功能：串口中断函数
*输入参数：无
*输出参数：无
*调用周期：
*说明：
*****************************************************/
void UART0Interrupt()		interrupt 4		
{
    static u8  s_u8UartReceiveBuff = 0;
	static u8  s_u8ReceiveLength = 0;

	//发送中断
	if (_TEST_UART_TI)
	{
		_CLR_UART_TI;
		
		if (g_u8SendCnt < g_u8NeedSendLength)
		{
			REGISTER_UART = SEND_TABLE[g_u8SendCnt];

			if (++g_u8SendCnt == g_u8NeedSendLength)
			{
				g_u8SendStatus = C_SEND_DELAY;
				g_u8SendTimeOut = C_SEND_DELAYTIME;
				g_u8SendCnt = 1;
				g_u8NeedSendLength = 0;	
			}
		}
		else
		{
			g_u8SendStatus = C_SEND_DELAY;
			g_u8SendTimeOut = C_SEND_DELAYTIME;
			g_u8SendCnt = 1;
			g_u8NeedSendLength = 0;			
		}							
	}	


	//接收中断
	if (_TEST_UART_RI)
	{
		_CLR_UART_RI;
		
		s_u8UartReceiveBuff = REGISTER_UART;

		 if(g_u8ReceiveStatus == C_REC_ENABLE)
		 {
			 g_u8ReceiveTimeOut = C_RECEIVE_TIMEOUT;
				
			 if( g_u8ReceiveCnt < C_RECEIVE_MAX_NUM )
			 {
				 RECEIVE_TABLE[g_u8ReceiveCnt] = s_u8UartReceiveBuff;

				 if (!g_bBlueStatus)
				 {
				 	if (g_u8ReceiveCnt >= 5)
					{
						g_u8ReceiveStatus = C_REC_DISABLE;
					}				 	
				 }

				 else
				 {
					 //接收结束判断
					 if (g_u8ReceiveCnt == 0)		   //校验头码
					 {
					 	if (RECEIVE_TABLE[0] != 0xFE)
						{
							return ;
						}
					 }
					 else if (g_u8ReceiveCnt == 1)	   //校验头码
					 {
					 	if (RECEIVE_TABLE[1] != 0xFE)
						{
							g_u8ReceiveCnt = 0;
							return ;
						}
					 }
					 else if (g_u8ReceiveCnt == 2) 	   //接收字长
					 {
					 	s_u8ReceiveLength = (RECEIVE_TABLE[2] + 2);
					 }
					 else							   //结束判断
					 {
					 	if (g_u8ReceiveCnt >= s_u8ReceiveLength)
						{
							g_u8ReceiveStatus = C_REC_DISABLE;
						}
					 }
				 }				
			 	 g_u8ReceiveCnt++;
			 }
		 }
	}		
}

/*****************************************************
*函数名称：void F_ClearReadFrameData(void)
*函数功能：清接收缓存区
*输入参数：无
*输出参数：无
*调用周期：
*说明：
*****************************************************/
void F_ClearReadFrameData(void)
{
     u8 xdata i;

	 for( i = 0 ; i < C_RECEIVE_MAX_NUM ; i++)
	 {
		  RECEIVE_TABLE[i] = 0;
	 }
	 g_u8ReceiveCnt = 0;
	 g_u8ReceiveStatus = C_REC_ENABLE;
}


/*****************************************************
*函数名称：void F_OneFrameDataReceived(void)
*函数功能：接收数据处理
*输入参数：无
*输出参数：无
*调用周期：
*说明：
*****************************************************/
void F_OneFrameDataReceived(void)
{
	if (F_CHECKOUT_ReadData(RECEIVE_TABLE,(RECEIVE_TABLE[2] + 3)))
	{
		switch (RECEIVE_TABLE[3])
		{
			case C_TYPE_BINGDING:
			{
				if ((g_u8BingDingFlag != C_BINGDING_ASK))
				{
					g_u8BingDingFlag = C_BINGDING_ASK;
					_BUZZER_SHORT_BEEP;
					g_u8AskTime = 15;
				}				
				break;
			}

			case C_TYPE_CHECK:
			{
				_BS(g_u8SendFlag,SEND_CHECK_);
				break;
			}
						
			case C_TYPE_SET:
			{
				F_DATA_DownLoad();
				_BUZZER_SHORT_BEEP;
			    _BS(g_u8SendFlag,SEND_SET_);
				break;
			}

			case C_TYPE_RESET:
			{
				g_u8ModeSet = C_MODESET_LOWESET_TEMPER;
				F_ParameterReset();
				_BS(g_u8SendFlag,SEND_RESET_);
				break;
			}

			case C_TYPE_LBOX_SET:
			{
				_BUZZER_SHORT_BEEP;
				g_u8CtrlTemper_LBox = RECEIVE_TABLE[4];
//				F_BUFF_ModeSet(C_MODESET_CTRL_TEMPER_LBOX);
				_BS(g_u8SendFlag,SEND_LBOX_SET_);

#if Def_DoubleBox
				if (g_u8CtrlTemper_LBox > g_u8CtrlTemper_RBox)
				{
					g_u8CtrlTemper_RBox = g_u8CtrlTemper_LBox;	
				}
#endif

				break;
			}

#if Def_DoubleBox
			case C_TYPE_RBOX_SET:
			{
				_BUZZER_SHORT_BEEP;
				g_u8CtrlTemper_RBox = RECEIVE_TABLE[4];
//				F_BUFF_ModeSet(C_MODESET_CTRL_TEMPER_RBOX);
				_BS(g_u8SendFlag,SEND_RBOX_SET_);

				if (g_u8CtrlTemper_RBox < g_u8CtrlTemper_LBox)
				{
					g_u8CtrlTemper_RBox = g_u8CtrlTemper_LBox;
				}
				break;
			}
#endif

			default: break;												 
		}
	}

//	else
//	{
//		if (++g_u8UartRecErro > 250)
//		{g_u8UartRecErro = 250;}
//	}

}

/*****************************************************
*函数名称：void F_OneFrameDataSend(u8 l_u8SendType)
*函数功能：发送数据处理
*输入参数：无
*输出参数：无
*调用周期：
*说明：
*****************************************************/
void F_OneFrameDataSend(u8 l_u8SendType)
{


}

/*****************************************************
*函数名称：void F_UART_ReceiveTimeout(void)
*函数功能：串口接收超时计算
*输入参数：无
*输出参数：无
*调用周期：10mS
*说明：
*****************************************************/
void F_UART_ReceiveTimeout(void)
{
	if (g_u8ReceiveTimeOut > 0)
	{
		if (--g_u8ReceiveTimeOut == 0)
		{
			 g_u8ReceiveStatus = C_REC_DISABLE;

			 if (!g_bBlueStatus)
			 {F_CheckAtReceiveData();}
			 else
			 {F_OneFrameDataReceived();}

			 F_ClearReadFrameData();
		}		
	}
}

/*****************************************************
*函数名称：void F_UART_SendDelay(void)
*函数功能：串口发送延时时计算
*输入参数：无
*输出参数：无
*调用周期：10mS
*说明：
*****************************************************/
void F_UART_SendDelay(void)
{
	u8 i = 0;

	if (g_u8SendTimeOut > 0)
	{
		if (--g_u8SendTimeOut == 0)
		{
			g_u8SendStatus = C_SEND_DONE;
		}		
	}
}
/*****************************************************
*函数名称：void F_RECEIVE_Control(void)
*函数功能：串口接收处理
*输入参数：无
*输出参数：无
*调用周期：10ms
*说明：
*****************************************************/
void F_RECEIVE_Control(void)
{	
	 if	(g_u8ReceiveStatus == C_REC_DISABLE)
	 {
		 g_u8ReceiveTimeOut = 0;

		 if (!g_bBlueStatus)
		 {F_CheckAtReceiveData();}
		 else
		 {F_OneFrameDataReceived();}
		 F_ClearReadFrameData();		 
	 }	
}

/*****************************************************
*函数名称：void F_SEND_Control(void)
*函数功能：串口发送处理
*输入参数：无
*输出参数：无
*调用周期：10ms
*说明：
*****************************************************/
void F_SEND_Control(void)
{
	if (g_u8SendStatus == C_SEND_DONE)
	{
		//绑定回复
		if (_BTS(g_u8SendFlag,SEND_BINGDING_))
		{
			g_u8SendStatus = C_SEND_ING;
			F_OneFrameDataSend(C_FRAME_BINGDING);
			_BC(g_u8SendFlag,SEND_BINGDING_);
		}
	
		//查询设置回复
		else if (_BTS(g_u8SendFlag,SEND_CHECK_))
		{
			g_u8SendStatus = C_SEND_ING;
			F_OneFrameDataSend(C_FRAME_NACK_CHECK);
			_BC(g_u8SendFlag,SEND_CHECK_);		
		}

		//设置回复
		else if (_BTS(g_u8SendFlag,SEND_SET_))
		{
			g_u8SendStatus = C_SEND_ING;

			F_OneFrameDataSend(C_FRAME_NACK_SET);
			_BC(g_u8SendFlag,SEND_SET_);			
		}

		//恢复出厂设置回复
		else if (_BTS(g_u8SendFlag,SEND_RESET_))
		{
			g_u8SendStatus = C_SEND_ING;
		
			F_OneFrameDataSend(C_FRAME_NACK_RESET);
			_BC(g_u8SendFlag,SEND_RESET_);		
		}

		//左箱温度设置回复
		else if (_BTS(g_u8SendFlag,SEND_LBOX_SET_))
		{
			g_u8SendStatus = C_SEND_ING;

			F_OneFrameDataSend(C_FRAME_LBOX_TEMP);
			_BC(g_u8SendFlag,SEND_LBOX_SET_);		
		}

#if  Def_DoubleBox
		//右箱温度设置回复
		else if (_BTS(g_u8SendFlag,SEND_RBOX_SET_))
		{
			g_u8SendStatus = C_SEND_ING;

			F_OneFrameDataSend(C_FRAME_RBOX_TEMP);
			_BC(g_u8SendFlag,SEND_RBOX_SET_);		
		}
#endif

		//蓝牙检测
		else if (_BTS(g_u8SendFlag,SEND_BLUE_CHECK_))
		{
			g_u8SendStatus = C_SEND_ING;

			F_TestAtDataSend();
			_BC(g_u8SendFlag,SEND_BLUE_CHECK_);		
		}

		else
		{

		}
	}
}



/*****************************************************
*函数名称：void F_TIME_Ask(void)
*函数功能：APP接入计时
*输入参数：无
*输出参数：无
*调用周期：1s
*说明：
*****************************************************/
void F_TIME_Ask(void)
{
	if (g_u8BingDingFlag == C_BINGDING_ASK)
	{
		if (g_u8AskTime)
		{
			if (--g_u8AskTime == 0)
			{
				g_u8BingDingFlag = C_BINGDING_IDLE;	
			}
		}
	}
	else
	{
		g_u8AskTime = 0;	
	}
}


/*****************************************************
*函数名称：void F_APP_Request(void)
*函数功能：APP请求
*输入参数：无
*输出参数：无
*调用周期：100ms
*说明：	   如未收到蓝牙模块的AT回复，间隔1秒发一次AT指令
*****************************************************/
void F_APP_Request(void)
{
	static u8 xdata s_u8Time1sCnt = 1;	

	if ((g_bBlueStatus == 0) && ((g_u8RunTime100mS > 2) && (g_u8RunTime100mS < 100)))
	{
		if (s_u8Time1sCnt > 0)
		{
			if (--s_u8Time1sCnt == 0)
			{
				_BS(g_u8SendFlag,SEND_BLUE_CHECK_);
				s_u8Time1sCnt = 10;
			}
		}
	}
}





















