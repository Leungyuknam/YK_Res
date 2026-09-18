#define	_Native_FUN_H
#include "Include\Include.h"


u8 code TempGoalTab[] ={  								
	//  OK的									OK 						//OK					OK
	//	L1			L2			L3			L4			L5			L6			L7			L8			L9				L10	
	//	60, 		80, 		110,		120,		140 ,		160,		180,		200,		220,			240    温度
		TE_30,  	TE_43,  	TE_50,  	TE_57,  	TE_64,  	TE_71, 		TE_77, 		TE_83,		TE_89,			TE_95 //TE_88//TE_98 //TE_93
};


u8 code TempHeatTimeTab[] ={ 
	//	L1			L2			L3			L4			L5			L6			L7			L8			L9				L10	
		50,			70,			80,			90,			100,		110,		100,		120,		140,			150																								
};



#if HEAT_MODE	==		HEAT_M_ONE
u8 Drive_Heat_Control(u8 nowTemp,u8 GoalTemp,u8 tempUp,u8 tempDown,u8 maxTemp,u8 minTemp,u8 openTime,u8 closeTime,OVEN_Str *Fryer)
{  
	u8 HeatLevelSet = 0;
	if(nowTemp > (GoalTemp + maxTemp))			//TEMP SO HOT
	{      
		Fryer->BHeatFlag = 0; 		
		//Fryer->DispHotStep = H_HOT_WAIT;		//过温,停加热
	}
	else if(nowTemp <= (GoalTemp - minTemp))		//TEMP SO COLD
	{ 
		//if((Fryer->DecSec > closeTime)) 		
		{
			Fryer->BHeatFlag = 1; 
		}
	}	  
	else if(0 == Fryer->BHeatFlag)	
	{     
		if((Fryer->DecSec > closeTime))	//MINI CLOSE TIME && LOW TEMP (When Temp more hot) 
		{
			if(nowTemp <= (GoalTemp + tempDown))		//在范围内提前加热(要是范围大要延后，注意(GoalTemp - minTemp) < (GoalTemp + tempDown))
			{
				Fryer->BHeatFlag = 1; 
			}
			//else if((Fryer->DecSec > closeTime))
			//{
			//	Fryer->BHeatFlag = 1; 
			//}
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
			//else if((Fryer->IncSec >= openTime))  // + openTime/2)  /&& (nowTemp > GoalTemp)
			//{
			//	Fryer->BHeatFlag = 0; 
				//Fryer->DispHotStep = H_LONG_FORCE_STOP;				//在范围内加热时间过长，先强制停一下，防止过冲。
			//}
		}
	}	

	if(Fryer->BHeatFlag)
	{
		if (GetTimeTick(TIME_1S))
		{
			if(Fryer->HeatSec < 255)
				Fryer->HeatSec ++;
			Fryer->HeatCycSec ++;
		}	

		{
			if(Fryer->HeatCycSec > 2)
				HeatLevelSet = 6;//LeveLMaxTab[Sys.CookLevel];
			if(Fryer->HeatCycSec > 12)
				Fryer->HeatCycSec = 0;
		}
	}
	else
	{
		if(Fryer->DecSec >= 60) 
		{
			Fryer->HeatSec = 0;
			Fryer->BHeatFlag = TRUE;
		}
	}
	return HeatLevelSet;
}



//当前AD是下拉的,温度越高AD值越大
u8 DriveControDeal(u8 NowTempDat)
{
	static u16 xdata HeatLevel = 0;
	u8 xdata GoalTempAD = 0;
	u8 xdata u8temp0 = 0;
	u8 xdata u8tempDiff = 0;
	
	GoalTempAD = TempGoalTab[2]; 
	
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
			sHeat.KeepPowSec = 0;
			sHeat.HeatSec = 0;
			HeatLevel = 5;
		break;
		
		case 1:

			if(u8tempDiff < 35)  //6
			{
				sHeat.HeatStop_Sec = 10;//20;
				sHeat.BHeatFlag = 0;
				sHeat.HeatStep ++;	
			}	
			else if(0 == u8tempDiff)
			{
				sHeat.HeatStep ++;	

			}
			else
			{
				if (GetTimeTick(TIME_1S))
				{
					if(sHeat.KeepPowSec < 65535)
						sHeat.KeepPowSec ++;
					if(sHeat.HeatCycSec < 255)
						sHeat.HeatCycSec ++;
				}	

				
				if(sHeat.KeepPowSec < 2*60 && NowTempDat < 80)
				{
					u8temp0 = 11;
				}
				else if(sHeat.KeepPowSec < 4*60 && NowTempDat < 100)
				{
					u8temp0 = 16;
				}
				else 
				{
					u8temp0 = 22;
				}

				if(sHeat.HeatCycSec > u8temp0)
					sHeat.HeatCycSec = 0;

			}
		break;
		
		case 2:
				u8temp0 = TempHeatTimeTab[2]; //Sys.CookLevel
				u8tempDiff = 30;
				HeatLevel = Drive_Heat_Control(NowTempDat,GoalTempAD,17,3,22,0,u8temp0,u8tempDiff,&sHeat); //12 50  //100 40 13
		
		



		break;
		
		default:
			sHeat.HeatStep = 1;
		break;
	}

	if(HeatLevel >= 3)//LeveLMaxTab[Sys.CookLevel])
		HeatLevel = 3;//LeveLMaxTab[Sys.CookLevel];

	if(sHeat.HeatStop_Sec)
	{
		if(GetTimeTick(TIME_1S))
			sHeat.HeatStop_Sec -- ;
		HeatLevel = 0;	
	}

	
	if(sHeat.BHeatFlag)
	{
		if(GetTimeTick(TIME_1S) ) //&&	(T_G_INC == sHeat.mTempGoal )
		{			
			if(sHeat.IncSec < 255)
				sHeat.IncSec++;
		}
		sHeat.DecSec = 0;

	}
	else
	{
		if(GetTimeTick(TIME_1S) ) //&&  (T_G_DEC == sHeat.mTempGoal )
		{
			if(sHeat.DecSec < 255)
				sHeat.DecSec++;
		}
		sHeat.IncSec = 0;


	}



	return HeatLevel;
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

    }
    if (GetTimeTick(TIME_1S))
    {

    }
}
