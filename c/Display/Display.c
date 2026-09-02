#include "h\Allinclude.h"

U8 xdata g_u8VocDispState = 0;              //VOC RGB灯显示状态
U8 xdata g_u8PM25DispState = 0;             //PM25 RGB灯显示状态
U8 xdata g_u8CH2ODispState = 0;             //CH2O RGB灯显示状态
U8 xdata g_u8FilterDispState = 0;           //滤网 RGB灯显示状态


U8 xdata g_u8RGBDispMode = C_RGB_DISMODE_AUTO;				    //当前RGB显示类别 自动/手动
U8 xdata g_u8RGBManualDispTime = C_RGB_MANUAL_DISP_TIME; 	//手动切换RGB类型时的显示时间
U8 xdata g_u8RGBAutoDispTime = C_RGB_AUTO_DISP_TIME;   		//自动切换RGB类型时的显示时间

bit g_bRgbDispEn = 1;                       //RGB显示使能

U8 xdata g_u8LockBlinkTime = 0;             //童锁闪烁时间
U8 xdata g_u8TimeHBlinkTime = 0;            //定时闪烁时间

U8 xdata g_u8KeyDispTime = C_KEY_DISP_TIME; //按键显示时间
U8 xdata g_u8SleepDispTime = 0;             //睡眠显示时间
bit g_bHalfDispEn = 0;                      //半亮使能标记

U8 xdata g_u8PowerOnDispTime = C_POWERON_DISP_TIME; //按键显示时间


U8 xdata g_u8DisBright = 5;//LED灯显示亮度

//U16 xdata g_sPm25.u16DispData = 0;            //PM25显示数值
U16 xdata g_u16DispPm10Data = 0;            //PM10显示数值
U16 xdata g_u16DispPm1Data = 0;             //PM1显示数值


U8 xdata g_u8DisTypeSwitchTime = 0;					//湿度温度类型切换间隔，单位100ms
U8 xdata g_u8DisTypeSwitchFlag = 0;	

U8 xdata g_u8DisFliterTime = 0;
U8 xdata g_u8DisFliterSwitchTime = 100;	



bit g_bVoiceFilterEn = 0;

U8 xdata g_u8RGBDispType = 0;
U8 xdata g_u8RgbColorIndex = 0;

U8 xdata DispBuf[TOTAL_COM] = {0};
U8 xdata ChipDispBuf[TOTAL_COM] = {0};

U8 xdata g_u8DispLastT = 0;
U8 xdata g_u8DispIndex = 0;
U8 xdata g_u8DispBlink = 0;
bit g_bDispMask = 0;

U8 xdata g_u8GobalBlink = 0;

code TYPE_DISP_TASK DispTskTbl[DISP_TOTAL_TASK];

U8 code NumTable[16] = {N_0,N_1,N_2,N_3,N_4,N_5,N_6,N_7,N_8,N_9,N_A,N_B,N_C,N_D,N_E,N_F};

U8 code NumTableTempHumi[16] = {N_TEMP_0,N_TEMP_1,N_TEMP_2,N_TEMP_3,N_TEMP_4,N_TEMP_5,
																N_TEMP_6,N_TEMP_7,N_TEMP_8,N_TEMP_9,N_TEMP_A,N_TEMP_B,
																N_TEMP_C,N_TEMP_D,N_TEMP_E,N_TEMP_F};

U8 code NumTableTimeOnes[16] = {
    N_ONES_0, N_ONES_1, N_ONES_2, N_ONES_3,
    N_ONES_4, N_ONES_5, N_ONES_6, N_ONES_7,
    N_ONES_8, N_ONES_9, N_ONES_A, N_ONES_B,
    N_ONES_C, N_ONES_D, N_ONES_E, N_ONES_F
};

U8 code NumTableTimeTens[16] = {
    N_TENS_0, N_TENS_1, N_TENS_2, N_TENS_3,
    N_TENS_4, N_TENS_5, N_TENS_6, N_TENS_7,
    N_TENS_8, N_TENS_9, N_TENS_A, N_TENS_B,
    N_TENS_C, N_TENS_D, N_TENS_E, N_TENS_F
};
																		

U8 code RgbDispTable[2][6][3] = 
{
    {   {0,0,0}, {0,30,0}, {0,15,30}, {23,8,0}, {30,0,0}, {30,30,30},    },
    {   {0,0,0}, {0,15,0}, {0,7,15},  {11,4,0}, {15,0,0}, {15,15,15},    },
};

U8 code RgbOnTable[6][3] = {
    /*R,G,B*/
    {0,0,0}, // 0 OFF
    {0,1,0}, // 1 GREEN
    {0,0,1}, // 2 BLUE
    {1,1,0}, // 3 ORANGE/YELLOW（直通红+绿）
    {1,0,0}, // 4 RED
    {1,1,1}, // 5 WHITE
};

/**************************************************
*函数名称：void  F_Blink500ms(void) 
*函数功能：500ms闪烁控制
*入口参数：void
*出口参数：void  
**************************************************/
U8 F_Blink500ms()
{
    if((g_u8GobalBlink % 10) < 5)
    {   return 1;}
    else
    {   return 0;}
}


/**************************************************
*函数名称：U8 F_Blink1s(void)
*函数功能：1s闪烁控制（亮1s，灭1s）
*入口参数：void
*出口参数：U8  (1=亮, 0=灭)
**************************************************/
U8 F_Blink1s(void)
{
    // g_u8GobalBlink 每100ms加1 → 20计数=2s
    if ((g_u8GobalBlink % 20) < 10)
        return 1;   // 前1秒亮
    else
        return 0;   // 后1秒灭
}


/**************************************************
*函数名称：void  F_RgbTestDispDrv(void) 
*函数功能：RGB测试显示输出 (仅更新索引)
*入口参数：l_u8DisType (颜色索引)
*出口参数：void  
**************************************************/
void F_RgbTestDispDrv(U8 l_u8DisType)
{ 
    g_u8RgbColorIndex = l_u8DisType;
}

/**************************************************
*函数名称：void  F_RgbDispDrv(void) 
*函数功能：RGB业务逻辑显示 (仅更新索引)
*入口参数：void
*出口参数：void  
**************************************************/
void F_RgbDispDrv()
{ 
    // 1. 错误或门被打开，关闭RGB
    if(g_bQDErr || g_bDoorOpenLeft || g_bDoorOpenRight)
    {
        g_u8RgbColorIndex = 0; // 0 为 OFF
    }
    // 2. 上电轮流显示
    else if(DIPS_POWERUP == F_GetDispType())            
    {
        g_u8RgbColorIndex = g_u8CH2ODispState;
    }
    // 3. 正常开机状态
    else if(g_bTurnOn)
    {
        // 睡眠模式关闭RGB
        if(MODE_SLEEP == g_u8WorkMode)
        {
            g_u8RgbColorIndex = 0; 
        }
        else
        {
            // 根据当前的显示类型，赋予对应的颜色状态索引
            switch(g_u8RGBDispType)
            {
                case C_DISPTYPE_CH2O:
                    g_u8RgbColorIndex = g_u8CH2ODispState;
                    break;
                case C_DISPTYPE_PM25:
                    g_u8RgbColorIndex = g_u8PM25DispState;
                    break;
                case C_DISPTYPE_VOC:
                    g_u8RgbColorIndex = g_u8VocDispState;
                    break;
                default:
                    // 默认绿色 (假设宏 C_DISP_GREEN 为 1)
                    g_u8RgbColorIndex = 1; 
                    break;
            }
        }
    }
    // 4. 关机状态
    else 
    {
        g_u8RgbColorIndex = 0;
    }
}



/**************************************************
*函数名称：void  F_DispThreeNum(void) 
*函数功能：三八显示(最左侧数字数码管不显示)
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispThreeNum(U16 l_u16Num)
{
    if(l_u16Num > 999)
    {   l_u16Num = 999;}
    
    F_SpecialDisFour(0,NumTable[l_u16Num / 100],NumTable[l_u16Num % 100 / 10],NumTable[l_u16Num % 10]);
}

/**************************************************
*函数名称：void  F_DispFourNum(void) 
*函数功能：四八显示
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispFourNum(U16 l_u16Num)
{
    if(l_u16Num > 9999)
    {   l_u16Num = 9999;}
    
    F_SpecialDisFour(NumTable[l_u16Num / 1000],NumTable[l_u16Num % 1000 / 100],NumTable[l_u16Num % 100 / 10],NumTable[l_u16Num % 10]);
}


/*****************************************************************************
* 辅助函数：专门用于显示倒计时数字 (居中显示)
******************************************************************************/
void F_DispPreheatCountdown(U16 l_u16Sec)
{
    if(l_u16Sec < 10)
    {
        F_SpecialDisFour(0, 0, 0, NumTable[l_u16Sec]);
    }
    else if(l_u16Sec < 100)
    {
        F_SpecialDisFour(0, 0, NumTable[l_u16Sec / 10], NumTable[l_u16Sec % 10]);
    }
    else
    {
        F_SpecialDisFour(0, NumTable[l_u16Sec / 100], NumTable[l_u16Sec % 100 / 10], NumTable[l_u16Sec % 10]);
    }
}

/*****************************************************************************
* 传感器显示逻辑更新：包含预热倒计时
******************************************************************************/
void F_DispPM25()
{   
    DIS_PM25_LOGO();
    
    // 如果还没度过预热期，显示倒计时秒数
    if(g_u16SenseEnTime < c_STIVE_PREHEAT_TIME)
    {   
        // 计算剩余秒数并显示
        U16 sec = (c_STIVE_PREHEAT_TIME - g_u16SenseEnTime + 9) / 10;
        F_DispPreheatCountdown(sec);
        
        // 预热期强制亮绿灯
        g_u8PM25DispState = C_DISP_BLUE; 
    }
    else
    {   
        DIS_UG();
        if(g_sPm25.u16DispData < 10)
        {
            F_SpecialDisFour(0, 0, 0, NumTable[g_sPm25.u16DispData]);
        }
        else if(g_sPm25.u16DispData < 100)
        {
            F_SpecialDisFour(0, 0, NumTable[g_sPm25.u16DispData / 10], NumTable[g_sPm25.u16DispData % 10]);
        }
        else if(g_sPm25.u16DispData < 1000)
        {
            F_SpecialDisFour(0, NumTable[g_sPm25.u16DispData / 100], NumTable[g_sPm25.u16DispData % 100 / 10], NumTable[g_sPm25.u16DispData % 10]);
        }
    }
}

void F_DispVOC()
{   
    DIS_VOC_LOGO();
    
    if(g_u16SenseEnTime < c_VOC_PREHEAT_TIME)
    {   
        U16 sec = (c_VOC_PREHEAT_TIME - g_u16SenseEnTime + 9) / 10;
        F_DispPreheatCountdown(sec);
        g_u8VocDispState = C_DISP_BLUE;
    }
    else
    {   
//        DIS_MG();
//        if(g_u16DispVocData < 10)
//        {
//            F_SpecialDisFour(0, 0, 0, NumTable[g_u16DispVocData]);
//        }
//        else if(g_u16DispVocData < 100)
//        {
//            F_SpecialDisFour(0, 0, NumTable[g_u16DispVocData / 10], NumTable[g_u16DispVocData % 10]);
//        }
//        else if(g_u16DispVocData < 1000)
//        {
//            F_SpecialDisFour(0, NumTable[g_u16DispVocData / 100], NumTable[g_u16DispVocData % 100 / 10], NumTable[g_u16DispVocData % 10]);
//        }
		
		DIS_DP();
        DIS_MG();
        F_DispFourNum(g_u16DispVocData); // 原有四位显示
    }
}

void F_DispCH2O()
{   
    DIS_CH2O_LOGO();
    
    if(g_u16SenseEnTime < c_CH2O_PREHEAT_TIME)
    {   
        U16 sec = (c_CH2O_PREHEAT_TIME - g_u16SenseEnTime + 9) / 10;
        F_DispPreheatCountdown(sec);
        g_u8CH2ODispState = C_DISP_BLUE;
    }
    else
    {   
        DIS_DP();
        DIS_MG();
        F_DispFourNum(g_u16DispCH2OData); // 原有四位显示
    }
}

///**************************************************
//*函数名称：void  F_DispPM25(void) 
//*函数功能：显示PM25及符号
//*入口参数：void
//*出口参数：void  
//**************************************************/
//void F_DispPM25()
//{	
//	if(g_sPm25.u16DispData < 10)
//	{
//		F_SpecialDisFour(0,0,0,NumTable[g_sPm25.u16DispData]);
//	}
//	else if(g_sPm25.u16DispData < 100)
//	{
//		F_SpecialDisFour(0,0,NumTable[g_sPm25.u16DispData / 10],NumTable[g_sPm25.u16DispData % 10]);
//	}
//	else if(g_sPm25.u16DispData < 1000)
//	{
//		F_SpecialDisFour(0,NumTable[g_sPm25.u16DispData / 100],NumTable[g_sPm25.u16DispData % 100 / 10],NumTable[g_sPm25.u16DispData % 10]);
//	}
//    DIS_PM25_LOGO();
//	DIS_UG();
//}
///**************************************************
//*函数名称：void  F_DispVOC(void) 
//*函数功能：显示VOC及符号
//*入口参数：void
//*出口参数：void  
//**************************************************/
//void F_DispVOC()
//{	
//	if(g_u16DispVocData < 10)
//	{
//			F_SpecialDisFour(0,0,0,NumTable[g_u16DispVocData]);
//	}
//	else if(g_u16DispVocData < 100)
//	{
//			F_SpecialDisFour(0,0,NumTable[g_u16DispVocData / 10],NumTable[g_u16DispVocData % 10]);
//	}
//	else if(g_u16DispVocData < 1000)
//	{
//			F_SpecialDisFour(0,NumTable[g_u16DispVocData / 100],NumTable[g_u16DispVocData % 100 / 10],NumTable[g_u16DispVocData % 10]);
//	}
//    DIS_VOC_LOGO();
//	DIS_MG();
//}
///**************************************************
//*函数名称：void  F_DispCH2O(void) 
//*函数功能：显示CH2O及符号
//*入口参数：void
//*出口参数：void  
//**************************************************/
//void F_DispCH2O()
//{	
//	F_DispFourNum(g_u16DispCH2OData);
//	DIS_DP();
//    DIS_CH2O_LOGO();
//	DIS_MG();
//}
/**************************************************
*函数名称：void  F_DispTemp(void) 
*函数功能：显示温度及符号
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispTemp()
{
		if(g_s8NowTemp < 10)
		{
			F_SpecialDisTempHumi(0,NumTableTempHumi[g_s8NowTemp]);
		}
		else
		{
			F_SpecialDisTempHumi(NumTableTempHumi[g_s8NowTemp/10],NumTableTempHumi[g_s8NowTemp%10]);
		}
		DIS_TEMP_C();
}
/**************************************************
*函数名称：void  F_DispHumi(void) 
*函数功能：显示湿度及符号
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispHumi()
{
		if(g_u8NowHumi < 10)
		{
			F_SpecialDisTempHumi(0,NumTableTempHumi[g_u8NowHumi]);
		}
		else
		{
			F_SpecialDisTempHumi(NumTableTempHumi[g_u8NowHumi/10],NumTableTempHumi[g_u8NowHumi%10]);
		}
		DIS_HUMI_BFB();	
}
/**************************************************
*函数名称：void F_DispFan(void) 
*函数功能：显示风速
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispFan()
{	
    if(MODE_LOW == g_u8WorkMode)
    {   F_SpecialDis(0,0,N_1);}
    else if(MODE_MID == g_u8WorkMode)
    {   F_SpecialDis(0,0,N_2);}
    else //if(MODE_HIGH == g_u8WorkMode)
    {   F_SpecialDis(0,0,N_3);}
}
/**************************************************
*函数名称：void F_DispTimeNum(void) 
*函数功能：显示定时数字
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispTimeNum(U8 l_u8Time)
{	
    if(l_u8Time > 99)
    {   l_u8Time = 99;}
	if(!l_u8Time && !g_u8TimeHBlinkTime)
    {
        return; 
    }
	if(!l_u8Time)
    {   F_SpecialDisTime(N_TENS__,N_ONES__);}
    else if(l_u8Time < 10)
    {   F_SpecialDisTime(0,NumTableTimeOnes[l_u8Time]);}
    else
    {   F_SpecialDisTime(NumTableTimeTens[l_u8Time / 10],NumTableTimeOnes[l_u8Time % 10]);}
	DIS_TIME_LOGO();
	DIS_TIME_H();
		
}
/**************************************************
*函数名称：void F_DispTime(void) 
*函数功能：显示定时
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispTime()
{
    F_DispTimeNum(g_u8TimeData);
}
/**************************************************
*函数名称：void  F_DispNormal(void) 
*函数功能：常规显示
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispNormal()
{
	U8 l_u8i = 0;		
	if(g_bTurnOn)
	{
		if(MODE_SLEEP != g_u8WorkMode)
		{
			if(g_u8PowerOnDispTime)
			{
				l_u8i = g_u8PowerOnDispTime/10;
				F_SpecialDisFour(0,0,0,NumTable[l_u8i+1]);
			}
			else
			{
				if(g_bScreenInitFlg && g_u8DisFliterTime)
				{   
					F_SpecialDisFour(0,0,N_E,N_E);
				}
				else{
					switch(g_u8RGBDispType)		//四位数码管显示数据+对应单位、指示灯点亮
					{
						case C_DISPTYPE_CH2O:
							F_DispCH2O();
							break;
						case C_DISPTYPE_PM25:
							F_DispPM25();
							break;
						case C_DISPTYPE_VOC:
							F_DispVOC();
							break;
						default:
							break;
					}
				}
			}
			if (g_u8DisTypeSwitchFlag) {
				F_DispTemp(); // 显示室温
			} else {
				F_DispHumi(); // 显示湿度
			}
			if(g_bDLZEn)
			{
				DIS_ION();
			}
			if(g_bWaterBox && !g_bWaterShortage)
			{
				DIS_WATERBOX();
			}
			else if(g_bWaterBox && g_bWaterShortage)
			{
				DIS_WATER();
			}
		}
	}
}
/**************************************************
*函数名称：void  F_ReadErrState(void) 
*函数功能：读错误状态
*入口参数：void
*出口参数：1表示有故障  
**************************************************/
U8 F_ReadErrState()
{
    if(g_bQDErr || g_bDoorOpenLeft || g_bDoorOpenRight)
    {   return 1;}
//    else if(g_bUartErrFlg)
//    {   return 1;}   
    return 0;
}
/**************************************************
*函数名称：void  F_DispErrState(void) 
*函数功能：显示错误状态
*入口参数：void
*出口参数：void
**************************************************/
void F_DispErrState()
{
	if(F_Blink1s()){
    if(g_bQDErr)
    {   F_SpecialDis(0,N_E,N_1);}
    else if(g_bDoorOpenLeft || g_bDoorOpenRight)
    {   F_SpecialDis(0,N_E,N_0);}
	}
//    else if(g_bUartErrFlg)
//    {   F_SpecialDis(0,N_E,N_C);}
}
/**************************************************
*函数名称：void  F_DispLed(void) 
*函数功能：LED显示
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispLed()
{
    static U8 s_u8VocDispStateBak = 0;
    static U8 s_u8PM25DispStateBak = 0;
	static U8 s_u8CH2ODispStateBak = 0;
        
    if(g_bTurnOn)
    {
		if(F_ReadErrState())
        {   return;}
		if(MODE_SLEEP == g_u8WorkMode)		//如果是睡眠模式，关闭其他指示灯，睡眠指示灯及睡眠按键半亮
		{				
			DIS_MODE_KEY();
			DIS_MODE_SLEEP();
		}
		else
		{
			DIS_LOCK_KEY();	
			DIS_MODE_KEY();	
			DIS_WIFI_KEY();	
			DIS_ION_KEY();	
			DIS_POWER_KEY();	
			DIS_TIME_KEY();	
		} 
        if(g_bLockEn)
        {
			if(g_u8LockBlinkTime)
			{
				if((g_u8LockBlinkTime % 10) >= 5)
				{   DIS_LOCK();}
			}
			else
			{   DIS_LOCK();}
        }

		if(g_u8TimeHBlinkTime)
		{
			if((g_u8TimeHBlinkTime % 10) >= 5)
			{   F_DispTime();}		//显示数码管及符号
		}
		else if(MODE_SLEEP != g_u8WorkMode)
		{   F_DispTime();}    
				
        if(MODE_HIGH == g_u8WorkMode)
        {   DIS_MODE_HI();}
				else if(MODE_MID == g_u8WorkMode)
        {   DIS_MODE_MID();}
				else if(MODE_LOW == g_u8WorkMode)
        {   DIS_MODE_LOW();}
        else if(MODE_AI == g_u8WorkMode)
        {   DIS_MODE_AI();}
        else if(MODE_SLEEP == g_u8WorkMode)
        {   
			DIS_MODE_SLEEP();
		}

		s_u8PM25DispStateBak = 0;
		g_u8PM25DispState = g_u8PM25AirQuality + 1;

		s_u8VocDispStateBak = 0;
		g_u8VocDispState = g_u8VocAirQuality + 1;

		s_u8CH2ODispStateBak = 0;
		g_u8CH2ODispState = g_u8CH2OAirQuality + 1;

        F_WifiLedProc();
		if(F_Blink500ms())
		{
			if(g_bScreenInitFlg)
			{
				DIS_FILTER();
			}
		}
    }
    else
    {  
        s_u8VocDispStateBak = 0;
        s_u8PM25DispStateBak = 0;
		s_u8CH2ODispStateBak = 0;
		DIS_POWER_KEY();
		if(g_u8TimeHBlinkTime)
		{
			if((g_u8TimeHBlinkTime % 10) >= 5)
			{   F_DispTime();}		//显示数码管及符号
		}
		else if(g_u8TimeData)			//关机，闪烁完毕，且定时不为零
		{   F_DispTime();}
//		F_WifiLedProc();
    }
    
}
/**************************************************
*函数名称：void  F_DispTime(void) 
*函数功能：显示时间
*入口参数：void
*出口参数：void  
**************************************************/
//void F_DispTime()
//{
//	DispBuf[COM_1] |= NumTable[g_u8TimeData / 10];
//	DispBuf[COM_2] |= NumTable[g_u8TimeData % 10];
//}
/**************************************************
*函数名称：void  F_DispKey(void) 
*函数功能：显示Key
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispKey()
{
	F_DispFourNum(g_u16KeyState);
}
//--------------------------不需修改-----------------------------------------
/**************************************************
*函数名称：void  F_DispPowerUp(void) 
*函数功能：全显
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispPowerUp()
{
	F_SetStr(&DispBuf,0xff,TOTAL_COM);
    if(g_u8DispLastT >= 15)
    {   g_u8VocDispState = g_u8PM25DispState = g_u8CH2ODispState = C_DISP_RED;}
    else if(g_u8DispLastT >= 10)
    {   g_u8VocDispState = g_u8PM25DispState = g_u8CH2ODispState = C_DISP_YELLOW;}
    else if(g_u8DispLastT >= 5)
    {   g_u8VocDispState = g_u8PM25DispState = g_u8CH2ODispState = C_DISP_BLUE;}
    else
    {   g_u8VocDispState = g_u8PM25DispState = g_u8CH2ODispState = C_DISP_GREEN;}
}
/**************************************************
*函数名称：void  F_DispClean(void) 
*函数功能：清除
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispClean()
{
	F_SetStr(&DispBuf,0,TOTAL_COM);
    g_u8VocDispState = 0;
    g_u8PM25DispState = 0;
}
/**************************************************
*函数名称：void  F_mDispInital(void) 
*函数功能：显示初始
*入口参数：void
*出口参数：void  
**************************************************/
void F_mDispInital()
{
	F_DispTskChange(DIPS_POWERUP);
}
/**************************************************
*函数名称：void  F_GetDispType(void) 
*函数功能：获取当前显示状态
*入口参数：void
*出口参数：void  
**************************************************/
U8 F_GetDispType()
{
	return DispTskTbl[g_u8DispIndex].Index;
}
/**************************************************
*函数名称：void  F_DispTskChange(void) 
*函数功能：显示事件切换函数
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispTskChange(unsigned char temp)
{
	unsigned char i;
	for(i = 0; i < DISP_TOTAL_TASK; i ++)
	{
		if(DispTskTbl[i].Index == temp)
		{
			g_u8DispIndex = i;
			g_u8DispLastT = DispTskTbl[i].LastTime;
			g_u8DispBlink = DispTskTbl[i].Blink;
			g_bDispMask = 0;
			return;
		}
	}
}
//------------------------------------------------------------------
/******************************************************************
*函数名称：void  F_UpdateDispBuff(void) 
*函数功能：最终Buf数据刷新
*入口参数：void
*出口参数：void  
*******************************************************************/
void F_UpdateDispBuff()
{   
	U8 l_u8cnt = 0;
	for(l_u8cnt = 0;l_u8cnt < TOTAL_COM;l_u8cnt++)
	{
		ChipDispBuf[l_u8cnt] = DispBuf[l_u8cnt];
	}
}
/**************************************************
*函数名称：void  F_DisplayUpData(void) 
*函数功能：显示事件处理函数
*入口参数：void
*出口参数：void  
**************************************************/
void F_DisplayUpData()
{
    if(g_bTurnOn && ((LIGHT_OFF == g_u8LightState) || (LIGHT_HALF == g_u8LightState)) && !F_ReadErrState())
    {   
        if(g_u8KeyDispTime)g_u8KeyDispTime--;
    }
    else
    {   g_u8KeyDispTime = C_KEY_DISP_TIME;}
    
    if((MODE_SLEEP == g_u8WorkMode) && g_bTurnOn )
    {   
				g_bHalfDispEn = 1;
    }
    else
    {   
		if(!g_bTurnOn)		//待机状态，开关键半亮显示
        {   g_bHalfDispEn = 1;}
        else
        {   g_bHalfDispEn = 0;}
    }

	if(g_u8PowerOnDispTime)
	{
		g_u8PowerOnDispTime--;
	}
	
	if(g_bTurnOn) {
		if (g_u8DisTypeSwitchTime == 0) {
			g_u8DisTypeSwitchFlag = !g_u8DisTypeSwitchFlag; // 切换标志位（0/1交替）
			g_u8DisTypeSwitchTime = 50; // 重置为5秒（50×100ms）

		} 
		else 
		{
			g_u8DisTypeSwitchTime--; // 每100ms递减
		}
	}
					
    
    if(g_bLockEn)
    {
        if(g_u8LockBlinkTime)
        {   g_u8LockBlinkTime--;}
    }
    else
    {   g_u8LockBlinkTime = 0;}
		
		
	if(g_u8TimeHBlinkTime)
	{   
		g_u8TimeHBlinkTime--;
	}
	
	
	if(g_bScreenInitFlg && g_bTurnOn)			//需更换滤网
	{
		g_u8DisFliterSwitchTime--;
		if(!g_u8DisFliterSwitchTime)			//每10秒显示一次
		{
			g_u8DisFliterTime = 10;			//显示1秒
//			g_bVoiceFilterEn = 1;
			g_u8DisFliterSwitchTime = 100;   //重置
		}
	}
	else
	{
		g_u8DisFliterSwitchTime = 100;   //重置
	}
		
	if(g_u8DisFliterTime)
	{
		g_u8DisFliterTime--;
	}
		
		
	if(g_u8RGBDispMode == C_RGB_DISMODE_MANUAL)
	{
        if(g_u8RGBManualDispTime)
        {
            g_u8RGBManualDispTime--;
        }
        if(!g_u8RGBManualDispTime)
        {
			g_u8RGBDispMode = C_RGB_DISMODE_AUTO;
			g_u8RGBManualDispTime = 0;
			g_u8RGBDispType = C_DISPTYPE_CH2O;         // 明确要求按固定顺序开始
			g_u8RGBAutoDispTime = C_RGB_AUTO_DISP_TIME;// 5s
        }
    }
    else  // 自动模式：按 5s 节拍轮播 CH2O -> PM25 -> VOC -> CH2O ...
    {
        if(g_u8RGBAutoDispTime)
        {
            g_u8RGBAutoDispTime--;
        }
        else
        {
			if(++g_u8RGBDispType > C_DISPTYPE_VOC)
			{
				g_u8RGBDispType = C_DISPTYPE_CH2O;
			}
            g_u8RGBAutoDispTime = C_RGB_AUTO_DISP_TIME; // 5s
        }
    }
    
/***************************************************************************************/
	if(g_u8DispLastT)
	{
		g_u8DispLastT --;
		if(g_u8DispLastT == 0)
		{
			F_DispTskChange(DIPS_NORMAL);
		}
	}
	if(g_u8DispBlink)
	{
		g_u8DispBlink --;
		if(g_u8DispBlink == 0)
		{
			g_u8DispBlink = DispTskTbl[g_u8DispIndex].Blink;
			g_bDispMask = !g_bDispMask;
		}
	}
    
	F_DispClean();
    
	F_DispLed();
    
    
    if(F_ReadErrState())
    {   
        F_DispErrState();
    }
    else if(g_bDispMask == 0)
	{
		(DispTskTbl[g_u8DispIndex].DispFunc)();
	}
}





code TYPE_DISP_TASK DispTskTbl[DISP_TOTAL_TASK] = 
{
	// Index,		LastTime,	Blink *100ms,		Function
	{DIPS_NORMAL, 		0,		0,			&F_DispNormal	},
	{DIPS_FAN, 		    50,		0,			&F_DispNormal	    },
	{DIPS_TIME, 	    50,		0,			&F_DispNormal	    },
    
	{DIPS_POWERUP, 		22,		0,			&F_DispPowerUp	},
};

void F_mDisplayControl(void)
{
	if(F_GetTimeTick(C_TIME_10ms) && !g_u8TestState)
	{
		 F_RgbDispDrv();
	}
	if(F_GetTimeTick(C_TIME_100ms))
	{
		//显示数据刷新
        if(g_u8TestState)
        {   
            F_DispClean();
            F_SelfTestDislayUpData();
        }
        else
        {   F_DisplayUpData();}
        F_UpdateDispBuff();

        if(g_bHalfDispEn && (F_GetDispType() != DIPS_POWERUP))
        {   g_u8DisBright = 2;}
        else
        {   g_u8DisBright = 7;}
        if(++g_u8GobalBlink >= 250)g_u8GobalBlink = 0;
	}
}
