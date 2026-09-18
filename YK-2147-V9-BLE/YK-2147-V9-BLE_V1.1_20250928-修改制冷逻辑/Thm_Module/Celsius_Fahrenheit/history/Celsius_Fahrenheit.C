/*********************************************************
文 件 名 : Celsius_Fahrenheit.C
文件说明 : 摄氏温度、华氏温度互换(仅用于-76至80度温度显示）
编写作者 : thm
修订作者 : 
编写时间 : 2021-1-28
版 本 号 : V1.0
==========================================================*/
#include "COMMON\AllInclude.h"
#include "Celsius_Fahrenheit/Celsius_Fahrenheit_C.H"


#if TEMPER_TRANSLATE_ENABLE

/*****************************************************
*函数名称：u8 F_Celsius2Fahrenheit(u8 l_u8Celsius)
*函数功能：摄氏温度转华氏温度（绝对值）
*输入参数：l_u8Celsius（无符号字符型摄氏温度值）
*输出参数：无符号字符型华氏温度
*说明：    适用于-40至100度的摄氏温度转换至华氏温度
*****************************************************/
u8 F_Celsius2Fahrenheit(u8 l_u8Celsius)
{
	u16	 l_u16SearchIndex = 0;

	if (l_u8Celsius <= 100)
	{
		l_u16SearchIndex = (l_u8Celsius + 256);		
	}
	else
	{
		l_u16SearchIndex = l_u8Celsius;
	}
	l_u16SearchIndex -= 216;		//获取查表的序号

	return (C2F_TABLE[l_u16SearchIndex]); 
}

/*****************************************************
*函数名称：u8 F_Fahrenheit2Celsius(u8 l_u8Fahrenheit)
*函数功能：华氏温度转摄氏温度（绝对值）
*输入参数：l_u8Fahrenheit（无符号字符型华氏温度值）
*输出参数：无符号字符型摄氏温度
*说明：    适用于-40至212度的华氏温度转换至摄氏温度
*****************************************************/
u8 F_Fahrenheit2Celsius(u8 l_u8Fahrenheit)
{
	u16	 l_u16SearchIndex = 0;

	if (l_u8Fahrenheit <= 212)
	{
		l_u16SearchIndex = (l_u8Fahrenheit + 256);		
	}
	else
	{
		l_u16SearchIndex = l_u8Fahrenheit;
	}
	l_u16SearchIndex -= 216;		//获取查表的序号

	return (F2C_TABLE[l_u16SearchIndex]); 
}

/*****************************************************
*函数名称：u8 F_RelativeValue_C2F(u8 l_u8RelativeCelsius)
*函数功能：摄氏温度相对值转华氏温度绝对值
*输入参数：l_u8RelativeCelsius（无符号字符型摄氏氏温度相对值）
*输出参数：无符号字符型华氏温度相对值
*说明：    适用于-20至20度的摄氏温度相对值转换成华氏温度相对值
*****************************************************/
u8 F_RelativeValue_C2F(u8 l_u8RelativeCelsius)
{
	u16	 l_u16SearchIndex = 0;

	if (l_u8RelativeCelsius <= 20)
	{
		l_u16SearchIndex = (l_u8RelativeCelsius + 256);		
	}
	else
	{
		l_u16SearchIndex = l_u8RelativeCelsius;
	}
	l_u16SearchIndex -= 236;		//获取查表的序号

	return (Relative_C2F_TABLE[l_u16SearchIndex]);
}

/*****************************************************
*函数名称：u8 F_RelativeValue_F2C(u8 l_u8RelativeFahrenheit)
*函数功能：华氏温度相对值转摄氏温度绝对值
*输入参数：l_u8RelativeFahrenheit（无符号字符型华氏温度相对值）
*输出参数：无符号字符型摄氏温度相对值
*说明：    适用于-36至36度的华氏温度相对值转换成摄氏温度相对值
*****************************************************/
u8 F_RelativeValue_F2C(u8 l_u8RelativeFahrenheit)
{
	u16	 l_u16SearchIndex = 0;

	if (l_u8RelativeFahrenheit <= 36)
	{
		l_u16SearchIndex = (l_u8RelativeFahrenheit + 256);		
	}
	else
	{
		l_u16SearchIndex = l_u8RelativeFahrenheit;
	}
	l_u16SearchIndex -= 220;		//获取查表的序号

	return (Relative_F2C_TABLE[l_u16SearchIndex]);
}

#endif

/*****************************************************
*函数名称：u16 F_TakeCompareTemper(u8 l_u8Temperature)
*函数功能：字符型转化成整型的温度值
*输入参数：l_u8Temperature（无符号字符型温度值）
*输出参数：整型的温度值
*说明：    适用于-40至212度的温度值转换
*****************************************************/
u16 F_TakeCompareTemper(u8 l_u8Temperature)
{
    if (l_u8Temperature < 180)
    {
        return (l_u8Temperature + 256);
    }
    return l_u8Temperature;
}

