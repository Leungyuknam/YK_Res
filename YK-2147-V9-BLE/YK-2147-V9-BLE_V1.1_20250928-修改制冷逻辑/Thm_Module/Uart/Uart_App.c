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
								  
u8 xdata g_u8AppSn = 0;


/*****************************************************
*函数名称：u16 F_CheckSum_Calc(u8 *pu8Table, u8 l_u8Length)
*函数功能：校验和计算
*调用周期：无
*输入参数：pu8Table待计算表格
		   l_u8Length表格数据长度
*返回值：  校验和
*说明：	   
*****************************************************/
static u16 F_CheckSum_Calc(u8 *pu8Table, u8 l_u8Length)
{
	u8   i;
	u16  l_u16CheckSum = 0;

	for (i = 0; i < l_u8Length; i++)
	{
		l_u16CheckSum += pu8Table[i];
	}

	return l_u16CheckSum;
}


/*****************************************************
*函数名称：void F_Update(void)
*函数功能：数据更新
*调用周期：无
*输入参数：无
*返回值：  无
*说明：	   
*****************************************************/
static void F_Update(void)
{
	g_CarRfgrt.u8AppLock              =   g_u8ReadBuf[4];
	g_CarRfgrt.u8OnOff                =   g_u8ReadBuf[5];

	if (g_CarRfgrt.u8SpeedLv != g_u8ReadBuf[6])
	{
		g_CarRfgrt.u8SpeedLv    =   g_u8ReadBuf[6];

		if (g_CarRfgrt.u8OnOff == C_Power_ON)
		{
			g_sModeSet.u8Status     =   C_MS_STATUS_SPEED;
			g_sModeSet.u8Time       =   C_MS_TIME_3S;
		}
	}

	if (g_CarRfgrt.u8BatProtectLv != g_u8ReadBuf[7])
	{
		g_CarRfgrt.u8BatProtectLv    =   g_u8ReadBuf[7];

		if (g_CarRfgrt.u8OnOff == C_Power_ON)
		{
			g_sModeSet.u8Status     =   C_MS_STATUS_PROTECT;
			g_sModeSet.u8Time       =   C_MS_TIME_3S;
		}
	} 

	if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_RIGHT_EN)
	{
		g_CarRfgrt.s8SetTemp_L            =   g_u8ReadBuf[8];
		g_CarRfgrt.s8SetTemp_Top          =   g_u8ReadBuf[9];
		g_CarRfgrt.s8SetTemp_Bot          =   g_u8ReadBuf[10];
		g_CarRfgrt.s8StartBias_L          =   g_u8ReadBuf[11];
		g_CarRfgrt.u8ComProtectTime       =   g_u8ReadBuf[12];
	
		g_CarRfgrt.s8TempComp_N6_L        =   g_u8ReadBuf[14];
		g_CarRfgrt.s8TempComp_N12_L       =   g_u8ReadBuf[15];
		g_CarRfgrt.s8TempComp_N13_L       =   g_u8ReadBuf[16];
		g_CarRfgrt.s8StopBias_L           =   g_u8ReadBuf[17];
	}

#if (Def_Machine_Type != C_Machine_Type_SingleBox)

	if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_LEFT_EN)
	{
		g_CarRfgrt.s8SetTemp_R            =   g_u8ReadBuf[18];
		g_CarRfgrt.s8StartBias_R          =   g_u8ReadBuf[21];
		g_CarRfgrt.s8TempComp_N6_R        =   g_u8ReadBuf[22];
		g_CarRfgrt.s8TempComp_N12_R       =   g_u8ReadBuf[23];
		g_CarRfgrt.s8TempComp_N13_R       =   g_u8ReadBuf[24];
		g_CarRfgrt.s8StopBias_R           =   g_u8ReadBuf[25];
	}

#endif

	if (g_CarRfgrt.u8TempUnit != g_u8ReadBuf[13])
	{
		g_CarRfgrt.u8TempUnit         =   g_u8ReadBuf[13];

		if (g_CarRfgrt.u8TempUnit == C_Unit_Celsius)
		{
			g_CarRfgrt.s8DispTemp_L       =   F_F2C(g_CarRfgrt.s8DispTemp_L);
			g_CarRfgrt.s8DispTemp_R       =   F_F2C(g_CarRfgrt.s8DispTemp_R);
			g_CarRfgrt.s8RoomTemp_L       =   F_F2C(g_CarRfgrt.s8RoomTemp_L);
			g_CarRfgrt.s8RoomTemp_R       =   F_F2C(g_CarRfgrt.s8RoomTemp_R);			
		}
		else
		{
			g_CarRfgrt.s8DispTemp_L       =   F_C2F(g_CarRfgrt.s8DispTemp_L);
			g_CarRfgrt.s8DispTemp_R       =   F_C2F(g_CarRfgrt.s8DispTemp_R);
			g_CarRfgrt.s8RoomTemp_L       =   F_C2F(g_CarRfgrt.s8RoomTemp_L);
			g_CarRfgrt.s8RoomTemp_R       =   F_C2F(g_CarRfgrt.s8RoomTemp_R);			
		}
	}		
}


/*****************************************************
*函数名称：void F_Refill_FrameHead(u8 l_DataLength, u8 l_u8CmeType)
*函数功能：帧头填充
*调用周期：无
*输入参数：l_DataLength后续数据长度
		   l_u8CmeType帧类型
*返回值：  无
*说明：	   
*****************************************************/
static void F_Refill_FrameHead(u8 l_DataLength, u8 l_u8CmeType)
{
	g_u8SendBuf[0]  = C_FRAME_HEAD;
	g_u8SendBuf[1]  = C_FRAME_HEAD;
	g_u8SendBuf[2]  = l_DataLength;
	g_u8SendBuf[3]  = l_u8CmeType;
}


/*****************************************************
*函数名称：void F_Refill_FrameTail(void)
*函数功能：帧尾填充并启动发送
*调用周期：无
*输入参数：无
*返回值：  无
*说明：	   
*****************************************************/
static void F_Refill_FrameTail(void)
{
	u16 l_u16CheckSum = F_CheckSum_Calc(&g_u8SendBuf,g_u8SendBuf[2] + 1);

	g_u8SendBuf[g_u8SendBuf[2] + 1]  = _HI8BIT(l_u16CheckSum);
	g_u8SendBuf[g_u8SendBuf[2] + 2]  = _LO8BIT(l_u16CheckSum);

	F_StartSend(g_u8SendBuf[2] + 3);
}


/*****************************************************
*函数名称：void F_UpLoad_LeftTemp(void)
*函数功能：左箱温度上报
*调用周期：无
*输入参数：无
*返回值：  无
*说明：	   
*****************************************************/
static void F_UpLoad_LeftTemp(void)
{
	F_Refill_FrameHead(0x04,C_CMD_SET_TP_L);

	g_u8SendBuf[4]  = g_CarRfgrt.s8SetTemp_L;

	F_Refill_FrameTail();	
}


/*****************************************************
*函数名称：void F_UpLoad_RightTemp(void)
*函数功能：右箱温度上报
*调用周期：无
*输入参数：无
*返回值：  无
*说明：	   
*****************************************************/
static void F_UpLoad_RightTemp(void)
{
	F_Refill_FrameHead(0x04,C_CMD_SET_TP_R);

	g_u8SendBuf[4]  = g_CarRfgrt.s8SetTemp_R;

	F_Refill_FrameTail();	
}


/*****************************************************
*函数名称：void F_AllData_Refill(u8 l_u8CmdType)
*函数功能：全系列参数上报
*调用周期：无
*输入参数：无
*返回值：  无
*说明：	   
*****************************************************/
static void F_AllData_Refill(u8 l_u8CmdType)
{

	F_Refill_FrameHead(0x21,l_u8CmdType);

	g_u8SendBuf[4]   = g_CarRfgrt.u8AppLock;
	g_u8SendBuf[5]   = g_CarRfgrt.u8OnOff;
	g_u8SendBuf[6]   = g_CarRfgrt.u8SpeedLv;
	g_u8SendBuf[7]   = g_CarRfgrt.u8BatProtectLv;
	g_u8SendBuf[8]   = g_CarRfgrt.s8SetTemp_L;
	g_u8SendBuf[9]   = g_CarRfgrt.s8SetTemp_Top;
	g_u8SendBuf[10]  = g_CarRfgrt.s8SetTemp_Bot;
	g_u8SendBuf[11]  = g_CarRfgrt.s8StartBias_L;
	g_u8SendBuf[12]  = g_CarRfgrt.u8ComProtectTime;
	g_u8SendBuf[13]  = g_CarRfgrt.u8TempUnit;
	g_u8SendBuf[14]  = g_CarRfgrt.s8TempComp_N6_L;
	g_u8SendBuf[15]  = g_CarRfgrt.s8TempComp_N12_L;
	g_u8SendBuf[16]  = g_CarRfgrt.s8TempComp_N13_L;
	g_u8SendBuf[17]  = g_CarRfgrt.s8StopBias_L;



#if ((Def_Machine_Type == C_Machine_Type_SingleBox))
	if (TEMPER_LBox.AD_Staut != c_AD_NORMAL)
	{g_u8SendBuf[18]  = 128;}
	else
	{g_u8SendBuf[18]  = g_CarRfgrt.s8DispTemp_L;}
#elif (Def_Machine_Type == C_Machine_Type_DoubleBox_LeftMaster)
	if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_RIGHT_EN)
	{g_u8SendBuf[18]  = g_CarRfgrt.s8DispTemp_L;}
	else
	{g_u8SendBuf[18]  = (-128);}
#endif

	g_u8SendBuf[19]  = g_sPower.u8Level * 20;
    g_u8SendBuf[20]  = g_sPower.u16Value / 10;
	g_u8SendBuf[21]  = g_sPower.u16Value % 10;



#if (Def_Machine_Type != C_Machine_Type_SingleBox)

	g_u8SendBuf[22]  = g_CarRfgrt.s8SetTemp_R;
	g_u8SendBuf[23]  = 0x00;
	g_u8SendBuf[24]  = 0x00;
	g_u8SendBuf[25]  = g_CarRfgrt.s8StartBias_R;
	g_u8SendBuf[26]  = g_CarRfgrt.s8TempComp_N6_R;
	g_u8SendBuf[27]  = g_CarRfgrt.s8TempComp_N12_R;
	g_u8SendBuf[28]  = g_CarRfgrt.s8TempComp_N13_R;
	g_u8SendBuf[29]  = g_CarRfgrt.s8StopBias_R;

#if (Def_Machine_Type == C_Machine_Type_DoubleBox_LeftMaster)
	if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_LEFT_EN)
	{g_u8SendBuf[30]  = g_CarRfgrt.s8DispTemp_R;}
	else
	{g_u8SendBuf[30]  = (-128);}
#elif (Def_Machine_Type == C_Machine_Type_DoubleBox_DoubleMaster)
	if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_LEFT_EN)
	{g_u8SendBuf[30]  = g_CarRfgrt.s8DispTemp_R;}
	else
	{g_u8SendBuf[30]  = (-128);}
#endif

#else

	g_u8SendBuf[22]  = 128;
	g_u8SendBuf[23]  = 0x00;
	g_u8SendBuf[24]  = 0x00;
	g_u8SendBuf[25]  = 0x00;
	g_u8SendBuf[26]  = 0x00;
	g_u8SendBuf[27]  = 0x00;
	g_u8SendBuf[28]  = 0x00;
	g_u8SendBuf[29]  = 0x00;
	g_u8SendBuf[30]  = 128;

#endif

	g_u8SendBuf[31]  = g_CarRfgrt.u8ValveStatus;
	g_u8SendBuf[32]  = 1;
	g_u8SendBuf[33]  = 0x00;



	F_Refill_FrameTail();	
}


/*****************************************************
*函数名称：void F_AtCmd_Tx(void)
*函数功能：AT指令测试
*调用周期：无
*输入参数：无
*返回值：  无
*说明：	   
*****************************************************/
static void F_AtCmd_Tx(void)
{
	g_u8SendBuf[0] = 'A';
 	g_u8SendBuf[1] = 'T';
	g_u8SendBuf[2] = '+';
	g_u8SendBuf[3] = 'U'; 
	g_u8SendBuf[4] = 'A';
	g_u8SendBuf[5] = 'R';
	g_u8SendBuf[6] = 'T';
	g_u8SendBuf[7] = '?';
	g_u8SendBuf[8] = 0x0D;
	g_u8SendBuf[9] = 0x0A;

	F_StartSend(10);	
}

/*****************************************************
*函数名称：void F_AtCmd_Rx(void)
*函数功能：AT指令解释
*调用周期：无
*输入参数：无
*返回值：  无
*说明：	   
*****************************************************/
static void F_AtCmd_Rx(void)
{
	if(g_u8ReadBuf[0] != 'A')	{	return;	} 	
	if(g_u8ReadBuf[1] != 'T')	{	return;	}
	if(g_u8ReadBuf[2] != '+')	{	return;	}	
	if(g_u8ReadBuf[3] != 'U')	{	return;	}	
	if(g_u8ReadBuf[4] != 'A')	{	return;	}
	if(g_u8ReadBuf[5] != 'R')	{	return;	}	
	if(g_u8ReadBuf[6] != 'T')	{	return;	}
	if(g_u8ReadBuf[7] != ':')	{	return;	}
//	if(g_u8ReadBuf[8] != 0x00)	{	return;	}
//	if(g_u8ReadBuf[9] != 0x00)	{	return;	}
//	if(g_u8ReadBuf[10] != 0xE1)	{	return;	}
//	if(g_u8ReadBuf[11] != 0x00)	{	return;	}
	
	g_CarRfgrt.u8BleStatus = C_BLE_IDLE;	
}


/*****************************************************
*函数名称：void F_UpLoad_Check(void)
*函数功能：查询回复
*调用周期：无
*输入参数：无
*返回值：  无
*说明：	   
*****************************************************/
static void F_UpLoad_Check(void)
{
	F_AllData_Refill(C_CMD_CHECK);		
}


/*****************************************************
*函数名称：void F_UpLoad_Set(void)
*函数功能：设置回复
*调用周期：无
*输入参数：无
*返回值：  无
*说明：	   
*****************************************************/
static void F_UpLoad_Set(void)
{
	F_AllData_Refill(C_CMD_SET);		
}


/*****************************************************
*函数名称：void F_UpLoad_Reset(void)
*函数功能：恢复出厂回复
*调用周期：无
*输入参数：无
*返回值：  无
*说明：	   
*****************************************************/
static void F_UpLoad_Reset(void)
{
	F_AllData_Refill(C_CMD_RESET);		
}

/*****************************************************
*函数名称：void F_UpLoad_Bind(void)
*函数功能：绑定回复
*调用周期：无
*输入参数：无
*返回值：  无
*说明：	   
*****************************************************/
void F_UpLoad_Bind(void)
{
	F_Refill_FrameHead(0x04,C_CMD_BIND);

	if (g_CarRfgrt.u8BleStatus == C_BLE_BIND)
	{g_u8SendBuf[4]  = 1;}
	else
	{g_u8SendBuf[4]  = 0;}

	F_Refill_FrameTail();
}


/*****************************************************
*函数名称：void F_OneFrameDataReceived(void)
*函数功能：接收数据解码
*调用周期：无
*输入参数：无
*返回值：  无
*说明：	   
*****************************************************/
void F_OneFrameDataReceived(void)
{
	u16 l_u16CheckSum;

	if (g_CarRfgrt.u8BleStatus == C_BLE_MISS)
	{
		F_AtCmd_Rx();
//		return ;
	}

	if (g_u8ReadBuf[0] != C_FRAME_HEAD)
	{return ;}

	if (g_u8ReadBuf[1] != C_FRAME_HEAD)
	{return ;}

	if (g_u8ReadBuf[2] < 3)
	{return ;}

	l_u16CheckSum = F_CheckSum_Calc(&g_u8ReadBuf,g_u8ReadBuf[2]+1);

	if ((_HI8BIT(l_u16CheckSum) == (g_u8ReadBuf[g_u8ReadBuf[2]+1])) && ((_LO8BIT(l_u16CheckSum) == (g_u8ReadBuf[g_u8ReadBuf[2]+2]))))
	{
		if (g_CarRfgrt.u8BleStatus == C_BLE_MISS)
		{
			g_CarRfgrt.u8BleStatus = C_BLE_IDLE;
		}

		switch (g_u8ReadBuf[3])
		{
			case C_CMD_BIND:
			{
				g_CarRfgrt.u8BleStatus = C_BLE_ASK;
				g_CarRfgrt.u8BleBindTime = 100;
				_BUZZER_SHORT_BEEP;
				break;
			}

			case C_CMD_CHECK:
			{
				F_AddActiveSendFun(F_UpLoad_Check,++g_u8AppSn);
				break;
			}

			case C_CMD_SET:
			{
				_BUZZER_SHORT_BEEP;
				F_Update();
				F_AddActiveSendFun(F_UpLoad_Set,++g_u8AppSn);
				break;
			}
			
			case C_CMD_RESET:
			{
				_BUZZER_SHORT_BEEP;

				F_FactoryDataReset(0,0);
				g_sModeSet.u8Status = C_MS_SYS_RESET;
				g_sModeSet.u8Time   = C_MS_TIME_5S;
				F_AddActiveSendFun(F_UpLoad_Reset,++g_u8AppSn);
				break;
			}
			
			case C_CMD_SET_TP_L:
			{
				_BUZZER_SHORT_BEEP;
				g_CarRfgrt.s8SetTemp_L = g_u8ReadBuf[4];

				if (g_CarRfgrt.u8OnOff == C_Power_ON)
				{
					g_sModeSet.u8Status = C_MS_STATUS_CTRL_LBOX;
					g_sModeSet.u8Object = C_MS_OBJECT_LBOX;
					g_sModeSet.u8Time   = C_MS_TIME_3S;
				}

#if (Def_Machine_Type == C_Machine_Type_DoubleBox_LeftMaster)
				if (g_CarRfgrt.s8SetTemp_L < g_CarRfgrt.s8SetTemp_R)
				{g_CarRfgrt.s8SetTemp_L = g_CarRfgrt.s8SetTemp_R;}
#endif
		   
				F_AddActiveSendFun(F_UpLoad_LeftTemp,++g_u8AppSn);
				F_AddActiveSendFun(F_UpLoad_Set,++g_u8AppSn);
				break;
			}
			
			case C_CMD_SET_TP_R:
			{
				_BUZZER_SHORT_BEEP;
				g_CarRfgrt.s8SetTemp_R = g_u8ReadBuf[4];
				if (g_CarRfgrt.u8OnOff == C_Power_ON)
				{
					g_sModeSet.u8Status = C_MS_STATUS_CTRL_RBOX;
					g_sModeSet.u8Object = C_MS_OBJECT_RBOX;
					g_sModeSet.u8Time   = C_MS_TIME_3S;
				}
				F_AddActiveSendFun(F_UpLoad_RightTemp,++g_u8AppSn);	
				F_AddActiveSendFun(F_UpLoad_Set,++g_u8AppSn);


#if (Def_Machine_Type == C_Machine_Type_DoubleBox_LeftMaster)
				if (g_CarRfgrt.s8SetTemp_L < g_CarRfgrt.s8SetTemp_R)
				{
					g_CarRfgrt.s8SetTemp_L = g_CarRfgrt.s8SetTemp_R;
					F_AddActiveSendFun(F_UpLoad_LeftTemp,++g_u8AppSn);
				}
#endif				
				break;
			}											
		}
	}
}


/*****************************************************
*函数名称：void F_UartApp_Handle(void)
*函数功能：应用程序处理
*调用周期：1s
*输入参数：无
*返回值：  无
*说明：	   
*****************************************************/
void F_UartApp_Handle(void)
{					   
	if (g_CarRfgrt.u8BleStatus == C_BLE_MISS)
	{
		F_AddActiveSendFun(F_AtCmd_Tx,++g_u8AppSn);	
	}
}

