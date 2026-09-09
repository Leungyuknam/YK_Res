#define CodeI2cDrv
//#include "Define.h" 
#include "h\Allinclude.h"
//SH79F1621/3283 /1618:防窜货码存放地址0x830，SN码地址 0x840
//SH79F9260:10位防窜码地址  0x1030，SN地址0x1040  
#if WIFI_SN_EN
#define ID_ADDR		0x1040
#else
#define ID_ADDR		0x1030
#endif

#define CBYTE ((unsigned char volatile code *) 0)

uchar ucI2cData;
uchar ucI2cSum;

void delayId(uint data k)
{
	uint data i;
	for(i = 0; i < k; i++)
	{
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	}
}

uchar GetFactoryID(uchar idx)
{
	uchar data id;
	FLASHCON = 0x01;//buf1~buf5 读取SH79F081A FLASH BANK1隐藏地址5Byte BCD防串货号
	id = CBYTE[ID_ADDR+idx];		
	FLASHCON = 0x00;//取完ROM里面的防串货号后恢复回去ROM BANK0
	return id;
}
/*******************************************************************
FUNCTION NAME:void	write8Bit(uchar ch)           
FUNCTION:读取8位数据
INPUT:	null
RETURN:	null			                             
********************************************************************/
void write8Bit(uchar ch)
{
	uchar i;	
	P_E2SDA_OUTMODE();
	P_E2SCL_OUTMODE();
	for(i = 0; i < 8; i++ )
	{
		if((ch & 0x80) == 0x80)
		{
			P_E2SDA_SETB();
		}
		else
		{
			P_E2SDA_CLRB();
		}
		delayId(40);//40us
		ch = ch << 1;
		P_E2SCL_SETB();
        	delayId(40);//40us
		P_E2SCL_CLRB();
	}	
}
/*******************************************************************
FUNCTION NAME: startBit(void)               
FUNCTION:启始位
INPUT:	null
RETURN:	null			                             
********************************************************************/
void startBit(void)
{
	P_E2SDA_INMODE();
	P_E2SCL_INMODE();
   	delayId(1000);

	P_E2SDA_OUTMODE();	
	P_E2SDA_SETB();
	P_E2SCL_OUTMODE();	
	P_E2SCL_SETB();
    	delayId(100);
	P_E2SDA_CLRB();
	delayId(100);
	P_E2SCL_CLRB();
	delayId(100);		
}
/*******************************************************************
FUNCTION NAME: stopBit(void)                   
FUNCTION:停止位
INPUT:	null
RETURN:	null			                             
********************************************************************/
void stopBit(void)
{
	P_E2SDA_OUTMODE();	
	P_E2SDA_CLRB();
	delayId(40);//40us
	P_E2SCL_SETB();
	delayId(40);//40us
	P_E2SDA_SETB();	
	delayId(2000-2);//10ms
}
/*******************************************************************
FUNCTION NAME:void	cAck(void)                       
FUNCTION:IIC应答
INPUT:	null
RETURN:	null			                             
********************************************************************/
void cAck(void)
{				
	delayId(40);
	P_E2SDA_INMODE();
	if(P_E2SDA_READ())
	{
		//fgNoAck = 1;					
	}	
	delayId(40);
	P_E2SCL_SETB();
    	delayId(40);
	P_E2SCL_CLRB();
	P_E2SDA_OUTMODE();
}
/*******************************************************************
FUNCTION NAME:CodeWriteProc                          
FUNCTION:防串码数据输出
INPUT:	null
RETURN:	null			                             
********************************************************************/
void CodeWriteProc(void)
{
	uchar ucTmp_i;
	#if WIFI_SN_EN
	startBit();
	ucI2cSum=0;
	for(ucTmp_i = 0;ucTmp_i < 35;ucTmp_i++ )
	{		
		if(0 == ucTmp_i)
		{
			write8Bit(0xA5);
		}
		else if(ucTmp_i < 33)//SN 防串码
		{
			ucI2cData= GetFactoryID(ucTmp_i-1);     
			ucI2cSum = ucI2cData + ucI2cSum;
			write8Bit(ucI2cData);
		}
		else if(ucTmp_i == 33)
		{
			write8Bit(ucI2cSum);
		}
		else if(ucTmp_i == 34)
		{
			write8Bit(0xD5);
		}
		cAck();	
	}
	#else
	startBit();
	ucI2cSum = 0;
	for(ucTmp_i = 0;ucTmp_i < 8;ucTmp_i++ )
	{
		if(0 == ucTmp_i)
		{
			write8Bit(0xfe);
		}
		else if(ucTmp_i < 6)//ID方串码
		{
			ucI2cData = GetFactoryID(5-ucTmp_i); // 4 3 2 1 0
			ucI2cSum = ucI2cData + ucI2cSum;
			write8Bit(ucI2cData);
		}
		else if(ucTmp_i == 6)
		{
			write8Bit(ucI2cSum);
		}
		else if(ucTmp_i == 7)
		{
			write8Bit(0xD5);
		}
		cAck();
	}
	#endif
	stopBit();
}
/*******************************************************************
						END	
*******************************************************************/
