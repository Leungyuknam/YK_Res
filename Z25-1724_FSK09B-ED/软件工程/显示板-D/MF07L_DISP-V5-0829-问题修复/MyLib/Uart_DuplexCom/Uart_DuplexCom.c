#include "Include\Include.h"


U8 xdata ControlDisConnect_Cnt = 0;




xdata U8 g_u8FanSpeed;

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

/******************************************************************
 函数名称：						MakeEnCode();
 功能描述：						发送的数据处理
 其他说明：						无
*******************************************************************/
U8 MakeEnCode()
{


//	Uart1TXDBuf[0] = g_sIrRecData.Bytes.CmdReverse;
//	Uart1TXDBuf[1] = g_sIrRecData.Bytes.Cmd;
//	Uart1TXDBuf[2] = g_sIrRecData.Bytes.ClientCodeReverse;
//	Uart1TXDBuf[3] = g_sIrRecData.Bytes.ClientCode;
	
	Uart1TXDBuf[0] = *((u8*)&Out.LeftRightAngleCol);
	Uart1TXDBuf[1] = Out.LeftRightFarAngleVal;
	Uart1TXDBuf[2] = Out.LeftRightFarAngleVal>>8;
	Uart1TXDBuf[3] = Out.LeftRightNearAngleVal;
	Uart1TXDBuf[4] = Out.LeftRightNearAngleVal>>8;

	Uart1TXDBuf[5] = *((u8*)&Out.UpDnAngleCol);
	Uart1TXDBuf[6] = Out.UpDnFarAngleVal;
	Uart1TXDBuf[7] = Out.UpDnFarAngleVal>>8;
	Uart1TXDBuf[8] = Out.UpDnNearAngleVal;
	Uart1TXDBuf[9] = Out.UpDnNearAngleVal>>8;

	Uart1TXDBuf[10] = Out.mFanSpeed;
	
	Uart1TXDBuf[11] = Out.LRHallState;
	Uart1TXDBuf[TXD1_BUF_LENGTH-1] = F_CheckNumCrc8(Uart1TXDBuf,TXD1_BUF_LENGTH-1);
	return TXD1_BUF_LENGTH;
}

void DeCodeProc(U8 l_u8ByteNum)
{
	static unsigned char xdata RxUartCmpBuf[5] = 0;
	static unsigned char xdata u8DatSame_Cnt = 0;
	if(l_u8ByteNum == RXD1_BUF_LENGTH)
	{
        if(Uart1RXDBuf[RXD1_BUF_LENGTH-1] == F_CheckNumCrc8(Uart1RXDBuf,RXD1_BUF_LENGTH-1))
        {
			if(TRUE == CmpStrAndCopy(&RxUartCmpBuf[0],&Uart1RXDBuf[0],5))
			{
				ControlDisConnect_Cnt = 0;
				if(u8DatSame_Cnt < 255)
					u8DatSame_Cnt ++;
				if(1 == u8DatSame_Cnt)
				{
					*((U8 *)&Out.LeftRightAngleState) = Uart1RXDBuf[0];
					*((U8 *)&Out.UpDnAngleState) = Uart1RXDBuf[1];
					*((U8 *)&Sys.PowMessage) = Uart1RXDBuf[2];
					g_u16Bat_Vol = (u16)(Uart1RXDBuf[4]<<8) + Uart1RXDBuf[3];
				}
				
			}
			else
				u8DatSame_Cnt = 0;
			ControlDisConnect_Cnt = 0;
			return;
        }
    }
}

void F_CheckCommucate(void)
{
	if(GetTimeTick(TIME_1S))
	{
		if(ControlDisConnect_Cnt != 0xff)ControlDisConnect_Cnt ++;
   	}
	if(ControlDisConnect_Cnt >= 5)
	{
		OutPutAllOFF();
	}
}