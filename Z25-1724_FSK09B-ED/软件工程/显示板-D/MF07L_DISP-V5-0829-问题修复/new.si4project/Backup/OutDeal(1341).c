#define _Native_OutDeal_H
#include "Include\Include.h"

u8 code NatureTab[] = {
	FAN_SEVEN,
	FAN_SEVEN,
	FAN_FIVE,
	FAN_FIVE,
	FAN_THREE,
	FAN_THREE,
	FAN_ONE,
	FAN_FIVE,
	FAN_FIVE,
	FAN_SEVEN,
	FAN_SEVEN,
	FAN_SEVEN,
	FAN_SEVEN,
	FAN_FIVE,
	FAN_FIVE,
	FAN_THREE,
	FAN_THREE,
	FAN_FIVE,
	FAN_FIVE,
	FAN_THREE,
	FAN_THREE,
	FAN_SEVEN,
	FAN_SEVEN,
	FAN_ONE,
	FAN_FIVE,
	FAN_FIVE,
	FAN_FIVE,
	FAN_FIVE
};
#define	FAN_STEP_MAX 	sizeof(NatureTab)/sizeof(NatureTab[0])
//100ms
void OutPutDeal(void)
{
	static u8 LR_RecordCnt = 0,LR_LastRecordCnt = 0xff;
	static u8 UpDn_RecordCnt = 0,UpDn_LastRecordCnt = 0xff;
	static u8 NatureStep = 0;
	static u8 NatureSec = 0;
	if(STATE_WORK == Sys.MState) 
	{
		if(Sys.Fg.FanSleep)
		{
			if(Sys.SleepHalfHour == 0)
				Out.mFanSpeed = FAN_THREE;
			else if(Sys.SleepHalfHour <= 3)
				Out.mFanSpeed = FAN_TWO;
			else
				Out.mFanSpeed = FAN_ONE;	
		}
		else if(FAN_NATURE == Sys.Mode)
		{
			if(GetTimeTick(TIME_1S))
			{
				if(++NatureSec >= 3)
				{
					NatureSec = 0;
					if(++NatureStep >= FAN_STEP_MAX)
						NatureStep = 0;		
				}
			}
			Out.mFanSpeed = NatureTab[NatureStep];
		}
		else
			Out.mFanSpeed = Sys.mFan;
	}
	else
	{
		Out.mFanSpeed = FAN_OFF;
	}
	if((Sys.Mode != FAN_NATURE) || Sys.Fg.FanSleep)
	{
		NatureSec = 0;
		NatureStep = 0;
	}
		

	if(Sys.Fg.RLOSC)
	{
		if(LR_LastRecordCnt != LR_RecordCnt)
		{
			LR_LastRecordCnt = LR_RecordCnt;
			Out.LeftRightAngleCol.SetCol = M_RST_NEED;
			Out.LeftRightAngleCol.RollCode ++;
			Out.LeftRightFarAngleVal = c_RL_AngleRst;
			Out.LeftRightNearAngleVal = c_RL_AngleRst;
		}
		if((Rst_Ok == Out.LeftRightAngleState.NowStep) && (Out.LeftRightAngleCol.RollCode == Out.LeftRightAngleState.RollCode))
		{
			Out.LeftRightFarAngleVal = C50D_R;
			Out.LeftRightNearAngleVal = C50D_L;

		}
		Out.LeftRightAngleState.AngleEn = TRUE;
	}
	else
	{
		if(Out.LeftRightAngleState.AngleEn)
			LR_RecordCnt ++;
		Out.LeftRightAngleState.AngleEn = FALSE;
	}

	if(Sys.Fg.UDOSC)
	{
		if(UpDn_LastRecordCnt != UpDn_RecordCnt)
		{
			UpDn_LastRecordCnt = UpDn_RecordCnt;
			Out.UpDnAngleCol.SetCol = M_RST_NEED;
			Out.UpDnAngleCol.RollCode ++;
			Out.UpDnFarAngleVal = c_AngleRst;
			Out.UpDnNearAngleVal = c_AngleRst;
		}
		if((Rst_Ok == Out.UpDnAngleState.NowStep) && (Out.UpDnAngleCol.RollCode == Out.UpDnAngleState.RollCode))
		{
			Out.UpDnFarAngleVal = UPDN_UP;
			Out.UpDnNearAngleVal = UPDN_DN;

		}
		Out.UpDnAngleState.AngleEn = TRUE;
	}
	else
	{
		if(Out.UpDnAngleState.AngleEn)
			UpDn_RecordCnt ++;
		Out.UpDnAngleState.AngleEn = FALSE;
	}	
}

#define TIME_DIV		10
#define	DRV_NUM			4
void FactoryOutPutDeal(void)
{
    static X_REG u8 FstFactory_TCnt = 0;
	SysLoadBuf.Byte = 0;
	if(GetTimeTick(TIME_100MS))
	{
		if(FstFactory_TCnt < 255)
			FstFactory_TCnt ++;
		if(FstFactory_TCnt >= TIME_DIV*DRV_NUM)
			FstFactory_TCnt = 0;
	}

	
    if(FstFactory_TCnt < TIME_DIV)
    {
		SysLoadBuf.Bits.Heat = TRUE;
    }
	else if(FstFactory_TCnt < 2*TIME_DIV)
	{
		SysLoadBuf.Bits.Fan = TRUE;
	}
	else if(FstFactory_TCnt < 3*TIME_DIV)
	{
		SysLoadBuf.Bits.WarmHeat = TRUE;
	}
	else 
	{
		SysLoadBuf.Bits.Light = TRUE;
	}
	SysLoad.Byte = SysLoadBuf.Byte;
}

#if 0
void CustomerTestOutPutDeal(void)
{
	SysLoadBuf.Byte = 0;
    if(1 == FactoryKeyNum)
    {
    	SysLoadBuf.Bits.Fan = TRUE;
		
    }
	else if(2 == FactoryKeyNum)
	{
		SysLoadBuf.Bits.Light = TRUE;
	}
	else if(4 == FactoryKeyNum)
	{
		SysLoadBuf.Bits.Heat = TRUE;
	}
	SysLoad.Byte = SysLoadBuf.Byte;
}
#endif


