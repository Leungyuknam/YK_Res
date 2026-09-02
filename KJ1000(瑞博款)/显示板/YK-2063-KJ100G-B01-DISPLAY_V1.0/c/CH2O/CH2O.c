#include "h\Allinclude.h"

#define CH2O_SDASet_Input()		{P2CON &= ~BIT7;}
#define CH2O_SDASet_Output()		{P2CON |= BIT7;}
#define CH2O_SCLSet_Input()		{P1CON &= ~BIT1;}
#define CH2O_SCLSet_Output()		{P1CON |= BIT1;}

#define CH2O_SDASet_H()			{P_CH2O_SDA = 1;}
#define CH2O_SDASet_L()			{P_CH2O_SDA = 0;}
#define	CH2O_GetSDApin()			(P_CH2O_SDA)

#define CH2O_SCLSet_H()			{P_CH2O_SCL = 1;}
#define CH2O_SCLSet_L()			{P_CH2O_SCL = 0;}

#define	I2C_ACK_ERR			1
#define	I2C_ACK_OK			0

/* 参数 */
#define CH2O_BUF_SIZE       5
#define CH2O_FAST_THR       5      /* ug/m3，快速跟随阈值 */
#define DELAY_FAST_SEC      5
#define DELAY_SLOW_SEC      64

U8 xdata g_u8CH2OLinkTime = 5;//甲醛通讯时间

U8 xdata g_u8CH2OReadType = 0;//CH2O读取类型

U16 xdata g_u16CH2OData = 0;//CH2O数值 0~5000对应0~5000ppb
//S8 xdata g_s8TempData = 24;//温度 范围-20~50℃
//U8 xdata g_u8HumiData = 60;//湿度 范围15~90%RH

U8 code t_CH2OTaple[2] = {0x53,0x32};//测量CH2O值命令码
U8 code t_SelfCheckTaple[2] = {0x56,0x10};//测量CH2O值命令码
//U8 code t_TempHumiTaple[2] = {0x46,0xFD};//测量温湿度值命令码

U8 code t_DispCH2OData[3] = {1,3,2};//甲醛数值小于3随机数

U8 xdata CH2OReadBuf[c_CH2O_LENGTH_MAX] = {0};

/**************************************************
*函数名称：void  F_Delay(void) 
*函数功能：延时(单位1us)
*入口参数：void
*出口参数：void  
**************************************************/
static void F_Delay(U8 l_u8Cnt)	
{
     do
	 {
	 	NOP();NOP();NOP();NOP();NOP();
	 	NOP();NOP();NOP();NOP();NOP();
	 	NOP();NOP();NOP();NOP();NOP();
	 	NOP();NOP();NOP();
	 }while(--l_u8Cnt);
     WDT_clear();
}
/**************************************************
*函数名称：void  F_StartBit(void) 
*函数功能：IIC起始位
*入口参数：void
*出口参数：void  
**************************************************/
static void F_StartBit()
{
	CH2O_SCLSet_Output();
	CH2O_SDASet_Output();
	CH2O_SCLSet_H();
	CH2O_SDASet_H();
	F_Delay(7);
	CH2O_SDASet_L();
	F_Delay(7);
	CH2O_SCLSet_L();
	F_Delay(7);
}
/**************************************************
*函数名称：void  F_StopBit(void) 
*函数功能：IIC停止位
*入口参数：void
*出口参数：void  
**************************************************/
static void F_StopBit()
{
	CH2O_SDASet_L();
	F_Delay(7);
	CH2O_SCLSet_H();
	F_Delay(7);
	CH2O_SDASet_H();
	F_Delay(7);
	CH2O_SCLSet_Input();
	CH2O_SDASet_Input();
}
/**************************************************
*函数名称：void  F_mAck(void) 
*函数功能：应答
*入口参数：void
*出口参数：void  
**************************************************/
static void F_mAck()
{
	CH2O_SDASet_L();
	F_Delay(7);
	CH2O_SCLSet_H();
	F_Delay(7);
	CH2O_SCLSet_L();
	F_Delay(7);
	CH2O_SDASet_H();
	F_Delay(7);
}
/**************************************************
*函数名称：void F_mNack(void) 
*函数功能：结束ACK
*入口参数：void
*出口参数：void  
**************************************************/
static void F_mNack(void)
{
	CH2O_SDASet_H();
	F_Delay(7);
	CH2O_SCLSet_H();
	F_Delay(7);
	CH2O_SCLSet_L();
	F_Delay(7);
	CH2O_SDASet_L();
	F_Delay(7);
}
/**************************************************
*函数名称：void  F_CAck(void) 
*函数功能：应答检测
*入口参数：void
*出口参数：void  
**************************************************/
static void F_CAck()
{
	bit l_bAckFg = 0;
    U8 l_u8Cnt = 20;
    
	CH2O_SDASet_H();		
	CH2O_SDASet_Input();
	F_Delay(7);
	CH2O_SCLSet_H();		
	F_Delay(7);
    while(l_u8Cnt)
    {
        l_u8Cnt--;
        if(!CH2O_GetSDApin())
        {
            l_bAckFg = I2C_ACK_OK;
            break;
        }
        else if(!l_u8Cnt)
        {   
            l_bAckFg = I2C_ACK_ERR;
            break;
        }
    }
    CH2O_SCLSet_L();
	F_Delay(7);
	CH2O_SDASet_Output();
}
/**************************************************
*函数名称：void F_Write8Bit_Voc(void) 
*函数功能：写一个字节
*入口参数：ch:写入数据
*出口参数：void  
**************************************************/
static void F_Write8Bit(U8 ch)
{
	U8 uctemp;
									
	for (uctemp = 0; uctemp < 8; uctemp++ )
	{
		if (ch & 0x80)
		{
			CH2O_SDASet_H();
		}
		else
		{
			CH2O_SDASet_L();
		}
		ch = ch << 1;
        F_Delay(7);
		CH2O_SCLSet_H();							
        F_Delay(7);
		CH2O_SCLSet_L();							
        F_Delay(7);
	}
}
/**************************************************
*函数名称：void F_Read8Bit_Voc(void) 
*函数功能：读入一个字节
*入口参数：*p存放该字节的指针
*出口参数：void  
**************************************************/
static void F_Read8Bit(U8 *p)
{
	U8 uctemp;

	CH2O_SDASet_Input();
	for (uctemp = 0; uctemp < 8; uctemp++)
	{
		*p = *p << 1;			
		CH2O_SCLSet_H();
        F_Delay(7);
		if (CH2O_GetSDApin() == 1)
		{
			*p = *p | 0x01;		
		}
		CH2O_SCLSet_L();
        F_Delay(7);
	}
	CH2O_SDASet_Output();
}
/**************************************************
*函数名称：void F_ReadEeprom(void) 
*函数功能：连续读入多个字节到指定地址
*入口参数：*pData:数据,ucNum:长度
*出口参数：void  
**************************************************/
static void F_ReadEeprom(U8 *pData,U8 ucNum)
{
	F_StartBit();
  	F_Write8Bit(0xB3);
	F_CAck();								 		
	for ( ; ucNum != 1; ucNum--)
	{
		F_Read8Bit(pData);				
		pData++;
		F_mAck();
	}
	F_Read8Bit(pData);
	F_mNack();
	F_StopBit();
}
/**************************************************
*函数名称：void WriteEeprom(void) 
*函数功能：连续写入多个字节到指定地址
*入口参数：*pData:数据,ucNum:长度
*出口参数：void  
**************************************************/
static void F_WriteEeprom(U8 *pData,U8 ucNum)
{
	F_StartBit();
  	F_Write8Bit(0xB2);
	F_CAck();
	for ( ; ucNum != 0; ucNum--)
	{
		F_Write8Bit(*pData);	
		F_CAck();
		pData++;
	}
	F_StopBit();
}
/**************************************************
*函数名称：void Calc_CRC8(void) 
*函数功能：校验和计算
*入口参数：*message:数据,Num:长度
*出口参数：void  
**************************************************/
static U8 Calc_CRC8(U8 *message, U8 Num)
{
    U8 i;
    U8 byte;
    U8 crc = 0xFF;
    
    for(byte = 0; byte < Num; byte++)
    {
        crc ^= (message[ byte ]);
        for(i = 8; i > 0; --i)
        {
            if(crc & 0x80) 
            {   crc = ( crc << 1 ) ^ 0x31;}
            else 
            {   crc = ( crc << 1 );}
        }
    }
    return crc;
}
/**************************************************
*函数名称：void F_QueryVocData(void) 
*函数功能：查询CH2O传感器数据
*调用周期：1s
*入口参数：void
*出口参数：void  
**************************************************/
//void F_QueryCH2OData()
//{    
//    U16 l_u16Temp = 0;

//    if(g_u8CH2OLinkTime)g_u8CH2OLinkTime--;
//    
//    if(c_CH2O_READ_INIT == g_u8CH2OReadType)
//    {
//        F_WriteEeprom(&t_CH2OTaple,2);
//        g_u8CH2OReadType = c_CH2O_READ_CH2O_DATA;
//    }
////    else if(c_CH2O_READ_TEMP_HUMI == g_u8CH2OReadType)
////    {
////        F_SetStr(&CH2OReadBuf,0,6);
////        F_ReadEeprom(&CH2OReadBuf,6);
////        F_WriteEeprom(&t_CH2OTaple,2);
////        if(CH2OReadBuf[2] == Calc_CRC8(&CH2OReadBuf,2))
////        {
////            l_u16Temp = ((CH2OReadBuf[0] << 8) | CH2OReadBuf[1]);
////            if(l_u16Temp < 32767)
////            {   g_s8TempData = l_u16Temp / 100;}
////            else
////            {   g_s8TempData = (l_u16Temp - 65536) / 100;}
////            if(g_s8TempData > c_TEMP_MAX)
////            {   g_s8TempData = c_TEMP_MAX;}
////            else if(g_s8TempData < c_TEMP_MIN)
////            {   g_s8TempData = c_TEMP_MIN;}
////        }
////        if(CH2OReadBuf[5] == Calc_CRC8(&CH2OReadBuf[3],2))
////        {
////            l_u16Temp = ((CH2OReadBuf[3] << 8) | CH2OReadBuf[4]);
////            g_u8HumiData = l_u16Temp / 100;
////            if(g_u8HumiData > c_HUMI_MAX)
////            {   g_u8HumiData = c_HUMI_MAX;}
////            else if(g_u8HumiData < c_HUMI_MIN)
////            {   g_u8HumiData = c_HUMI_MIN;}
////        }
////        g_u8CH2OReadType = c_CH2O_READ_CH2O_DATA;
////    }
//    else
//    {
//        F_SetStr(&CH2OReadBuf,0,6);
//        F_ReadEeprom(&CH2OReadBuf,3);
//        F_WriteEeprom(&t_CH2OTaple,2);
//        if(CH2OReadBuf[2] == Calc_CRC8(&CH2OReadBuf,2))
//        {
//						g_u8CH2OLinkTime = c_CH2O_LINK_ERR_TIME;
//            g_u16CH2OData = (CH2OReadBuf[0] << 8) | CH2OReadBuf[1];
//            g_u16CH2OData = (U16)(((U32)g_u16CH2OData * 125) / 100);  //1ppb=1.25ug/m3
//            if(g_u16CH2OData > c_CH2O_DATA_MAX)
//            {   g_u16CH2OData = c_CH2O_DATA_MAX;}
//        }
//        g_u8CH2OReadType = c_CH2O_READ_CH2O_DATA;
//    }
//    //甲醛显示数据处理
//    if(g_u16CH2OData < 3)
//    {   g_u16DispCH2OData = t_DispCH2OData[g_u8DispCnt % 3];}
//    else
//    {   g_u16DispCH2OData = g_u16CH2OData;}
//}


void F_QueryCH2OData(void)
{
    U16 l_u16Temp = 0;

    /* —— 顶部统一变量 —— */
    static U16 s_u16Buf[CH2O_BUF_SIZE] = {0};
    static U32 s_u32Sum  = 0;
    static U8  s_u8Idx   = 0;
    static bit s_bReady  = 0;

    static U16 s_u16DispBak = 5;   /* 目标显示值(Bak) */
    static U8  s_u8Delay    = 0;   /* 延时计数 */
    static U8  s_u8DelaySet = 0;   /* 本轮延时阈值 */

    static U8  s_u8ErrNum   = 6;   /* 通讯错误时的个位循环值 6~9 */
    static U8  s_u8ErrTick  = 0;   /* 错误占位的节拍 */

    U16 l_u16Diff = 0;

    /* 链路计时 */
    if (g_u8CH2OLinkTime) g_u8CH2OLinkTime--;

    /* 状态机保持不变 */
    if (c_CH2O_READ_INIT == g_u8CH2OReadType)
    {
        F_WriteEeprom(&t_CH2OTaple, 2);
        g_u8CH2OReadType = c_CH2O_READ_CH2O_DATA;
    }
    else
    {
        F_SetStr(&CH2OReadBuf, 0, 6);
        F_ReadEeprom(&CH2OReadBuf, 3);
        F_WriteEeprom(&t_CH2OTaple, 2);
        if (CH2OReadBuf[2] == Calc_CRC8(&CH2OReadBuf, 2))
        {
            g_u8CH2OLinkTime = c_CH2O_LINK_ERR_TIME;
            g_u16CH2OData    = (U16)((CH2OReadBuf[0] << 8) | CH2OReadBuf[1]);
            g_u16CH2OData    = (U16)(((U32)g_u16CH2OData * 125U) / 100U);  /* 1ppb=1.25ug/m3 */
            if (g_u16CH2OData > c_CH2O_DATA_MAX) g_u16CH2OData = c_CH2O_DATA_MAX;
        }
        g_u8CH2OReadType = c_CH2O_READ_CH2O_DATA;
    }

    /* —— 通讯错误：显示 6~9 的个位循环值，立即生效 —— */
    if (0 == g_u8CH2OLinkTime)
    {
        if (++s_u8ErrTick >= 8)       /* 每 8 秒换一次数字，可按需改 */
        {
            s_u8ErrTick = 0;
            s_u8ErrNum  = (U8)((g_u8DispCnt % 4) + 6);  /* 6~9 循环 */
        }
        g_u16DispCH2OData = s_u8ErrNum;
        s_u16DispBak      = s_u8ErrNum;  /* 目标也设为同值，避免恢复时大跳变 */
        s_u8Delay         = 0;
        return;
    }

    /* —— 正常：生成目标显示值 Bak —— */
    if (g_u16CH2OData < 3)
    {
        /* 小于 3 时从预置数组按节拍取值 */
        s_u16DispBak = t_DispCH2OData[g_u8DispCnt % 3U];
    }
    else
    {
        if (s_bReady)
        {
            s_u32Sum -= s_u16Buf[s_u8Idx];
            s_u16Buf[s_u8Idx] = g_u16CH2OData;
            s_u32Sum += g_u16CH2OData;
            s_u8Idx = (U8)((s_u8Idx + 1) % CH2O_BUF_SIZE);
            s_u16DispBak = (U16)(s_u32Sum / CH2O_BUF_SIZE);
        }
        else
        {
            if (s_u8Idx < CH2O_BUF_SIZE)
            {
                s_u16Buf[s_u8Idx] = g_u16CH2OData;
                s_u32Sum += g_u16CH2OData;
                s_u8Idx++;
                s_u16DispBak = (U16)(s_u32Sum / s_u8Idx);
                if (s_u8Idx >= CH2O_BUF_SIZE) { s_u8Idx = 0; s_bReady = 1; }
            }
            else { s_u8Idx = 0; s_bReady = 1; }
        }
    }

    /* —— 两档延时跟随：到点把显示值对齐到 Bak —— */
    if (g_u16DispCH2OData >= s_u16DispBak) l_u16Diff = g_u16DispCH2OData - s_u16DispBak;
    else                                   l_u16Diff = s_u16DispBak - g_u16DispCH2OData;

    s_u8DelaySet = (l_u16Diff >= CH2O_FAST_THR) ? DELAY_FAST_SEC : DELAY_SLOW_SEC;

    if (++s_u8Delay >= s_u8DelaySet)
    {
        s_u8Delay = 0;
        g_u16DispCH2OData = s_u16DispBak;
    }
}