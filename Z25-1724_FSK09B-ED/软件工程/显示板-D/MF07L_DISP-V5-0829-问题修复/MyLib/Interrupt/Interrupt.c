#include "Include\Include.h"

/***************************************************************************** 
* 功能说明: 中断服务程序
*****************************************************************************/
void Timer0Int() interrupt 1
{
	TF0 = 0;
	BeepRing();
	F_IrReceive();
	TimeBaseTickIRQ();
//	DispPIN_DR();
	TimeCountFrq();


	
}
