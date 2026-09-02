#include "h\Allinclude.h"

U8 xdata g_u8TestState = 0;

U8 code t_SEGTable[8] = {c_SEGA,c_SEGB,c_SEGC,c_SEGD,c_SEGE,c_SEGF,c_SEGG,c_SEGDP};

bit g_bShrinkTimeFlg = 0; //缩时标记

/**************************************************
*函数名称：void  F_SelfTestKey(void) 
*函数功能：自检按键
*入口参数：void
*出口参数：void  
**************************************************/
void F_SelfTestKey()
{
	if(C_SelfTest == g_u8TestState)
	{	g_u8TestState = C_TestOff;}
	else if(g_u8MCURunTime <= 10)
	{	
        g_u8TestState = C_SelfTest;
        F_mSystemInital();
				F_RgbTestDispDrv(C_DISP_OFF);
        g_bEepromWriteBan = 1;
        if(g_bEepromEn)
        {   F_EepromWriteCtrl();}
        TuyaWifiTask(WIFI_CMD_TEST);
				g_bWifiWorkStart = 1;
				g_bHalfDispEn = 0;
    }
	else
	{	return;}
	F_DispTskChange(DIPS_NORMAL);
	F_BuzzerSet(BUZ_SHORT);
}
/**************************************************
*函数名称：void  F_QuickTestKey(void) 
*函数功能：快检控制
*入口参数：void
*出口参数：void  
**************************************************/
void F_QuickTestKey()
{	
	if(C_Quick_Test == g_u8TestState)
	{	g_u8TestState = C_TestOff;}
	else if(g_u8MCURunTime <= 10)
	{	
        g_u8TestState = C_Quick_Test;
        F_mSystemInital();
        g_bEepromWriteBan = 1;

        if(g_bEepromEn)
        {   F_EepromWriteCtrl();}
        F_RgbTestDispDrv(C_DISP_OFF);
        g_bShrinkTimeFlg = 1;
				g_bHalfDispEn = 0;
    }
    F_BuzzerSet(BUZ_SHORT);
    F_DispTskChange(DIPS_NORMAL);
}
/**************************************************
*函数名称：void  F_SelfTestDislay(void) 
*函数功能：自检显示
*调用时间：100ms
*入口参数：void
*出口参数：void  
**************************************************/
void F_SelfTestDislay(void)
{
	static U8 s_u8TimeCnt = 0;
	static U8 s_u8ClientCheckCnt = 0;
	
		if(++s_u8TimeCnt > 5)
		{
			s_u8TimeCnt = 0;
			if(++s_u8ClientCheckCnt > 20)
					{   s_u8ClientCheckCnt = 0;}
		}
        
    if(g_u8KeyData)
    {   F_DispKey();}
    else if(s_u8ClientCheckCnt <= 1)
    {   F_SpecialDis(N_U,N_0,N_2);}
//    else if(s_u8ClientCheckCnt <= 3)
//    {   F_SpecialDis(N_U,NumTable[g_u8DispVersion / 10],NumTable[g_u8DispVersion % 10]);}
    else if(s_u8ClientCheckCnt <= 9)
    {
        DIS_WIFI();
        if(WIFI_TEST_Ok == g_u8WifiTestState)
        {   F_SpecialDis(0,N_P,N_A);}
        else
        {   F_SpecialDis(0,N_F,N_A);}
    }
    else if(g_bQDErr)
    {   F_SpecialDis(0,N_P,N_4);}
    else if(g_bDoorOpenLeft)
    {   F_SpecialDis(0,N_P,N_3);}
//    else if(g_bUartErrFlg)
//    {   F_SpecialDis(0,N_F,N_7);}
    else if(g_bInFanErr)
    {   F_SpecialDis(0,N_F,N_5);}
//    else if(!g_u8CH2OLinkTime)
//    {   F_SpecialDis(0,N_A,N_E);}
    else if(!g_u8StiveState)
    {   F_SpecialDis(0,N_F,N_U);}
    else if(!g_u8VocLinkTime)
    {   F_SpecialDis(0,N_F,N_1);}
    else if(s_u8ClientCheckCnt <= 17)
    {   
        F_SetStr(&DispBuf,t_SEGTable[s_u8ClientCheckCnt - 10],4);
        g_u8FilterDispState = g_u8PM25DispState = g_u8VocDispState = s_u8ClientCheckCnt % 5;
    }
    else
    {   F_DispPowerUp();}

}
/**************************************************
*函数名称：void  F_DispQuickTest(void) 
*函数功能：显示快检
*调用频率：100ms
*入口参数：void
*出口参数：void  
**************************************************/
//void F_DispQuickTest()
//{
//	static U8 s_u8TimeCnt = 0;
//	static U8 s_u8ClientCheckCnt = 0;
//	static U8 s_u8LedCnt = 0;
//	
//	if(++s_u8TimeCnt > 5)
//	{
//		s_u8TimeCnt = 0;
//		if(++s_u8ClientCheckCnt > 15)
//        {   s_u8ClientCheckCnt = 6;}
//        if((s_u8ClientCheckCnt >= 6) && (s_u8LedCnt < 0xff))
//        {   s_u8LedCnt++;}
//	}
//    
//    if(g_bInFanErr)
//    {   F_SpecialDis(0,N_F,N_5);}
//    else if(!g_u8VocLinkTime)
//    {   F_SpecialDis(0,N_F,N_4);}
//    else if(!g_u8StiveState)
//    {   F_SpecialDis(0,N_F,N_3);}
//		else if(!g_u8CH2OLinkTime)
//    {   F_SpecialDis(0,N_F,N_2);}
//    else if(s_u8ClientCheckCnt < 3)
//    {   F_SpecialDis(N_F,N_0,N_2);}
////    else if(s_u8ClientCheckCnt < 6)
////    {   F_SpecialDis(N_U,NumTable[g_u8DispVersion / 10],NumTable[g_u8DispVersion % 10]);}
//    else 
//    {
//        if(s_u8ClientCheckCnt <= 15)
//        {   F_SpecialDis(NumTable[s_u8ClientCheckCnt - 6],NumTable[s_u8ClientCheckCnt - 6],NumTable[s_u8ClientCheckCnt - 6]);}
//        switch(s_u8LedCnt)
//        {
//            case 1:
//                DIS_PM25();
//                break;
//            
//            case 2:
//                DIS_LOCK();
//                break;

//            case 3:
//                DIS_AI();
//                break;

//            case 4:
//                DIS_WIFI();
//                break;

//            case 5:
//                DIS_FILTER();
//                break;

//            case 6:
//                DIS_PET();
//                break;
//            
//            case 7:
//                DIS_SLEEP();
//                break;

//            case 8:
//                DIS_FAN();
//                break;

//            case 9:
//                g_u8VocDispState = g_u8PM25DispState = C_DISP_RED;
//                break;

//            case 10:
//                g_u8VocDispState = g_u8PM25DispState = C_DISP_GREEN;
//                break;

//            case 12:
//                g_u8VocDispState = g_u8PM25DispState = C_DISP_BLUE;
//                break;
//            
//            default:
//                break;
//        }
//    }
//}




void F_DispQuickTest()
{
	static U8 s_u8TimeCnt = 0;
	static U8 s_u8ClientCheckCnt = 0;
	static U8 s_u8LedCnt = 0;
	
	if(++s_u8TimeCnt > 10)
	{
		s_u8TimeCnt = 0;
		if(++s_u8ClientCheckCnt > 20)
        {   s_u8ClientCheckCnt = 0;}
        if((s_u8ClientCheckCnt >= 6) && (s_u8LedCnt < 0xff))
        {   s_u8LedCnt++;}
	}
    
    if(g_bInFanErr)
    {   F_SpecialDis(0,N_F,N_5);}
    if(!g_u8VocLinkTime)
    {   F_SpecialDis(0,N_F,N_4);}
    else if(!g_u8StiveState)
    {   F_SpecialDis(0,N_F,N_3);}
		else if(!g_u8CH2OLinkTime)
    {   F_SpecialDis(0,N_F,N_2);}
//    else if(s_u8ClientCheckCnt < 3)
//    {   F_SpecialDis(N_F,N_0,N_2);}
//    else if(s_u8ClientCheckCnt < 6)
//    {   F_SpecialDis(N_U,NumTable[g_u8DispVersion / 10],NumTable[g_u8DispVersion % 10]);}
    else 
    {
//		if(g_u16KeyState)
//		{   F_DispKey();}
        if(s_u8ClientCheckCnt < 1)
		{		DIS_POWER_KEY();}
		else if(s_u8ClientCheckCnt < 2)
		{		
			DIS_POWER_KEY();
			DIS_LOCK_KEY();
			DIS_MODE_KEY();
			DIS_ION_KEY();
			DIS_WIFI_KEY();
			DIS_TIME_KEY();
		}
		else if(s_u8ClientCheckCnt < 12)
		{		
			F_SpecialDisFour(
			NumTable[s_u8ClientCheckCnt-2],
			NumTable[s_u8ClientCheckCnt-2],
			NumTable[s_u8ClientCheckCnt-2],
			NumTable[s_u8ClientCheckCnt-2]);
	
			F_SpecialDisTime(
			NumTableTimeTens[s_u8ClientCheckCnt-2],
			NumTableTimeOnes[s_u8ClientCheckCnt-2]);
		
			F_SpecialDisTempHumi(
			NumTableTempHumi[s_u8ClientCheckCnt-2],
			NumTableTempHumi[s_u8ClientCheckCnt-2]);
		}
		else if(s_u8ClientCheckCnt < 13)
		{		F_SetStr(&DispBuf,0xff,TOTAL_COM);	}				
		else if(s_u8ClientCheckCnt < 14)
		{	F_SetStr(&DispBuf,0xff,TOTAL_COM);		
			F_RgbTestDispDrv(C_DISP_RED);	}
		else if(s_u8ClientCheckCnt < 15)
		{	F_SetStr(&DispBuf,0xff,TOTAL_COM);	
			F_RgbTestDispDrv(C_DISP_GREEN);	}
		else if(s_u8ClientCheckCnt < 16)
		{	F_SetStr(&DispBuf,0xff,TOTAL_COM);	
			F_RgbTestDispDrv(C_DISP_BLUE);	}
		else if(s_u8ClientCheckCnt < 17)
		{	F_RgbTestDispDrv(C_DISP_OFF);	}
		else if(s_u8ClientCheckCnt < 20)
		{		F_SpecialDisFour(0,N_F,N_1,N_0);	}

//        if(s_u8ClientCheckCnt <= 15)
//        {   F_SpecialDis(NumTable[s_u8ClientCheckCnt - 6],NumTable[s_u8ClientCheckCnt - 6],NumTable[s_u8ClientCheckCnt - 6]);}
//        switch(s_u8LedCnt)
//        {
//            case 1:
//                DIS_PM25();
//                break;
//            
//            case 2:
//                DIS_LOCK();
//                break;

//            case 3:
//                DIS_AI();
//                break;

//            case 4:
//                DIS_WIFI();
//                break;

//            case 5:
//                DIS_FILTER();
//                break;

//            case 6:
//                DIS_PET();
//                break;
//            
//            case 7:
//                DIS_SLEEP();
//                break;

//            case 8:
//                DIS_FAN();
//                break;

//            case 9:
//                g_u8VocDispState = g_u8PM25DispState = C_DISP_RED;
//                break;

//            case 10:
//                g_u8VocDispState = g_u8PM25DispState = C_DISP_GREEN;
//                break;

//            case 12:
//                g_u8VocDispState = g_u8PM25DispState = C_DISP_BLUE;
//                break;
//            
//            default:
//                break;
//        }
    }
}

/**************************************************
*函数名称：void  F_SelfTestDislayUpData(void) 
*函数功能：自检显示
*调用时间：100ms
*入口参数：void
*出口参数：void  
**************************************************/
void F_SelfTestDislayUpData(void)
{
    if(C_SelfTest == g_u8TestState)
    {   F_SelfTestDislay();}
    else if(C_Quick_Test == g_u8TestState)
    {   F_DispQuickTest();}
}

/**************************************************
*函数名称：void  F_SelfTestOut(void) 
*函数功能：自检输出
*调用时间：100ms
*入口参数：void
*出口参数：void  
**************************************************/
void F_SelfTestOut(void)
{
	static U8 s_u8TimeCnt = 0;
	static U8 s_u8SelfCheckCnt = 0;
	
	if(++s_u8TimeCnt > 10)
	{
		s_u8TimeCnt = 0;
		if(++s_u8SelfCheckCnt > 7)
		{	s_u8SelfCheckCnt = 0;}	
	}
    
    g_u8LoadState = 0x01 << s_u8SelfCheckCnt;
	g_bSenseLoad = 1;
    g_u8FanSpeed = FAN_MID;
    F_LoadDry();
}
/**************************************************
*函数名称：void  F_QuickTestOut(void) 
*函数功能：快检输出
*入口参数：void
*出口参数：void  
**************************************************/
void F_QuickTestOut()
{
	static U8 s_u8TimeCnt = 0;
	static U8 s_u8SelfCheckCnt = 0;
    
	if(++s_u8TimeCnt > 20)
	{
		s_u8TimeCnt = 0;
		if(++s_u8SelfCheckCnt > 5)
		{	s_u8SelfCheckCnt = 0;}	
	}
	g_bSenseLoad = 1;
    if(0 == s_u8SelfCheckCnt)
    {   g_bUvLoad = 1;}
    else
    {   g_bUvLoad = 0;}
    if(2 == s_u8SelfCheckCnt)
    {   g_bIonLoad = 1;}
    else
    {   g_bIonLoad = 0;}
    if(s_u8SelfCheckCnt >= 4)
    {   g_u8FanSpeed = 0;}
    else
    {   g_u8FanSpeed = FAN_MID;}
    F_LoadDry();
    
}
/**************************************************************
//Fuction Name: void F_mSelfTestSchdule(void)
//Desrition: 系统运行逻辑
//Input: 
//Output:
***************************************************************/ 
void F_mSelfTestSchdule(void)
{
    if(F_GetTimeTick(C_TIME_100ms))
	{
        if(C_SelfTest == g_u8TestState)
		{   F_SelfTestOut();}
        else if(C_Quick_Test == g_u8TestState)
        {   F_QuickTestOut();}
    }
}