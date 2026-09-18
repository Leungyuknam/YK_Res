/*********************************************************
文 件 名 : PowerCalculate.H
文件说明 : 电量计算头文件
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/
#ifndef _POWERCALCULATE_H_
#define _POWERCALCULATE_H_ 

/****************配置部分*****************/

#define     C_POWER_DIV            		175	 //12V与24V的AD值分界点	    
#define     Def_Client            		1	 //1：适用于艾凯客户   0：适用于铁夫客户
#define     Def_AdcRef            		0	 //ADC参考电压： 0:2.048V ， 1:2.4V 
#define     Def_ContainDiode            0	 //1:采样点在防反二极管后   0:采样点在防反二级管前

/****************配置结束*****************/

//电源电压类型（输入12V还是24V）
#define     C_POWER_12V                 0    //输入12V电源系统
#define     C_POWER_24V            		1    //输入24V电源系统
#define     C_POWER_UNKOWN         		2    //输入电源系统未知

//电池电量等级
#define     C_POWER_LEVEL1_LOWEST       0
#define     C_POWER_LEVEL2_LOWER        1
#define     C_POWER_LEVEL3_LOW          2
#define     C_POWER_LEVEL4_HIGH         3
#define     C_POWER_LEVEL5_HIGHER       4
#define     C_POWER_LEVEL6_HIGHEST      5

//Def_Client
#define     C_CLIENT_AIKAI              1
#define     C_CLIENT_TIEFU              0

//Def_AdcRef
#define     C_ADCREF_2_048              0
#define     C_ADCREF_2_4                1

//Formula
#if (Def_AdcRef == C_ADCREF_2_4)
#define     _Formula_Pu56K_Dn3K3_PowerCalc(x)      ((unsigned long)x*1053)/10000
#else
#define     _Formula_Pu56K_Dn3K3_PowerCalc(x)      ((unsigned long)x*8985)/100000
#endif

#if Def_ContainDiode
#define     C_DIODE_COMP                3
#else
#define     C_DIODE_COMP                0
#endif


//电压数据类型定义
typedef struct
{
	unsigned int   u16Value;
	unsigned char  u8Level;		
		
}Power_Typdef;

extern Power_Typdef	xdata g_sPower;
	
//函数申明
extern Power_Typdef F_BAT_PowerCalculate(unsigned int l_u16PwValue, unsigned char l_u8FilterTimes);

extern code unsigned char LITHIUM_INCRESE_TABLE[6];
extern code unsigned char LITHIUM_DECRESE_TABLE[6];
			
#endif


