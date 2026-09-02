#ifndef _SENSE_DEAL_H_
#define _SENSE_DEAL_H_

/* ========================================== */
/* 环境数据与空气质量等级                     */
/* ========================================== */
extern S8 xdata g_s8NowTemp;            // 当前温度
extern U8 xdata g_u8NowHumi;            // 当前湿度

extern U8 xdata g_u8PM25AirQuality; //PM25空气质量
extern U8 xdata g_u8PM10AirQuality; //PM10空气质量
extern U8 xdata g_u8PM1AirQuality;  //PM1空气质量
extern U8 xdata g_u8VocAirQuality;  //VOC空气质量
extern U8 xdata g_u8CH2OAirQuality; //甲醛空气质量
#define AIR_QUALITY_ACTOR       0//空气质量优
#define AIR_QUALITY_GOOD        1//空气质量良
#define AIR_QUALITY_MID         2//空气质量中
#define AIR_QUALITY_BAD         3//空气质量差

#define c_STIVE_GOOD                50     //粉尘优良等级分界
#define c_STIVE_MID                 100    //粉尘良中等级分界
#define c_STIVE_BAD                 150    //粉尘中差等级分界

#define c_VOC_GOOD                  2000    //VOC优良等级分界
#define c_VOC_MID                   4000    //VOC良中等级分界
#define c_VOC_BAD                   6000    //VOC中差等级分界
#define c_VOC_BIAS                  10      //VOC质量下降回差

#define c_CH2O_GOOD                 60     //甲醛优良等级分界
#define c_CH2O_MID                  80     //甲醛良中等级分界
#define c_CH2O_BAD                  120    //甲醛中差等级分界

/* ========================================== */
/* 滤网寿命管理                               */
/* ========================================== */
#define DUST_TOTAL_TIM          21600   // 滤网最长使用时间(3000小时)，放大10倍

extern U16 g_u16ScreenRunTime;          // 滤网已使用时间
extern U8  g_u8RunPercent;              // 滤网使用百分比
extern U8  g_u8FhFilterPer;             // 滤网剩余百分比
extern bit g_bScreenInitFlg;            // 滤网复位标记

/* ========================================== */
/* 硬件异常传感器标记联合体                   */
/* ========================================== */
extern StrByte SENSE;
#define g_u8SenseState          SENSE.dchar
#define g_bQDErr                SENSE.BBit.b0  // 倾倒故障
#define g_bQDErrBak             SENSE.BBit.b1  // 倾倒故障备份
#define g_bWaterShortage        SENSE.BBit.b2  // 缺水
#define g_bWaterShortageBak     SENSE.BBit.b3  // 缺水备份
#define g_bDoorOpenLeft         SENSE.BBit.b4  // 左门盖检测
#define g_bDoorOpenLeftBak      SENSE.BBit.b5  // 左门盖检测备份
#define g_bDoorOpenRight        SENSE.BBit.b6  // 右门盖检测
#define g_bDoorOpenRightBak     SENSE.BBit.b7  // 右门盖检测备份

extern bit g_bWaterBox;                 // 水箱在位
extern bit g_bWaterBoxBak;              // 水箱在位备份

/* ========================================== */
/* 外部接口声明                               */
/* ========================================== */
extern void F_mAllSenseDeal(void);

#endif /* _SENSE_DEAL_H_ */