//#ifndef _DCFANDRV_H_					
//#define _DCFANDRV_H_					

////#define DEBUG_FAN        //屏蔽电机故障检测

//#define CAL_CYCLE			    (24)

//#define	RUN_CYCLE				(12000)
//#define	MAX_DUTY				(6000)
//#define	MIN_DUTY				(1200)
//#define	START_DUTY				(2160)
//#define	MAX_CAL					(50)

//typedef struct _TYPE_DC_FAN
//{
//	unsigned int AimSpeed;
//	unsigned int CurSpeed;
//	unsigned char ErrCnt;
//	unsigned int  Duty;
//	unsigned char PulseCnt;
//	unsigned int CalTurnCnt;
//	unsigned int SpeedTime;
//	unsigned int TimeCnt;
//}TYPE_DC_FAN;

//extern bit g_bInFanErr;//风机故障标志

//extern TYPE_DC_FAN InFan;

//extern void DCFanIRQ();
//extern void DCFanProc();
//extern void F_PwmInit();


#ifndef _DCFANDRV_H_
#define _DCFANDRV_H_

// PWM 参数定义 (Fsys = 24MHz, Fpwm = 7.5kHz)
// Period = 24000000 / 7500 / 2（双沿）= 1600
#define PWM_PERIOD_ARR      (1600)          
#define PWM_MAX_DUTY        (1600)          // 100% 占空比
#define PWM_MIN_DUTY        (160)           // 10% 最小启动占空比(示例)

// 软启动参数
#define DUTY_THRESHOLD_10P  (160)           // 10% 占空比对应的数值 (1600 * 0.1)
#define DUTY_STEP_VAL       (4)             // 0.25% 占空比对应的数值 (1600 * 0.0025)

typedef struct _TYPE_DC_FAN
{
    unsigned int AimDuty;   // 目标占空比数值 (范围 0 ~ 1600)
    unsigned int CurDuty;   // 当前实际输出占空比
    unsigned int RampTimer; // 软启动延时计数器
} TYPE_DC_FAN;

extern TYPE_DC_FAN InFan;
extern bit g_bFanPwmEn;     // 风机使能标志

// 函数声明
extern void DCFanProc(void);
extern void F_PwmInit(void);

#endif
