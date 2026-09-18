/*--------------------------------------------------------------------
模块名称：    串口模块
模块作者：    CJB
编制时间：    2016-6-30
功能描述：    实现串口数据重发，数据帧完毕回调
修改记录：    2016-7-9 1：在F_ReceiveInital中增加对两个队列数据的初始化操作
                       2：在F_ClearReadFrameData中清除 g_u8ReadDataLength1 = 0;操作
		      2016-9-18 代码优化升级
			            1：增加主动判断数据帧结束的判断和处理
						2：增加接收和发送时的特殊字符转义控制（例如第二个字节后出现0xFF发送要在其后添加0x55,
						                                       接收要去掉0x55的宏定义）
						3：增加说明activeSend中的SN Buf上电是等于0的，所以在请求发送时要注意第一次SN不要等于0									   
---------------------------------------------------------------------*/
#include "COMMON\AllInclude.h"

#if Def_BatCommunicateEn

static bit sg_bUartRecOK = 0;

static volatile unsigned char    sg_u8RecPoint = 0;  //读取指针
static volatile unsigned char    sg_u8SendPoint = 0; //发送指针

static unsigned char RamDataType sg_u8NoRecieveCNT = C_NO_RECEIVE_TIME_1;
static unsigned char RamDataType sg_u8NeedSendLength = 0;
static unsigned char RamDataType sg_u8SendDelayTime = 0;
static unsigned char RamDataType sg_u8ResendDelayTime = 0;
static unsigned char RamDataType sg_u8ResendTimes = 0;
static unsigned int  RamDataType sg_u16CurFrameLength = 0;

static OneParametQueue1  RamDataType   S_FrebackSendQueue;
static OneParametQueue1  RamDataType   S_ActiveSendQueue;
static OneParametFunStr  RamDataType   S_FunStrBuf;

volatile unsigned char  RamDataType   g_u8ReadDataLength1 = 0;       //可用数据个数
		 unsigned char	RamDataType   g_u8ReadBuf1[C_RECEIVE_MAX_1];
		 unsigned char  RamDataType   g_u8SendBuf1[C_SEND_MAX_1];
		 unsigned char  RamDataType   g_u8FrebackInf1 = 0;



static void Idle(void)
{	
	_nop_();
}

static void F_ClearReadFrameData(void)
{
     unsigned char RamDataType i;

	 for( i = 0 ; i < C_RECEIVE_MAX_1 ; i++)
	 {
		  g_u8ReadBuf1[i] = 0;
	 }
	 sg_u8RecPoint = 0;
	 g_u8ReadDataLength1 = 0;    //2016-7-9  增加更改
	 sg_u16CurFrameLength = 0;
	 sg_bUartRecOK = 0;

}

//串口接收中断处理
static void F_ReadINT(void) 
{
     static unsigned char  x;

	 x = USER_REG_RECEIVE_S;

	 if( sg_bUartRecOK == 0)
	 {
		 sg_u8NoRecieveCNT = 0;

		 if( sg_u8RecPoint < C_RECEIVE_MAX_1 )
		 {		 
			 //----根据特殊协议添加-----比如接收到0xFF后的0x55要丢弃----------
			 C_ReadOtherProtocolDeal_S();		  
			 //////////////////////////////////////////////////////
			 
			 g_u8ReadBuf1[sg_u8RecPoint] = x;

			 //-----增加根据协议来判断数据帧结束的判断--2016-9-18---
			 C_CheckProtocolFrameFinish_S();

//			 if (++sg_u16CurFrameLength >= 6)
//			 {
//			 	if (sg_u8RecPoint >= (6 + _MAKEWORD(g_u8ReadBuf1[4],g_u8ReadBuf1[5])))
//				{
//					sg_bUartRecOK = 1;
//				}
//			 }
			 ////////////////////////////////////////////////////////
		 }
			
		 if (g_u8ReadBuf1[2] == 0x06)
		 {
			 if (++sg_u8RecPoint >= C_RECEIVE_MAX_1)
			 {
				sg_bUartRecOK = 1;
			 }		 
		 }
		 else
		 {
			 if (++sg_u8RecPoint >= 8)
			 {
				sg_bUartRecOK = 1;
			 }
		 }
	 }
}


//串口接收主程序处理
static void F_ReceiveControl(void)
{	
	 if (sg_u8NoRecieveCNT < C_NO_RECEIVE_TIME_1)
	 {
	 	sg_u8NoRecieveCNT++;
		if (sg_u8NoRecieveCNT >= C_NO_RECEIVE_TIME_1)
		{sg_bUartRecOK = 1;}
	}

//	 if((++sg_u8NoRecieveCNT >= C_NO_RECEIVE_TIME) || (sg_bUartRecOK))
	 if((sg_bUartRecOK))
	 {
	 	 sg_u8NoRecieveCNT = C_NO_RECEIVE_TIME_1;
		 sg_bUartRecOK = 1;
		 g_u8ReadDataLength = sg_u8RecPoint;	  //修改？删除该变量？
		 F_OneFrameDataReceived_S();
		 F_ClearReadFrameData();
	 }
}



//串口发送中断处理
static void F_SendINT(void) 
{	
	sg_u8SendDelayTime = C_SEND_INTERVAL_TIME_1;

	//--------特殊协议处理--例如两个连续0xFF后需要加多一个0x55----------------------
	C_SendOtherProtocolDeal_S();
	/////////////////////////////////////////////
	if( (sg_u8SendPoint < sg_u8NeedSendLength) && (sg_u8SendPoint < C_SEND_MAX_1 ) )
	{
		USER_REG_SEND_S = g_u8SendBuf1[sg_u8SendPoint];
		
		sg_u8SendPoint++;
		//根据不同芯片调整设置
		C_ONEBitSendOverRigistSet_S();
	
	}
	else
	{
		//根据不同芯片调整设置 
		C_ONEFrameSendOverRigistSet_S();
	}
}

static unsigned char F_CheckIfNeedSendData(OneParametQueue1 *str)
{
	 if((str->Head) == (str->Tail) )
	 {
		 return 0;
	 }
	 
	 else
	 {
		 return 1;
	 }
}

static void F_GetSendFun(OneParametQueue1 *QueueStr,OneParametFunStr *str)
{
	 if((QueueStr->Head) == (QueueStr->Tail) )
	 {
	     str->Fan = &Idle;
		 str->SN  = 0;
	 }
	 
	 else
	 {
		 str->Fan  = (QueueStr->Fun[QueueStr->Tail]).Fan;
		 str->SN   = (QueueStr->Fun[QueueStr->Tail]).SN;
	 }
}

static void F_DecSendFun(OneParametQueue1 *QueueStr)
{
	  if((QueueStr->Head) == (QueueStr->Tail) )
	  {
	      return ;
	  }
	  
	  else
	  {
		  if( ++(QueueStr->Tail) >= C_SEND_QUEUE_MAX_1 )
		  {
		  	QueueStr->Tail = 0;
		  }
	  }
}



//串口发送主程序处理
static void F_SendControl(void)
{
	 static unsigned char r_SNBuf = 0;
	 
	 //单次发送
	 if( sg_u8SendDelayTime != 0 )
	 {
		 sg_u8SendDelayTime--;
		 return;
	 }

	 if( F_CheckIfNeedSendData(&S_FrebackSendQueue) )
	 {
         F_GetSendFun(&S_FrebackSendQueue,&S_FunStrBuf);
		 (*(S_FunStrBuf.Fan))(S_FunStrBuf.SN);
		 F_DecSendFun(&S_FrebackSendQueue);
		 return;
	 }


	 //多次发送
	 if( sg_u8ResendDelayTime   != 0 )
	 {
		 sg_u8ResendDelayTime--;
  		 return;
	 }

     if( F_CheckIfNeedSendData( &S_ActiveSendQueue ) )
	 {
		 F_GetSendFun( &S_ActiveSendQueue, &S_FunStrBuf );
		 if( S_FunStrBuf.SN != r_SNBuf )
		 {
			 sg_u8ResendTimes = C_RESEND_TIMES_1;
			 r_SNBuf = S_FunStrBuf.SN;
			 g_u8FrebackInf1 = S_FunStrBuf.SN;
		 }
		 
		 if( sg_u8ResendTimes != 0 )
		 {
		     sg_u8ResendTimes--;
			 (*(S_FunStrBuf.Fan))(S_FunStrBuf.SN);
			 sg_u8ResendDelayTime = C_RESEND_DELAY_TIME_1;
	     }
		 
		 else
		 {
			 F_DecSendFun(&S_ActiveSendQueue);
			 g_u8FrebackInf1 = 0;
		 }	 
	 }
}




static void F_AddSendFun(OneParametQueue1 *QueueStr,U8Fan Fun,unsigned char SN)
{
	 unsigned char  x;

	 x = QueueStr->Head;
	 if( ++x >=C_SEND_QUEUE_MAX_1 )
	 {
		 x = 0;
	 }

	 if( x == QueueStr->Tail)
	 {
		 
	 }
	 
	 else
	 {
		 QueueStr->Fun[QueueStr->Head].Fan = Fun;
		 QueueStr->Fun[QueueStr->Head].SN = SN;
		 QueueStr->Head = x;
	 }
}



#if F_GetReadFrameNData_En_S
unsigned char F_GetReadFrameNData_S(unsigned char N)
{
   return g_u8ReadBuf1[N];
}
#endif

#if F_AddActiveSendFun_En_S
void F_AddActiveSendFun_S(U8Fan Fun,	unsigned char SN)
{
     F_AddSendFun(&S_ActiveSendQueue,Fun, SN );
}
#endif

#if F_AddFrebackSendFun_En_S
void F_AddFrebackSendFun_S(U8Fan Fun, unsigned char SN )
{
     F_AddSendFun(&S_FrebackSendQueue, Fun, SN );
}
#endif

#if F_DecActiveSendFun_En_S 
void F_DecActiveSendFun_S( void )
{
     F_DecSendFun(&S_ActiveSendQueue);
}
#endif




void F_StartSend_S(unsigned char l_u8Length)
{
	 sg_u8NeedSendLength = l_u8Length;
	 sg_u8SendPoint = 1;
	 F_UART_Send_init_S() ;
	 USER_REG_SEND_S = g_u8SendBuf1[0];
}



void F_UartRecInital_S(void)
{
	 F_UART_Recieve_init_S();

	 //此部分初始化为2016-7-9日修改添加
	 S_FrebackSendQueue.Head = 0;
	 S_FrebackSendQueue.Tail = 0;
	 S_ActiveSendQueue.Head = 0;
	 S_ActiveSendQueue.Tail = 0;
}




void F_UartControl_S(void)
{   
   F_SendControl();
	 F_ReceiveControl();
}


void F_Uart1_Int() interrupt USER1_UART_C51_INT_NUM   
{
	if(_TEST_UART1_TX)    //发送标志位判断
	{
		_CLR_UART1_TX;
		F_SendINT();
		g_u8RecDelay = 5;	//每发送一个字节就延迟5MS使能串口接收
	}

	if(_TEST_UART1_RX)  //接收标志位判断
	{
		_CLR_UART1_RX;
		F_ReadINT(); 
	}	
}

#endif
