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

#define c_PM25_GOOD         50     //PM25优良等级分界
#define c_PM25_MID          100    //PM25良中等级分界
#define c_PM25_BAD          150    //PM25中差等级分界

#define c_VOC_GOOD         100     //VOC优良等级分界
#define c_VOC_MID          200     //VOC良中等级分界
#define c_VOC_BAD          300     //VOC中差等级分界
#define c_VOC_BIAS         10      //VOC质量下降回差

#define c_CH2O_GOOD         60     //CH2O优良等级分界
#define c_CH2O_MID          80     //CH2O良中等级分界
#define c_CH2O_BAD          120    //CH2O中差等级分界


extern U16 g_u16ScreenRunTime;//滤网使用时间
extern U8 g_u8RunPercent;//滤网使用百分比
extern U8 g_u8FhFilterPer;//滤网剩余百分比
extern bit g_bScreenInitFlg;//滤网复位标记
#define DUST_TOTAL_TIM      30000//滤网最长使用时间 扩大10倍
//#define DUST_TOTAL_TIM      60//滤网最长使用时间 扩大10倍

extern StrByte SENSE;
#define g_u8SenseState      SENSE.dchar
#define g_bQDErr            SENSE.BBit.b0//倾倒故障               P4
#define g_bQDErrBak         SENSE.BBit.b1//倾倒故障
#define g_bWaterShortage    SENSE.BBit.b2//缺水                   P4
#define g_bWaterShortageBak SENSE.BBit.b3//缺水
#define g_bDoorOpenLeft    				SENSE.BBit.b4//左门盖检测               P4
#define g_bDoorOpenLeftBak 				SENSE.BBit.b5//左门盖检测
#define g_bDoorOpenRight    			SENSE.BBit.b6//右门盖检测               P4
#define g_bDoorOpenRightBak 			SENSE.BBit.b7//右门盖检测

extern bit g_bWaterBox;		//水箱在位
extern bit g_bWaterBoxBak;	//水箱在位

extern void F_mAllSenseDeal(void);

#endif 