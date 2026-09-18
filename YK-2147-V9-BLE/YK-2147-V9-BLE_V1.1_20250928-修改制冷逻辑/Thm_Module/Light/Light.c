/*********************************************************
文 件 名 : Light.C
文件说明 : 灯光处理
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/
//头文件
#include "ALLInclude.h"

//变量定义
static u8 xdata g_u8Light3MinCnt = 0;
//       u8		g_u8LightSetBuff = 0;

/*****************************************************
*函数名称：void F_BUFF_LightSetBuff(LIGHT_EVENT_TypeDef l_u8LightEvent)
*函数功能：灯光缓存处理
*输入参数：灯光事件
*输出参数：无
*说明：
*****************************************************/
void F_BUFF_LightSetBuff(LIGHT_EVENT_TypeDef l_u8LightEvent)
{
	//开灯在任何情况下都能执行
	if (l_u8LightEvent == C_LIGHT_KEYACK_EVENT)
	{
		if (_TEST_BOX_LIGHT)
		{
			_BOX_LIGHT_OFF;
			g_u8Light3MinCnt = 0;	
		}
		else
		{
			_BOX_LIGHT_ON;
			g_u8Light3MinCnt = C_LIGHT_ONTIME_SET;
		}
	}

	//定时时间到，关灯
	else if (l_u8LightEvent == C_LIGHT_TIMEUP_EVENT)
	{
		_BOX_LIGHT_OFF;
	}
}

/*****************************************************
*函数名称：void F_TIME_Light(void)
*函数功能：灯光时间事件处理
*输入参数：无
*输出参数：无
*调用周期：1s
*说明：
*****************************************************/
void F_TIME_Light(void)
{
	if (g_u8Light3MinCnt > 0)
	{
		if (--g_u8Light3MinCnt == 0)
		{
			F_BUFF_LightSetBuff(C_LIGHT_TIMEUP_EVENT);
		}
	}
}

/*****************************************************
*函数名称：void F_DRIVE_Light(void)
*函数功能：灯光驱动
*输入参数：无
*输出参数：无
*调用周期：
*说明：
*****************************************************/
//void F_DRIVE_Light(void)
//{
//	//驱动输出
//	if (g_u8LightSetBuff == C_LIGHT_ON)
//	{
//		_IO_R_LIGHT_ON;
//		_IO_L_LIGHT_ON;		
//	}
//	else
//	{
//		_IO_L_LIGHT_OFF;
//		_IO_R_LIGHT_OFF;
//	}	
//}



