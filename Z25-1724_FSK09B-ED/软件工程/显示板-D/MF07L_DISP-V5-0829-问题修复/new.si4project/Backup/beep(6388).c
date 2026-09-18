#define	_Native_BEEP_H
#include "Include\Include.h"

void BeepSet(u8 Type)
{
	BuzzSt.Set.Type = Type;
	BuzzSt.Set.Id ++;
}








