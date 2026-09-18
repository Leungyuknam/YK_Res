#define	_Native_FUN_H
#include "Include\Include.h"

static U16 s_u161sTime = 0;

//35度   147.3343	247  //256-247
//45度 	96.2750		243
//50度 	78.4837		240
//55度 	64.3204 	237



u16 code TempGoalTab[] ={  								
	//  OK的												OK 						//OK				
	//	L1						L2						L3						L4							
	//	40, 					50, 					60,						70,		
		(0x3ff-IG_31),  		(0x3ff-IG_39),			(0x3ff-IG_44),	 		(0x3ff-IG_52)
};//

																				
u16 code TempRealGoalTab[] ={								
	//	OK的												OK						//OK				
	//	L1						L2						L3						L4							
	//	40, 					50, 					60, 					70, 	
		(0x3ff-IG_34),			(0x3ff-IG_41),			(0x3ff-IG_50),			(0x3ff-IG_55)
};

																				//50
						
u8 code TempHeatTimeTab[] ={ 
	//	L1						L2						L3						L4			
		10,						13,						16,						20,																									
};

u8 code TempHeatCloseTimeTab[] ={ 
	//	L1						L2						L3						L4			
		10,						20,						20,						20,																									
};

u16 code PreHeatTimeTab[] ={ 
	//	L1						L2						L3						L4			
		1*60+30,				2*60+30,				4*60,					5*60,																									
};


#if HEAT_MODE	==		HEAT_M_ONE
u8 Drive_Heat_Control(u16 nowTemp,u16 GoalTemp,u8 tempUp,u8 tempDown,u8 maxTemp,u8 minTemp,u8 openTime,u8 closeTime,OVEN_Str *Fryer)
{  
	u8 HeatLevelSet = 0;
	if(nowTemp > (GoalTemp + maxTemp))			//TEMP SO HOT
	{      
		if(Fryer->BHeatFlag)
			Fryer->CycLoopCnt++;
		Fryer->BHeatFlag = 0; 		
		Fryer->DispHotStep = H_HOT_WAIT;		//过温,停加热
	}
	else if(nowTemp <= (GoalTemp - minTemp))		//TEMP SO COLD
	{ 
		//if((Fryer->DecSec > closeTime)) 		
		{
			if(0 == Fryer->BHeatFlag)
				Fryer->CycLoopCnt++;
			Fryer->BHeatFlag = 1; 
			Fryer->DispHotStep = H_COLD_START;		//过温,停加热
		}
	}	  
	else if(0 == Fryer->BHeatFlag)	
	{     
		if((Fryer->DecSec > closeTime))	//MINI CLOSE TIME && LOW TEMP (When Temp more hot) 
		{
			if(nowTemp <= (GoalTemp + tempDown))		//在范围内提前加热(要是范围大要延后，注意(GoalTemp - minTemp) < (GoalTemp + tempDown))
			{
				if(0 == Fryer->BHeatFlag)
					Fryer->CycLoopCnt++;

				Fryer->BHeatFlag = 1; 
				Fryer->DispHotStep = H_START_BEFORE;	
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
				if(1 == Fryer->BHeatFlag)
					Fryer->CycLoopCnt++;

				Fryer->BHeatFlag = 0; 
				Fryer->DispHotStep = H_STOP_BEFORE;				//在范围内提前停止加热,(要是范围大要延后，注意(GoalTemp + maxTemp) > GoalTemp - tempUp)
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
		HeatLevelSet = 1;
	}
	else
	{
		HeatLevelSet = 0;
	}
	return HeatLevelSet;
}



//当前AD是下拉的,温度越高AD值越大
u16 DriveControDeal(u16 NowTempDat)
{
	static u16 xdata HeatLevel = 0;
	u16 xdata GoalTempAD = 0;
	u16 xdata u8temp0 = 0;
	u16 xdata u8tempDiff = 0;
	u16 xdata u16Buf = 0;


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
			sHeat.CycLoopCnt = 0;
			HeatLevel = 5;
			
		break;
		
		case 1:
			GoalTempAD = TempGoalTab[Sys.mTempLevel];	
			
			if(GoalTempAD  > NowTempDat)
				u8tempDiff = GoalTempAD - NowTempDat;
			else
				u8tempDiff = 0;


		
			if(u8tempDiff < 6)  
			{
				if(TEMP_70C == Sys.mTempLevel)
					sHeat.HeatStop_Sec = 50;//10;
				else
					sHeat.HeatStop_Sec = 30;
				sHeat.BHeatFlag = 0;
				sHeat.HeatStep ++;	
				BUZZ_PROHEAT_OK();
				sHeat.DispHotStep = H_PRE_WAIT;
			}	
			else if(0 == u8tempDiff)
			{
				sHeat.HeatStep ++;	
				BUZZ_PROHEAT_OK();
				sHeat.DispHotStep = H_PRE_SECOND_TWO;
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

				u16Buf = PreHeatTimeTab[Sys.mTempLevel]; 
				if(sHeat.KeepPowSec < u16Buf && u8tempDiff > 30)
				{
					u8temp0 = 0;
				}
				else if(sHeat.KeepPowSec < (u16Buf + 2*60) && u8tempDiff <= 30)
				{
					u8temp0 = 10;
				}
				else if(sHeat.KeepPowSec < (u16Buf + 3*60) && u8tempDiff < 20)
				{
					u8temp0 = 6;
				}
				else if(sHeat.KeepPowSec < (u16Buf + 4*60) && u8tempDiff < 10)
				{
					u8temp0 = 4;
				}
				else if(sHeat.KeepPowSec < (u16Buf + 3*60) && u8tempDiff <= 60)
				{
					u8temp0 = 10;
					//对于冷机预热长时间升温慢的处理，保证有一段时间缓加热
				}
				else 
				{
					u8temp0 = 0;
				}

				if(sHeat.HeatCycSec > 20)
					sHeat.HeatCycSec = 0;

				if(sHeat.HeatCycSec < u8temp0)
					HeatLevel = 0;
				else
					HeatLevel = 1;	



				
				//HeatLevel = 1;
				sHeat.DispHotStep = H_PRE_FST;
			}
		break;
		
		case 2:

				GoalTempAD = TempGoalTab[Sys.mTempLevel]; 
				
				if(GoalTempAD  > NowTempDat)
					u8tempDiff = GoalTempAD - NowTempDat;
				else
					u8tempDiff = 0;


		
				u8temp0 = TempHeatTimeTab[Sys.mTempLevel]; //Sys.CookLevel
				u8tempDiff = TempHeatCloseTimeTab[Sys.mTempLevel];
				HeatLevel = Drive_Heat_Control(NowTempDat,GoalTempAD,6,3,8,0,u8temp0,u8tempDiff,&sHeat);
				//HeatLevel = Drive_Heat_Control(NowTempDat,GoalTempAD,6,3,8,0,u8temp0,u8tempDiff,&sHeat);
		break;
		
		default:
			sHeat.HeatStep = 1;
		break;
	}

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


/**************************************************
*函数名称：void    	 TimeCountFrq(void) 
*函数功能：定时控制
*调用时间：125us
*入口参数：void
*出口参数：void  
**************************************************/
void TimeCountFrq()
{
	if(s_u161sTime < 65535)
		s_u161sTime ++;
	#ifdef _DEBUG_IO_
	#warning "-----------屏蔽功能，调用IO测试用-----------"
	if(s_u161sTime == 8000)
	{
		PIN1 ^= 1;
		PIN2 ^= 1;
	}
	#endif

}


void F_TimeCount()
{
#ifdef _FAST_TEST_
	#warning "-----当前定时开机、关机为缩时模式------"
	#define	TIME_SECOND		80		//10ms
#else
	#define	TIME_SECOND		8000
#endif
	if(s_u161sTime >= TIME_SECOND)
	{
		s_u161sTime -= TIME_SECOND;
		if(Sys.Time)
		{
			if(++Sys.u16TimeContDown >= 3600)
			{
				Sys.u16TimeContDown = 0;
				Sys.Time --;
				if(!Sys.Time)
				{

				}
			}
		}
		else
		{	
            Sys.u16TimeContDown = 0;
        }
	}
	#ifdef		_RUN_HEAT_TEST_
		#message " ---取消门开计时一段时间自动回到待机----"
	#else
		#message " ---正常功能----"
	if((COOK_RUN_PAUSE == Sys.MState) && !Sys.PowMessage.DoorState)
	{
		#ifdef _FAST_TEST_
		if (GetTimeTick(TIME_100MS))
		#else
		if (GetTimeTick(TIME_1S))
		#endif
		{
			if(++Sys.RunOpenDoorSec >= COOK_DOOR_OPEN_TIME)
			{
				Sys.RunOpenDoorSec = 0;
				WorkModeNewSet(SET_FST_ON);
			}
		}
	}
	else
		Sys.RunOpenDoorSec = 0;	
	#endif

	if((COOK_RUN_PAUSE == Sys.MState) && !Sys.Time)
	{
	#ifdef _FAST_TEST_
		if (GetTimeTick(TIME_100MS))
	#else
		if (GetTimeTick(TIME_1S))
	#endif
		{
			BELL_NOR();
			if(++Sys.CookComplete_Sec >= COOK_COMPLETE_TIME)
			{
				Sys.CookComplete_Sec = 0;
				WorkModeNewSet(SET_FST_ON);
			}
		}
	}
	else
		Sys.CookComplete_Sec = 0;	


	if(COOK_IDLE == Sys.MState)
	{
		#ifdef _FAST_TEST_
		if (GetTimeTick(TIME_100MS))
		#else
		if (GetTimeTick(TIME_1S))
		#endif
		{
			if(++Sys.IDLE_Sec >= 180)
			{
				Sys.IDLE_Sec = 0;
				WorkModeNewSet(SET_STATE_NONE);
				BELL_NOR();
			}
		}
	}
	else
		Sys.IDLE_Sec = 0;	
}




/********************************************************************************************************
* @ name   : void F_TimeEvent(void)
* @ msg    : 时间变量处理
* @ param   (*)
* @ return  (*)
 ********************************************************************************************************/
void F_TimeEvent(void)
{
	F_TimeCount();
    if (GetTimeTick(TIME_100MS))
    {
		
    }
    if (GetTimeTick(TIME_1S))
    {
		
    }
}
