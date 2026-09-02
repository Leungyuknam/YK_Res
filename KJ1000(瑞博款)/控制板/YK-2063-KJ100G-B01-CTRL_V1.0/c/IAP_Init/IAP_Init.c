#include "h\Allinclude.h"

#define IapROM  0x00	  //0x00：选择ROM区操作  0x02：选择EEPROM区操作
#define IapEPPROM  0x02	  //0x00：选择ROM区操作  0x02：选择EEPROM区操作

/**************************************************
*函数名称：unsigned char IAP_Read(unsigned long Add,unsigned char Iapade)
*函数功能：单Byte读取
*入口参数：Add ：地址
*          Iapade:操作对象 APROM为00，EEPROM为02
*出口参数：void
**************************************************/
unsigned char IAP_Read(unsigned long Add,unsigned char Iapade)
{
	unsigned char xdata IAP_IapData;
    unsigned char code *point = 0;
    //保存IAPADE、EA
	unsigned char tempADER = IAPADE;
    unsigned char tempEA = EA;

	EA = 0; //关闭中断

	IAPADE = Iapade;  
	IAP_IapData = *(point+Add);
    
    //恢复IAPADE、EA，避免MOVC位置出错
	IAPADE = tempADER;    
	EA = tempEA;
    
	return IAP_IapData;
}
