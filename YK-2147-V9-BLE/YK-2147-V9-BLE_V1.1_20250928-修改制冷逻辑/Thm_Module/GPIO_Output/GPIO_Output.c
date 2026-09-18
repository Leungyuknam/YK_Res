/*********************************************************
文 件 名 : GPIO_Output.C
文件说明 : GPIO驱动输出
编写作者 : thm
修订作者 : 
编写时间 : 2021-12-27
版 本 号 : V1.0
==========================================================*/
#include "COMMON\MY_MAC.h"
#include "GPIO_Output\GPIO_Output.h"

unsigned int xdata g_u16OutputBuff = 0;

/*****************************************************
*函数名称：void F_GPIO_Output(void)
*函数功能：GPIO驱动输出
*输入参数：无
*输出参数：无
*调用周期：无
*说明：
*****************************************************/
void F_GPIO_Output(void)
{
	#if (C_LOAD_NUMBERS > 0)
    _IO_BCP(_GPIO_PIN0,g_u16OutputBuff,Buffer_B0_);
	#endif

	#if (C_LOAD_NUMBERS > 1)
	_IO_BCP(_GPIO_PIN1,g_u16OutputBuff,Buffer_B1_);
	#endif

	#if (C_LOAD_NUMBERS > 2)
	_IO_BCP(_GPIO_PIN2,g_u16OutputBuff,Buffer_B2_);
	#endif

	#if (C_LOAD_NUMBERS > 3)
	_IO_BCP(_GPIO_PIN3,g_u16OutputBuff,Buffer_B3_);
	#endif

	#if (C_LOAD_NUMBERS > 4)
	_IO_BCP(_GPIO_PIN4,g_u16OutputBuff,Buffer_B4_);
	#endif

	#if (C_LOAD_NUMBERS > 5)
	_IO_BCP(_GPIO_PIN5,g_u16OutputBuff,Buffer_B5_);
	#endif

	#if (C_LOAD_NUMBERS > 6)
	_IO_BCP(_GPIO_PIN6,g_u16OutputBuff,Buffer_B6_);
    #endif

	#if (C_LOAD_NUMBERS > 7)
	_IO_BCP(_GPIO_PIN7,g_u16OutputBuff,Buffer_B7_);
	#endif

	#if (C_LOAD_NUMBERS > 8)
	_IO_BCP(_GPIO_PIN8,g_u16OutputBuff,Buffer_B8_);
	#endif

	#if (C_LOAD_NUMBERS > 9)
	_IO_BCP(_GPIO_PIN9,g_u16OutputBuff,Buffer_B9_);
	#endif

	#if (C_LOAD_NUMBERS > 10)
	_IO_BCP(_GPIO_PIN10,g_u16OutputBuff,Buffer_B10_);
    #endif

	#if (C_LOAD_NUMBERS > 11)
	_IO_BCP(_GPIO_PIN11,g_u16OutputBuff,Buffer_B11_);
	#endif

	#if (C_LOAD_NUMBERS > 12)
	_IO_BCP(_GPIO_PIN12,g_u16OutputBuff,Buffer_B12_);
	#endif

	#if (C_LOAD_NUMBERS > 13)
	_IO_BCP(_GPIO_PIN13,g_u16OutputBuff,Buffer_B13_);
	#endif

	#if (C_LOAD_NUMBERS > 14)
	_IO_BCP(_GPIO_PIN14,g_u16OutputBuff,Buffer_B14_);
	#endif

	#if (C_LOAD_NUMBERS > 15)
	_IO_BCP(_GPIO_PIN15,g_u16OutputBuff,Buffer_B15_);
	#endif
}



