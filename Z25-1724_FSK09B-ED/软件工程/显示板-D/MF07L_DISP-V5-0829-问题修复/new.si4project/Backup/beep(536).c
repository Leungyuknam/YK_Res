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
				//SET_CON_OUT;  //使IO输出
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
 
void F_BellCtrl()
{
	if (GetTimeTick(TIME_10MS))
	{
		//P_BUZZ = 1;
		if(BuzzSt.Set.Type != 0)
		{
			if(BuzzSt.Byte != LastBuzzSt.Byte)
			{
				LastBuzzSt.Byte = BuzzSt.Byte;
				
				if(BUZZ_NOR == BuzzSt.Set.Type)
				{
					BELL_NOR();
				}
				else if(BUZZ_ON == BuzzSt.Set.Type)
				{
					BELL_ON();
				}
				else if(BUZZ_WARN == BuzzSt.Set.Type)
				{
					BELL_WARN();
				}
				else if(BUZZ_ERR == BuzzSt.Set.Type)
				{
					BELL_ERR();
				}
				else if(BUZZ_FINISH == BuzzSt.Set.Type)
				{
					BELL_FINISH();
				}
				else if(BUZZ_KEY_WARN == BuzzSt.Set.Type)
				{
					BELL_KEY_WARN();
				}
				else if(BUZZ_FST_POWER_ON == BuzzSt.Set.Type)
				{
					BELL_FST_POWER_ON();
				}
				else if(BUZZ_HALF_TIME_WARN == BuzzSt.Set.Type)
				{
					BELL_HALF_TIME_WARN();
				}
				else if(BUZZ_LOCK == BuzzSt.Set.Type)
				{
					BELL_LOCK();
				}
			}
			BuzzSt.Set.Type = 0;
		}
		BeepDeal();
	}
}

void BeepRing(void)
{
	static u8 X_REG RingCnt = 0;
	if( ++ RingCnt >= 2)
	{
		RingCnt = 0;
		if(B_BuzzEn)
		{
			//RING_BUZZING;
		}
		else
		{
			//RING_BUZZ_OFF;
		}
	}

}




