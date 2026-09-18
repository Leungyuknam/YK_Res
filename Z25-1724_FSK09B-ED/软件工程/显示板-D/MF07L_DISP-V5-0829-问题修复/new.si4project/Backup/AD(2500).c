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

//u8 code AD_CH_Tab[] = {
//	WATER_CUR_CH,
//	NTC_CH_CH,
//};


static u8 xdata GetAD_Mode;
ADCH_Str xdata NTC = {0};

u16 u16AD_ShowData = 0;


unsigned char F_GetAD(unsigned char ch,AD_Structure *AD_Str,unsigned int MaxValue,unsigned int MinValue)
{
	ch = 0;//无用，去掉报警
	//AD_Str->AD_Sum += ADC_read(ch);

	//if( ++AD_Str->AD_CNT >= c_GETAD_AVERAGE_TIMER )
	//{	AD_Str->AD_CNT = 0;
	//	AD_Str->AD_Value = AD_Str->AD_Sum / c_GETAD_AVERAGE_TIMER;			//获取对应通道的AD值
	//	AD_Str->AD_Sum = 0;
		AD_Str->AD_Value = Sys.TempAD;

		if( AD_Str->AD_Staut == c_AD_NORMAL )
		{
			if( AD_Str->AD_Value >= MaxValue )
			{
				if( ++AD_Str->AD_ErrorCNT >= c_AD_ERROR_CHECK_TIMES )
				{	AD_Str->AD_ErrorCNT = 0;
					AD_Str->AD_Staut = c_AD_OVERSIZE;
				}
			}
			else if( AD_Str->AD_Value <= MinValue )
			{
				if( ++AD_Str->AD_ErrorCNT >= c_AD_ERROR_CHECK_TIMES )
				{	AD_Str->AD_ErrorCNT = 0;
					AD_Str->AD_Staut = c_AD_LESS_THAN;
				}
			}
			else
			{	AD_Str->AD_ErrorCNT = 0;
			}
		}
		else if( AD_Str->AD_Staut == c_AD_OVERSIZE )
		{
			if( AD_Str->AD_Value <= MinValue )
			{
				if( ++AD_Str->AD_ErrorCNT >= c_AD_ERROR_CHECK_TIMES )
				{	AD_Str->AD_ErrorCNT = 0;
					AD_Str->AD_Staut = c_AD_LESS_THAN;
				}
			}
			else if( AD_Str->AD_Value < MaxValue )
			{
				if( ++AD_Str->AD_ErrorCNT >= c_AD_RETURN_NORMAL_TIMES )
				{	AD_Str->AD_ErrorCNT = 0;
					AD_Str->AD_Staut = c_AD_NORMAL;
				}
			}
			else
			{	AD_Str->AD_ErrorCNT = 0;
			}
		}
		else if( AD_Str->AD_Staut == c_AD_LESS_THAN )
		{
			if( AD_Str->AD_Value >= MaxValue )
			{
				if( ++AD_Str->AD_ErrorCNT >= c_AD_ERROR_CHECK_TIMES )
				{	AD_Str->AD_ErrorCNT = 0;
					AD_Str->AD_Staut = c_AD_OVERSIZE;
				}
			}
			else if( AD_Str->AD_Value > MinValue )
			{
				if( ++AD_Str->AD_ErrorCNT >= c_AD_RETURN_NORMAL_TIMES )
				{	AD_Str->AD_ErrorCNT = 0;
					AD_Str->AD_Staut = c_AD_NORMAL;
				}
			}
			else
			{	AD_Str->AD_ErrorCNT = 0;
			}
		}
		return 1;
	//}
	//return 0;
}


#if	FIND_TABLE_ENABLE
/*
unsigned int F_Find_8BitADSize(unsigned char *p_Table,unsigned int Num,unsigned int ADValue)
{
	unsigned int r_min,r_max,temp;
	r_min = 0;
	r_max = Num-1;

	if( p_Table[Num-1] >= p_Table[0] )
	{//表格为升序排序
		if( ADValue <= p_Table[0] )
		{	temp = 0;
		}
		else if( ADValue >= p_Table[Num-1] )
		{	temp = Num-1;
		}
		else
		{	do
			{	temp = (r_min+r_max)>>1;
				if( temp == r_min )
				{	break;
				}
				if( ADValue < p_Table[temp] )
				{	r_max = temp;
				}
				else
				{	r_min = temp;
				}
			}while(1);
		}
	}
	else
	{//表格为降序排序
		if( ADValue >= p_Table[0] )
		{	temp = 0;
		}
		else if( ADValue <= p_Table[Num-1] )
		{	temp = Num-1;
		}
		else
		{	do
			{	temp = (r_min+r_max)>>1;
				if( temp == r_min )
				{	break;
				}
				if( ADValue > p_Table[temp] )
				{	r_max = temp;
				}
				else
				{	r_min = temp;
				}
			}while(1);
		}
	}

	return temp;
}
*/
unsigned int F_Find_16BitADSize(unsigned int *p_Table,unsigned int Num,unsigned int ADValue)
{
	unsigned int r_min,r_max,temp;
	r_min = 0;
	r_max = Num-1;

	if( p_Table[Num-1] >= p_Table[0] )
	{//表格为升序排序
		if( ADValue <= p_Table[0] )
		{	temp = 0;
		}
		else if( ADValue >= p_Table[Num-1] )
		{	temp = Num-1;
		}
		else
		{	do
			{	temp = (r_min+r_max)>>1;
				if( temp == r_min )
				{	break;
				}
				if( ADValue < p_Table[temp] )
				{	r_max = temp;
				}
				else
				{	r_min = temp;
				}
			}while(1);
		}
	}
	else
	{//表格为降序排序
		if( ADValue >= p_Table[0] )
		{	temp = 0;
		}
		else if( ADValue <= p_Table[Num-1] )
		{	temp = Num-1;
		}
		else
		{	do
			{	temp = (r_min+r_max)>>1;
				if( temp == r_min )
				{	break;
				}
				if( ADValue > p_Table[temp] )
				{	r_max = temp;
				}
				else
				{	r_min = temp;
				}
			}while(1);
		}
	}

	return temp;
}

#endif

//1S
void F_UpdataADTable()
{	
    if(c_AD_NORMAL == g_sTempAD.AD_Staut)
    {
		//Fuc.Sign.T1Err = 0; 故障不允许屏蔽
		Sys.EnvironTemp = F_Find_16BitADSize(c_TempTable,c_SizeTempTable,g_sTempAD.AD_Value);
    }
    else	
    {
//		if(SysSet.mWingMode == WING_HEAT)
//		{
//		  if(c_AD_LESS_THAN == g_sTempAD.AD_Staut)
//			Fuc.Sign.OpenErr = 1;
//		  else if(c_AD_OVERSIZE == g_sTempAD.AD_Staut)
//			Fuc.Sign.ShortOutErr = 1;
//		}
    }
}


void F_mAllSenseDeal()
{
	static u8 xdata WarningSec = 0;
	static u8 xdata WarningMS = 0;
	if(GetTimeTick(TIME_100MS))
	{
		F_GetAD(2,&g_sTempAD,0x3e0,0x005);
		if(Sys.PowMessage.PowRecSIErr )
		{
			if(Sys.PowSIErrCnt < 255)
				Sys.PowSIErrCnt  ++;	
		}
		else
		{
			Sys.PowSIErrCnt = 0;
		}

		if(Sys.Sign.Err)
		{	
			if(GetTimeTick(TIME_1S) && WarningSec < 255)
				WarningSec++;
			if(Sys.MState != COOK_NONE)
			{
				if(WarningSec < 120 && (0 == Sys.Sign.ErrBuzzDisEn))
				{
					if(++WarningMS >= 3)
					{	
						WarningMS = 0;
						BELL_KEY_WARN();
					}
				}
			}
			if((g_sTempAD.AD_Staut == c_AD_NORMAL) ) //&& !Sys.PowMessage.PowRecSIErr && (ControlDisConnect_Cnt < SI_ERR_TIMES)
			{
				Sys.Sign.Err = 0;
				WorkModeNewSet(SET_STATE_NONE);
			}
		}
		else
		{
			Sys.Sign.ErrBuzzDisEn = 0;
			if((g_sTempAD.AD_Staut != c_AD_NORMAL) ) //|| Sys.PowSIErrCnt >= SI_ERR_TIMES|| ControlDisConnect_Cnt >= SI_ERR_TIMES
			{
				Sys.Sign.Err = TRUE;
			}
		}	
	}
	if (GetTimeTick(TIME_1S))
	{
		F_UpdataADTable();
	}




	
	if(Sys.MState != COOK_NONE)
	{

	}
}



