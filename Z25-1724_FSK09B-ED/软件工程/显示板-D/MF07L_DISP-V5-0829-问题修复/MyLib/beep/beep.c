#define	_Native_BEEP_H
#include "Include\Include.h"







void BeepSet(u8 Cnt,u8 OnT,u8 OffT)
{
    Bper.Cnt = Cnt; 
    Bper.OnT = OnT; 
    Bper.OffT = OffT; 
	  Bper.GoT = 0; 

	  B_BuzzEn = TRUE;
	//SET_CON_OUT;  //使IO输出
}


//void BeepSetOFF()
//{
//	Bper.Cnt = 0;
//	B_BuzzEn = 0;
//}

void BeepDeal(void)
{
    if(Bper.Cnt)
    {
        if(++Bper.GoT == Bper.OnT)     		
        {
			B_BuzzEn = FALSE;
		}
        else if(Bper.GoT >= (Bper.OnT+Bper.OffT))	
        {
        	if(--Bper.Cnt > 0)
			{
//				BOT_NTC_BUZZ_SET_OUT();	//使IO输出
				B_BuzzEn = TRUE;
			}
			else
			{
				//使输入
				//SET_CON_IN;
			}
            Bper.GoT = 0;
        }
    }
}

void BeepRing(void)
{
	if(B_BuzzEn)
	{
		RING_BUZZING;
	}
	else
	{
		RING_BUZZ_OFF;
	}
}















