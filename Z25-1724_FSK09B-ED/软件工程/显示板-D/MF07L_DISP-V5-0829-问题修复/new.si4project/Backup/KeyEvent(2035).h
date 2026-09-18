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
	MENU_BAOWEN,
	MENU_DIE_ZI,
	MENU_CUP,
}SelectMenu_Enum;

typedef enum				
{
	SET_STATE_NONE,				//回到待机
	SET_FST_ON,					//首次开机的状态
	SET_NOW_MENU_RESET,			//当前菜单未启动状态
	SET_RET_IDLE,				//当前模式返回设置状态
	SET_M_BAOWEN,				//设置选择保温菜单
	SET_M_DIE_ZI,				//设置选择碟子菜单
	SET_M_CUP,					//设置选择杯子菜单
}SetMode_Enum;


typedef struct
{
    u8 HeatEn               		: 1;        //是否开启加热中,在COOK_RUN_PAUSE为0时表示进入暂停状态。
    u8 MenuSure                     : 1;        //菜单项是否确认了
  
	u8 Lock                			: 1;		//是否童锁
	u8 TimekeepingStart             : 1;		//一键计时提醒是否开始计时
	
	u8 BooktimeEn                	: 1;		//预约计时是否开启
	u8 BooktimeStart                : 1;		//预约计时是否开启开始计时
	
	u8 CooktimeEn                	: 1;		//烹饪倒计时是否开启
    u8 KeepWarmEn                	: 1;		//保温开启使能
} CookFlag;

typedef struct
{
    u8 PowRecSIErr	               	: 1;        //接收通信
    u8 DoorState	               	: 1;        //门状态			0打开		1闭合
    u8 PowVer						: 6;		//程序版本号
} PowMessage_t;

typedef enum
{
	TEMP_40C,
	TEMP_50C,
	TEMP_60C,
	TEMP_70C
}Temp_Enum;

typedef struct
{
	//----------------------------要记忆的数据-------------------------------------------------
	SelectMenu_Enum mMenu;			//当前选择的菜单
	COOKState_Enum	MState;			//当前烹饪的运行状态
	CookFlag	Fg;					//位标志状态量
	u16	EppromCookRunMin;			//用于记录烹饪时间 = CookRunSec/60
	u16 MenuWorkSec;				//菜单烹饪运行时间
	Temp_Enum mTempLevel;			//温度菜单选择
	u8 Time;						//倒计时
	u8 TimeBuf;						//倒计时缓存
	u16 u16TimeContDown;			//倒计时时间
	//------------------------------记忆数据截止-----------------------------------------------
	u32 CookRunSec;					//烹饪时间Sec
	u8  IDLE_Sec;					//待机计时
	u8 Err;							//烹饪故障代码
	u16 CookComplete_Sec;			//运行完成时间
	SelectSet_Enum TempTimeSet;		//温度、时间对象设置
	PowMessage_t PowMessage;		//来自电源板的信息	
	u16 TempAD;						//温度的AD值,来自电源板子

} Fun_t;


//烹饪倒计时
#define	CT_MAX							24*60*60L	//*1S
#define	CT_MIN 							60			//*1S

#ifdef _Native_KeyEvent_H
	EXT_KEY_EVENT xdata u32 g_u32KeyBuf;
	EXT_KEY_EVENT idata SysMode_Enum SysMode = SysMX_FstPOW_ON;
	EXT_KEY_EVENT idata Fun_t Sys;

	//									MENU_BAOWEN		MENU_DIE_ZI		MENU_CUP	
	u8 code MenuCookTempDefTab []   = 	{TEMP_50C,		TEMP_50C,		TEMP_40C	};
	u8 code MenuCookTempMinTab []   = 	{TEMP_40C,		TEMP_40C,		TEMP_40C	};
	u8 code MenuCookTempMaxTab []   = 	{4*TEMP_70C,	TEMP_70C,		TEMP_60C	};


	//默认时间								
	u8 code MenuCookTimeDefTab []   = 	{3,				3,				3		};
	//默认时间设定范围
	u8 code MenuCookTimeMinTab []   = 	{1,				1,				1		};
	u8 code MenuCookTimeMaxTab []   = 	{8,				8,				8		};
#else
	EXT_KEY_EVENT idata SysMode_Enum SysMode;
	EXT_KEY_EVENT xdata u32 g_u32KeyBuf;
	EXT_KEY_EVENT idata Fun_t Sys;
#endif

#ifdef _Native_KeyEvent_H


#endif

#define KEY_CUP						BIT4
#define KEY_PLATE					BIT5
#define KEY_WARM					BIT6
#define KEY_TEMP 					BIT7
#define KEY_TIME 					BIT8
#define KEY_DEC						BIT9
#define KEY_INC 					BIT3
#define KEY_START_PAUSE				BIT2
#define KEY_POW						BIT1
#define KEY_LOCK					BIT0




EXT_KEY_EVENT u32 F_KeyScan();
EXT_KEY_EVENT void WorkModeNewSet(SetMode_Enum mNewDat);

EXT_KEY_EVENT void KeyPow(void);
EXT_KEY_EVENT void KEY_Lock(void);
EXT_KEY_EVENT void KEY_StartPause(void);
EXT_KEY_EVENT void KEY_Dec(void);
EXT_KEY_EVENT void KEY_Inc(void);
EXT_KEY_EVENT void KEY_Time(void);
EXT_KEY_EVENT void KEY_Temp(void);
EXT_KEY_EVENT void KeyBaoWen(void);
EXT_KEY_EVENT void KeyDieZi(void);
EXT_KEY_EVENT void KeyCup(void);



#endif








