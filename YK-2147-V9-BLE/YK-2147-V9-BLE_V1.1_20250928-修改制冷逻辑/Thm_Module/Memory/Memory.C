/*********************************************************
文 件 名 : Memory.C
文件说明 : 存储记忆
编写作者 : thm
修订作者 : 
编写时间 : 2022-3-30
版 本 号 : V1.0
==========================================================*/
//头文件
#include "COMMON\AllInclude.h"

static unsigned char   	xdata  MEMORY_TABLE[C_MEMORY_LENGTH] = {0};
	   unsigned char    code   FIXED_ADDR[C_MEMORY_LENGTH + C_IAP_BACKUP_BIAS] _at_ C_IAP_ADDR_STAR;

/*****************************************************
*函数名称：void F_TAKE_MemoryTable(void)
*函数功能：从MEMORY_TABLE缓存区提取数据
*输入参数：无
*输出参数：无
*调用周期：无
*说明：	   连续校验失败三次，则恢复出厂值
*****************************************************/
void F_TAKE_MemoryTable(void)
{
	F_FactoryDataReset(C_Power_ON,1);

	F_MEMORY_AllRead(C_IAP_ADDR_STAR);

	if (F_CHECKOUT_MemoryData(MEMORY_TABLE,C_MEMORY_LENGTH) == 0)
	{
		F_MEMORY_AllRead(C_IAP_ADDR_STAR + C_IAP_BACKUP_BIAS);
	}
		
	if (F_CHECKOUT_MemoryData(MEMORY_TABLE,C_MEMORY_LENGTH))
	{

		g_CarRfgrt.u8BatProtectLv  =  MEMORY_TABLE[0] >> 6;

		if (_BTS(MEMORY_TABLE[0],0))	
		{g_CarRfgrt.u8OnOff = C_Power_ON;}
		else
		{g_CarRfgrt.u8OnOff = C_Power_OFF;}

		if (_BTS(MEMORY_TABLE[0],1))	
		{g_CarRfgrt.u8SpeedLv = C_Speed_ECO;}
		else
		{g_CarRfgrt.u8SpeedLv = C_Speed_MAX;}

		if (_BTS(MEMORY_TABLE[0],2))	
		{g_CarRfgrt.u8TempUnit = C_Unit_Fahrenheit;}
		else
		{g_CarRfgrt.u8TempUnit = C_Unit_Celsius;}

		if (_BTS(MEMORY_TABLE[0],3))	
		{g_CarRfgrt.u8NtcToward = C_Toward_Backward;}
		else
		{g_CarRfgrt.u8NtcToward = C_Toward_Forward;}

		if (_BTS(MEMORY_TABLE[0],4))	
		{g_CarRfgrt.u8ValveToward = C_Toward_Backward;}
		else
		{g_CarRfgrt.u8ValveToward = C_Toward_Forward;}

		if (_BTS(MEMORY_TABLE[0],5))	
		{g_CarRfgrt.u8CloseFlag_L = 1;}
		else
		{g_CarRfgrt.u8CloseFlag_L = 0;}

		g_CarRfgrt.u8BoxEn = MEMORY_TABLE[1] & 0x0F;
		g_CarRfgrt.u8ComProtectTime = MEMORY_TABLE[1] >> 4;

		g_CarRfgrt.s8SetTemp_L       =  MEMORY_TABLE[2];
		g_CarRfgrt.s8SetTemp_Top     =  MEMORY_TABLE[3];
		g_CarRfgrt.s8SetTemp_Bot     =  MEMORY_TABLE[4];
		g_CarRfgrt.s8TempComp_N6_L   =  MEMORY_TABLE[5];
		g_CarRfgrt.s8TempComp_N12_L  =  MEMORY_TABLE[6];
		g_CarRfgrt.s8TempComp_N13_L  =  MEMORY_TABLE[7];
		g_CarRfgrt.s8StartBias_L     =  MEMORY_TABLE[8];
		g_CarRfgrt.s8StopBias_L      =  MEMORY_TABLE[9];
		
		g_CarRfgrt.s8SetTemp_R       =  MEMORY_TABLE[10];
		g_CarRfgrt.s8TempComp_N6_R   =  MEMORY_TABLE[11];
		g_CarRfgrt.s8TempComp_N12_R  =  MEMORY_TABLE[12];
		g_CarRfgrt.s8TempComp_N13_R  =  MEMORY_TABLE[13];
		g_CarRfgrt.s8StartBias_R     =  MEMORY_TABLE[14];	
		g_CarRfgrt.s8StopBias_R      =  MEMORY_TABLE[15];
		g_CarRfgrt.u8DefrostOnTime   =  MEMORY_TABLE[16];	
		g_CarRfgrt.u8DefrostOffTime  =  MEMORY_TABLE[17];		
	}
	
	g_CarRfgrt.u8LastComProtectTime = g_CarRfgrt.u8ComProtectTime;	
}

/*****************************************************
*函数名称：void F_TABLE_DataRefill(void)
*函数功能：把存储数据写入MEMORY_TABLE缓存区
*输入参数：无
*输出参数：无
*调用周期：无
*说明：	   不用填写校验字节
*****************************************************/
void F_REFILL_MemoryTable(void)
{
	MEMORY_TABLE[0] = g_CarRfgrt.u8BatProtectLv << 6;

	if (g_CarRfgrt.u8OnOff == C_Power_ON)	        
	{MEMORY_TABLE[0] |= 0x01;}

	if (g_CarRfgrt.u8SpeedLv == C_Speed_ECO)	    
	{MEMORY_TABLE[0] |= 0x02;}

	if (g_CarRfgrt.u8TempUnit == C_Unit_Fahrenheit)	
	{MEMORY_TABLE[0] |= 0x04;}
	
	if (g_CarRfgrt.u8NtcToward == C_Toward_Backward)	
	{MEMORY_TABLE[0] |= 0x08;}
					 
	if (g_CarRfgrt.u8ValveToward == C_Toward_Backward)	
	{MEMORY_TABLE[0] |= 0x10;}

	if (g_CarRfgrt.u8CloseFlag_L)	
	{MEMORY_TABLE[0] |= 0x20;}

	MEMORY_TABLE[1]   =  g_CarRfgrt.u8ComProtectTime;
	MEMORY_TABLE[1] <<= 4;
	MEMORY_TABLE[1]  |=  g_CarRfgrt.u8BoxEn;

	MEMORY_TABLE[2]   =  g_CarRfgrt.s8SetTemp_L;
	MEMORY_TABLE[3]   =  g_CarRfgrt.s8SetTemp_Top;
	MEMORY_TABLE[4]   =  g_CarRfgrt.s8SetTemp_Bot;
	MEMORY_TABLE[5]   =  g_CarRfgrt.s8TempComp_N6_L;
	MEMORY_TABLE[6]   =  g_CarRfgrt.s8TempComp_N12_L;
	MEMORY_TABLE[7]   =  g_CarRfgrt.s8TempComp_N13_L;	
	MEMORY_TABLE[8]   =  g_CarRfgrt.s8StartBias_L;
	MEMORY_TABLE[9]   =  g_CarRfgrt.s8StopBias_L;
	
	MEMORY_TABLE[10]  =  g_CarRfgrt.s8SetTemp_R;
	MEMORY_TABLE[11]  =  g_CarRfgrt.s8TempComp_N6_R;
	MEMORY_TABLE[12]  =  g_CarRfgrt.s8TempComp_N12_R;
	MEMORY_TABLE[13]  =  g_CarRfgrt.s8TempComp_N13_R;
	MEMORY_TABLE[14]  =  g_CarRfgrt.s8StartBias_R;	
	MEMORY_TABLE[15]  =  g_CarRfgrt.s8StopBias_R;
	MEMORY_TABLE[16]  =  g_CarRfgrt.u8DefrostOnTime;	
	MEMORY_TABLE[17]  =  g_CarRfgrt.u8DefrostOffTime;	
}


/*****************************************************
*函数名称：unsigned char F_CHECKSUM_MemoryCalculate(u8 *pTable,u8 l_u8TableLength)
*函数功能：校验和计算
*输入参数：无
*输出参数：无
*调用周期：
*说明：	   校验和额外值0xAA
*****************************************************/
unsigned char F_CHECKSUM_MemoryCalculate(unsigned char *pTable,unsigned char l_u8TableLength)
{
	unsigned char	 i;
	unsigned char    l_u8CheckSum = 0;

	for (i = 0; i < (l_u8TableLength - 1); i++)
	{
		l_u8CheckSum += pTable[i];
	}

	l_u8CheckSum += 0xAA;

	return l_u8CheckSum;
}

/*****************************************************
*函数名称：u8 F_CHECKOUT_MemoryData(u8 *pTable,u8 l_u8TableLength)
*函数功能：校验读取的数据
*输入参数：无
*输出参数：校验成功返回1，否则返回0
*调用周期：
*说明：
*****************************************************/
unsigned char F_CHECKOUT_MemoryData(unsigned char *pTable,unsigned char l_u8TableLength)
{

	if (pTable[l_u8TableLength - 1] == F_CHECKSUM_MemoryCalculate(pTable,l_u8TableLength))
    {
   		return 1;
    }

	return 0;
}


#if (Def_MemoryType == 1)

/*****************************************************
*函数名称：void F_MEMORY_AllWrite(void)
*函数功能：写Flash
*输入参数：无
*输出参数：无
*调用周期：
*说明：
*****************************************************/
void F_MEMORY_AllWrite(void)
{
	unsigned char   i;
	unsigned int    l_u16AddrCnt = C_IAP_ADDR_STAR;
	unsigned char   l_u8CheckSum = 0;

	IAP_SectorErase(C_IAP_MEMTYPE,C_IAP_ADDR_STAR,C_IAP_LIMIT_TIME);

	//第一份数据
	for (i = 0; i < (C_MEMORY_LENGTH - 1); i++)
	{
		IAP_ProgramByte(l_u16AddrCnt,MEMORY_TABLE[i],C_IAP_MEMTYPE,C_IAP_LIMIT_TIME);
		l_u16AddrCnt++;	
	}

	l_u8CheckSum = F_CHECKSUM_MemoryCalculate(MEMORY_TABLE,C_MEMORY_LENGTH);
    
	IAP_ProgramByte(l_u16AddrCnt,(l_u8CheckSum),C_IAP_MEMTYPE,C_IAP_LIMIT_TIME);
    

	//备份数据
	l_u16AddrCnt = (C_IAP_ADDR_STAR + C_IAP_BACKUP_BIAS);
	for (i = 0; i < (C_MEMORY_LENGTH - 1); i++)
	{
		IAP_ProgramByte(l_u16AddrCnt,MEMORY_TABLE[i],C_IAP_MEMTYPE,C_IAP_LIMIT_TIME);
		l_u16AddrCnt++;	
	}
    
	IAP_ProgramByte(l_u16AddrCnt,(l_u8CheckSum),C_IAP_MEMTYPE,C_IAP_LIMIT_TIME);
}

/*****************************************************
*函数名称：void F_MEMORY_AllRead(unsigned int l_u16Addr)
*函数功能：读Flash
*输入参数：无
*输出参数：无
*调用周期：
*说明：
*****************************************************/
void F_MEMORY_AllRead(unsigned int l_u16Addr)
{
	unsigned char   i;

	for (i = 0; i < C_MEMORY_LENGTH; i++)
	{
		MEMORY_TABLE[i] = IAP_ReadByte(l_u16Addr,C_IAP_MEMTYPE);
		l_u16Addr++;
	}

}


#endif











