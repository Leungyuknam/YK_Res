#include "h\Allinclude.h"

U8 xdata g_u8TestState = 0;

U8 code t_SEGTable[8] = {c_SEGA,c_SEGB,c_SEGC,c_SEGD,c_SEGE,c_SEGF,c_SEGG,c_SEGDP};

bit g_bShrinkTimeFlg = 0; //缩时标记


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
/*****************************************************************************
* 函数名称：void F_SelfTestKey(void) 
* 函数功能：内部自检模式 - 触发与进入逻辑
* 功能说明：在开机的前10秒内，通过特定的按键组合触发进入产线自检模式。
* 进入后会初始化系统、关闭常规显示、屏蔽EEPROM正常写入，并触发WiFi测试。
******************************************************************************/
void F_SelfTestKey()
{
    // 如果已经在自检模式中，再次触发则退出自检
    if(C_SelfTest == g_u8TestState)
    {   
        g_u8TestState = C_TestOff;
    }
    // 只能在 MCU 刚运行的 10 秒内进入自检
    else if(g_u8MCURunTime <= 10)
    {   
        g_u8TestState = C_SelfTest;     // 设置状态为自检模式
        F_mSystemInital();              // 系统参数初始化
        F_RgbTestDispDrv(C_DISP_OFF);   // 熄灭RGB氛围灯
        
        g_bEepromWriteBan = 1;          // 禁止常规的EEPROM写入(保护用户数据)
        if(g_bEepromEn)
        {   F_EepromWriteCtrl();}
        
        // 触发 WiFi 产线测试指令
        TuyaWifiTask(WIFI_CMD_TEST);
        g_bWifiWorkStart = 1;           // 启动WiFi工作标记
        g_bHalfDispEn = 0;              // 关闭半亮显示模式
    }
    else
    {   return;}
    
    F_DispTskChange(DIPS_NORMAL);       // 切换显示任务为正常状态
    F_BuzzerSet(BUZ_SHORT);             // 蜂鸣器短鸣提示进入/退出自检
}

/**************************************************
*函数名称：void  F_SelfTestDislay(void) 
*函数功能：内部自检显示 (按视觉区域顺序进行扫灯)
*调用时间：100ms
**************************************************/
void F_SelfTestDislay(void)
{
    static U16 s_u16TimeCnt = 0;   // 整体自检时间轴
    static U16 s_u16SweepCnt = 0;  // 跑马灯计时器
    static U8 s_u8LastTestState = 0; 
    U8 logical_idx, bit_idx, physical_com, rgb_step;

    // 【核心优化】：按屏幕视觉区域整理的 COM 扫描顺序表
    static U8 code t_ScanSeq[TOTAL_COM] = {
        COM_7, COM_8, COM_9, COM_10,  // 区域1：中间 4 位大数码管 (主显示区)
        COM_1, COM_2,                 // 区域2：左上角 2 位数码管 (温湿度)
        COM_3, COM_4,                 // 区域3：右上角 2 位数码管 (定时)
        COM_5, COM_6,                 // 区域4：中间及上半部分散落的图标
        COM_11, COM_12                // 区域5：底部触摸按键及右侧图标
    };

    // --- 状态重置机制 ---
    if(s_u8LastTestState != g_u8TestState)
    {
        s_u8LastTestState = g_u8TestState;
        s_u16TimeCnt = 0;
        s_u16SweepCnt = 0;
    }

    s_u16TimeCnt++;

    // 扫灯 12 秒后重置时间轴 (15秒前置 + 12秒扫灯 = 27秒 = 270 counts)
    if(s_u16TimeCnt > 270)
    {
        s_u16TimeCnt = 0;
        s_u16SweepCnt = 0;
    }

    // ==========================================
    // 优先级 1：按键反馈测试 
    // ==========================================
    if(g_u16KeyState)
    {   
        F_DispKey();
        return; 
    }
    
    // ==========================================
    // 优先级 2：定序时间轴显示
    // ==========================================
    else if(s_u16TimeCnt <= 20)      // 前 2 秒：显示机型代码 U 0 2
    {   F_SpecialDis(N_U,N_0,N_2);}
    
    else if(s_u16TimeCnt <= 100)     // 第 2~10 秒：WiFi 测试结果
    {
        DIS_WIFI();
        if(WIFI_TEST_Ok == g_u8WifiTestState)
        {   F_SpecialDis(0,N_P,N_A);} 
        else
        {   F_SpecialDis(0,N_F,N_A);} 
    }
    
    // ==========================================
    // 优先级 3：传感器报错拦截
    // ==========================================
    else if(g_bQDErr)          { F_SpecialDis(0,N_P,N_4); return;} 
    else if(g_bUartErr)        { F_SpecialDis(0,N_F,N_7); return;} 
    else if(g_bDoorOpenLeft)   { F_SpecialDis(0,N_P,N_3); return;} 
    else if(g_bDoorOpenRight)  { F_SpecialDis(0,N_P,N_2); return;} 
    else if(g_bInFanErr)       { F_SpecialDis(0,N_F,N_5); return;} 
    else if(!g_u8CH2OLinkTime) { F_SpecialDis(0,N_F,N_E); return;} 
    else if(!g_u8StiveState)   { F_SpecialDis(0,N_F,N_U); return;} 
    else if(!g_u8VocLinkTime)  { F_SpecialDis(0,N_F,N_1); return;} 

    // ==========================================
    // 优先级 4：屏幕与氛围灯硬件排查
    // ==========================================
    else if(s_u16TimeCnt <= 150)    // 第 10~15 秒：全屏全显
    {
        F_SetStr(&DispBuf, 0xFF, TOTAL_COM);
        F_RgbTestDispDrv(C_DISP_OFF);
    }
    else                            // 第 15~27 秒：按区域逐点扫描
    {
        s_u16SweepCnt++;
        
        // 计算逻辑索引：不再直接对应显存，而是对应 t_ScanSeq 数组的下标
        logical_idx = (s_u16SweepCnt / 1) / 8; 
        bit_idx = (s_u16SweepCnt / 1) % 8;
        
        if (logical_idx >= TOTAL_COM) 
        {
            s_u16SweepCnt = 0; 
            logical_idx = 0;
            bit_idx = 0;
        }

        // 查表获取真正的显存物理地址
        physical_com = t_ScanSeq[logical_idx];

        // 清空显存，点亮对应的物理地址
        F_SetStr(&DispBuf, 0x00, TOTAL_COM); 
        DispBuf[physical_com] = (0x01 << bit_idx);

        // RGB 灯带循环切换
        rgb_step = ((s_u16TimeCnt - 150) / 10) % 4; 
        if(rgb_step == 0)      F_RgbTestDispDrv(C_DISP_RED);
        else if(rgb_step == 1) F_RgbTestDispDrv(C_DISP_GREEN);
        else if(rgb_step == 2) F_RgbTestDispDrv(C_DISP_BLUE);
        else                   F_RgbTestDispDrv(C_DISP_OFF);
    }
}



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


/*****************************************************************************
* 函数名称：void F_SelfTestOut(void) 
* 函数功能：内部自检模式 - 负载驱动控制
* 调用时间：100ms
* 功能说明：按固定节拍强制开启各项底层硬件（负离子、电机等）。
* 特别注意：风机严禁停转（防 FG 测速误报），采用 4 档风速无限循环。
******************************************************************************/
void F_SelfTestOut(void)
{
    static U8 s_u8TimeCnt = 0;         // 节拍计时器
    static U8 s_u8SelfCheckCnt = 0;    // 负载动作步进状态 (每2秒加1)
    static U8 s_u8LastTestState = 0;
    
    // 风速循环表（睡眠->低->中->高），绝对不包含 0
    static U8 code t_TestFanSpeed[4] = {FAN_SLEEP, FAN_LOW, FAN_MID, FAN_HI};

    // --- 状态重置机制 ---
    if(s_u8LastTestState != g_u8TestState)
    {
        s_u8LastTestState = g_u8TestState;
        s_u8TimeCnt = 0;
        s_u8SelfCheckCnt = 0;
    }
    
    // 定时轮询步进 (每 2 秒切一个动作阶段)
    if(++s_u8TimeCnt > 20)
    {
        s_u8TimeCnt = 0;
        if(++s_u8SelfCheckCnt > 5)
        {   s_u8SelfCheckCnt = 0;}  
    }
    
    // 1. 强制开启所有传感器供电
    g_bSenseLoad = 1;
    
    // 2. 【核心优化】：负离子发生器测试 (每 2 秒切换一次开关)
    // 利用步进变量的奇偶性：0,2,4开启；1,3,5关闭，完美实现2秒交替
    if((s_u8SelfCheckCnt % 2) == 0)
    {   g_bIonLoad = 1;}
    else
    {   g_bIonLoad = 0;}
    
    // 3. 风机测试阶段 (按步进状态查表切换风速，保持电机常开测 FG)
    g_u8FanSpeed = t_TestFanSpeed[s_u8SelfCheckCnt % 4];
    
    // 统一调用底层驱动更新硬件状态
    F_LoadDry();
}


/*****************************************************************************
* 函数名称：void F_QuickTestOut(void) 
* 函数功能：客户快检模式 - 负载驱动控制
* 调用时间：100ms
******************************************************************************/
void F_QuickTestOut()
{
    static U8 s_u8TimeCnt = 0;
    static U8 s_u8SelfCheckCnt = 0;
    
    // 同样使用风速循环表，确保验机过程风机不停
    static U8 code t_TestFanSpeed[4] = {FAN_SLEEP, FAN_LOW, FAN_MID, FAN_HI};
    
    if(++s_u8TimeCnt > 20)
    {
        s_u8TimeCnt = 0;
        if(++s_u8SelfCheckCnt > 5)
        {   s_u8SelfCheckCnt = 0;}  
    }
    
    g_bSenseLoad = 1;
    
    // 负离子发生器测试 (每 2 秒切换一次开关)
    if((s_u8SelfCheckCnt % 2) == 0)
    {   g_bIonLoad = 1;}
    else
    {   g_bIonLoad = 0;}
    
    // 直接查表循环切风速
    g_u8FanSpeed = t_TestFanSpeed[s_u8SelfCheckCnt % 4];
    
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