/**
*   ************************************************************************************
*								上海芯圣电子股份有限公司
*								    www.holychip.cn
*	************************************************************************************
*	@Examle Version		V1.0.0.0
*	@Demo 	Version		V1.0.0.0
*	@Date				2025.02.12
*	************************************************************************************
*  								       客户服务
*	感谢您使用我们的单片机，若发现错误或对函数的使用存在疑问。请添加上海芯圣电子官方QQ群
*	****************************技术支持群：201030494***********************************
*   *********************************************************A***************************
**/



////30
//#define		UPDN_UP				  (1201-835)		//离复位角度的远端角度	
//#define		UPDN_DN				  (1201-435)		//离复位角度的近端角度
//50
#define		UPDN_UP50				(1201-935)		//离复位角度的远端角度	
#define		UPDN_DN50				(1201-335)		//离复位角度的近端角度
//80
#define		UPDN_UP80				(1201-1135)		//离复位角度的远端角度
#define		UPDN_DN80				(1201-135)		//离复位角度的近端角度


#define UPDN_UP30   850u   // 左端（角度小的那个）
#define UPDN_DN30   1190u  // 右端

#define UPDN_UP60   680u
#define UPDN_DN60   1360u

#define UPDN_UP90   510u
#define UPDN_DN90   1530u

#define UPDN_UP180  0u
#define UPDN_DN180  2040u


#define	ALLOCATE_EXTERN

#include "..\Include\Include.h"
/***************************************************************************************
  * @实现效果	对P00口进行高、低电平转换，LED1不断翻转
***************************************************************************************/
void main()
{
	F_McuInit();
	SystemInit();
	Out.FanSpeed = 1;
	*((u8*)&Out.UpDnAngleCol) = b0100_1100;
	Out.UpDnFarAngleVal = UPDN_UP180;
	Out.UpDnNearAngleVal = UPDN_DN180;
	while(1)
	{
		WDTC |= 0x10;                        //清狗
		
		TimeProc();
		
		if(g_u8BaseTime >= 2)
		{
			g_u8BaseTime -= 2;
			//ADValue_Get();
		}

		if( g_u82MS_Cnt >= 16)
		{
			g_u82MS_Cnt -= 16;
			F_StepMotorDrive(1);
			//F_RL_StepMotorDrive(1);
		}

		
		if(GetTimeTick(TIME_10MS))
		{

		}

		if(GetTimeTick(TIME_100MS))
		{
			F_ModeWork();
		}
		F_FunCtrl();
		F_UartDuplexProc();
	}
}

