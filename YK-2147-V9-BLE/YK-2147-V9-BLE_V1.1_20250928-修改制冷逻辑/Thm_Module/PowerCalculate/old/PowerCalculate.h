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
#define     Def_CalValue                1

#define     C_POWER_DIV            		798	 //12V与24V的AD值分界点
#define     C_POWER_SEL_TIME       		100  //识别电源输入电压的滤波次数	    
#define     C_POWER_LEVEL_TIME     		75   //电量滤波次数
#define     C_POWER_DELAY_CAL_TIME 		4   //前500mS不判断电池电压

/****************配置结束*****************/

//电源电压类型（输入12V还是24V）
#define     C_POWER_12V                 0    //输入12V电源
#define     C_POWER_24V            		1    //输入24V电源
#define     C_POWER_UNKOWN         		2    //电源电压未知

//电池电量等级
#define     C_POWER_LEVEL1_LOWEST       0
#define     C_POWER_LEVEL2_LOWER        1
#define     C_POWER_LEVEL3_LOW          2
#define     C_POWER_LEVEL4_HIGH         3
#define     C_POWER_LEVEL5_HIGHER       4
#define     C_POWER_LEVEL6_HIGHEST      5
	
//函数申明
extern u8 F_BAT_PowerCalculate(u16 l_u16BattaryAd);
extern u16 F_POWER_Compensation(u16 l_u16CompValue);

#if Def_CalValue
extern code u16 POWER_AD_TABEL[251];
extern code u16 INDEX2PV_TABLE[251];
#endif
			
#endif


