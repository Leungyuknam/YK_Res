//----------需要你配置的部分-----------------------------

//----------按键模块标准时间基准-------------------------
#define C_TimeStandard  10//标准时间单位MS 即F_KeyDealControl函数调用的时间间隔

//----------不需要你配置的部分---------------------------
#define C_KEY_60MS  (60/C_TimeStandard)
#define C_KEY_100MS (100/C_TimeStandard)
#define C_KEY_200MS (200/C_TimeStandard)
#define C_KEY_300MS (300/C_TimeStandard)
#define C_KEY_500MS (500/C_TimeStandard)
#define C_KEY_1S    (1000/C_TimeStandard)
#define C_KEY_1S5   (1500/C_TimeStandard)
#define C_KEY_2S    (2000/C_TimeStandard)
#define C_KEY_3S    (3000/C_TimeStandard)
#define C_KEY_5S    (5000/C_TimeStandard)

/*声明结构体======================================*/
/*按键时触发*/

#if (KEY_VALUE_LENGTH == LENGTH_32BIT)
struct	KeyEventDeal
{
	unsigned long int KeyValue;		    //表示键码
	unsigned int FirstTriggerTime; 		//表示按键相应时间
	unsigned char IfRepeat;				//表示是否需要重复响应
	unsigned int RepeatTriggerTime;		//表示重复响应的间隔时间
	void (*voidKeyEvent)(void);			//表示响应对应的按键事件函数
};

#elif (KEY_VALUE_LENGTH == LENGTH_16BIT)
struct	KeyEventDeal
{
	unsigned int KeyValue;		        //表示键码
	unsigned int FirstTriggerTime; 		//表示按键相应时间
	unsigned char IfRepeat;				//表示是否需要重复响应
	unsigned int RepeatTriggerTime;		//表示重复响应的间隔时间
	void (*voidKeyEvent)(void);			//表示响应对应的按键事件函数
};

#elif (KEY_VALUE_LENGTH == LENGTH_8BIT)
struct	KeyEventDeal
{
	unsigned char KeyValue;		        //表示键码
	unsigned int FirstTriggerTime; 		//表示按键相应时间
	unsigned char IfRepeat;				//表示是否需要重复响应
	unsigned int RepeatTriggerTime;		//表示重复响应的间隔时间
	void (*voidKeyEvent)(void);			//表示响应对应的按键事件函数
};
#endif



/*抬起按键时触发*/
#if (KEY_VALUE_LENGTH == LENGTH_32BIT)
struct	ReleseKeyEventDeal
{
	unsigned long int KeyValue;		    //表示键码
	unsigned int MinReleseTime; 		//表示最小抬起时间
	unsigned int MaxReleseTime;			//表示最大抬起时间
	void (*voidReleseKeyEvent)(void);	//表示响应对应的抬起按键事件函数
};

#elif (KEY_VALUE_LENGTH == LENGTH_16BIT)
struct	ReleseKeyEventDeal
{
	unsigned int KeyValue;		        //表示键码
	unsigned int MinReleseTime; 		//表示最小抬起时间
	unsigned int MaxReleseTime;			//表示最大抬起时间
	void (*voidReleseKeyEvent)(void);	//表示响应对应的抬起按键事件函数
};

#elif (KEY_VALUE_LENGTH == LENGTH_8BIT)
struct	ReleseKeyEventDeal
{
	unsigned char KeyValue;		        //表示键码
	unsigned int MinReleseTime; 		//表示最小抬起时间
	unsigned int MaxReleseTime;			//表示最大抬起时间
	void (*voidReleseKeyEvent)(void);	//表示响应对应的抬起按键事件函数
};
#endif

//----------按键响应参数配置部分需要你配置-------------------------
code	struct	KeyEventDeal  KeyEventDealCode[] = 
{
//按键码			   按键相应时间	重复响应否   后续再次响应时间 	  响应的事件
 {C_KEY_POWER_ONOFF,	C_KEY_2S,		0,			C_KEY_60MS,		&F_PowerOnOff		},
 
 {C_KEY_SELECT,			C_KEY_60MS,		0,			C_KEY_60MS,		&F_CoreSelect      	},   	  
 {C_KEY_SELECT,			C_KEY_3S,		0,			C_KEY_60MS,		&F_IntoCoreSel  	},
     
 {C_KEY_RESET,			C_KEY_3S,		0,			C_KEY_60MS,		&F_CoreLifeReset    },

 {C_KEY_SELFCHECK,		C_KEY_5S,		0,			C_KEY_60MS,		&F_IntoSelfCheck    },

 {0,0,0,0,0} 
};
		 
code	struct	ReleseKeyEventDeal  ReleseKeyEventDealCode[] = 
{
  //按键码         抬起最小时间       抬起最大时间		响应的事件
// {C_KEY_SET,	    C_KEY_60MS,			C_KEY_1S,		&F_LevelSelect},	
 {0,0,0,0} 
};
