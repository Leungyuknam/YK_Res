#include "Include\Include.h"


#define C_HEAD_CODE 0x5A


static unsigned char xdata RecOKTime = 0;
unsigned char xdata UartMissCnt = 0;
unsigned char xdata u8DatSame_Cnt;
unsigned char xdata ControlDisConnect_Cnt = 0; 




code unsigned char crc8_854_table[] = // 2014042601 
{ 	 
	0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
	157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
	35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
	190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
	70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
	219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
	101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
	248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
	140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
	17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
	175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
	50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
	202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
	87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
	233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
	116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
}; 



unsigned char F_CheckNumCrc8(unsigned char *StartAddr,unsigned int size)
{
	unsigned int ucTmp = 0;  
	unsigned char ucCheckSum = 0;  
	ucCheckSum = 0x5a;  
	for(ucTmp = 0; ucTmp < size; ucTmp++)
	{  
		ucCheckSum = crc8_854_table[ucCheckSum^(*(StartAddr+ucTmp))];  
	}  
	return(ucCheckSum); 
}

u8 CmpStrAndCopy(unsigned char *TargetStr, unsigned char *SourceStr, unsigned char Count)
{
	u8 u8temp0 = 1;
	while (Count)
	{
		Count--;
		if(*TargetStr != *SourceStr)
		{
			u8temp0 = 0;
		}
		*TargetStr = *SourceStr;
		TargetStr++;
		SourceStr++;
	}
	
	return u8temp0;
}


/********************************************************************************************************
* @ name   : unsigned char DeCodeProc(unsigned char pUartBuf[], unsigned char Length)
* @ msg    : 数据解码
* @ param   (unsigned char) pUartBuf        解码数据
* @ param   (unsigned char) Length          个数
* @ return  (unsigned char) DECODE_FAIL     失败
* @ return  (unsigned char) DECODE_PASS     成功
 ********************************************************************************************************/

unsigned char DeCodeProc(unsigned char pUartBuf[], unsigned char Length)
{
    unsigned char Result;
	static unsigned char RxUartCmpBuf[C_REC_LENGTH] = 0;
    Result = C_DECODE_FAIL;
    Length--;
    if (pUartBuf[Length] == F_CheckNumCrc8(pUartBuf, Length))
    {
         Result = C_DECODE_PASS;
		if(TRUE == CmpStrAndCopy(&RxUartCmpBuf[0],&pUartBuf[0],C_REC_LENGTH - 3)) //比较去掉校验一个字节+ g_u16Voltage两个字节
		{
			ControlDisConnect_Cnt = 0;
			if(u8DatSame_Cnt < 255)
				u8DatSame_Cnt ++;
			if(1 == u8DatSame_Cnt)
			{
				//if(pUartBuf[0] == 0xa5)
				{
					Sys.TempAD = (u16)(pUartBuf[2]<<8);
					Sys.TempAD |= pUartBuf[1];
					//Sys.TempAD >>= 2;
				}
				*((u8 *)&Sys.PowMessage) = pUartBuf[3];
			}
		}
		else
			u8DatSame_Cnt = 0;

    }

    if (Result != C_DECODE_FAIL)
    {
        RecOKTime = 5;
        UartMissCnt--;
    }
    return Result;
}





/********************************************************************************************************
* @ name   : unsigned char MakeEnCode(unsigned char UartBuf[])
* @ msg    : 数码发送
* @ param   (unsigned char) UartBuf     发送数据
* @ return  (unsigned char) Count       发送个数
 ********************************************************************************************************/

unsigned char MakeEnCode(unsigned char UartBuf[])
{

	unsigned char Buf = 0;
	UartBuf[0] = 0x5a;
	UartBuf[1] = SysLoad.Byte;
	UartBuf[2] = BuzzSt.Byte;
	UartBuf[3] = F_CheckNumCrc8(UartBuf,3);
    return 4;
}

