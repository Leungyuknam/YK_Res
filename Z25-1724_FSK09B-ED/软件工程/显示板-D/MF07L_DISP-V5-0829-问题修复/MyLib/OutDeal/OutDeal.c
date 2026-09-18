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
	static u8 FstRunTCnt = 0;
	if(STATE_WORK == Sys.MState && !Sys.TimeOnCount) 
	{
		if(FAN_SLEEP == Sys.Mode)
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
		else if(FAN_MAX == Sys.Mode)
		{
			Out.mFanSpeed = FAN_SEVEN;
		}
		else
		{
			if(FstRunTCnt < C_FST_TIME)		//刚开机的前3s
			{
				if(Out.mFanSpeed <= FAN_THREE)
					Out.mFanSpeed = FAN_THREE;
			}
			else
			Out.mFanSpeed = Sys.mFan;
		}
		if(FstRunTCnt < 255)
			FstRunTCnt ++;
	}
	else
	{
		Out.mFanSpeed = FAN_OFF;
		FstRunTCnt = 0;
		NatureSec = 0;
		NatureStep = 0;
	}
	if(Sys.Mode != FAN_NATURE)
	{
		NatureSec = 0;
		NatureStep = 0;
	}

	if(Sys.Fg.RLOSC && !Sys.TimeOnCount && STATE_WORK == Sys.MState)
	{
		if(LR_LastRecordCnt != LR_RecordCnt)
		{
			LR_LastRecordCnt = LR_RecordCnt;
			Out.LeftRightAngleCol.SetCol = M_RST_NEED;
			Out.LeftRightAngleCol.RollCode ++;
			Out.LeftRightFarAngleVal = c_LR_AngleRst;
			Out.LeftRightNearAngleVal = c_LR_AngleRst;
		}
		else if((Rst_Ok == Out.LeftRightAngleState.NowStep) && (Out.LeftRightAngleCol.RollCode == Out.LeftRightAngleState.RollCode))
		{
			//Out.LeftRightFarAngleVal = C50D_R;
			//Out.LeftRightNearAngleVal = C50D_L;
			
			if(Sys.mAngle == LR_ANGLE_45)
			{
				Out.LeftRightFarAngleVal = LR_UP45;
				Out.LeftRightNearAngleVal = LR_DN45;				
				
			}
			else if(Sys.mAngle == LR_ANGLE_90)
			{
				Out.LeftRightFarAngleVal = LR_UP90;
				Out.LeftRightNearAngleVal = LR_DN90;				
			}
			else if(Sys.mAngle == LR_ANGLE_180)
			{
				Out.LeftRightFarAngleVal = LR_UP180;
				Out.LeftRightNearAngleVal = LR_DN180;				
			}
			else if(Sys.mAngle == LR_ANGLE_360)
			{
				Out.LeftRightFarAngleVal = LR_UP360;
				Out.LeftRightNearAngleVal = LR_DN360;				
			}			
			Out.LeftRightAngleCol.SetCol = M_NORMAL;
		}
		Out.LeftRightAngleCol.AngleEn = TRUE;
	}
	else
	{
		if((Rst_Ok == Out.LeftRightAngleState.NowStep) && (STATE_NONE == Sys.MState))
		{
			Out.LeftRightAngleCol.SetCol = M_MID_NEED;
		}
		if(Out.LeftRightAngleCol.RollCode != Out.LeftRightAngleState.RollCode)
			LR_RecordCnt ++;
		Out.LeftRightAngleCol.AngleEn = FALSE;
	}

	if(Sys.Fg.UDOSC && !Sys.TimeOnCount && STATE_WORK == Sys.MState)
	{
		if(UpDn_LastRecordCnt != UpDn_RecordCnt)
		{
			UpDn_LastRecordCnt = UpDn_RecordCnt;
			Out.UpDnAngleCol.SetCol = M_RST_NEED;
			Out.UpDnAngleCol.RollCode ++;
			Out.UpDnFarAngleVal = c_UD_AngleRst;
			Out.UpDnNearAngleVal = c_UD_AngleRst;

			
		}
		else if((Rst_Ok == Out.UpDnAngleState.NowStep) && (Out.UpDnAngleCol.RollCode == Out.UpDnAngleState.RollCode))
		{
			//Out.UpDnFarAngleVal = UPDN_UP;
			//Out.UpDnNearAngleVal = UPDN_DN;
			if(Sys.mAngleUpDn == UD_ANGLE_30)
			{
				Out.UpDnFarAngleVal = UPDN_UP30;
				Out.UpDnNearAngleVal = UPDN_DN30;				
			}
			else if(Sys.mAngleUpDn == UD_ANGLE_60)
			{
				Out.UpDnFarAngleVal = UPDN_UP60;
				Out.UpDnNearAngleVal = UPDN_DN60;						
			}
			else if(Sys.mAngleUpDn == UD_ANGLE_90)
			{
				Out.UpDnFarAngleVal = UPDN_UP90;
				Out.UpDnNearAngleVal = UPDN_DN90;						
			}
			else if(Sys.mAngleUpDn == UD_ANGLE_180)
			{
				Out.UpDnFarAngleVal = UPDN_UP180;
				Out.UpDnNearAngleVal = UPDN_DN180;						
			}			
			
			Out.UpDnAngleCol.SetCol = M_NORMAL;
		}
		Out.UpDnAngleCol.AngleEn = TRUE;
	}
	else
	{
		if(Out.UpDnAngleCol.RollCode != Out.UpDnAngleState.RollCode)
			UpDn_RecordCnt ++;
		if((Rst_Ok == Out.UpDnAngleState.NowStep) && (STATE_NONE == Sys.MState))
		{
			Out.UpDnAngleCol.SetCol = M_MID_NEED;
		}
		Out.UpDnAngleCol.AngleEn = FALSE;
	}	
}


void OutPutAllOFF(void)
{
	Out.mFanSpeed = FAN_OFF;
	*((u8*)(&Out.UpDnAngleCol)) = 0;
	*((u8*)(&Out.LeftRightAngleCol)) = 0;
}

#define TIME_DIV		10
#define	DRV_NUM			4
void FactoryOutPutDeal(void)
{
	static u8 LR_RecordCnt = 0,LR_LastRecordCnt = 0xff;
	static u8 UpDn_RecordCnt = 0,UpDn_LastRecordCnt = 0xff;

    static X_REG u8 FstFactory_TCnt = 0;
	SysLoadBuf.Byte = 0;
	if(GetTimeTick(TIME_100MS))
	{
		if(FstFactory_TCnt < 255)
			FstFactory_TCnt ++;
		if(FstFactory_TCnt >= 100)
			FstFactory_TCnt = 0;
	}
	Sys.Fg.RLOSC = 1;
	Sys.Fg.UDOSC = 1;
		

	if(Sys.Fg.RLOSC)
	{
		if(LR_LastRecordCnt != LR_RecordCnt)
		{
			LR_LastRecordCnt = LR_RecordCnt;
			Out.LeftRightAngleCol.SetCol = M_RST_NEED;
			Out.LeftRightAngleCol.RollCode ++;
			Out.LeftRightFarAngleVal = c_LR_AngleRst;
			Out.LeftRightNearAngleVal = c_LR_AngleRst;
		}
		else if((Rst_Ok == Out.LeftRightAngleState.NowStep) && (Out.LeftRightAngleCol.RollCode == Out.LeftRightAngleState.RollCode))
		{
			
			if(Sys.mAngle == LR_ANGLE_45)
			{
				Out.LeftRightFarAngleVal = LR_UP45;
				Out.LeftRightNearAngleVal = LR_DN45;				
				
			}
			else if(Sys.mAngle == LR_ANGLE_90)
			{
				Out.LeftRightFarAngleVal = LR_UP90;
				Out.LeftRightNearAngleVal = LR_DN90;				
			}
			else if(Sys.mAngle == LR_ANGLE_180)
			{
				Out.LeftRightFarAngleVal = LR_UP180;
				Out.LeftRightNearAngleVal = LR_DN180;				
			}
			else if(Sys.mAngle == LR_ANGLE_360)
			{
				Out.LeftRightFarAngleVal = LR_UP360;
				Out.LeftRightNearAngleVal = LR_DN360;				
			}					
			
			Out.LeftRightAngleCol.SetCol = M_NORMAL;
		}
		Out.LeftRightAngleCol.AngleEn = TRUE;
	}
	else
	{
		if(Out.LeftRightAngleCol.AngleEn)
			LR_RecordCnt ++;
		Out.LeftRightAngleCol.AngleEn = FALSE;
	}

	if(Sys.Fg.UDOSC)
	{
		if(UpDn_LastRecordCnt != UpDn_RecordCnt)
		{
			UpDn_LastRecordCnt = UpDn_RecordCnt;
			Out.UpDnAngleCol.SetCol = M_RST_NEED;
			Out.UpDnAngleCol.RollCode ++;
			Out.UpDnFarAngleVal = c_UD_AngleRst;
			Out.UpDnNearAngleVal = c_UD_AngleRst;

			
		}
		else if((Rst_Ok == Out.UpDnAngleState.NowStep) && (Out.UpDnAngleCol.RollCode == Out.UpDnAngleState.RollCode))
		{
			
			if(Sys.mAngleUpDn == UD_ANGLE_30)
			{
				Out.UpDnFarAngleVal = UPDN_UP30;
				Out.UpDnNearAngleVal = UPDN_DN30;				
			}
			else if(Sys.mAngleUpDn == UD_ANGLE_60)
			{
				Out.UpDnFarAngleVal = UPDN_UP60;
				Out.UpDnNearAngleVal = UPDN_DN60;						
			}
			else if(Sys.mAngleUpDn == UD_ANGLE_90)
			{
				Out.UpDnFarAngleVal = UPDN_UP90;
				Out.UpDnNearAngleVal = UPDN_DN90;						
			}
			else if(Sys.mAngleUpDn == UD_ANGLE_180)
			{
				Out.UpDnFarAngleVal = UPDN_UP180;
				Out.UpDnNearAngleVal = UPDN_DN180;						
			}			
			
			Out.UpDnAngleCol.SetCol = M_NORMAL;
		}
		Out.UpDnAngleCol.AngleEn = TRUE;
	}
	else
	{
		if(Out.UpDnAngleCol.AngleEn)
			UpDn_RecordCnt ++;
		Out.UpDnAngleCol.AngleEn = FALSE;
	}	

	if(FstFactory_TCnt < 50)
		Out.mFanSpeed = FAN_SEVEN;
	else
		Out.mFanSpeed = FAN_OFF;	

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


