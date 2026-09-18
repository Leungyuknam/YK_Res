#include "h\Allinclude.h"

U8 xdata g_u8TestState = 0;

// =================================================================
// 自检进入与退出
//   - 进入：上电前 15 秒内同时长按开机键和人体感应键 3 秒
//   - 退出：再次按相同组合键，或连续 60 秒无操作
// =================================================================
void F_SelfTestKey(void)
{
    if(g_u8TestState == C_SelfTest)
    {
        g_u8TestState = C_TestOff;
        F_mSystemInital();
        BUZ_KEY();
        F_DispTskChange(DIPS_NORMAL);
        return;
    }

    if(g_u8MCURunTime <= 15)
    {
        g_u8TestState = C_SelfTest;
        F_mSystemInital();
        BUZ_KEY();
    }
}

// =================================================================
// 自检显示（100 ms 节拍）
//   - 空闲：8 个指示灯流水显示（每灯 300 ms）
//   - 按键按下：点亮与所按按键对应的指示灯
//   - 传感器：倾倒状态显示在档位灯，人体状态显示在定时灯
// =================================================================
static void F_DispSelfTest(void)
{
    static U16 s_u16ExitCnt = 0;
    static U8  s_u8StepCnt  = 0;
    static U8  s_u8LedIdx   = 0;

    F_DispClean();

    // 始终显示传感器输入，便于操作人员检查。
    if(g_stSys.bIsTipOver)       
	{
		DIS_LOW();
		DIS_HIGH();
	}
    if(g_stSys.bIsHumanDetected) 
	{
		DIS_TIME_1H();
		DIS_TIME_2H();
		DIS_TIME_4H();		
	}

    // 按键反馈：任一按键按下时，点亮其对应指示灯。
    if(g_u8KeyData != 0)
    {
        s_u16ExitCnt = 0;

        if(g_u8KeyData & c_KEY_POWER) DIS_POWER();
        if(g_u8KeyData & c_KEY_GEAR)  DIS_LOW();
        if(g_u8KeyData & c_KEY_TIME)  DIS_TIME_1H();
        if(g_u8KeyData & c_KEY_HUMAN) DIS_HUMAN();
        if(g_u8KeyData & c_KEY_LOCK)  DIS_LOCK();
        return;
    }

    // 连续 60 秒无输入后自动退出
    if(++s_u16ExitCnt >= 600)
    {
        s_u16ExitCnt = 0;
        g_u8TestState = C_TestOff;
        F_mSystemInital();
        BUZ_KEY();
        F_DispTskChange(DIPS_NORMAL);
        return;
    }

    // 指示灯流水显示（每灯 300 ms，共 8 个）
    if(++s_u8StepCnt >= 3)
    {
        s_u8StepCnt = 0;
        if(++s_u8LedIdx >= 8) s_u8LedIdx = 0;
    }
    DispBuf[s_u8LedIdx / 4] |= (1 << (s_u8LedIdx % 4));
}

void F_SelfTestDislayUpData(void)
{
    if(g_u8TestState == C_SelfTest) F_DispSelfTest();
}

// =================================================================
// 自检负载循环（100 ms 节拍）
//   每步持续 2 秒，顺序如下：
//     0 - 全部关闭
//     1 - 仅开启低档 PTC
//     2 - 仅开启高档 PTC（单独检查 1200 W 加热器）
//     3 - 仅开启风机（检查风机/负离子驱动）
// =================================================================
void F_SelfTestOut(void)
{
    static U8 s_u8TimeCnt = 0;
    static U8 s_u8Step    = 0;

    if(++s_u8TimeCnt >= 20)
    {
        s_u8TimeCnt = 0;
        s_u8Step++;
        if(s_u8Step >= 4) s_u8Step = 0;
    }

    switch(s_u8Step)
    {
        case 0: P_PTC_LOW = PTC_LOAD_OFF; P_PTC_HI = PTC_LOAD_OFF; P_FAN = FAN_LOAD_OFF; break;
        case 1: P_PTC_LOW = PTC_LOAD_ON;  P_PTC_HI = PTC_LOAD_OFF; P_FAN = FAN_LOAD_OFF; break;
        case 2: P_PTC_LOW = PTC_LOAD_OFF; P_PTC_HI = PTC_LOAD_ON;  P_FAN = FAN_LOAD_OFF; break;
        case 3: P_PTC_LOW = PTC_LOAD_OFF; P_PTC_HI = PTC_LOAD_OFF; P_FAN = FAN_LOAD_ON;  break;
    }
}

void F_mSelfTestSchdule(void)
{
    if(F_GetTimeTick(C_TIME_100ms))
    {
        if(g_u8TestState == C_SelfTest) F_SelfTestOut();
    }
}
