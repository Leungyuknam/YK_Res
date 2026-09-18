/*********************************************************
文 件 名 : AnalogPwm.C
文件说明 : 模拟PWM
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/
//头文件
#include "COMMON\AllInclude.h"

/********************************************************************
*函数名：  void F_AnalogPwm_Drive(ANALOG_PWM_Typedef PWM,unsigned char l_u8PwmChannel)
*输入参数：PWM     		          PWM数据缓存
*输出参数：无
*调用周期：125uS
*函数说明：
/*******************************************************************/

void F_AnalogPwm_Drive(ANALOG_PWM_Typedef *PWM)
{
	unsigned char  l_u8PwmBuff = 0;

	if (PWM->u16Duty == 0)
	{
		PWM->u16PwmCnt = 0;
		_IO_PWM_OFF;
	}
	else
	{
		if (++PWM->u16PwmCnt > PWM->u16Period)
		{
			PWM->u16PwmCnt = 1;
		}
	
		if (PWM->u16PwmCnt <= PWM->u16Duty)
		{
			_IO_PWM_ON;
		}
		else
		{
		    _IO_PWM_OFF;
		}
	}

	switch (PWM->u8Channel)
	{

#if (Def_PWM0)
		case C_CH_PWM0:
		{ 
		#if Def_IO_Drive
			if (l_u8PwmBuff & (1 << C_CH_PWM0))
			{PWM0_GPIO |= (1 << PWM0_PIN);}
			else
			{PWM0_GPIO &= ~(1 << PWM0_PIN);}
		#else
			if (l_u8PwmBuff & (1 << C_CH_PWM0))
			{IO_PWM0 = 1;}
			else
			{IO_PWM0 = 0;}
		#endif
			break;
		}
#endif
	
#if (Def_PWM1)
		case C_CH_PWM1:
		{ 
		#if Def_IO_Drive
			if (l_u8PwmBuff & (1 << C_CH_PWM1))
			{PWM1_GPIO |= (1 << PWM1_PIN);}
			else
			{PWM1_GPIO &= ~(1 << PWM1_PIN);}
		#else
			if (l_u8PwmBuff & (1 << C_CH_PWM1))
			{IO_PWM1 = 1;}
			else
			{IO_PWM1 = 0;}
		#endif
			break;
		}
#endif

#if (Def_PWM2)
		case C_CH_PWM2:
		{ 
		#if Def_IO_Drive
			if (l_u8PwmBuff & (1 << C_CH_PWM2))
			{PWM2_GPIO |= (1 << PWM2_PIN);}
			else
			{PWM2_GPIO &= ~(1 << PWM2_PIN);}
		#else
			if (l_u8PwmBuff & (1 << C_CH_PWM2))
			{IO_PWM2 = 1;}
			else
			{IO_PWM2 = 0;}
		#endif
			break;
		}
#endif

#if (Def_PWM3)
		case C_CH_PWM3:
		{ 
		#if Def_IO_Drive
			if (l_u8PwmBuff & (1 << C_CH_PWM3))
			{PWM3_GPIO |= (1 << PWM3_PIN);}
			else
			{PWM3_GPIO &= ~(1 << PWM3_PIN);}
		#else
			if (l_u8PwmBuff & (1 << C_CH_PWM3))
			{IO_PWM3 = 1;}
			else
			{IO_PWM3 = 0;}
		#endif
			break;
		}
#endif

#if (Def_PWM4)
		case C_CH_PWM4:
		{ 
		#if Def_IO_Drive
			if (l_u8PwmBuff & (1 << C_CH_PWM4))
			{PWM4_GPIO |= (1 << PWM4_PIN);}
			else
			{PWM4_GPIO &= ~(1 << PWM4_PIN);}
		#else
			if (l_u8PwmBuff & (1 << C_CH_PWM4))
			{IO_PWM4 = 1;}
			else
			{IO_PWM4 = 0;}
		#endif
			break;
		}
#endif

#if (Def_PWM5)
		case C_CH_PWM5:
		{ 
		#if Def_IO_Drive
			if (l_u8PwmBuff & (1 << C_CH_PWM5))
			{PWM5_GPIO |= (1 << PWM5_PIN);}
			else
			{PWM5_GPIO &= ~(1 << PWM5_PIN);}
		#else
			if (l_u8PwmBuff & (1 << C_CH_PWM5))
			{IO_PWM5 = 1;}
			else
			{IO_PWM5 = 0;}
		#endif
			break;
		}
#endif

#if (Def_PWM6)
		case C_CH_PWM6:
		{ 
		#if Def_IO_Drive
			if (l_u8PwmBuff & (1 << C_CH_PWM6))
			{PWM6_GPIO |= (1 << PWM6_PIN);}
			else
			{PWM6_GPIO &= ~(1 << PWM6_PIN);}
		#else
			if (l_u8PwmBuff & (1 << C_CH_PWM6))
			{IO_PWM6 = 1;}
			else
			{IO_PWM6 = 0;}
		#endif
			break;
		}
#endif

#if (Def_PWM7)
		case C_CH_PWM7:
		{ 
		#if Def_IO_Drive
			if (l_u8PwmBuff & (1 << C_CH_PWM7))
			{PWM7_GPIO |= (1 << PWM7_PIN);}
			else
			{PWM7_GPIO &= ~(1 << PWM7_PIN);}
		#else
			if (l_u8PwmBuff & (1 << C_CH_PWM7))
			{IO_PWM7 = 1;}
			else
			{IO_PWM7 = 0;}
		#endif
			break;
		}
#endif

		default: break;	
	}

}


/********************************************************************
*函数名：  void F_BreatheLight_Drive(ANALOG_BREATH_Typedef *BREATH, ANALOG_PWM_Typedef *PWM)
*输入参数：BREATH     呼吸变化速率相关参数
		   PWM        执行PWM
*输出参数：无
*调用周期：
*函数说明：
/*******************************************************************/

void F_BreatheLight_Drive(ANALOG_BREATH_Typedef *BREATH, ANALOG_PWM_Typedef *PWM)
{
	
	//响应速率控制
	if (++BREATH->u8ResponseCnt < BREATH->u8Response)
	{
		return ;
	}
	else
	{
		BREATH->u8ResponseCnt = 0;	
	}

	//占空比递增
	if (!BREATH->u8PwmDirection)
	{
		if (++PWM->u16Duty >= BREATH->u16PwmDutyMax)
		{
			BREATH->u8PwmDirection = 1;
		}
	}

	//占空比递减
	else
	{
		if (PWM->u16Duty > BREATH->u16PwmDutyMin)
		{
			if (--PWM->u16Duty == BREATH->u16PwmDutyMin)
			{
				BREATH->u8PwmDirection = 0;
				PWM->u16Duty = 0;
			}
		}
		else
		{
			BREATH->u8PwmDirection = 0;
			PWM->u16Duty = 0;		
		}			
	}			
}






