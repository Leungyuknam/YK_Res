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

extern U8 xdata g_u8PM25UpLimit;       //智能模式，PM25上限值
extern U8 xdata g_u8PM25DownLimit;     //智能模式，PM25下限值

extern U16 xdata g_u16SenseEnTime;     //传感器使能时间  单位100ms

extern U16 xdata g_u16Time_1h;//1h时间

//g_u16AiModeRunTime
#define c_AI_MODE_RUN_TIME      (2*g_u16Time_1h)//智慧模式停风机时间

//g_u16NetIonRunTime
#define c_NETION_RUN_TIME       (11*g_u16Time_1h)
#define c_NETION_STOP_TIME      (1*g_u16Time_1h)

/*g_u8FanSpeed*/ 
//1540 1450 1160 800 475
#define FAN_OFF     0
#define FAN_SLEEP   1
#define FAN_L       2
#define FAN_M       3
#define FAN_H       4
#define FAN_MAX     5

#define EXPD_FAN_SPEED_MIN       200//扩展AI模式风速偏移可达到的最小风速
#define EXPD_FAN_SPEED_MAX      1540//扩展AI模式风速偏移可达到的最大风速

extern StrByte LOAD;
#define g_u8LoadState	LOAD.dchar
#define g_bNetIonLoad	LOAD.BBit.b0
#define g_bSenseLoad	LOAD.BBit.b1
#define g_bIonLoad	    LOAD.BBit.b2
#define g_bUvLoad	    LOAD.BBit.b3

extern U8 xdata g_u8FanSpeed;

extern void F_LoadDry();

extern void F_mFunctionSchdule(void);
extern void F_mSystemInital(void);

#endif 



#ifndef _FUNCTION_H_
#define _FUNCTION_H_

//#define DEBUG_TIME_QUICK      //缩时
#ifdef DEBUG_TIME_QUICK
#pragma message "---------缩时测试---------"
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


#define FAN_OFF     0
#define FAN_SLEEP   1
#define FAN_L       2
#define FAN_M       3
#define FAN_H       4


extern StrByte LOAD;
#define g_u8LoadState	LOAD.dchar
#define g_bNetIonLoad	LOAD.BBit.b0
#define g_bIonLoad	    LOAD.BBit.b1
#define g_bUvLoad	    LOAD.BBit.b2


extern U8 xdata g_u8FanSpeed;


typedef struct
{
	StrByte LOAD_FLG;	//负载开关
	StrByte EN_FLG;		//使能开关
	
	bit g_bTurnOn = 0;     //开关机标志
	bit g_bLockEn = 0;     //童锁使能
	bit g_bNetIonEn = 1;   //净离子使能
	bit g_bQuickTest = 0;  //快检标记
	bit g_bShrinkTimeFlg = 0; //缩时标记
	bit g_bSwtich2Ai = 0;
}Sys;

#define g_u8LoadState	LOAD.dchar
#define g_bNetIonLoad	LOAD.BBit.b0
#define g_bIonLoad	    LOAD.BBit.b1
#define g_bUvLoad	    LOAD.BBit.b2

#endif 