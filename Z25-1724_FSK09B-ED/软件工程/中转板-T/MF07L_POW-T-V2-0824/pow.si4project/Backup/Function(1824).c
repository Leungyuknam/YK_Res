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
	if(Out.UpDnAngleCol.AngleEn)
	{
		F_StepMotorRstControl();
		if(Rst_Ok == g_u8SMRstState)
		{
			F_StepMotorSwing(UPDN_UP,UPDN_DN,c_SMSpeedSwing,0,0);
		}
		Out.LeftRightAngleState.AngleEn = 1;
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
		F_RL_StepMotorRstControl();
		if(Rst_Ok == g_u8RL_SMRstState)
		{
			F_RL_StepMotorSwing(C30D_R,C30D_L,c_RL_SMSpeedSwing,0,0);  	//
		}	
		Out.UpDnAngleState.AngleEn = 1;
	}
	else
	{	
		F_RL_StepMotorStop();
		Out.UpDnAngleState.AngleEn = 0;
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


