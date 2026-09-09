#ifndef _KEYFUNCTION_H_
#define _KEYFUNCTION_H_

extern bit g_bAirDryingOn;   //风干标志
extern U16 xdata g_u16AirDryingTime;    //风干时间

/*******************灯状态******************************/
extern U8 xdata g_u8LightState;//灯状态
#define LIGHT_OFF        0//熄灭
#define LIGHT_ON         1//点亮
#define LIGHT_HALF       2//半亮
/*******************工作模式******************************/
extern U8 xdata g_u8WorkMode;
#define MODE_LOW        0
#define MODE_MID        1
#define MODE_HIGH       2
#define MODE_AI         3
#define MODE_SLEEP      4




/*******************开关类********************************/
extern bit g_bTurnOn;           //开关机标志
extern bit g_bLockEn;           //童锁使能
extern bit g_bNetIonEn;         //净离子使能
extern bit g_bQuickTest;        //快检标记
extern bit g_bShrinkTimeFlg;    //缩时标记
#define c_ON    1
#define c_OFF   0
/*******************定时**********************************/
extern U8 xdata g_u8TimeData;           //定时控制，单位H
extern U16 xdata g_u16TimeCountDown;    //定时倒计时
#define TIME_MAX        12               //定时最长时间
/*********************************************************/
extern U8 xdata g_u8KeyData;
#define c_KEY_LOCK 	    0x01
#define c_KEY_JLZ 	    0x02
#define c_KEY_DRYING 	0x04
#define c_KEY_MODE 		0x08
#define c_KEY_LIGHT 	0x10
#define c_KEY_MESSAGE 	0x20
#define c_KEY_POWER 	0x40

#define C_KEY_TEST          (c_KEY_JLZ|c_KEY_LIGHT)
#define C_KEY_TIME_QUICK    (c_KEY_LOCK|c_KEY_MESSAGE)
#define c_KEY_WIFI_CANCEL   (c_KEY_MODE|c_KEY_POWER) //WIFI解绑

//#define c_KEY_WIFI_RST      (c_KEY_TIME|c_KEY_POWER) //WIFI复位

extern void F_TimeType(S8 time);

extern void F_PowerKey();
extern void F_LockKey();
extern void F_LockLongKey();
extern void F_NetIonKey();
extern void F_DryingKey();
extern void F_MessageKey();
extern void F_MessageLongKey();
extern void F_LightKey();
extern void F_ModeKey();
extern void F_TimeKey();
extern void F_WifiKey();
extern void F_WifiCancelKey();
//extern void F_WifiRstKey();
extern void F_ShrinkTimeKey();
extern void F_QuickTestKey();

#endif


#ifndef _KEYFUNCTION_H_
#define _KEYFUNCTION_H_

extern bit g_bAirDryingOn;   //风干标志
extern U16 xdata g_u16AirDryingTime;    //风干时间

/*******************灯状态******************************/
extern U8 xdata g_u8LightState;//灯状态
#define LIGHT_OFF        0//熄灭
#define LIGHT_ON         1//点亮
#define LIGHT_HALF       2//半亮
/*******************工作模式******************************/
extern U8 xdata g_u8WorkMode;
#define MODE_HIGH    0
#define MODE_AI      1
#define MODE_SLEEP   2
#define MODE_LOW     3
#define MODE_MID     4

/*******************开关类********************************/
extern bit g_bTurnOn;           //开关机标志
extern bit g_bNetIonEn;         //净离子使能
extern bit g_bQuickTest;        //快检标记
extern bit g_bShrinkTimeFlg;    //缩时标记
extern bit g_bSwtich2Ai;		//进入智能模式标记
#define c_ON    1
#define c_OFF   0
/*******************定时**********************************/
extern U8 xdata g_u8TimeData;           //定时控制，单位H
extern U16 xdata g_u16TimeCountDown;    //定时倒计时
#define TIME_MAX        12               //定时最长时间
/*********************************************************/
extern U8 xdata g_u8KeyData;
#define c_KEY_POWER 	0x01
#define c_KEY_MODE		0x02
#define c_KEY_TIME		0x04
#define c_KEY_LIGHT		0x08

#define C_KEY_TEST          (c_KEY_POWER|c_KEY_LIGHT)
#define c_KEY_WIFI_RST   (c_KEY_MODE|c_KEY_POWER) //WIFI复位

//短按
extern void F_PowerKey();
extern void F_ModeKey();
extern void F_TimeKey();
extern void F_FWLightKey();

//长按
extern void F_QuickTestKey();
extern void F_WifiKey();
extern void F_AllLightKey();
extern void F_ScreenLightKey();

#endif
