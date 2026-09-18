#ifndef _AD_H_
#define _AD_H_

				                                                                                         
//ð����������ֵ
#define AD_BUMBLE_NUM				5			//ð����ֵ��ȡ����
#define AD_MIDDLE_NUM				3			//ȡ����ð�ݵ���ֵ
//ƽ��ֵ��ֵ
#define	AD_ADD_NUM					34			//��͸���(Ҫע����͵�λ��)
#define	AVER_GET_RIGHT_BIT			5			//ȥ�����ֵ�����ֵ����ƽ��ֵ,��Ҫ��λ�ĸ�����

#define	CHECK_AD_GET_FLAG_OK()		1//((ADCCON & 0x20))	
#define	ClEAR_AD_FLAG()				P0IO = 0;//ADCCON&=~(0X20)
#define	GET_AD_VALUE()				16//(u16)(ADCVH<<4)+(ADCVL>>4)   //����ͨ����ͬ,ͨ���Ĵ���Ҳ��ͬ

typedef struct{
	u16 ADTemp[AD_BUMBLE_NUM];
	u8 AD_Cnt;
	u16 AD_Max;
	u16 AD_Mini;
	u32 AD_ADD; 					//����ۼ�16*65536��
	u8 ADAver_Cnt;
	u16 Aver;
}ADCH_Str;

extern U8 g_SampeGetTCnt;


	//ADͨ������
#define     C_CH_NUMBERS              3
	
	//ADͨ������
#define     C_CH_TEMPER_LBox          8  //<<
#define     C_CH_TEMPER_RBox          10 //<<
#define     C_CH_BATTERY              6	 //δʹ��
#define     C_CH_VDD                  0x1F
	
	
	//����¼�����
#define     C_OFFTIME_EVENT   		   0
#define     C_LBOX_EVENT   			   1
#define     C_RBOX_EVENT   			   2
#define     C_BATTERY_EVENT   		   3
#define     C_POWER_EVENT   		   4

	
	extern u16 u16AD_ShowData;


	extern ADCH_Str xdata NTC;


	extern void ADValue_Get(void);
	extern void ADC_Module_Init();
	
	extern u16 xdata g_u16Voltage;


/*****************************************************************/
typedef struct
{
	unsigned char AD_ErrorCNT;			//报错判断次数计数�?
	unsigned char AD_Staut;				//AD状�?
	unsigned char AD_CNT;				//读取AD次数计数�?
	unsigned long int AD_Sum;				//读取AD次数之和
	unsigned int AD_Value;				//读取AD  c_GETAD_AVERAGE_TIMER次之后的平均�?
}AD_Structure;
#define	c_AD_NORMAL						0		//AD值正�?
#define	c_AD_OVERSIZE					1		//AD值超过最大设定�?
#define	c_AD_LESS_THAN					2		//AD值小于最大设定�?
 


	extern AD_Structure g_sTempAD;

/*****************************************************************/
#define	c_GETAD_AVERAGE_TIMER			10		//读c_GETAD_AVERAGE_TIMER次后取一次平�?

#define	c_AD_ERROR_CHECK_TIMES			100	//*85ms		//检测到AD超过最大或最小值时,报错的判断次�?


//单次获取通道ch的AD�?
extern unsigned int ADC_read(unsigned char ch);
//获取通道ch的AD值c_GETAD_AVERAGE_TIMER次后取一次平�?若返�?则表示还没转换换�?若返�?则表示转换完成了
extern unsigned char F_GetAD(unsigned char ch,AD_Structure *AD_Str,unsigned int MaxValue,unsigned MinValue);

#define		FIND_TABLE_ENABLE			1	//使用查表功能

#if	FIND_TABLE_ENABLE
//在p_Table表格中查找ADValue所在的位置
extern unsigned int F_Find_8BitADSize(unsigned char *p_Table,unsigned int Num,unsigned int ADValue);
//extern unsigned int F_Find_16BitADSize(unsigned int *p_Table,unsigned int Num,unsigned int ADValue);

#endif


extern void F_ADCInital(AD_Structure *l_psSt);
extern void DelayUs(unsigned char Time);
extern void F_mAllSenseDeal();

#endif
