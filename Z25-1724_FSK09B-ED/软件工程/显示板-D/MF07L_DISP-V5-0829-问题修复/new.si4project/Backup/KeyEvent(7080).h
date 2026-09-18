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
	SET_OFF,
	SET_ON_INIT,
	SET_GRIDDLE
}NewWorkSet_Enum;


typedef enum				
{
	SET_STATE_NONE,				//回到待机
	SET_FST_ON,					//首次开机的状态
	SET_NOW_MENU_RESET,			//当前菜单未启动状态
	//SET_RET_IDLE		//当前模式返回设置状态
	//SET_KEY_IDLE_EFF,	//待机按下重置计时
}SetMode_Enum;

typedef enum
{
	COOK_NONE,
	COOK_IDLE,
	COOK_RUN_PAUSE,
	//COOK_COMPLETE
}COOKState_Enum;

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
#define	BIT_MENU_SURE				0x02		//记忆除了菜单项可以选中，其他不需要了
#define	BIT_BOOK_COOK_T_SIGN		0x50		//考量下要保存处理的标志位，其余重置为0

typedef struct
{
	u8 NoPot                    	: 1;        //无锅
    u8 None							: 7;		//None
} CookSign;



typedef enum
{
	SET_POW,
	SET_BOOK_TIME,
	SET_COOK_TIME,
	SET_TIMEKEEPING,
}SelectSet_Enum;



typedef enum {
	NONE_SET,				//尚未有输入数据
	HAD_SET					//有数据输入
}SetSign_Enum;

typedef enum				
{
	CODE_DEC,
	CODE_INC
}Col_Enum;

typedef enum
{
	//MENU_NONE,
	HUOGUO,
	ZHENGZHU,
	MENGZHU,
	JIANKAO,
	BAOTANG,
	MANDUN
}SelectMenu_Enum;

typedef enum
{
	POW_P1, //200
	POW_P2, //400
	POW_P3,	//600
	POW_P4, //800
	POW_P5, //1000
	POW_P6,
	POW_P7,
	POW_P8,
	POW_P9,
	POW_P10
}Pow_Enum;

typedef enum
{
	TEMP_L1,
	TEMP_L2,
	TEMP_L3,
	TEMP_L4,
	TEMP_L5,
	TEMP_L6,
	TEMP_L7,
	TEMP_L8,
	TEMP_L9,
	TEMP_L10,
}Temp_Enum;




typedef struct
{
	SelectMenu_Enum mMenu;			//当前选择的菜单
	COOKState_Enum	MState;			//当前烹饪的运行状态
	CookFlag	Fg;					//位标志状态量
	CookSign	Sign;				//位标志状态量
	u16 CookLevel;					//烹饪功率档位
	u16	EppromBookRunSec;			//用于记录预约时间 = BookRunSec/60
	u16	EppromCookRunMin;			//用于记录烹饪时间 = CookRunSec/60
	u32 BookRunSec;					//预约时间Sec
	u32 CookRunSec;					//烹饪时间Sec
	u16 MenuWorkSec;				//菜单烹饪运行时间
	u8  MenuWorkStep;				//菜单烹饪运行步骤
	u16 TKeepSec;					//计时提醒时间
	u8  IDLE_Sec;					//待机计时
	SetMode_Enum 	Mode;			//按键操作进入的模式
	u8 Err;							//烹饪故障代码
	u16 CookComplete_Sec;			//运行完成时间
	SelectSet_Enum TempTimeSet;		//温度、时间对象设置
	u8 TempHigh_Cnt;
	//u8 TempLow_Cnt;
	u8 Count_IgbtDown;				//IGBT功率下降次数
	u8 ComSlopeCurr_Adjust;			//斜率调整值。
	u8 ShowEppSlope;				//显示当前电源斜率调整值。
	u8 CheckPotCnt;					//检锅判断时间
} Fun_t;

#define	MAX_POW_LEVEL			13


	
#define	TIME_SHOW_TIME					5//*100MS 显示定时时间		
#define	CLEAN_TIME						3000
	
#define	MAX_TEMP_VAL					450	//最大温度
#define	MIN_TEMP_VAL					200	//最小温度
	
	//计时提醒默认设定时间
#define	HUOGUO_TIMEKEEP_INIT			10			//*1S  
#define	JIANKAO_TIMEKEEP_INIT			10			//*1S

//计时提醒
//#define	TK_MAX							30*60		//*1S
//#define	TK_MIN 							1			//*1S
//#define STEP_TK_S						1			//*1S 	短按时间步差
//#define STEP_TK_L						10			//*1S 	长按时间步差
//#define STEP_TK_MIN						60			//*1S 	时间有MIN的步差
//#define	MIN_VAL							60

#define MAX_TKEEP_TIME					59*60+59	//*1S




//烹饪倒计时
#define	CT_MAX							24*60*60L	//*1S
#define	CT_MIN 							0//60			//*1S
#define STEP_CT_S						60			//*1S 	短按时间步差
#define STEP_CT_L						600			//*1S 	长按时间步差


//预约计时
#define	BT_MAX							24*60*60L	//*1S
#define	BT_MIN 							1*60*60		//*1S
#define STEP_BT_S						60*30		//*1S 	短按时间步差
#define STEP_BT_L						60*30		//*1S 	长按时间步差

#define	NONE							0

#ifdef _Native_KeyEvent_H
	EXT_KEY_EVENT xdata u32 g_u32KeyBuf = 0;
	EXT_KEY_EVENT idata SysMode_Enum SysMode = SysMX_FstPOW_ON;
	EXT_KEY_EVENT idata Fun_t Sys = {0};
	//默认起始功率
	//									HUOGUO			ZHENGZHU		MENGZHU		JIANKAO		BAOTAN		MANDUN
	u8 code MenuLevelDefTab []   = 		{POW_P10,		POW_P8,			POW_P7,		TEMP_L8,	POW_P7,		POW_P4};
	//各个菜单最大功率选择
	u8 code MenuLevelMaxTab []   = 		{POW_P10,		POW_P10,		POW_P8,		TEMP_L10,	POW_P8,		POW_P6};
	//默认时间								
	u32 code MenuCookTimeDefTab []   = 	{120*60,		45*60,			40*60,		30*60,		60*60,		90*60};
	//默认时间设定范围
	u32 code MenuCookTimeMinTab []   = 	{CT_MIN,		CT_MIN,			CT_MIN,		CT_MIN,		CT_MIN,		CT_MIN};
	u32 code MenuCookTimeMaxTab []   = 	{4*3600,		2*3600,			4*3600,		2*3600,		4*3600,		6*3600};
	
	//默认预定时间								
	//u32 code MenuBookTimeDefTab []   = 	{3600,			3600,			3600,		3600,		3600,		3600};
	u32 code MenuBookTimeDefTab []   = 	{7200,			2*3600,			4*3600,		7200,		4*3600,		4*3600};
	//默认预定时间范围		
	u32 code MenuBookTimeMinTab []   = 	{BT_MIN,		BT_MIN,			BT_MIN,		BT_MIN,		BT_MIN,		BT_MIN};
	u32 code MenuBookTimeMaxTab []   = 	{BT_MAX,		BT_MAX,			BT_MAX,		BT_MAX,		BT_MAX,		BT_MAX};

	//煎烤温度
	#define	JIANKAO_DEF_LEVEL			TEMP_L9		
	//									L1		L2		L3		L4		L5		L6		L7		L8		L9		L10
	u8  code TempShowTab[]			=  {60,		80,		110,	120,	140	,	160,	180,	200,	220,	240};	
#else
	EXT_KEY_EVENT idata SysMode_Enum SysMode;
	EXT_KEY_EVENT xdata u32 g_u32KeyBuf;
	EXT_KEY_EVENT idata Fun_t Sys;
	EXT_KEY_EVENT u8  code TempShowTab[];	
#endif






#define	TIME_STEP_VAL			10
#define	MIN_TIME_VAL			TIME_STEP_VAL
#define	MAX_TIME_VAL			90*60



//----------------------------------------------------------------------

//----------------------------------------------------------------------

#ifdef _Native_KeyEvent_H


#endif



EXT_KEY_EVENT u32 F_KeyScan();
EXT_KEY_EVENT void WorkModeNewSet(SetMode_Enum mNewDat);


extern void KeyInc(void);
extern void KeyLongInc(void);
extern void KeyDec(void);
extern void KeyLongDec(void);
extern void KeyPauseStart(void);
extern void KeyPowOFF(void);
extern void KeyMenuSelect(void);
extern void KeyTimekeeping(void);
extern void KeyBooktime(void);
extern void KeyCooktime(void);
extern void KeyTimekeepingOFF(void);

extern void KeyAdjust(void);
extern void KeyFactory(void);



#endif








