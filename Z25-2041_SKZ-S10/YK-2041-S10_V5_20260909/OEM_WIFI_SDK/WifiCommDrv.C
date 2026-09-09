#define UartDrv
//#include "Define.h"
#include "h\Allinclude.h"
/***************************************************************
FUNCTION NAME:EUART0_ISP                          
FUNCTION:中断接收子程序及发送处理
INPUT:	null
RETURN:	null			                             
***************************************************************/
//void EUART0_ISP(void) interrupt INT_VECTOR_UART0
//{ 
//	uchar data ucTemp;
//	Push_Context();
//	if(0 != UART0_SEND_REQUEST_FLAG)
//	{
//		UART0_SEND_REQUEST_FLAG =0;
//		if(UART_SENDING == ucSendState)	//在发送状态中
//		{
//			if(ucSendCountWifi >= ucSendTotalCnt)
//			{
//				ucSendCountWifi = 0;
//				ucSendState = UART_SEND_END;
//			}
//			else
//			{
//				WifiUartSendByte(ucSendBufWifi[ucSendCountWifi]);
//				ucSendCountWifi++;
//			}
//		}
//	}

//	if(0 != UART0_RECE_REQUEST_FLAG)
//	{
//		UART0_RECE_REQUEST_FLAG = 0;
//		WifisUartGetByte(ucTemp);
//		if(UART_RECING == ucReceState)//在接收状态中  //正常数据接收处理	
//		{
//			if(ucRecCountWifi < U8_UART_LENGTH)	//保证缓冲区不会溢出
//			{
//				ucRecBufWifi[ucRecCountWifi] = ucTemp;
//				ucRecCountWifi++;
//			}
//			ucWifiRecFinishTime = U8_T30MS;	//30ms没有数据的时候，证明接收完成，进行解码
//			ucWifiNoRecDataInt = U8_T30S;	//连续一段时间没有接收到串口数据就信息接收初始化处理
//			ucWifiRecDataTime = U8_UART_DELAY_TIME_1000ms;//接收到数据后，计时1s。连续1s没有收到数据，进行串口接收初始化	
//		}
//	}
//	Pop_Context();
//}



void EUART0_ISP(void) interrupt 4
{ 
	uchar    ucTemp;
	//-----------------------------------------------------------
	// 压栈保护
    _push_(INSCON);
//	INSCON = 0;
	INSCON &= 0xBF;
    _push_(FLASHCON);
    FLASHCON = 0;
	
	//-----------------------------------------------------------
//	UART0_INT_FLAG_CLR;//清除UART0中断标志 
//	UART0_INT_IF_RO_CLR(0);
//	UART0_INT_IF_F_CLR(0);
//	UART0_INT_IF_P_CLR(0);
	//---------收发数据处理---------------
	if(UART0_INT_CLR_TF)	//发送中断有效
	{
		UART0_INT_IF_TF_CLR();//清除发送中断标志
		if(UART_SENDING == ucSendState)		//在发送状态中
		{
			if(ucSendCountWifi >= ucSendTotalCnt)
			{
				ucSendCountWifi = 0;
				ucSendState = UART_SEND_END;
			}
			else
			{
				SetUART0SBUF(ucSendBufWifi[ucSendCountWifi]);
				ucSendCountWifi++;
			}
		}
	}

	if(UART0_INT_CLR_RF)	//接收中断有效
	{
		UART0_INT_IF_RF_CLR();//清除接收中断标志位
		GetUART0SBUF(ucTemp);
		if(UART_RECING == ucReceState)		//在接收状态中  //正常数据接收处理	
		{
			if(ucRecCountWifi < U8_UART_LENGTH)				//保证缓冲区不会溢出
			{
				ucRecBufWifi[ucRecCountWifi] = ucTemp;
				ucRecCountWifi++;
			}
			ucWifiRecFinishTime = U8_T30MS;				//30ms没有数据的时候，证明接收完成，进行解码
			ucWifiNoRecDataInt = U8_T30S;				//连续一段时间没有接收到串口数据就信息接收初始化处理
			ucWifiRecDataTime = U8_UART_DELAY_TIME_1000ms;//接收到数据后，计时1s。连续1s没有收到数据，进行串口接收初始化	
		}
	}
	//-----------------------------------------------------------
	// 出栈
//    INSCON = 0;
	INSCON &= 0xBF;
	_pop_(FLASHCON);
    _pop_(INSCON);
}
/*******************************************************************
							END																															
*******************************************************************/
