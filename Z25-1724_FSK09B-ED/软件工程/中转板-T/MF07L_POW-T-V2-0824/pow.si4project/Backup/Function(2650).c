#define _Native_FUN_H
#include "..\Include\Include.h"







/********************************************************************************************************
* @ name   : void F_TimeEvent(void)
* @ msg    : 时间变量处理
* @ param   (*)
* @ return  (*)
 ********************************************************************************************************/
static void F_TimeEvent(void)
{
    if (GetTimeTick(TIME_100MS))
    {

    }
    if (GetTimeTick(TIME_1S))
    {
        
    }
}


/********************************************************************************************************
* @ name   : void F_FunCtrl(void)
* @ msg    : 功能处理总函数
* @ param   (*)
* @ return  (*)
 ********************************************************************************************************/
void F_FunCtrl(void)
{
    F_TimeEvent();
	

    if (GetTimeTick(TIME_10MS))
    {

    }
    if (GetTimeTick(TIME_1S))
    {

    }
}




void OutputDeal()
{	
	u8 xdata s_u8OutBuff = 0xFF;
	if(ControlDisConnect_Cnt < 65535)
		ControlDisConnect_Cnt ++;
	if(ControlDisConnect_Cnt < 500)
	{
		Out.PowMessage.PowRecSIErr = 0;
//		if(Sys.Load.Heat)
//		{HEAT_ON();}
//		else 
//		{HEAT_OFF();}




	}
	else
	{
		Out.PowMessage.PowRecSIErr = 1;
		//HEAT_OFF();

	}
}



/**************************************************
*函数名称：void  F_UDOscEvent(void) 
*函数功能：摆头控制
*调用时间：100ms
*入口参数：void
*出口参数：void  
**************************************************/
void F_UDOscEvent()
{
	u8 BufEn = 0;
	if(Out.UpDnAngleCol.AngleEn)
	{
		if((Out.UpDnAngleCol.SetCol == M_RST_NEED) && (Out.UpDnAngleCol.RollCode != Out.UpDnAngleState.RollCode))
		{
			g_u8SMRstState = Rst_Need;
			Out.UpDnAngleState.RollCode =  Out.UpDnAngleCol.RollCode;
		}
		F_StepMotorRstControl();
		if(Rst_Ok == g_u8SMRstState)
		{
			F_StepMotorSwing(Out.UpDnFarAngleVal,Out.UpDnNearAngleVal,c_SMSpeedSwing,0,0);
		}
		Out.UpDnAngleState.AngleEn = 1;
		Out.UpDnAngleState.NowStep =  g_u8SMRstState;
		
	}
	else
	{	
		F_StepMotorStop();
		Out.LeftRightAngleState.AngleEn = 0;
	}
}


/**************************************************
*函数名称：void  F_RLOscEvent(void) 
*函数功能：摆头控制
*调用时间：100ms
*入口参数：void
*出口参数：void  
**************************************************/
void F_RLOscEvent()
{
	u8 BufEn = 0;
	if(Out.LeftRightAngleCol.AngleEn)
	{
		if((Out.LeftRightAngleCol.SetCol == M_RST_NEED) && (Out.LeftRightAngleCol.RollCode != Out.LeftRightAngleState.RollCode))
		{
			g_u8RL_SMRstState = Rst_Need;
			Out.LeftRightAngleState.RollCode =  Out.LeftRightAngleCol.RollCode;
		}
		F_RL_StepMotorRstControl();
		if(Rst_Ok == g_u8RL_SMRstState)
		{
			F_RL_StepMotorSwing(Out.LeftRightFarAngleVal,Out.LeftRightNearAngleVal,c_RL_SMSpeedSwing,0,0);  	//
		}	
		Out.LeftRightAngleState.AngleEn = 1;
		Out.LeftRightAngleState.NowStep =  g_u8RL_SMRstState;
	}
	else
	{	
		F_RL_StepMotorStop();
		Out.LeftRightAngleState.AngleEn = 0;
	}
}


void PWM_Set(u8 Dat)
{
	if(Dat)
	{
		//P1M0 = P1M0&0x0F|0x80;              //P11设置为推挽输出	
		PWM3_MAP = 0x11;					//PWM3映射P11口
		//周期计算 	= 0xFF / (Fosc / PWM分频系数)		（Fosc见系统时钟配置的部分）
		//			= 0xFF /(16000000 / 4)			
		// 			= 255 /4000000
		//			= 63.75us		即15.69KHZ		

		PWM3P = 0xFF;						 //PWM周期为0xFF
		//有效电平时间计算（即占空比） 	
		//			= 0x55 / (Fosc / PWM分频系数)		（Fosc见系统时钟配置的部分）
		//			= 0x55 /(16000000 / 4)			
		// 			= 85 /4000000
		//			= 21.25us		占空比为 21.25 / 63.75 = 34%

		PWM3D = 0x7f;						 //PWM占空比设置
		PWM3C = 0x97; 						 //使能PWM3，关闭中断，允许输出，时钟4分频
	}
	else
	{
		PWM3C &= ~0x10;
		P_PWM = 0;
	}
}

void F_FanSpeedEvent()
{
	static u8 xdata LastDat = 0;
	if(Out.FanSpeed != LastDat)
	{
		LastDat = Out.FanSpeed;
		PWM_Set(1);
	}
	else if(0 == Out.FanSpeed)
	{
		PWM_Set(0);
	}
}

/**************************************************
*函数名称：void  F_ModeWork(void) 
*函数功能：工作逻辑
*调用时间：100ms
*入口参数：void
*出口参数：void  
**************************************************/
void F_ModeWork(void)
{
	F_FanSpeedEvent();
	F_UDOscEvent();
	F_RLOscEvent();
}


