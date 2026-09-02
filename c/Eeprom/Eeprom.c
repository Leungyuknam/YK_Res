#include "h\Allinclude.h"
#include "c\IAP_Init\IAP_Init.h"

U8 xdata g_u8EepromBuf[c_u8SizeBuf] = {0};
U16 g_u16PowerADValue = 0;

bit g_bEepromEn = 0;//掉电记忆使能
bit g_bEepromRedFlg = 0;        //读标记
bit g_bEepromWriteBan = 0;      //写禁止控制

void F_EepromBufFillIn()//写
{
	U8 i;
    
	for(i=0; i<c_u8SizeBuf; i++)
	{
		g_u8EepromBuf[i] = 0;	
	}
	g_u8EepromBuf[0] = g_u16ScreenRunTime & 0xff;
	g_u8EepromBuf[1] = g_u16ScreenRunTime >> 8;
//    g_u8EepromBuf[4] = g_u8WorkMode & 0x0f;
//    if(g_bTurnOn)
//	{   g_u8EepromBuf[4] |= BIT1;}
//==========================校验和===================================
	for(i=0; i<c_u8SizeBuf-1; i++)
	{
		g_u8EepromBuf[c_u8SizeBuf-1] += g_u8EepromBuf[i];	
	}
	g_u8EepromBuf[c_u8SizeBuf-1] += 0xAA;

}

void F_EepromBufGet()//读
{	  
    g_u16ScreenRunTime = g_u8EepromBuf[0];
    g_u16ScreenRunTime |= g_u8EepromBuf[1] << 8;
//    g_u8WorkMode = g_u8EepromBuf[4] & 0x0f;
//    if(g_u8WorkMode > MODE_AI)
//    {   g_u8WorkMode = MODE_LOW;}
//    if(g_u8EepromBuf[4] & BIT1)
//    {	g_bTurnOn = 1;}
}

void F_IAP_Write()
{
	U8 addr = 0;
    
    IAP_EEPROMSectorEraseOption(EEPROM_ADDRESS);   //页擦除
	for(addr = 0; addr < c_u8SizeBuf; addr++)
	{
		IAP_EEPROMProgramByteOption(EEPROM_ADDRESS + addr, g_u8EepromBuf[addr]);			//往EEPROM addr地址写入g_u8EepromBuf[addr]
	}
}

void F_IAP_Read()
{
	U8 addr = 0;
    
	for(addr=0; addr<c_u8SizeBuf; addr++)
	{
		g_u8EepromBuf[addr] = IAP_Read(EEPROM_ADDRESS + addr, IapEPPROM);		//读取EEPROM addr地址数据
	}
}
void F_EepromWriteCtrl()
{
    EA = 0;
//    IO_OFF();
    EEPRON_TEXT_IO_SET;
    F_EepromBufFillIn();
    F_IAP_Write();
    EEPRON_TEXT_IO_CLR;
    EA = 1;
}
void F_EepromReadCtrl()
{
	U8 i = 0;
	U8 l_u8CheckSum = 0;

    if(!g_bEepromRedFlg && g_bEepromEn)
    {
        if(F_GetTimeTick(C_TIME_500ms))
        {
            g_bEepromRedFlg = 1;
            F_IAP_Read();
            for(i=0; i<c_u8SizeBuf-1; i++)	
            {	l_u8CheckSum += g_u8EepromBuf[i];}
            l_u8CheckSum += 0xAA;
            if(l_u8CheckSum == g_u8EepromBuf[c_u8SizeBuf-1])	
            {	F_EepromBufGet();}
        }
    }
}

U8 F_EnableWrite()//  //1:允许写 0：禁止写
{
    if(g_bEepromRedFlg && !g_bEepromWriteBan)
    {   return 1;}
    else
    {   return 0;}
}

void F_IapWriteCtrl()
{
	static U8 s_u8WriteStep = 0;

	if(F_EnableWrite())
	{
		if(g_u8MCURunTime >= 3)
		{
            g_u16PowerADValue = ADC_Read(AIN_VDD_D4);//8位AD
            if(g_u16PowerADValue <= 125)//4.02V 参考电压2.048
            {
                if(s_u8WriteStep == 0)
                {
                    s_u8WriteStep = 30;
                    F_EepromWriteCtrl();	
                }
            }  
		}
	}
    if(F_GetTimeTick(C_TIME_100ms))
    {	
        if(g_u16PowerADValue >= 143)//4.59V 参考电压2.048
        {
            if(s_u8WriteStep)
            {   s_u8WriteStep--;}
        }	
        else
        {
            if(s_u8WriteStep)
            {   s_u8WriteStep = 30;}
        }
    }
}
