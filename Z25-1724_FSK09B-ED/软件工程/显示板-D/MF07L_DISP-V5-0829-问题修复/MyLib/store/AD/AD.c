
#include "Include\Include.h"

#ifdef _EPPROM_

unsigned char xdata AD_ErrorCNT;			//报错判断次数计数器
unsigned char xdata AD_Staut;				//AD状态
unsigned char xdata AD_CNT;					//读取AD次数计数器
unsigned long int xdata AD_Sum;					//读取AD次数之和
unsigned int  xdata AD_Value;				//读取AD  c_GETAD_AVERAGE_TIMER次之后的平均值

#define CAP_CH						9		//电容的AD通道

unsigned int ADC_read(unsigned char ch)
{	//read A/D convertor
	ADCCON = 0X80|ch;		//开启ADC，ADC采样频率为2M ,选择Channel位ADC采样口
	if(ch<8)
	{
		ADCCFG0 = 1<<ch;   //设置Channel作为采样口
	}
	else
	{
		ADCCFG1 = 1<<(ch-8);   //设置Channel作为采样口
	}
	ADCCFG2 = 0x03;	   //设置频率/12
	ADCCON |= 0X40;   //开始ADC转换

	while(!(ADCCON & 0x20));	// wait ADC busy
	return ((ADCVH<<2)+(ADCVL>>6));	// read ADC
}

void F_GetCapAD()
{
	u16 xdata CapMinDat = 0xfff;
    u16 xdata CapMaxDat = 0;
	u16 xdata u16temp = 0;
	AD_CNT = c_GETAD_AVERAGE_TIMER;
	AD_Sum = 0;
	
	while(AD_CNT)
	{
		u16temp = ADC_read(CAP_CH);
		AD_Sum += u16temp;
		if(u16temp < CapMinDat)
			CapMinDat = u16temp;
		if(u16temp > CapMaxDat)
			CapMaxDat = u16temp;
		AD_CNT --;
	}
	AD_Sum -= CapMinDat;
	AD_Sum -= CapMaxDat;
	AD_Value = AD_Sum / (c_GETAD_AVERAGE_TIMER - 2);			//获取对应通道的AD值
	
	//转充电
//	ADCCON = 0;
//	ADCCFG0 = 0;
//	ADCCFG1 = 0;
//	
//	P1CON |= BIT4;
//	P1PH |= BIT4;
//	P14 = 1;
}



void CAP_SetInPULL_ON()
{
	//转充电
	ADCCFG0 = 0;
	ADCCFG1 = 0;
	P1PH |= BIT4;
	P1CON |= BIT4;
	P14 = 1;
}


#define	AD_CHN_LC			(8)
#define	AD_CHN_DF			(5)
#define	AD_CHN_DOOR			(4)
#define	AD_CHN_POWERCUT		(1)


#define AD_SECTION_NON			0
#define AD_SECTION_START		1
#define AD_SECTION_SAMPLEING	2
#define AD_SECTION_READFINISH	3
#define AD_SECTION_POWERDECTECT	4

uchar LONG_REG	g_u8ADSection = AD_SECTION_NON;
uint LONG_REG 	PowADValue = 0;



extern void F_FlashSaveData(void);



void StarADConver(void)
{
	//if(fgDetectPower)
	{
		//EA=0;
		//OPINX = 0XC2;
		//OPREG|=0X80;//vref=2.4V
		//EA = 1;
		ADCCON = 0X9f;
	}
	
	//IE |= 0X40; 	   //开启ADC中断
//=======================================================
	ADCCON |= 0X40;   //开始ADC转换
}



void ReadPowerOff(void)
{
	static uchar LONG_REG	AdChangeTime = 0xff;
	//感应
	if(ADCCON & 0X20)
	{
		PowADValue = ADCVH;
		ADCCON &= ~(0X20);  //清中断标志位
		g_u8ADSection = AD_SECTION_NON;
		if(PowADValue <= 105)
		{
			TM1668_SetBright(b1000_0000); //关显示
			g_u8SysTime_Cnt = 0;
			if(AdChangeTime < 255)AdChangeTime ++;
			if(AdChangeTime == 10)   //20
			{
				EA = 0;
				//LED_ON();
				F_FlashSaveData();
				//LED_OFF();
				EA = 1;
			}
		}
		else if(PowADValue > 120)
		{
			AdChangeTime = 0;
			//LED_OFF();
		}
	}
}


void ADProc(void)
{
	if(SysMode == SysMX_FstPOW_ON)
		return;
	if(AD_SECTION_NON == g_u8ADSection)
	{ 

		StarADConver();
		g_u8ADSection = AD_SECTION_POWERDECTECT;
		
	}
	else if(AD_SECTION_POWERDECTECT == g_u8ADSection)
	{
		ReadPowerOff();
	}
	else
	{
		g_u8ADSection = AD_SECTION_NON;
	}
}
#endif







