#ifndef _CH2O_H_
#define _CH2O_H_

#define c_CH2O_PREHEAT_TIME             90//甲醛预热时间 单位100ms

extern U8 xdata g_u8CH2OLinkTime;       //甲醛通讯时间
    #define c_CH2O_LINK_ERR_TIME        10//单位s

extern U16 xdata g_u16CH2OData;         //CH2O数值 0~5ppm对应0~5000ppb 最终单位ug/m3 范围0~6250ug/m3
extern U16 xdata g_u16DispCH2OData;     //甲醛显示数值
//    #define c_CH2O_DATA_MAX             6250
    #define c_CH2O_DATA_MAX             5000

extern S8 xdata g_s8TempData;           //温度 范围-20~50℃
//    #define c_TEMP_MIN                  -20
    #define c_TEMP_MIN                  0
    #define c_TEMP_MAX                  50
extern U8 xdata g_u8HumiData;           //湿度 范围15~90%RH
    #define c_HUMI_MIN                  15
    #define c_HUMI_MAX                  90

//g_u8CH2OReadType
    #define c_CH2O_READ_INIT            0
    #define c_CH2O_READ_TEMP_HUMI       1
    #define c_CH2O_READ_CH2O_DATA       2

//s_u8CH2ODataState
    #define CH2O_DATA_STATE_NORMAL      0//显示数据常规阶段
    #define CH2O_DATA_STATE_DECLINE     1//显示数据下降阶段(强降)

//---------- 显示缓变(常规阶段显示值跟随采集值的快/慢刷新) ----------
    #define CH2O_FAST_THR              5 //ug/m3,显示与采集差值≥此值时快速刷新
    #define DELAY_FAST_SEC             5 //差值大时刷新周期(s)
    #define DELAY_SLOW_SEC            64 //差值小时刷新周期(s)

//---------- 强降算法参数(单位ug/m3,80=0.080mg/m3) ----------
    #define c_CH2O_DECLINE_TRIG       80 //>0.080触发(持续30min后进入强降)
    #define c_CH2O_DECLINE_FLOOR      40 //强降下限0.040
    #define c_CH2O_DECLINE_STEP        2 //每1min强制下降0.002
    #define c_CH2O_RISE_THR           20 //新污染判定:真实值相对下降期最低值回升≥0.020则退出强降

#define c_CH2O_LENGTH_MAX           6//接收最大数据长度

extern void F_QueryCH2OData();

#endif
