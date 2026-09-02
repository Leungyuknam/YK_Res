#ifndef _CH2O_H_
#define _CH2O_H_

//#define c_CH2O_PREHEAT_TIME      900//甲醛预热时间 单位100ms
#define c_CH2O_PREHEAT_TIME      300//甲醛预热时间 单位100ms

extern U8 xdata g_u8CH2OLinkTime;//甲醛通讯时间
#define c_CH2O_LINK_ERR_TIME      10//单位s

extern U16 xdata g_u16CH2OData;//CH2O数值 0~5ppm对应0~5000ppb 最终单位ug/m3 范围0~6250ug/m3
#define c_CH2O_DATA_MAX  6250

//extern S8 xdata g_s8TempData;//温度 范围-20~50℃
//#define c_TEMP_MIN      -20
//#define c_TEMP_MAX      50
//extern U8 xdata g_u8HumiData;//湿度 范围15~90%RH
//#define c_HUMI_MIN      15
//#define c_HUMI_MAX      90

//g_u8CH2OReadType
#define c_CH2O_READ_INIT         0
//#define c_CH2O_READ_TEMP_HUMI    1
#define c_CH2O_READ_CH2O_DATA    1

#define c_CH2O_LENGTH_MAX        6//接收最大数据长度

extern void F_QueryCH2OData();

#endif