#include "Include\Include.h"
//#include "ADC.h"  

U8 g_SampeGetTCnt = 0;
AD_Structure g_sTempAD = {0};


code u16	c_TempTable[] = 
{//5K B3470 5.1K下拉
IG_00	,
IG_01	,
IG_02	,
IG_03	,
IG_04	,
IG_05	,
IG_06	,
IG_07	,
IG_08	,
IG_09	,
IG_10	,
IG_11	,
IG_12	,
IG_13	,
IG_14	,
IG_15	,
IG_16	,
IG_17	,
IG_18	,
IG_19	,
IG_20	,
IG_21	,
IG_22	,
IG_23	,
IG_24	,
IG_25	,
IG_26	,
IG_27	,
IG_28	,
IG_29	,
IG_30	,
IG_31	,
IG_32	,
IG_33	,
IG_34	,
IG_35	,
IG_36	,
IG_37	,
IG_38	,
IG_39	,
IG_40	,
IG_41	,
IG_42	,
IG_43	,
IG_44	,
IG_45	,
IG_46	,
IG_47	,
IG_48	,
IG_49	,
IG_50	,
IG_51	,
IG_52	,
IG_53	,
IG_54	,
IG_55	,
IG_56	,
IG_57	,
IG_58	,
IG_59	,
IG_60	,
IG_61	,
IG_62	,
IG_63	,
IG_64	,
IG_65	,
IG_66	,
IG_67	,
IG_68	,
IG_69	,
IG_70	,
IG_71	,
IG_72	,
IG_73	,
IG_74	,
IG_75	,
IG_76	,
IG_77	,
IG_78	,
IG_79	,
IG_80	,
IG_81	,
IG_82	,
IG_83	,
IG_84	,
IG_85	,
IG_86	,
IG_87	,
IG_88	,
IG_89	,
IG_90	,
IG_91	,
IG_92	,
IG_93	,
IG_94	,
IG_95	,
IG_96	,
IG_97	,
IG_98	,
IG_99	,
IG_100	,
};
#define c_SizeTempTable (sizeof(c_TempTable)/sizeof(c_TempTable[0]))


#define	WATER_CUR_CH				3			
#define	NTC_CH_CH					6	

    
#define	NTC_KEYAD_CH				1
#define	CHK_ERR_TIME				200			//确认错误计数次*5ms



//静态常量===============================================================
enum{
	STEP_NTC,
	STEP_MAX_AD
};





