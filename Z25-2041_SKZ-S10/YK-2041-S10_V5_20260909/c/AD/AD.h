#ifndef _AD_H_
#define _AD_H_

/******************ADC通道******************/
#define 		AIN0 			0		//< 选用AIN0为ADC的输入 >
#define 		AIN1 			1		//< 选用AIN1为ADC的输入 >
#define 		AIN2 			2		//< 选用AIN2为ADC的输入 >
#define 		AIN3 			3		//< 选用AIN3为ADC的输入 >
#define 		AIN4 			4		//< 选用AIN4为ADC的输入 >
#define 		AIN5 			5		//< 选用AIN5为ADC的输入 >
#define 		AIN6 			6		//< 选用AIN6为ADC的输入 >
#define 		AIN7 			7		//< 选用AIN7为ADC的输入 >
#define 		AIN8 			8		//< 选用AIN8为ADC的输入 >
#define 		AIN9 			9		//< 选用AIN9为ADC的输入 >
#define 		AIN10 			10		//< 选用AIN10为ADC的输入 >
#define 		AIN11 			11		//< 选用AIN11为ADC的输入 >
#define 		AIN12 			12		//< 选用AIN12为ADC的输入 >
#define 		AIN_V1_25 		13		//< 内部1.25V基准 >

typedef struct
{
	unsigned char AD_ErrorCNT; //报错判断次数计数器
	unsigned char AD_Staut;	   //AD状态
	unsigned char AD_CNT;	   //读取AD次数计数器
	unsigned int AD_Sum;	   //读取AD次数之和
	unsigned int AD_Value;	   //读取AD  c_GETAD_AVERAGE_TIMER次之后的平均值
} AD_Structure;
#define c_AD_NORMAL 0	 //AD值正常
#define c_AD_OVERSIZE 1	 //AD值超过最大设定值
#define c_AD_LESS_THAN 2 //AD值小于最大设定值

/*****************************************************************/
#define c_GETAD_AVERAGE_TIMER 64 //读c_GETAD_AVERAGE_TIMER次后取一次平均

#define c_AD_ERROR_CHECK_TIMES 5 //检测到AD超过最大或最小值时,报错的判断次数

//单次获取通道ch的AD值
extern unsigned int ADC_Read(unsigned char Channel);
//获取通道ch的AD值c_GETAD_AVERAGE_TIMER次后取一次平均,若返回0则表示还没转换换成,若返回1则表示转换完成了
extern unsigned char F_GetAD(unsigned char ch, AD_Structure *AD_Str, unsigned int MaxValue, unsigned MinValue);

#define FIND_TABLE_ENABLE 1 //使用查表功能

#if FIND_TABLE_ENABLE
//在p_Table表格中查找ADValue所在的位置

extern unsigned int F_Find_8BitADSize(unsigned char *p_Table, unsigned int Num, unsigned int ADValue);
//extern unsigned int F_Find_16BitADSize(unsigned int *p_Table, unsigned int Num, unsigned int ADValue);

#endif

#endif