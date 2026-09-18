#ifndef __BUZZER_H_
#define __BUZZER_H_

// 鸣叫和间隔时间均以 F_BellControl 的 10 ms 节拍为单位。
// BUZ_KEY     ：鸣叫 100 ms，用于开机、按键、档位/定时切换
// BUZ_OFF     ：鸣叫 300 ms，用于正常关机
// BUZ_LOCK    ：鸣叫 100 ms，用于切换童锁
// BUZ_TIPOK   ：鸣叫 300 ms，用于倾倒后扶正提醒
// BUZ_KEYINVAL：短鸣两次，用于锁键反馈（可选，当前未使用）

extern xdata U8 g_u8BellOnTime;
extern xdata U8 g_u8BellTimes;
extern xdata U8 g_u8BellOffTime;
extern xdata U8 g_u8SetBellOnTime;
extern xdata U8 g_u8SetBellOffTime;

extern void F_BellControl(void);
extern void F_BellIrq(void);
extern void F_SetBuz(U8 l_u8Times, U8 l_u8SetBellOnTime, U8 l_u8SetBellOffTime);

#define BUZ_POWERON()       F_SetBuz(1, 50, 10)
#define BUZ_KEY()           F_SetBuz(1, 10, 10)
#define BUZ_OFF()           F_SetBuz(1, 30, 10)
#define BUZ_LOCK()          F_SetBuz(1, 10, 10)
#define BUZ_TIPOK()         F_SetBuz(1, 30, 10)
#define BUZ_KEYINVAL()      F_SetBuz(2, 10, 10)

#endif
