#ifndef _SENSE_DEAL_H_
#define _SENSE_DEAL_H_

// 对外暴露计算好的湿度区间，供 Function.c 的智能模式使用
extern U8 xdata g_u8HumiRange;
extern U8 xdata g_bElectrolysisWaterReady;

#define c_HUMI_RANGE_1		0
#define c_HUMI_RANGE_2		1
#define c_HUMI_RANGE_3		2
#define c_HUMI_RANGE_4		3

// 传感器处理主函数
void F_mAllSenseDeal(void);


// 如果底层获取的原始数据是全局变量，确保这里能引用
// extern S16 g_s16TempData; // 原始温度
// extern U16 g_u16HumiData; // 原始湿度

#endif