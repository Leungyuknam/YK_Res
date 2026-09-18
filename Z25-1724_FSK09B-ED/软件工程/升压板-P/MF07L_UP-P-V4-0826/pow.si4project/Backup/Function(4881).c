#include "..\Include\Include.h"

X_REG SW_Status_Enum mSwStatus = SW_CLOSE;
Sys_TypeDef X_REG Sys;




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




/*****************************************************
*函数名称：void F_CompError_Test(void)
*函数功能：压缩机报错检测
*入口参数：void
*出口参数：g_CarRfgrt.u8CompError
*调用周期：125uS
*说明：
*****************************************************/

void OutputDeal()
{	
	u8 xdata s_u8OutBuff = 0xFF;
	if(ControlDisConnect_Cnt < 65535)
		ControlDisConnect_Cnt ++;
	if(ControlDisConnect_Cnt < MAX_DISCONECT_TIME)
	{
		Sys.PowMessage.PowRecSIErr = 0;
//		if(Sys.Load.Heat)
//		{HEAT_ON();}
//		else 
//		{HEAT_OFF();}




	}
	else
	{
		Sys.PowMessage.PowRecSIErr = 1;
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
	if(SysSet.fg.TurnOn)
	{
		if(WING_HEAT == SysSet.mWingMode)
		{
			if(SysInfo.Loader.PTC && !Fuc.Sign.HotIDLEErr)
				BufEn = 1;	
			else
				BufEn = 0;			//	温度设定，室温大于设定值时，风机、左右摇头和上下摇头一直运行要求是停止工作
		}
		else
			BufEn = 1;	
		if(SysSet.fg.UDOSC && !SysSet.TimeOnCount && !Fuc.Sign.T1Err && BufEn)
		{
			F_StepMotorRstControl();
			if(Rst_Ok == g_u8SMRstState)
			{
				F_StepMotorSwing(UPDN_UP,UPDN_DN,c_SMSpeedSwing,0,0);
			}
		}
		else
		{
			F_StepMotorStop();
		}
	}
	else
	{	F_StepMotorStop();}
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
	if(SysSet.fg.TurnOn)
	{
		if(WING_HEAT == SysSet.mWingMode)
		{
			if(SysInfo.Loader.PTC && !Fuc.Sign.HotIDLEErr)
				BufEn = 1;	
			else
				BufEn = 0;			//	温度设定，室温大于设定值时，风机、左右摇头和上下摇头一直运行要求是停止工作
		}
		else
			BufEn = 1;	

		if(SysSet.fg.RLOSC && !SysSet.TimeOnCount && !Fuc.Sign.T1Err && BufEn)
		{
			F_RL_StepMotorRstControl();
			if(Rst_Ok == g_u8RL_SMRstState)
			{
				if(SysSet.mAngle == ANGLE_30)
					F_RL_StepMotorSwing(C30D_R,C30D_L,c_RL_SMSpeedSwing,0,0);  	//
				else if(SysSet.mAngle == ANGLE_50)
					F_RL_StepMotorSwing(C50D_R,C50D_L,c_RL_SMSpeedSwing,0,0);	//
				else if(SysSet.mAngle == ANGLE_80)
					F_RL_StepMotorSwing(C80D_R,C80D_L,c_RL_SMSpeedSwing,0,0);	//
			}														  
		}
		else
		{
			F_RL_StepMotorStop();
		}
	}
	else
	{	
		F_RL_StepMotorStop();
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
	//F_FanSpeedEvent();
	F_UDOscEvent();
	F_RLOscEvent();
}


