#ifndef __FUNCTION_H_
#define __FUNCTION_H_

#define     __DEBUG__           0



#ifdef _Native_FUN_H
   #define	EXT_FUN
#else
   #define	EXT_FUN extern	
#endif

#define	POW_ON_TIME				20 //*100MS 上电全亮时间

#ifdef _Native_FUN_H

#else

#endif

extern void F_TimeEvent(void);

#endif



