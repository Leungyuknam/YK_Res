#include "h\Allinclude.h"

#define SDASet_Input()		{P0CR &= ~BIT0;}
#define SDASet_Output()		{P0CR |= BIT0;}
#define SCLSet_Input()		{P0CR &= ~BIT1;}
#define SCLSet_Output()		{P0CR |= BIT1;}

#define SDASet_H()			{P_VOC_SDA = 1;}
#define SDASet_L()			{P_VOC_SDA = 0;}
#define	GetSDApin()			(P_VOC_SDA)

#define SCLSet_H()			{P_VOC_SCL = 1;}
#define SCLSet_L()			{P_VOC_SCL = 0;}

#define	I2C_ACK_ERR			1
#define	I2C_ACK_OK			0

U8 xdata g_u8VocLinkTime = c_VOC_LINK_ERR_TIME;//VOC通讯时间

U8 xdata g_u8VocReadType = 0;//VOC读取类型

U16 xdata g_u16VocData = 0;//VOC数值 0~500对应0~5ppm

U8 code t_VocTaple[2] = {0x26,0x0F};//测量VOC值命令码

U8 xdata VocReadBuf[c_VOC_LENGTH_MAX] = {0};

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
	SCLSet_Output();
	SDASet_Output();
	SCLSet_H();
	SDASet_H();
	F_Delay(7);
	SDASet_L();
	F_Delay(7);
	SCLSet_L();
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
	SDASet_L();
	F_Delay(7);
	SCLSet_H();
	F_Delay(7);
	SDASet_H();
	F_Delay(7);
	SCLSet_Input();
	SDASet_Input();
}
/**************************************************
*函数名称：void  F_mAck(void) 
*函数功能：应答
*入口参数：void
*出口参数：void  
**************************************************/
static void F_mAck()
{
	SDASet_L();
	F_Delay(7);
	SCLSet_H();
	F_Delay(7);
	SCLSet_L();
	F_Delay(7);
	SDASet_H();
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
	SDASet_H();
	F_Delay(7);
	SCLSet_H();
	F_Delay(7);
	SCLSet_L();
	F_Delay(7);
	SDASet_L();
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
    
	SDASet_H();		
	SDASet_Input();
	F_Delay(7);
	SCLSet_H();		
	F_Delay(7);
    while(l_u8Cnt)
    {
        l_u8Cnt--;
        if(!GetSDApin())
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
    SCLSet_L();
	F_Delay(7);
	SDASet_Output();
    
//    if(I2C_ACK_OK == l_bAckFg)
//	{   g_u8VocLinkTime = c_VOC_LINK_ERR_TIME;}
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
			SDASet_H();
		}
		else
		{
			SDASet_L();
		}
		ch = ch << 1;
        F_Delay(7);
		SCLSet_H();							
        F_Delay(7);
		SCLSet_L();							
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

	SDASet_Input();
	for (uctemp = 0; uctemp < 8; uctemp++)
	{
		*p = *p << 1;			
		SCLSet_H();
        F_Delay(7);
		if(GetSDApin() == 1)
		{
			*p = *p | 0x01;		
		}
		SCLSet_L();
        F_Delay(7);
	}
	SDASet_Output();
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
*函数功能：查询VOC传感器数据
*调用周期：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_QueryVocData()
{    
    if(g_u8VocLinkTime)g_u8VocLinkTime--;
    
    if(c_VOC_READ_INIT == g_u8VocReadType)
    {
        F_WriteEeprom(&t_VocTaple,2);
        g_u8VocReadType = c_VOC_READ_VOC_DATA;
    }
    else
    {
        F_SetStr(&VocReadBuf,0,3);
        F_ReadEeprom(&VocReadBuf,3);
        F_WriteEeprom(&t_VocTaple,2);
        if(VocReadBuf[2] == Calc_CRC8(&VocReadBuf,2))
        {
            g_u8VocLinkTime = c_VOC_LINK_ERR_TIME;
            g_u16VocData = (VocReadBuf[0] << 8) | VocReadBuf[1];
            if(g_u16VocData > c_VOC_DATA_MAX)
            {   g_u16VocData = c_VOC_DATA_MAX;}
        }
        g_u8VocReadType = c_VOC_READ_VOC_DATA;
    }
}