/*********************************************************
文 件 名 : AdEvent.C
文件说明 : AD事件处理
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/
//头文件
#include "COMMON\AllInclude.h"
#include "AdEvent_C.h"

//变量定义

FILTER8_T xdata LBOX_TEMPER = {0,0,0};
FILTER8_T xdata RBOX_TEMPER = {0,0,0};
AD_Structure xdata TEMPER_RBox = {0};
AD_Structure xdata TEMPER_LBox = {0};
AD_Structure xdata BATTERY = {0};
static AD_Structure xdata POWER = {0};
u16  g_u16MemoryTime = 1500;
bit  g_bMemoryEn = 0;

/*****************************************************
*函数名称：void F_LIMIT_8Bit(s8 *l_ps8LimitData,s8 l_s8LimitMax, s8 l_s8LimitMin)
*函数功能：限值函数
*输入参数：
*输出参数：无
*说明：
*****************************************************/
void F_LIMIT_8Bit(s8 *l_ps8LimitData,s8 l_s8LimitMax, s8 l_s8LimitMin)
{
	if (*l_ps8LimitData > l_s8LimitMax)
	{
		*l_ps8LimitData = l_s8LimitMax;	
	}
	else if (*l_ps8LimitData < l_s8LimitMin)
	{
		*l_ps8LimitData = l_s8LimitMin;
	}
}


/*****************************************************
*函数名称：s8 F_TempCompensation(s8 l_s8SetTemp, s8 *l_ps8Temp)
*函数功能：获取不同设置温度下的补偿值
*输入参数：无
*输出参数：无
*调用周期：无
*说明：	   
*****************************************************/
void F_TempCompensation(s8 *l_s8SetTemp, s8 *l_ps8Temp)	//第一个参数是设置温度   第二个参数是当前实际温度
{
	s8 l_s8Compensation;

	if (g_CarRfgrt.u8TempUnit == C_Unit_Celsius)
	{
		if (l_s8SetTemp == &g_CarRfgrt.s8SetTemp_L)
		{
			if (*l_s8SetTemp >= (-6))																//大于-6度设置温度的温度补偿
			{l_s8Compensation = g_CarRfgrt.s8TempComp_N6_L;}
			else if (*l_s8SetTemp >= (-12))
			{l_s8Compensation = g_CarRfgrt.s8TempComp_N12_L;}
			else
			{l_s8Compensation = g_CarRfgrt.s8TempComp_N13_L;}
		}
		else
		{
			if (*l_s8SetTemp >= (-6))
			{l_s8Compensation = g_CarRfgrt.s8TempComp_N6_R;}
			else if (*l_s8SetTemp >= (-12))
			{l_s8Compensation = g_CarRfgrt.s8TempComp_N12_R;}
			else
			{l_s8Compensation = g_CarRfgrt.s8TempComp_N13_R;}		
		}
	}
	else
	{	
		if (l_s8SetTemp == &g_CarRfgrt.s8SetTemp_L)
		{
			if (*l_s8SetTemp >= (21))
			{l_s8Compensation = g_CarRfgrt.s8TempComp_N6_L;}
			else if (*l_s8SetTemp >= (10))
			{l_s8Compensation = g_CarRfgrt.s8TempComp_N12_L;}
			else
			{l_s8Compensation = g_CarRfgrt.s8TempComp_N13_L;}
		}
		else
		{
			if (*l_s8SetTemp >= (21))
			{l_s8Compensation = g_CarRfgrt.s8TempComp_N6_R;}
			else if (*l_s8SetTemp >= (10))
			{l_s8Compensation = g_CarRfgrt.s8TempComp_N12_R;}
			else
			{l_s8Compensation = g_CarRfgrt.s8TempComp_N13_R;}		
		}			
	}

	//越界判断-----------加上补偿值以后不能小于-127
	if ((l_s8Compensation < 0) && (*l_ps8Temp < 0))
	{
		if ((*l_ps8Temp + l_s8Compensation) >= 0)
		{*l_ps8Temp = (-40);}
		else
		{*l_ps8Temp += l_s8Compensation;}
	}
	else if ((l_s8Compensation > 0) && (*l_ps8Temp > 0))//加上补偿值以后不能大于40
	{
		if ((*l_ps8Temp + l_s8Compensation) < 0)
		{
			if (g_CarRfgrt.u8TempUnit == C_Unit_Celsius)
			{*l_ps8Temp = (40);}
			else
			{*l_ps8Temp = (104);}
		}
		else
		{*l_ps8Temp += l_s8Compensation;}			
	}
	else
	{*l_ps8Temp += l_s8Compensation;}


	//限值
	if (g_CarRfgrt.u8TempUnit == C_Unit_Celsius)
	{F_LIMIT_8Bit(l_ps8Temp,40,(-40));}
	else
	{F_LIMIT_8Bit(l_ps8Temp,104,(-40));}

}


/*****************************************************
*函数名称：void F_PowerVdd_Test(void)
*函数功能：电源VDD检测
*输入参数：无
*输出参数：无
*调用周期：无
*说明：	   上电超2秒，低于4V写FLASH数据
*****************************************************/
void F_PowerVdd_Test(void)
{
	static bit s_bPowerFallFlag = 1;

	if (g_u8RunTime100mS > 20)	//上电超2秒，写数据	
	{
		F_GetAD(C_CH_VDD,&POWER,4090,2050);
		{
			if (POWER.AD_Staut == c_AD_LESS_THAN)
			{
				if (s_bPowerFallFlag == 0)
				{
					EA = 0;

					P0 = 0x00;
					P2 = 0x00;
					P1 = 0x00;	
					P5 = 0x00;
					
					if (g_u8ForbidSaveData)
					{
						F_FactoryDataReset(C_Power_ON,0);
					}

					s_bPowerFallFlag = 1;
					F_REFILL_MemoryTable();
					F_MEMORY_AllWrite();

					EA = 1; 				
				}
			}								   
			else
			{
				if (s_bPowerFallFlag)
				{
					if (POWER.AD_Value > 2300)		  //大于4.5V使能写数据
					{
						s_bPowerFallFlag = 0;
					}
				}
			}
		}		
	}
}




//2024-8-9
static void F_TemperOfQZ_Logic(s8 *l_ps8TempCh)
{
	u16 l_u16TempUpdateRate;
	s8  l_s8CmpTemper,l_s8NowTemper;
	u8  l_u8BiasUnit_10Degree;
	u8  l_u8BiasUnit_5Degree;
	static s8  xdata s_s8LastDispTemper_L = 0;
	static u16 xdata s_u16TempUpdateTime_L = 0;

	static s8  xdata s_s8LastDispTemper_R = 0;
	static u16 xdata s_u16TempUpdateTime_R = 0;

	//单箱2分钟变化，双箱4分钟变化
//	if (g_CarRfgrt.u8BoxEn == C_BOX_ALL_EN)
//	{l_u16TempUpdateRate = 24000;}	
//	else
	{l_u16TempUpdateRate = 12000;}		//2分钟

	//华氏与摄氏偏差不同
	if (g_CarRfgrt.u8TempUnit == C_Unit_Celsius)
	{
		l_u8BiasUnit_10Degree = 10;
		l_u8BiasUnit_5Degree = 5;
	}
	else
	{
		l_u8BiasUnit_10Degree = 18;
		l_u8BiasUnit_5Degree = 9;
	}

	//左箱温度处理
	if (l_ps8TempCh == (&g_CarRfgrt.s8RoomTemp_L))
	{
		l_s8NowTemper = g_CarRfgrt.s8RoomTemp_L;	//g_CarRfgrt.s8RoomTemp_L 当前实际温度值
		
		//上电前两秒直接显示当前温度
		if (g_u8RunTime100mS < 20)  
		{
			g_CarRfgrt.s8DispTemp_L = l_s8NowTemper;
			s_s8LastDispTemper_L = l_s8NowTemper;
			s_u16TempUpdateTime_L = 0;			 
		}
		else
		{ 
			//目标显示温度处理，实际温度大于（设置温度-5）且小于（设置温度+5），目标温度=设置温度。否则目标温度=实际温度。
			if ((l_s8NowTemper <= (g_CarRfgrt.s8SetTemp_L - l_u8BiasUnit_5Degree)) || (l_s8NowTemper >= (g_CarRfgrt.s8SetTemp_L + l_u8BiasUnit_5Degree)))
			{
				l_s8CmpTemper = l_s8NowTemper;		//看看目标温度是设置温度  还是实际温度
			}
			else
			{	
				l_s8CmpTemper = g_CarRfgrt.s8SetTemp_L;
			}

			if (((l_s8NowTemper > (g_CarRfgrt.s8SetTemp_L - l_u8BiasUnit_5Degree)) && (l_s8NowTemper < (g_CarRfgrt.s8SetTemp_L + l_u8BiasUnit_5Degree))) &&
			    ((g_CarRfgrt.s8DispTemp_L > (g_CarRfgrt.s8SetTemp_L - l_u8BiasUnit_5Degree)) && (g_CarRfgrt.s8DispTemp_L < (g_CarRfgrt.s8SetTemp_L + l_u8BiasUnit_5Degree))))
			{
			//双箱4分钟变化1度，单箱2分钟变化1度
			}
			else if ((g_CarRfgrt.s8DispTemp_L >= (l_s8CmpTemper - l_u8BiasUnit_10Degree)) && 
				    (g_CarRfgrt.s8DispTemp_L <= (l_s8CmpTemper + l_u8BiasUnit_10Degree)))	
			{
				l_u16TempUpdateRate = 3000;		  //30秒变化一次
			}
			else
			{
				g_CarRfgrt.s8DispTemp_L = l_s8CmpTemper;
				s_s8LastDispTemper_L = l_s8CmpTemper;
				s_u16TempUpdateTime_L = 0;			
			}			


			if (g_CarRfgrt.u8TempUnit != C_Unit_Celsius)			//华氏度变化的时间会快一倍！！！！！！！！！！！
			{l_u16TempUpdateRate >>= 1;}

			if (s_u16TempUpdateTime_L < 0xFFFF)
			{s_u16TempUpdateTime_L++;}
	
			if (s_u16TempUpdateTime_L >= l_u16TempUpdateRate)
			{
				if ((s_s8LastDispTemper_L != l_s8CmpTemper))
				{
					//显示温度加减
					if (g_CarRfgrt.s8DispTemp_L > l_s8CmpTemper)  	//显示温度和设定温度/当前温度比较
					{g_CarRfgrt.s8DispTemp_L--;}	
					else if (g_CarRfgrt.s8DispTemp_L < l_s8CmpTemper)
					{g_CarRfgrt.s8DispTemp_L++;}
					s_s8LastDispTemper_L = g_CarRfgrt.s8DispTemp_L;
	
					s_u16TempUpdateTime_L = 0;	
				}
			}
		}
	}

	else if (l_ps8TempCh == (&g_CarRfgrt.s8RoomTemp_R))
	{
		l_s8NowTemper = g_CarRfgrt.s8RoomTemp_R;
		
		//上电前两秒直接显示当前温度
		if ((g_u8RunTime100mS < 20))    
		{
			g_CarRfgrt.s8DispTemp_R = l_s8NowTemper;
			s_s8LastDispTemper_R = l_s8NowTemper;
			s_u16TempUpdateTime_R = 0;			 
		}
		else
		{ 
			//目标显示温度处理，实际温度大于（设置温度-5）且小于（设置温度+5），目标温度=设置温度。否则目标温度=实际温度。
			if ((l_s8NowTemper <= (g_CarRfgrt.s8SetTemp_R - l_u8BiasUnit_5Degree)) || (l_s8NowTemper >= (g_CarRfgrt.s8SetTemp_R + l_u8BiasUnit_5Degree)))
			{
				l_s8CmpTemper = l_s8NowTemper;
			}
			else
			{	
				l_s8CmpTemper = g_CarRfgrt.s8SetTemp_R;
			}

			if (((l_s8NowTemper > (g_CarRfgrt.s8SetTemp_R - l_u8BiasUnit_5Degree)) && (l_s8NowTemper < (g_CarRfgrt.s8SetTemp_R + l_u8BiasUnit_5Degree))) &&
			    ((g_CarRfgrt.s8DispTemp_R > (g_CarRfgrt.s8SetTemp_R - l_u8BiasUnit_5Degree)) && (g_CarRfgrt.s8DispTemp_R < (g_CarRfgrt.s8SetTemp_R + l_u8BiasUnit_5Degree))))
			{
			//双箱4分钟变化1度，单箱2分钟变化1度
			}
			else if ((g_CarRfgrt.s8DispTemp_R >= (l_s8CmpTemper - l_u8BiasUnit_10Degree)) && 
				    (g_CarRfgrt.s8DispTemp_R <= (l_s8CmpTemper + l_u8BiasUnit_10Degree)))		
			{
				l_u16TempUpdateRate = 188;
			}
			else
			{
				g_CarRfgrt.s8DispTemp_R = l_s8CmpTemper;
				s_s8LastDispTemper_R = l_s8CmpTemper;
				s_u16TempUpdateTime_R = 0;			
			}			

			if (g_CarRfgrt.u8TempUnit != C_Unit_Celsius)
			{l_u16TempUpdateRate >>= 1;}

			if (s_u16TempUpdateTime_R < 0xFFFF)
			{s_u16TempUpdateTime_R++;}
	
			if (s_u16TempUpdateTime_R >= l_u16TempUpdateRate)
			{
				if ((s_s8LastDispTemper_R != l_s8CmpTemper))
				{
					//显示温度加减
					if (g_CarRfgrt.s8DispTemp_R > l_s8CmpTemper)
					{g_CarRfgrt.s8DispTemp_R--;}
					else if (g_CarRfgrt.s8DispTemp_R < l_s8CmpTemper)
					{g_CarRfgrt.s8DispTemp_R++;}
					s_s8LastDispTemper_R = g_CarRfgrt.s8DispTemp_R;
	
					s_u16TempUpdateTime_R = 0;	
				}
			}
		}
	}
}



/*****************************************************
*函数名称：F_ADC_Handle(void)
*函数功能：ADC数据处理
*输入参数：无
*输出参数：无
*调用周期：10mS
*说明：	   240ms产生一个有效数据
*****************************************************/
void F_ADC_Handle(void)
{	
	static u8 xdata  s_u8AdcSampleEvent = 0;
	static signed char xdata  s_s8TempLeft = 0;
//	static signed char xdata  s_s8TempRight = 0;
//	static u8 xdata  s_u8ChangeTime = 0;
//	static u8 xdata  s_u8DetectDelay = 0;
	u8 l_u8TempFilterTimes;

	F_TemperOfQZ_Logic(&g_CarRfgrt.s8RoomTemp_L);	//决定显示温度到底是往设置温度靠近还是实际温度靠近，然后显示温度的处理
	
	if (g_u8RunTime100mS < 30)
	{l_u8TempFilterTimes = 2;}
	else
	{l_u8TempFilterTimes = 4;}
		
	//左箱温度检测
	if (s_u8AdcSampleEvent == 0)
	{
//		if (g_u8BuzzerTimerDrive)
//		{
//			s_u8DetectDelay = 10;
//		}
//		else if (s_u8DetectDelay)
//		{
//			s_u8DetectDelay--;
//		}
//		else
		{
			if (F_GetAD(C_CH_TEMPER_LBox,&TEMPER_LBox,4030,50))
			{
				s_s8TempLeft = F_Find_16BitADSize(&FAHRENHEIT_B3839_R5K06_PULL5K1,163,TEMPER_LBox.AD_Value) + C_Table_StartTemp;	//获取实际的温度
	
				if (g_CarRfgrt.u8TempUnit == C_Unit_Celsius)	//摄氏度的话转换为华氏度
				{								   
					s_s8TempLeft = F_F2C(s_s8TempLeft);
				}
	
				if (F_FILTER_8Bit(s_s8TempLeft,l_u8TempFilterTimes,&LBOX_TEMPER,30))
				{
					F_TempCompensation(&g_CarRfgrt.s8SetTemp_L,&s_s8TempLeft);		//根据代入的设定温度和当前实际温度值，来算出补偿温度值多少，更新当前的实际温度值
					
					g_CarRfgrt.s8RoomTemp_L = s_s8TempLeft;		//实际温度=实际温度+补偿值（根据设定温度去决定补偿值）
				}
			}
		}
	}


	//电量检测
	else if (s_u8AdcSampleEvent == 1)
	{
		if (F_GetAD(C_CH_BATTERY,&BATTERY,1380,275))            
		{
//			if (g_CarRfgrt.u8PowerSource == C_Power_Adaptor)
//			{
//				if (s_u8ChangeTime)
//				{
//					s_u8ChangeTime--;
//				    g_sPower = F_BAT_PowerCalculate((_Formula_Pu56K_Dn3K3_PowerCalc(BATTERY.AD_Value) + C_DIODE_COMP),0);
//				}
//				else
				{
					g_sPower = F_BAT_PowerCalculate((_Formula_Pu56K_Dn3K3_PowerCalc(BATTERY.AD_Value) + C_DIODE_COMP),l_u8TempFilterTimes);
				}
//			}
//			else
//			{
//				s_u8ChangeTime = 3;
//			}			
		}	
	}
//
//
//
//	//右箱温度检测
//#if Def_NtcNumbers
//
//	else if (s_u8AdcSampleEvent == 2)
//	{
//		if (F_GetAD(C_CH_TEMPER_RBox,&TEMPER_RBox,4030,50))
//		{
//			s_s8TempRight = F_Find_16BitADSize(&FAHRENHEIT_B3839_R5K06_PULL5K1,163,TEMPER_RBox.AD_Value) + C_Table_StartTemp;
//
//			if (g_CarRfgrt.u8TempUnit == C_Unit_Celsius)
//			{								   
//				s_s8TempRight = F_F2C(s_s8TempRight);
//			}
//
//			if (F_FILTER_8Bit(s_s8TempRight,l_u8TempFilterTimes,&RBOX_TEMPER,30))
//			{
//				F_TempCompensation(&g_CarRfgrt.s8SetTemp_R,&s_s8TempRight);
//			
//				g_CarRfgrt.s8RoomTemp_R = s_s8TempRight;		
//			}
//		}
//	}
//
//	if (++s_u8AdcSampleEvent > 2)
//	{
//		s_u8AdcSampleEvent = 0;
//	}
//
//#else
//
	if (++s_u8AdcSampleEvent > 1)
	{
		s_u8AdcSampleEvent = 0;
	}
//
//#endif	
									
}

