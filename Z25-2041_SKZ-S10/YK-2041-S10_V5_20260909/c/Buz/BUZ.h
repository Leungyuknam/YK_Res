
	
#ifndef __BUZZER_H_
#define __BUZZER_H_	

#define c_u8BuzTypeNormal	0
#define c_u8BuzTypeError	1

//#define  BUZ_ON_1TIME   (g_u8BellOnTime = 10)

#define  BUZ_ON_SHORT	15
#define  BUZ_OFF_SHORT	15

#define  BUZ_ON_LONG	60											
#define  BUZ_OFF_LONG	60


#define  BUZ_ON_ERR		20											
#define  BUZ_OFF_ERR	30

extern xdata U8 g_u8BellOnTime;
extern xdata U8 g_u8BellTimes;
extern xdata U8 g_u8BellOffTime;
extern xdata U8	g_u8SetBellOnTime;
extern xdata U8 g_u8SetBellOffTime;
extern bit g_bMuteOn;
extern void F_BellControl();
extern void F_BellIrq();
extern void F_SetBuz(U8 l_u8Times,U8 l_u8SetBellOnTime,U8 l_u8SetBellOffTime);

#define BUZ_POWERON()		F_SetBuz(1,50,10)
#define BUZ_KEY()			F_SetBuz(1,20,10)
#define BUZ_KEYINVAL()		F_SetBuz(2,20,10)
#define BUZ_TIMESET()		F_SetBuz(1,50,10)
#endif

