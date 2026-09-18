/*********************************************************
文 件 名 : CarRefrigerator.C
文件说明 : 车载冰箱通用
编写作者 : thm
修订作者 : 
编写时间 : 2022-3-18
版 本 号 : V1.0
==========================================================*/
//头文件
#include "COMMON\AllInclude.h"
#include "PowerCalculate\PowerCalculate_C.h"


//全局变量定义
u8    RamDataType  g_u8SysErroStatus = C_ERRO_NULL;
Power_Typdef RamDataType  g_sPower = {0,0};
CarRfgrt_TypeDef RamDataType g_CarRfgrt;
Modeset_Typedef  RamDataType g_sModeSet = {0,0,0};

ANALOG_PWM_Typedef xdata g_sDefrost = {0,0,100,C_CH_PWM0};


uint g_u16Compress_RunTime=0;
uint g_u16Compress_2MinRunTime=0;
bit g_bChangeSetTemp=0;
char LastTemp=0;

/*****************************************************
*函数名称：void F_DivideBoard_Test(void)
*函数功能：间隔板判断处理
*输入参数：无
*输出参数：无
*调用周期：100ms
*说明：	  
*****************************************************/
void F_DivideBoard_Test(void)
{
	static 	FILTER8_T RamDataType s_sDevideBoard = {1,0,1,0};	

	if (F_FILTER_8Bit(_TEST_IO_DEVIDE,15,&s_sDevideBoard,0))
	{
		if (s_sDevideBoard.u8OutValue)
		{
			g_CarRfgrt.u8BoxEn = C_BOX_ONLY_LEFT_EN;
		}
		else
		{
			if (g_CarRfgrt.u8CloseFlag_L)
			{g_CarRfgrt.u8BoxEn = C_BOX_ONLY_RIGHT_EN;}
			else
			{g_CarRfgrt.u8BoxEn = C_BOX_ALL_EN;}

			if (g_CarRfgrt.s8SetTemp_R > g_CarRfgrt.s8SetTemp_L)
			{
				g_CarRfgrt.s8SetTemp_R = g_CarRfgrt.s8SetTemp_L;
			}
		}
	}

}


/*****************************************************
*函数名称：Power_Typdef F_BAT_PowerCalculate(unsigned int l_u16PwValue, unsigned char l_u8FilterTimes)
*函数功能：电量事件处理
*调用周期：160ms
*输入参数：l_u16PwValue（电池电压值）、l_u8FilterTimes（滤波次数）
*返回值：  s_sPower (电量值、电量等级)
*说明：
*****************************************************/
Power_Typdef F_BAT_PowerCalculate(unsigned int l_u16PwValue, unsigned char l_u8FilterTimes)
{
	static unsigned char xdata	s_u8PowerSource = C_POWER_UNKOWN;        
	static FILTER16_T    xdata  s_sFitBuf  = {0,0,0};                
	static Power_Typdef	 xdata  s_sPower = {0,0};

#if (Def_Client == C_CLIENT_TIEFU)
	unsigned char l_u8PwLevel;
#else
	unsigned char i;
#endif

	if (F_FILTER_16Bit(l_u16PwValue,l_u8FilterTimes,&s_sFitBuf,30))
	{
		if (s_sFitBuf.u16OutValue <= (C_POWER_DIV - 3))
		{s_u8PowerSource = C_POWER_12V;}
		else if (s_sFitBuf.u16OutValue > (C_POWER_DIV + 3))
		{s_u8PowerSource = C_POWER_24V;}
		
#if (Def_Client == C_CLIENT_AIKAI)

		for (i = 0; i < 6; i++)
		{
			if (s_sFitBuf.u16OutValue >= BAT_INCREASE_TABLE[s_u8PowerSource][s_sPower.u8Level])
			{
				if (++s_sPower.u8Level > C_POWER_LEVEL6_HIGHEST)
				{
					s_sPower.u8Level = C_POWER_LEVEL6_HIGHEST;
				}
			}
			else if (s_sFitBuf.u16OutValue <= BAT_DECREASE_TABLE[s_u8PowerSource][s_sPower.u8Level])
			{
				if (s_sPower.u8Level > 0)
				{
					s_sPower.u8Level--;	
				}	
			}
		}

#else

		if (s_sFitBuf.u16OutValue <= BAT_TABLE[s_u8PowerSource][0])
		{l_u8PwLevel = 0;}
		else if (s_sFitBuf.u16OutValue <= BAT_TABLE[s_u8PowerSource][1])
		{l_u8PwLevel = 1;} 
		else if (s_sFitBuf.u16OutValue <= BAT_TABLE[s_u8PowerSource][2])
		{l_u8PwLevel = 2;} 
		else if (s_sFitBuf.u16OutValue <= BAT_TABLE[s_u8PowerSource][3])
		{l_u8PwLevel = 3;} 
		else if (s_sFitBuf.u16OutValue <= BAT_TABLE[s_u8PowerSource][4])
		{l_u8PwLevel = 4;} 
		else
		{l_u8PwLevel = 5;}

		if (s_sPower.u8Level < l_u8PwLevel)
		{s_sPower.u8Level++;}
		else if (s_sPower.u8Level > l_u8PwLevel)
		{s_sPower.u8Level--;}

#endif
											
	}		
	
	s_sPower.u16Value = s_sFitBuf.u16OutValue;

	return s_sPower;	 	
}

/*****************************************************
*函数名称：void F_CompError_Test(void)
*函数功能：压缩机报错检测
*入口参数：void
*出口参数：g_CarRfgrt.u8CompError
*调用周期：125uS
*说明：
*****************************************************/
void F_CompError_Test(void)
{
	static bit 		 s_bLastIoStatus = 0;
	static bit 		 s_bNowIoStatus = 0;
	static bit 		 s_bStarCountFlag = 0;
	static u16 xdata s_u16FlashCnt = 0;
	static u16 xdata s_u16RunTimeCnt = 0;
	static u16 xdata s_u16LastCntValue = 0;
	static u16 xdata s_u16NormalCnt = 0;

    //获取IO口状态
    if (_TEST_IO_ERRO)
    {s_bNowIoStatus = 1;}
    else
    {s_bNowIoStatus = 0;}
    
    //IO口电平跳变
    if (s_bLastIoStatus != s_bNowIoStatus)
    {
        if (s_bStarCountFlag == 0)
        {s_bStarCountFlag = 1;}
        
        if (s_bStarCountFlag == 1)
        {s_u16FlashCnt++;}
        
        //相邻两次跳变间隔时间少于50ms，退出报错检测
        if (((s_u16RunTimeCnt - s_u16LastCntValue) < 200) && (s_u16RunTimeCnt != 0))
        {
            s_u16RunTimeCnt = 0;
            s_bStarCountFlag = 0;
            s_u16FlashCnt = 0;        
        }            
        s_u16LastCntValue = s_u16RunTimeCnt;
    }
    s_bLastIoStatus = s_bNowIoStatus;
    
    //3秒倒计时达到后根据闪烁次数判断报错类型
    if (s_bStarCountFlag == 1)
    {
        if (++s_u16RunTimeCnt >= 23800)
        {
            s_u16RunTimeCnt = 0;
            s_bStarCountFlag = 0;
            
            if (s_u16FlashCnt == 2)
            {g_CarRfgrt.u8CompError = C_ERRO_F1;}
            else if (s_u16FlashCnt == 4)
            {g_CarRfgrt.u8CompError = C_ERRO_F2;}
            else if (s_u16FlashCnt == 6)
            {g_CarRfgrt.u8CompError = C_ERRO_F3;}
            else if (s_u16FlashCnt == 8)
            {g_CarRfgrt.u8CompError = C_ERRO_F4;}
            else if (s_u16FlashCnt == 10)
            {g_CarRfgrt.u8CompError = C_ERRO_F5;}
            else if (s_u16FlashCnt == 12)
            {g_CarRfgrt.u8CompError = C_ERRO_F6;}
            else
            {g_CarRfgrt.u8CompError = C_ERRO_NULL;}

            s_u16FlashCnt = 0;  
        }
		s_u16NormalCnt = 0;
    }
	else
	{
		if (++s_u16NormalCnt >= 32000)
		{
			s_u16NormalCnt = 0;
			g_CarRfgrt.u8CompError = C_ERRO_NULL;
		}
	}   
}


/*****************************************************
*函数名称：void F_Compressor_ON_Logic(Compressor_TypeDef *psComp)
*函数功能：压缩机开启逻辑处理
*入口参数：
*出口参数：
*调用周期：
*说明：
*****************************************************/
static void F_Compressor_ON_Logic(Compressor_TypeDef *psComp)
{
	u16 l_u16DelayCmp;

	if (g_sModeSet.u8Status != C_MS_STATUS_CTRL_LBOX)
	{psComp->s8SetTemp_L = g_CarRfgrt.s8SetTemp_L;}

	if (g_sModeSet.u8Status != C_MS_STATUS_CTRL_RBOX)
	{psComp->s8SetTemp_R = g_CarRfgrt.s8SetTemp_R;}

	if (g_CarRfgrt.u8LastComProtectTime != g_CarRfgrt.u8ComProtectTime)
	{
		g_CarRfgrt.u8LastComProtectTime = g_CarRfgrt.u8ComProtectTime;
		psComp->u16DelayOnTime = 0;	
	}

	if (psComp->u8CompRunEn == C_Disable)			//压缩机停机保护时间
	{
		if (psComp->u16DelayOnTime < 600)
		{psComp->u16DelayOnTime++;}

		l_u16DelayCmp = (g_CarRfgrt.u8ComProtectTime * 60);
		if (psComp->u16DelayOnTime < l_u16DelayCmp)
		{return ;}		
	}

	if (g_CarRfgrt.u8OnOff == C_Power_OFF)
	{return ;}

	if (psComp->u8CompRunEn == C_Enable)
	{
		if((g_bChangeSetTemp==0) && (TEMPER_LBox.AD_Staut == c_AD_NORMAL))
		{
			if(g_u16Compress_RunTime<3600)
				g_u16Compress_RunTime++;
		}
		else
			g_u16Compress_RunTime=0;
		
		if(LastTemp!=psComp->s8SetTemp_L)
		{
			LastTemp=psComp->s8SetTemp_L;
			if(TEMPER_LBox.AD_Staut == c_AD_NORMAL)
				g_bChangeSetTemp=1;
		}	
		return ;
	}

	

#if (Def_Machine_Type == C_Machine_Type_SingleBox)

	if (g_CarRfgrt.s8RoomTemp_L >= (psComp->s8SetTemp_L + g_CarRfgrt.s8StartBias_L))		//实际的温度大于设定温度就开启压缩机  还要加上温度回差值E3
	{
		if(LastTemp!=psComp->s8SetTemp_L)
		{
			LastTemp=psComp->s8SetTemp_L;
			g_bChangeSetTemp=0;
		}	
		
		psComp->u8CompRunEn = C_Enable;
	}

	if (TEMPER_LBox.AD_Staut != c_AD_NORMAL)
	{
		g_bChangeSetTemp=0;
		g_u16Compress_RunTime=0;
		psComp->u8CompRunEn = C_Enable;
	}

#else

	if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_RIGHT_EN)
	{
		if (g_CarRfgrt.s8RoomTemp_L >= (psComp->s8SetTemp_L + g_CarRfgrt.s8StartBias_L))	//实际的温度大于设定温度就开启压缩机  还要加上温度回差值E3
		{psComp->u8CompRunEn = C_Enable;}
	
		if (TEMPER_LBox.AD_Staut != c_AD_NORMAL)
		{psComp->u8CompRunEn = C_Enable;}	
	}

	if (g_CarRfgrt.u8BoxEn != C_BOX_ONLY_LEFT_EN)
	{
		if (g_CarRfgrt.s8RoomTemp_R >= (psComp->s8SetTemp_R + g_CarRfgrt.s8StartBias_R))
		{psComp->u8CompRunEn = C_Enable;}
	
		if (TEMPER_RBox.AD_Staut != c_AD_NORMAL)
		{psComp->u8CompRunEn = C_Enable;}	
	}

#endif

	if (psComp->u8CompRunEn == C_Enable)	//开机了压缩机------延迟时间清0
	{
		psComp->u16DelayOnTime = 0;	
	}
}


/*****************************************************
*函数名称：void F_Compressor_OFF_Logic(Compressor_TypeDef *psComp)
*函数功能：压缩机关闭逻辑处理
*入口参数：
*出口参数：
*调用周期：
*说明：
*****************************************************/
static void F_Compressor_OFF_Logic(Compressor_TypeDef *psComp)
{
	
	if (psComp->u8CompRunEn == C_Disable)
	{
		g_u16Compress_RunTime=0;
		g_bChangeSetTemp=0;
		return ;
	}

	if (g_CarRfgrt.u8OnOff == C_Power_OFF)
	{
		g_bChangeSetTemp=0;
		psComp->u8CompRunEn = C_Disable;
		g_u16Compress_RunTime=0;
		return ;
	}

	if (g_CarRfgrt.u8CompError)	 //压缩机报错时，保持工作状态，不需要切换电磁阀
	{
		g_bChangeSetTemp=0;
		g_u16Compress_RunTime=0;
		return ;
	}

#if (Def_Machine_Type == C_Machine_Type_SingleBox)

	if (TEMPER_LBox.AD_Staut != c_AD_NORMAL)
	{}
	else if (g_CarRfgrt.s8RoomTemp_L <= (psComp->s8SetTemp_L + g_CarRfgrt.s8StopBias_L))	//停机温度是达到设定温度减去过冷点E9参数
	{psComp->u8CompRunEn = C_Disable;}

#elif (Def_Machine_Type == C_Machine_Type_DoubleBox_DoubleMaster)

	if (g_CarRfgrt.u8BoxEn == C_BOX_ONLY_LEFT_EN)
	{
		_BS(psComp->u8ValveTurn,C_VALVE_TURN_LEFT);
		_BC(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT);

		if (TEMPER_LBox.AD_Staut != c_AD_NORMAL)
		{}
		else if (g_CarRfgrt.s8RoomTemp_L <= (psComp->s8SetTemp_L + g_CarRfgrt.s8StopBias_L))
		{
			psComp->u8CompRunEn = C_Disable;
			psComp->u8ValveTurn = C_VALVE_TURN_NONE;
		}	
	}

	else if (g_CarRfgrt.u8BoxEn == C_BOX_ONLY_RIGHT_EN)
	{
		_BS(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT);
		_BC(psComp->u8ValveTurn,C_VALVE_TURN_LEFT);

		if (TEMPER_RBox.AD_Staut != c_AD_NORMAL)
		{}
		if (g_CarRfgrt.s8RoomTemp_R <= (psComp->s8SetTemp_R + g_CarRfgrt.s8StopBias_R))
		{
			psComp->u8CompRunEn = C_Disable;
			psComp->u8ValveTurn = C_VALVE_TURN_NONE;
		}	
	}

	else if (g_CarRfgrt.u8BoxEn == C_BOX_ALL_EN)
	{

		if (g_CarRfgrt.s8RoomTemp_L <= (psComp->s8SetTemp_L + g_CarRfgrt.s8StopBias_L))
		{_BC(psComp->u8ValveTurn,C_VALVE_TURN_LEFT);}
		else if (g_CarRfgrt.s8RoomTemp_L >= (psComp->s8SetTemp_L + g_CarRfgrt.s8StartBias_L))
		{_BS(psComp->u8ValveTurn,C_VALVE_TURN_LEFT);}

		if (TEMPER_LBox.AD_Staut != c_AD_NORMAL)
		{_BS(psComp->u8ValveTurn,C_VALVE_TURN_LEFT);}


		if (g_CarRfgrt.s8RoomTemp_R <= (psComp->s8SetTemp_R + g_CarRfgrt.s8StopBias_R))
		{_BC(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT);}
		else if (g_CarRfgrt.s8RoomTemp_R >= (psComp->s8SetTemp_R + g_CarRfgrt.s8StartBias_R))
		{_BS(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT);}

		if (TEMPER_RBox.AD_Staut != c_AD_NORMAL)
		{_BS(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT);}
							
	}

	if (psComp->u8ValveTurn == C_VALVE_TURN_NONE)
	{
		psComp->u8CompRunEn = C_Disable;	
	}

#elif (Def_Machine_Type == C_Machine_Type_DoubleBox_LeftMaster)

	if (g_CarRfgrt.u8BoxEn == C_BOX_ONLY_LEFT_EN)
	{
		_BS(psComp->u8ValveTurn,C_VALVE_TURN_LEFT);
		_BC(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT);

		if (TEMPER_LBox.AD_Staut != c_AD_NORMAL)
		{}
		else if (g_CarRfgrt.s8RoomTemp_L <= (psComp->s8SetTemp_L + g_CarRfgrt.s8StopBias_L))
		{
			psComp->u8CompRunEn = C_Disable;
			psComp->u8ValveTurn = C_VALVE_TURN_NONE;
		}	
	}

	else if (g_CarRfgrt.u8BoxEn == C_BOX_ONLY_RIGHT_EN)
	{
		_BC(psComp->u8ValveTurn,C_VALVE_TURN_LEFT);
		_BS(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT);

		if (TEMPER_RBox.AD_Staut != c_AD_NORMAL)
		{}
		else if (g_CarRfgrt.s8RoomTemp_R <= (psComp->s8SetTemp_R + g_CarRfgrt.s8StopBias_R))
		{
			psComp->u8CompRunEn = C_Disable;
			psComp->u8ValveTurn = C_VALVE_TURN_NONE;
		}	
	}

	else if (g_CarRfgrt.u8BoxEn == C_BOX_ALL_EN)
	{

		if (g_CarRfgrt.s8RoomTemp_R <= (psComp->s8SetTemp_R + g_CarRfgrt.s8StopBias_R))
		{_BC(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT);}
		else if (g_CarRfgrt.s8RoomTemp_R >= (psComp->s8SetTemp_R + g_CarRfgrt.s8StartBias_R))
		{_BS(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT);}

		if (TEMPER_RBox.AD_Staut != c_AD_NORMAL)
		{_BS(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT);}


		if (g_CarRfgrt.s8RoomTemp_L <= (psComp->s8SetTemp_L + g_CarRfgrt.s8StopBias_L))
		{_BC(psComp->u8ValveTurn,C_VALVE_TURN_LEFT);}
		else if (g_CarRfgrt.s8RoomTemp_L >= (psComp->s8SetTemp_L + g_CarRfgrt.s8StartBias_L))
		{_BS(psComp->u8ValveTurn,C_VALVE_TURN_LEFT);}

		if (TEMPER_LBox.AD_Staut != c_AD_NORMAL)
		{_BS(psComp->u8ValveTurn,C_VALVE_TURN_LEFT);}

		if (_BTS(psComp->u8ValveTurn,C_VALVE_TURN_LEFT))
		{_BC(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT);}
							
	}

	if (psComp->u8ValveTurn == C_VALVE_TURN_NONE)
	{
		psComp->u8CompRunEn = C_Disable;	
	}

#elif (Def_Machine_Type == C_Machine_Type_DoubleBox_RightMaster)

	if (g_CarRfgrt.u8BoxEn == C_BOX_ONLY_RIGHT_EN)
	{
		_BS(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT);
		_BC(psComp->u8ValveTurn,C_VALVE_TURN_LEFT);

		if (TEMPER_RBox.AD_Staut != c_AD_NORMAL)
		{}
		else if (g_CarRfgrt.s8RoomTemp_R <= (psComp->s8SetTemp_R + g_CarRfgrt.s8StopBias_R))
		{
			psComp->u8CompRunEn = C_Disable;
			psComp->u8ValveTurn = C_VALVE_TURN_NONE;
		}	
	}

	else if (g_CarRfgrt.u8BoxEn == C_BOX_ALL_EN)
	{

		if (g_CarRfgrt.s8RoomTemp_L <= (psComp->s8SetTemp_L + g_CarRfgrt.s8StopBias_L))
		{_BC(psComp->u8ValveTurn,C_VALVE_TURN_LEFT);}
		else if (g_CarRfgrt.s8RoomTemp_L >= (psComp->s8SetTemp_L + g_CarRfgrt.s8StartBias_L))
		{_BS(psComp->u8ValveTurn,C_VALVE_TURN_LEFT);}

		if (TEMPER_LBox.AD_Staut != c_AD_NORMAL)
		{_BS(psComp->u8ValveTurn,C_VALVE_TURN_LEFT);}


		if (g_CarRfgrt.s8RoomTemp_R <= (psComp->s8SetTemp_R + g_CarRfgrt.s8StopBias_R))
		{_BC(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT);}
		else if (g_CarRfgrt.s8RoomTemp_R >= (psComp->s8SetTemp_R + g_CarRfgrt.s8StartBias_R))
		{_BS(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT);}

		if (TEMPER_RBox.AD_Staut != c_AD_NORMAL)
		{_BS(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT);}

		if (_BTS(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT))
		{_BC(psComp->u8ValveTurn,C_VALVE_TURN_LEFT);}					
	}

	if (psComp->u8ValveTurn == C_VALVE_TURN_NONE)
	{
		psComp->u8CompRunEn = C_Disable;	
	}

#endif

}



/*****************************************************
*函数名称：void F_ValveTurn_Logic(Compressor_TypeDef *psComp)
*函数功能：电磁阀切换逻辑处理
*入口参数：
*出口参数：
*调用周期：
*说明：
*****************************************************/
static void F_ValveTurn_Logic(Compressor_TypeDef *psComp)
{

#if (Def_Machine_Type == C_Machine_Type_DoubleBox_DoubleMaster)

	static u8  RamDataType s_u8LastTurn_Status = C_VALVE_TURN_NONE;
	static u16 RamDataType s_u16IntervalTurnTime = C_Interval_Time;

	if (s_u8LastTurn_Status != psComp->u8ValveTurn)
	{
		if (g_CarRfgrt.u8ValveStatus == C_VALVE_TURN_LEFT)
		{
			if (_BTC(psComp->u8ValveTurn,C_VALVE_TURN_LEFT))
			{s_u16IntervalTurnTime = 0;}
		}
		else if (g_CarRfgrt.u8ValveStatus == C_VALVE_TURN_RIGHT)
		{
			if (_BTC(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT))
			{s_u16IntervalTurnTime = 0;}
		}		
		s_u8LastTurn_Status = psComp->u8ValveTurn;	
	}

	if (psComp->u8CompRunEn == C_Enable)
	{
		if (s_u16IntervalTurnTime == 0)
		{
			s_u16IntervalTurnTime = C_Interval_Time;
			if ((_BTS(psComp->u8ValveTurn,C_VALVE_TURN_LEFT)) &&
				(_BTS(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT)))
			{
				if (g_CarRfgrt.u8ValveStatus == C_VALVE_TURN_RIGHT)
				{g_CarRfgrt.u8ValveStatus = C_VALVE_TURN_LEFT;}
				else
				{g_CarRfgrt.u8ValveStatus = C_VALVE_TURN_RIGHT;}	
			}
			
			else if (_BTS(psComp->u8ValveTurn,C_VALVE_TURN_LEFT))
			{
				g_CarRfgrt.u8ValveStatus = C_VALVE_TURN_LEFT;	
			}
			
			else if (_BTS(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT))
			{
				g_CarRfgrt.u8ValveStatus = C_VALVE_TURN_RIGHT;	
			}

			else
			{
				g_CarRfgrt.u8ValveStatus = C_VALVE_TURN_NONE;	
			}									
		}
		else
		{
			s_u16IntervalTurnTime--;
		}	
	}	
	else
	{
		s_u16IntervalTurnTime = 0;
	    g_CarRfgrt.u8ValveStatus = C_VALVE_TURN_NONE;
	}

#elif ((Def_Machine_Type == C_Machine_Type_DoubleBox_LeftMaster) || (Def_Machine_Type == C_Machine_Type_DoubleBox_RightMaster))

	if (_BTS(psComp->u8ValveTurn,C_VALVE_TURN_LEFT))
	{
		g_CarRfgrt.u8ValveStatus = C_VALVE_TURN_LEFT;	
	}
	
	else if (_BTS(psComp->u8ValveTurn,C_VALVE_TURN_RIGHT))
	{
		g_CarRfgrt.u8ValveStatus = C_VALVE_TURN_RIGHT;	
	}

	else
	{
		g_CarRfgrt.u8ValveStatus = C_VALVE_TURN_NONE;	
	}

#elif (Def_Machine_Type == C_Machine_Type_SingleBox)

	_BS(psComp->u8ValveTurn,C_VALVE_TURN_LEFT);
	g_CarRfgrt.u8ValveStatus = C_VALVE_TURN_LEFT;

#endif					
		
}


/*****************************************************
*函数名称：void F_CompSpeed_Logic(Compressor_TypeDef *psComp)
*函数功能：压缩机转速逻辑处理
*入口参数：
*出口参数：
*调用周期：
*说明：
*****************************************************/
static void F_CompSpeed_Logic(Compressor_TypeDef *psComp)
{
	static u16 RamDataType s_u16FixedEcoTime = C_FIXED_2000RPM;
	static bit s_bFistMax4=1;
	if (g_sModeSet.u8Status != C_MS_STATUS_SPEED)
	{psComp->u8Speed = g_CarRfgrt.u8SpeedLv;}

	if ((g_CarRfgrt.u8OnOff == C_Power_ON) && (psComp->u8CompRunEn == C_Enable))
	{
		if(s_u16FixedEcoTime)
		{
			s_u16FixedEcoTime--;
			g_CarRfgrt.u8SpeedDrv = C_Speed_MAX;
		}
		else if(psComp->u8Speed == C_Speed_MAX)
		{
			if(g_u16Compress_RunTime==3600 || g_bChangeSetTemp || s_bFistMax4)
			{
				g_CarRfgrt.u8SpeedDrv = C_Speed_MAX4;
			}
			else
				g_CarRfgrt.u8SpeedDrv = C_Speed_MAX;
		}
		else
		{
			s_bFistMax4=0;
			g_u16Compress_RunTime=0;
			g_u16Compress_2MinRunTime=0;
			g_CarRfgrt.u8SpeedDrv = C_Speed_ECO;
		}
	}
	else
	{
		s_bFistMax4=0;
		g_CarRfgrt.u8SpeedDrv = C_Speed_STOP;
	}

	if (g_CarRfgrt.u8OnOff != C_Power_ON)
	{s_u16FixedEcoTime = C_FIXED_2000RPM;
	s_bFistMax4=1;}		//压缩机每次启动，前6分钟的转速固定为2000转
}


/*****************************************************
*函数名称：void F_CarRefrigerator_Init(Compressor_TypeDef *psComp)
*函数功能：压缩机转速逻辑处理
*入口参数：
*出口参数：
*调用周期：
*说明：
*****************************************************/
static void F_CarRefrigerator_Init(Compressor_TypeDef *psComp)   //CarRfgrt_TypeDef
{
	psComp->u16DelayOnTime = 0;
	psComp->u8CompRunEn = C_Disable;
	psComp->u8ValveTurn = C_VALVE_TURN_NONE;
	psComp->u8Speed = C_Speed_ECO;
	psComp->s8SetTemp_L = g_CarRfgrt.s8SetTemp_L;
	psComp->s8SetTemp_R = g_CarRfgrt.s8SetTemp_R;
	LastTemp=psComp->s8SetTemp_L;
}

/*****************************************************
*函数名称：void F_CarRefrigerator_Handle(void)
*函数功能：冰箱系统处理
*输入参数：无
*输出参数：无
*调用周期：1s
*说明：
*****************************************************/
void F_CarRefrigerator_Handle(void)
{
	static Compressor_TypeDef xdata s_sCompressor;
	static u8 xdata s_u8InitTime = 2;

	if (s_u8InitTime)
	{
		s_u8InitTime--;
		if (s_u8InitTime == 0)
		{
			F_CarRefrigerator_Init(&s_sCompressor);
		}
	}
	else
	{
		F_Compressor_ON_Logic(&s_sCompressor);
		F_Compressor_OFF_Logic(&s_sCompressor);
		F_ValveTurn_Logic(&s_sCompressor);
		F_CompSpeed_Logic(&s_sCompressor);
	}
}


/*****************************************************
*函数名称：void F_Drive_Valve(void)
*函数功能：电磁阀转向驱动
*输入参数：无
*输出参数：无
*调用周期：100ms
*说明：
*****************************************************/
static void F_Drive_Valve(void)
{
	static u8 RamDataType s_u8TurnTimes = 0;
	static u8 RamDataType s_u8DriveCnt = 0;
	static u8 RamDataType s_u8LastValveStatus = C_VALVE_TURN_NONE;	
		   u8 RamDataType s_u8OutBuff = 0xFF;

	if (s_u8LastValveStatus != g_CarRfgrt.u8ValveStatus)
	{
		s_u8LastValveStatus = g_CarRfgrt.u8ValveStatus;
		s_u8DriveCnt = 0;
		if (g_CarRfgrt.u8ValveStatus != C_VALVE_TURN_NONE)	
		{	
			s_u8TurnTimes = 3;	 //每次转向，重装3次，避免电磁阀没转到位
		}	 
	}
	
	if (s_u8TurnTimes)	         //单个转向周期，开500ms，停2秒
	{
		if (++s_u8DriveCnt < 9)
		{
			if (g_CarRfgrt.u8ValveToward == C_Toward_Forward)
			{
				if (g_CarRfgrt.u8ValveStatus == C_VALVE_TURN_LEFT)
				{_SIGNAL_B_OFF;}
				else
				{_SIGNAL_A_OFF;}
			}
			else
			{
				if (g_CarRfgrt.u8ValveStatus == C_VALVE_TURN_LEFT)
				{_SIGNAL_A_OFF;}
				else
				{_SIGNAL_B_OFF;}
			}
		}
		else if (s_u8DriveCnt > 25)
		{
			s_u8DriveCnt = 0;
			s_u8TurnTimes--;					
		}
	}
	
	if (_TEST_SIGNAL_A)
	{_IO_DCF_A_ON;}
	else
	{_IO_DCF_A_OFF;}
	
	if (_TEST_SIGNAL_B)
	{_IO_DCF_B_ON;}
	else
	{_IO_DCF_B_OFF;}
			
}

/*****************************************************
*函数名称：void F_Drive_Speed(void)
*函数功能：压缩机转速驱动
*输入参数：无
*输出参数：无
*调用周期：100ms
*说明：
*****************************************************/
static void F_Drive_Speed(void)
{
	if (g_CarRfgrt.u8SpeedDrv == C_Speed_MAX)
	{//_IO_RPM3000_ON;
	}
	else
	{//_IO_RPM3000_OFF;
		}
	
	if (g_CarRfgrt.u8SpeedDrv == C_Speed_ECO)
	{//_IO_RPM2000_ON;
		}
	else
	{//_IO_RPM2000_OFF;
		}		
}

/*****************************************************
*函数名称：void F_Drive_ProtectLv(void)
*函数功能：压缩机保护等级驱动
*输入参数：无
*输出参数：无
*调用周期：100ms
*说明：
*****************************************************/
static void F_Drive_ProtectLv(void)
{

	static u8 xdata s_u8DriveLv = C_Protect_High;

	if (g_sModeSet.u8Status != C_MS_STATUS_PROTECT)
	{
		s_u8DriveLv = g_CarRfgrt.u8BatProtectLv;
	}

	if (s_u8DriveLv == C_Protect_Low)
	{;//_IO_LOWPRT_ON;
		}
	else
	{;//_IO_LOWPRT_OFF;
		}
	
	if (s_u8DriveLv == C_Protect_Mid)
	{;//_IO_MIDPRT_ON;
		}
	else
	{;//_IO_MIDPRT_OFF;
		}
			
}

/*****************************************************
*函数名称：void F_DriveOutput_Handle(void)
*函数功能：IO驱动输出
*输入参数：无
*输出参数：无
*调用周期：100ms
*说明：
*****************************************************/
void F_DriveOutput_Handle(void)
{

	if (g_u8SelfCheckFlag)
	{
		F_SELFCHECK_Output();
	}
	else
	{

#if (Def_Machine_Type != C_Machine_Type_SingleBox)
		F_Drive_Valve();
#endif
		F_Drive_Speed();
		F_Drive_ProtectLv();
	}	

}



void F_ParameterProtect(void)
{
	
	if (g_CarRfgrt.u8OnOff > C_Power_ON)
	{g_CarRfgrt.u8OnOff = C_Power_ON;}

	if (g_CarRfgrt.u8TempUnit > C_Unit_Fahrenheit)
	{g_CarRfgrt.u8TempUnit = C_Unit_Celsius;}

	if (g_CarRfgrt.u8BatProtectLv > C_Protect_High)
	{g_CarRfgrt.u8BatProtectLv = C_Protect_High;}

	if (g_CarRfgrt.u8SpeedLv > C_Speed_STOP)
	{g_CarRfgrt.u8SpeedLv = C_Speed_STOP;}

	if (g_CarRfgrt.u8SpeedDrv > C_Speed_STOP)
	{g_CarRfgrt.u8SpeedDrv = C_Speed_STOP;}

	if (g_CarRfgrt.u8BoxEn > C_BOX_ONLY_LEFT_EN)
	{g_CarRfgrt.u8BoxEn = C_BOX_ALL_EN;}

	if (g_CarRfgrt.u8NtcToward > C_Toward_Backward)
	{g_CarRfgrt.u8NtcToward = C_Toward_Forward;}

	if (g_CarRfgrt.u8ValveToward > C_Toward_Backward)
	{g_CarRfgrt.u8ValveToward = C_Toward_Forward;}

	if (g_CarRfgrt.u8PowerSource > C_Power_Battery)
	{g_CarRfgrt.u8PowerSource = C_Power_Adaptor;}

	if (g_CarRfgrt.u8AppLock > C_APP_LOCK_ENABLE)
	{g_CarRfgrt.u8AppLock = C_APP_LOCK_DISABLE;}

	if (g_CarRfgrt.u8BleStatus > C_BLE_BIND)
	{g_CarRfgrt.u8BleStatus = C_BLE_BIND;}

	if (g_CarRfgrt.s8SetTemp_Bot > g_CarRfgrt.s8SetTemp_L)
	{g_CarRfgrt.s8SetTemp_Bot = g_CarRfgrt.s8SetTemp_L;}
	else if (g_CarRfgrt.s8SetTemp_Bot < (-40))
	{g_CarRfgrt.s8SetTemp_Bot = (-40);}

	if (g_CarRfgrt.u8TempUnit == C_Unit_Celsius)
	{
		if (g_CarRfgrt.s8SetTemp_L > 40)
		{g_CarRfgrt.s8SetTemp_L = (-20);}
	
		if (g_CarRfgrt.s8SetTemp_Top > 40)
		{g_CarRfgrt.s8SetTemp_Top = 40;}
		else if (g_CarRfgrt.s8SetTemp_Top < g_CarRfgrt.s8SetTemp_L)
		{g_CarRfgrt.s8SetTemp_Top = g_CarRfgrt.s8SetTemp_L;}

		if (g_CarRfgrt.s8TempComp_N6_L > 10)
		{g_CarRfgrt.s8TempComp_N6_L = 10;}
		else if (g_CarRfgrt.s8TempComp_N6_L < (-10))
		{g_CarRfgrt.s8TempComp_N6_L = (-10);}

		if (g_CarRfgrt.s8TempComp_N12_L > 10)
		{g_CarRfgrt.s8TempComp_N12_L = 10;}
		else if (g_CarRfgrt.s8TempComp_N12_L < (-10))
		{g_CarRfgrt.s8TempComp_N12_L = (-10);}

		if (g_CarRfgrt.s8TempComp_N13_L > 10)
		{g_CarRfgrt.s8TempComp_N13_L = 10;}
		else if (g_CarRfgrt.s8TempComp_N13_L < (-10))
		{g_CarRfgrt.s8TempComp_N13_L = (-10);}

		if (g_CarRfgrt.s8StartBias_L > 10)
		{g_CarRfgrt.s8StartBias_L = 10;}
		else if (g_CarRfgrt.s8StartBias_L < (1))
		{g_CarRfgrt.s8StartBias_L = (1);}

		if (g_CarRfgrt.s8StopBias_L > 0)
		{g_CarRfgrt.s8StopBias_L = 0;}
		else if (g_CarRfgrt.s8StopBias_L < (-10))
		{g_CarRfgrt.s8StopBias_L = (-10);}
	}
	else
	{
		if (g_CarRfgrt.s8SetTemp_L > 104)
		{g_CarRfgrt.s8SetTemp_L = (-4);}
	
		if (g_CarRfgrt.s8SetTemp_Top > 104)
		{g_CarRfgrt.s8SetTemp_Top = 104;}
		else if (g_CarRfgrt.s8SetTemp_Top < g_CarRfgrt.s8SetTemp_L)
		{g_CarRfgrt.s8SetTemp_Top = g_CarRfgrt.s8SetTemp_L;}

		if (g_CarRfgrt.s8TempComp_N6_L > 18)
		{g_CarRfgrt.s8TempComp_N6_L = 18;}
		else if (g_CarRfgrt.s8TempComp_N6_L < (-18))
		{g_CarRfgrt.s8TempComp_N6_L = (-18);}

		if (g_CarRfgrt.s8TempComp_N12_L > 18)
		{g_CarRfgrt.s8TempComp_N12_L = 18;}
		else if (g_CarRfgrt.s8TempComp_N12_L < (-18))
		{g_CarRfgrt.s8TempComp_N12_L = (-18);}

		if (g_CarRfgrt.s8TempComp_N13_L > 18)
		{g_CarRfgrt.s8TempComp_N13_L = 18;}
		else if (g_CarRfgrt.s8TempComp_N13_L < (-18))
		{g_CarRfgrt.s8TempComp_N13_L = (-18);}

		if (g_CarRfgrt.s8StartBias_L > 18)
		{g_CarRfgrt.s8StartBias_L = 18;}
		else if (g_CarRfgrt.s8StartBias_L < (2))
		{g_CarRfgrt.s8StartBias_L = (2);}

		if (g_CarRfgrt.s8StopBias_L > 0)
		{g_CarRfgrt.s8StopBias_L = 0;}
		else if (g_CarRfgrt.s8StopBias_L < (-18))
		{g_CarRfgrt.s8StopBias_L = (-18);}

	}

}


void F_Defrost(void)
{
	static u16 xdata s_u16DefrostMin = 0;
	static u16 xdata s_u16DefrostTime = 0;
	static u16 xdata s_u16Filter = 0;
	static bit s_bSource = 0;
	static bit s_bHeat = 0;
	u8 l_u8CmpTemp = 0;
	s8 l_s8DefrostTemp;
	
	if (g_u8SelfCheckFlag)
	{return ;}

	if (g_CarRfgrt.u8TempUnit == C_Unit_Celsius)
	{l_s8DefrostTemp = 0;}
	else
	{l_s8DefrostTemp = 32;}
	
	//根据输入电压调节加热占空比
	if (g_sPower.u16Value > 177)
	{s_bSource = C_POWER_24V;}
	else if (g_sPower.u16Value < 173)
	{s_bSource = C_POWER_12V;}

	//保护电压判定
	if (s_bSource == C_POWER_12V)
	{
		if ((g_sPower.u16Value <= 120))
		{l_u8CmpTemp = 100;}
		else if ((g_sPower.u16Value > 170))
		{}
		else
		{l_u8CmpTemp = 98 - (g_sPower.u16Value - 120);}
	}
	else
	{
		if ((g_sPower.u16Value < 315) && (g_sPower.u16Value >= 215))
		{l_u8CmpTemp = 32 - (g_sPower.u16Value - 215)/5;}
	}
	
	//温度条件滤波
	if (g_CarRfgrt.s8RoomTemp_L <= l_s8DefrostTemp)	//小于0发热丝工作
	{
		if (s_u16Filter < (300))
		{
			s_u16Filter++;
		}

		else
		{
			s_bHeat = 1;
		}			
	}
	else
	{
		if (s_u16Filter > 0)
		{
			s_u16Filter--;
		}

		else
		{
			s_bHeat = 0;
		}
	}
	
	//仅开机和非低电量故障时允许工作，每小时开40分钟停20分钟
	if (s_bHeat && (g_CarRfgrt.u8OnOff == C_Power_ON) && (g_CarRfgrt.u8CompError != C_ERRO_F1))
	{
		if (++s_u16DefrostTime >= 600)
		{
			s_u16DefrostTime = 0;
			
			if (s_u16DefrostMin < 0xFFFF)
			{s_u16DefrostMin++;}
		}
		
		if (s_u16DefrostMin >= (g_CarRfgrt.u8DefrostOnTime + g_CarRfgrt.u8DefrostOffTime))  //60
		{
			s_u16DefrostMin = 0;
		}
		
		else if (s_u16DefrostMin < (g_CarRfgrt.u8DefrostOnTime))  //40
		{
			
		}
		
		else
		{
			l_u8CmpTemp = 0;
		}
	}
	else
	{
		s_u16DefrostTime = 0;
		s_u16DefrostMin = 0;
		l_u8CmpTemp = 0;
	}
	
	g_sDefrost.u16Duty = l_u8CmpTemp;	
}

