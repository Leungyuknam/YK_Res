#define	_Native_BEEP_H
#include "Include\Include.h"

void BuzzSet(u8 Type)
{
	BuzzSt.Set.Type = Type;
	BuzzSt.Set.Id ++;
}


void BeepSet(u8 Cnt,u8 OnT,u8 OffT,u8 Type)
{
    Bper.Cnt = Cnt; 
    Bper.OnT = OnT; 
    Bper.OffT = OffT; 
	Bper.GoT = 0; 
	Bper.BuzzNo = Type;
	B_BuzzEn = TRUE;
}


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






