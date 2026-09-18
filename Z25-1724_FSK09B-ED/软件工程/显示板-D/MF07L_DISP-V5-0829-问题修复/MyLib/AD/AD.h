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

#define	IG_00	1008 
#define	IG_01	1007 
#define	IG_02	1006 
#define	IG_03	1005 
#define	IG_04	1004 
#define	IG_05	1003 
#define	IG_06	1002 
#define	IG_07	1000 
#define	IG_08	999 
#define	IG_09	998 
#define	IG_10	996 
#define	IG_11	995 
#define	IG_12	994 
#define	IG_13	992 
#define	IG_14	990 
#define	IG_15	989 
#define	IG_16	987 
#define	IG_17	985 
#define	IG_18	983 
#define	IG_19	981 
#define	IG_20	979 
#define	IG_21	977 
#define	IG_22	974 
#define	IG_23	972 
#define	IG_24	969 
#define	IG_25	967 
#define	IG_26	964 
#define	IG_27	961 
#define	IG_28	958 
#define	IG_29	955 
#define	IG_30	952 
#define	IG_31	948 
#define	IG_32	945 
#define	IG_33	941 
#define	IG_34	937 
#define	IG_35	934 
#define	IG_36	930 
#define	IG_37	925 
#define	IG_38	921 
#define	IG_39	917 
#define	IG_40	912 
#define	IG_41	908 
#define	IG_42	903 
#define	IG_43	898 
#define	IG_44	893 
#define	IG_45	888 
#define	IG_46	882 
#define	IG_47	877 
#define	IG_48	871 
#define	IG_49	865 
#define	IG_50	859 
#define	IG_51	853 
#define	IG_52	847 
#define	IG_53	840 
#define	IG_54	834 
#define	IG_55	827 
#define	IG_56	820 
#define	IG_57	813 
#define	IG_58	806 
#define	IG_59	798 
#define	IG_60	791 
#define	IG_61	783 
#define	IG_62	775 
#define	IG_63	767 
#define	IG_64	759 
#define	IG_65	751 
#define	IG_66	743 
#define	IG_67	734 
#define	IG_68	726 
#define	IG_69	717 
#define	IG_70	708 
#define	IG_71	699 
#define	IG_72	690 
#define	IG_73	681 
#define	IG_74	672 
#define	IG_75	663 
#define	IG_76	654 
#define	IG_77	644 
#define	IG_78	635 
#define	IG_79	625 
#define	IG_80	616 
#define	IG_81	606 
#define	IG_82	597 
#define	IG_83	587 
#define	IG_84	578 
#define	IG_85	568 
#define	IG_86	558 
#define	IG_87	549 
#define	IG_88	539 
#define	IG_89	529 
#define	IG_90	520 
#define	IG_91	510 
#define	IG_92	500 
#define	IG_93	491 
#define	IG_94	481 
#define	IG_95	472 
#define	IG_96	463 
#define	IG_97	453 
#define	IG_98	444 
#define	IG_99	435 
#define	IG_100	426 


extern void F_ADCInital(AD_Structure *l_psSt);
extern void DelayUs(unsigned char Time);
extern void F_mAllSenseDeal();

#endif
