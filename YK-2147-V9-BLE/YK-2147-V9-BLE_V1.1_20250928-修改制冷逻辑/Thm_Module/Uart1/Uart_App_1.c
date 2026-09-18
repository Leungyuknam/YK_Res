 /*********************************************************
文 件 名 : Uart_App.C
文件说明 : 串口应用程序
编写作者 : thm
修订作者 : 
编写时间 : 2022-02-27
版 本 号 : V1.0
==========================================================*/
//头文件
#include "COMMON\AllInclude.h"

#if Def_BatCommunicateEn

static   unsigned int   RamDataType   sg_u16RecErroCnt = C_REC_ERROR_TIME_1;								  
static u8 xdata s_u8HandshakeStatus = 0;
static u8 xdata s_u8Sn = 0;
static u16 xdata s_u16SendDelay = 100;
u8  g_u8RecDelay = 0;
bit g_bCompCmnError = 0;

/*****************************************************
*函数名称：u8 F_CheckSumCalc(u8 *pTable, u8 l_u8Length)
*函数功能：校验和计算
*输入参数：无
*输出参数：无
*调用周期：
*说明：	    
*****************************************************/
//static u8 F_CheckSumCalc(u8 *pTable, u8 l_u8Length)
//{
//	unsigned char c;
//	unsigned char cCrc = 0xff;
//	 
//	while(l_u8Length--)
//	{
//		cCrc = cCrc ^ (*pTable);
//		for(c = 8; c > 0; c--)
//		{
//			if(cCrc & 0x80) 
//			{
//				cCrc = (cCrc<<1) ^ 0x31;
//			}
//			else
//			{
//				cCrc <<= 1;
//			}
//		}
//		pTable++;
//	}

//	return cCrc;
//		
//}


u8 F_CheckSumCalc(uint8_t *puchMsg, uint8_t usDataLen){
	
	u8 uCRC = 0x00;//CRC寄存器
	u8 num = 0;
	u8 x = 0;
	
	for(num = 0;num < usDataLen; num++)
	{
		uCRC = (*puchMsg++)^uCRC;//把数据与8位的CRC寄存器的8位相异或，结果存放于CRC寄存器。
		for(x = 0;x<8;x++)
		{	//循环8次
			if(uCRC&0x80){	//判断最低位为：“1”
				uCRC = uCRC<<1;	//先左移
				uCRC = uCRC^0x07;	//再与多项式0x07异或
			}else{	//判断最低位为：“0”
				uCRC = uCRC<<1;	//右移
			}
		}
	}
	return uCRC;//返回CRC校验值
}




static void F_Handshake_Send(void)
{
	g_u8SendBuf1[0]  = 0xA5;
	g_u8SendBuf1[1]  = 0xE1;
	g_u8SendBuf1[2]  = 0x04;
	
	g_u8SendBuf1[3]  = 0x00;
	g_u8SendBuf1[4]  = 0x00;
	
	g_u8SendBuf1[5]  = 0x00;
	g_u8SendBuf1[6]  = 0x00;
	
	g_u8SendBuf1[7]  = F_CheckSumCalc(&g_u8SendBuf1,7);
	
	F_StartSend_S(8);	
}




static void F_BoarData_Send(void)
{
	g_u8SendBuf1[0]  = 0xA5;
	g_u8SendBuf1[1]  = 0xD2;
	g_u8SendBuf1[2]  = 0x04;
	
	if (g_CarRfgrt.u8SpeedDrv == C_Speed_STOP)
	{g_u8SendBuf1[3]  = 0;}
	else
	{g_u8SendBuf1[3]  = 1;}
	
	if(g_CarRfgrt.u8SpeedDrv == C_Speed_MAX)		//3000
	{g_u8SendBuf1[4]  = 0x0e;}
	else if (g_CarRfgrt.u8SpeedDrv == C_Speed_ECO)	//2000
	{g_u8SendBuf1[4]  = 0x00;}
	else if (g_CarRfgrt.u8SpeedDrv == C_Speed_MAX4)	//4000
	{g_u8SendBuf1[4]  = 0x0f;}	
	else
	{g_u8SendBuf1[4]  = 0x10;}
	
	
	g_u8SendBuf1[5]  = 135;

	if (g_CarRfgrt.u8BatProtectLv == C_Protect_Low)
	{g_u8SendBuf1[6]  = 0 << 4;}
	else if (g_CarRfgrt.u8BatProtectLv == C_Protect_Mid)
	{g_u8SendBuf1[6]  = 4 << 4;}
	else
	{g_u8SendBuf1[6]  = 12 << 4;}
	
	g_u8SendBuf1[7]  = F_CheckSumCalc(&g_u8SendBuf1,7);
	
	F_StartSend_S(8);	
}




void F_Uart1App_Handle(void)
{
	static u8 xdata s_u8RpeatCnt = 0;
	
	if (sg_u16RecErroCnt)
	{
		sg_u16RecErroCnt--;
	}
	else
	{
		g_bCompCmnError = 1;
		
		sg_u16RecErroCnt = C_REC_ERROR_TIME_1;
	}
	
	if (s_u16SendDelay > 0)
	{//解码成功后，延时100ms回复
		s_u16SendDelay--;
		s_u8RpeatCnt = 0;
		
		if (s_u16SendDelay == 0)
		{
			_UART_RX_DIS;		//接收关闭---------------发送数据前必须关掉接收中断
			
			if (s_u8HandshakeStatus == 0)		//还没有握手成功
			{
				F_AddActiveSendFun_S(F_Handshake_Send,++s_u8Sn);
			}
			else 
			{
				F_AddActiveSendFun_S(F_BoarData_Send,++s_u8Sn);
			}			
		}
	}
	else
	{
		if (++s_u8RpeatCnt >= 0xFE)	//2.5秒没有收到串口信息
		{			s_u8RpeatCnt = 0;
			s_u16SendDelay = 20;			//200MS发送一次周期
		}
	}
	
	if (g_u8RecDelay > 0)
	{//发送完成后，延时50ms打开接收
		g_u8RecDelay--;
		
		if (g_u8RecDelay == 0)
		{
			_UART_RX_EN;			//打开使能接收
		}
	}
	
}


void F_OneFrameDataReceived_S(void)
{

	if (g_u8ReadBuf1[0] != 0x5A)
	{return ;}

	if (s_u8HandshakeStatus == 0)		//还没有握手成功
	{//握手
		if (g_u8ReadBuf1[1] != 0xE1)
		{return ;}		

		if (g_u8ReadBuf1[2] != 0x04)
		{return ;}	
		
		if (F_CheckSumCalc(&g_u8ReadBuf1,7) != g_u8ReadBuf1[7])
		{return ;}
			
		s_u8HandshakeStatus = 1;		//握手成功了
	}
	else
	{//数据
		if (g_u8ReadBuf1[1] != 0xD2)
		{return ;}

		if (g_u8ReadBuf1[2] != 0x06)
		{return ;}	

		if (F_CheckSumCalc(&g_u8ReadBuf1,9) != g_u8ReadBuf1[9])
		{return ;}
		
		//g_sPower = F_BAT_PowerCalculate(_MAKEWORD(g_u8ReadBuf1[4],g_u8ReadBuf1[3]),0);
		
		if (g_u8ReadBuf1[8] < 7)
		{g_CarRfgrt.u8CompError = g_u8ReadBuf1[8];}	
		else
			g_CarRfgrt.u8CompError=0;
		
	}
	
	sg_u16RecErroCnt = C_REC_ERROR_TIME_1;	//1S错误串口计时
	
	s_u16SendDelay = 10;			//收到返回的串口信息了，就使能100ms的串口发送
	g_bCompCmnError = 0;
}

#endif
