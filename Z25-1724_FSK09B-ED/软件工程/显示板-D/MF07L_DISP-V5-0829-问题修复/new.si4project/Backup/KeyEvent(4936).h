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
	//SysMX_CustomerTest,
}SysMode_Enum;

typedef enum
{
	STATE_NONE,
	STATE_WORK,
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
	SET_OFF,					//进入关机，保留不断电记忆的
}SetMode_Enum;


typedef struct
{
    u8 HeatEn               		: 1;        //是否开启加热中,在COOK_RUN_PAUSE为0时表示进入暂停状态。
    u8 MenuSure                     : 1;        //菜单项是否确认了
  
	u8 Lock                			: 1;		//是否童锁

	
	u8 CooktimeEn                	: 1;		//烹饪倒计时是否开启
    u8 KeepWarmEn                	: 1;		//保温开启使能

	u8 none							: 3;
} CookFlag;

typedef struct
{
	u8 Err                			: 1;		//故障标志
	u8 ErrBuzzDisEn                	: 1;		//禁止故障蜂鸣
	u8 HadWork		                : 1;		//当COOK_IDLE状态下，此时HadWork为1,则视为临时暂停，风扇需保持运行，无则停风扇输出
	u8 none							: 5;
} Flag_t;


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

typedef enum
{
	ANGLE_30,
	ANGLE_50,
	ANGLE_80,

	ANGLE_OFF//new
}Angle_Enum;

typedef	struct	_TYPE_SET_FLAG
{
	unsigned char	TurnOn:			1;
	unsigned char	UDOSC:			1;
	unsigned char	RLOSC:			1;
	unsigned char	Lock:			1;
	unsigned char	LightSleep:		1;
	unsigned char	FanSleep:		1;
	unsigned char	d:				1;
	unsigned char	e:				1;	
}TYPE_SET_FLAG;



typedef enum
{
	FAN_OFF,
	FAN_ONE,
	FAN_TWO,
	FAN_THREE,
	FAN_FOUR,
	FAN_FIVE,
	FAN_SIX,
	FAN_SEVEN
}FanLevel_Enum;

typedef enum
{
	FAN_NORMAL,
	FAN_NATURE,
}FanMode_Enum;

typedef enum
{
	T_ON_OFF,
	T_ON_TWO,
	T_ON_FOUR,
	T_ON_SIX,
}TimeOnSet_Enum;

typedef enum
{
	T_OFF_OFF,
	T_OFF_ONE,
	T_OFF_TWO,
	T_OFF_FOR,
}TimeOFFSet_Enum;

typedef	struct	_TYPE_SET_Err
{
	unsigned char	PowVolLow:			1;
	unsigned char	BitBuf:				1;
	unsigned char	StoreBit:			1;
	unsigned char	None:				5;
}TYPE_Err_t;

typedef struct
{
	//----------------------------要记忆的数据-------------------------------------------------
	TYPE_SET_FLAG 	Fg;
	FanLevel_Enum	mFan;
	unsigned char 	RecordFan;
	unsigned char	Time;			//关机倒计时
	unsigned int	TimeContDown;	//关机机倒计秒时
		
	unsigned char	TimeOnCount;	//倒计时
	unsigned char	TimeBuf;		//倒计时缓存时间
	unsigned int	TimeContOn;		//开机倒计秒时

	unsigned char	SleepHalfHour;	//睡眠计时
	unsigned int	SleepSec;		//运行秒计时
	
	TimeOnSet_Enum	mFanOnTime;		
	TimeOFFSet_Enum	mFanOFF_Time;	
	
	unsigned char   Edition;
	Angle_Enum		mAngle;
	Angle_Enum		mAngleUpDn;//new 0826
	unsigned char   mHeatFanDelaySec;
	COOKState_Enum	MState;			//当前运行状态
	FanMode_Enum	Mode;			//运行模式
	Flag_t Sign;					//
	TYPE_Err_t 		Err;			//故障代码
	
	//------------------------------记忆数据截止-----------------------------------------------
	u32 CookRunSec;					//烹饪时间Sec
	u8  IDLE_Sec;					//待机计时
	u16 RunOpenDoorSec;				//运行过程中，门开的时间
	u16 CookComplete_Sec;			//运行完成时间
	SelectSet_Enum TempTimeSet;		//温度、时间对象设置
	PowMessage_t PowMessage;		//来自电源板的信息
	u8 PowSIErrCnt;					//电源接收故障累计时间
	u16 TempAD;						//温度的AD值,来自电源板子

} Fun_t;


//烹饪倒计时
#define	CT_MAX							24*60*60L	//*1S
#define	CT_MIN 							60			//*1S

#ifdef _Native_KeyEvent_H
	EXT_KEY_EVENT xdata u32 g_u32KeyBuf;
	EXT_KEY_EVENT idata SysMode_Enum SysMode = SysMX_FstPOW_ON;
	EXT_KEY_EVENT idata Fun_t Sys;

#else
	EXT_KEY_EVENT idata SysMode_Enum SysMode;
	EXT_KEY_EVENT xdata u32 g_u32KeyBuf;
	EXT_KEY_EVENT idata Fun_t Sys;
#endif

#ifdef _Native_KeyEvent_H


#endif

#define	KEY_EFF()			{Sys.IDLE_Sec = 0;}		//按键有效效应


#define KEY_LR						BIT19
#define KEY_UP_DN					BIT7
#define KEY_T_OFF					BIT5

#define KEY_DEC  					BIT4
#define KEY_ONOFF 				BIT8
#define KEY_INC						BIT15

#define KEY_T_ON 					BIT14
#define KEY_NATURE				BIT6
#define KEY_SLEEP_WIND		BIT18





EXT_KEY_EVENT u32 F_KeyScan();
EXT_KEY_EVENT void WorkModeNewSet(SetMode_Enum mNewDat);
EXT_KEY_EVENT void KeyFactory(void);


EXT_KEY_EVENT void KeyOFFON(void);
EXT_KEY_EVENT void F_WingLevelInc(void);
EXT_KEY_EVENT void F_WingLevelDec(void);
EXT_KEY_EVENT void F_WingNature(void);
EXT_KEY_EVENT void F_WingSleep(void);
EXT_KEY_EVENT void F_SetTimeOff(void);
EXT_KEY_EVENT void F_SetTimeOn(void);
EXT_KEY_EVENT void F_RLOSC_AngleKey();
EXT_KEY_EVENT void F_UpDnOscKey();
EXT_KEY_EVENT void F_WingIR(void);


#endif








