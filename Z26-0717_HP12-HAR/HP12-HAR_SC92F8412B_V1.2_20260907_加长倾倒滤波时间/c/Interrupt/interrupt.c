// =================================================================
// 定时器 0 中断服务：125 us 节拍
//   - 系统时基
//   - 软件 UART 位定时
//   - 蜂鸣器驱动
//   - 显示矩阵扫描（每 4 ms 一次）
// =================================================================
#include "h\Allinclude.h"

void INT_Timer0() interrupt 1
{
    static U8 s_u8DispTick = 0;

    TF0 = 0;

    F_TimeBaseTick_IRQ();
//    F_Uart_Analog_IRQ();
    F_BellIrq();

//    // 32 * 125 us = 每个 COM 步进 4 ms；2 个 COM 构成 8 ms 一帧（125 Hz）
//    if(++s_u8DispTick >= 32)
//    {
//        s_u8DispTick = 0;
//        F_DisplayDrive_IRQ();
//    }
}
