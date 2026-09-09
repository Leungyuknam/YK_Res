#ifndef _RGBDRV_H_
#define _RGBDRV_H_

// ------------------- 类型定义 -------------------

// 颜色索引枚举 (必须与 Function.c 的逻辑对应)
typedef enum
{
    COLOR_OFF = 0,
    COLOR_ORANGE,   // 橘色 (<40%)
    COLOR_WHITE,    // 白色 (40-65%)
    COLOR_YELLOW,   // 黄色 (>65%)
    // COLOR_RED,   // 预留
    // COLOR_BLUE,  // 预留
    COLOR_MAX
} E_COLOR_INDEX;

// RGB 控制结构体
typedef struct
{
    // --- 控制标志 ---
    U8 bEnable    : 1;    // 总开关
    U8 bRainbow   : 1;    // 彩虹模式标志
    
    // --- 目标 PWM (经过亮度计算后的最终值) ---
    U8 u8RedDuty;
    U8 u8GreenDuty;
    U8 u8BlueDuty;

    // --- 彩虹模式变量 ---
    U16 u16EffectPhase;   // 相位
    U8  u8SpeedDiv;       // 速度分频
} TYPE_RGB_CTRL;

// ------------------- 外部接口 -------------------

// 初始化
extern void F_RgbInit(void);

// 核心处理函数 (建议 100ms 调用，用于计算亮度和颜色)
extern void F_RgbControl_Task(void);

#endif