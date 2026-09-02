#ifndef _PERIPHERAL_H_
#define _PERIPHERAL_H_

#define cli() {EA = 0;}
#define sei() {EA = 1;}

#define XTAL_FREQUENCY  24//¾§ÕñÆµÂÊ MHz
#define	TIME_125US		(125*XTAL_FREQUENCY)

#define Time4cli()  TXINX = 0x04;\
                    TRX = 0;\	
                    IE1 &= ~0x80;

#define Time4sei()  TXINX = 0x04;\
                    TRX = 1;\	
                    IE1 |= 0x80; 

extern void Timer4_Set(U16 temp);

#define PWM0	0x01
#define PWM1	0x02
#define PWM2	0x04
#define PWM3	0x08
#define PWM4	0x10
#define PWM5	0x20
#define PWM6	0x40
#define PWM7	0x80

extern void WDT_clear();             	// Clear Watch-dog timer
extern void F_mCpuInital(void);

#endif