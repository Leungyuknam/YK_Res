#ifndef _BUZZER_H_
#define _BUZZER_H_

//g_u8BuzClk
#define c_BUZ_CLK_OFF	    0
#define c_BUZ_CLK_1800	    1//1.8KHz
#define c_BUZ_CLK_2000	    2
#define c_BUZ_CLK_2300	    3
#define c_BUZ_CLK_2800	    4

#define c_MuteTimeBak	10

#define BUZ_OFF		0
#define BUZ_POWER	1
#define BUZ_SHORT	2
#define BUZ_UP	    3
#define BUZ_DOWN	4
#define BUZ_SHORT_2	5
#define BUZ_SHORT_3	6
#define BUZ_LOCK	7

extern U8 xdata g_u8BuzMode;
extern U8 xdata g_u8BuzType;
extern bit g_bMuteOn;

extern U8 xdata g_u8SoundTimeCntDwn;
extern U8 xdata g_u8MuteTimeCntDwn;
//extern U8 xdata g_u8BeepTimesCntDwn;
extern U16 xdata g_u16BeepTimesCntDwn;
extern void F_mBuzProc();			
extern void F_Buzzer_IRQ();

extern void F_BuzzerSet(U8 l_u8Flg);
//extern void S_API_Buz(uchar cnt, uint on_time, uint off_time);

#endif
