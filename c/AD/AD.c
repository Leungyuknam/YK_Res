#include "h\Allinclude.h"

/*******************变量定义*******************/

//=======================================================================================
/**
 * @name:  unsigned int ADC_Read(unsigned char ch)
 * @cycle: 10ms
 * @msg:   读取ch通道ADC
 * @param {unsigned char} Channel	ADC通道
 * @return {unsigned int} 		8bit ADC
 */
unsigned int ADC_Read(unsigned char Channel)
//void ADC_Read(unsigned char Channel)
{
//    if(AIN_VDD_D4 == Channel)
//    {   
//        OPINX = 0xC2;
//        OPREG |= 0x40; //ADC参考电压改为2.048
//    }
//    else
//    {
//        OPINX = 0xC2;
//        OPREG &= ~0xc0;//ADC参考电压改为VDD
//    }
    ADCCON = 0x80|Channel;		//开启ADC,选择Channel位ADC采样口
    ADCCFG2 = B00011100;        //32个ADC采样时钟2.950us
    if(Channel<8)
    {
        ADCCFG0 = 1<<Channel;   //设置Channel作为采样口
    }
    else if(Channel<16)
    {
        ADCCFG1 = 1<<(Channel-8);   //设置Channel作为采样口
    }
    
    ADCCON |= 0x40;
	while (!(ADCCON & 0x20));			//等待转换完成
	ADCCON &= ~0x20;					//EOC标志位清零

	return ADCVH; //8位ADC 高位ADCVH 低位ADCVL
//	ADCCON &= ~0x20;  //清中断标志位
//    EADC = 1;
//    ADCCON |= 0x40;   //开始ADC转换
}
#if FIND_TABLE_ENABLE

/**
 * @name:  unsigned char F_GetAD(unsigned char ch, AD_Structure *AD_Str, unsigned int MaxValue, unsigned MinValue)
 * @cycle: 10ms
 * @msg:   ADC滤波
 * @param {unsigned char} ch		ADC通道
 * @param {AD_Structure} *AD_Str	变量结构体
 * @param {unsigned int} MaxValue	ADC允许最大值
 * @param {unsigned} MinValue		ADC允许最小值
 * @return {1} finish / {0} unfinsh
 */
unsigned char F_GetAD(unsigned char ch, AD_Structure *AD_Str, unsigned int MaxValue, unsigned MinValue)
//unsigned char F_GetAD(unsigned int ADCV, AD_Structure *AD_Str, unsigned int MaxValue, unsigned MinValue)
{
	AD_Str->AD_Sum += ADC_Read(ch);
//	AD_Str->AD_Sum += ADCV;

	if (++AD_Str->AD_CNT >= c_GETAD_AVERAGE_TIMER)
	{
		AD_Str->AD_CNT = 0;
		AD_Str->AD_Value = AD_Str->AD_Sum / c_GETAD_AVERAGE_TIMER; //获取对应通道的AD值
		AD_Str->AD_Sum = 0;

		if (AD_Str->AD_Staut == c_AD_NORMAL)
		{
			if (AD_Str->AD_Value >= MaxValue)
			{
				if (++AD_Str->AD_ErrorCNT >= c_AD_ERROR_CHECK_TIMES)
				{
					AD_Str->AD_ErrorCNT = 0;
					AD_Str->AD_Staut = c_AD_OVERSIZE;
				}
			}
			else if (AD_Str->AD_Value <= MinValue)
			{
				if (++AD_Str->AD_ErrorCNT >= c_AD_ERROR_CHECK_TIMES)
				{
					AD_Str->AD_ErrorCNT = 0;
					AD_Str->AD_Staut = c_AD_LESS_THAN;
				}
			}
			else
			{
				AD_Str->AD_ErrorCNT = 0;
			}
		}
		else if (AD_Str->AD_Staut == c_AD_OVERSIZE)
		{
			if (AD_Str->AD_Value <= MinValue)
			{
				if (++AD_Str->AD_ErrorCNT >= c_AD_ERROR_CHECK_TIMES)
				{
					AD_Str->AD_ErrorCNT = 0;
					AD_Str->AD_Staut = c_AD_LESS_THAN;
				}
			}
			else if (AD_Str->AD_Value < MaxValue)
			{
				if (++AD_Str->AD_ErrorCNT >= c_AD_ERROR_CHECK_TIMES)
				{
					AD_Str->AD_ErrorCNT = 0;
					AD_Str->AD_Staut = c_AD_NORMAL;
				}
			}
			else
			{
				AD_Str->AD_ErrorCNT = 0;
			}
		}
		else if (AD_Str->AD_Staut == c_AD_LESS_THAN)
		{
			if (AD_Str->AD_Value >= MaxValue)
			{
				if (++AD_Str->AD_ErrorCNT >= c_AD_ERROR_CHECK_TIMES)
				{
					AD_Str->AD_ErrorCNT = 0;
					AD_Str->AD_Staut = c_AD_OVERSIZE;
				}
			}
			else if (AD_Str->AD_Value > MinValue)
			{
				if (++AD_Str->AD_ErrorCNT >= c_AD_ERROR_CHECK_TIMES)
				{
					AD_Str->AD_ErrorCNT = 0;
					AD_Str->AD_Staut = c_AD_NORMAL;
				}
			}
			else
			{
				AD_Str->AD_ErrorCNT = 0;
			}
		}
		return 1;
	}
	return 0;
}

unsigned int F_Find_8BitADSize(unsigned char *p_Table, unsigned int Num, unsigned int ADValue)
{
	unsigned int r_min, r_max, temp;
	r_min = 0;
	r_max = Num - 1;

	if (p_Table[Num - 1] >= p_Table[0])
	{ //表格为升序排序
		if (ADValue <= p_Table[0])
		{
			temp = 0;
		}
		else if (ADValue >= p_Table[Num - 1])
		{
			temp = Num - 1;
		}
		else
		{
			do
			{
				temp = (r_min + r_max) >> 1;
				if (temp == r_min)
				{
					break;
				}
				if (ADValue < p_Table[temp])
				{
					r_max = temp;
				}
				else
				{
					r_min = temp;
				}
			} while (1);
		}
	}
	else
	{ //表格为降序排序
		if (ADValue >= p_Table[0])
		{
			temp = 0;
		}
		else if (ADValue <= p_Table[Num - 1])
		{
			temp = Num - 1;
		}
		else
		{
			do
			{
				temp = (r_min + r_max) >> 1;
				if (temp == r_min)
				{
					break;
				}
				if (ADValue > p_Table[temp])
				{
					r_max = temp;
				}
				else
				{
					r_min = temp;
				}
			} while (1);
		}
	}

	return temp;
}

/*unsigned int F_Find_16BitADSize(unsigned int *p_Table, unsigned int Num, unsigned int ADValue)
{
	unsigned int r_min, r_max, temp;
	r_min = 0;
	r_max = Num - 1;

	if (p_Table[Num - 1] >= p_Table[0])
	{ //表格为升序排序
		if (ADValue <= p_Table[0])
		{
			temp = 0;
		}
		else if (ADValue >= p_Table[Num - 1])
		{
			temp = Num - 1;
		}
		else
		{
			do
			{
				temp = (r_min + r_max) >> 1;
				if (temp == r_min)
				{
					break;
				}
				if (ADValue < p_Table[temp])
				{
					r_max = temp;
				}
				else
				{
					r_min = temp;
				}
			} while (1);
		}
	}
	else
	{ //表格为降序排序
		if (ADValue >= p_Table[0])
		{
			temp = 0;
		}
		else if (ADValue <= p_Table[Num - 1])
		{
			temp = Num - 1;
		}
		else
		{
			do
			{
				temp = (r_min + r_max) >> 1;
				if (temp == r_min)
				{
					break;
				}
				if (ADValue > p_Table[temp])
				{
					r_max = temp;
				}
				else
				{
					r_min = temp;
				}
			} while (1);
		}
	}

	return temp;
}*/

#endif