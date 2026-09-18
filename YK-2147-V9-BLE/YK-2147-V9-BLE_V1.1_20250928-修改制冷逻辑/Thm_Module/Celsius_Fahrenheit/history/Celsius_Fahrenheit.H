/*********************************************************
文 件 名 : Celsius_Fahrenheit.H
文件说明 : 华氏、摄氏互转头文件
编写作者 : thm
修订作者 : 
编写时间 : 2021-1-28
版 本 号 : V1.0
==========================================================*/
#ifndef _CELSIUS_FAHRENHEIT_H_
#define _CELSIUS_FAHRENHEIT_H_ 

/****************配置部分*****************/

//转换使能配置
#define     TEMPER_TRANSLATE_ENABLE				1

//配置AD列表第一个温度值
#define     u8CelsiusTableStartTemper           ((u8)(-30))	  //摄氏
#define     u8FahrenheitTableStartTemper        ((u8)(-22))   //华氏

/****************配置结束*****************/



//从AD列表序号取摄氏温度
#define     _AdIndex2Celsius(u8AdIndex)		    (u8AdIndex + u8CelsiusTableStartTemper)

//从AD列表取华氏温度
#define     _AdIndex2Fahrenheit(u8AdIndex)		(u8AdIndex + u8FahrenheitTableStartTemper)



//外部函数声明
extern u8 F_Celsius2Fahrenheit(u8 l_u8Celsius);	           //摄氏(-40至100)转华氏温度(-40至212)
extern u8 F_Fahrenheit2Celsius(u8 l_u8Fahrenheit);	       //华氏(-40至212)转摄氏温度(-40至100)
extern u8 F_RelativeValue_C2F(u8 l_u8RelativeCelsius);	   //摄氏相对值(-20至20)转华氏相对值(-36至36)
extern u8 F_RelativeValue_F2C(u8 l_u8RelativeFahrenheit);  //华氏相对值(-36至36)转摄氏相对值(-20至20)
extern u16 F_TakeCompareTemper(u8 l_u8Temperature);		   //字符型温度值(-40至212)转化成无符号整型数据(216至468)，用于比较大小

#endif