/*********************************************************
文 件 名 : SystemTime.H
文件说明 : 系统时钟相关头文件
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/
#ifndef _SystemTime_H_
#define _SystemTime_H_ 

//时基选择
#define		def_Flag1ms			0
#define		def_Flag2ms			0
#define		def_Flag3ms			0
#define		def_Flag4ms			0
#define		def_Flag5ms			0
#define		def_Flag10ms		1

//时基缓存位地址
#define		C_BASETIME_1MS_		0
#define		C_BASETIME_2MS_		1
#define		C_BASETIME_3MS_		2
#define		C_BASETIME_4MS_		3
#define		C_BASETIME_5MS_		4
#define		C_BASETIME_10MS_	5

//时间片长度定义
#define		C_SET_50MS			5
#define		C_SET_100MS			10	
#define		C_SET_500MS			5
#define		C_SET_1S			10
#define		C_SET_3S			30
#define		C_SET_5S			50

//时间片定义
#define		C_SYS_10MS			0X02
#define		C_SYS_50MS			0X04	
#define		C_SYS_100MS			0X08
#define		C_SYS_500MS			0X10
#define		C_SYS_1S			0X20
#define		C_SYS_3S			0X40
#define		C_SYS_5S			0X80

//时间片位定义
#define		C_SYS_10MS_			1
#define		C_SYS_50MS_			2	
#define		C_SYS_100MS_		3
#define		C_SYS_500MS_		4
#define		C_SYS_1S_			5
#define		C_SYS_3S_			6
#define		C_SYS_5S_			7

//时间片判断
#define		TEST_10MS_FLAG		_BTS(g_SysTimeBuff,C_SYS_10MS_)
#define		TEST_50MS_FLAG		_BTS(g_SysTimeBuff,C_SYS_50MS_)
#define		TEST_100MS_FLAG		_BTS(g_SysTimeBuff,C_SYS_100MS_)
#define		TEST_500MS_FLAG		_BTS(g_SysTimeBuff,C_SYS_500MS_)
#define		TEST_1S_FLAG		_BTS(g_SysTimeBuff,C_SYS_1S_)
#define		TEST_3S_FLAG		_BTS(g_SysTimeBuff,C_SYS_3S_)
#define		TEST_5S_FLAG		_BTS(g_SysTimeBuff,C_SYS_5S_)

//外部变量声明
extern  	bit		g_bFlash1HzFlag;
extern  	bit		g_bFlash2HzFlag;
extern  	u8		g_u8SysTimeBuff;

//函数声明
extern void F_TIME_Event(void);
extern void F_Delay_Us(u8 u8InputDelayTime);    //微秒级粗略延时
extern void F_SYSTEM_Time(void);	            //产生系统时间标志位
#endif