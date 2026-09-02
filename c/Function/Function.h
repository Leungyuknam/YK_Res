#ifndef _FUNCTION_H_
#define _FUNCTION_H_

/* ========================================== */
/* 调试与时间宏配置               */
/* ========================================== */
//#define DEBUG_TIME_QUICK      // 开启缩时测试标记

#ifdef DEBUG_TIME_QUICK
#pragma message "---------缩时测试---------"
    #define c_Time_1h         60
    #define c_Time_1min       1
#else
    #define c_Time_1h         3600
    #define c_Time_1min       60
#endif

/* ========================================== */
/* 风机档位定义                 */
/* ========================================== */
#define FAN_OFF                 0
#define FAN_SLEEP               1
#define FAN_LOW                 2
#define FAN_MID                 3
#define FAN_HI                  4

/* ========================================== */
/* 外部变量声明                 */
/* ========================================== */
extern U16 xdata g_u16SenseEnTime;      // 传感器使能时间，单位 100ms
extern U8  xdata g_u8AirQuality;        // 综合空气质量
extern U8  xdata g_u8FanSpeed;          // 目标风速档位
extern U8  xdata g_u8AiFanSpeed;        // AI 自动模式下的风速
extern U8  xdata g_u8LocalTimeIsNight;  // 昼夜标志位
extern U8  xdata g_u8VoiceFilterBeepCnt;
extern U8  xdata g_u8VoiceCntTime;
/* ========================================== */
/* 负载控制联合体                 */
/* ========================================== */
extern StrByte LOAD;
#define g_u8LoadState           LOAD.Byte
#define g_bSenseLoad            LOAD.BBit.b0  // 传感器电源开启标志
#define g_bIonLoad              LOAD.BBit.b1  // 等离子开启标志
#define g_bUvLoad               LOAD.BBit.b2  // UV 灯开启标志

/* ========================================== */
/* 外部接口声明                 */
/* ========================================== */
extern void F_LoadDry(void);
extern void F_mFunctionSchdule(void);
extern void F_mSystemInital(void);

#endif /* _FUNCTION_H_ */