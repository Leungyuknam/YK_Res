#include "Include\Include.h"
//#include "ADC.h"  

U8 g_SampeGetTCnt = 0;
AD_Structure g_sTempAD = {0};


code U8	c_TempTable[] = 
{//5K B3470 5.1K下拉
68, //0
70, //1
72, //2 
74, //3
77, //4
79, //5
82, //6
84, //7
86, //8
89, //9
91, //10
94, //11
96, //12
99, //13
101,//14
104,//15
107,//16 
109,//17
112,//18 
114,//19 
117,//20 
122,//21 
122,//22 
124,//23 
127,//24 
129,//25 
132,//26 
134,//27 
137,//28 
139,//29 
141,//30 
144,//31 
146,//32 
148,//33 
151,//34 
153,//35 
155,//36 
157,//37 
160,//38 
162,//39 
164,//40 
166,//41 
168,//42 
170,//43 
172,//44 
174,//45 
176,//46 
178,//47 
179,//48 
181,//49 
183,//50 
185,//51 
186,//52 
188,//53 
190,//54 
191,//55 
193,//56 
194,//57 
196,//58 
197,//59 
199//60
};
#define c_SizeTempTable (sizeof(c_TempTable)/sizeof(c_TempTable[0]))






#define NTC_AIN()  				\
	{                        	\
		P0IO &= ~0x80; 			\
		P0IO  &= ~0x80;      	\
	}

	
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
		AD_Str->AD_Value = NTC.Aver;

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
				if( ++AD_Str->AD_ErrorCNT >= c_AD_ERROR_CHECK_TIMES )
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
				if( ++AD_Str->AD_ErrorCNT >= c_AD_ERROR_CHECK_TIMES )
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
/*
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
*/
#endif

//1S
void F_UpdataADTable()
{	
    if(c_AD_NORMAL == g_sTempAD.AD_Staut)
    {
		//Fuc.Sign.T1Err = 0; 故障不允许屏蔽
		Sys.EnvironTemp = F_Find_8BitADSize(c_TempTable,c_SizeTempTable,g_sTempAD.AD_Value);
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
	if(GetTimeTick(TIME_10MS))
		F_GetAD(2,&g_sTempAD,250,5);
	if (GetTimeTick(TIME_1S))
	{
		F_UpdataADTable();
	}
}



