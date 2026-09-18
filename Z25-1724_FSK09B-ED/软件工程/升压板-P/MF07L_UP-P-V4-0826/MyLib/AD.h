#ifndef _ADC_h_					// Judge the "DisDrv_h" has been defined or not,make sure this file only include once 
#define _ADC_h_					// define "MCU_8513_H" 

				                                                                                         
//冒泡排序求中值
#define AD_BUMBLE_NUM				5			//冒泡数值获取个数
#define AD_MIDDLE_NUM				3			//取数组冒泡的中值
//平均值滤值
#define	AD_ADD_NUM					34			//求和个数(要注意求和的位数)
#define	AVER_GET_RIGHT_BIT			5			//去掉最高值和最低值后求平均值,所要移位的个数。

#define	CHECK_AD_GET_FLAG_OK()		(ADCC0&0x20)	
#define	ClEAR_AD_FLAG()				ADCC0 &=~ 0x20
#define	GET_AD_VALUE()				(u16)(ADCVH<<4)+(ADCVL>>4)   //根据通道不同,通道寄存器也不同




typedef struct{
	u16 ADTemp[AD_BUMBLE_NUM];
	u8 AD_Cnt;
	u16 AD_Max;
	u16 AD_Mini;
	u32 AD_ADD; 					//最多累加16*65536次
	u8 ADAver_Cnt;
	u16 Aver;
}ADCH_Str;


	//AD通道总数
#define     C_CH_NUMBERS              3
	
	//AD通道定义
#define   C_CH_VOL                    6
	
	
	//检测事件定义
#define     C_OFFTIME_EVENT   		   0
#define     C_LBOX_EVENT   			   1
#define     C_RBOX_EVENT   			   2
#define     C_BATTERY_EVENT   		   3
#define     C_POWER_EVENT   		   4

//1/11 * VBAT / VREF (2V) = AD / 4096 => VBAT = 11*AD*VREF/4096
#define    			 _Formula_Pu56K_Dn3K3_PowerCalc1(x)      ((unsigned long)x*22000)/4096  //mv

	
	extern u16 u16AD_ShowData;

	extern u16 xdata g_u16Bat_Vol; //输入电压
	extern ADCH_Str xdata Voltage;


	extern void ADValue_Get(void);
	extern void ADC_Module_Init();
	extern void ADC_Init(uint Channel); //,uchar ADCFreq

	extern u16 xdata g_u16Voltage;
	
#endif

