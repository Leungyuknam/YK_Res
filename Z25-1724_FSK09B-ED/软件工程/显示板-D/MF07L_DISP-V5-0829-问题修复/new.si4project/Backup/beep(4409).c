#define	_Native_BEEP_H
#include "Include\Include.h"

void BuzzSet(u8 Type)
{
	BuzzSt.Set.Type = Type;
	BuzzSt.Set.Id ++;
}


void BeepSet(u8 Cnt,u8 OnT,u8 OffT,u8 type)
{
    Bper.Cnt = Cnt; 
    Bper.OnT = OnT; 
    Bper.OffT = OffT; 
	Bper.GoT = OnT + OffT; 
    Bper.Btype = type;
	//SET_CON_OUT;  //Ê¹IOÊä³ö
}


void BeepDeal(void)
{
    if (GetTimeTick(TIME_10MS) == FALSE)
		return;
    if(Bper.Cnt)
    {
        if(++Bper.GoT == Bper.OnT)     		
        {
			B_BuzzEn = FALSE;
		}
        else if(Bper.GoT >= (Bper.OnT+Bper.OffT))	
        {
        	if(Bper.Cnt > 0)
			{
                Bper.Cnt  --;
				BuzzSet(Bper.Btype);
				/*
                Buzz.Set.Type = Bper.Btype;  //³¤Ãù
                if (++Buzz.Set.Id >= 4)
                {
                    Buzz.Set.Id = 0;
                }
                Buzz_Cnt = 0;
				B_BuzzEn = TRUE;
				*/
			}
            Bper.GoT = 0;
        }
    }
}









