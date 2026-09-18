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
#define C_KEY_8S    (8000/C_TimeStandard)
#define C_KEY_15S   (15000/C_TimeStandard)

/*声明结构体======================================*/
/*按键时触发*/

#if (KEY_NUMBERS > 16)
struct	KeyEventDeal
{
	unsigned long int KeyValue;		    //表示键码
	unsigned int FirstTriggerTime; 		//表示按键相应时间
	unsigned char IfRepeat;				//表示是否需要重复响应
	unsigned int RepeatTriggerTime;		//表示重复响应的间隔时间
	void (*voidKeyEvent)(void);			//表示响应对应的按键事件函数
};

#elif (KEY_NUMBERS > 8)
struct	KeyEventDeal
{
	unsigned int KeyValue;		        //表示键码
	unsigned int FirstTriggerTime; 		//表示按键相应时间
	unsigned char IfRepeat;				//表示是否需要重复响应
	unsigned int RepeatTriggerTime;		//表示重复响应的间隔时间
	void (*voidKeyEvent)(void);			//表示响应对应的按键事件函数
};

#else
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
#if (KEY_NUMBERS > 16)
struct	ReleseKeyEventDeal
{
	unsigned long int KeyValue;		    //表示键码
	unsigned int MinReleseTime; 		//表示最小抬起时间
	unsigned int MaxReleseTime;			//表示最大抬起时间
	void (*voidReleseKeyEvent)(void);	//表示响应对应的抬起按键事件函数
};

#elif (KEY_NUMBERS > 8)
struct	ReleseKeyEventDeal
{
	unsigned int KeyValue;		        //表示键码
	unsigned int MinReleseTime; 		//表示最小抬起时间
	unsigned int MaxReleseTime;			//表示最大抬起时间
	void (*voidReleseKeyEvent)(void);	//表示响应对应的抬起按键事件函数
};

#else
struct	ReleseKeyEventDeal
{
	unsigned char KeyValue;		        //表示键码
	unsigned int MinReleseTime; 		//表示最小抬起时间
	unsigned int MaxReleseTime;			//表示最大抬起时间
	void (*voidReleseKeyEvent)(void);	//表示响应对应的抬起按键事件函数
};
#endif

//----------按键响应参数配置部分需要你配置-------------------------
code struct	KeyEventDeal  KeyEventDealCode[] = 
{
//按键码			   按键相应时间	重复响应否   后续再次响应时间 	  响应的事件
// {C_KEY_ONOFF,	        C_KEY_1S,		0,			C_KEY_60MS,		&F_PowerOn		}, 
 {C_KEY_ONOFF,	        C_KEY_60MS,		0,			C_KEY_60MS,		&F_PowerOff		}, 	  
 {C_KEY_SET,			C_KEY_60MS,		0,			C_KEY_60MS,		&F_SetBeep	},
 {C_KEY_SET,			C_KEY_1S,		0,			C_KEY_60MS,		&F_KeyEvent_Unlock	}, 
 {C_KEY_SET,			C_KEY_3S,		0,			C_KEY_60MS,		&F_ModeSetChange	},
// {C_KEY_RESET_CLOSE,	C_KEY_3S,		0,			C_KEY_60MS,		&F_SystemReset   	},
      
 {C_KEY_DOWN_L,			C_KEY_60MS,		0,			C_KEY_60MS,		&F_SetBeep      	},
 {C_KEY_DOWN_L,			C_KEY_1S5,		1,			C_KEY_200MS,	&F_LongDownSet		},
// {C_KEY_DOWN_R,			C_KEY_60MS,		0,			C_KEY_60MS,		&F_DownSet_R      	},
// {C_KEY_DOWN_R,			C_KEY_1S5,		1,			C_KEY_200MS,	&F_LongDownSet_R	},
    
 {C_KEY_UP_L,		    C_KEY_60MS,		0,			C_KEY_100MS,	&F_SetBeep        },
 {C_KEY_UP_L,			C_KEY_1S5,		1,			C_KEY_200MS,	&F_LongUpSet   	},
// {C_KEY_UP_R,		    C_KEY_60MS,		0,			C_KEY_100MS,	&F_UpSet_R      },
// {C_KEY_UP_R,			C_KEY_1S5,		1,			C_KEY_200MS,	&F_LongUpSet_R  },
 {C_KEY_VERDISP,	    C_KEY_3S,		0,			C_KEY_200MS,	&F_IntoVersion  },

// {C_KEY_SELFCHECK,		C_KEY_3S,		0,			C_KEY_60MS,	    &F_IntoSelfCheck	},

// {C_KEY_CH_SET,			C_KEY_15S,		0,			C_KEY_200MS,	&F_IntoSensorChannelSet},

 {C_KEY_SPEED,			C_KEY_1S,		0,			C_KEY_60MS,		&F_LevelSelect      	},
 {C_KEY_PROTECT,		C_KEY_1S,		0,			C_KEY_60MS,		&F_KeyEvent_SetProtect      	},
 
 {0,0,0,0,0} 
};
		 
code	struct	ReleseKeyEventDeal  ReleseKeyEventDealCode[] = 
{
  //按键码         抬起最小时间       抬起最大时间		响应的事件
// {C_KEY_SET,	    C_KEY_60MS,			C_KEY_1S,		&F_LevelSelect},
 {C_KEY_DOWN_L,	    C_KEY_60MS,			C_KEY_1S,		&F_DownSet},
 {C_KEY_UP_L,	    C_KEY_60MS,			C_KEY_1S,		&F_UpSet},
 	
 {0,0,0,0} 
};
