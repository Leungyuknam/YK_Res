#include "h\Allinclude.h"
#include "c\IAP_Init\IAP_Init.h"

U8 xdata g_u8EepromBuf[c_u8SizeBuf] = {0};

bit g_bEepromRedFlg = 0;        //读标记

void F_EepromBufFillIn()//写
{
	U8 i;
    
	for(i=0; i<c_u8SizeBuf; i++)
	{
		g_u8EepromBuf[i] = 0;	
	}
	g_u8EepromBuf[0] = g_stSys.u8LightState;
//==========================校验和===================================
	for(i=0; i<c_u8SizeBuf-1; i++)
	{
		g_u8EepromBuf[c_u8SizeBuf-1] += g_u8EepromBuf[i];	
	}
	g_u8EepromBuf[c_u8SizeBuf-1] += 0xAA;

}

void F_EepromBufGet()//读
{	  
	g_stSys.u8LightState = g_u8EepromBuf[0];
}

void F_IAP_Write()
{
	U8 addr = 0;
    
  //======EEPROM 扇区X擦除========
    ssp_flag = 0xA5;
    EEPromSectorErase(0);
  //=====EEPROM 0x0100地址写值======
	for(addr = 0; addr < c_u8SizeBuf; addr++)
	{
        ssp_flag = 0x5A;
        EEPromByteProgram(0x01,addr,g_u8EepromBuf[addr]);//写数据
	}
}

void F_IAP_Read()
{
	U8 addr = 0;
    
	for(addr=0; addr<c_u8SizeBuf; addr++)
	{
		g_u8EepromBuf[addr] = EEPromByteRead(0x01,addr);//读数据
	}
}
void F_EepromWriteCtrl()
{
    EA = 0;
    IO_OFF();
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

    if(!g_bEepromRedFlg && g_stSys.bEepromEn)
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
    if(g_bEepromRedFlg && !g_stSys.bEepromWriteBan)
    {   return 1;}
    else
    {   return 0;}
}

void F_IapWriteCtrl()
{
	static U8 s_u8WriteStep = 0;
	static U8 s_u8LvdTime = 0;

	if(F_EnableWrite())
	{
        if(F_GetTimeTick(C_TIME_100ms))
        {	
            if(s_u8LvdTime < 0xff)s_u8LvdTime++;
        }
        
        if(!s_u8WriteStep)
        {
            if(LPDCON & 0x40)
            {
                if(s_u8LvdTime >= 20)//连续2s检测电压高于4.5V
                {
                    s_u8WriteStep = 1;
                    LPD_LOW_SET();
                }	
            }
            else
            {   s_u8LvdTime = 0;}
        }
        else
        {
            s_u8LvdTime = 0;
            if(LPDCON & 0x40)
            {
                s_u8WriteStep = 0;
                F_EepromWriteCtrl();
            }
        }            
	}
}
