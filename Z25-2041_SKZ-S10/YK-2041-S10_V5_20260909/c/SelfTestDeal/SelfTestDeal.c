#include "h\Allinclude.h"

U8 xdata g_u8TestState = 0;

/**************************************************
*函数名称：void  F_SelfTestKey(void) 
*函数功能：自检按键
*入口参数：void
*出口参数：void  
**************************************************/
void F_SelfTestKey()
{
	if(C_SelfTest == g_u8TestState)
	{	
		g_u8TestState = C_TestOff;
		BUZ_POWERON();
	}
	else if(g_u16MCURunTime <= 15)
	{	
        g_u8TestState = C_SelfTest;
        F_mSystemInital();
        g_stSys.bEepromWriteBan = 1;
        if(g_stSys.bEepromEn)
        {   F_EepromWriteCtrl();}
		g_stSys.bTurnOn = 1;
		S_API_AP_Tuch();
        BUZ_POWERON();
    }
	else
	{	
		return;
	}
	F_DispTskChange(DIPS_NORMAL);
}



/**************************************************
* 函数名称：F_SelfTestDislay
* 函数功能：自检模式专用显示调度 (100ms周期调用)
* 说明：采用独立的强时序状态机，替代常规显示任务
**************************************************/
void F_SelfTestDislay(void)
{
    static U16 s_u16TestTick = 0;   // 100ms 滴答计时器
	
	// --- 灯珠步进扫描状态机变量 ---
    static U8 s_u8ScanPhase = 0;    // 0:扫点阵, 1:扫指示灯, 2:扫按键灯, 3:全亮完成
    static U8 s_u8ScanIdx1 = 0;     // 扫描大索引 (列号 或 图标号)
    static U8 s_u8ScanIdx2 = 0;     // 扫描小索引 (行号 或 灯珠号)
    
    U8 phyCom, phySeg;
    U8 u8MaxBright = 255;           // 自检模式默认使用最大亮度 100%

    // 1. 每帧必须做：清空上一帧的显存
    F_DispClean();

    // 2. 维护自检时间轴
    if(s_u16TestTick < 0xFFFF) s_u16TestTick++;
	
    // ==========================================
    // 优先级 2=1: 第 1 秒强制显示版本号
    // ==========================================
    if (s_u16TestTick <= 10) // 0~1秒 (10 个 100ms)
    {
        // 我们底层的 F_TMS_ShowNum 会自动对个位数补 0，传入 1 就会显示 "01"
        F_TMS_ShowNum(&g_Area_Number, 1); 
        return;
    }
	
    // ==========================================
    // 优先级 2: 按键按下响应 (最高优先级)
    // ==========================================
    if (g_u8KeyData != 0)
    {
        // 只要有按键，中断所有的扫描和报错，直接全亮度显示键值
        F_TMS_ShowNum(&g_Area_Number, g_u8KeyData);
        return; 
    }



    // ==========================================
    // 优先级 3: 致命故障拦截 (定格显示)
    // ==========================================
    // 假设外部有 g_bSnCodeError 这个全局变量，拦截 Sn 码故障
    if (g_bSnCodeError) 
    {
        F_TMS_ShowErrChars(CH_S, CH_n);
        return;
    }
    // 拦截温湿度故障
    else if (g_stSys.bHumiErr)
    {
        F_TMS_ShowErrChars(CH_F, CH_H);
        return;
    }
	else if (g_stSys.bTankRemoved)
    {
		F_TMS_SetLed(L_AIR_DRY, 1); 
		return;
    }
	else if ( g_stSys.bWaterLack)
    {
        // 使用最高亮度强行点亮缺水灯
		F_TMS_SetLed(L_WATER_LACK, 1); 
		return;
    }


// --- 扫描状态机推进 ---

    // 阶段 0：扫描 9x9 点阵区域 (共 81 颗灯珠)
    if (s_u8ScanPhase == 0) 
    {
        // 调用底层接口，每次只点亮当前列的当前行的一颗灯珠
        F_TMS_Mapping_Write_Ex(&g_Area_Icon9x9, s_u8ScanIdx1, (1 << s_u8ScanIdx2), 10);
        
        if (++s_u8ScanIdx2 >= 9) // 扫完一列的9行
        {
            s_u8ScanIdx2 = 0;
            if (++s_u8ScanIdx1 >= 9) // 扫完9列
            {
                s_u8ScanPhase = 1; // 切入下一阶段
                s_u8ScanIdx1 = 4;  // 指示灯在枚举中从 L_TIME (4) 开始
                s_u8ScanIdx2 = 0;
            }
        }
    }
    
    // 阶段 1：扫描各个指示灯图标 (L_TIME 到 L_HUMI_PERCENT，枚举值 4~9)
    else if (s_u8ScanPhase == 1)
    {
        // 精准获取当前图标的第 N 颗物理灯珠的 COM 和 SEG
        phyCom = t_stLedMap[s_u8ScanIdx1].pList[s_u8ScanIdx2].u8Com;
        phySeg = t_stLedMap[s_u8ScanIdx1].pList[s_u8ScanIdx2].u8Seg;
        
        if (phyCom < 9 && phySeg < 16) 
            g_t_u8TMS6644_DispBuf[phyCom][phySeg] = u8MaxBright;

        if (++s_u8ScanIdx2 >= t_stLedMap[s_u8ScanIdx1].u8Count) // 扫完当前图标的所有灯珠
        {
            s_u8ScanIdx2 = 0;
            if (++s_u8ScanIdx1 > 9) // 扫完所有图标
            {
                s_u8ScanPhase = 2; // 切入下一阶段
                s_u8ScanIdx1 = 0;  // 按键灯在枚举中从 L_LIGHT_KEY (0) 开始
                s_u8ScanIdx2 = 0;
            }
        }
    }
    
    // 阶段 2：扫描各个按键灯 (L_LIGHT_KEY 到 L_POWER_KEY，枚举值 0~3)
    else if (s_u8ScanPhase == 2)
    {
        // 精准获取当前按键的第 N 颗物理灯珠的 COM 和 SEG
        phyCom = t_stLedMap[s_u8ScanIdx1].pList[s_u8ScanIdx2].u8Com;
        phySeg = t_stLedMap[s_u8ScanIdx1].pList[s_u8ScanIdx2].u8Seg;
        
        if (phyCom < 9 && phySeg < 16) 
            g_t_u8TMS6644_DispBuf[phyCom][phySeg] = u8MaxBright;

        if (++s_u8ScanIdx2 >= t_stLedMap[s_u8ScanIdx1].u8Count) // 扫完当前按键的所有灯珠
        {
            s_u8ScanIdx2 = 0;
            if (++s_u8ScanIdx1 > 3) // 扫完所有按键
            {
                s_u8ScanPhase = 3; // 扫描全部结束，进入常亮状态
            }
        }
    }
    
    // 阶段 3：扫描完毕，全屏点亮轰机
    else if (s_u8ScanPhase == 3)
    {
        U8 i, j;
        for(i = 0; i < 9; i++)
        {
            for(j = 0; j < 16; j++) 
            {
                g_t_u8TMS6644_DispBuf[i][j] = u8MaxBright;
            }
        }
        if (++s_u8ScanIdx1 >= 80) 
        {
            s_u8ScanPhase = 4; // 8秒时间到，切入结束阶段
        }
    }
    
    // 阶段 4：自检流程全部结束，安全退出
    else if (s_u8ScanPhase == 4)
    {
        // 1. 清除快检标志位，解除自检锁定
        g_u8TestState = 0;    
        BUZ_POWERON();
        // 2. 释放屏幕硬控权，任务切回常规模式 (关机状态下底层会自动息屏并保留电源键半亮)
        F_DispTskChange(DIPS_NORMAL); 
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
}

/**************************************************
*函数名称：void  F_SelfTestOut(void) 
*函数功能：自检输出 (100ms周期)
*出口参数：void  
**************************************************/
void F_SelfTestOut(void)
{
    static U8 s_u8TimeCnt = 0;       // 100ms 滴答累加
    static U8 s_u8SelfCheckStep = 0; // 自检步进阶段 (0~3)

    // 1. 维护 2 秒的时间轴 (20 个 100ms)
    if(++s_u8TimeCnt >= 20)
    {
        s_u8TimeCnt = 0;
        if(++s_u8SelfCheckStep > 3)
        {   
            s_u8SelfCheckStep = 0;
        }   
    }
    
    // 2. 测试项 K: 负离子每 2 秒切换开关
    if(s_u8SelfCheckStep % 2 == 0)
    {
        P_FLZ = 0; 
    }
    else
    {
        P_FLZ = 1; 
    }

    // 3. 测试项 L & F: 风机每 2 秒切换档位，RGB 依次变色
    switch(s_u8SelfCheckStep)
    {
        case 0: // 停机，RGB 灭
            g_stSys.u8FanSpeed = SPEED_STOP;
            g_stSys.u8ColorIdx = 0; 
            break;
            
        case 1: // 1档，RGB 颜色1 (橘/红)
            g_stSys.u8FanSpeed = SPEED_GEAR_1;
            g_stSys.u8ColorIdx = 1;
            break;
            
        case 2: // 3档，RGB 颜色2 (白/绿)
            g_stSys.u8FanSpeed = SPEED_GEAR_3;
            g_stSys.u8ColorIdx = 2;
            break;
            
        case 3: // 最高档，RGB 颜色3 (黄/蓝)
            g_stSys.u8FanSpeed = SPEED_GEAR_4;
            g_stSys.u8ColorIdx = 3;
            break;
    }
    F_FanUpdate(); 
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
    }
}