/*********************************************************
文 件 名 : ZeroCrossDetection.C
文件说明 : 过零检测
编写作者 : thm
修订作者 : 
编写时间 : 2022-3-11
版 本 号 : V1.2
==========================================================*/
//头文件
#include "ZeroCrossDetection.H"

//变量声明
static RamDataType struct 
{
    unsigned char   u8Filter;           //滤波次数
    unsigned char   u8CaptureTimes;     //过零捕捉次数
    unsigned char   u8Time10msCnt;      //10ms计数
    unsigned char   u8LastIoStatus;     //上一次IO状态
	unsigned char   u8BaseTime1SecFlag; //1秒时基标志位
	unsigned char   u8AcFrequence;      //电网频率
    unsigned int    u16Time125usCnt;    //125us计数
}sg_sZeroBuff;
    


/*=================================================
函数名称：unsigned char F_ZeroCrossDetection_Scan(unsigned char l_u8IoStatus)
调用周期：125us
修改日期：2022-3-12
输入参数：l_u8IoStatus：过零检测IO电平状态
输出参数：检测到过零，返回1，否则返回0
函数说明：仅适用于双过零电路 
================================================*/
unsigned char F_ZeroCrossDetection_Scan(unsigned char l_u8IoStatus)
{
    static unsigned char RamDataType s_u8PowerOnInit = 1;
    unsigned char l_u8ZeroCrossFlag = 0;
        
    if (s_u8PowerOnInit)
    {
        s_u8PowerOnInit = 0;
        
        sg_sZeroBuff.u8Filter = 0;
        sg_sZeroBuff.u8CaptureTimes = 0;
        sg_sZeroBuff.u8Time10msCnt = 0;
        sg_sZeroBuff.u8LastIoStatus = 0;
        sg_sZeroBuff.u8BaseTime1SecFlag = 0;
        sg_sZeroBuff.u8AcFrequence = 0;
        sg_sZeroBuff.u16Time125usCnt = 0;
    }
    
    if (sg_sZeroBuff.u8LastIoStatus != l_u8IoStatus)
    { 
        if (++sg_sZeroBuff.u8Filter >= C_ZERO_FILTER_TIMES)
        {
            sg_sZeroBuff.u8Filter = 0;
            sg_sZeroBuff.u8LastIoStatus = l_u8IoStatus;
        
            if (l_u8IoStatus == C_ZERO_TYPE)
            {
                l_u8ZeroCrossFlag = 1;
                       
                #if ( Def_AC_FrequenceTestEn || Def_AC_BaseTimeEn )
                    sg_sZeroBuff.u8CaptureTimes++;
                #endif
                
                #if Def_AC_BaseTimeEn
                    if (sg_sZeroBuff.u8AcFrequence == C_ZERO_AC_50HZ)
                    {
                        if (++sg_sZeroBuff.u8Time10msCnt > 99)
                        {
                            sg_sZeroBuff.u8Time10msCnt = 0;
                            sg_sZeroBuff.u8BaseTime1SecFlag = 1;
                        }
                    }
                    else if (sg_sZeroBuff.u8AcFrequence == C_ZERO_AC_60HZ)
                    {
                        if (++sg_sZeroBuff.u8Time10msCnt >= 120)
                        {
                            sg_sZeroBuff.u8Time10msCnt = 0;
                            sg_sZeroBuff.u8BaseTime1SecFlag = 1;
                        }                
                    }
                    else
                    {
                        if (++sg_sZeroBuff.u8Time10msCnt >= (sg_sZeroBuff.u8AcFrequence << 1))
                        {
                            sg_sZeroBuff.u8Time10msCnt = 0;
                            sg_sZeroBuff.u8BaseTime1SecFlag = 1;
                        }                
                    }
                #endif     
            }
        }        
    }
    else
    {
        sg_sZeroBuff.u8Filter = 0;
    }
            
    
#if ( Def_AC_FrequenceTestEn || Def_AC_BaseTimeEn )
    
    if (++sg_sZeroBuff.u16Time125usCnt >= 8000)
    {
        sg_sZeroBuff.u16Time125usCnt = 0;
        
        if ((sg_sZeroBuff.u8CaptureTimes > 97) && (sg_sZeroBuff.u8CaptureTimes < 103))
        {
            sg_sZeroBuff.u8AcFrequence = C_ZERO_AC_50HZ;
        }

        else if ((sg_sZeroBuff.u8CaptureTimes > 117) && (sg_sZeroBuff.u8CaptureTimes < 123))
        {
            sg_sZeroBuff.u8AcFrequence = C_ZERO_AC_60HZ;
        }

        else
        {
            sg_sZeroBuff.u8AcFrequence = (sg_sZeroBuff.u8CaptureTimes >> 1);        
        }
    }
    
#endif 
    
    return l_u8ZeroCrossFlag;  
}




/*=================================================
函数名称：unsigned char F_TakeZeroOneSecFlag(void)
调用周期：小于等于500ms
修改日期：2022-3-12
输入参数：无
输出参数：1秒的时间标志位
函数说明：使用前必须先调用一次，对计数器进行清零后再使用
================================================*/
unsigned char F_TakeZeroOneSecFlag(void)
{
    if (sg_sZeroBuff.u8BaseTime1SecFlag)
    {
        sg_sZeroBuff.u8BaseTime1SecFlag = 0;
        return 1;
    }
    
    return 0;
}





#if (Def_Simple_TempCal == 0)

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


Temp_TypeDef F_Table_C2F(Temp_TypeDef l_sTemper)
{
    signed int x;
    
    //摄氏度使用值改变过则使用最新值进行计算，否则使用记录的计算值进行计算
    if (l_sTemper.s16LastTemp != l_sTemper.s16Temp)
    {
#if  Def_CelsiusTable_ContainDecimals
	   	x = l_sTemper.s16Temp;
#else
        x = l_sTemper.s16Temp * 10;
#endif 
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

signed int F_Table_C2F(signed int l_s16Temp)
{

#if  Def_CelsiusTable_ContainDecimals
    l_s16Temp = (l_s16Temp * 9 / 5) + 320;
#else
	l_s16Temp = l_s16Temp * 18 + 320;
#endif

    if (l_s16Temp < 0)
    {l_s16Temp -= 5;}
    else
    {l_s16Temp += 5;}
    
    l_s16Temp /= 10;

	return l_s16Temp;
}

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

