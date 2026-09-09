#ifndef _PERIPHERAL_H_
#define _PERIPHERAL_H_

#define     SET_BANK_0()                    {INSCON &= Bin(10111111);}
#define     SET_BANK_1()                    {INSCON |= Bin(01000000);}
#define     SET_BANK0                       SET_BANK_0()
#define     SET_BANK1                       SET_BANK_1()

#define XTAL_FREQUENCY  24//æß’Ò∆µ¬  MHz
#define	TIME_125US		(125*XTAL_FREQUENCY)

#define TIME_185US      (185*XTAL_FREQUENCY)

#define cli() {EA = 0;}
#define sei() {EA = 1;}

#define Time2cli()  T2CON &= ~0x04;\	
                    IEN1 &= ~0x04;

#define Time2sei()  T2CON |= 0x04;\	
                    IEN1 |= 0x04; 

//ºÏ≤‚µÕ”⁄4.05VµÁ—π
#define LPD_LOW_SET()       LPDCON = B10000000;\
                            LPDSEL = B00001011;//ºÏ≤‚µÁ—π4.05V

//ºÏ≤‚∏ﬂ”⁄4.50VµÁ—π
#define LPD_HIGHT_SET()     LPDCON = B10001000;\
                            LPDSEL = B00001110;//ºÏ≤‚µÁ—π4.50V

extern void Timer2_Set(U16 temp);
extern void Uart0_Init(uint Freq,unsigned long int baud); 

extern void WDT_clear(void);             	// Clear Watch-dog timer
extern void F_mCpuInital(void);

#endif