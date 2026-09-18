/*********************************************************
文 件 名 : ADEVENT.H
文件说明 : AD事件头文件
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-6
版 本 号 : V1.0
==========================================================*/
#ifndef _ADEVENT_H_
#define _ADEVENT_H_ 

//AD通道总数
#define     C_CH_NUMBERS              3

//AD通道定义
#define     C_CH_TEMPER_LBox          6
#define     C_CH_TEMPER_RBox          9
#define     C_CH_BATTERY              4
#define     C_CH_VDD                  0x1F


//检测事件定义
#define     C_OFFTIME_EVENT   		   0
#define     C_LBOX_EVENT   			   1
#define     C_RBOX_EVENT   			   2
#define     C_BATTERY_EVENT   		   3
#define     C_POWER_EVENT   		   4


//外部声明
extern AD_Structure xdata BATTERY;
extern FILTER8_T xdata LBOX_TEMPER;
extern FILTER8_T xdata RBOX_TEMPER;
extern AD_Structure xdata TEMPER_RBox;
extern  AD_Structure xdata TEMPER_LBox;

extern  void F_ADC_Handle(void);
extern	void F_PowerVdd_Test(void); 
extern	void F_TempCompensation(s8 *l_s8SetTemp, s8 *l_ps8Temp);
              
#endif