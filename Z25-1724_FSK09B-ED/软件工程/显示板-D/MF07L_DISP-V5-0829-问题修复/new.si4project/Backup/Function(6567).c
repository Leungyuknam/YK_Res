#include "Include\Include.h"
#include <math.h>

/**************************************************************************
--------------------------------- 变量定义 --------------------------------
**************************************************************************/
//volatile unsigned char X_REG DangWei;
volatile bit B_TmprStop;
//volatile unsigned char X_REG Delay_Flash,Wait_Flash;
//volatile bit B_Dly_PowerOn,B_DispFlash,B_DispTurn;
//volatile unsigned char X_REG Delay_Sec,Delay_5S;
//volatile bit B_Adjust_SlopeCurr;			//电流斜率调节 
//volatile bit B_DingShi,B_TimeSet;
//volatile unsigned char X_REG Delay_DS; 
//volatile unsigned char X_REG Min_Set,Hour_Set;
//volatile unsigned char X_REG Delay_NoPan,Delay_Error;
//volatile unsigned char X_REG Time_HMI;			//人机界面处理计时 
//volatile bit B_KeyLong;

volatile  unsigned char X_REG Num_LxLowOn;			//连续低功率关个数
volatile  unsigned char X_REG Num_LxLowOff;			//连续低功率关个数

//volatile bit B_Pan_No;							//无锅
//volatile bit B_Zero_No;							//无过零

OVEN_Str xdata sHeat;

typedef enum
{
	HL_0,
	HL_1,
	HL_2,
	HL_3,
	HL_4,
	HL_5,
	HL_6,
	HL_7,
	HL_8,
	HL_9,
	HL_10
}HeatLevel;

#define PROTECT_AD					TE_122			//防止可能温升不起来，使锅过高温

//TE_122   180~154
//TE_80    142~119
//TE_135   233~192   

//TE_128

u8 code TempGoalTab[] ={  								
	//  OK的									OK 						//OK					OK
	//	L1			L2			L3			L4			L5			L6			L7			L8			L9				L10	
	//	60, 		80, 		110,		120,		140 ,		160,		180,		200,		220,			240    温度
		TE_40,  	TE_53,  	TE_66,  	TE_79,  	TE_92,  	TE_105, 	TE_118, 	TE_130,		TE_130,			TE_130 
};


u8 code LeveLMaxTab[] ={  
	//	L1			L2			L3			L4			L5			L6			L7			L8			L9				L10	
	//	60, 		80, 		110,		120,		140 ,		160,		180,		200,		220,			240    温度
		HL_4,  		HL_4, 		HL_4,  		HL_5,  		HL_5,  		HL_5, 		HL_5, 		HL_5,		HL_5,			HL_5   
};


u8 code MenuBotTempOffTab[] = {
//HUOGUO		ZHENGZHU	MENGZHU		JIANKAO		BAOTANG		MANDUN
	200,		200,		200,		250,		200,		200
};

u8 code MenuBotTempOnTab[] = {
//HUOGUO		ZHENGZHU	MENGZHU		JIANKAO		BAOTANG		MANDUN
  190,			190,		190,		245,		190,		190
};



/**************************************************************************
* 从Sys.CookLevel则对应DangOut的值	
**************************************************************************/
//低功率持续加热 20S为一个周期
#define LEVE1_POW		200
#define LEVE2_POW		400
#define LEVE3_POW		600
//持续加热
#define LEVE4_POW		800
#define LEVE5_POW		1000
#define LEVE6_POW		1300
#define LEVE7_POW		1400
//开启抖频
#define LEVE8_POW		1600
#define LEVE9_POW		1800
#define LEVE10_POW		2000


#define DISP_LEVE6_POW	1200
#define DISP_LEVE10_POW	2100




#define DANG_POW			100

#define KEEP_HEAT_OUT		POW_P6			//该档位以上,Sys.CookLevel开启持续加热的功率
#define	DOU_POW_PPGDP_EN	POW_P8			//该档位以上,Sys.CookLevel开启抖频使能加热的功率

code unsigned char LxOnOFF_Tab[5][2]={
	//ON个数    ,	OFF个数
	
	{4		,	24},//{5		,	15},			//800W 中输出 200W
	{8		,	20},//{10		,	10},			//800W 中输出 400W
	{12		,	16},//{15		,	5},				//800W 中输出 600W
	{16		,	12},//{15		,	5},				//800W 中输出 800W
	{20		,	8},//{15		,	5},				//800W 中输出 1000W
};
//1200W

code unsigned char CookLevelPowTab[] = 
{
	LEVE7_POW/C_Mult_Power,		//LEVE4_POW/C_Mult_Power,		// 1档-- 
	LEVE7_POW/C_Mult_Power,		//LEVE4_POW/C_Mult_Power,		// 2档--  
	LEVE7_POW/C_Mult_Power,		//LEVE4_POW/C_Mult_Power,		// 3档--  
	LEVE7_POW/C_Mult_Power,		//LEVE4_POW/C_Mult_Power,		// 4档--  
	LEVE7_POW/C_Mult_Power,		// 5档--     1000W
	
	LEVE6_POW/C_Mult_Power,		// 6档--  
	LEVE7_POW/C_Mult_Power,		// 7档--  
	LEVE8_POW/C_Mult_Power,		// 8档--
	LEVE9_POW/C_Mult_Power,		// 9档--  
	LEVE10_POW/C_Mult_Power,	//10档-- 
};


unsigned char code  CookLevelDispPowTab[] = 
{
	LEVE1_POW/C_Mult_Power,		// 1档-- 
	LEVE2_POW/C_Mult_Power,		// 2档--  
	LEVE3_POW/C_Mult_Power,		// 3档--  
	LEVE4_POW/C_Mult_Power,		// 4档--  
	LEVE5_POW/C_Mult_Power,		// 5档--  
	
	DISP_LEVE6_POW/C_Mult_Power,		// 6档--  
	LEVE7_POW/C_Mult_Power,		// 7档--  
	LEVE8_POW/C_Mult_Power,		// 8档--
	LEVE9_POW/C_Mult_Power,		// 9档--  
	DISP_LEVE10_POW/C_Mult_Power,	//10档-- 


};


//正常模式下
//当前操作变化档位						对输出档位DangOut						对应输出功率 CookLevelDispPowTab(自定义)
//从Sys.CookLevel				查CookLevelGetDangOutTab				//对应显示功率



void TempChangeReset()
{
	u8 xdata i = 0;
	for(i = 0; i < STORE_LENGTH; i++)
		sHeat.TempDat[i] = 0;
	sHeat.KeepPowSec = 0;
	sHeat.HotHeatCnt = 0;
	sHeat.Inter10ms = 0;
	sHeat.Heat500MS = 0;
	sHeat.mForeState = FORE_NON;
	sHeat.State = HEAT_NORMAL;
	sHeat.s8TempSlope  = 0;
	sHeat.HotHeatCnt = 0;

	sHeat.HotHeatAD = 0;
	sHeat.ColdHeatAD = 0;
}

//温度数据分析
void TempChangeAnalyse()
{
	u8 xdata i = 0;
	u8 xdata MinDat = 0xff;
	u8 xdata MinDatNO = 0;
    u8 xdata MaxDat = 0;
	u8 xdata MaxDatN0 = 0;
	u8 u8Buf = 0;
	u8 u8TempON = 0;
	u8 u8TempOFF = 0;
	if(FALSE == Sys.Fg.HeatEn)
	{
		TempChangeReset();
		return;
	}
	if(++ sHeat.Inter10ms >= 50)  //
	{
		sHeat.Inter10ms = 0;

		if(sHeat.Heat500MS < 255)
			sHeat.Heat500MS ++;


		sHeat.mForeState = FORE_NON;	//默认不强制功率输出，以下判断有特殊情况时，强制输出。
		if(HEAT_NORMAL == sHeat.State)
		{
			if(AD_TmprPan > TEM_MID_VAL)
			{
				if(sHeat.Heat500MS >= TEMP_SLOPE_RD_SEC)	
				{
					if(sHeat.s8TempSlope >= 15)
					{
						if(++sHeat.HotHeatCnt >= 6*C_SLOPE1S)
						{
							sHeat.HotHeatCnt = 0;
							sHeat.State = HEAT_PRO_COOK;
						}
					}
					else if(sHeat.s8TempSlope <= 1)
					{
						sHeat.HotHeatCnt = 0;
					}
				}
				sHeat.mForeState = FORE_NON;
			}

			if((sHeat.Heat500MS >= HEAT_RD_SEC) && (sHeat.Heat500MS <= COLD_RD_SEC))
				sHeat.mForeState = FORE_STOP;

			
			if(sHeat.Heat500MS == HEAT_RD_SEC)
				sHeat.HotHeatAD = AD_TmprPan;
			if(sHeat.Heat500MS == COLD_RD_SEC)
			{
				sHeat.ColdHeatAD = AD_TmprPan;
				if(sHeat.HotHeatAD > sHeat.ColdHeatAD)
				{
					u8Buf = sHeat.HotHeatAD - sHeat.ColdHeatAD;
					if(u8Buf >= CHK_DIFF_HOT)
						sHeat.State = HEAT_PRO_COOK;	
				}
			}

			
			u8TempOFF = MenuBotTempOffTab[Sys.mMenu];
			if(AD_TmprPan < (u8TempOFF - 5))
			{
				Sys.TempHigh_Cnt = 0;	
				//if(++Sys.TempLow_Cnt > TEMP_CHK_CNT)		//消抖  
				//{		
				//	Sys.TempLow_Cnt = 0;		
				//	B_TmprStop = 0;		//锅底高温恢复 
				//}		
			}
			else if(AD_TmprPan > u8TempOFF)
			{
				//Sys.TempLow_Cnt = 0;	
				if(++Sys.TempHigh_Cnt  > TEMP_CHK_CNT)		//消抖  
				{		
					Sys.TempHigh_Cnt = 0;				
					sHeat.State = HEAT_PRO_COOK;	
				}
			}	
			else
			{
				Sys.TempHigh_Cnt = 0; 				
			}
		}

		for(i = 0; i < STORE_LENGTH - 1; i++)
			sHeat.TempDat[i] = 	sHeat.TempDat[i+1];
		sHeat.TempDat[STORE_LENGTH - 1] = AD_TmprPan;
		if(sHeat.KeepPowSec < 255)
			sHeat.KeepPowSec ++;

		

		for(i = 0; i < STORE_LENGTH; i++)
		{
			if(MinDat > sHeat.TempDat[i])
			{
				MinDat = sHeat.TempDat[i];
				MinDatNO = i;
			}
			
			if(MaxDat <= sHeat.TempDat[i])
			{
				MaxDat = sHeat.TempDat[i];
				MaxDatN0 = i;
			}
		}
		
		if(MinDatNO < MaxDatN0)
		{
			//if(MaxDat  > sHeat.TempDat[STORE_LENGTH - 1])
			//	sHeat.s8TempSlope = (s8)(sHeat.TempDat[STORE_LENGTH - 1] - MaxDat); //温度递减趋势
			//else
				sHeat.s8TempSlope = (s8)(MaxDat - MinDat);  //温度递增趋势
		}
		else
		{
			//if(MinDat  < sHeat.TempDat[STORE_LENGTH - 1])
			//	sHeat.s8TempSlope = (s8)(sHeat.TempDat[STORE_LENGTH - 1] - MinDat); //温度递增趋势
			//else
				sHeat.s8TempSlope = (s8)( MinDat - MaxDat);  //温度递减趋势
		}
		

		if(sHeat.State == HEAT_PRO_COOK)
		{
			if(sHeat.s8TempSlope <= -7)
			{
				sHeat.State = HEAT_NORMAL;
			}
		}
		
	}
}




//当前AD是下拉的,温度越高AD值越大
u16 DriveControDeal(u8 NowTempDat)
{
	static u16 xdata HeatLevel = 0;
	u8 xdata GoalTempAD = 0;
	u8 xdata u8temp0 = 0;
	u8 xdata u8tempDiff = 0;
	
	GoalTempAD = TempGoalTab[Sys.CookLevel];
	if(GoalTempAD  > NowTempDat)
		u8tempDiff = GoalTempAD - NowTempDat;
	else
		u8tempDiff = 0;
	switch(sHeat.HeatStep)
	{
		case 0:
			sHeat.RecordTempDat = NowTempDat;
			sHeat.HeatStep ++;
			sHeat.IncSec = 0;
			sHeat.DecSec = 0;
			sHeat.HeatStop_Sec = 0;
			sHeat.KeepPowSec = HEAT_KEEP_TIME;
			HeatLevel = 5;
		break;
		
		case 1:

			if(u8tempDiff < 15)
			{
				sHeat.HeatStop_Sec = 20;
				sHeat.HeatStep ++;	

			}	
			else if(0 == u8tempDiff)
			{
				sHeat.HeatStep ++;	

			}
			else
			{
				if(sHeat.KeepPowSec >= HEAT_KEEP_TIME)
				{
					//冷机下的持续加热过久、温差大的处理，强制间歇上升
					u8tempDiff = GoalTempAD - NowTempDat;
					if((sHeat.IncSec > 30)	&& (NowTempDat > sHeat.RecordTempDat))
					{
						sHeat.IncSec = 0;
						sHeat.RecordTempDat = NowTempDat;
						if(HeatLevel)
							HeatLevel --;	
						if(sHeat.s8TempSlope >= 4)
						{
							if(HeatLevel)
								HeatLevel --;
						}
						if(sHeat.s8TempSlope >= 8)
						{
							if(HeatLevel)
								HeatLevel --;
						}
					}
					else
					{
						if(HeatLevel <= 3)
							HeatLevel = 3;	
						if(sHeat.s8TempSlope < 5)
							HeatLevel++;
						if(sHeat.s8TempSlope < 8)
							HeatLevel++;	
						if(sHeat.s8TempSlope >= 13)
						{
							if(HeatLevel)
								HeatLevel--;	
						}
					}
					sHeat.KeepPowSec = 0;
				}	
			}
			if(HeatLevel)
				sHeat.BHeatFlag = 1;
			else
				sHeat.BHeatFlag = 0;
		break;
		
		case 2:
			//3,1,5,5,6,10,&sHeat)
			//3,1,5,4,6,6,&sHeat); 
			//2,0,4,4,6,16,&sHeat); 
			//2,0,4,4,8,16,&sHeat); 较OK
			//2,0,4,4,9,16,&sHeat); 
			Drive_Heat_Control(NowTempDat,GoalTempAD,2,0,4,4,9,16,&sHeat);  
			//Drive_Heat_Control(NowTempDat,GoalTempAD,2,0,3,3,9,14,&sHeat); 
			if(sHeat.BHeatFlag)
				HeatLevel = LeveLMaxTab[Sys.CookLevel];
			else
				HeatLevel = 0;

		break;
		
		default:
			sHeat.HeatStep = 1;
		break;
	}

	if(HeatLevel >= LeveLMaxTab[Sys.CookLevel])
		HeatLevel = LeveLMaxTab[Sys.CookLevel];
	//if(HeatLevel <	LeveLMinTab[Sys.CookLevel])
	//	HeatLevel = LeveLMinTab[Sys.CookLevel];
	if(sHeat.HeatStop_Sec)
	{
		if(GetTimeTick(TIME_1S))
			sHeat.HeatStop_Sec -- ;
		HeatLevel = 0;	
	}

	
	if(0 == HeatLevel)
	{
		if(GetTimeTick(TIME_1S) ) //&&  (T_G_DEC == sHeat.mTempGoal )
		{
			if(sHeat.DecSec < 255)
				sHeat.DecSec++;
		}
		sHeat.IncSec = 0;
	}
	else
	{
		if(GetTimeTick(TIME_1S) ) //&&  (T_G_INC == sHeat.mTempGoal )
		{			if(sHeat.IncSec < 255)
				sHeat.IncSec++;
		}

		sHeat.DecSec = 0;
	}



	return HeatLevel;
}

void DriveMenuReset()
{
	Sys.MenuWorkSec = 0;
	Sys.MenuWorkStep = 0;
}

typedef struct
{
	u8 	FstTemp;			//第一步骤大功率运行,需上升的温度	
	u16	FstReachTempTime;	//第一步骤大功率运行,温度至少持续时间*Sec
	u16 FstPowMaxTime;		//第一步骤大功率运行,允许的最大时间     *Sec
	
	u8 SecPow;				//第二步功率
	u16 SecPowWorkTime;		//第二步功率,持续运行时间     *Sec
	
	u8 ThirdPowN0;			//第三步功率一
	u8 ThirdPowN1;			//第三步功率二
	u16 ThirdPowTime;		//第三步功率运行时间	
	u16 ThirdPowTotalTime;	//第三步总运行时间
	
	u8 FourthPow;			//第四步功率
	u16 FourthPowTime;		//第四步运行总时间大于多少跳下一步
	
	u8 FifthPow;			//第伍步功率
	u16 FifthPowTime;		//第伍步运行总时间大于多少跳下一步

	u8 SixthPow;			//第六步功率
}MenuHeat_Str;


#define  MINT		60

code MenuHeat_Str MenuCookDeal[3] = { 
	{200, 3*MINT, 10*MINT,		POW_P8, 5*MINT,		POW_P4,	POW_P6,	4*MINT,	40*MINT,	POW_P4,	60*MINT,	POW_P3, 90*MINT,		POW_P3},		//MENGZHU 	
	{200, 3*MINT, 10*MINT,		POW_P7, 5*MINT,		POW_P4,	POW_P5,	2*MINT,	45*MINT,	POW_P3,	90*MINT,	POW_P2, 0xffff,			POW_P3}, 		//BAOTANG 	
	{185, 3*MINT, 10*MINT,		POW_P4, 5*MINT,		POW_P4,	POW_P4,	2*MINT,	30*MINT,	POW_P3,	60*MINT,	POW_P3, 180*MINT,		POW_P2},		//MANDUN		
};

u16 DriveMenuDeal()
{
	static u16 xdata TempReachSec = 0;
	static u16 xdata LastStepRecordSec = 0;
	static u8 xdata ReachTempEn = FALSE;
	u8 MenuBuf = 0;
	u8 HeatLevel = 0;
	if(MENGZHU == Sys.mMenu)
		MenuBuf = MENU_MENZHU_COOK_M;
	else if(BAOTANG == Sys.mMenu)
		MenuBuf = MENU_BAOTANG_COOK_M;
	else 
		MenuBuf = MENU_MANDUN_COOK_M;
	if(GetTimeTick(TIME_1S))
	{
		Sys.MenuWorkSec ++;
		TempReachSec  ++;
	}
	switch(Sys.MenuWorkStep)
	{
		case 0:
			TempReachSec = 0;
			Sys.MenuWorkStep++;
		break;
		case 1:
			//全功率运行
			HeatLevel = POW_P10;
			if(MENU_MANDUN_COOK_M == MenuBuf)
				HeatLevel = POW_P6;
			if(AD_TmprPan >= MenuCookDeal[MenuBuf].FstTemp)
				ReachTempEn = TRUE;
			else if(AD_TmprPan <= (MenuCookDeal[MenuBuf].FstTemp - 20))
				ReachTempEn = FALSE;
			
			if(FALSE == ReachTempEn)
				TempReachSec = 0;

			if(TempReachSec >= MenuCookDeal[MenuBuf].FstReachTempTime || Sys.MenuWorkSec >= MenuCookDeal[MenuBuf].FstPowMaxTime)
			{
				Sys.MenuWorkStep ++;	
				LastStepRecordSec = Sys.MenuWorkSec;
			}
		break;
		case 2:
			//降功率运行一会
			HeatLevel = MenuCookDeal[MenuBuf].SecPow;
			if(Sys.MenuWorkSec >= LastStepRecordSec + MenuCookDeal[MenuBuf].SecPowWorkTime)
			{
				Sys.MenuWorkStep ++;	
				LastStepRecordSec = Sys.MenuWorkSec;
				TempReachSec = 0;
			}
		break;
		case 3:
			//两个功率间相互切换
				HeatLevel = MenuCookDeal[MenuBuf].ThirdPowN0;
			if(TempReachSec < MenuCookDeal[MenuBuf].ThirdPowTime/2)
				HeatLevel = MenuCookDeal[MenuBuf].ThirdPowN1;
			else if(TempReachSec >= MenuCookDeal[MenuBuf].ThirdPowTime)
				TempReachSec = 0;
			if(Sys.MenuWorkSec >=MenuCookDeal[MenuBuf].ThirdPowTotalTime)
			{
				Sys.MenuWorkStep ++;	
				LastStepRecordSec = Sys.MenuWorkSec;
				TempReachSec = 0;
			}
		break;
		case 4:
			HeatLevel = MenuCookDeal[MenuBuf].FourthPow;
			if(Sys.MenuWorkSec >= MenuCookDeal[MenuBuf].FourthPowTime)
			{
				Sys.MenuWorkStep ++;	
				LastStepRecordSec = Sys.MenuWorkSec;
				TempReachSec = 0;
			}
		break;
		
		case 5:
			HeatLevel = MenuCookDeal[MenuBuf].FifthPow;
			if(Sys.MenuWorkSec >= MenuCookDeal[MenuBuf].FifthPowTime)
			{
				Sys.MenuWorkStep ++;	
				LastStepRecordSec = Sys.MenuWorkSec;
				TempReachSec = 0;
			}
		break;
		
		case 6:
			//间隙加热
			HeatLevel = MenuCookDeal[MenuBuf].SixthPow;
		break;
		
		default:
			Sys.MenuWorkStep = 4;	//异常处理
		break;
	}
	return HeatLevel;
}




//Fryer->BHeatFlag 变量不可删除
void Drive_Heat_Control(u8 nowTemp,u8 GoalTemp,u8 tempUp,u8 tempDown,u8 maxTemp,u8 minTemp,u8 openTime,u8 closeTime,OVEN_Str *Fryer)
{  
	u16 u16RelayCycTime = 0;
	if(nowTemp > (GoalTemp + maxTemp))			//TEMP SO HOT
	{      
		Fryer->BHeatFlag = 0; 		
		//Fryer->DispHotStep = H_HOT_WAIT;		//过温,停加热
	}
	else if(nowTemp < (GoalTemp - minTemp))		//TEMP SO COLD
	{ 
		if((Fryer->DecSec > closeTime)) 		
		{
			Fryer->BHeatFlag = 1; 
			//Fryer->DispHotStep = H_COLD_START;	//温度过低,停加热时间已到，开始加热						
		}
	}	  
	else if(0 == Fryer->BHeatFlag)	
	{     
		if((Fryer->DecSec > closeTime))	//MINI CLOSE TIME && LOW TEMP (When Temp more hot) 
		{
			if(nowTemp <= (GoalTemp + tempDown))		
			{
				Fryer->BHeatFlag = 1; 
				//Fryer->DispHotStep = H_START_BEFORE;	//在范围内提前加热(要是范围大要延后，注意(GoalTemp - minTemp) < (GoalTemp + tempDown))
			}
		}
	}
	else 
	{   
		if(Fryer->IncSec > openTime)				//MINI OPEN TIME && MAX TEMP (When Temp more cold)  >= opentime
		{
			if(nowTemp > (GoalTemp + tempUp))
			{
				Fryer->BHeatFlag = 0; 
				//Fryer->DispHotStep = H_STOP_BEFORE;				//在范围内提前停止加热,(要是范围大要延后，注意(GoalTemp + maxTemp) > GoalTemp - tempUp)
			}
			else if((Fryer->IncSec >= openTime))  // + openTime/2)  /&& (nowTemp > GoalTemp)
			{
				Fryer->BHeatFlag = 0; 
				//Fryer->DispHotStep = H_LONG_FORCE_STOP;				//在范围内加热时间过长，先强制停一下，防止过冲。
			}
		}
	}	
}


/**************************************************************************
* 函数名称：Check_IgbtNtcHigh 
* 函数功能：IGBT-NTC高温检测
* 入口参数：无
* 出口参数：无 
* 备    注：内部调用，如果高温就降功率 
**************************************************************************/
u8 code DownTab[] = 	{0	,	176	,184}; //{0	,	182	,190};
u8 code	UpTab[] 	=	{188,	196	,255};

#define	COUNT_VAL_CHANGE			50  //*10MS

void Check_IgbtNtcHigh()
{
	static unsigned char CounterL,CounterH;	
	//---------------------------------------------------------------------
	if(Sys.Count_IgbtDown > 2)
		Sys.Count_IgbtDown = 2;	

	if(AD_TmprIgbt >= UpTab[Sys.Count_IgbtDown])
	{
		CounterL = 0;
		if(++CounterH > COUNT_VAL_CHANGE)
		{
			CounterH = 0;	
			if(Sys.Count_IgbtDown < 2)
				Sys.Count_IgbtDown ++;
		}
	}
	else if(AD_TmprIgbt <= DownTab[Sys.Count_IgbtDown])
	{
		CounterH = 0;
		if(++CounterL > COUNT_VAL_CHANGE)
		{
			CounterL = 0;		
			if(Sys.Count_IgbtDown)
				Sys.Count_IgbtDown--;
		}
	}
}


/**************************************************************************
* 函数名称：Set_Work
* 函数功能：工作设置
* 入口参数：无
* 出口参数：无
* 备    注：内部调用
**************************************************************************/
//10ms
void Set_Work()
{ 		
	u16 SetLevel = 0;
	u8 u8buf = 0;

	static u16 xdata LastCookLevel = 0;
	//注意这里的控制赋值非1即0,使状态可重置。Flag1_Work、Flag2_Work的标志位
	{
		/*
		if(B_Adjust_SlopeCurr)
		{
			PowerWork = Tab_DangOut[10];		//工作功率值			
			
			B_Heat_En = 1;  					//加热使能		
			B_Fan_En = 1;  						//风扇使能	
			
			B_HeatStop_NoCheck = 0;
			B_HeatStop_CheckPan = 0;
		}
		else
		*/

		{	
		
			if(JIANKAO == Sys.mMenu)
				B_PanTempReportEn = TRUE;
			else
				B_PanTempReportEn = FALSE;
			TempChangeAnalyse();
			
			#ifdef   _TEST_DEBUG_
			#message "====================进入调试模式======================"
//				Sys.mMenu = JIANKAO;
//				Sys.Fg.MenuSure = TRUE;
				
			#endif 



				
			
			if(((COOK_RUN_PAUSE == Sys.MState)  && Sys.Fg.HeatEn || (SysMode == SysMX_Adjust)) && !Flag1_Error && !Flag2_Error && (Status2_Work & 0x20))  //
			{
				//	//无锅
				//	if(Status1_Work&0x02)
				//	{
				//		L_RESIDUE_SIGN();
				//	}
				//	//有锅
				//	if(Status1_Work&0x04)
				//	{
				//		L_MIN_SIGN();
				//	}

			
				if(Sys.Sign.NoPot)
				{
					if(Status1_Work&0x04) //有锅状态
					{
						//if(++Sys.CheckPotCnt > CHK_POT_TIME)
						{
							Sys.CheckPotCnt = 0;
							Sys.Sign.NoPot = FALSE;
						}
					}
					else
						Sys.CheckPotCnt = 0;	
				}
				else
				{
					if(Status1_Work&0x02) //无锅状态
					{
						if(++Sys.CheckPotCnt > CHK_POT_TIME)
						{
							Sys.CheckPotCnt = 0;
							Sys.Sign.NoPot = TRUE;
						}
					}
					else
						Sys.CheckPotCnt = 0;	
				}
				
				if(SysMode == SysMX_Adjust)
					SetLevel =	POW_P10;
				else if((JIANKAO == Sys.mMenu) || (HEAT_PRO_COOK == sHeat.State))
				{
					if(LastCookLevel != Sys.CookLevel)
					{
						LastCookLevel = Sys.CookLevel;
						sHeat.HeatStep = 0;				//重置煎烤步骤
					}
					SetLevel = DriveControDeal(AD_TmprPan);
					
				}
				else if((HUOGUO == Sys.mMenu) || (ZHENGZHU == Sys.mMenu))
				{
					SetLevel = Sys.CookLevel;
				}
				else
				{
					if(Sys.Fg.KeepWarmEn)
					{
						Sys.CookLevel = 0;		//保温60度
						SetLevel = DriveControDeal(AD_TmprPan);	
					}
					else
						SetLevel = DriveMenuDeal();
				}
				sHeat.ShowLevel = SetLevel + 1;			
				if(SetLevel > POW_P7)
				{
					u8buf = SetLevel - POW_P7;
					if(u8buf <= Sys.Count_IgbtDown)
						SetLevel = POW_P7;
					else if(Sys.Count_IgbtDown == 2)
						SetLevel = POW_P7; //2100W的直接降到1400W。
					else
						SetLevel -= Sys.Count_IgbtDown;
				}
		
				Num_LxLowOn = 0;				//无连续低功率输出方式
				Num_LxLowOff = 0;				//无连续低功率输出方式

				PowerWork = CookLevelPowTab[SetLevel%(POW_P10 + 1)];	//工作功率值		

				if(SetLevel >= DOU_POW_PPGDP_EN)
				{
					B_PPGDP_Can = 1;
					B_HeatStop_CheckPan = 0;		 
					B_HeatStop_NoCheck = 0; 	
					//Cnt_JXHeat = 0;
				}
				else if(SetLevel >= KEEP_HEAT_OUT)
				{
					B_PPGDP_Can = 0;
					B_HeatStop_CheckPan = 0;		 
					B_HeatStop_NoCheck = 0; 	
				}
				else
				{
					if(JIANKAO == Sys.mMenu)
					{
						if(SetLevel)
						{
							B_PPGDP_Can = 0;
							B_HeatStop_CheckPan = 0;		 
							B_HeatStop_NoCheck = 0; 
							Num_LxLowOn = LxOnOFF_Tab[SetLevel][0];
							Num_LxLowOff = LxOnOFF_Tab[SetLevel][1];
						}
						else if(0 == SetLevel)
						{
							B_HeatStop_NoCheck = 1;
						}
					}	
					else
					{
						B_PPGDP_Can = 0;
						B_HeatStop_CheckPan = 0;		 
						B_HeatStop_NoCheck = 0; 
						Num_LxLowOn = LxOnOFF_Tab[SetLevel][0];
						Num_LxLowOff = LxOnOFF_Tab[SetLevel][1];
					}
				}

				if(HEAT_PRO_COOK == sHeat.State)
				{
					//处理要前置
				}
				else
				{
					if(Sys.mMenu != JIANKAO)
					{
						if(B_TmprStop || (FORE_STOP == sHeat.mForeState))
						{  
							B_HeatStop_NoCheck = 1;		//不加热不检锅
						}
						else
						{
							B_HeatStop_NoCheck = 0;
						}
					}
				}
				B_Fan_En = 1;  					//风扇使能
			

				if(B_HeatStop_NoCheck)
				{
					B_Heat_En = 0;  		//避免高压265V时，将未启动加热的电压判电压故障，误判高压故障。
					sHeat.ShowLevel = 0;
				}
				else
					B_Heat_En = 1;  
					
			}
			else
			{
				sHeat.RecordTempDat = AD_TmprPan;
				B_Heat_En = 0;  				//加热关闭		
				B_Fan_En = 0; 					//风扇关闭
				PowerWork = 0;
				B_HeatStop_NoCheck = 0;		
				B_PPGDP_Can = 0;	
				sHeat.HeatStep = 0;				//重置煎烤步骤
				TempChangeReset();
				
			}

			if(!Sys.Fg.HeatEn)
				Sys.Sign.NoPot = FALSE;
			Check_IgbtNtcHigh();
		}
	}
}


//1S
void CookTimeRun()
{
	if((COOK_RUN_PAUSE == Sys.MState) && Sys.Fg.HeatEn)
	{
		if(Sys.Fg.CooktimeEn)
		{
			if((GetDispType() != DISP_SET_COOK_TIME)) 
			{
				if(Sys.CookRunSec)
				{
					Sys.CookRunSec --;				
				}
			}
			
			if(0 == Sys.CookRunSec)
			{
				if((MENGZHU == Sys.mMenu) || (BAOTANG == Sys.mMenu) || (MANDUN == Sys.mMenu))
				{
					Sys.Fg.KeepWarmEn = TRUE;
					Sys.Fg.CooktimeEn = FALSE;
				}
				else
				{
					WorkModeNewSet(SET_NOW_MENU_RESET);
				}
				BeepSet(3,20,80);
			}
		}
		else if(Sys.Fg.KeepWarmEn)
		{
			if(++Sys.CookRunSec >= 4*60*60) 
			{
				//Sys.Fg.KeepWarmEn = FALSE;
				WorkModeNewSet(SET_NOW_MENU_RESET);
				BeepSet(3,20,80);
			}
		}
	}
}

//10ms
void BookTimeRun()
{
	if (GetTimeTick(TIME_1S))
	{
		if((COOK_RUN_PAUSE == Sys.MState) && Sys.Fg.BooktimeEn)
		{
			if((GetDispType() != DISP_SET_BOOK_TIME)) 
			{
				if(Sys.BookRunSec)
				{
					Sys.BookRunSec --;				
				}
			}
			if(Sys.Fg.CooktimeEn && (Sys.BookRunSec >= Sys.CookRunSec))
			{
				if(Sys.BookRunSec == Sys.CookRunSec)
				{
					Sys.Fg.BooktimeEn = FALSE;
					Sys.Fg.HeatEn = TRUE;
				}
			}
			else
			{
				if(0 == Sys.BookRunSec)
				{
					Sys.Fg.BooktimeEn = FALSE;
					Sys.Fg.HeatEn = TRUE;
				}
			}
		}
	}
	if(Sys.Fg.HeatEn)
		Sys.Fg.BooktimeEn = FALSE;
}

//BookTimeRun
void  TimeKeepingRun()
{
	if((COOK_RUN_PAUSE == Sys.MState) && Sys.Fg.TimekeepingEn)
	{
		if(Sys.Fg.HeatEn) 
		{
			if(++ Sys.TKeepSec > MAX_TKEEP_TIME)
			{
				Sys.TKeepSec = 0;		
				Sys.Fg.TimekeepingEn = FALSE;
			}
		}
		else
		{
			Sys.TKeepSec = 0;
		}
		//if(0 == Sys.TKeepSec)
		//{
		//	Sys.Fg.TimekeepingEn = FALSE;
		//	BeepSet(3,50,50);
		//}
	}
}



/********************************************************************************************************
* @ name   : void F_TimeEvent(void)
* @ msg    : 时间变量处理
* @ param   (*)
* @ return  (*)
 ********************************************************************************************************/
void F_TimeEvent(void)
{
    if (GetTimeTick(TIME_100MS))
    {
		BookTimeRun();
    }
    if (GetTimeTick(TIME_1S))
    {
		if(COOK_IDLE == Sys.MState)
		{
			if(++Sys.IDLE_Sec >= 120)
			{
				Sys.IDLE_Sec = 0;
				WorkModeNewSet(SET_STATE_NONE);
				BUZZ();
			}
		}
		CookTimeRun();
		
		TimeKeepingRun();

		if(g_u8PowerON_TCnt > 0)
		{
			g_u8PowerON_TCnt -- ;
		}
    }
}
/********************************************************************************************************
* @ name   : void F_FunCtrl(void)
* @ msg    : 功能处理总函数
* @ param   (*)
* @ return  (*)
 ********************************************************************************************************/
//void F_FunCtrl(void)
//{


//    if (GetTimeTick(TIME_10MS))
//    {

//    }
//    if (GetTimeTick(TIME_1S))
//    {
//		
//    }
//}
