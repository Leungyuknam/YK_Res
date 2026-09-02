#ifndef _VOC_H_
#define _VOC_H_

#define c_VOC_PREHEAT_TIME      600//VOC预热时间 单位100ms 预热时间60s,10~60s强制显示优等级

extern U8 xdata g_u8VocLinkTime;
#define c_VOC_LINK_ERR_TIME      10//单位s

/* 绝对保留此应用层显示变量 */
extern U16 xdata g_u16DispVocData;//VOC数值 0~500对应0~5ppm
#define c_VOC_DATA_MAX  9999

/* ------------ 算法相关宏定义 ------------ */
#define VOC_BUF_SIZE       5      /* VOC均值窗口 */
#define VOC_FAST_THR       200    
#define DELAY_FAST_SEC     5      /* 快速变化延时值 */
#define DELAY_SLOW_SEC     64     /* 微小变化延时值 */

//g_u8VocReadType
#define c_VOC_READ_INIT         0
#define c_VOC_READ_VOC_DATA     2

#define c_VOC_LENGTH_MAX        6//接收最大数据长度

extern void F_QueryVocData();

#endif