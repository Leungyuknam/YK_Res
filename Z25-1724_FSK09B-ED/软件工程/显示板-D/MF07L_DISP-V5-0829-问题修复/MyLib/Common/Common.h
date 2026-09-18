#ifndef Common_H
#define Common_H

#ifdef _Native_Time_Fuction_H
   #define	EXT_TIME
#else
   #define	EXT_TIME extern	
#endif






#ifdef _Native_Time_Fuction_H
	EXT_TIME X_REG u8 g_u8Bright_500MS_Cnt = 0;
	EXT_TIME X_REG u8 g_u8PowerON_Cnt = 0;
	EXT_TIME X_REG u8 g_u8100MS_Cnt = 0;
	EXT_TIME X_REG u8 g_u81S_Cnt = 0;
	EXT_TIME X_REG u8 g_u81Min_Cnt = 0;
	EXT_TIME X_REG u8 g_u8PowerON_TCnt = 0;  
	EXT_TIME X_REG Flag8 g_TimeSign = {0};
	EXT_TIME bit B_4MS = 0;


	
	EXT_TIME  u8 g_u8BaseTime = 0;
	EXT_TIME  u8 g_u8SysTime_Cnt = 0;
	EXT_TIME  bdata u8 g_u8ScanBaseTime = 0;

#else
	EXT_TIME X_REG u8 g_u8Bright_500MS_Cnt;
	EXT_TIME X_REG u8 g_u8PowerON_Cnt;
	EXT_TIME X_REG u8 g_u8100MS_Cnt;
	EXT_TIME X_REG u8 g_u81S_Cnt;
	EXT_TIME X_REG u8 g_u81Min_Cnt;
	EXT_TIME X_REG u8 g_u8PowerON_TCnt;
	EXT_TIME X_REG Flag8 g_TimeSign;
	EXT_TIME bit B_4MS;
	EXT_TIME  u8 g_u8BaseTime;
	EXT_TIME  u8 g_u8SysTime_Cnt;
	EXT_TIME  bdata u8 g_u8ScanBaseTime;
#endif




#define TIME_BASE				0x01
#define TIME_5MS				0x02
#define TIME_10MS				0x04
#define TIME_100MS				0x08
#define TIME_1S					0x10
#define TIME_1MIN				0x20
#define TIME_1H					0x40

#define	MS10_CNT				80  //10ms






#define	B_Bright_500MS		g_TimeSign.Bits.Bit0	//显示闪烁亮灭用，不能整个字节重置	
#define	BIT_BRIGHT_500MS					0x01
#define	B_Bright_1S			g_TimeSign.Bits.Bit1
#define	BIT_BRIGHT_1S						0x02
#define	B_Bright_OFF_1S		g_TimeSign.Bits.Bit2
#define	BIT_BRIGHT_OFF_1S					0x04

#define	B_500MS_BriOn		g_TimeSign.Bits.Bit3	//显示闪烁亮灭用，不能整个字节重置	
#define	BIT_500MS_BRION						0x08
#define	B_500MS				g_TimeSign.Bits.Bit4	//显示闪烁亮灭用，不能整个字节重置	
#define	B_OneSec_OFFON		g_TimeSign.Bits.Bit5	//显示闪烁亮灭用，不能整个字节重置	
#define	BIT_ONESEC_OFFON					0x20



#define	TOUCH_SCAN_TIME		32						//触摸模块扫描时间间隔 * 125us   //一般4MS



//extern void TCntReset();

extern void TimeFucRunStart(void);
extern void TimeBaseTickIRQ(void);
extern void TimeProc(void);
extern unsigned char GetTimeTick(unsigned char temp);
//extern unsigned char CheckSum(unsigned char *ucPoint, unsigned char Count);
extern void CopyStr(unsigned char *TargetStr, unsigned char *SourceStr, unsigned char Count);
extern void SetStr(unsigned char *TargetStr, unsigned char Value, unsigned char Count);
extern unsigned char CompareStr(unsigned char *TargetStr, unsigned char *SourceStr, unsigned char Count);
extern u8 CmpStrAndCopy(unsigned char *TargetStr, unsigned char *SourceStr, unsigned char Count);


#endif
