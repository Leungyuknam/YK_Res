#include "Include\Include.h"

/***************************************************************************** 
* 功能说明: 中断服务程序
*****************************************************************************/
void Timer0Int() interrupt 1
{
	TF0 = 0;
	#ifndef __Uart_DEBUG__
	F_Uart_Analog_IRQ();
	#endif
	TimeBaseTickIRQ();
	DispPIN_DR();
	TimeCountFrq();
//	P12 ^= 1;
//	P13 ^= 1;

	
}
