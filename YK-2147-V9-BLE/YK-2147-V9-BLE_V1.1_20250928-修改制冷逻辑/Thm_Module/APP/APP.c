/*********************************************************
文 件 名 : APP.C
文件说明 : APP串口处理文件
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-6
版 本 号 : V1.0
==========================================================*/
#include "COMMON\AllInclude.h"
#include "APP\APP_C.h"

//变量声明
static u8    g_u8SendStatus = C_SEND_DONE;
static u8    g_u8ReceiveTimeOut = 0;
static volatile u8  g_u8ReceiveCnt = 0;

static u8    g_u8ReceiveStatus = C_REC_ENABLE;
static u8    g_u8SendCnt = 1;
static u8    g_u8NeedSendLength = 0;
static u8    g_u8SendDelayTime = 0;
static u8    g_u8SendTimeOut = 0;

static u8 xdata RECEIVE_TABLE[C_APP_RECEIVE_MAX_NUM] = {0};

static u8 xdata g_u8DisConnectCnt = 0;

/*****************************************************
*函数名称：u8 F_BAT_ChecekSumCal(u8 *pTable, u8 l_u8Length)
*函数功能：校验和计算
*输入参数：无
*输出参数：无
*调用周期：
*说明：	    
*****************************************************/
u8 F_BAT_ChecekSumCal(u8 *pTable, u8 l_u8Length)
{
	unsigned char c;
	unsigned char cCrc = 0xff;
	 
	while(l_u8Length--)
	{
		cCrc = cCrc ^ (*pTable);
		for(c = 8; c > 0; c--)
		{
			if(cCrc & 0x80) 
			{
				cCrc = (cCrc<<1) ^ 0x31;
			}
			else
			{
				cCrc <<= 1;
			}
		}
		pTable++;
	}

	return cCrc;
}


/*****************************************************
*函数名称：u8 F_BAT_DataChecek(void)
*函数功能：数据校验
*输入参数：无
*输出参数：无
*调用周期：
*说明：	   校对成功返回1，否则返回0
*****************************************************/
static u8 F_BAT_DataChecek(void)
{

	if (RECEIVE_TABLE[0] != 0x5A)
	{return 0;}

	if (RECEIVE_TABLE[1] > 1)
	{return 0;}

	if (F_BAT_ChecekSumCal(RECEIVE_TABLE,(C_APP_RECEIVE_MAX_NUM-1)) != RECEIVE_TABLE[C_APP_RECEIVE_MAX_NUM-1])
	{return 0;}

	return 1;
}



/*****************************************************
*函数名称：void UART1Interrupt()  interrupt 15
*函数功能：串口中断函数
*输入参数：无
*输出参数：无
*调用周期：
*说明：
*****************************************************/
void UART1Interrupt()		interrupt 15		
{
    static u8  s_u8UartReceiveBuff = 0;
	static u8  s_u8ReceiveLength = C_APP_RECEIVE_MAX_NUM;

	//接收中断
	if (_TEST_UART_RI)
	{
		_CLR_UART_RI;
	
		s_u8UartReceiveBuff = REGISTER_UART;

		 if(g_u8ReceiveStatus == C_REC_ENABLE)
		 {
			 g_u8ReceiveTimeOut = C_RECEIVE_TIMEOUT;
			
			 if (g_u8ReceiveCnt == 0)
			 {s_u8ReceiveLength = C_APP_RECEIVE_MAX_NUM;}
			 	
			 if( g_u8ReceiveCnt < s_u8ReceiveLength )
			 {
				 RECEIVE_TABLE[g_u8ReceiveCnt] = s_u8UartReceiveBuff;

				 //头码判断
				 if (RECEIVE_TABLE[0] != 0x5A)
				 {g_u8ReceiveCnt = 0;}
				 else				
			 	 {g_u8ReceiveCnt++;}

				 //接收结束判断
			 	 if (g_u8ReceiveCnt >= s_u8ReceiveLength)
				 {
				 	g_u8ReceiveStatus = C_REC_DISABLE;
			 	 }
			 }
			 else
			 {
			 	 g_u8ReceiveCnt = 0;
				 g_u8ReceiveStatus = C_REC_DISABLE;	
			 }
		 }
	}
	
	_CLR_UART_TI;		
}

/*****************************************************
*函数名称：void F_BAT_ClearReadFrameData(void)
*函数功能：清接收缓存区
*输入参数：无
*输出参数：无
*调用周期：
*说明：
*****************************************************/
static void F_BAT_ClearReadFrameData(void)
{
     u8 xdata i;

	 for( i = 0 ; i < C_APP_RECEIVE_MAX_NUM ; i++)
	 {
		  RECEIVE_TABLE[i] = 0;
	 }
	 g_u8ReceiveCnt = 0;
	 g_u8ReceiveStatus = C_REC_ENABLE;
}


/*****************************************************
*函数名称：void F_BAT_OneFrameDataReceived(void)
*函数功能：接收数据处理
*输入参数：无
*输出参数：无
*调用周期：
*说明：
*****************************************************/
static void F_BAT_OneFrameDataReceived(void)
{
	static FILTER16_T xdata POWER_LITHIUM = {0,0,0};

	if (F_BAT_DataChecek())
	{	
		g_u8DisConnectCnt = 0;

		if ((RECEIVE_TABLE[1] == 0) && (RECEIVE_TABLE[2] == 0) && (RECEIVE_TABLE[3] == 0))
		{
			g_bBatteryType = C_BATTERY_LEAD;
		}
		else
		{
			g_bBatteryType = C_BATTERY_LITHIUM;
	
			if (RECEIVE_TABLE[1])
			{
				g_bChargeStatus = C_IN_CHARGE;	
			}
			else
			{
				g_bChargeStatus = C_NO_CHARGE;		
			}
			
//			g_u16PowerValue = (_MAKEWORD(RECEIVE_TABLE[2],RECEIVE_TABLE[3]));
//			g_u8PowerMainValue  = g_u16PowerValue / 10;
//			g_u8PowerSlaveValue = g_u16PowerValue % 10;
	
			if (F_FILTER_16Bit(g_u16PowerValue,1,&POWER_LITHIUM,0))
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
		}						
	}
	else
	{	  
//		if (++g_u8UartRecErro > 250)
//		{g_u8UartRecErro = 250;}
	}
}

/*****************************************************
*函数名称：void F_BAT_UART_ReceiveTimeout(void)
*函数功能：串口接收超时计算
*输入参数：无
*输出参数：无
*调用周期：10mS
*说明：
*****************************************************/
void F_BAT_UART_ReceiveTimeout(void)
{
	if (g_u8ReceiveTimeOut > 0)
	{
		if (--g_u8ReceiveTimeOut == 0)
		{
			 g_u8ReceiveStatus = C_REC_DISABLE;
			 F_BAT_OneFrameDataReceived();
			 F_BAT_ClearReadFrameData();
		}		
	}

	//超过600毫秒没收到锂电池串口数据，则默认为适配器供电
	if (++g_u8DisConnectCnt > 60)
	{
		g_u8DisConnectCnt = 100;
		g_bBatteryType = C_BATTERY_LEAD;	
	}
}

/*****************************************************
*函数名称：void F_BAT_RECEIVE_Control(void)
*函数功能：串口接收处理
*输入参数：无
*输出参数：无
*调用周期：10ms
*说明：
*****************************************************/
void F_BAT_RECEIVE_Control(void)
{	
	 if	(g_u8ReceiveStatus == C_REC_DISABLE)
	 {
		 g_u8ReceiveTimeOut = 0;
		 F_BAT_OneFrameDataReceived();
		 F_BAT_ClearReadFrameData();		 
	 }	
}




