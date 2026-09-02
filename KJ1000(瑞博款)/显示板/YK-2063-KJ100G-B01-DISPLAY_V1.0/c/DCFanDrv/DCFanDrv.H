#ifndef _DCFANDRV_H_					
#define _DCFANDRV_H_					

//#define DEBUG_FAN        //屏蔽电机故障检测

#define CAL_CYCLE			    (24)

#define	RUN_CYCLE				(16000)
#define	MAX_DUTY				(12800)//80%
#define	MIN_DUTY				(1600)//10%
//#define	START_DUTY				(2880)//18%
#define	START_DUTY				(1920)//12%

//#define	RUN_CYCLE				(2000)
//#define	MAX_DUTY				(1600)//80%
//#define	MIN_DUTY				(200)//10%
//#define	START_DUTY				(240)//12%


//#define	RUN_CYCLE				(16000)
//#define	MAX_DUTY				(14400)//90%
//#define	MIN_DUTY				(6400)//40%
////#define	START_DUTY				(2880)//18%
//#define	START_DUTY				(1920)//12%

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
