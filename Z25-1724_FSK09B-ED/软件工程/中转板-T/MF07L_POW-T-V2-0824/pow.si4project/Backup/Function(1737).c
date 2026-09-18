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
		Out.UpDnAngleState.AngleEn = 0;
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


void PWM_Set(u8 Frq)
{
	u16 xdata u16Buf = 0;
	if(Frq)
	{
		P1M0 = P1M0&0x0F|0x80;              //P11设置为推挽输出	
		PWM0_MAP = 0x11;					//PWM0通道映射P11口
		//PWM01_MAP = 0x11;					//PWM01通道映射P10口
		//独立模式下，PWM0和PWM01共用一个周期寄存器
		//PWM0的占空比调节使用			PWM0组的占空比寄存器
		//PWM01的占空比调节使用			PWM0组的死区寄存器

		//周期计算 	= 0x03ff / (Fosc / PWM分频系数)		（Fosc见系统时钟配置的部分）
		//			= 0x03ff / (16000000 / 8)			
		// 			= 1023   /2000000
		//			= 511.5us		   		约1.955kHz
		u16Buf = 125000/Frq;
		PWM0PH = u16Buf>>8; 		//0x03;						//周期高4位设置为0x03
		PWM0PL = u16Buf;		//0xFF;						//周期低8位设置为0xFF

		//占空比计算= 0x0155 / (Fosc / PWM分频系数)		（Fosc见系统时钟配置的部分）
		//			= 0x0155 / (16000000 / 8)			
		// 			= 341 	 / 2000000
		//			= 170.5us		   占空比为 170.5/511.5 = 33.3%
		u16Buf >>= 1;
		PWM0DH = u16Buf>>8;						//PWM0高4位占空比0x01
		PWM0DL = u16Buf;						//PWM0低8位占空比0x55
		PWM0DTH = u16Buf>>8;						//PWM01高4位占空比0x01
		PWM0DTL = u16Buf;						//PWM01低8位占空比0x55
		
		PWM0C = 0x03;					  	//PWM0高有效，PWM01高有效，时钟8分频 
		PWM0EN = 0x0f;						//使能PWM0，工作于独立模式

		
	}
	else
	{
		PWM0C &= ~0x01;
		PWM0EN &= ~0x01;
		P_PWM = 0;
	}
}

void F_FanSpeedEvent()
{
	static u8 xdata LastDat = 0;
	if(Out.FanSpeed != LastDat)
	{
		LastDat = Out.FanSpeed;
		PWM_Set(Out.FanSpeed);
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


