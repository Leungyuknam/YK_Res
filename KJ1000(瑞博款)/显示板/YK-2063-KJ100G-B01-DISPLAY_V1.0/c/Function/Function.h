#ifndef _FUNCTION_H_
#define _FUNCTION_H_

//#define DEBUG_TIME_QUICK      //缩时
#ifdef DEBUG_TIME_QUICK
#pragma message "---------缩时测试---------"
    #define c_Time_1h         60
    #define c_Air_Time        30
#else
    #define c_Time_1h         3600
    #define c_Air_Time        1800
#endif

extern U16 xdata g_u16SenseEnTime;     //传感器使能时间  单位100ms

extern U8 xdata g_u8AirQuality;

extern U8 xdata g_u8FanSpeed;
#define FAN_OFF     0
#define FAN_SLEEP		1
#define FAN_LOW     2
#define FAN_MID     3
#define FAN_HI      4

extern U8 xdata g_u8AiFanSpeed;        //AI风速
extern U8 xdata g_u8LocalTimeIsNight;
extern StrByte LOAD;
#define g_u8LoadState	LOAD.Byte
#define g_bSenseLoad	LOAD.BBit.b0
#define g_bIonLoad	    LOAD.BBit.b1
#define g_bUvLoad	    LOAD.BBit.b2

extern void F_LoadDry();

extern void F_mFunctionSchdule(void);
extern void F_mSystemInital(void);

#endif 