/*********************************************************
文 件 名 : CRC_Check.C
文件说明 : CRC校验
编写作者 : thm
修订作者 : 
编写时间 : 2021-2-24
版 本 号 : V1.0
==========================================================*/
//头文件
#include "COMMON\AllInclude.h"


/*****************************************************
*函数名称：u8 F_REVERSE_OneByte(u8 l_u8Byte)
*函数功能：按位反转数据
*输入参数：l_u8Byte（待转换数据）
*输出参数：l_u8OutByte（转换后数据）
*说明：
			转换前：1101 0101
			转换后：1010 1011
*****************************************************/
static u8 F_REVERSE_OneByte(u8 l_u8Byte)
{
	u8	l_u8OutByte = 0;
	u8  i = 0;

	for (i = 0; i < 8; i++)
	{
		l_u8OutByte >>= 1;
		if (l_u8Byte & 0x80)
		{
			l_u8OutByte |= 0x80;		 	
		}
		l_u8Byte <<= 1;
	}

	return l_u8OutByte;
}


/*****************************************************
*函数名称：u16 F_REVERSE_OneWord(u16 l_u16Word)
*函数功能：按位反转数据
*输入参数：l_u16Word（待转换数据）
*输出参数：l_u16OutWord（转换后数据）
*说明：
			转换前：1101 0101 1111 0001
			转换后：1000 1111 1010 1011
*****************************************************/
static u16 F_REVERSE_OneWord(u16 l_u16Word)
{
	u16	l_u16OutWord = 0;
	u8  i = 0;

	for (i = 0; i < 16; i++)
	{  
		l_u16OutWord >>= 1;
		if (l_u16Word & 0x8000)
		{
			l_u16OutWord |= 0x8000;		 	
		}
		l_u16Word <<= 1;
	}

	return l_u16OutWord;
}

/*****************************************************
*函数名称：u8 F_CRC16_Check(u8 *pTable, u8 l_u8CheckLength)
*函数功能：校验CRC
*输入参数：pTable（待校验列表）
		   l_u8CheckLength（校验数据长度）
*输出参数：校验成功（返回1）
		   校验失败（返回0）
*说明：
（1）、预置1个16位的寄存器为十六进制FFFF（即全为1），称此寄存器为CRC寄存器；
（2）、把第一个8位二进制数据（既通讯信息帧的第一个字节）与16位的CRC寄存器的低
       8位相异或，把结果放于CRC寄存器，高八位数据不变；
（3）、把CRC寄存器的内容右移一位（朝低位）用0填补最高位，并检查右移后的移出位；
（4）、如果移出位为0：重复第3步（再次右移一位）；如果移出位为1，CRC寄存器与多
       项式A001（1010 0000 0000 0001）进行异或；
（5）、重复步骤3和4，直到右移8次，这样整个8位数据全部进行了处理；
（6）、重复步骤2到步骤5，进行通讯信息帧下一个字节的处理；
（7）、将该通讯信息帧所有字节按上述步骤计算完成后，得到的16位CRC寄存器的高、低
       字节进行交换；
（8）、最后得到的CRC寄存器内容即为：CRC码。

以上计算步骤中的多项式A001是8005按位颠倒后的结果。
*****************************************************/
#if  Def_CRC16
u8 F_CRC16_Check(u8 *pTable, u8 l_u8CheckLength)
{
	u8 	  i = 0;
	u8    j = 0;
	u8    l_u8InData = 0;
	u16   l_u16PolyValue = 0;
	u16	  l_u16Original_CRC = 0;
	u16   l_u16Calculate_CRC = C_INIT;
	
#if (Def_REFPOLY)
	l_u16PolyValue = F_REVERSE_OneWord(C_POLY);
#else
	l_u16PolyValue = (C_POLY);		
#endif

	for (i = 0; i < l_u8CheckLength; i++)
	{

#if (Def_REFIN)
		l_u8InData = F_REVERSE_OneByte(*pTable++);	
#else
		l_u8InData = (*pTable++);
#endif

		l_u16Calculate_CRC ^= (l_u8InData & 0x00FF);

		for (j = 0; j < 8; j++)
		{
			if (l_u16Calculate_CRC & 0x0001)
			{
				l_u16Calculate_CRC >>= 1;
				l_u16Calculate_CRC ^= l_u16PolyValue;
			}
			else
			{ 
				l_u16Calculate_CRC >>= 1;
			}
		}
	}
	
#if (Def_REFOUT)
	l_u16Calculate_CRC = F_REVERSE_OneWord(l_u16Calculate_CRC);	
#else
	l_u16Calculate_CRC = (l_u16Calculate_CRC);	
#endif

#if (Def_NEEDXOR)
	l_u16Calculate_CRC ^= C_XOROUT;		
#endif

	l_u16Original_CRC = *pTable++;
	l_u16Original_CRC += (*pTable) << 8;

	if (l_u16Original_CRC == l_u16Calculate_CRC)
	{
		return 1;
	}
	
	return 0;


//  unsigned int i,sum;
//  unsigned char j;
//  unsigned int crc = 0xFFFF;
//  for (i=0; i<len; i++)
//  {
//    crc ^= ((*pdat++) & 0x00FF);
//    for (j=0; j<8; j++)
//    {
//      if (crc & 0x0001)
//      {
//        crc >>= 1;
//        crc ^= 0xA001;
//      }
//      else
//      { 
//        crc >>= 1;
//      }
//    }
//  }
//  
//  sum = *pdat++;
//  sum += (*pdat) << 8;
//  if (sum == crc) return true;
//
//
//  return false;

}
#endif

