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
	
	
	P0 	= 0x00;
	P0CON = 0xF3;
	P0PH  = 0x0C;
	P0 	= 0x00;
    
	P1 = 0x00;
	P1CON = 0x4F;
	P1PH  = 0x20;
	P1 = 0x00;
    
	P2 = 0x00;
	P2CON = 0xB4;
	P2PH  = 0x00;			
	P2 = 0x00;
    
	P5 = 0x00;
    P5CON = 0xFF;
	P5PH  = 0x00;			
	P5 = 0x00;
	
	SC95R503M28_NIO_Init();
	
}

/**************************************************
*函数名称：void Timer3_init(void) 
*函数功能：定时器初始化
*入口参数：void
*出口参数：void
**************************************************/
void Timer3_init()
{
    TXINX = 0x03;//指向定时器3
    TXMOD = 0x80;//T3频率为Fsys    
	TXCON = 0x00;//设置为16位重载寄存器

	RCAPXH = (65536-TIME_125US)/256;    //125us 125/(1/Fsys) 
	RCAPXL = (65536-TIME_125US)%256;
    
	TRX = 0;
	IE1 |= 0x40; //定时器3允许
    IP1 |= 0x40;
	TRX = 1;     //打开定时器3	
}
/**************************************************
*函数名称：void Timer4_init(void) 
*函数功能：定时器初始化
*入口参数：void
*出口参数：void
**************************************************/
void Timer4_init()
{
    TXINX = 0x04;//指向定时器4
    TXMOD = 0x80;//T4频率为Fsys    
	TXCON = 0x00;//设置为16位重载寄存器

	RCAPXH = (65536-8000)/256;
	RCAPXL = (65536-8000)%256;
    
	TRX = 0;
//	IE1 |= 0x80; //定时器4允许
    IP1 |= 0x80;
//	TRX = 1;     //打开定时器4	
}
void Timer4_Set(U16 temp)
{    
    Time4cli();	
    
	RCAPXH = (65536-temp)/256;    
	RCAPXL = (65536-temp)%256;
    
    Time4sei();   
}

void WDT_clear()
{
	WDTCON = 0x10;  	//1--1 -- 00    开WDT,WDT清0,WDT 524.288ms溢出;烧录时，可Code Option选择ENWDT
}

void F_mCpuInital(void)
{
	WDT_clear();     	// initialize Watch-dog timer
	port_init();    	// initialize ports
	Timer3_init();  	// initialize Timer
	Timer4_init();  	// initialize Timer
    F_PwmInit();
	F_SysUart1Init();
    F_DustUartInit();
    F_WifiSysUartInit();
}