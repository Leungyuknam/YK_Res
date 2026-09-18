/*********************************************************
文 件 名 : Celsius_Fahrenheit.C
文件说明 : 华氏度与摄氏度转换
编写作者 : thm
修订作者 : 
编写时间 : 2022-3-18
版 本 号 : V1.0
==========================================================*/
//头文件
#include "Celsius_Fahrenheit.H"


#if (C_Table_Rate > 5)
#error "AD表格分辨率异常"
#endif


#if (Def_Memory_TempCalc)

/*=================================================
函数名称：Temp_TypeDef F_F2C(Temp_TypeDef l_sTemper)
修改日期：2022-3-18
输入参数：华氏度绝对值结构体
输出参数：摄氏度绝对值结构体
函数说明：华氏度绝对值转摄氏度绝对值 
================================================*/
Temp_TypeDef F_F2C(Temp_TypeDef l_sTemper)
{
    //华氏度放大10位后得到的摄氏度计算值
    l_sTemper.s16Real = (((l_sTemper.s16Temp * 10) - 320 ) * 5) / 9;
    
    //4舍5入计算，获得摄氏度的使用值
    if (l_sTemper.s16Real < 0)
    {
        l_sTemper.s16Temp  = (l_sTemper.s16Real - 5) / 10;
    }
    else
    {
        l_sTemper.s16Temp  = (l_sTemper.s16Real + 5) / 10;    
    }
    
    //使用值备份
    l_sTemper.s16LastTemp = l_sTemper.s16Temp;
    
	return 	l_sTemper;
}

/*=================================================
函数名称：Temp_TypeDef F_C2F(Temp_TypeDef l_sTemper)
修改日期：2022-3-18
输入参数：摄氏度绝对值结构体
输出参数：华氏度绝对值结构体
函数说明：摄氏度绝对值转华氏度绝对值 
================================================*/
Temp_TypeDef F_C2F(Temp_TypeDef l_sTemper)
{
    signed int x;
    
    //摄氏度使用值改变过则使用最新值进行计算，否则使用记录的计算值进行计算
    if (l_sTemper.s16LastTemp != l_sTemper.s16Temp)
    {
        x = l_sTemper.s16Temp * 10;
    }
    else
    {
        x = l_sTemper.s16Real;
    }   
    
    //通过计算得到放大10倍的华氏度值
    l_sTemper.s16Temp = (x * 9 / 5) + 320;
    
    
    //4舍5入计算
    if (l_sTemper.s16Temp < 0)
    {
        l_sTemper.s16Temp -= 5;
    }
    else
    {
        l_sTemper.s16Temp += 5;
    }
    
    l_sTemper.s16Temp /= 10;
    
	return 	l_sTemper;
}


/*=================================================
函数名称：Temp_TypeDef F_Table2C(unsigned int l_u16TableNum)
修改日期：2022-3-28
输入参数：AD表格序列号
输出参数：摄氏度绝对值结构体
函数说明：摄氏AD表格序列号转摄氏度绝对值 
================================================*/
Temp_TypeDef F_Table2C(unsigned int l_u16TableNum)
{
	
   Temp_TypeDef l_sTemper;

#if (C_Table_Rate > 0)

	l_sTemper.s16Real = (C_Table_StartTemp * 10) + (l_u16TableNum * C_Table_Rate);
    l_sTemper.s16Temp = l_sTemper.s16Real / 10;
	l_sTemper.s16LastTemp = l_sTemper.s16Temp;

#else

	l_sTemper.s16Real = C_Table_StartTemp + l_u16TableNum;
	l_sTemper.s16Temp = l_sTemper.s16Real;
	l_sTemper.s16LastTemp = l_sTemper.s16Real;

#endif

	return l_sTemper;

}


/*=================================================
函数名称：Temp_TypeDef F_Table2F(unsigned int l_u16TableNum)
修改日期：2022-3-28
输入参数：AD表格序列号
输出参数：华氏度绝对值结构体
函数说明：摄氏AD表格序列号转华氏度绝对值 
================================================*/
Temp_TypeDef F_Table2F(unsigned int l_u16TableNum)
{
   Temp_TypeDef l_sTemper;

#if (C_Table_Rate > 0)

	l_sTemper.s16Real = (C_Table_StartTemp * 10) + (l_u16TableNum * C_Table_Rate);

#else

	l_sTemper.s16Real = (C_Table_StartTemp + l_u16TableNum) * 10;

#endif
    
    l_sTemper.s16Temp = (l_sTemper.s16Real * 9 / 5) + 320;
      
    if (l_sTemper.s16Temp < 0)
    {
        l_sTemper.s16Temp -= 5;
    }
    else
    {
        l_sTemper.s16Temp += 5;
    }
    
    l_sTemper.s16Temp /= 10;
	l_sTemper.s16LastTemp = l_sTemper.s16Temp;
    
	return 	l_sTemper;
}


/*=================================================
函数名称：Temp_TypeDef F_Relative_F2C(Temp_TypeDef l_sTemper)
修改日期：2022-3-18
输入参数：华氏度相对值结构体
输出参数：摄氏度相对值结构体
函数说明：华氏度相对值转摄氏度相对值 
================================================*/
Temp_TypeDef F_Relative_F2C(Temp_TypeDef l_sTemper)
{
    //华氏度放大10位后得到的摄氏度计算值
    l_sTemper.s16Real = ((l_sTemper.s16Temp * 50) / 9);
    
    //4舍5入计算，获得摄氏度的使用值
    if (l_sTemper.s16Real < 0)
    {
        l_sTemper.s16Temp  = (l_sTemper.s16Real - 5) / 10;
    }
    else
    {
        l_sTemper.s16Temp  = (l_sTemper.s16Real + 5) / 10;    
    }
    
    //使用值备份
    l_sTemper.s16LastTemp = l_sTemper.s16Temp;
    
	return 	l_sTemper;
}


/*=================================================
函数名称：Temp_TypeDef F_Relative_C2F(Temp_TypeDef l_sTemper)
修改日期：2022-3-18
输入参数：摄氏度相对值结构体
输出参数：华氏度相对值结构体
函数说明：摄氏度相对值转华氏度相对值
================================================*/
Temp_TypeDef F_Relative_C2F(Temp_TypeDef l_sTemper)
{
    signed int x;
    
    //摄氏度使用值改变过则使用最新值进行计算，否则使用记录的计算值进行计算
    if (l_sTemper.s16LastTemp != l_sTemper.s16Temp)
    {
        x = l_sTemper.s16Temp * 10;
    }
    else
    {
        x = l_sTemper.s16Real;
    }
    
    
    //通过计算得到放大10倍的华氏度值
    l_sTemper.s16Temp = (x * 9 / 5);
    
    
    //4舍5入计算
    if (l_sTemper.s16Temp < 0)
    {
        l_sTemper.s16Temp -= 5;
    }
    else
    {
        l_sTemper.s16Temp += 5;
    }
    
    l_sTemper.s16Temp /= 10;
    
	return 	l_sTemper;
}



#else



/*=================================================
函数名称：signed int F_C2F(signed int l_s16Temp)
修改日期：2022-3-18
输入参数：摄氏度绝对值
输出参数：华氏度绝对值
函数说明：摄氏度绝对值转华氏度绝对值
================================================*/
signed int F_C2F(signed int l_s16Temp)
{

	l_s16Temp = l_s16Temp * 18 + 320;

    if (l_s16Temp < 0)
    {l_s16Temp -= 5;}
    else
    {l_s16Temp += 5;}
    
    l_s16Temp /= 10;

	return l_s16Temp;
}

/*=================================================
函数名称：signed int F_F2C(signed int l_s16Temp)
修改日期：2022-3-18
输入参数：华氏度绝对值
输出参数：摄氏度绝对值
函数说明：华氏度绝对值转摄氏度绝对值
================================================*/
signed int F_F2C(signed int l_s16Temp)
{
    l_s16Temp = (l_s16Temp * 10 - 320) * 5 / 9;
    
    if (l_s16Temp < 0)
    {l_s16Temp -= 5;}
    else
    {l_s16Temp += 5;}
    
    l_s16Temp /= 10;

	return l_s16Temp;
}

/*=================================================
函数名称：signed int F_Table2C(unsigned int l_u16TableNum)
修改日期：2022-3-28
输入参数：AD表格序列号
输出参数：摄氏度绝对值
函数说明：摄氏AD表格序列号转摄氏度绝对值 
================================================*/
signed int F_Table2C(unsigned int l_u16TableNum)
{
	signed int l_s16Temper;

#if (C_Table_Rate > 0)

	l_s16Temper = (C_Table_StartTemp * 10) + (l_u16TableNum * C_Table_Rate);
	l_s16Temper /= 10;

#else

	l_s16Temper = C_Table_StartTemp + l_u16TableNum;

#endif

	return l_s16Temper;

}

/*=================================================
函数名称：signed int F_Table2F(unsigned int l_u16TableNum)
修改日期：2022-3-28
输入参数：AD表格序列号
输出参数：华氏度绝对值
函数说明：摄氏AD表格序列号转华氏度绝对值 
================================================*/
signed int F_Table2F(unsigned int l_u16TableNum)
{
   signed int l_s16Temper;

#if (C_Table_Rate > 0)

	l_s16Temper = (C_Table_StartTemp * 10) + (l_u16TableNum * C_Table_Rate);

#else

	l_s16Temper = (C_Table_StartTemp + l_u16TableNum) * 10;

#endif
    
    l_s16Temper = (l_s16Temper * 9 / 5) + 320;
      
    if (l_s16Temper < 0)
    {
        l_s16Temper -= 5;
    }
    else
    {
        l_s16Temper += 5;
    }
    
    l_s16Temper /= 10;
    
	return 	l_s16Temper;
}


/*=================================================
函数名称：signed int F_Relative_C2F(signed int l_s16Temp)
修改日期：2022-3-18
输入参数：摄氏度相对值
输出参数：华氏度相对值
函数说明：摄氏度相对值转华氏度相对值
================================================*/
signed int F_Relative_C2F(signed int l_s16Temp)
{
    l_s16Temp = l_s16Temp * 18;
    
    if (l_s16Temp < 0)
    {l_s16Temp -= 5;}
    else
    {l_s16Temp += 5;}
    
    l_s16Temp /= 10;

	return l_s16Temp;
}


/*=================================================
函数名称：signed int F_Relative_F2C(signed int l_s16Temp)
修改日期：2022-3-18
输入参数：摄氏度相对值
输出参数：华氏度相对值
函数说明：摄氏度相对值转华氏度相对值
================================================*/
signed int F_Relative_F2C(signed int l_s16Temp)
{
    l_s16Temp = (l_s16Temp * 50) / 9;
    
    if (l_s16Temp < 0)
    {l_s16Temp -= 5;}
    else
    {l_s16Temp += 5;}
    
    l_s16Temp /= 10;

	return l_s16Temp;
}

#endif

