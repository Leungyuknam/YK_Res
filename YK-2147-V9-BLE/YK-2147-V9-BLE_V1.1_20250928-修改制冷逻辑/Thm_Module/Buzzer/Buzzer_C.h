/*********************************************************
文 件 名 : Buzzer_C.H
文件说明 : 
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/
#ifndef _BUZZER_C_H_
#define _BUZZER_C_H_ 

#define		_TEST_BUZZER_OUT(x)			_BTS(x,0)	    //1:蜂鸣器打开		0：蜂鸣器关闭
#define		_TEST_BUZZER_STATUS(x)		_BTS(x,4)		//1：驱动完成		0：驱动未完成				


//结构体定义
typedef struct
{
	u8  u8BeepCnt;		 //鸣响次数
	u8  u8ContinueTime;	 //单次鸣响持续时间
	u8  u8IntervalTime;  //鸣响间隔时间
	u8	u8Beep100msCnt;	 //100ms计数器
}BEEP_T;

//变量定义
static  BEEP_T	xdata  BUZ = {0,0,0,0};

//#if (DEF_BUZZER_RUNTYPE == C_TIMER_DRIVE)
u8 xdata g_u8BuzzerTimerDrive = 0;
//#endif

//u8BeepOnOffFlag
#define			C_BUZZER_ON				0x01
#define			C_BUZZER_OFF			0x00
#define			C_BUZZER_DONE			0x10
		
			
#endif


