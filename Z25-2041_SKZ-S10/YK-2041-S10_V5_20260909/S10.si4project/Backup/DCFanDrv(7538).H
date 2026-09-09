#ifndef _DCFANDRV_H_					
#define _DCFANDRV_H_					

//#define DEBUG_FAN        //屏蔽电机故障检测

#define CAL_CYCLE			    (24)

#define	RUN_CYCLE				(12000)
#define	MAX_DUTY				(6000)
#define	MIN_DUTY				(1200)
#define	START_DUTY				(2160)
#define	MAX_CAL					(50)

typedef struct _TYPE_DC_FAN
{
	unsigned int AimSpeed;
	unsigned int CurSpeed;
	unsigned char ErrCnt;
	unsigned int  Duty;
	unsigned char PulseCnt;
	unsigned int CalTurnCnt;
	unsigned int SpeedTime;
	unsigned int TimeCnt;
}TYPE_DC_FAN;

extern bit g_bInFanErr;//风机故障标志

extern TYPE_DC_FAN InFan;

extern void DCFanIRQ();
extern void DCFanProc();
extern void F_PwmInit();

#endif
