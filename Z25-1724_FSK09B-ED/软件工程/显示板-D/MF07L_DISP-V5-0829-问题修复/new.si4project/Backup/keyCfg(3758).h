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

//Key1 		Key2 	Key3	Key4	Key5 	Key6	Key7	Key8	Key9 	Key10
//PIN24		PIN23	PIN22	PIN21	PIN20	PIN19	PIN25	PIN26	PIN27	PIN28
//TK4		TK5		TK6		TK7		TK8		TK9		TK3		TK2		TK1		TK0

//F_UserTest  长按进入滤网加速 
code	struct	KeyEventDeal  KeyEventDealCode[] = {
//按键码			按键相应时间	重复响应否  后续再次响应时间 	响应的事件
//	{KEY_SHORTCUT_T,			C_KEY_2S,		0,					C_KEY_1S5,			&KeyTimekeepingOFF	},
	{KEY_CUP,					C_KEY_60MS,		0,					C_KEY_1S5,			&KeyCup	},
	{KEY_PLATE,					C_KEY_60MS,		0,					C_KEY_1S5,			&KeyDieZi	},
	{KEY_WARM,					C_KEY_60MS,		0,					C_KEY_1S5,			&KeyBaoWen	},
	{KEY_TEMP,					C_KEY_60MS,		0,					C_KEY_1S5,			&KEY_Temp	},

	{KEY_TIME,					C_KEY_60MS,		0,					C_KEY_1S5,			&KEY_Time	},
	{KEY_INC,					C_KEY_60MS,		0,					C_KEY_1S5,			&KEY_Inc 	},
	{KEY_DEC,					C_KEY_60MS,		0,					C_KEY_1S5,			&KEY_Dec 	},

	{KEY_START_PAUSE,			C_KEY_60MS,		0,					C_KEY_1S5,			&KEY_StartPause	},

	
	{KEY_POW,					C_KEY_2S,		0,					C_KEY_1S5,			&KeyPowFstOn	},
	{KEY_LOCK,					C_KEY_2S,		0,					C_KEY_1S5,			&KEY_Lock	},

	{KEY_TIME + KEY_POW,		C_KEY_3S,		0,					C_KEY_1S5,			&KeyFactory	},


	

 	{0,0,0,0,0} 
};
 
code	struct	ReleseKeyEventDeal  ReleseKeyEventDealCode[] = {
  //按键码         				抬起最小时间  		抬起最大时间				响应的事件
	{KEY_POW,					C_KEY_60MS,		C_KEY_2S,			&KeyPowToIDLE	},


 	{0,0,0,0} 
};
