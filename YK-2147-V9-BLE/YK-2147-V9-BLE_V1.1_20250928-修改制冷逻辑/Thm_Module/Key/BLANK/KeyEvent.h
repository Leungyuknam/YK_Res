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

//键值长度
#define 	LENGTH_8BIT						8
#define 	LENGTH_16BIT					16
#define 	LENGTH_32BIT					32
#define 	KEY_VALUE_LENGTH				LENGTH_32BIT


//单键值
#define 	C_KEY_POWER_ONOFF				0X10000000
#define 	C_KEY_SELECT					0X80000
#define 	C_KEY_RESET				     	0X40000

//组合键
#define C_KEY_SELFCHECK     			C_KEY_POWER_ONOFF|C_KEY_RESET

//IO口操作
#define     _TEST_KEY_UP           		Test_P13
#define     _TEST_KEY_DOWN         		Test_P11
#define     _TEST_KEY_SET          		Test_P10
#define     _TEST_KEY_ONOFF        		Test_P12


//函数申明
extern void F_PowerOnOff(void);
extern void F_CoreSelect(void);
extern void F_IntoCoreSel(void);
extern void F_CoreLifeReset(void);
extern void F_IntoSelfCheck(void);
		
#endif


