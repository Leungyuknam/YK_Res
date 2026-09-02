#ifndef _KEYFUNCTION_H_
#define _KEYFUNCTION_H_

#define KEY_TK0		0X0001
#define KEY_TK1		0X0002
#define KEY_TK2		0X0004
#define KEY_TK3		0X0008
#define KEY_TK4		0X0010
#define KEY_TK5		0X0020
#define KEY_TK6		0X0040
#define KEY_TK7		0X0080
#define KEY_TK8		0X0100
#define KEY_TK9		0X0200
#define KEY_TK10	0X0400
#define KEY_TK11	0X0800
#define KEY_TK12	0X1000
#define KEY_TK13	0X2000
#define KEY_TK14	0X4000
#define KEY_TK15	0X8000
#define KEY_TK16	0X010000
#define KEY_TK17	0X020000
#define KEY_TK18	0X040000
#define KEY_TK19	0X080000
#define KEY_TK20	0X100000
#define KEY_TK21	0X200000
#define KEY_TK22	0X400000
#define KEY_TK23	0X800000
#define KEY_TK24	0X1000000
#define KEY_TK25	0X2000000


extern U8 xdata g_u8SetSpeed;          //风速
/*******************灯状态******************************/
extern U8 xdata g_u8LightState;//灯状态
#define LIGHT_OFF        0//熄灭
#define LIGHT_ON         1//点亮
#define LIGHT_HALF       2//半亮
/*******************工作模式******************************/
extern U8 xdata g_u8WorkMode;
#define MODE_LOW        1
#define MODE_MID        2
#define MODE_HIGH       3
#define MODE_SLEEP      0
#define MODE_PET        4
#define MODE_AI         4
/*******************开关类********************************/
extern bit g_bTurnOn;           //开关机标志
extern bit g_bLockEn;           //童锁使能
extern bit g_bQuickTest;        //快检标记
extern bit g_bDLZEn;            //等离子使能
#define c_ON    1
#define c_OFF   0
/*******************定时**********************************/
extern U8 xdata g_u8TimeData;           //定时控制，单位H
extern U16 xdata g_u16TimeCountDown;    //定时倒计时
#define TIME_MAX        12               //定时最长时间
/*********************************************************/
extern U8 xdata g_u8KeyData;
#define c_KEY_POWER 	    0x01
#define c_KEY_MODE 		    0x02
#define c_KEY_TIME 		    0x04
#define c_KEY_LOCK 	      0x08
#define c_KEY_ION   	    0x10
#define c_KEY_WIFI 	      0x20

#define C_KEY_TEST          0x40
#define C_KEY_QUICK_TEST    0x80
#define C_KEY_CHANGE_RGB    0x100
#define c_KEY_FILTER 	      0x200

extern void F_TimeType(S8 time);

extern void F_PowerKey();
extern void F_LockKey();
extern void F_LockLongKey();
//extern void F_MessageKey();
extern void F_FilterKey();
extern void F_FilterLongKey();
extern void F_FilterLong20sKey();
extern void F_LightKey();
extern void F_ModeKey();
extern void F_TimeKey();
extern void F_TimeLongKey();
extern void F_WifiKey();
extern void F_WifiLongKey();
extern void F_RgbDispKey();


extern void F_RgbChangeKey();
extern void F_IonKey();
	

#endif