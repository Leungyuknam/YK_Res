/*********************************************************
文 件 名 : ADC.H
文件说明 : ADC头文件
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-6
版 本 号 : V1.0
==========================================================*/
#ifndef _ADC_H_
#define _ADC_H_ 

//ADC结构体定义
typedef struct 
{
	u8	 u8AdStatus;
	u16	 u16AdValue;
}ADC_T;

//出错消抖检测次数（设定值需小于16）
#define     C_ADC_ERRO_CNT            10
#define     C_ADC_NORMAL_CNT          10

//出错状态定义
#define     C_TEST_NORMAL			   0
#define     C_TEST_TOOSMALL			   1
#define     C_TEST_TOOSLARGE		   2

//ADC参考电压
#define     C_ADC_VREF_VDD            0x00    //VDD做参考ADC电压
#define     C_ADC_VREF_2_048V         0x40    //内部2.048V做ADC参考电压
#define     C_ADC_VREF_1_024V         0x80    //内部1.024V做ADC参考电压
  	


typedef struct
{
	unsigned char AD_ErrorCNT;			//报错判断次数计数器
	unsigned char AD_Staut;				//AD状态
	unsigned char AD_CNT;				//读取AD次数计数器
	unsigned int AD_Sum;				//读取AD次数之和
	unsigned int AD_Value;				//读取AD  c_GETAD_AVERAGE_TIMER次之后的平均值
}AD_Structure;

typedef struct
{
	unsigned char u8LastInValue;		//上次滤波前的数值
	unsigned int  u16FilterCnt;		    //滤波计数
	unsigned char u8OutValue;			//滤波后的数值
	unsigned char u8ErroTimes;			//连续出错次数
}FILTER8_T;

typedef struct
{
	unsigned int  u16LastInValue;		//上次滤波前的数值
	unsigned int  u16FilterCnt;		    //滤波计数
	unsigned int  u16OutValue;			//滤波后的数值
	unsigned char u8ErroTimes;			//连续出错次数
}FILTER16_T;

#define	c_AD_NORMAL						0		//AD值正常
#define	c_AD_OVERSIZE					1		//AD值超过最大设定值
#define	c_AD_LESS_THAN					2		//AD值小于最大设定值


/*****************************************************************/
#define	c_GETAD_AVERAGE_TIMER			8		//读c_GETAD_AVERAGE_TIMER次后取一次平均

#define	c_AD_ERROR_CHECK_TIMES			5		//检测到AD超过最大或最小值时,报错的判断次数


////单次获取通道ch的AD值
//extern unsigned int ADC_read(unsigned char ch);
//获取通道ch的AD值c_GETAD_AVERAGE_TIMER次后取一次平均,若返回0则表示还没转换换成,若返回1则表示转换完成了
extern unsigned char F_GetAD(unsigned char ch,AD_Structure *AD_Str,unsigned int MaxValue,unsigned MinValue);



//外部声明
extern u16 F_ADC_Read(u8 l_u8AdcChannel);
extern u8 F_AD_Sample_Mode0(u8 l_u8AdcChannel,ADC_T *pTable,u16 l_u16SetValueMax,u16 l_u16SetValueMin);
extern u16 F_Find_16BitADSize(u16 *p_Table,u16 Num,u16 ADValue);
extern u8 F_FILTER_8Bit(u8 l_u8InValue, u16 l_u16FilterTime, FILTER8_T *l_pFilter, u8 l_u8ErroTimes);
extern u8 F_FILTER_16Bit(u16 l_u16InValue, u16 l_u16FilterTime, FILTER16_T *l_pFilter, u8 l_u8ErroTimes);
              
#endif