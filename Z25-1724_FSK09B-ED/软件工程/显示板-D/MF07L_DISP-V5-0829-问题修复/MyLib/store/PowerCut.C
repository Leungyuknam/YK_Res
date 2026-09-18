#define POWERCUT_C

#include "Include\Include.h"

#define	DECT_HIGH_VOLT		1
#define	DECT_LOW_VOLT		0

#define	EPPROM_LENGTH   7//10
uchar  xdata FlashSaveBuf[EPPROM_LENGTH + 2];


#define IapROM  0x00	  //0x00￡o????ROM??2ù×÷  0x02￡o????EEPROM??2ù×÷
#define IapEPPROM  0x02	  //0x00￡o????ROM??2ù×÷  0x02￡o????EEPROM??2ù×÷

void IAPWrite(uint Addr,uchar Value,uchar IAPArea);
uchar IAPRead(uint Addr,uchar IAPArea);




void F_FlashSaveData(void)
{
	/*
	u8 i =0;
	u8 r_CheckSum = 0;
	u8 *pSys = 0;

	FlashSaveBuf[0] = 0x5A;
	Sys.EppromCookRunMin = Sys.CookRunSec / 60;
	if(Sys.CookRunSec % 60)
		Sys.EppromCookRunMin ++;

	
	pSys = (u8 *)&Sys;
	for(i = 1; i< (EPPROM_LENGTH + 1); i++)
	{
		FlashSaveBuf[i] = *pSys;
		pSys ++;
	}
	for( i=0; i< EPPROM_LENGTH + 1; i++ )
	{	
		r_CheckSum += FlashSaveBuf[i];	
	}
	FlashSaveBuf[EPPROM_LENGTH+1] = 0xff - r_CheckSum;

	for(i = 0; i < EPPROM_LENGTH+2 ; i++)
	{
		IAPWrite(i,FlashSaveBuf[i],IapEPPROM);
	}
	*/
}

void F_ReadFlashData(void)
{
	U8 i =0;
	U8	r_CheckSum = 0;
		/*
	uchar *pSys;

	if(AD_Value >= 310)
	{
		for(i = 0; i<(EPPROM_LENGTH + 2); i++)
		{
			FlashSaveBuf[i] = IAPRead(i,IapEPPROM);
			r_CheckSum += FlashSaveBuf[i];
		}
		if(( r_CheckSum == 0xff ) &&   		\
			(FlashSaveBuf[0] == 0x5A) &&	\
			(FlashSaveBuf[1] && (FlashSaveBuf[1] <= MANDUN)) &&    \
			(FlashSaveBuf[2] == COOK_RUN_PAUSE) && \
			(!(FlashSaveBuf[3] & BIT_KEEP_WARM_SIGN)))
		{
			pSys = (uchar *)&Sys;
			for(i = 0; i < EPPROM_LENGTH; i++)
			{
				*pSys = FlashSaveBuf[i + 1];
				pSys ++;
			}
			*((u8 *)&Sys.Fg) &= BIT_KEEP_COOK_T_SIGN;

			//Sys.Fg.BooktimeEn = FALSE;
			if(Sys.Fg.CooktimeEn)
			{
				Sys.CookRunSec = Sys.EppromCookRunMin * 60;		
			}
			SysMode = SysMX_Run;
			F_DispTaskChange(DISP_NORMAL);
		}
	}
	else
	{
		for(i = 0; i < EPPROM_LENGTH+2 ; i++)
		{
			IAPWrite(i,0,IapEPPROM);
		}
	}
	*/
}


/*
void EppromProc(void)
{
	static u8 xdata FstInit = 0;
	static u8 xdata LastStoreDat[EPPROM_LENGTH] = 0;
//	static u8 xdata StoreDat[EPPROM_LENGTH] = 0;
	static u8 xdata AdChangeTime = 0;
	u8 *pSys;
	u8 i = 0,u8Buf = 0,u8Dat = 0;
	if(GetTimeTick(TIME_10MS))
	{
		if(0 == FstInit)
		{
			pSys = (uchar *)&Sys;
			for(i = 0; i < EPPROM_LENGTH; i++)
			{
				LastStoreDat[i] = *pSys;
//				StoreDat[i] = *pSys;
				pSys ++;
			}
			FstInit = 1;
			AdChangeTime = 0xff;
		}
		else
		{
			
			pSys = (uchar *)&Sys;
			
			for(i = 0; i < EPPROM_LENGTH; i++)
			{
				if(LastStoreDat[i] != *pSys)
				{
					
					AdChangeTime = 0;
					LastStoreDat[i] = *pSys;
				}
				pSys ++;
			}
			
		
			if(AdChangeTime < 255)
				AdChangeTime ++;
			if(100 == AdChangeTime)
			{
//				u8Buf = 0;
//				
//				for(i = 0;i < EPPROM_LENGTH;i++)
//				{
//					if(StoreDat[i] != LastStoreDat[i])
//					{
//						u8Buf = 1;			//跟已保存数据不同
//						StoreDat[i] = LastStoreDat[i];
//						//BUZZ();
//					}
//				}
//				if(1 == u8Buf)
				{
					EA = 0;
					F_FlashSaveData();
					EA = 1;
				}
			}
		}
	}
}
*/



