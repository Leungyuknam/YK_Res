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
	u8 Btype;
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



#ifdef __cplusplus
 extern "C" {
#endif
     
extern xdata U16 g_u16BeepData;

#define 	PWM_CHANNL        3
     
#define 	C_beep_power        (1<<0)////
#define 	C_beep_keypress     (1<<1)////
#define 	C_beep_error        (1<<2)////
#define 	C_beep_start        (1<<3)////
#define 	C_beep_stop         (1<<4)////
#define 	C_beep_ECO          (1<<5)////
#define 	C_beep_NOPRESS      (1<<6)////
#define 	C_beep_TIPS         (1<<7)////
#define 	C_beep_NOdo         (1<<8)////
#define 	C_beep_OFF          (1<<9)////   
     
//;┃(11)P2.5	 	<-->|BUZZ-PWM[PWM3]
//;┃(18)P0.2		<-->|BUZZ-IO[PWM2]
//#define	P_BUZ_HZ			P26
//#define	P_BUZ_P				P27

////24M==48/Fsy=500kHZ////不预分频会溢出
#define	MAIN_FREQUENCY			24000000UL			//芯片主频
#define	TIMER_PRESCALER			32					//预分频数
					
#define	P_FrequenceOn()			{	PWMCON |= BIT5;}//开始产生频率		
#define	P_FrequenceOff()		{	PWMCON &= ~BIT5;  P_BUZZ_PWM = 0;	}//停止频率发生	
#define P_BuzzerPowerOn()		{	P_BUZZ_IO = 0;	}	//开启充电管脚供电
#define P_BuzzerPowerOff()		{	P_BUZZ_IO = 1;	}	//停止充电管脚供电

#define	c_BUZZER_ALL_TYPE		10//蜂鸣器鸣叫类型
#define	c_MAX_BUZZER_TIMES		5//蜂鸣器每个类型最多鸣叫次数


//设定蜂鸣器鸣叫类型		
extern void F_SetBuzzerType(unsigned char Type);

enum MUSIC_Type{
	M_POW_ON,
	M_KEY_PRESS,
	M_ERROR,
	M_START,
	M_PAUSE,
	M_POW_OFF,
};

#define 	C_power        (0)////
#define 	C_keypress     (1)////
#define 	C_error        (2)////
#define 	C_start        (3)////
#define 	C_stop         (4)////
#define 	C_ECO          (5)////
#define 	C_NOPRESS      (6)////
#define 	C_TIPS         (7)////
#define 	C_NOdo         (8)////
#define 	C_Off          (9)////

#define     C_Voice_1       2634////HZ
#define     C_Voice_2       2958////HZ
#define     C_Voice_3       3320////HZ
#define     C_Voice_4       3520////HZ
#define     C_Voice_5       3952////HZ
#define     C_Voice_6       4436////HZ
#define     C_Voice_7       4982////HZ
#define     C_Voice_H1      5280////HZ
#define     C_Voice_End     5280////HZ


extern void F_BuzzerControl(void);
extern void PWMInit();

extern 	 uint8	r_BellOnTime;
////extern U8 r_BuzzerID;//蜂鸣器ID

extern void F_BuzErrCtrl();

extern xdata U8 r_u8BuzErrFlag;
extern xdata U8 r_u8BuzTime;
///////////////////////////////////////////////////////////////////////
#define V_TimeStandard  10
#define V_KEY_50MS  (50/V_TimeStandard)
#define V_KEY_100MS (100/V_TimeStandard)
#define V_KEY_200MS (200/V_TimeStandard)


extern void F_mVoiceControl(void);
extern void PWM_Init(void);

////16M=
////#define 	T2_125US   (500*4)=2000
///1Mm*4=0.25US

extern void F_FrequencyInit(unsigned int Freq);

#ifdef __cplusplus
}
#endif



	
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

#define BELL_NOR()        				{ F_SetBuzzerType(M_KEY_PRESS);Sys.Sign.ErrBuzzDisEn = 1;}		//有效按键音
#define	BELL_OFF()						F_SetBuzzerType(M_POW_OFF)				//关机音
#define	BELL_ON()						F_SetBuzzerType(M_POW_ON)				//开机音
#define	BELL_WARN()						F_SetBuzzerType(M_ERROR)
#define	BELL_PAUSE()					F_SetBuzzerType(M_PAUSE)	
#define	BELL_START()					F_SetBuzzerType(M_START)	




//#define	BELL_FINISH()				BuzzSet(BUZZ_FINISH)	
#define		BELL_KEY_WARN()				F_SetBuzzerType(M_PAUSE)	
//#define	BELL_FST_POWER_ON()			BuzzSet(BUZZ_FST_POWER_ON)	
//#define	BELL_HALF_TIME_WARN()		BuzzSet(BUZZ_HALF_TIME_WARN)	
//#define	BELL_LOCK()					BuzzSet(BUZZ_LOCK)	

//特别的
#define	BUZZ_PROHEAT_OK()			BeepSet(3,50,50,BUZZ_NOR)



EXT_BEEP void BuzzSet(u8 Type);
EXT_BEEP void BeepSet(u8 Cnt,u8 OnT,u8 OffT,u8 Type);
EXT_BEEP void BeepDeal(void);

EXT_BEEP void PWM_Init(void);

#endif
