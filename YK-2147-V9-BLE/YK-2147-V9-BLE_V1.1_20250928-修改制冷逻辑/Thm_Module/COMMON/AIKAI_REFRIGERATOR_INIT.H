/*********************************************************
文 件 名 : Refrigerator_Init.H
文件说明 : 冰箱初始化头文件
编写作者 : thm
修订作者 : 
编写时间 : 2021-2-26
版 本 号 : V1.0
==========================================================*/
#ifndef _AIKAI_REFRIGERATOR_INIT_H_
#define _AIKAI_REFRIGERATOR_INIT_H_ 


#define     C_INIT_SystemStatus					C_SYSTEM_TURNON
#define     C_INIT_SpeedSetValue				C_SPEED_MAX
#define     C_INIT_BatteryProSetValue			C_CELL_HIGH
#define     C_INIT_SetHighestTemper				20
#define     C_INIT_SetLowestTemper         		(-20)
#define     C_INIT_BiasTemper     		  		2
#define     C_INIT_DelayOnTime     		   		0
#define     C_INIT_UnitConversion     			C_CENTIGRADE_C;

#define     C_INIT_CtrlTemper_LBox				(-20)
#define     C_INIT_MakupForN6_LBox				0
#define     C_INIT_MakupForN12_LBox				0
#define     C_INIT_MakupForN13_LBox				(-3)
#define     C_INIT_FreezePoint_LBox				0

#define     C_INIT_CtrlTemper_RBox				(-20)
#define     C_INIT_MakupForN6_RBox				0
#define     C_INIT_MakupForN12_RBox				0
#define     C_INIT_MakupForN13_RBox				(-3)
#define     C_INIT_FreezePoint_RBox				0
 
#endif
