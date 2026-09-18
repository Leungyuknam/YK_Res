#define ADC_C
#include "..\Include\Include.h"

u16 xdata g_u16Bat_Vol = 0; //电池电压


//#define	WATER_CUR_CH				3			
//#define	NTC_CH_CH					6	

//    
//#define	NTC_KEYAD_CH				1
//#define	CHK_ERR_TIME				200			//确认错误计数次*5ms



//静态常量===============================================================
enum{
	STEP_WATER_CUR,
	STEP_ENV_NTC,
	STEP_VOLTAGE,
	STEP_MAX_AD
};

//u8 code AD_CH_Tab[] = {
//	WATER_CUR_CH,
//	NTC_CH_CH,
//};


static u8 xdata GetAD_Mode;

ADCH_Str xdata Voltage = {0};

u16 u16AD_ShowData = 0;

#define	SC92F7411	0
#define	SC92F7412	1
#define	SC92F7413	2
#define	IC_MODEL 	SC92F7412

/***********ADC采样频率选择*************/
enum ADCFreq {F32=0,F24,F16,F12,F8,F6,F4,F3};

/***********ADC采样口选择*************/
#if (IC_MODEL == SC92F7413)
	#pragma message("------芯片型号是 SC92F7413 ----------------")
enum Channel {AIN0=0,AIN1,AIN2,AIN3,AIN4,AIN5,AIN6,AIN7,AIN8,AIN9,VDD4=31};
#endif

#if (IC_MODEL == SC92F7412)
	#pragma message("------芯片型号是 SC92F7412 ----------------")
enum Channel {AIN2=2,AIN3,AIN4,AIN5,AIN8=8,AIN9,VDD4=31};
#endif

#if (IC_MODEL == SC92F7411)
	#pragma message("------芯片型号是 SC92F7411 ----------------")
enum Channel {AIN2=2,AIN3,AIN4,AIN8=8,AIN9,VDD4=31};
#endif


u16 xdata g_u16Voltage = 0;


/**
  * @说明  	延时函数
  * @参数  	fui_i : 延时时间
  * @返回值 无
  * @注 	  Fcpu = 16MHz,fui_i = 1时,延时时间约为2us
  */
void Delay_2us(unsigned int fui_i)
{
	while(fui_i--);	
}

/*****************************************************
*函数名称：void ADC_Init(uint Channel) //,uchar ADCFreq
*函数功能：ADC初始化
*入口参数：void
*出口参数：void
*****************************************************/
void ADC_Init(uint Channel) //,uchar ADCFreq
{
/************************************ADC初始化*****************************************/
	//P0M3 = P0M3&0xF0|0x03;				//P06设置为模拟输入
	ADCC0 = 0x83;							//打开ADC转换电源,选择内部2V为参考电压
	Delay_2us(10);					      	//延时20us，确保ADC系统稳定
	ADCC1 = Channel;						//选择外部通道
	ADCC2 = 0x4D;						  	//转换结果12位数据，数据右对齐，ADC时钟16分频
	ADCC0 |= 0x40;							//启动ADC转换
}


/********************************************************************************************************
 * @ name   : void AD_AverGet(ADCH_Str *AD_Buf,u16 u16ADVal,u8 u8BumbleNum,u8 u8MiddleNum,u8 u8AverNum,u8 u8AverRightBit)
 * @ msg    :
 * @ param
 * @ return  (*)
 ********************************************************************************************************/
u8 AD_AverGet(ADCH_Str *AD_Buf, u16 u16ADVal, u8 u8BumbleNum, u8 u8MiddleNum, u8 u8AverNum, u8 u8AverRightBit)
{
	u8 i, j, B_AD_Get;
	u16 u16Temp0;
	B_AD_Get = FALSE;
	AD_Buf->ADTemp[AD_Buf->AD_Cnt] = u16ADVal;
	if (++AD_Buf->AD_Cnt >= u8BumbleNum)
	{
		// 冒泡排序，从小到大
		for (i = 0; i < u8BumbleNum - 1; i++)
		{
			for (j = i + 1; j < u8BumbleNum; j++)
			{
				if (AD_Buf->ADTemp[i] > AD_Buf->ADTemp[j])
				{
					u16Temp0 = AD_Buf->ADTemp[i];
					AD_Buf->ADTemp[i] = AD_Buf->ADTemp[j];
					AD_Buf->ADTemp[j] = u16Temp0;
				}
			}
		}
		AD_Buf->AD_Cnt = 0;
		u16Temp0 = AD_Buf->ADTemp[u8MiddleNum];
		B_AD_Get = TRUE;
	}

	if (B_AD_Get)
	{
		B_AD_Get = FALSE;
		if (u16ADVal > AD_Buf->AD_Max)
		{
			AD_Buf->AD_Max = u16Temp0;
		}
		if (u16ADVal < AD_Buf->AD_Mini)
		{
			AD_Buf->AD_Mini = u16Temp0;
		}
		AD_Buf->AD_ADD += u16Temp0;
		if (++AD_Buf->ADAver_Cnt >= u8AverNum)
		{
			// if(AD_ADDNTC_Dat>AD_Dat_Max)
			AD_Buf->AD_ADD -= AD_Buf->AD_Max;
			// if(AD_ADDNTC_Dat>AD_Dat_Mini)
			AD_Buf->AD_ADD -= AD_Buf->AD_Mini;
			// if(GetAD_Mode == GND220_AD)
			//	u16Temp0 = AD_Buf->ADTemp[AD_BUMBLE_NUM - 2];  //地线获取最高值
			// else
			u16Temp0 = AD_Buf->AD_ADD >> u8AverRightBit; // AD_Dat_Mini 当缓存下数据
			AD_Buf->Aver = u16Temp0;
			AD_Buf->AD_ADD = 0;
			AD_Buf->AD_Max = 0;
			AD_Buf->AD_Mini = 0xffff;
			AD_Buf->ADAver_Cnt = 0;
			return 1;
		}
		
		
	}
	
	return 0;
}


// 求各个NTC的AD值 250us
void ADValue_Get(void)
{
	static u8 xdata u16Fail_GetAD_TCnt = 0;
	u8 u8GetAD_CH = 0;
	u16 u16ADTemp = 0;
	// u8 u8Vref_Tool = 0;
	if (CHECK_AD_GET_FLAG_OK() ) 
	{
		u16Fail_GetAD_TCnt = 0;
		ClEAR_AD_FLAG();

		//u16ADTemp = GET_AD_VALUE();
		//u16ADTemp = (u16)(ADCVH<<4);
		//u16ADTemp += (ADCVL>>4);
		u16ADTemp = ADCR;
		u16AD_ShowData = ADCR;

//		switch (GetAD_Mode)
//		{
//		case STEP_WATER_CUR:													// 15*(125us*4)*4 = 30ms  冒泡取值* 一轮时间 * 求平均值数
//			AD_AverGet(&WaterCur,u16ADTemp,AD_BUMBLE_NUM,AD_MIDDLE_NUM,AD_ADD_NUM,AVER_GET_RIGHT_BIT); 							
//			break;
//		
//		case STEP_ENV_NTC:													// 15*(125us*4)*4 = 30ms  冒泡取值* 一轮时间 * 求平均值数
//			AD_AverGet(&NTC,u16ADTemp,AD_BUMBLE_NUM,AD_MIDDLE_NUM,AD_ADD_NUM,AVER_GET_RIGHT_BIT); 							
//			break;

//		case STEP_VOLTAGE:													// 15*(125us*4)*4 = 30ms  冒泡取值* 一轮时间 * 求平均值数
//			if (AD_AverGet(&VOLTAGE,u16ADTemp,AD_BUMBLE_NUM,AD_MIDDLE_NUM,AD_ADD_NUM,AVER_GET_RIGHT_BIT))
//			{g_u16Voltage = (_Formula_Pu56K_Dn3K3_PowerCalc1(u16ADTemp));}				
//			break;		
//		  
//		default:

//			break;
//		}
		if(AD_AverGet(&Voltage,u16ADTemp,AD_BUMBLE_NUM,AD_MIDDLE_NUM,AD_ADD_NUM,AVER_GET_RIGHT_BIT))
		{g_u16Bat_Vol = (_Formula_Pu56K_Dn3K3_PowerCalc1(Voltage.Aver));}	
//		if(++GetAD_Mode >= STEP_MAX_AD)
//			GetAD_Mode = STEP_WATER_CUR;
		//u8GetAD_CH = AD_CH_Tab[GetAD_Mode];
//		if(STEP_WATER_CUR == GetAD_Mode)
//		ADC_Init(AIN2,F8);
//		else if(STEP_ENV_NTC == GetAD_Mode)
//		ADC_Init(AIN6,F8);
//		else if(STEP_VOLTAGE == GetAD_Mode)

//		ADC_Init(AIN0,F8);		
		
		ADCC0 |= 0x40;					//启动ADC转换 			//开始一次ADC转换
	}
	
	
	if (++u16Fail_GetAD_TCnt > 99)
	{
		u16Fail_GetAD_TCnt = 0;		
		ADC_Init(C_CH_VOL); //,F8
	}
}

