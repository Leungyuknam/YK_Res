#ifndef _MCU_79F9260_H_  
#define _MCU_79F9260_H_  

#define	WatchDog_Reset()	 RSTSTAT = 0x01	//看门狗复位操作
#define	INT_Disable()		 EA = 0	//关闭总中断
#define	INT_Enable()		 EA = 1	//开启总中断
#define	Push_Context()		 {_push_(INSCON);INSCON &= 0xBF;_push_(FLASHCON);FLASHCON=0;}//入栈，保护中断前的现场									
#define	Pop_Context()		 {INSCON &= 0xBF;_pop_(FLASHCON);_pop_(INSCON);}//出栈，恢复中断前的现场	
#define	SETBANK1()			 {INSCON |=  b0100_0000;}
#define	SETBANK0()			 {INSCON &= (~b0100_0000);}
#define	NOP()			     { _nop_();}

#define	CPU_Freq			 12	//CPU频率，以MHz为单位，要求取整数

#define SETB(r,n)			 (r |= (1<<n))
#define CLRB(r,n)			 (r &= ~(1<<n))
#define TEST(r,n)			 (r & (1<<n))
#define SWAP(r,n)			 (r ^= (1<<n))
#define Bin(a,b,c,d,e,f,g,h) ((a*0x80)+(b*0x40)+(c*0x20)+(d*0x10)+(e*0x08)+(f*0x04)+(g*0x02)+(h*0x01))

#define BIT0	(0x01)
#define BIT1	(0x02)
#define BIT2	(0x04)
#define BIT3	(0x08)
#define BIT4	(0x10)
#define BIT5	(0x20)
#define BIT6	(0x40)
#define BIT7	(0x80)
/*******************************************************************************
* @说明  	INTERRUPTE MACRO
*******************************************************************************/			
//-IEN0 sbit define |-中断允许寄存器
#define _IEN0_EA_EN_			7
#define _IEN0_EADC_EN_			6
#define _IEN0_ET3_EN_			5
#define _IEN0_ES0_EN_			4
#define _IEN0_ET5_EN_			3
#define _IEN0_ETK_EN_			1

#define INT_T3_ENALBE()			SETB(IEN0,_IEN0_ET3_EN_)
#define INT_T3_DISNALBE()		CLRB(IEN0,_IEN0_ET3_EN_)
#define INT_T5_ENALBE()			SETB(IEN0,_IEN0_ET5_EN_)
#define INT_T5_DISNALBE()		CLRB(IEN0,_IEN0_ET5_EN_)
#define INT_UART0_ENALBE()		SETB(IEN0,_IEN0_ES0_EN_)
#define INT_UART0_DISNALBE()	CLRB(IEN0,_IEN0_ES0_EN_)
#define INT_ADC_ENALBE()		SETB(IEN0,_IEN0_EADC_EN_)
#define INT_ADC_DISNALBE()		CLRB(IEN0,_IEN0_EADC_EN_)
#define INT_ETK_ENALBE()		SETB(IEN0,_IEN0_EADC_EN_)
#define INT_ETK_DISNALBE()		CLRB(IEN0,_IEN0_EADC_EN_)

#define EnableInterrupt()		SETB(IEN0,_IEN0_EA_EN_)
#define DisableInterrupt()		CLRB(IEN0,_IEN0_EA_EN_)

//-IEN1 sbit define |-中断允许寄存器
#define _IEN1_ESCM_EN_			7
#define _IEN1_ELPD_EN_			6
#define _IEN1_EX2_EN_			5
#define _IEN1_EX3_EN_			4
#define _IEN1_EX4_EN_			3
#define _IEN1_ET4_EN_			2
#define _IEN1_ELED_EN_			1
#define _IEN1_ETWI_EN_			0

#define INT_SCM_ENALBE()		SETB(IEN1,_IEN1_ESCM_EN_)
#define INT_SCM_DISNALBE()		CLRB(IEN1,_IEN1_ESCM_EN_)
#define INT_LPD_ENALBE()		SETB(IEN1,_IEN1_ELPD_EN_)
#define INT_LPD_DISNALBE()		CLRB(IEN1,_IEN1_ELPD_EN_)
#define INT_EX2_ENALBE()		SETB(IEN1,_IEN1_EX2_EN_)
#define INT_EX2_DISNALBE()		CLRB(IEN1,_IEN1_EX2_EN_)
#define INT_EX3_ENALBE()		SETB(IEN1,_IEN1_EX3_EN_)
#define INT_EX3_DISNALBE()		CLRB(IEN1,_IEN1_EX3_EN_)
#define INT_EX4_ENALBE()		SETB(IEN1,_IEN1_EX4_EN_)
#define INT_EX4_DISNALBE()		CLRB(IEN1,_IEN1_EX4_EN_)
#define INT_T4_ENALBE()			SETB(IEN1,_IEN1_ET4_EN_)
#define INT_T4_DISNALBE()		CLRB(IEN1,_IEN1_ET4_EN_)
#define INT_LED_ENALBE()		SETB(IEN1,_IEN1_ELED_EN_)
#define INT_LED_DISNALBE()		CLRB(IEN1,_IEN1_ELED_EN_)
#define INT_TWI_ENALBE()		SETB(IEN1,_IEN1_ETWI_EN_)
#define INT_TWI_DISNALBE()		CLRB(IEN1,_IEN1_ETWI_EN_)

#define _IEN2_EPWM1_EN_			6
#define _IEN2_EPCA0_EN_			5
#define _IEN2_EPWM0_EN_			3
#define _IEN2_ECRC_EN_			2
#define _IEN2_ES2_EN_			1
#define _IEN2_ES1_EN_			0

#define INT_PWM1_ENALBE()		SETB(IEN2,_IEN2_EPWM1_EN_)
#define INT_PWM1_DISNALBE()		CLRB(IEN2,_IEN2_EPWM1_EN_)
#define INT_PCA0_ENALBE()		SETB(IEN2,_IEN2_EPCA0_EN_)
#define INT_PCA0_DISNALBE()		CLRB(IEN2,_IEN2_EPCA0_EN_)
#define INT_PWM0_ENALBE()		SETB(IEN2,_IEN2_EPWM0_EN_)
#define INT_PWM0_DISNALBE()		CLRB(IEN2,_IEN2_EPWM0_EN_)
#define INT_CRC_ENALBE()		SETB(IEN2,_IEN2_ECRC_EN_)
#define INT_CRC_DISNALBE()		CLRB(IEN2,_IEN2_ECRC_EN_)
#define INT_ES2_ENALBE()		SETB(IEN2,_IEN2_ES2_EN_)
#define INT_ES2_DISNALBE()		CLRB(IEN2,_IEN2_ES2_EN_)
#define INT_ES1_ENALBE()		SETB(IEN2,_IEN2_ES1_EN_)
#define INT_ES1_DISNALBE()		CLRB(IEN2,_IEN2_ES1_EN_)

//9260  中断向量号
#define INT_VECTOR_TK			1
#define INT_VECTOR_T5			3
#define INT_VECTOR_UART0		4
#define INT_VECTOR_T3			5
#define INT_VECTOR_ADC			6
#define INT_VECTOR_TWI			7
#define INT_VECTOR_LED			8
#define INT_VECTOR_T4			9
#define INT_VECTOR_INT4			10
#define INT_VECTOR_INT3			11
#define INT_VECTOR_INT2			12
#define INT_VECTOR_LPD			13
#define INT_VECTOR_SCM			14
#define INT_VECTOR_UART1		15
#define INT_VECTOR_UART2		16
#define INT_VECTOR_CRC			17
#define INT_VECTOR_PWM0			18
#define INT_VECTOR_PAC0			20
#define INT_VECTOR_PWM1			21

//使用PCA0
//系统时钟Ft2=Fsys/4 

#define	PCA0_CLOCK_1MS			(CPU_Freq*1000)/4
#define	PCA0_CLOCK_125US		(CPU_Freq*1000)/4/8

#define _ECF0_					7
#define _CF0_					7
#define _PR0_					0
#define PCA0_T4PS_CLK_DIV_004	0x01// Bin(1,0,0,0, 0,0,0,1)

#define P0CMD_CLR()				(P0CMD = 0x00)
#define PCA0_CLKDIV_SET(val)	(P0CMD |= val)
#define PCA0_ECF0_ENABLE()		SETB(P0CMD, _ECF0_)

#define PCA0_CLR_CF0()          CLRB(P0CF, _CF0_)
#define PCA0_TIME_ENABLE()		SETB(PCACON,_PR0_)

//先高位，再写低位
#define SET_PCA0_Period(val)	{\
									P0TOPH = ((val>>8)&0xFF);\	
									P0TOPL = (val&0xFF); \	
								}
#define PWM_CLOCK   (CPU_Freq*1000/8)//12M 的MCU主频输出8kHZ控制信号周期  
#define	delay1us()	{_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();\
					 _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}

#define	Pwm1Init()	{\	
    P50_SETB();\
    delay1us();\   
    INSCON |= 0x40;\
    PWM1CON = 0x00;\ 
    PWMCR &= 0x00;\
    PWMCR |= 0x60;\
    delay1us();\
    PWM1PL = (PWM_CLOCK & 0xFF);\
    PWM1PH = (PWM_CLOCK >> 8);\
    PWM1DL = (PWM_CLOCK & 0xFF);\
	PWM1DH = (PWM_CLOCK >> 8);\
    PWM1CON |= 0xC0;\
    INSCON &= 0xBF;\
    P50_SETB();\
}// P5.0输出PWM，高电平

#define	RED_PWM_OUTPUT(SetPwmVlaue)  {INSCON |= 0x40;PWM1DL=(SetPwmVlaue & 0xFF);PWM1DH=(SetPwmVlaue >> 8);PWM1CON |= 0xC1;INSCON &= 0xBF;}
#define	RED_PWM_OFF()	{INSCON |= 0x40;delay1us(); PWM1CON = 0x00;PWM1CON |= 0xC0;INSCON &= 0xBF;P50_OUTMODE();P50_SETB();}//PWM不输出,P5.0为高电平
#define P51_DUTYCYCLE(pwm_h,pwm_l)  do{\
	INSCON |= 0x40;\
	P0CPH1 = (pwm_h);\
	P0CPL1 = (pwm_l);\
	INSCON &= 0xBF;\
	}while(0)
#define P51_OFF()	{INSCON |= 0x40;P0CPH1 = 0;P0CPL1 = 0;INSCON &= 0xBF;}

#define	GREE_PWM_OUTPUT(SetPwmVlaue) P51_DUTYCYCLE(SetPwmVlaue>>8,SetPwmVlaue & 0xFF)
#define	GREE_PWM_OFF()				 P51_OFF()

//----------------125us----------------
#define P44_DUTYCYCLE(pwm_h,pwm_l)  do{\
	INSCON |= 0x40;\
	P0CPH0 = (pwm_h);\
	P0CPL0 = (pwm_l);\
	INSCON &= 0xBF;\
	}while(0)
#define P44_OFF()	{INSCON |= 0x40;P0CPH0 = 0;P0CPL0 = 0;INSCON &= 0xBF;}
#define	BLUE_PWM_OUTPUT(SetPwmVlaue) P44_DUTYCYCLE(SetPwmVlaue>>8,SetPwmVlaue & 0xFF)
#define	BLUE_PWM_OFF()				 P44_OFF() 	
#define	INT_TIMER_125US_VECTOR		INT_VECTOR_T5 //定时中断向量号
#define	INT_Timer_125us_init()		do{\
	SETBANK1(); \
	T5CON = 0x00;\
	TL5 = ((65536 - CPU_Freq*125)&0xFF);\
	TH5 = (((65536 - CPU_Freq*125)>>8)&0xFF);\
	T5CON |= bit1;\	
	SETBANK0(); \
	INT_T5_ENALBE();\
	IPL0 |= bit3;\
	IPH0 |= bit3;						\
}while(0)

#define	INT_Timer_125us_Reset()		{SETBANK1();T5CON &= ~bit7;SETBANK0(); }													//中断定时器复位
#define	INT_Timer_125us_Disable()	INT_T5_DISNALBE()
#define	INT_Timer_125us_Enable()	INT_T5_ENALBE();

//--------------PWM LED----------------
#define P52_DUTYCYCLE(pwm_h,pwm_l)  do{\
	INSCON |= 0x40;\
	P0CPH1 = (pwm_h);\
	P0CPL1 = (pwm_l);\
	INSCON &= 0xBF;\
	}while(0)
#define P52_OFF()	{INSCON |= 0x40;P0CPH1 = 0;P0CPL1 = 0;INSCON &= 0xBF;}

#define	P_LEDPWM_DUTYCYCLE(SetPwmVlaue)	P52_DUTYCYCLE(SetPwmVlaue>>8,SetPwmVlaue & 0xFF)
#define	LEDPWM_STOP()					P52_OFF()

//-----------------------1ms---------------------
#define	INT_Timer_CLOCK_1MS		(65536- CPU_Freq*1000)

#define	INT_TIMER_1MS_VECTOR	INT_VECTOR_T3 //timer 3
#define	INT_Timer_1ms_init()	do{	\
	INSCON|= 0x40;\	
	T3CON = 0x00;\
	TL3 = (INT_Timer_CLOCK_1MS&0xFF);\
	TH3 = ((INT_Timer_CLOCK_1MS>>8)&0xFF);\
	T3CON |= bit2;\
	INSCON &= 0xBF;\
	INT_T3_ENALBE();\	
}while(0);

#define	INT_Timer_1ms_Reset()	do{\
	INSCON |= 0x40;\
	T3CON &= ~bit7;\
	TL3 = (INT_Timer_CLOCK_1MS&0xFF);\
	TH3 = ((INT_Timer_CLOCK_1MS>>8)&0xFF);\
	INSCON &= 0xBF;\	
}while(0);

#define	INT_Timer_1ms_Disable()	INT_T3_DISNALBE()
#define	INT_Timer_1ms_Enable()	INT_T3_ENALBE()
typedef enum UART_BAUDRATE
{
	UART_BAUDRATE_600 = 0,
	UART_BAUDRATE_1200 = 1,
	UART_BAUDRATE_2400 = 2,
	UART_BAUDRATE_4800 = 3,
	UART_BAUDRATE_9600 = 4,
}UART_BAUDRATE;

//#define USE_UART1
//#define UART1_BAUDRATE		UART_BAUDRATE_9600

//#define USE_UART2
//#define UART2_BAUDRATE		UART_BAUDRATE_9600

#ifdef USE_UART1
#define	INT_UART1_VECTOR	INT_VECTOR_UART1
extern void Uart1_init(UART_BAUDRATE rate);
extern uchar IsUart1_RXDInterrupt(void);
#define	Uart1_RXDInterruptClr()	{INSCON |= 0x40;SCON1 &= ~bit0;INSCON &= 0xBF;}
extern uchar IsUart1_TXDInterrupt(void);
#define	Uart1_TXDInterruptClr()	{INSCON |= 0x40;SCON1 &= ~bit1;INSCON &= 0xBF;}

#define	Uart1_RXD_Disable()	{INSCON |= 0x40;SCON1 &= ~bit4;INSCON &= 0xBF;}
#define	Uart1_RXD_Enable()	{INSCON |= 0x40;SCON1 |= bit4;INSCON &= 0xBF;}
#define	Uart1_TXD(Data)	{INSCON |= 0x40;SBUF1= (Data);INSCON &= 0xBF;}

#define GetUART1SBUF(Data)	{INSCON |= 0x40;(Data) = SBUF1;INSCON &= 0xBF;}
#define SetUART1SBUF(Data)	{INSCON |= 0x40;SBUF1 = (Data);INSCON &= 0xBF;}
#define	Uart1_InterruptEnable()	 {IEN2 |= bit0;}
#define	Uart1_InterruptDisable() {IEN2 &= ~bit0;}
#endif

#ifdef USE_UART2
#define	INT_UART2_VECTOR	INT_VECTOR_UART2
extern void Uart2_init(UART_BAUDRATE rate);
extern uchar IsUart2_RXDInterrupt(void);
#define	Uart2_RXDInterruptClr()	{INSCON |= 0x40;SCON2 &= ~bit0;INSCON &= 0xBF;}
extern uchar IsUart2_TXDInterrupt(void);
#define	Uart2_TXDInterruptClr()	{INSCON |= 0x40;SCON2 &= ~bit1;INSCON &= 0xBF;}

#define	Uart2_RXD_Disable()	{INSCON |= 0x40;SCON2 &= ~bit4;INSCON &= 0xBF;}
#define	Uart2_RXD_Enable()	{INSCON |= 0x40;SCON2 |= bit4;INSCON &= 0xBF;}
#define	Uart2_TXD(Data)		{INSCON |= 0x40;SBUF2= (Data);INSCON &= 0xBF;}

#define GetUART2SBUF(Data)	{INSCON |= 0x40;(Data) = SBUF2;INSCON &= 0xBF;}
#define SetUART2SBUF(Data)	{INSCON |= 0x40;SBUF2 = (Data);INSCON &= 0xBF;}
#define	Uart2_InterruptEnable()	 {IEN2 |= bit1;}
#define	Uart2_InterruptDisable() {IEN2 &= ~bit1;}
#endif

#define P07_PWMMODE()				{;}
#define P07_ADMODE()				{;}

//--------------------------------------------
#define P00_PWMMODE()				{;}
#define P00_ADMODE()				{;}
#define P00_INMODE()				{P0CR &= ~bit0;} 
#define P00_OUTMODE()				{P0CR |=  bit0;}
#define P00_SETB()					{P0_0=1;}//{P0 |= bit0;}
#define P00_CLRB()					{P0_0=0;}//{P0 &= ~bit0;}
#define P00_READ()					(P0&bit0)
#define P00_ENPULL() 				{P0PCR |= bit0;} 
#define P00_DISPULL()				{P0PCR &=~bit0;}
//--------------------------------------------
#define P01_PWMMODE()				{;}          
#define P01_ADMODE()				{;}          
#define P01_INMODE()				{P0CR &= ~bit1;} 
#define P01_OUTMODE()				{P0CR |=  bit1;}
#define P01_SETB()					{P0_1=1;}//{P0 |= bit1;}
#define P01_CLRB()					{P0_1=0;}//{P0 &= ~bit1;}
#define P01_READ()					(P0&bit1)
#define P01_ENPULL() 				{P0PCR |= bit1;} 
#define P01_DISPULL()				{P0PCR &=~bit1;}
//--------------------------------------------
#define P02_PWMMODE()				{;}          
#define P02_ADMODE()				{;}          
#define P02_INMODE()				{P0CR &= ~bit2;} 
#define P02_OUTMODE()				{P0CR |=  bit2;}
#define P02_SETB()					{P0_2=1;}//{P0 |= bit2;}
#define P02_CLRB()					{P0_2=0;}//{P0 &= ~bit2;}
#define P02_READ()					(P0&bit2)
#define P02_ENPULL() 				{P0PCR |= bit2;} 
#define P02_DISPULL()				{P0PCR &=~bit2;}
//--------------------------------------------
#define P03_PWMMODE()				{;}          
#define P03_ADMODE()				{;}          
#define P03_INMODE()				{P0CR &= ~bit3;} 
#define P03_OUTMODE()				{P0CR |=  bit3;}
#define P03_SETB()					{P0_3=1;}//{P0 |= bit3;}
#define P03_CLRB()					{P0_3=0;}//{P0 &= ~bit3;}
#define P03_READ()					(P0&bit3)
#define P03_ENPULL() 				{P0PCR |= bit3;} 
#define P03_DISPULL()				{P0PCR &=~bit3;}
//--------------------------------------------
#define P04_PWMMODE()				{;}          
#define P04_ADMODE()				{;}          
#define P04_INMODE()				{P0CR &= ~bit4;} 
#define P04_OUTMODE()				{P0CR |=  bit4;}
#define P04_SETB()					{P0_4=1;}//{P0 |= bit4;}
#define P04_CLRB()					{P0_4=0;}//{P0 &= ~bit4;}
#define P04_READ()					(P0&bit4)
#define P04_ENPULL() 				{P0PCR |= bit4;} 
#define P04_DISPULL()				{P0PCR &=~bit4;}
//--------------------------------------------
#define P05_PWMMODE()				{;}          
#define P05_ADMODE()				{;}          
#define P05_INMODE()				{P0CR &= ~bit5;} 
#define P05_OUTMODE()				{P0CR |=  bit5;}
#define P05_SETB()					{P0_5=1;}//{P0 |= bit5;}
#define P05_CLRB()					{P0_5=0;}//{P0 &= ~bit5;}
#define P05_READ()					(P0&bit5)
#define P05_ENPULL() 				{P0PCR |= bit5;} 
#define P05_DISPULL()				{P0PCR &=~bit5;}
//--------------------------------------------
#define P06_PWMMODE()				{;}          
#define P06_ADMODE()				{;}          
#define P06_INMODE()				{P0CR &= ~bit6;} 
#define P06_OUTMODE()				{P0CR |=  bit6;}
#define P06_SETB()					{P0_6=1;}//{P0 |= bit6;}
#define P06_CLRB()					{P0_6=0;}//{P0 &= ~bit6;}
#define P06_READ()					(P0&bit6)
#define P06_ENPULL() 				{P0PCR |= bit6;} 
#define P06_DISPULL()				{P0PCR &=~bit6;}
//--------------------------------------------
#define P07_PWMMODE()				{;}          
#define P07_ADMODE()				{;}          
#define P07_INMODE()				{P0CR &= ~bit7;} 
#define P07_OUTMODE()				{P0CR |=  bit7;}
#define P07_SETB()					{P0_7=1;}//{P0 |= bit7;}
#define P07_CLRB()					{P0_7=0;}//{P0 &= ~bit7;}
#define P07_READ()					(P0&bit7)
#define P07_ENPULL() 				{P0PCR |= bit7;} 
#define P07_DISPULL()				{P0PCR &=~bit7;}

//--------------------------------------------
#define P10_PWMMODE()				{;}
#define P10_ADMODE()				{;}
#define P10_INMODE()				{P1CR &= ~bit0;} 
#define P10_OUTMODE()				{P1CR |=  bit0;}
#define P10_SETB()					{P1_0=1;}//{P1 |= bit0;}
#define P10_CLRB()					{P1_0=0;}//{P1 &= ~bit0;}
#define P10_READ()					(P1&bit0)
#define P10_ENPULL() 				{P1PCR |= bit0;} 
#define P10_DISPULL()				{P1PCR &=~bit0;}
//--------------------------------------------
#define P11_PWMMODE()				{;}
#define P11_ADMODE()				{;}
#define P11_INMODE()				{P1CR &= ~bit1;} 
#define P11_OUTMODE()				{P1CR |=  bit1;}
#define P11_SETB()					{P1_1=1;}//{P1 |= bit1;}
#define P11_CLRB()					{P1_1=0;}//{P1 &= ~bit1;}
#define P11_READ()								(P1&bit1)
#define P11_ENPULL() 				{P1PCR |= bit1;} 
#define P11_DISPULL()				{P1PCR &=~bit1;}
//--------------------------------------------
#define P12_PWMMODE()				{;}
#define P12_ADMODE()				{;}
#define P12_INMODE()				{P1CR &= ~bit2;} 
#define P12_OUTMODE()				{P1CR |=  bit2;}
#define P12_SETB()					{P1_2=1;}//{P1 |= bit2;}
#define P12_CLRB()					{P1_2=0;}//{P1 &= ~bit2;}
#define P12_READ()					(P1&bit2)
#define P12_ENPULL() 				{P1PCR |= bit2;} 
#define P12_DISPULL()				{P1PCR &=~bit2;}
//--------------------------------------------
#define P13_PWMMODE()				{;}
#define P13_ADMODE()				{;}
#define P13_INMODE()				{P1CR &= ~bit3;} 
#define P13_OUTMODE()				{P1CR |=  bit3;}
#define P13_SETB()					{P1_3=1;}//{P1 |= bit3;}
#define P13_CLRB()					{P1_3=0;}//{P1 &= ~bit3;}
#define P13_READ()					(P1&bit3)
#define P13_ENPULL() 				{P1PCR |= bit3;} 
#define P13_DISPULL()				{P1PCR &=~bit3;}
//--------------------------------------------
#define P14_PWMMODE()				{;}
#define P14_ADMODE()				{;}
#define P14_INMODE()				{P1CR &= ~bit4;} 
#define P14_OUTMODE()				{P1CR |=  bit4;}
#define P14_SETB()					{P1_4=1;}//{P1 |= bit4;}
#define P14_CLRB()					{P1_4=0;}//{P1 &= ~bit4;}
#define P14_READ()					(P1&bit4)
#define P14_ENPULL() 				{P1PCR |= bit4;} 
#define P14_DISPULL()				{P1PCR &=~bit4;}
//--------------------------------------------
#define P15_PWMMODE()				{;}
#define P15_ADMODE()				{;}
#define P15_INMODE()				{P1CR &= ~bit5;} 
#define P15_OUTMODE()				{P1CR |=  bit5;}
#define P15_SETB()					{P1_5=1;}//{P1 |= bit5;}
#define P15_CLRB()					{P1_5=0;}//{P1 &= ~bit5;}
#define P15_READ()					(P1&bit5)
#define P15_ENPULL() 				{P1PCR |= bit5;} 
#define P15_DISPULL()				{P1PCR &=~bit5;}
//--------------------------------------------
#define P16_PWMMODE()				{;}
#define P16_ADMODE()				{;}
#define P16_INMODE()				{P1CR &= ~bit6;} 
#define P16_OUTMODE()				{P1CR |=  bit6;}
#define P16_SETB()					{P1_6=1;}//{P1 |= bit6;}
#define P16_CLRB()					{P1_6=0;}//{P1 &= ~bit6;}
#define P16_READ()					(P1&bit6)
#define P16_ENPULL() 				{P1PCR |= bit6;} 
#define P16_DISPULL()				{P1PCR &=~bit6;}
//--------------------------------------------
#define P17_PWMMODE()				{;}
#define P17_ADMODE()				{;}
#define P17_INMODE()				{P1CR &= ~bit7;} 
#define P17_OUTMODE()				{P1CR |=  bit7;}
#define P17_SETB()					{P1_7=1;}//{P1 |= bit7;}
#define P17_CLRB()					{P1_7=0;}//{P1 &= ~bit7;}
#define P17_READ()					(P1&bit7)
#define P17_ENPULL() 				{P1PCR |= bit7;} 
#define P17_DISPULL()				{P1PCR &=~bit7;}
	
//--------------------------------------------
#define P20_PWMMODE()				{;}
#define P20_ADMODE()				{;}
#define P20_INMODE()				{P2CR &= ~bit0;} 
#define P20_OUTMODE()				{P2CR |=  bit0;}
#define P20_SETB()					{P2_0=1;}//{P2 |=  bit0;}
#define P20_CLRB()					{P2_0=0;}//{P2 &= ~bit0;}
#define P20_READ()					(P2&bit0)
#define P20_ENPULL() 				{P2PCR |= bit0;} 
#define P20_DISPULL()				{P2PCR &=~bit0;}

//--------------------------------------------
#define P21_PWMMODE()				{;}
#define P21_ADMODE()				{;}
#define P21_INMODE()				{P2CR &= ~bit1;} 
#define P21_OUTMODE()				{P2CR |=  bit1;}
#define P21_SETB()					{P2_1=1;}//{P2 |=  bit1;}
#define P21_CLRB()					{P2_1=0;}//{P2 &= ~bit1;}
#define P21_READ()					(P2&bit1)
#define P21_ENPULL() 				{P2PCR |= bit1;} 
#define P21_DISPULL()				{P2PCR &=~bit1;}

//--------------------------------------------
#define P22_PWMMODE()				{;}
#define P22_ADMODE()				{;}
#define P22_INMODE()				{P2CR &= ~bit2;} 
#define P22_OUTMODE()				{P2CR |=  bit2;}
#define P22_SETB()					{P2_2=1;}//{P2 |=  bit2;}
#define P22_CLRB()					{P2_2=0;}//{P2 &= ~bit2;}
#define P22_READ()					(P2&bit2)
#define P22_ENPULL() 				{P2PCR |= bit2;} 
#define P22_DISPULL()				{P2PCR &=~bit2;}

//--------------------------------------------
#define P23_PWMMODE()				{;}
#define P23_ADMODE()				{;}
#define P23_INMODE()				{P2CR &= ~bit3;} 
#define P23_OUTMODE()				{P2CR |=  bit3;}
#define P23_SETB()					{P2_3=1;}//{P2 |=  bit0;}
#define P23_CLRB()					{P2_3=0;}//{P2 &= ~bit0;}
#define P23_READ()					(P2&bit3)
#define P23_ENPULL() 				{P2PCR |= bit3;} 
#define P23_DISPULL()				{P2PCR &=~bit3;}

//--------------------------------------------
#define P24_PWMMODE()				{;}
#define P24_ADMODE()				{;}
#define P24_INMODE()				{P2CR &= ~bit4;} 
#define P24_OUTMODE()				{P2CR |=  bit4;}
#define P24_SETB()					{P2_4=1;}//{P2 |=  bit1;}
#define P24_CLRB()					{P2_4=0;}//{P2 &= ~bit1;}
#define P24_READ()					(P2&bit4)
#define P24_ENPULL() 				{P2PCR |= bit4;} 
#define P24_DISPULL()				{P2PCR &=~bit4;}

//--------------------------------------------
#define P25_PWMMODE()				{;}
#define P25_ADMODE()				{;}
#define P25_INMODE()				{P2CR &= ~bit5;} 
#define P25_OUTMODE()				{P2CR |=  bit5;}
#define P25_SETB()					{P2_5=1;}//{P2 |=  bit2;}
#define P25_CLRB()					{P2_5=0;}//{P2 &= ~bit2;}
#define P25_READ()					(P2&bit5)
#define P25_ENPULL() 				{P2PCR |= bit5;} 
#define P25_DISPULL()				{P2PCR &=~bit5;}

//--------------------------------------------
#define P26_PWMMODE()				{;}
#define P26_ADMODE()				{;}
#define P26_INMODE()				{P2CR &= ~bit6;} 
#define P26_OUTMODE()				{P2CR |=  bit6;}
#define P26_SETB()					{P2_6=1;}//{P2 |=  bit6;}
#define P26_CLRB()					{P2_6=0;}//{P2 &= ~bit6;}
#define P26_READ()					(P2&bit6)
#define P26_ENPULL() 				{P2PCR |= bit6;} 
#define P26_DISPULL()				{P2PCR &=~bit6;}

//--------------------------------------------
#define P27_PWMMODE()				{;}
#define P27_ADMODE()				{;}
#define P27_INMODE()				{P2CR &= ~bit7;} 
#define P27_OUTMODE()				{P2CR |=  bit7;}
#define P27_SETB()					{P2_7=1;}//{P2 |=  bit7;}
#define P27_CLRB()					{P2_7=0;}//{P2 &= ~bit7;}
#define P27_READ()					(P2&bit7)
#define P27_ENPULL() 				{P2PCR |= bit7;} 
#define P27_DISPULL()				{P2PCR &=~bit7;}
 
//--------------------------------------------
#define P30_PWMMODE()				{;}
#define P30_ADMODE()				{;}
#define P30_INMODE()				{P3CR &= ~bit0;} 
#define P30_OUTMODE()				{P3CR |=  bit0;}
#define P30_SETB()					{P3_0=1;}//{P3 |=  bit0;}
#define P30_CLRB()					{P3_0=0;}//{P3 &= ~bit0;}
#define P30_READ()					(P3&bit0)
#define P30_ENPULL() 				{P3PCR |= bit0;} 
#define P30_DISPULL()				{P3PCR &=~bit0;}

//--------------------------------------------
#define P31_PWMMODE()				{;}
#define P31_ADMODE()				{;}
#define P31_INMODE()				{P3CR &= ~bit1;} 
#define P31_OUTMODE()				{P3CR |=  bit1;}
#define P31_SETB()					{P3_1=1;}//{P3 |=  bit1;}
#define P31_CLRB()					{P3_1=0;}//{P3 &= ~bit1;}
#define P31_READ()					(P3&bit1)
#define P31_ENPULL() 				{P3PCR |= bit1;} 
#define P31_DISPULL()				{P3PCR &=~bit1;}

//--------------------------------------------
#define P32_PWMMODE()				{;}
#define P32_ADMODE()				{;}
#define P32_INMODE()				{P3CR &= ~bit2;} 
#define P32_OUTMODE()				{P3CR |=  bit2;}
#define P32_SETB()					{P3_2=1;}//{P3 |=  bit2;}
#define P32_CLRB()					{P3_2=0;}//{P3 &= ~bit2;}
#define P32_READ()					(P3&bit2)
#define P32_ENPULL() 				{P3PCR |= bit2;} 
#define P32_DISPULL()				{P3PCR &=~bit2;}

//--------------------------------------------
#define P33_PWMMODE()				{;}
#define P33_ADMODE()				{;}
#define P33_INMODE()				{P3CR &= ~bit3;} 
#define P33_OUTMODE()				{P3CR |=  bit3;}
#define P33_SETB()					{P3_3=1;}//{P3 |=  bit3;}
#define P33_CLRB()					{P3_3=0;}//{P3 &= ~bit3;}
#define P33_READ()					(P3&bit3)
#define P33_ENPULL() 				{P3PCR |= bit3;} 
#define P33_DISPULL()				{P3PCR &=~bit3;}
//--------------------------------------------
#define P34_PWMMODE()				{;}
#define P34_ADMODE()				{;}
#define P34_INMODE()				{P3CR &= ~bit4;} 
#define P34_OUTMODE()				{P3CR |=  bit4;}
#define P34_SETB()					{P3_4=1;}//{P3 |=  bit4;}
#define P34_CLRB()					{P3_4=0;}//{P3 &= ~bit4;}
#define P34_READ()					(P3&bit4)
#define P34_ENPULL() 				{P3PCR |= bit4;} 
#define P34_DISPULL()				{P3PCR &=~bit4;}

//--------------------------------------------
#define P35_PWMMODE()				{;}
#define P35_ADMODE()				{;}
#define P35_INMODE()				{P3CR &= ~bit5;} 
#define P35_OUTMODE()				{P3CR |=  bit5;}
#define P35_SETB()					{P3_5=1;}//{P3 |=  bit5;}
#define P35_CLRB()					{P3_5=0;}//{P3 &= ~bit5;}
#define P35_READ()					(P3&bit5)
#define P35_ENPULL() 				{P3PCR |= bit5;} 
#define P35_DISPULL()				{P3PCR &=~bit5;}

//--------------------------------------------
#define P36_PWMMODE()				{;}
#define P36_ADMODE()				{;}
#define P36_INMODE()				{P3CR &= ~bit6;}
#define P36_OUTMODE()				{P3CR |=  bit6;}
#define P36_SETB()					{P3_6=1;}//{P3 |=  bit6;}
#define P36_CLRB()					{P3_6=0;}//{P3 &= ~bit6;}
#define P36_READ()					(P3&bit6)
#define P36_ENPULL() 				{P3PCR |= bit6;} 
#define P36_DISPULL()				{P3PCR &=~bit6;}

//--------------------------------------------
#define P37_PWMMODE()				{;}
#define P37_ADMODE()				{;}
#define P37_INMODE()				{P3CR &= ~bit7;} 
#define P37_OUTMODE()				{P3CR |=  bit7;}
#define P37_SETB()					{P3_7=1;}//{P3 |=  bit7;}
#define P37_CLRB()					{P3_7=0;}//{P3 &= ~bit7;}
#define P37_READ()					(P3&bit7)
#define P37_ENPULL() 				{P3PCR |= bit7;} 
#define P37_DISPULL()				{P3PCR &=~bit7;}

//--------------------------------------------
#define P40_PWMMODE()				{;}
#define P40_ADMODE()				{;}
#define P40_INMODE()				{P4CR &= ~bit0;} 
#define P40_OUTMODE()				{P4CR |=  bit0;}
#define P40_SETB()					{P4_0=1;}//{P4 |=  bit0;}
#define P40_CLRB()					{P4_0=0;}//{P4 &= ~bit0;}
#define P40_READ()					(P4&bit0)
#define P40_ENPULL() 				{P4PCR |= bit0;} 
#define P40_DISPULL()				{P4PCR &=~bit0;}

//--------------------------------------------
#define P41_PWMMODE()				{;}
#define P41_ADMODE()				{;}
#define P41_INMODE()				{P4CR &= ~bit1;} 
#define P41_OUTMODE()				{P4CR |=  bit1;}
#define P41_SETB()					{P4_1=1;}//{P4 |=  bit1;}
#define P41_CLRB()					{P4_1=0;}//{P4 &= ~bit1;}
#define P41_READ()					(P4&bit1)
#define P41_ENPULL() 				{P4PCR |= bit1;} 
#define P41_DISPULL()				{P4PCR &=~bit1;}

//-------------------------------------------
#define P42_PWMMODE()				{;}
#define P42_ADMODE()				{;}
#define P42_INMODE()				{P4CR &= ~bit2;} 
#define P42_OUTMODE()				{P4CR |=  bit2;}
#define P42_SETB()					{P4_2=1;}//{P4 |=  bit2;}
#define P42_CLRB()					{P4_2=0;}//{P4 &= ~bit2;}
#define P42_READ()					(P4&bit2)
#define P42_ENPULL() 				{P4PCR |= bit2;} 
#define P42_DISPULL()				{P4PCR &=~bit2;}

//-------------------------------------------
#define P43_PWMMODE()				{;}
#define P43_ADMODE()				{;}
#define P43_INMODE()				{P4CR &= ~bit3;} 
#define P43_OUTMODE()				{P4CR |=  bit3;}
#define P43_SETB()					{P4_3=1;}//{P4 |=  bit3;}
#define P43_CLRB()					{P4_3=0;}//{P4 &= ~bit3;}
#define P43_READ()					(P4&bit3)
#define P43_ENPULL() 				{P4PCR |= bit3;} 
#define P43_DISPULL()				{P4PCR &=~bit3;}

//-------------------------------------------
#define P44_PWMMODE()				{;}
#define P44_ADMODE()				{;}
#define P44_INMODE()				{P4CR &= ~bit4;} 
#define P44_OUTMODE()				{P4CR |=  bit4;}
#define P44_SETB()					{P4_4=1;}//{P4 |=  bit4;}
#define P44_CLRB()					{P4_4=0;}//{P4 &= ~bit4;}
#define P44_READ()					(P4&bit4)
#define P44_ENPULL() 				{P4PCR |= bit4;} 
#define P44_DISPULL()				{P4PCR &=~bit4;}
//-------------------------------------------
#define P45_PWMMODE()				{;}
#define P45_ADMODE()				{;}
#define P45_INMODE()				{P4CR &= ~bit5;} 
#define P45_OUTMODE()				{P4CR |=  bit5;}
#define P45_SETB()					{P4_5=1;}//{P4 |=  bit4;}
#define P45_CLRB()					{P4_5=0;}//{P4 &= ~bit4;}
#define P45_READ()					(P4&bit5)
#define P45_ENPULL() 				{P4PCR |= bit5;} 
#define P45_DISPULL()				{P4PCR &=~bit5;}
//-------------------------------------------
#define P46_PWMMODE()				{;}
#define P46_ADMODE()				{;}
#define P46_INMODE()				{P4CR &= ~bit6;} 
#define P46_OUTMODE()				{P4CR |=  bit6;}
#define P46_SETB()					{P4_6=1;}//{P4 |=  bit4;}
#define P46_CLRB()					{P4_6=0;}//{P4 &= ~bit4;}
#define P46_READ()					(P4&bit6)
#define P46_ENPULL() 				{P4PCR |= bit6;} 
#define P46_DISPULL()				{P4PCR &=~bit6;}
//-------------------------------------------
#define P47_PWMMODE()				{;}
#define P47_ADMODE()				{;}
#define P47_INMODE()				{P4CR &= ~bit7;} 
#define P47_OUTMODE()				{P4CR |=  bit7;}
#define P47_SETB()					{P4_7=1;}//{P4 |=  bit4;}
#define P47_CLRB()					{P4_7=0;}//{P4 &= ~bit4;}
#define P47_READ()					(P4&bit7)
#define P47_ENPULL() 				{P4PCR |= bit7;} 
#define P47_DISPULL()				{P4PCR &=~bit7;}

//---------------------------------------------
#define P50_PWMMODE()				{;}
#define P50_ADMODE()				{;}
#define P50_INMODE()				{INSCON |= 0x40;P5CR &= ~bit0;INSCON &= 0xBF;} 
#define P50_OUTMODE()				{INSCON |= 0x40;P5CR |= bit0;INSCON &= 0xBF;}
#define P50_SETB()					{INSCON |= 0x40;P5_0=1;INSCON &= 0xBF;}	
#define P50_CLRB()					{INSCON |= 0x40;P5_0=0;INSCON &= 0xBF;}
extern  uchar P50_READ(void);	
#define P50_ENPULL() 				{INSCON |= 0x40;P5PCR |=  bit0;INSCON &= 0xBF;}
#define P50_DISPULL()				{INSCON |= 0x40;P5PCR &= ~bit0;INSCON &= 0xBF;}

//--------------------------------------------
#define P51_PWMMODE()				{;}
#define P51_ADMODE()				{;}
#define P51_INMODE()				{INSCON |= 0x40;P5CR &=~bit1;INSCON &= 0xBF;} 
#define P51_OUTMODE()				{INSCON |= 0x40;P5CR |= bit1;INSCON &= 0xBF;}
#define P51_SETB()					{INSCON |= 0x40;P5_1=1;INSCON &= 0xBF;}	
#define P51_CLRB()					{INSCON |= 0x40;P5_1=0;INSCON &= 0xBF;}	
extern  uchar P51_READ(void);	
#define P51_ENPULL() 				{INSCON |= 0x40;P5PCR |=  bit1;INSCON &= 0xBF;}
#define P51_DISPULL()				{INSCON |= 0x40;P5PCR &= ~bit1;INSCON &= 0xBF;}

//--------------------------------------------
#define P52_PWMMODE()				{;}
#define P52_ADMODE()				{;}
#define P52_INMODE()				{INSCON |= 0x40;P5CR &=~bit2;INSCON &= 0xBF;} 
#define P52_OUTMODE()				{INSCON |= 0x40;P5CR |= bit2;INSCON &= 0xBF;}
#define P52_SETB()					{INSCON |= 0x40;P5_2=1;INSCON &= 0xBF;}
#define P52_CLRB()					{INSCON |= 0x40;P5_2=0;INSCON &= 0xBF;}	
extern  uchar P52_READ(void);
#define P52_ENPULL() 				{INSCON |= 0x40;P5PCR |=  bit2;INSCON &= 0xBF;}
#define P52_DISPULL()				{INSCON |= 0x40;P5PCR &= ~bit2;INSCON &= 0xBF;}
//--------------------------------------------
#define P53_PWMMODE()				{;}
#define P53_ADMODE()				{;}
#define P53_INMODE()				{INSCON |= 0x40;P5CR &=~bit3;INSCON &= 0xBF;} 
#define P53_OUTMODE()				{INSCON |= 0x40;P5CR |= bit3;INSCON &= 0xBF;}
#define P53_SETB()					{INSCON |= 0x40;P5_3=1;INSCON &= 0xBF;}
#define P53_CLRB()					{INSCON |= 0x40;P5_3=0;INSCON &= 0xBF;}
extern  uchar P53_READ(void);
#define P53_ENPULL() 				{INSCON |= 0x40;P5PCR |=  bit3;INSCON &= 0xBF;}
#define P53_DISPULL()				{INSCON |= 0x40;P5PCR &= ~bit3;INSCON &= 0xBF;}
//----------------ADT---------------------

/*******************************************************************************
  * @说明  	ADC MACRO
*******************************************************************************/			
//-ADCON2 sbit define 
#define _ADCON2_VBG_		7  
#define	_ADCON2_GRP_1_		Bin(0,0,0,0, 0,0,0,0) //待转换序列的总数为1 通道寄存器为SEQCH0
#define	_ADCON2_GRP_2_		Bin(0,0,0,1, 0,0,0,0)
#define	_ADCON2_GRP_3_		Bin(0,0,1,0, 0,0,0,0)
#define	_ADCON2_GRP_4_		Bin(0,0,1,1, 0,0,0,0)
#define	_ADCON2_GRP_5_		Bin(0,1,0,0, 0,0,0,0)
#define	_ADCON2_GRP_6_		Bin(0,1,0,1, 0,0,0,0)
#define	_ADCON2_GRP_7_		Bin(0,1,1,0, 0,0,0,0)
#define	_ADCON2_GRP_8_		Bin(0,1,1,1, 0,0,0,0) //待转换序列的总数为8 通道寄存器为SEQCH0~SEQCH8

#define ADCON2_CLR()		(ADCON2 = 0)
#define ADCON2_SET()		(ADCON2 = _ADCON2_GRP_2_)

//-ADT sbit define  _000X_XXXX_ 
#define _ADT_Tad_1Tsys_		Bin(0,0,0,0, 0,0,0,0)
#define _ADT_Tad_2Tsys_		Bin(0,0,0,1, 0,0,0,0)
#define _ADT_Tad_3Tsys_		Bin(0,0,1,0, 0,0,0,0)
#define _ADT_Tad_4Tsys_		Bin(0,0,1,1, 0,0,0,0)
#define _ADT_Tad_5Tsys_		Bin(0,1,0,0, 0,0,0,0)
#define _ADT_Tad_6Tsys_		Bin(0,1,0,1, 0,0,0,0)
#define _ADT_Tad_8Tsys_		Bin(0,1,1,0, 0,0,0,0)
#define _ADT_Tad_12Tsys_	Bin(0,1,1,1, 0,0,0,0)
#define _ADT_Tad_16Tsys_	Bin(1,0,0,0, 0,0,0,0)	
#define _ADT_Tad_24Tsys_	Bin(1,0,0,1, 0,0,0,0)//-tAD=x*Tsys >= 1us,=> x>12.
#define _ADT_Tad_32Tsys_	Bin(1,0,1,0, 0,0,0,0)
#define _ADT_Tad_48Tsys_	Bin(1,0,1,1, 0,0,0,0)
#define _ADT_Tad_64Tsys_	Bin(1,1,0,0, 0,0,0,0)
#define _ADT_Tad_96Tsys_	Bin(1,1,0,1, 0,0,0,0)
#define _ADT_TS_			Bin(0,0,0,0, 0,1,1,1)//-2tAD<= 采样时间=(TS[3:0]+1)*tAD<=15tAD 
#define _ADT_DEFAULT_		Bin(0,0,0,0, 0,0,0,0)

#define	_SEQCON_ALR_		7
#define	_SEQCON_REQ_0_		Bin(0,0,0,0, 0,0,0,0)//-SEQCH0和ADD0L ADD0H
#define	_SEQCON_REQ_1_		Bin(0,0,0,0, 0,0,0,1)//-SEQCH1和ADD1L ADD1H
#define	_SEQCON_REQ_2_		Bin(0,0,0,0, 0,0,1,0)//-SEQCH2和ADD2L ADD2H
#define	_SEQCON_REQ_3_		Bin(0,0,0,0, 0,0,1,1)//-SEQCH3和ADD3L ADD3H
#define	_SEQCON_REQ_4_		Bin(0,0,0,0, 0,1,0,0)//-SEQCH4和ADD4L ADD4H
#define	_SEQCON_REQ_5_		Bin(0,0,0,0, 0,1,0,1)//-SEQCH5和ADD5L ADD5H
#define	_SEQCON_REQ_6_		Bin(0,0,0,0, 0,1,1,0)//-SEQCH6和ADD6L ADD6H
#define	_SEQCON_REQ_7_		Bin(0,0,0,0, 0,1,1,1)//-SEQCH7和ADD7L ADD7H
#define	SEQCON_SET_ALR()	CLRB(SEQCON, _SEQCON_ALR_) //-左对齐

#define ADCH1_CH0			0
#define ADCH1_CH1			1
#define ADCH1_CH2			2
#define ADCH1_CH3			3
#define ADCH1_CH4			4
#define ADCH1_CH5			5
#define ADCH1_CH6			6
#define ADCH1_CH7			7

#define ADCH2_CH8			0

#define _ADCON1_ADON_		7
#define _ADCON1_ADCIF_		6
#define _ADCON1_REFC_		5//-基准电压选择为 0:选择内部VDD为基准电压.
#define _ADCON1_XTRGEN_		4//-外中断2启动序列转换	
#define _ADCON1_PCATRGEN_	3//-PCA0中断启动序列转换
#define _ADCON1_PWMTRGEN_	2//-PWM1溢出启动序列转换
#define _ADCON1_TIMTRGEN_	1//-TIME4溢出启动序列转换
#define _ADCON1_GO_START_	0
#define	ADCON1_CLR()		(ADCON1 = 0)

//-macro for configuration 
#define ADT_Config()		(ADT = (_ADT_Tad_24Tsys_|_ADT_TS_))
#define ADC_Convert_Start()	SETB(ADCON1,_ADCON1_GO_START_)
#define ADC_Convert_Stop()	CLRB(ADCON1,_ADCON1_GO_START_)
#define ADC_Module_Enable()	SETB(ADCON1,_ADCON1_ADON_)
#define ADC_Module_Disable()CLRB(ADCON1,_ADCON1_ADON_)
#define ADC_ADCIF_Clear()	CLRB(ADCON1,_ADCON1_ADCIF_)
#define Is_ADC_Finished()	TEST(ADCON1,_ADCON1_ADCIF_)
#define ADC_CLR_IF()		CLRB(ADCON1,_ADCON1_ADCIF_)

//10 位AD
#define Read_ADC_Value(val)	{\
			val	= ADDXH;;\
			val <<= 2;\
			val += (ADDXL>>6);\
}

#define ADC_Initial()		{\
								ADCON1_CLR();\
								ADCON2_CLR();\
								ADT_Config();\
								INT_ADC_DISNALBE();\
}
/*******************************************************************************
* @说明  pADC_Channel_Confing
* @参数	channle -> ADC channel: 0~15
* @返回	none
* @属性	private  rTab_SCH_Channel
*******************************************************************************/
//AN8
#define pADC_Channel_Confing(ch)	{\
										if(ch<8){ADCH1 |=(1<<ch);}else{ADCH2|=(1<<(ch-8));}	\
									}
/*******************************************************************************
* @说明  pADC_Channel_Select
* @参数	channle -> ADC channel: 0~15
* @返回	none
* @属性	private
*******************************************************************************/
#define pADC_Channel_Select(ch)	{\
									SEQCHX =ch;\
								}
/*******************************************************************************
* @说明  	PWM0 MACRO
*******************************************************************************/			
//-PWM0-系统时钟Ft3=Fsys/8,Tt3 =8/12us--->8us=12*Tt3.
//-PWMEN

//-可配置模块
#define PWM1_USE_P0_2		Bin(0,0,0,0, 0,0,0,0)
#define PWM1_USE_P2_6		Bin(0,1,0,0, 0,0,0,0)
#define PWM1_USE_P2_7		Bin(0,1,0,1, 0,0,0,0)
#define PWM1_USE_P5_0		Bin(0,1,1,0, 0,0,0,0)

#define PWM0_USE_P3_0		Bin(0,0,0,0, 0,0,0,0)
#define PWM0_USE_P3_1		Bin(0,1,0,0, 0,0,0,1)
#define PWM0_USE_P3_2		Bin(0,1,0,1, 0,0,1,0)
#define PWM0_USE_P3_3		Bin(0,1,1,0, 0,0,1,1)
#define PWM0_USE_P3_4		Bin(0,0,0,0, 0,1,0,0)
#define PWM0_USE_P3_5		Bin(0,1,0,0, 0,1,0,1)
#define PWM0_USE_P3_6		Bin(0,1,0,1, 0,1,1,0)
#define PWM0_USE_P3_7		Bin(0,1,1,0, 0,1,1,1)

#define PWM_SET_IO(val)		{ \
								SETBANK1();	\
								PWMCR |= val;\
								SETBANK0();	\
							}

#define	_PWM0EN_			7
#define	_PWM0S_				6
#define	_PWM0CK2_			5
#define	_PWM0CK1_			4
#define	_PWM0CK0_			3
#define	_PWM0IE_			2
#define	_PWM0IF_			1
#define	_PWM0SS_			0

#define	PWM0CON_CLR()		(PWM0CON = 0)

#define PWM0_ENABLE()		SETB(PWM0CON, _PWM0EN_)
#define PWM0_DISABLE()		CLRB(PWM0CON, _PWM0EN_)
#define PWM0_OUTPUT_LOW()	SETB(PWM0CON, _PWM0S_)

#define PWM0_CLK_DIV_1		Bin(0,0,0,0, 0,0,0,0)
#define PWM0_CLK_DIV_2		Bin(0,0,0,0, 1,0,0,0)
#define PWM0_CLK_DIV_4		Bin(0,0,0,1, 0,0,0,0)
#define PWM0_CLK_DIV_8		Bin(0,0,0,1, 1,0,0,0)
#define PWM0_CLK_DIV_16		Bin(0,0,1,0, 0,0,0,0)
#define PWM0_CLK_DIV_32		Bin(0,0,1,0, 1,0,0,0)
#define PWM0_CLK_DIV_64		Bin(0,0,1,1, 0,0,0,0)
#define PWM0_CLK_DIV_128	Bin(0,0,1,1, 1,0,0,0)
#define PWM0_CLKDIV_SET(val) (PWM0CON |= val)

#define PWM0_INTERRUPT_ENABLE()	SETB(PWM0CON, _PWM0IE_)
#define PWM0_INTERRUPT_DISABLE()CLRB(PWM0CON, _PWM0IE_)

#define PWM0_OUTPUT_ENABLE()	SETB(PWM0CON, _PWM0SS_)
#define PWM0_OUTPUT_DISABLE()	CLRB(PWM0CON, _PWM0SS_)

#define PORT_BUZ_PWM0_OFF()		(P3_4 = 1)
//-PWMPL/PWMPH
//-系统时钟Fpwm0=Fsys/8,Tpwm0 =8/12us--->8us=12*Tt3.
//-BUZ:F=4K-->T=250us=375*8/12us
#define BUZ_F4K_T250US			375
#define BUZ_DUTY_HALF			187
#define PWM0_PERIOD_SET(val)	{\
									PWM0PL =( val & 0xFF);\
									PWM0PH =((val>>8)& 0xFF);\
								} 
#define PWM0_DUTY_SET(val)		{\
									PWM0DL = ( val & 0xFF);\
									PWM0DH = ((val>>8)& 0xFF);\
								}
//-
#define PWM0_Init()				{\
									P3_3 = 1;\
									PWM0CON_CLR();\
									PWM_SET_IO(PWM0_USE_P3_3);\
									PWM0_CLKDIV_SET(PWM0_CLK_DIV_8);\
									PWM0_PERIOD_SET(BUZ_F4K_T250US);\
									PWM0_DUTY_SET(BUZ_DUTY_HALF);\
									PWM0_ENABLE();\
									PWM0_OUTPUT_LOW();\
								}
//参考
#define PWM0_BUZZER_ON()		{\
									PWM0_OUTPUT_ENABLE();\
								}
//-
#define PWM0_BUZZER_OFF()		{\
									PWM0_OUTPUT_DISABLE();\
									PORT_BUZ_PWM0_OFF();\
								}

void S_DR_InitMCU(void);
uint P40_AD_READ(void);	//AN0
uint P41_AD_READ(void);	//AN1
uint P42_AD_READ(void);	//AN2
uint P43_AD_READ(void);	//AN3
uint P44_AD_READ(void); //AN 8
uint P34_AD_READ(void);	//AN4
uint P35_AD_READ(void);	//AN7
uint P36_AD_READ(void);	//AN5
uint P37_AD_READ(void);	
#endif
/************************************************************************
							END
*************************************************************************/
