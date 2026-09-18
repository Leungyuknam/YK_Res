/*********************************************************
文 件 名 : Buzzer.H
文件说明 : 
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/
#ifndef _BUZZER_H_
#define _BUZZER_H_ 

/*************************需配置部分***********************/
//IO口定义（仅定时器驱动需配置）
#define     _IO_BUZZER_OFF		        P16 = 0
#define     _SET_IO_BUZZER_PP		    Set_P16_pp;
#define     _IO_BUZZER_ON		        P16 = 1
#define     _SET_IO_BUZZER_IN		    Set_P16_in;

//蜂鸣器运行模式（0：硬件PWM驱动  1：定时器中断驱动）
#define		C_PWM_DRIVE        			0
#define		C_TIMER_DRIVE     			1
#define		DEF_BUZZER_RUNTYPE			C_TIMER_DRIVE

/*************************配置结束*************************/

extern u8 xdata g_u8BuzzerTimerDrive;

//函数声明
extern void F_BuzzerInput(u8 l_u8BeepCnt,u8 l_u8ContinueTime, u8 l_u8IntervalTime);
extern void F_Buzzer_Handle(void);

#if (DEF_BUZZER_RUNTYPE == C_TIMER_DRIVE)
extern void F_DRIVE_Buzzer(void);
#endif

			
#endif


