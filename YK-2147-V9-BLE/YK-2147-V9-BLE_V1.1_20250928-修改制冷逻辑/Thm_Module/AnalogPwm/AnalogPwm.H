/*********************************************************
文 件 名 : AnalogPwm.H
文件说明 : 
编写作者 : thm
修订作者 : 
编写时间 : 2021-2-5
版 本 号 : V1.0
==========================================================*/
#ifndef _ANALOG_PWM_H_
#define _ANALOG_PWM_H_ 

/*************************需配置部分***********************/
//模拟PWM使能（0：除能， 1：使能）
#define		Def_PWM0          1
#define		Def_PWM1          0
#define		Def_PWM2          0
#define		Def_PWM3          0
#define		Def_PWM4          0
#define		Def_PWM5          0
#define		Def_PWM6          0
#define		Def_PWM7          0

//模拟PWM输出方向（1：正向，0：反向）
#define		Def_PWM_Direction     1

//IO驱动方式（0：IO位驱动，1：PIN驱动）
#define		Def_IO_Drive          0

//PWM的IO口定义（根据使能的PWM进行相应配置，仅PIN驱动时需配置）
#if (Def_IO_Drive == 1)

#define		PWM0_GPIO         P0
#define		PWM0_PIN          0

#define		PWM1_GPIO         P0
#define		PWM1_PIN          0

#define		PWM2_GPIO         P0
#define		PWM2_PIN          0

#define		PWM3_GPIO         P0
#define		PWM3_PIN          0

#define		PWM4_GPIO         P0
#define		PWM4_PIN          0

#define		PWM5_GPIO         P0
#define		PWM5_PIN          0

#define		PWM6_GPIO         P0
#define		PWM6_PIN          0

#define		PWM7_GPIO         P0
#define		PWM7_PIN          0

#else	//（根据使能的PWM进行相应配置，仅IO位驱动时需配置）

#define		IO_PWM0           P27
#define		IO_PWM1           P15
#define		IO_PWM2           P00
#define		IO_PWM3           P00
#define		IO_PWM4           P00
#define		IO_PWM5           P00
#define		IO_PWM6           P00
#define		IO_PWM7           P00

#endif

/**************************配置结束************************/

//模拟PWM通道
#define		C_CH_PWM0         0
#define		C_CH_PWM1         1
#define		C_CH_PWM2         2
#define		C_CH_PWM3         3
#define		C_CH_PWM4         4
#define		C_CH_PWM5         5
#define		C_CH_PWM6         6
#define		C_CH_PWM7         7

//输出方向
#if Def_PWM_Direction
#define		_IO_PWM_OFF       l_u8PwmBuff &= ~(1 << (PWM->u8Channel))	    
#define		_IO_PWM_ON        l_u8PwmBuff |= (1 << (PWM->u8Channel))			
#else
#define		_IO_PWM_OFF       l_u8PwmBuff |= (1 << (PWM->u8Channel))	    
#define		_IO_PWM_ON        l_u8PwmBuff &= ~(1 << (PWM->u8Channel))			
#endif

//数据类型定义
typedef struct
{
	unsigned int  u16PwmCnt;
	unsigned int  u16Duty;
	unsigned int  u16Period;
	unsigned char u8Channel;	
}ANALOG_PWM_Typedef;

typedef struct
{
	unsigned char u8ResponseCnt;	  //起始占空比（通常设置0）
	unsigned char u8PwmDirection;	  //呼吸变化的方向（0：占空比逐渐增加，1：占空比逐渐减少）
	unsigned char u8Response;		  //调节呼吸速率
	unsigned int  u16PwmDutyMin;	  //PWM最小占空比
	unsigned int  u16PwmDutyMax;	  //PWM最大占空比	
}ANALOG_BREATH_Typedef;


extern ANALOG_PWM_Typedef xdata g_sDefrost;

//函数声明
extern void F_AnalogPwm_Drive(ANALOG_PWM_Typedef *PWM);
extern void F_BreatheLight_Drive(ANALOG_BREATH_Typedef *BREATH,ANALOG_PWM_Typedef *PWM);
			
#endif


