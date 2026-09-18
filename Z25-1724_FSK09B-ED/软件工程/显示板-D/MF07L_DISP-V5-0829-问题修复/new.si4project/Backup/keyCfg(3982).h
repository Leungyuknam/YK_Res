//----------需要你配置的部分-----------------------------
//声明你的按键处理函数
//extern void F_PowerEvent(); 
#include "Include\Include.h"

//----------按键模块标准时间基准-------------------------
#define C_TimeStandard  10//标准时间单位MS 即F_KeyDealControl函数调用的时间间隔

//----------下面是按键的码值-----------------------------


//----------不需要你配置的部分---------------------------
#define C_KEY_10MS  (10/C_TimeStandard)
#define C_KEY_60MS  (60/C_TimeStandard)
#define C_KEY_100MS (100/C_TimeStandard)
#define C_KEY_200MS (200/C_TimeStandard)
#define C_KEY_250MS (250/C_TimeStandard)

#define C_KEY_300MS (300/C_TimeStandard)
#define C_KEY_500MS (500/C_TimeStandard)
#define C_KEY_1S    (1000/C_TimeStandard)
#define C_KEY_1S5   (1500/C_TimeStandard)
#define C_KEY_2S    (2000/C_TimeStandard)

#define C_KEY_3S    (3000/C_TimeStandard)
#define C_KEY_5S    (5000/C_TimeStandard)

/*声明结构体======================================*/
/*按键时触发*/
struct	KeyEventDeal{
	u32 KeyValue;				//表示键码
	unsigned int FirstTriggerTime; 		//表示按键相应时间
	unsigned char IfRepeat;				//表示是否需要重复响应
	unsigned int RepeatTriggerTime;		//表示重复响应的间隔时间
	void (*voidKeyEvent)(void);			//表示响应对应的按键事件函数
	};

/*抬起按键时触发*/
struct	ReleseKeyEventDeal{
	u32 KeyValue;				//表示键码
	unsigned int MinReleseTime; 		//表示最小抬起时间
	unsigned int MaxReleseTime;			//表示最大抬起时间
	void (*voidReleseKeyEvent)(void);	//表示响应对应的抬起按键事件函数
	};
//----------按键响应参数配置部分需要你配置-------------------------

code	struct	KeyEventDeal  KeyEventDealCode[] = {
//按键码			按键相应时间	     重复响应否  后续再次响应时间 	响应的事件
	{KEY_LR, 					  C_KEY_60MS,		0,					C_KEY_1S5,			&F_RLOSC_AngleKey },
	{KEY_UP_DN, 				C_KEY_60MS,		0,					C_KEY_1S5,			&F_UpDnOscKey },
	{KEY_T_OFF, 				C_KEY_60MS,		0,					C_KEY_1S5,			&F_SetTimeOff },

	{KEY_DEC, 					C_KEY_60MS,		0,					C_KEY_1S5,			&F_WingLevelDec },
	{KEY_ONOFF, 				C_KEY_60MS,		0,					C_KEY_1S5,			&KeyOFFON },
	{KEY_INC, 					C_KEY_60MS,		0,					C_KEY_1S5,			&F_WingLevelInc },

	{KEY_T_ON, 					C_KEY_60MS,		0,					C_KEY_1S5,			&F_SetTimeOn },
	{KEY_NATURE, 				C_KEY_60MS,		0,					C_KEY_1S5,			&F_WingNature },
	{KEY_SLEEP_WIND, 		C_KEY_60MS, 	0,					C_KEY_1S5,			&F_WingSleep },

	{KEY_INC + KEY_DEC,		C_KEY_2S,	  0,					C_KEY_1S5,			&KeyFactory	},
 	{0,0,0,0,0} 
};
 
code	struct	ReleseKeyEventDeal  ReleseKeyEventDealCode[] = {
  //按键码         				抬起最小时间  		抬起最大时间				响应的事件
 	{0,0,0,0} 
};
