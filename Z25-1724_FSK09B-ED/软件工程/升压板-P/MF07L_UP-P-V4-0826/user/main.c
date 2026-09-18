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
// 45° 左右摇头（±22.5°）
#define LR_UP45         1785u
#define LR_DN45         2295u

// 90° 左右摇头（±45°）
#define LR_UP90         1530u
#define LR_DN90         2550u

// 180° 左右摇头（±90°）
#define LR_UP180        1020u
#define LR_DN180        3060u

// 360° 左右摇头（±180°）
#define LR_UP360        0u
#define LR_DN360        4080u

#define	ALLOCATE_EXTERN

#include "..\Include\Include.h"
/***************************************************************************************
  * @实现效果	对P00口进行高、低电平转换，LED1不断翻转
***************************************************************************************/
void main()
{
	F_McuInit();
	SystemInit();
//	*((u8*)&Out.LeftRightAngleCol) = b0100_1100;
//	Out.LeftRightFarAngleVal = LR_UP360;
//	Out.LeftRightNearAngleVal = LR_DN360;
	while(1)
	{
		WDTC |= 0x10;                        //清狗
		
		TimeProc();
		
		if(g_u8BaseTime >= 2)
		{
			g_u8BaseTime -= 2;
			ADValue_Get();
		}

		if( g_u82MS_Cnt >= 16)
		{
			g_u82MS_Cnt -= 16;
			//F_StepMotorDrive(1);  //0824
			F_RL_StepMotorDrive(1); //左右转动
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
		
		F_Uart2DuplexProc();
	}
}

