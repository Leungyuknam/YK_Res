#ifndef _KEYFUNCTION_H_
#define _KEYFUNCTION_H_

// -----------------------------------------------------------------
// HP12-HAR 加热档位
// 低档：PTC_LOW（600 W）+ 风机
// 高档：PTC_LOW + PTC_HI（1200 W）+ 风机
// -----------------------------------------------------------------
#define GEAR_OFF        0
#define GEAR_LOW        1
#define GEAR_HI         2

// 定时叠加位标志（1H + 2H + 4H 组合）
#define TMR_1H          0x01
#define TMR_2H          0x02
#define TMR_4H          0x04
#define TMR_BITS_MASK   (TMR_1H|TMR_2H|TMR_4H)

// keyCfg.h 表格使用的 5 键键值
#define c_KEY_POWER     0x01
#define c_KEY_GEAR      0x02
#define c_KEY_TIME      0x04
#define c_KEY_HUMAN     0x08
#define c_KEY_LOCK      0x10

// -----------------------------------------------------------------
// 整机全局状态
// -----------------------------------------------------------------
typedef struct
{
    // --- 整机主状态 ---
    U8  bTurnOn;                // 开关机状态
    U8  u8WorkGear;             // 用户设置档位：GEAR_OFF/LOW/HI

    // --- 童锁（状态改变后写入 EEPROM）---
    U8  bChildLockEn;

    // --- 人体感应 ---
    U8  bHumanSenseEn;          // 用户是否启用人体感应模式
    U8  bHumanSenseStandby;     // 是否进入连续 2 分钟无人的待机状态
    U8  bIsHumanDetected;       // 去抖后的传感器状态
    U16 u16NoPersonSec;         // 无人持续秒数（最大 12 小时，即 43200 秒）

    // --- 定时 ---
    // u16TimerRemainMin 是定时状态的唯一数据源。1H/2H/4H
    // 指示灯按二进制权重显示“剩余分钟 / 60”的向上取整值，
    // 三个灯的权重分别为 1/2/4，因此剩余整小时数可
    // 直接映射到亮灯组合。详见 F_GetTimerHourBits()。
    U16 u16TimerRemainMin;      // 剩余总分钟数（0～420）
    U8  u8TimerSecCnt;          // 分钟内秒计数（0～59）

    // --- 倾倒保护 ---
    // bIsTipOver 是去抖后的唯一倾倒标志：该标志置位时
    // 屏蔽所有按键，立即切断负载并熄灭全部指示灯。
    // 倾倒会强制 bTurnOn=0，因此机器扶正后仍保持关机，
    // 必须再按一次开机键才会重新启动。
    U8  bIsTipOver;

    // --- 冷却锁定与延时 ---
    U8  u8HiPtcCoolSec;         // 0=可启动，>0=禁止开启高档 PTC
    U8  u8FanDelaySec;          // 0=无延时，>0=PTC 关闭后风机继续运行

    // --- 实际输出状态（由 F_LoadArbiter 仲裁）---
    U8  bOutLowPtc;
    U8  bOutHiPtc;
    U8  bOutFan;

} ST_SYSTEM_DATA;

extern ST_SYSTEM_DATA xdata g_stSys;

void F_PowerKey(void);
void F_GearKey(void);
void F_TimerKey(void);
void F_HumanKey(void);
void F_LockKey(void);
void F_OnAnyKeyActivity(void);

// 剩余时间转为 1H/2H/4H 指示灯位组合（向上取整为小时，0～7）
U8   F_GetTimerHourBits(void);

#endif
