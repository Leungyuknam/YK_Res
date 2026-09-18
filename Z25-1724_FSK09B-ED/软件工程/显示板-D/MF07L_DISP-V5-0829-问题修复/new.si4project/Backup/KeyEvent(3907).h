#ifndef	_KeyEvent_H_
#define	_KeyEvent_H_



#ifdef _Native_KeyEvent_H
   #define	EXT_KEY_EVENT
#else
   #define	EXT_KEY_EVENT extern	
#endif
   
typedef enum				
{
	SysMX_FstPOW_ON,
	//SysMX_OFF,				
	//SysMX_IDLE,	
	//SysMX_Pause,
	SysMX_Run,				
	//SysMX_Burn, 			//老化测试		
	//SysMX_UserTest,
	SysMX_Factory,
	SysMX_Adjust,			//斜率调整模式
}SysMode_Enum;

typedef enum
{
	COOK_NONE,
	COOK_IDLE,
	COOK_RUN_PAUSE,
	//COOK_COMPLETE
}COOKState_Enum;

typedef enum
{
	SET_POW,
	SET_BOOK_TIME,
	SET_COOK_TIME,
	SET_TIMEKEEPING,
}SelectSet_Enum;

typedef enum
{
	MENU_NONE,
	HUOGUO,
	ZHENGZHU,
	MENGZHU,
	JIANKAO,
	BAOTANG,
	MANDUN
}SelectMenu_Enum;

typedef struct
{
    u8 HeatEn               		: 1;        //是否开启加热中,在COOK_RUN_PAUSE为0时表示进入暂停状态。
    u8 MenuSure                     : 1;        //菜单项是否确认了
  
	u8 TimekeepingEn                : 1;		//一键计时提醒是否开启
	u8 TimekeepingStart             : 1;		//一键计时提醒是否开始计时
	
	u8 BooktimeEn                	: 1;		//预约计时是否开启
	u8 BooktimeStart                : 1;		//预约计时是否开启开始计时
	
	u8 CooktimeEn                	: 1;		//烹饪倒计时是否开启
    u8 KeepWarmEn                	: 1;		//保温开启使能
} CookFlag;

typedef struct
{
	//----------------------------要记忆的数据-------------------------------------------------
	SelectMenu_Enum mMenu;			//当前选择的菜单
	COOKState_Enum	MState;			//当前烹饪的运行状态
	CookFlag	Fg;					//位标志状态量
	u16	EppromCookRunMin;			//用于记录烹饪时间 = CookRunSec/60
	u16 MenuWorkSec;				//菜单烹饪运行时间
	//------------------------------记忆数据截止-----------------------------------------------
	u32 CookRunSec;					//烹饪时间Sec
	u8  IDLE_Sec;					//待机计时
	u8 Err;							//烹饪故障代码
	u16 CookComplete_Sec;			//运行完成时间
	SelectSet_Enum TempTimeSet;		//温度、时间对象设置
} Fun_t;


#ifdef _Native_KeyEvent_H
	EXT_KEY_EVENT xdata u32 g_u32KeyBuf;
	EXT_KEY_EVENT idata SysMode_Enum SysMode;  // = SysMX_FstPOW_ON
	EXT_KEY_EVENT idata Fun_t Sys;
//	//默认起始功率
//	//									MENU_NONE		HUOGUO			ZHENGZHU		MENGZHU		JIANKAO		BAOTAN		MANDUN
//	u8 code MenuLevelDefTab []   = 		{POW_P10,		POW_P10,		POW_P8,			POW_P7,		TEMP_L8,	POW_P7,		POW_P4};
//	//各个菜单最大功率选择
//	u8 code MenuLevelMaxTab []   = 		{POW_P10,		POW_P10,		POW_P10,		POW_P8,		TEMP_L10,	POW_P8,		POW_P6};
//	//默认时间								
//	u32 code MenuCookTimeDefTab []   = 	{120*60,		120*60,			45*60,			40*60,		30*60,		60*60,		90*60};
//	//默认时间设定范围
//	u32 code MenuCookTimeMinTab []   = 	{CT_MIN,		CT_MIN,			CT_MIN,			CT_MIN,		CT_MIN,		CT_MIN,		CT_MIN};
//	u32 code MenuCookTimeMaxTab []   = 	{4*3600,		4*3600,			2*3600,			4*3600,		2*3600,		4*3600,		6*3600};
//	
//	//默认预定时间								
//	u32 code MenuBookTimeDefTab []   = 	{7200,			7200,			4*3600,			4*3600,		7200,		4*3600,		4*3600};
//	//默认预定时间范围		
//	u32 code MenuBookTimeMinTab []   = 	{BT_MIN,		BT_MIN,			BT_MIN,			BT_MIN,		BT_MIN,		BT_MIN,		2*60*60};
//	u32 code MenuBookTimeMaxTab []   = 	{BT_MAX,		BT_MAX,			BT_MAX,			BT_MAX,		BT_MAX,		BT_MAX,		BT_MAX};

#else
	EXT_KEY_EVENT idata SysMode_Enum SysMode;
	EXT_KEY_EVENT xdata u32 g_u32KeyBuf;
	EXT_KEY_EVENT idata Fun_t Sys;
#endif

#ifdef _Native_KeyEvent_H


#endif



EXT_KEY_EVENT u32 F_KeyScan();



#endif








