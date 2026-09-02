//SystemTime.h
#ifndef _SYSTEM_TIME_H_
#define _SYSTEM_TIME_H_

//#define C_TIME_4ms				0x01
#define C_TIME_10ms				0x02
#define C_TIME_100ms			0x04
#define C_TIME_1s				0x08
#define C_TIME_500ms			0x10
//#define C_TIME_1H				0x20
//#define	C_TIME_2ms				0x40

#define	C_TIME_NEXT				0x80

extern U8 xdata g_u8MCURunTime;         //上电时间
extern U8 xdata g_u8RandomNum;          //随机数

void F_mSystemTimeDeal(void);
void F_TimeBaseTick_IRQ(void);
unsigned char F_GetTimeTick(unsigned char temp);

extern void F_SetStr(unsigned char *AimStr,unsigned char Value,unsigned char Count);
extern void F_CopyStr(unsigned char *AimStr,unsigned char *F_CopyStr,unsigned char Count);

#endif 