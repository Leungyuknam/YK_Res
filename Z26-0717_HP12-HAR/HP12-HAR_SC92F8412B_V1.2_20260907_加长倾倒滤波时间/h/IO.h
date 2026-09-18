#ifndef _IO_H_
#define _IO_H_

// -----------------------------------------------------------------
// HP12-HAR 引脚映射（依据规格讨论）
// -----------------------------------------------------------------

// 蜂鸣器
#define P_BUZZ          P14

// 负载（均为 GPIO 数字输出，继电器/可控硅驱动默认低电平有效）
#define P_PTC_LOW       P12     // 低档 PTC（600 W）
#define P_PTC_HI        P13     // 高档 PTC（1200 W）
#define P_FAN           P05     // 风机电机与负离子共用

// 传感器
#define P_TIP           P11     // 倾倒开关输入
#define P_HUMAN         P04     // PIR 人体感应输入

// 显示矩阵：4 SEG × 2 COM（驱动 8 个独立指示灯）
#define P_SEG1          P03
#define P_SEG2          P02
#define P_SEG3          P01
#define P_SEG4          P00
#define P_COM1          P27
#define P_COM2          P26

// -----------------------------------------------------------------
// 有效电平约定
// -----------------------------------------------------------------
#define PTC_LOAD_ON         0
#define PTC_LOAD_OFF        1

#define FAN_LOAD_ON			0
#define FAN_LOAD_OFF		1
// 按共阴极方式设计：SEG 引脚输出高电平点亮指示灯，
// COM 引脚有效时输出低电平；若 PCB 接线不同需相应调整。
#define SEG_ON          1
#define SEG_OFF         0
#define COM_ON          0
#define COM_OFF         1

// 传感器有效电平为暂定值，确认硬件后定稿
#define TIP_ACTIVE      0       // 0=机器倾倒
#define HUMAN_ACTIVE    1       // 1=检测到人员

#endif
