#ifndef _SENSE_DEAL_H_
#define _SENSE_DEAL_H_

// 主循环入口：扫描倾倒和人体感应输入，
// 去抖时间为 200 ms，结果分别写入 g_stSys.bIsTipOver 和
// g_stSys.bIsHumanDetected。
void F_mAllSenseDeal(void);
#endif
