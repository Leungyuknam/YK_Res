#ifndef _VOC_H_
#define _VOC_H_

//#define c_VOC_PREHEAT_TIME      900//VOC预热时间 单位100ms 预热时间60s,10~60s强制显示优等级
#define c_VOC_PREHEAT_TIME      300

extern U8 xdata g_u8VocLinkTime;
#define c_VOC_LINK_ERR_TIME      10//单位s

extern U16 xdata g_u16VocData;//VOC数值 0~500对应0~5ppm
#define c_VOC_DATA_MAX  500

extern S8 xdata g_s8TempData;//温度 范围-40~85℃
//#define c_TEMP_MIN      -40
#define c_TEMP_MIN      0
#define c_TEMP_MAX      85
extern U8 xdata g_u8HumiData;//湿度 范围0~100%RH
#define c_HUMI_MIN      0
#define c_HUMI_MAX      100


extern S8  xdata g_s8DispTempData;   /* 对外显示/上报温度 */
extern U8  xdata g_u8DispHumiData;   /* 对外显示/上报湿度 */
extern U16 xdata g_u16DispVocData;    /* 对外显示/上报VOC */

//g_u8VocReadType
#define c_VOC_READ_INIT         0
#define c_VOC_READ_TEMP_HUMI    1
#define c_VOC_READ_VOC_DATA     2

#define c_VOC_LENGTH_MAX        6//接收最大数据长度

extern void F_QueryVocData();

#endif