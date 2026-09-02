//======================================================
// interrupt routines
//======================================================
#include "h\Allinclude.h"

/**************************************************
*函数名称：void timer0()interrupt 1 
*函数功能：定时器中断服务函数
*入口参数：void
*出口参数：void 
**************************************************/
//void INT_Timer0() interrupt 1
//{
//	TF0 = 0;        //溢出清零

////	F_Buzzer_IRQ();
//    F_DisplayDrive();
//    F_TimeBaseTick_IRQ();
//}
/**************************************************
*函数名称：void timer3()interrupt 13 
*函数功能：定时器中断服务函数
*入口参数：void
*出口参数：void 
**************************************************/
void INT_Timer3() interrupt 13
{
    U8 TXINX_STACK = TXINX;
    
    TXINX = 0x03;   //选择定时器T3
	TFX = 0;        //溢出清零
    TXINX = TXINX_STACK;  //还原所选的定时器

    DCFanIRQ();
    F_TimeBaseTick_IRQ();
}

/**************************************************
*函数名称：void timer4()interrupt 14 
*函数功能：定时器中断服务函数
*入口参数：void
*出口参数：void 
**************************************************/
void INT_Timer4() interrupt 14
{
    U8 TXINX_STACK = TXINX;
    
    TXINX = 0x04;   //选择定时器T4
	TFX = 0;        //溢出清零
    TXINX = TXINX_STACK;  //还原所选的定时器

    F_Buzzer_IRQ();
}