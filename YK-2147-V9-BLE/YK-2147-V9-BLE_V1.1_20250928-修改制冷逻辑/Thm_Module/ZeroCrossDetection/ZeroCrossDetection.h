/*********************************************************
文 件 名 : ZeroCrossDetection.H
文件说明 : 
编写作者 : thm
修订作者 : 
编写时间 : 2022-3-11
版 本 号 : V1.2
==========================================================*/
#ifndef _ZERO_CROSS_DETECTION_H_
#define _ZERO_CROSS_DETECTION_H_ 

/*************************需配置部分***********************/
#define		Def_AC_FrequenceTestEn          0   //市电频率检测使能
#define		Def_AC_BaseTimeEn               1   //市电产生时基1秒使能
#define		C_ZERO_FILTER_TIMES             3   //过零滤波次数
#define		C_ZERO_TYPE                     1   //过零时IO口电平状态（只能是0或1）
/**************************配置结束************************/

#define		C_ZERO_AC_50HZ                  50
#define		C_ZERO_AC_60HZ                  60

#ifndef    RamDataType
#define    RamDataType     xdata   
#endif

#define		Def_Simple_TempCal        0	   //1：常规转换     0：带记忆转换
#define		Def_CelsiusTable_ContainDecimals      0	   //1：温度AD表格是含一位小数点的摄氏度   0：温度AD表格是华氏度或无小数位的摄氏度

typedef struct
{  
    signed int     s16Temp;         //温度使用值（4舍5入原则）
    signed int     s16LastTemp;     //温度使用值备份（4舍5入原则）
    signed int     s16Real;         //摄氏温度值*10（含小数点后一位信息）
}Temp_TypeDef;



//函数声明
extern unsigned char F_ZeroCrossDetection_Scan(unsigned char l_u8IoStatus);
extern unsigned char F_TakeZeroOneSecFlag(void);


#if Def_Simple_TempCal
extern signed int F_C2F(signed int l_s16Temp);
extern signed int F_F2C(signed int l_s16Temp);
extern signed int F_Relative_C2F(signed int l_s16Temp);
extern signed int F_Relative_F2C(signed int l_s16Temp);
#else
extern Temp_TypeDef F_F2C(Temp_TypeDef l_sTemper);
extern Temp_TypeDef F_C2F(Temp_TypeDef l_sTemper);
extern Temp_TypeDef F_Relative_F2C(Temp_TypeDef l_sTemper);
extern Temp_TypeDef F_Relative_C2F(Temp_TypeDef l_sTemper);
#endif


#endif
