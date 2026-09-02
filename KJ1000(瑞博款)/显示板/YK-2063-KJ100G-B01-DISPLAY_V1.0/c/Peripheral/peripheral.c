#include "h\Allinclude.h"

void port_init()
{
    OPINX = 0x86;
	OPREG &= (~0x08); //关闭READ I/O口功能操作
    
   	//TK对应的IO设置为强推挽输出1
//	P0 	= 0x00;
//	P0CON = B11111011;
//	P0PH  = B00000000;
//	P0    = B00000000;
//    
//	P1 = 0x00;
//	P1CON = B00111111;
//	P1PH  = B10000000;
//	P1    = B00110100;
//    
//	P2 = B00000000;
//	P2CON = B11111100;
//	P2PH  = B00000000;			
//	P2 = B10101100;
//	
//	P5 = B00000000;
//	P5CON = B00001101;
//	P5PH  = B00000010;			
//	P5 = B00001100;
	
	
	P0 	= B00000000;
	P0CON = B11111111;
	P0PH  = B00111000;
	P0 	= B00111111;
    
	P1 = B00000000;
	P1CON = B00001000;
	P1PH  = B11000000;
	P1 = B00000000;
    
	P2 = B00000000;
	P2CON = B00111001;
	P2PH  = B11000011;			
	P2 = B00111001;
    
	P5 = B00000000;
    P5CON = B00000011;
	P5PH  = B00000000;			
	P5 = B00000000;
	
}

/********************************************************************************************************
* @ name   : void Timer_Init(void)
* @ msg    : 定时器初始化
* @ param   (*)
* @ return  (*)
 ********************************************************************************************************/
static void Timer0_Init(void)
{
	TMCON = (TMCON & 0xfe)|(0 << 0);	//bit0: 0为FOSC/12,1为FOSO
	TMOD = TMOD & 0xf0;		  			//设置定时0，工作方式0
	TMOD = TMOD | 0x02;
	TH0 = (256 - 250);       				//1000*1=1000us	,1MS
	TL0 = (256 - 250);
	TF0 = 0;						    //清中断标志
	TR0 = 0;							//关定时器0
   	ET0 = 1;							//使能定时器0中断
	IPT0 = 1;
	TR0 = 1;
	IP |= 0x02;	
}


void WDT_clear()
{
	WDTCON = 0x10;  	//1--1 -- 00    开WDT,WDT清0,WDT 524.288ms溢出;烧录时，可Code Option选择ENWDT
}

void F_mCpuInital(void)
{
	WDT_clear();     	// initialize Watch-dog timer
	port_init();    	// initialize ports
	Timer0_Init();
	F_SYSUartInit();
	F_SysUart1Init();
}