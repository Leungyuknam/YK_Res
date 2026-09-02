

typedef struct{
	U16 keyCode;
	U16  KeyConfrimTime;   //按键确认时间
	U16  lopKey;
	U16  lopKeyInnerTime;
	voidFan passKeyFun;
}strPassKeyCfgTable;

typedef struct{
	U16 keyCode;
	U16  releaseMixTime;
	U16  releaseMaxTime;
	voidFan releaseFun;
}strReleaseKeyCfgTable;

//----------需要你配置的部分-----------------------------
//声明你的按键处理函数

//----------按键模块标准时间基准-------------------------
#define C_TimeStandard  10//标准时间单位MS 即F_KeyDealControl函数调用的时间间隔

//----------下面是按键的码值-----------------------------

//----------不需要你配置的部分---------------------------
#define C_KEY_20MS  (20/C_TimeStandard)
#define C_KEY_30MS  (30/C_TimeStandard)
#define C_KEY_60MS  (60/C_TimeStandard)
#define C_KEY_100MS (100/C_TimeStandard)
#define C_KEY_200MS (200/C_TimeStandard)
#define C_KEY_400MS (400/C_TimeStandard)
#define C_KEY_500MS (500/C_TimeStandard)
#define C_KEY_1S    (1000/C_TimeStandard)
#define C_KEY_2S    (2000/C_TimeStandard)
#define C_KEY_3S    (3000/C_TimeStandard)
#define C_KEY_5S    (5000/C_TimeStandard)
#define C_KEY_10S   (10000/C_TimeStandard)
#define C_KEY_20S   (20000/C_TimeStandard)
//----------按键响应参数配置部分需要你配置-------------------------
/* 使用说明 KeyCodeTable[][]  第一维的编号与 KeyEventNumberTable的编号对应。
   KeyCodeTable[n][0]  表示键码
   KeyCodeTable[n][1]  表示按键相应时间
   KeyCodeTable[n][2]  表示是否需要重复响应
   KeyCodeTable[n][3]  表示重复响应的间隔时间
   KeyEventDealTable[] 按顺序按键响应函数名称 
*/ 

strPassKeyCfgTable code  T_passKeyTable[] ={
    //按键码			按键相应时间	重复响应否  后续再次响应时间 	响应的事件
    {c_KEY_WIFI,		C_KEY_3S,		0,			C_KEY_100MS,		&F_WifiLongKey	    },
    {c_KEY_LOCK,		C_KEY_2S,		0,			C_KEY_100MS,		&F_LockLongKey      },
    {c_KEY_FILTER, 	    C_KEY_3S,		0,			C_KEY_200MS,		&F_FilterLongKey    },
	{c_KEY_TIME,		C_KEY_3S,		0,			C_KEY_500MS,		&F_TimeLongKey 	    },
    {c_KEY_FILTER,		C_KEY_20S,	    0,		    C_KEY_200MS,	    &F_FilterLong20sKey },
    {C_KEY_QUICK_TEST,	C_KEY_3S,		0,			C_KEY_100MS,		&F_QuickTestKey     },
    {C_KEY_TEST,        C_KEY_3S,		0,			C_KEY_100MS,		&F_SelfTestKey      },
		
		
	{c_KEY_POWER,		C_KEY_60MS,	0,	C_KEY_500MS,		&F_PowerKey 	},
//    {c_KEY_LOCK,		C_KEY_60MS,0,		C_KEY_500MS,		&F_LockKey 	    },
    {C_KEY_CHANGE_RGB,	C_KEY_60MS,	0,	C_KEY_500MS,		&F_RgbChangeKey 	},
    {c_KEY_MODE,		C_KEY_60MS,	0,	C_KEY_500MS,		&F_ModeKey 	    },
    {c_KEY_WIFI,		C_KEY_60MS,	0,	C_KEY_500MS,		&F_WifiKey 	    },
//    {c_KEY_TIME,		C_KEY_60MS,	0,	C_KEY_500MS,		&F_TimeKey 	    },
	{c_KEY_ION,		  C_KEY_60MS,	0,	C_KEY_500MS,		&F_IonKey 	    },
    {0,0,0,0,0}
};

strReleaseKeyCfgTable code T_releaseKeyTable[]={
    //按键码			按键响应时间	按键响应最长时间 	响应的事件 

//		{c_KEY_POWER,		C_KEY_60MS,	C_KEY_500MS,		&F_PowerKey 	},
    {c_KEY_LOCK,		C_KEY_60MS,		C_KEY_500MS,		&F_LockKey 	    },
//    {C_KEY_CHANGE_RGB,	C_KEY_60MS,	C_KEY_500MS,&F_RgbChangeKey 	},
//    {c_KEY_MODE,		C_KEY_60MS,	C_KEY_500MS,		&F_ModeKey 	    },
//    {c_KEY_WIFI,		C_KEY_60MS,	C_KEY_500MS,		&F_WifiKey 	    },
    {c_KEY_TIME,		C_KEY_60MS,	C_KEY_500MS,		&F_TimeKey 	    },
    
	{0,0,0,0}
};

