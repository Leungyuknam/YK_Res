/*********************************************************
文 件 名 : KeyEvent.H
文件说明 : 按键事件头文件
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/
#ifndef _KEY_EVENT_H_
#define _KEY_EVENT_H_ 

//按键个数
#define 	Def_SocTouchKey			    0		          //0为机械按键，1为触摸按键
#define 	KEY_NUMBERS     			4				  //按键个数


//键值
#define 	C_KEY_ONOFF			        0x01
#define 	C_KEY_SET					C_KEY_UP_L|C_KEY_DOWN_L
#define 	C_KEY_UP_R				    0x04
#define 	C_KEY_DOWN_R				0x08
#define 	C_KEY_UP_L				    0x10
#define 	C_KEY_DOWN_L				0x20

//组合键
#define 	C_KEY_CH_SET                C_KEY_SET|C_KEY_DOWN_L
#define 	C_KEY_RESET_CLOSE           C_KEY_UP_L|C_KEY_DOWN_L
#define 	C_KEY_SELFCHECK     		C_KEY_DOWN_L|C_KEY_UP_L
#define 	C_KEY_VERDISP       		C_KEY_UP_L|C_KEY_DOWN_L|C_KEY_ONOFF
#define 	C_KEY_SPEED                 C_KEY_ONOFF|C_KEY_UP_L
#define 	C_KEY_PROTECT               C_KEY_ONOFF|C_KEY_DOWN_L

//按键口配置
#if (Def_SocTouchKey == 0)

#define     _TEST_KEY_UP           		(P5 & 0x01)
#define     _TEST_KEY_DOWN         		(P5 & 0x02)
#define     _TEST_KEY_SET          		(_TEST_KEY_UP|_TEST_KEY_DOWN)
#define     _TEST_KEY_ONOFF        		(P1 & 0x02)

#else

#define 	C_KEY_ONOFF_NUMBER			2
#define 	C_KEY_SET_NUMBER			28
#define 	C_KEY_UP_R_NUMBER			29
#define 	C_KEY_DOWN_R_NUMBER		    1
#define 	C_KEY_UP_L_NUMBER			30
#define 	C_KEY_DOWN_L_NUMBER		    0

#endif

extern signed int g_s16Input;
extern signed int g_s16Output;
extern unsigned char g_u8UpKeyRepeat;
extern u8 g_u8DispKeyBuff;

//函数申明
extern void F_PowerOn(void);
extern void F_PowerOff(void);
extern void F_LevelSelect(void);
extern void F_SetBeep(void);
extern void F_ModeSetChange(void);
extern void F_DownSet(void);
extern void F_LongDownSet(void);
extern void F_DownSet_R(void);
extern void F_LongDownSet_R(void);
extern void F_UpSet(void);
extern void F_LongUpSet(void);
extern void F_UpSet_R(void);
extern void F_LongUpSet_R(void);
extern void F_InToSelfCheck(void);
extern void F_TemperTranslate_C2F(void);
extern void F_TemperTranslate_F2C(void);
extern void F_FactoryDataReset(u8 l_u8PowerStatus, u8 l_u8AllReset);		   
extern void F_GotoAdvanceSetup(void);
extern void F_CloseLbox(void);
extern void F_CloseRbox(void);
extern void F_IntoSensorChannelSet(void);
extern void F_SensorDirectionSet(void);
extern void F_SystemReset(void);
extern void F_IntoVersion(void);
extern void F_DispKey_Scan(void);
extern void F_KeyEvent_SetProtect(void);
extern void F_KeyEvent_Unlock(void);
#endif


