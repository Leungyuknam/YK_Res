#include "h\Allinclude.h"

ST_SYSTEM_DATA xdata g_stSys;

// =================================================================
// 辅助函数
// =================================================================

// 返回非零表示应屏蔽开机键以外的按键
static U8 F_KeysLocked(void)
{
    return (g_stSys.bChildLockEn || g_stSys.bIsTipOver || !g_stSys.bTurnOn);
}

// 剩余分钟数转换为显示的整小时数（0～7），向上取整。
// 该数值同时可作为 1H/2H/4H 指示灯的位组合，因为各指示灯
// 权重为 1/2/4（例如 3 小时 -> 0b011 -> 点亮 1H+2H）。
U8 F_GetTimerHourBits(void)
{
    U8 h;
    if(g_stSys.u16TimerRemainMin == 0) return 0;
    h = (U8)((g_stSys.u16TimerRemainMin + 59) / 60);    // 向上取整
    if(h > 7) h = 7;
    return h;
}

// 切换到关机状态时，复位所有不需要记忆的字段。
// 按规格：人体感应、档位和定时均不记忆；
// 童锁需要记忆（由 EEPROM 读写函数处理，此处不修改）。
static void F_ResetVolatileOnOff(void)
{
    g_stSys.bHumanSenseEn      = 0;
    g_stSys.bHumanSenseStandby = 0;
    g_stSys.u8WorkGear         = GEAR_LOW;
    g_stSys.u16TimerRemainMin  = 0;
    g_stSys.u8TimerSecCnt      = 0;
    g_stSys.u16NoPersonSec     = 0;
}

// 每个按键处理函数都会调用：清零无人计时，
// 若当前处于人感待机状态则唤醒整机。
void F_OnAnyKeyActivity(void)
{
    g_stSys.u16NoPersonSec = 0;

    if(g_stSys.bHumanSenseStandby)
    {
        g_stSys.bHumanSenseStandby = 0;     // 唤醒并重新启动负载
    }
}

// =================================================================
// 按键处理函数
// =================================================================

void F_PowerKey(void)
{
    if(g_u8TestState == C_SelfTest)
    {
        BUZ_KEY();
        return;
    }

    // 机器处于倾倒状态时，屏蔽开机键。
    if(g_stSys.bIsTipOver) return;

    if(g_stSys.bTurnOn)
    {
        // 正常关机：PTC 停止，风机延时 30 秒关闭（由 Function.c 处理）。
        g_stSys.bTurnOn = 0;
        F_ResetVolatileOnOff();
        BUZ_OFF();                          // 蜂鸣 300 ms
    }
    else
    {
        // 开机：默认低档，人体感应关闭。
        g_stSys.bTurnOn       = 1;
        g_stSys.u8WorkGear    = GEAR_LOW;
        g_stSys.bHumanSenseEn = 0;
        g_stSys.bHumanSenseStandby = 0;
        g_stSys.u16TimerRemainMin = 0;
        g_stSys.u8TimerSecCnt = 0;
        g_stSys.u16NoPersonSec = 0;
        BUZ_KEY();                          // 蜂鸣 100 ms
    }
    F_DispTskChange(DIPS_NORMAL);
    F_OnAnyKeyActivity();
}

void F_GearKey(void)
{
    if(g_u8TestState == C_SelfTest)
    {
        BUZ_KEY();
        return;
    }
    if(F_KeysLocked()) return;

    // 两档循环：低档 <-> 高档
    if(g_stSys.u8WorkGear == GEAR_LOW) g_stSys.u8WorkGear = GEAR_HI;
    else                               g_stSys.u8WorkGear = GEAR_LOW;

    BUZ_KEY();
    F_OnAnyKeyActivity();
}

void F_TimerKey(void)
{
    if(g_u8TestState == C_SelfTest)
    {
        BUZ_KEY();
        return;
    }
    if(F_KeysLocked()) return;

    // 在当前显示的剩余小时数基础上增加 1 小时。
    // 循环顺序：显示 0 -> 1 小时 -> 2 小时 -> ... -> 7 小时 -> 关闭定时。
    // 例如：剩余 30 分钟时仅点亮 1H 指示灯（显示为 1 小时）；
    // 再按一次定时键后设置为 2 小时。
    {
        U8 u8NewHour = F_GetTimerHourBits() + 1;     // 当前显示值加 1
        if(u8NewHour > 7)
            g_stSys.u16TimerRemainMin = 0;           // 7 小时 -> 关闭定时
        else
            g_stSys.u16TimerRemainMin = (U16)u8NewHour * 60;
        g_stSys.u8TimerSecCnt = 0;
    }

    BUZ_KEY();
    F_OnAnyKeyActivity();
}

void F_HumanKey(void)
{
    if(g_u8TestState == C_SelfTest)
    {
        BUZ_KEY();
        return;
    }
    if(F_KeysLocked()) return;

    g_stSys.bHumanSenseEn = !g_stSys.bHumanSenseEn;
    if(g_stSys.bHumanSenseEn)
    {
        g_stSys.u16NoPersonSec     = 0;
        g_stSys.bHumanSenseStandby = 0;
    }
    else
    {
        g_stSys.bHumanSenseStandby = 0;
    }

    BUZ_KEY();
    F_OnAnyKeyActivity();
}

// 童锁：长按 2 秒切换；仅在整机开机时有效
// （关机或处于倾倒状态时屏蔽）。
// 童锁状态改变后立即写入 EEPROM。
void F_LockKey(void)
{
    if(g_u8TestState == C_SelfTest)
    {
        BUZ_KEY();
        return;
    }
    if(!g_stSys.bTurnOn || g_stSys.bIsTipOver) return;

    g_stSys.bChildLockEn = !g_stSys.bChildLockEn;
    F_MemoryWrite();
    BUZ_LOCK();
    F_OnAnyKeyActivity();
}
