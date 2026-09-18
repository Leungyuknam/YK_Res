/*********************************************************
文 件 名 : Celsius_Fahrenheit.H
文件说明 : 
编写作者 : thm
修订作者 : 
编写时间 : 2022-3-18
版 本 号 : V1.0
说明     ：

==========================================================*/
#ifndef _Celsius_Fahrenheit_H_
#define _Celsius_Fahrenheit_H_ 

#define		C_Table_Rate                          5	      //AD表格分辨率：0表示无小数位，1表示0.1度，5表示0.5度，支持0-5范围
#define		C_Table_StartTemp       (signed int)(-40)     //AD表格起始温度值（AD表格必须为由小到大的摄氏度表格）

#define		Def_Memory_TempCalc                   0	      //1：带记忆转换(华氏->摄氏->华氏互转一致)    
                                                          //0：不带记忆转换(华氏->摄氏->华氏互转不一定一致)
							                              //常规律配置为0即可，特殊要求才配为1	  

typedef struct
{  
    signed int     s16Temp;         //温度使用值（4舍5入原则）
    signed int     s16LastTemp;     //温度使用值备份（4舍5入原则）
    signed int     s16Real;         //摄氏温度值*10（含小数点后一位信息）
}Temp_TypeDef;


//函数声明

#if Def_Memory_TempCalc

extern Temp_TypeDef F_Table2C(unsigned int l_u16TableNum);   //输入摄氏AD表格的序列号，得到实际的摄氏度值
extern Temp_TypeDef F_Table2F(unsigned int l_u16TableNum);	 //输入摄氏AD表格的序列号，得到实际的华氏度值
extern Temp_TypeDef F_F2C(Temp_TypeDef l_sTemper);		     //输入华氏度值，得到摄氏度值
extern Temp_TypeDef F_C2F(Temp_TypeDef l_sTemper);		     //输入摄氏度值，得到华氏度值
extern Temp_TypeDef F_Relative_F2C(Temp_TypeDef l_sTemper);	 //输入华氏度相对值，得到摄氏度相对值
extern Temp_TypeDef F_Relative_C2F(Temp_TypeDef l_sTemper);	 //输入摄氏度相对值，得到华氏度相对值

#else

extern signed int F_Table2C(unsigned int l_u16TableNum);	 //输入摄氏AD表格的序列号，得到实际的摄氏度值
extern signed int F_Table2F(unsigned int l_u16TableNum);	 //输入摄氏AD表格的序列号，得到实际的华氏度值
extern signed int F_C2F(signed int l_s16Temp);			     //输入摄氏度值，得到华氏度值
extern signed int F_F2C(signed int l_s16Temp);			     //输入华氏度值，得到摄氏度值
extern signed int F_Relative_C2F(signed int l_s16Temp);		 //输入摄氏度相对值，得到华氏度相对值
extern signed int F_Relative_F2C(signed int l_s16Temp);		 //输入华氏度相对值，得到摄氏度相对值

#endif


#endif
