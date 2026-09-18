/*********************************************************
文 件 名 : KeyEvent.C
文件说明 : 按键事件处理
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/
//头文件
#include "COMMON\AllInclude.h"



/*****************************************************
*函数名称：F_KeyScan(void)
*函数功能：键值扫描
*调用周期：10ms
*输入参数：无
*返回值：  键值
*说明：
*****************************************************/
#if (KEY_VALUE_LENGTH == LENGTH_32BIT)
u32 F_KeyScan(void)
{
    u32  l_u32ReadKey = 0;
  
    
    return l_u32ReadKey;
}

#elif (KEY_VALUE_LENGTH == LENGTH_16BIT)
u16 F_KeyScan(void)
{
    u16  l_u16ReadKey = 0;
  
    
    return l_u16ReadKey;
}

#elif (KEY_VALUE_LENGTH == LENGTH_8BIT)
u8 F_KeyScan(void)
{
    u8  l_u8ReadKey = 0;
  
    
    return l_u8ReadKey;
}
#endif

/*****************************************************
*函数名称：void F_PowerOnOff(void)
*函数功能：开关机事件
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_PowerOnOff(void)
{
//	_BUZZER_SHORT_BEEP;
	
}


/*****************************************************
*函数名称：void F_PowerOnOff(void)
*函数功能：滤芯选择事件
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_CoreSelect(void)
{
//	_BUZZER_SHORT_BEEP;
	
}


/*****************************************************
*函数名称：void F_IntoCoreSel(void)
*函数功能：进入滤芯选择事件
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_IntoCoreSel(void)
{
//	_BUZZER_SHORT_BEEP;
	
}



/*****************************************************
*函数名称：void F_CoreLifeReset(void)
*函数功能：滤芯寿命复位事件
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_CoreLifeReset(void)
{
//	_BUZZER_SHORT_BEEP;
	
}


/*****************************************************
*函数名称：void F_IntoSelfCheck(void)
*函数功能：自检事件
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
void F_IntoSelfCheck(void)
{
//	_BUZZER_SHORT_BEEP;
	
}
