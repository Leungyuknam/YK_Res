#ifndef _FUNCTION_H_
#define _FUNCTION_H_

//#define DEBUG_TIME_QUICK     // 取消注释可加速定时功能测试
#ifdef DEBUG_TIME_QUICK
#pragma message "---- TIME ACCEL ----"
    #define C_TIME_1H        60
#else
    #define C_TIME_1H        3600
#endif


#define All_COM_OFF()   { \
							P0    = B00100000;\
							P1    = B00101100;\
							P2    = B11110011;\
                        }

extern void F_mSystemInital(void);
extern void F_mFunctionSchdule(void);
extern void F_MemoryRead(void);
extern void F_MemoryWrite(void);
#endif
