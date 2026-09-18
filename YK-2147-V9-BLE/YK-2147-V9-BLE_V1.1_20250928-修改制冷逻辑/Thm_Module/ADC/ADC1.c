/*********************************************************
文 件 名 : ADC.C
文件说明 : ADC处理文件
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-6
版 本 号 : V1.0
==========================================================*/
//头文件
#include "COMMON\AllInclude.h"


/*****************************************************
*函数名称: F_ADC_Read(u8 l_u8AdcChannel)
*函数功能: 获取通道AD值
*入口参数：l_u8AdcChannel
*出口参数：AD值
*****************************************************/
u16 F_ADC_Read(u8 l_u8AdcChannel)
{
	OPINX = 0xC2;
	OPREG &= 0x3f;
	
	if ((l_u8AdcChannel == C_CH_VDD) || (l_u8AdcChannel == C_CH_BATTERY))
	{
		OPREG |= 0x40;		//选择参考内部电压2.048V。		
	}		
		
	ADCCON = (0x80 | l_u8AdcChannel);

	if (l_u8AdcChannel < 8)
	{ADCCFG0 = 1<< l_u8AdcChannel;}
	else
	{ADCCFG1 = 1<< (l_u8AdcChannel - 8);}
	
//	if ((l_u8AdcChannel == C_CH_OZONE) || (l_u8AdcChannel == C_CH_DEFROST))
//	{ADCCFG2 = 0x1C;}			//ADC采样时间为32个时钟
//	else
	{ADCCFG2 = 0x14;}			//ADC采样时间为6个时钟
	
	ADCCON |= 0X40;   //开始ADC转换
	while(_BTC(ADCCON,5));
	_BC(ADCCON,5);

	ADCCON |= 0X40;   //开始ADC转换
	while(_BTC(ADCCON,5));
	_BC(ADCCON,5);

	return (((u16)ADCVH<<4)+(ADCVL>>4));
	 
}


/*****************************************************
*函数名称: F_AD_Sample_Mode0(u8 l_u8AdcChannel,u8 l_u8AdcVref,u16 l_u16SetValueMax,u16 l_u16SetValueMin)
*函数功能: 19次取值，滤波后输出
*入口参数：
l_u8AdcChannel：ADC通道
pTable：数据地址
l_u16SetValueMax：AD允许的上限值
l_u16SetValueMin：AD允许的下限值

*出口参数：
返回0，采样未完成，不允许切换通道
返回1，采样完成，允许切换通道
*****************************************************/
unsigned char F_GetAD(unsigned char ch,AD_Structure *AD_Str,unsigned int MaxValue,unsigned MinValue)
{
	AD_Str->AD_Sum += F_ADC_Read(ch);

	if( ++AD_Str->AD_CNT >= c_GETAD_AVERAGE_TIMER )
	{	AD_Str->AD_CNT = 0;
		AD_Str->AD_Value = AD_Str->AD_Sum / c_GETAD_AVERAGE_TIMER;			//获取对应通道的AD值
		AD_Str->AD_Sum = 0;

		if( AD_Str->AD_Staut == c_AD_NORMAL )
		{
			if( AD_Str->AD_Value >= MaxValue )
			{
				if( ++AD_Str->AD_ErrorCNT >= c_AD_ERROR_CHECK_TIMES )
				{	AD_Str->AD_ErrorCNT = 0;
					AD_Str->AD_Staut = c_AD_OVERSIZE;
				}
			}
			else if( AD_Str->AD_Value <= MinValue )
			{
				if( ++AD_Str->AD_ErrorCNT >= c_AD_ERROR_CHECK_TIMES )
				{	AD_Str->AD_ErrorCNT = 0;
					AD_Str->AD_Staut = c_AD_LESS_THAN;
				}
			}
			else
			{	AD_Str->AD_ErrorCNT = 0;
			}
		}
		else if( AD_Str->AD_Staut == c_AD_OVERSIZE )
		{
			if( AD_Str->AD_Value <= MinValue )
			{
				if( ++AD_Str->AD_ErrorCNT >= c_AD_ERROR_CHECK_TIMES )
				{	AD_Str->AD_ErrorCNT = 0;
					AD_Str->AD_Staut = c_AD_LESS_THAN;
				}
			}
			else if( AD_Str->AD_Value < MaxValue )
			{
				if( ++AD_Str->AD_ErrorCNT >= c_AD_ERROR_CHECK_TIMES )
				{	AD_Str->AD_ErrorCNT = 0;
					AD_Str->AD_Staut = c_AD_NORMAL;
				}
			}
			else
			{	AD_Str->AD_ErrorCNT = 0;
			}
		}
		else if( AD_Str->AD_Staut == c_AD_LESS_THAN )
		{
			if( AD_Str->AD_Value >= MaxValue )
			{
				if( ++AD_Str->AD_ErrorCNT >= c_AD_ERROR_CHECK_TIMES )
				{	AD_Str->AD_ErrorCNT = 0;
					AD_Str->AD_Staut = c_AD_OVERSIZE;
				}
			}
			else if( AD_Str->AD_Value > MinValue )
			{
				if( ++AD_Str->AD_ErrorCNT >= c_AD_ERROR_CHECK_TIMES )
				{	AD_Str->AD_ErrorCNT = 0;
					AD_Str->AD_Staut = c_AD_NORMAL;
				}
			}
			else
			{	AD_Str->AD_ErrorCNT = 0;
			}
		}
		return 1;
	}
	return 0;
}


/*****************************************************
*函数名称: F_Find_16BitADSize(unsigned int *p_Table,unsigned int Num,unsigned int ADValue)
*函数功能: 查找AD值在表格中的位置
*入口参数：
p_Table-------->表格首地址
Num------------>表格元素个数
ADValue-------->查找的AD值
*出口参数：
temp----------->元件所在表格的位置
*****************************************************/
u16 F_Find_16BitADSize(u16 *p_Table,u16 Num,u16 ADValue)
{
	u16 r_min,r_max,temp;
	r_min = 0;
	r_max = Num-1;

	if( p_Table[Num-1] >= p_Table[0] )
	{//表格为升序排序
		if( ADValue <= p_Table[0] )
		{	temp = 0;
		}
		else if( ADValue >= p_Table[Num-1] )
		{	temp = Num-1;
		}
		else
		{	do
			{	temp = (r_min+r_max)>>1;
				if( temp == r_min )
				{	break;
				}
				if( ADValue < p_Table[temp] )
				{	r_max = temp;
				}
				else
				{	r_min = temp;
				}
			}while(1);
		}
	}
	else
	{//表格为降序排序
		if( ADValue >= p_Table[0] )
		{	temp = 0;
		}
		else if( ADValue <= p_Table[Num-1] )
		{	temp = Num-1;
		}
		else
		{	do
			{	temp = (r_min+r_max)>>1;
				if( temp == r_min )
				{	break;
				}
				if( ADValue > p_Table[temp] )
				{	r_max = temp;
				}
				else
				{	r_min = temp;
				}
			}while(1);
		}
	}

	return temp;
}



/*****************************************************
*函数名称：u8 F_FILTER_8Bit(u8 l_u8InValue, u16 l_u16FilterTime, FILTER8_T *l_pFilter, u8 l_u8ErroTimes)
*函数功能：8位数据滤波器
*输入参数：l_u8InValue（滤波前的数据）
		   l_u16FilterTime（滤波次数）
		   l_pFilter（滤波后的数据）
*输出参数：无
*说明：
*****************************************************/
u8 F_FILTER_8Bit(u8 l_u8InValue, u16 l_u16FilterTime, FILTER8_T *l_pFilter , u8 l_u8ErroTimes)
{

	if (l_pFilter->u8LastInValue != l_u8InValue)
	{
		l_pFilter->u16FilterCnt = 0;
	}

	l_pFilter->u8LastInValue = l_u8InValue;

	if (++l_pFilter->u16FilterCnt > l_u16FilterTime)
	{
		l_pFilter->u16FilterCnt = 0;
		l_pFilter->u8ErroTimes = 0;
		l_pFilter->u8OutValue = l_u8InValue;
		
		return 1;	
	}
	else
	{
		if (l_u8ErroTimes)
		{
			if (++l_pFilter->u8ErroTimes > (l_u8ErroTimes))
			{
				l_pFilter->u8ErroTimes = 0;
				l_pFilter->u16FilterCnt = 0;
				l_pFilter->u8OutValue = l_u8InValue;
				return 1;	
			}
		}
	}

	return 0;
}

/*****************************************************
*函数名称：u8 F_FILTER_16Bit(u16 l_u16InValue, u16 l_u16FilterTime, FILTER16_T *l_pFilter, u8 l_u8ErroTimes)
*函数功能：8位数据滤波器
*输入参数：l_u8InValue（滤波前的数据）
		   l_u16FilterTime（滤波次数）
		   l_pFilter（滤波后的数据）
*输出参数：无
*说明：
*****************************************************/
u8 F_FILTER_16Bit(u16 l_u16InValue, u16 l_u16FilterTime, FILTER16_T *l_pFilter, u8 l_u8ErroTimes)
{

	if (l_pFilter->u16LastInValue != l_u16InValue)
	{
		l_pFilter->u16FilterCnt = 0;
	}

	l_pFilter->u16LastInValue = l_u16InValue;

	if (++l_pFilter->u16FilterCnt > l_u16FilterTime)
	{
		l_pFilter->u16FilterCnt = 0;
		l_pFilter->u8ErroTimes = 0;
		l_pFilter->u16OutValue = l_u16InValue;
		
		return 1;	
	}
	else
	{
		if (l_u8ErroTimes)
		{
			if (++l_pFilter->u8ErroTimes > (l_u8ErroTimes))
			{
				l_pFilter->u8ErroTimes = 0;
				l_pFilter->u16FilterCnt = 0;
				l_pFilter->u16OutValue = l_u16InValue;
				return 1;	
			}
		}
	}

	return 0;
}

