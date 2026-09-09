#ifndef _FUNCTION_H_
#define _FUNCTION_H_

//#define DEBUG_TIME_QUICK      //Àı ±
#ifdef DEBUG_TIME_QUICK
#pragma message "---------Àı ±≤‚ ‘---------"
    #define c_Time_1h         60
    #define c_Air_Time        30
	#define c_Air_H_Time		(45)
	#define c_Air_M_Time		(65)
	#define c_Air_L_Time		(75)
	#define c_Air_Sleep_Time	(99)
	#define c_WaterRemain_Time  (1*60)
#else
    #define c_Time_1h         3600
    #define c_Air_Time        1800
	#define c_Air_H_Time		(45*60)
	#define c_Air_M_Time		(65*60)
	#define c_Air_L_Time		(75*60)
	#define c_Air_Sleep_Time	(99*60)
	#define c_WaterRemain_Time  (72*60)
#endif


extern U8 xdata g_u8FanSpeed;


extern void F_LoadDry();
extern void F_EnterAirDry();
extern void F_mFunctionSchdule(void);
extern void F_mElectrolysisControl(void);
extern void F_mSystemInital(void);
extern void F_FanUpdate(void);
#endif 