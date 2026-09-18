/*********************************************************
文 件 名 : PowerCalculate.C
文件说明 : 电量计算
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/
//头文件
#include "COMMON\AllInclude.h"
#include "PowerCalculate\PowerCalculate_C.h"

/*****************************************************
*函数名称：void F_POWER_Compensation(void)
*函数功能：电量补偿处理
*输入参数：无
*输出参数：无
*调用周期：无
*说明：
*****************************************************/
u16 F_POWER_Compensation(u16 l_u16CompValue)
{
//	if (_TEST_IO_RPM3000)
//	{
//		return (l_u16CompValue + 36);
//	}
//	else if (_TEST_IO_RPM2000)
//	{
//		return (l_u16CompValue + 32);
//	}
//	else
	{
		return (l_u16CompValue + 16);
	}			
}


/*****************************************************
*函数名称：u8 F_BAT_PowerCalculate(u16 l_u16BattaryAd)
*函数功能：电量事件处理
*调用周期：10ms
*输入参数：l_u16BattaryAd（电池电压的AD值）
*返回值：  s_u8PowerBuff (0-5)
*说明：
*****************************************************/
u8 F_BAT_PowerCalculate(u16 l_u16BattaryAd)
{
	static u8	xdata   s_u8PowerBuff = C_POWER_LEVEL1_LOWEST;     //传递到外部的缓存数据
	static u8	xdata	s_u8BatFilter = 0;
	static u8	xdata	s_u8NowPowerBuff = C_POWER_LEVEL1_LOWEST;  //当前电压缓存
	static u8	xdata	s_u8LastPowerBuff = 0;	                   //上一次的电压缓存

	static u8	xdata	s_u8PowerSource = C_POWER_UNKOWN;          //电池电压：12V或24V
	static u8	xdata	s_u8BatSourceFilter = 0;
	static u8	xdata   s_u8NowSourceBuff = 0;     
	static u8	xdata	s_u8LastSourceBuff = 0;	                   
				   
		   u8	i = 0;
				 
	//补偿整流二极管压降
	l_u16BattaryAd += 15;

	//蓄电池电压源判断
	if (g_u8RunTime100mS > C_POWER_DELAY_CAL_TIME)
	{
		if (l_u16BattaryAd < C_POWER_DIV)
		{
			s_u8NowSourceBuff = C_POWER_12V;
		}
		else
		{
			s_u8NowSourceBuff = C_POWER_24V;
		}
		
		if (s_u8LastSourceBuff != s_u8NowSourceBuff)
		{
			s_u8BatSourceFilter = 0;
		}
		s_u8LastSourceBuff = s_u8NowSourceBuff;
		
		if (++s_u8BatSourceFilter > C_POWER_SEL_TIME)
		{
			s_u8BatSourceFilter = 0;
			s_u8PowerSource = s_u8NowSourceBuff;
			s_u8NowSourceBuff = 0;
			s_u8LastSourceBuff = 0;
		}
	}		

	//电量识别
	if (s_u8PowerSource != C_POWER_UNKOWN)
	{
		//刷新当前电量缓存
		for (i = 0; i < 6; i++)
		{
			if (s_u8NowPowerBuff == i)
			{
				if (l_u16BattaryAd > BAT_INCREASE_TABLE[s_u8PowerSource][i])
				{
					if (++s_u8NowPowerBuff > C_POWER_LEVEL6_HIGHEST)
					{
						s_u8NowPowerBuff = C_POWER_LEVEL6_HIGHEST;
					}
				}
				else if (l_u16BattaryAd < BAT_DECREASE_TABLE[s_u8PowerSource][i])
				{
					if (s_u8NowPowerBuff > 0)
					{
						s_u8NowPowerBuff--;	
					}	
				}
				break;
			}
		}


		//电量等级缓存改变，重新滤波
		if (s_u8LastPowerBuff != s_u8NowPowerBuff)
		{
			s_u8BatFilter = 0;
		}
		s_u8LastPowerBuff = s_u8NowPowerBuff;

		//滤波完成，更新真实电量等级
		if (++s_u8BatFilter > C_POWER_LEVEL_TIME)
		{
			s_u8BatFilter = 0;
			s_u8PowerBuff = s_u8NowPowerBuff;							
		}
	}
	
	return s_u8PowerBuff;	 	
}