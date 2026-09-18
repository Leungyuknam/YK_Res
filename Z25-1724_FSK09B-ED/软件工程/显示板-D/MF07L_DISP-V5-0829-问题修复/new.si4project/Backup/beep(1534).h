#ifndef	_beep_H											
#define	_beep_H

#ifdef _Native_BEEP_H
   #define	EXT_BEEP 
#else
   #define	EXT_BEEP extern	
#endif
//   
//#include "..\UserH\Typedef.h"
//   
typedef struct
{
    u8 Cnt;   
    u8 OnT;   
    u8 OffT;  
    u8 GoT;   
	u8 BuzzNo;
}Bp_Str;

typedef union{
    unsigned char Byte;
    struct
    {
        unsigned char Type  :6;
        unsigned char Id    :2;
    }Set;
}Buzz_St;





#ifdef _Native_BEEP_H
	EXT_BEEP bit B_BuzzEn = 0;
	EXT_BEEP Bp_Str xdata Bper = {0};
	EXT_BEEP xdata Buzz_St BuzzSt = {0};
	EXT_BEEP xdata Buzz_St LastBuzzSt = {0};
#else
	EXT_BEEP Bp_Str xdata Bper;
	EXT_BEEP bit B_BuzzEn;
	EXT_BEEP xdata Buzz_St BuzzSt;
	EXT_BEEP xdata Buzz_St LastBuzzSt;
#endif




#define RING_BUZZING	P_BUZZ_PWM ^= 1
#define RING_BUZZ_OFF	P_BUZZ_PWM = 0
	
#ifdef _Native_BEEP_H




#endif
	
enum{
	BUZZ_NONE,
	BUZZ_NOR,
	BUZZ_OFF,
	BUZZ_ON,
	BUZZ_WARN,
	BUZZ_ERR,
	BUZZ_FINISH,
	BUZZ_KEY_WARN,
	BUZZ_FST_POWER_ON,
	BUZZ_HALF_TIME_WARN,
	BUZZ_LOCK
};

#define BELL_NOR()        			BeepSet(BUZZ_NOR)			//有效按键音
#define	BELL_OFF()					BeepSet(BUZZ_OFF)				//关机音
#define	BELL_ON()					BeepSet(BUZZ_ON)			//开机音
#define	BELL_WARN()					BeepSet(BUZZ_WARN)	
#define	BELL_ERR()					BeepSet(BUZZ_ERR)	
#define	BELL_FINISH()				BeepSet(BUZZ_FINISH)	
#define	BELL_KEY_WARN()				BeepSet(BUZZ_KEY_WARN)	
#define	BELL_FST_POWER_ON()			BeepSet(BUZZ_FST_POWER_ON)	
#define	BELL_HALF_TIME_WARN()		BeepSet(BUZZ_HALF_TIME_WARN)	
#define	BELL_LOCK()					BeepSet(BUZZ_LOCK)	


EXT_BEEP void BeepSet(u8 Type);	// 脳垄拢潞OnT + OffT < 255

//EXT_BEEP void BeepSetOFF();


#endif
