//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: sc95f7x1x_pwm.c
//	作者		: 
//	模块功能	: PWM固件库函数C文件
//	局部函数列表:
//  最后更正日期: 2019/7/10
// 	版本		: V1.0 
//  说明        ：  
//*************************************************************

#include "sc95f7x1x_pwm.h"

uint16_t xdata PWMREG[8] _at_ 0x1040;	//PWM占空比调节寄存器
uint16_t pwm_tmpreg[8] = {0,0,0,0,0,0,0,0};		//PWM占空比调节寄存器缓存数组

/**************************************************
*函数名称：void PWM_DeInit(void)
*函数功能：PWM相关寄存器复位至缺省值
*入口参数：void
*出口参数：void
**************************************************/
void PWM_DeInit(void)
{
	static uint8_t i;
	PWMMOD  = 0X00;
	PWMCFG = 0;
	PWMCON = 0;		
	PWMFLT = 0X00;
	PWMDFR = 0X00;
	IE1 &= 0XFD;
	IP1 &= 0XFD;
	for(i=0;i<8;i++)
	{
		PWMREG[i]=0;
	}		
}

/**************************************************
*函数名称：void PWM_Init(PWM_PresSel_TypeDef PWM_PresSel, uint16_t PWM_Period)
*函数功能：PWM初始化配置函数
*入口参数：PWM_PresSel	预分频选择
		   PWM_Period	PWM周期配置
*出口参数：void
**************************************************/
void PWM_Init(PWM_PresSel_TypeDef PWM_PresSel, uint16_t PWM_Period)
{
	PWM_Period -= 1;
	PWMCFG = (PWMCFG & 0XCF) | PWM_PresSel;					//预分频
	PWMCFG = (PWMCFG & 0XF0) | (uint8_t)(PWM_Period / 256);	//周期高4位
	PWMCON = (uint8_t)(PWM_Period & 0X00FF);				//周期低8位
}

/**************************************************
*函数名称：void PWM_OutputStateConfig(uint8_t PWM_OutputPin, PWM_OutputState_TypeDef PWM_OutputState)
*函数功能：PWMx输出使能/失能配置函数
*入口参数：PWM_OutputPin	PWMx选择
		   PWM_OutputState	PWM输出状态配置
*出口参数：void
**************************************************/
void PWM_OutputStateConfig(uint8_t PWM_OutputPin, PWM_OutputState_TypeDef PWM_OutputState)
{
	uint8_t i; 
	for(i=0;i<8;i++)
	{
		if(PWM_OutputPin&(0x01<<i))
		{
			if(PWM_OutputState == PWM_OUTPUTSTATE_DISABLE)
			{
				pwm_tmpreg[i] &= 0X7FFF;	
			}
			else
			{
				pwm_tmpreg[i] |= 0X8000;
			} 
			PWMREG[i] = pwm_tmpreg[i];
		}
	}
}

/**************************************************
*函数名称：void PWM_PolarityConfig(uint8_t PWM_OutputPin, PWM_Polarity_TypeDef PWM_Polarity)
*函数功能：PWMx正/反向输出配置函数
*入口参数：PWM_OutputPin	PWMx选择
		   PWM_Polarity		PWM输出正/反向配置
*出口参数：void
**************************************************/
void PWM_PolarityConfig(uint8_t PWM_OutputPin, PWM_Polarity_TypeDef PWM_Polarity)
{
	uint8_t i; 
	for(i=0;i<8;i++)
	{
		if(PWM_OutputPin&(0x01<<i))
		{
			if(PWM_Polarity == PWM_POLARITY_NON_INVERT)
			{
				pwm_tmpreg[i] &= 0XBFFF;	
			}
			else
			{
				pwm_tmpreg[i] |= 0X4000;
			}
			PWMREG[i] = pwm_tmpreg[i];
		}
	}	
}

/**************************************************
*函数名称：void PWM_IndependentModeConfig(PWM_OutputPin_TypeDef PWM_OutputPin, uint16_t PWM_DutyCycle)
*函数功能：PWMx独立工作模式配置函数
*入口参数：PWM_OutputPin	PWMx独立通道选择
		   PWM_DutyCycle	PWM占空比配置
*出口参数：void
**************************************************/
void PWM_IndependentModeConfig(PWM_OutputPin_TypeDef PWM_OutputPin, uint16_t PWM_DutyCycle)
{
	uint8_t i; 
	for(i=0;i<8;i++)
	{
	   if(PWM_OutputPin&(0x01<<i))
	   {
	   	    pwm_tmpreg[i] = pwm_tmpreg[i] & 0XF000 | PWM_DutyCycle;
			PWMREG[i] = pwm_tmpreg[i]; 
	   }
	}
}
/**************************************************
*函数名称：void PWM_ComplementaryModeConfig(PWM_ComplementaryOutputPin_TypeDef PWM_ComplementaryOutputPin, uint16_t PWM_DutyCycle)
*函数功能：PWMxPWMy互补工作模式配置函数
*入口参数：PWM_ComplementaryOutputPin	PWMxPWMy互补通道选择
		   PWM_DutyCycle				PWM占空比配置
*出口参数：void
**************************************************/
void PWM_ComplementaryModeConfig(PWM_ComplementaryOutputPin_TypeDef PWM_ComplementaryOutputPin, uint16_t PWM_DutyCycle)
{
	PWMMOD = 0X08;
	switch(PWM_ComplementaryOutputPin)	//设置占空比
	{
		case PWM40PWM41:
			PWMREG[PWM40PWM41] = ( PWMREG[PWM40PWM41] & 0XC000 ) | PWM_DutyCycle;
			break;
		case PWM42PWM43:
			PWMREG[PWM42PWM43] = ( PWMREG[PWM42PWM43] & 0XC000 ) | PWM_DutyCycle;
			break;
		case PWM50PWM51:
			PWMREG[PWM50PWM51] = ( PWMREG[PWM50PWM51] & 0XC000 ) | PWM_DutyCycle;
			break;
		case PWM52PWM53:
			PWMREG[PWM52PWM53] = ( PWMREG[PWM52PWM53] & 0XC000 ) | PWM_DutyCycle;
			break;
		default:
			break;
	}		
}

/**************************************************
*函数名称：void PWM_DeadTimeConfig(uint8_t PWM_RisingDeadTime, uint8_t PWM_FallingDeadTime)
*函数功能：PWM互补工作模式下死区时间配置函数
*入口参数：PWM_RisingDeadTime	PWM死区上升时间	 00-FF
		   PWM_FallingDeadTime	PWM死区下降时间  00-FF
*出口参数：void
**************************************************/
void PWM_DeadTimeConfig(uint8_t PWM_RisingDeadTime, uint8_t PWM_FallingDeadTime)
{
	PWMDFR = (PWM_RisingDeadTime | (PWM_FallingDeadTime << 4));			
}

/*****************************************************
*函数名称：void PWM_Cmd(FunctionalState NewState)
*函数功能：PWM功能开关函数
*入口参数：FunctionalState NewState	功能启动/关闭选择
*出口参数：void
*****************************************************/
void PWM_Cmd(FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        PWMCFG |= 0X80;
    }
    else
    {
        PWMCFG &= 0X7F;
    }
}

/*****************************************************
*函数名称：void PWM_ITConfig(FunctionalState NewState, PriorityStatus Priority)
*函数功能：PWM中断初始化
*入口参数：FunctionalState NewState	中断使能/关闭选择
 		   PriorityStatus Priority	中断优先级选择
*出口参数：void
*****************************************************/
void PWM_ITConfig(FunctionalState NewState, PriorityStatus Priority)
{
    if (NewState != DISABLE)
    {
		IE1 |= 0X02;	        
    }
    else
    {
		IE1 &= 0XFD;   
    }

	if(Priority == LOW)
	{
		IP1 &= ~0X02;	
	}
	else
	{
		IP1 |= 0X02;	
	}
}

/*****************************************************
*函数名称：FlagStatus PWM_GetFlagStatus(void)
*函数功能：获得PWM中断标志状态
*入口参数：void
*出口参数：FlagStatus	PWM中断标志状态	
*****************************************************/
FlagStatus PWM_GetFlagStatus(void)
{
	return (FlagStatus)(PWMCFG & 0X40);
}

/*****************************************************
*函数名称：void PWM_ClearFlag(void)
*函数功能：清除PWM中断标志状态
*入口参数：void
*出口参数：void	
*****************************************************/
void PWM_ClearFlag(void)
{
	PWMCFG &= 0XBF;
}

/*****************************************************
*函数名称：FlagStatus PWM_GetFaultDetectionFlagStatus(void)
*函数功能：获得PWM故障检测标志位状态
*入口参数：void
*出口参数：FlagStatus	PWM故障检测标志位状态	
*****************************************************/
FlagStatus PWM_GetFaultDetectionFlagStatus(void)
{
	return (FlagStatus)(PWMFLT & 0X40);
}

/*****************************************************
*函数名称：void PWM_ClearFaultDetectionFlag(void)
*函数功能：清除PWM故障检测标志位状态   // ！注意,处于锁存模式下，此位可软件清除
*入口参数：void
*出口参数：void	
*****************************************************/
void PWM_ClearFaultDetectionFlag(void)
{
	PWMFLT &= 0XBF;
}

/*****************************************************
*函数名称：void PWM_FaultDetectionFunctionConfig(FunctionalState NewState)
*函数功能：PWM故障检测功能开启/关闭
*入口参数：FunctionalState NewState	故障检测功能开启/关闭
*出口参数：void
*****************************************************/
void PWM_FaultDetectionConfig(FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
		PWMFLT |= 0X80;	        
    }
    else
    {
		PWMFLT &= 0X7F;   
    }
}

/*****************************************************
*函数名称：void PWM_FaultDetectionModeConfig(PWM_FaultDetectionMode_TypeDef FaultDetectionMode, PWM_FaultDetectionVoltageSelect_TypeDef FaultDetectionVoltageSelect, PWM_FaultDetectionWaveFilteringTime_TypeDef FaultDetectionWaveFilteringTime)
*函数功能：PWM故障检测模式设置
*入口参数：FaultDetectionMode	故障检测功能模式设置： 立即模式/锁存模式
		   FaultDetectionVoltageSelect	故障检测电平选择
		   FaultDetectionWaveFilteringTime	故障检测输入信号滤波时间选择
*出口参数：void
*****************************************************/
void PWM_FaultDetectionModeConfig(PWM_FaultDetectionMode_TypeDef FaultDetectionMode, PWM_FaultDetectionVoltageSelect_TypeDef FaultDetectionVoltageSelect, PWM_FaultDetectionWaveFilteringTime_TypeDef FaultDetectionWaveFilteringTime)
{
	PWMFLT = (PWMFLT & 0XC0) | FaultDetectionMode | FaultDetectionVoltageSelect | FaultDetectionWaveFilteringTime;	        
  
}

/*****************************************************
*函数名称：void PWM_Aligned_Mode_Select(void)
*函数功能：选择PWM的对齐模式
*入口参数：PWM_Aligned_Mode 选择对齐模式
*出口参数：void	
*****************************************************/
void PWM_Aligned_Mode_Select(PWM_Aligned_Mode_TypeDef PWM_Aligned_Mode)
{
	if(PWM_Aligned_Mode == PWM_Edge_Aligned_Mode)
	{
	   PWMMOD&=0XFB;
	}
	else if(PWM_Aligned_Mode == PWM_Center_Alignment_Mode)
	{
	   PWMMOD|=0X04;
	}
}
/******************* (C) COPYRIGHT 2020 SinOne Microelectronics *****END OF FILE****/