#define	_Native_FUN_H
#include "Include\Include.h"

static U16 s_u161sTime = 0;

//35度   147.3343	247  //256-247
//45度 	96.2750		243
//50度 	78.4837		240
//55度 	64.3204 	237



u8 code TempGoalTab[] ={  								
	//  OK的												OK 						//OK				
	//	L1						L2						L3						L4							
	//	60, 					80, 					110,					120,		
		(0x3ff-0x19a),  		(0x3ff-0x137),			(0x3ff-0x10c),	 		(0x3ff-0xe7)
};


u8 code TempHeatTimeTab[] ={ 
	//	L1						L2						L3						L4			
		20,						30,						40,						50,																									
};

u8 code TempHeatCloseTimeTab[] ={ 
	//	L1						L2						L3						L4			
		10,						20,						20,						20,																									
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
	
	GoalTempAD = TempGoalTab[Sys.mTempLevel]; 
	
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
			if(u8tempDiff < 40)  
			{
				sHeat.HeatStop_Sec = 10;//20;
				sHeat.BHeatFlag = 0;
				sHeat.HeatStep ++;	
				BUZZ_PROHEAT_OK();
			}	
			else if(0 == u8tempDiff)
			{
				sHeat.HeatStep ++;	
				BUZZ_PROHEAT_OK();
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
				HeatLevel = 1;
			}
		break;
		
		case 2:
				u8temp0 = TempHeatTimeTab[Sys.mTempLevel]; //Sys.CookLevel
				u8tempDiff = TempHeatCloseTimeTab[Sys.mTempLevel];
				HeatLevel = Drive_Heat_Control(NowTempDat,GoalTempAD,40,10,50,0,u8temp0,u8tempDiff,&sHeat);
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
