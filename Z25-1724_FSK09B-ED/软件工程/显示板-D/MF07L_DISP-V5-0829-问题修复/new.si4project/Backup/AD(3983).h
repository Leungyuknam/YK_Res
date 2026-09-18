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

#define	c_AD_ERROR_CHECK_TIMES			50	//*100ms		//检测到AD超过最大或最小值时,报错的判断次�?
#define	c_AD_RETURN_NORMAL_TIMES		20	//*100ms

#define	SI_ERR_TIMES					50 //*100ms

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

#define	IG_00	803 
#define	IG_01	794 
#define	IG_02	784 
#define	IG_03	774 
#define	IG_04	764 
#define	IG_05	754 
#define	IG_06	744 
#define	IG_07	733 
#define	IG_08	722 
#define	IG_09	711 
#define	IG_10	700 
#define	IG_11	689 
#define	IG_12	678 
#define	IG_13	666 
#define	IG_14	654 
#define	IG_15	643 
#define	IG_16	631 
#define	IG_17	619 
#define	IG_18	607 
#define	IG_19	595 
#define	IG_20	584 
#define	IG_21	572 
#define	IG_22	560 
#define	IG_23	548 
#define	IG_24	536 
#define	IG_25	524 
#define	IG_26	512 
#define	IG_27	500 
#define	IG_28	489 
#define	IG_29	477 
#define	IG_30	466 
#define	IG_31	454 
#define	IG_32	443 
#define	IG_33	432 
#define	IG_34	421 
#define	IG_35	410 
#define	IG_36	400 
#define	IG_37	389 
#define	IG_38	379 
#define	IG_39	369 
#define	IG_40	359 
#define	IG_41	349 
#define	IG_42	339 
#define	IG_43	330 
#define	IG_44	320 
#define	IG_45	311 
#define	IG_46	303 
#define	IG_47	294 
#define	IG_48	285 
#define	IG_49	277 
#define	IG_50	269 
#define	IG_51	261 
#define	IG_52	253 
#define	IG_53	246 
#define	IG_54	239 
#define	IG_55	231 
#define	IG_56	224 
#define	IG_57	218 
#define	IG_58	211 
#define	IG_59	205 
#define	IG_60	199 
#define	IG_61	193 
#define	IG_62	187 
#define	IG_63	181 
#define	IG_64	175 
#define	IG_65	170 
#define	IG_66	165 
#define	IG_67	160 
#define	IG_68	155 
#define	IG_69	150 
#define	IG_70	145 
#define	IG_71	141 
#define	IG_72	137 
#define	IG_73	132 
#define	IG_74	128 
#define	IG_75	124 
#define	IG_76	121 
#define	IG_77	117 
#define	IG_78	113 
#define	IG_79	110 
#define	IG_80	106 
#define	IG_81	103 
#define	IG_82	100 
#define	IG_83	97 
#define	IG_84	94 
#define	IG_85	91 
#define	IG_86	88 
#define	IG_87	86 
#define	IG_88	83 
#define	IG_89	81 
#define	IG_90	78 
#define	IG_91	76 
#define	IG_92	74 
#define	IG_93	71 
#define	IG_94	69 
#define	IG_95	67 
#define	IG_96	65 
#define	IG_97	63 
#define	IG_98	61 
#define	IG_99	60 
#define	IG_100	58 


extern void F_ADCInital(AD_Structure *l_psSt);
extern void DelayUs(unsigned char Time);
extern void F_mAllSenseDeal();

#endif
