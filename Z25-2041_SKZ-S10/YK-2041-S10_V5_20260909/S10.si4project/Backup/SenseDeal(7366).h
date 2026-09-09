#ifndef _SENSE_DEAL_H_
#define _SENSE_DEAL_H_

extern S8 xdata g_s8NowTemp;//当前温度

extern U8 xdata g_u8NowHumi;//当前湿度

extern U8 xdata g_u8PM25AirQuality;//PM25空气质量
extern U8 xdata g_u8VocAirQuality;//VOC空气质量
extern U8 xdata g_u8CH2OAirQuality;//甲醛空气质量
#define c_AIR_QUALITY_ACTOR     0//空气质量优
#define c_AIR_QUALITY_GOOD      1//空气质量良
#define c_AIR_QUALITY_MID       2//空气质量中
#define c_AIR_QUALITY_BAD       3//空气质量差

#define c_VOC_GOOD         100//VOC优良等级分界
#define c_VOC_MID          200//VOC良中等级分界
#define c_VOC_BAD          300//VOC中差等级分界
#define c_VOC_BIAS         10//VOC质量下降回差

extern U16 g_u16ScreenRunTime;//滤网使用时间
extern U8 g_u8RunPercent;//滤网使用百分比
extern U8 g_u8FhFilterPer;//滤网剩余百分比
extern bit g_bScreenInitFlg;//滤网复位标记
#define DUST_TOTAL_TIM      21600//滤网最长使用时间 扩大10倍

extern StrByte SENSE;
#define g_u8SenseState      SENSE.dchar
#define g_bWaterLack        SENSE.BBit.b0
#define g_bWaterFull        SENSE.BBit.b1
#define g_bDispUartErr      SENSE.BBit.b2//显示板接收通讯故障

extern void F_mAllSenseDeal(void);

#endif 



#ifndef _SENSE_DEAL_H_
#define _SENSE_DEAL_H_

extern S8 xdata g_s8NowTemp;//当前温度
extern U8 xdata g_u8NowHumi;//当前湿度

extern U8 xdata g_u8PM25AirQuality;//PM25空气质量
extern U8 xdata g_u8VocAirQuality;//VOC空气质量
extern U8 xdata g_u8CH2OAirQuality;//甲醛空气质量
#define c_AIR_QUALITY_ACTOR     0//空气质量优
#define c_AIR_QUALITY_GOOD      1//空气质量良
#define c_AIR_QUALITY_MID       2//空气质量中
#define c_AIR_QUALITY_BAD       3//空气质量差

#define c_VOC_GOOD         100//VOC优良等级分界
#define c_VOC_MID          200//VOC良中等级分界
#define c_VOC_BAD          300//VOC中差等级分界
#define c_VOC_BIAS         10//VOC质量下降回差

extern U16 g_u16ScreenRunTime;//滤网使用时间
extern U8 g_u8RunPercent;//滤网使用百分比
extern U8 g_u8FhFilterPer;//滤网剩余百分比
extern bit g_bScreenInitFlg;//滤网复位标记
#define DUST_TOTAL_TIM      21600//滤网最长使用时间 扩大10倍

#define c_HUMI_RANGE_1		0//当前湿度区间
#define c_HUMI_RANGE_2		1
#define c_HUMI_RANGE_3		2
#define c_HUMI_RANGE_4		3

extern StrByte SENSE;
#define g_u8SenseState      SENSE.dchar
#define g_bWaterLack        SENSE.BBit.b0
#define g_bWaterFull        SENSE.BBit.b1
#define g_bDispUartErr      SENSE.BBit.b2//显示板接收通讯故障

extern void F_mAllSenseDeal(void);

#endif 
