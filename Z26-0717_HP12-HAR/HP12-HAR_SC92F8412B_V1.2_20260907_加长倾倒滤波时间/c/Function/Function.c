#include "h\Allinclude.h"

// =================================================================
// 系统数据初始化
// -----------------------------------------------------------------
// 童锁是唯一需要在复位后保留的字段。此处先保存原值，
// 清零系统状态后再恢复；实际 EEPROM 读写由下方函数完成。
// 上电时应在系统初始化后立即调用 F_MemoryRead()，
// 以 EEPROM 中的有效值覆盖 bChildLockEn。
// =================================================================
void F_mSystemInital(void)
{
    U8 u8LockBak = g_stSys.bChildLockEn;
    U8 *p = (U8*)&g_stSys;
    U16 i;

    for(i = 0; i < sizeof(ST_SYSTEM_DATA); i++) p[i] = 0;

    g_stSys.bChildLockEn = u8LockBak;
    g_stSys.u8WorkGear   = GEAR_LOW;        // 下次开机的默认档位

    // 强制负载引脚进入安全状态
    P_PTC_LOW = PTC_LOAD_OFF;
    P_PTC_HI  = PTC_LOAD_OFF;
    P_FAN     = FAN_LOAD_OFF;
}

// =================================================================
// 边沿触发辅助函数
// =================================================================

// 检测 bTurnOn 从 1 变为 0；若关机瞬间 PTC 仍在工作，
// 则启动 30 秒风机延时散热。
static void F_PowerEdge(void)
{
    static U8 s_u8LastTurnOn = 0;

    if(s_u8LastTurnOn != g_stSys.bTurnOn)
    {
        s_u8LastTurnOn = g_stSys.bTurnOn;

        if(!g_stSys.bTurnOn)
        {
            if(g_stSys.bOutLowPtc || g_stSys.bOutHiPtc)
                g_stSys.u8FanDelaySec = 30;
        }
    }
}

// 检测高档 PTC 从开启变为关闭，并启动 5 秒冷却锁定，
// 保证高档 PTC 再次通电前至少保持关闭 5 秒
// （即使快速切换低档和高档也同样生效）。
static void F_HiPtcCooldownEdge(void)
{
    static U8 s_u8LastHiPtc = 0;

    if(s_u8LastHiPtc && !g_stSys.bOutHiPtc)
        g_stSys.u8HiPtcCoolSec = 5;

    s_u8LastHiPtc = g_stSys.bOutHiPtc;
}

// =================================================================
// 输出仲裁
// =================================================================
// 优先级（从高到低）：
//   1. 倾倒保护     -> 所有负载立即关闭（风机不延时）
//   2. 正常关机     -> PTC 关闭，风机继续运行 u8FanDelaySec 秒
//   3. 人感待机     -> PTC 关闭，风机继续运行 u8FanDelaySec 秒
//   4. 正常工作     -> 按档位驱动 PTC 和风机
// 另外：u8HiPtcCoolSec > 0 时禁止开启高档 PTC
// =================================================================
static void F_LoadArbiter(void)
{
    U8 bWantLow = 0;
    U8 bWantHi  = 0;
    U8 bWantFan = 0;

    if(g_stSys.bIsTipOver || !g_stSys.bTurnOn || g_stSys.bHumanSenseStandby)
    {
        // PTC 关闭，风机由下方延时逻辑控制
        bWantLow = 0;
        bWantHi  = 0;
        bWantFan = 0;
    }
    else
    {
        if(g_stSys.u8WorkGear == GEAR_LOW)
        {
            bWantLow = 1; bWantHi = 0; bWantFan = 1;
        }
        else if(g_stSys.u8WorkGear == GEAR_HI)
        {
            bWantLow = 1; bWantHi = 1; bWantFan = 1;
        }
    }

    // 高档 PTC 冷却锁定
    if(bWantHi && g_stSys.u8HiPtcCoolSec > 0) bWantHi = 0;

    g_stSys.bOutLowPtc = bWantLow;
    g_stSys.bOutHiPtc  = bWantHi;

    // 风机：有运行请求，或延时关机仍未结束
    g_stSys.bOutFan = bWantFan || (g_stSys.u8FanDelaySec > 0);

    // 倾倒时风机必须立即关闭，不执行 30 秒延时
    if(g_stSys.bIsTipOver)
    {
        g_stSys.bOutFan       = 0;
        g_stSys.u8FanDelaySec = 0;
    }
}

static void F_LoadDrive(void)
{
    if(g_u8TestState == C_SelfTest) return;     // 自检模式直接控制这些引脚

    P_PTC_LOW = g_stSys.bOutLowPtc ? PTC_LOAD_ON : PTC_LOAD_OFF;
    P_PTC_HI  = g_stSys.bOutHiPtc  ? PTC_LOAD_ON : PTC_LOAD_OFF;
    P_FAN     = g_stSys.bOutFan    ? FAN_LOAD_ON : FAN_LOAD_OFF;
}

// =================================================================
// 1 秒周期事件
// =================================================================

// 倾倒状态边沿处理（每 100 ms 执行一次，以便立即关断
// 与负载仲裁保持同步）。
//   倾倒边沿：强制整机关机并清除所有非记忆状态。
//   复位边沿：蜂鸣提醒一次；整机仍保持关机，直到
//             再按一次开机键才重新启动。
static void F_TipOverEvent(void)
{
    static U8 s_u8LastTip = 0;

    if(s_u8LastTip == g_stSys.bIsTipOver) return;
    s_u8LastTip = g_stSys.bIsTipOver;

    if(g_stSys.bIsTipOver)
    {
        g_stSys.bTurnOn            = 0;
        g_stSys.bHumanSenseEn      = 0;
        g_stSys.bHumanSenseStandby = 0;
        g_stSys.u8WorkGear         = GEAR_LOW;
        g_stSys.u16TimerRemainMin  = 0;
        g_stSys.u8TimerSecCnt      = 0;
        g_stSys.u16NoPersonSec     = 0;
        g_stSys.u8FanDelaySec      = 0;     // 立即切断，不执行 30 秒延时
    }
    else
    {
        BUZ_TIPOK();                        // 扶正提醒
    }
}

// 人体感应：连续 2 分钟无人则待机；连续 12 小时无人则关机。
static void F_HumanSenseEvent(void)
{
    if(!g_stSys.bTurnOn || !g_stSys.bHumanSenseEn || g_stSys.bIsTipOver)
    {
        g_stSys.u16NoPersonSec     = 0;
        g_stSys.bHumanSenseStandby = 0;
        return;
    }

    if(g_stSys.bIsHumanDetected)
    {
        if(g_stSys.bHumanSenseStandby)
        {
            g_stSys.bHumanSenseStandby = 0;     // PIR 检测到人后静默唤醒
        }
        g_stSys.u16NoPersonSec = 0;
        return;
    }

    // 未检测到人员
    if(g_stSys.u16NoPersonSec < 0xFFFF) g_stSys.u16NoPersonSec++;

    if(!g_stSys.bHumanSenseStandby)
    {
        if(g_stSys.u16NoPersonSec >= 120U)      // 2 分钟
        {
            g_stSys.bHumanSenseStandby = 1;
            if(g_stSys.bOutLowPtc || g_stSys.bOutHiPtc)
                g_stSys.u8FanDelaySec = 30;
        }
    }
    else
    {
        // 连续 12 小时无人后彻底关机
        if(g_stSys.u16NoPersonSec >= (12U * 60U * 60U))
        {
            U8 bWasPtcRunning = (g_stSys.bOutLowPtc || g_stSys.bOutHiPtc);
            g_stSys.bTurnOn            = 0;
            g_stSys.bHumanSenseEn      = 0;
            g_stSys.bHumanSenseStandby = 0;
            g_stSys.u8WorkGear         = GEAR_LOW;
            g_stSys.u16TimerRemainMin  = 0;
            g_stSys.u8TimerSecCnt      = 0;
            g_stSys.u16NoPersonSec     = 0;
            if(bWasPtcRunning) g_stSys.u8FanDelaySec = 30;
        }
    }
}

// 1H/2H/4H 叠加定时倒计时，减至 0 时自动关机。
static void F_TimerEvent(void)
{
    if(!g_stSys.bTurnOn) { g_stSys.u8TimerSecCnt = 0; return; }
    if(g_stSys.u16TimerRemainMin == 0) return;

    if(++g_stSys.u8TimerSecCnt >= 60)
    {
        g_stSys.u8TimerSecCnt = 0;
        g_stSys.u16TimerRemainMin--;
        if(g_stSys.u16TimerRemainMin == 0)
        {
            U8 bWasPtcRunning = (g_stSys.bOutLowPtc || g_stSys.bOutHiPtc);
            g_stSys.bTurnOn            = 0;
            g_stSys.bHumanSenseEn      = 0;
            g_stSys.bHumanSenseStandby = 0;
            g_stSys.u8WorkGear         = GEAR_LOW;
            if(bWasPtcRunning) g_stSys.u8FanDelaySec = 30;
        }
    }
}

// 以 1 Hz 递减冷却锁定和风机延时秒数
static void F_DelayCounters(void)
{
    if(g_stSys.u8HiPtcCoolSec > 0) g_stSys.u8HiPtcCoolSec--;
    if(g_stSys.u8FanDelaySec > 0)  g_stSys.u8FanDelaySec--;
}



/**************************************************
* 函数名称：F_MemoryWrite（童锁状态改变后立即保存）
**************************************************/
void F_MemoryWrite(void)
{
    U8 l_au8Dat[2] = {0};

    // 仅在童锁状态实际改变后调用，避免在主循环中重复擦写 EEPROM。
    l_au8Dat[0] = g_stSys.bChildLockEn;
    l_au8Dat[1] = 0x5A + l_au8Dat[0];

    f_iapWrite(0x02, 0x00, l_au8Dat, 2);
}

/**************************************************
* 函数名称：F_MemoryRead（仅读取童锁状态）
**************************************************/
void F_MemoryRead(void)
{
    U8 l_au8Dat[2] = {0};
    U8 u8CheckSum = 0;

    f_iapRead(0x02, 0x00, l_au8Dat, 2);
    u8CheckSum = (U8)(0x5A + l_au8Dat[0]);

    if(l_au8Dat[1] == u8CheckSum)
    {
        if(l_au8Dat[0] <= 1)
        {
            g_stSys.bChildLockEn = l_au8Dat[0];
            return; 
        }
    }
	g_stSys.bChildLockEn = 0;
}

// =================================================================
// 对外调度入口
// =================================================================
void F_mFunctionSchdule(void)
{
    if(F_GetTimeTick(C_TIME_100ms))
    {
        F_TipOverEvent();       // 快速强制关断，与负载仲裁同步
        F_PowerEdge();
        F_LoadArbiter();
        F_HiPtcCooldownEdge();
        F_LoadDrive();
    }

    if(F_GetTimeTick(C_TIME_1s))
    {
        F_DelayCounters();
        F_HumanSenseEvent();
        F_TimerEvent();
    }
}
