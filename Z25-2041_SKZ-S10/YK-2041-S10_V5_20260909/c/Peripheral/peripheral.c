#include "h\Allinclude.h"

void SysClock_init(void)
{
	CLKCON = 0x00;						// Fsys = 24M   1T
}
void port_init(void)
{
	P0		= B00000000;		
    P0CR	= B00000000;				//1 output
	P0PCR	= B00000000;				//1 pull up
	P0		= B00000000;
	
    P1		= B00000000;
    P1CR	= B00000100;
	P1PCR	= B00001000;
    P1		= B00000000;
	
    P2		= B00000000;
    P2CR	= B00000000;
    P2PCR	= B11000011;
    P2		= B00000000;
	
    P3		= B00000000;
    P3CR	= B11111011;
	P3PCR	= B00000100;
    P3		= B00000000;
	
    P4		= B00000000;
    P4CR	= B00010110;
	P4PCR	= B00000000;
    P4		= B00000000;	
	
	SET_BANK_1();
    P5		= B00000000;
    P5CR	= B00000100;
	P5PCR	= B00000000;
    P5		= B00000000;	
	SET_BANK_0();	
}

void Timer3_init(void)
{    
    SET_BANK_1();
    
    T3CON = 0x00;       //定时器预分频为1/1
	TL3  = (65536 - TIME_125US) & 0xff;
    TH3  = ((65536 - TIME_125US) >> 8) & 0xff;
	T3CON |= 0x04;
    
    SET_BANK_0();
	
	IEN0 |= 0x20;

}


void Timer4_init(void)
{    
    SET_BANK_1();
    
    T4CON = 0x00;       //定时器预分频为1/1
	TL4  = (65536 - TIME_185US) & 0xff;
    TH4  = ((65536 - TIME_185US) >> 8) & 0xff;
	T4CON |= 0x02;
    
    SET_BANK_0();
	
	IEN1 |= 0x04;

}

void IP_init(void)
{
    /***********************************************************
	 *					中断优先级初始化
	 **********************************************************/

    //       7         6      5     4     3     2      1      0
    //IP0   INT        ADC    T3    S     T5    X1     TK     X0
    //IP1    -         LPD    EX2   X3    X4    T4     LED    TWI
    //
	IPL0 = B00000000;
	IPH0 = B00000000;
								// 11最高优先级 ：
								// 10次高优先级 ：
								// 01次低优先级 ：
								// 00默认优先级 ：
	
	IPL1 = B00000000;
	IPH1 = B00000000;
								// 11最高优先级 ：T2
								// 10次高优先级 ：
								// 01次低优先级 ：
								// 00默认优先级 ：
}
void WDT_clear(void)
{
    RSTSTAT = 0x00;							// 清狗 1024ms
}
/**************************************************
*函数名称：void UART0_Init(U16 rate)								  
*函数功能：UART0初始化
*入口参数：U16 rate
*出口参数：void  
**************************************************/
void Uart0_Init(uint Freq,unsigned long int baud)   
{
    SET_BANK_1();
    UART0CR = B01000101;//TX映射到P26 RX映射到P27
//	UART0CR = B01010100;//TX映射到P27 RX映射到P26
    SET_BANK_0();
	PCON &= ~0x40;
	SCON = 0x50; 	// 配置Uart工作在模式1，UART0接收器允许
	/*配置波特率参数，波特率9600*/
	/* 计算公式：(int)X=FSY/(16*波特率) ;  SBRT=32768-X  ;   SFINE=（FSY/波特率）-16*X   FSY=12M*/
	SBRTH = ( (32768-(Freq*1000000/16/baud) & 0x7f00) >> 8) | 0x80; //高位为波特率使能位
	SBRTL = ( 32768-(Freq*1000000/16/baud) )& 0x00ff;
	SFINE = (Freq*1000000/baud) - (16*(Freq*1000000/16/baud));
	IEN0 |= 0x10;
}
/**************************************************
*函数名称：void F_LPD_init()								  
*函数功能：低电压检测初始化
*入口参数：void
*出口参数：void  
**************************************************/
void F_LPD_init(void)
{
    LPD_HIGHT_SET();
}
void F_mCpuInital(void)
{
    INSCON	= 0x00;		// 分页0
    WDT_clear();     	// initialize Watch-dog timer

    SysClock_init();
    port_init();    	// initialize ports
    Timer3_init();  	// initialize Timer
//    Timer2_init();
//    F_SysUart1Init();
//    F_SysUartInit();
    F_PwmInit();
    F_RgbInit();
    IP_init();
    F_LPD_init();
}