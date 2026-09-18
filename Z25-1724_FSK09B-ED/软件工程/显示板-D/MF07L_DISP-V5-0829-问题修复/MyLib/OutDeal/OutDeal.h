#ifndef _OUTDEAL_H_    
#define _OUTDEAL_H_

#ifdef _Native_OutDeal_H
	#define	EXT_OUT_DEAL
#else
   #define	EXT_OUT_DEAL extern	
#endif

typedef union {
	struct {
		u8 Heat 						: 1;		//加热
		u8 Fan							: 1;		//风扇
		u8 Light						: 1;		//指示灯
		u8 WarmHeat 					: 1;		//较低功率加热
		u8 None 						: 4;		//None
	} Bits;
	unsigned char Byte;
} SysLoad_t;

typedef enum
{
	M_NORMAL,
	M_RST_NEED,
	M_MID_NEED,
}MotorSetCol_Enum;


typedef enum
{
	Rst_No,
	Rst_Need,
	Rst_Ing,
	Rst_Ok,
}MotorRecSate_Enum;





typedef	struct	_TYPE_State
{
	unsigned char	none:			2;
	unsigned char	AngleEn:		1;
	unsigned char	SetCol:			3;	//设置步进电机命令
	unsigned char	RollCode:		2;	//设置步进电机命令滚码
}TYPE_State;

typedef	struct	_RecAngle_State
{
	unsigned char	none:			2;
	unsigned char	AngleEn:		1;	
	unsigned char	NowStep:		3;	//当前步进电机状态
	unsigned char	RollCode:		2;	//当前步进电机执行命令状态
}RecAngle_State;


typedef struct
{
	u16 	UpDnFarAngleVal;
	u16     UpDnNearAngleVal;
	TYPE_State	UpDnAngleCol;
	
	u16 	LeftRightFarAngleVal;
	u16     LeftRightNearAngleVal;
	TYPE_State	LeftRightAngleCol;
	
	FanLevel_Enum	mFanSpeed;  
	u8	LRHallState;
	//接收的
	RecAngle_State UpDnAngleState;
	RecAngle_State LeftRightAngleState;
}Out_t;

#define		C_FST_TIME        30//*100MS



#define c_UD_AngleRst	2040

#define UPDN_UP30   850u   // 左端（角度小的那个）
#define UPDN_DN30   1190u  // 右端

#define UPDN_UP60   680u
#define UPDN_DN60   1360u

#define UPDN_UP90   510u
#define UPDN_DN90   1530u

#define UPDN_UP180  0u
#define UPDN_DN180  2040u



#define c_LR_AngleRst   4080u   // 360° 全程

// 45° 左右摇头（±22.5°）
#define LR_UP45         1785u
#define LR_DN45         2295u

// 90° 左右摇头（±45°）
#define LR_UP90         1530u
#define LR_DN90         2550u

// 180° 左右摇头（±90°）
#define LR_UP180        1020u
#define LR_DN180        3060u

// 360° 左右摇头（±180°）
#define LR_UP360        0u
#define LR_DN360        4080u



#ifdef _Native_OutDeal_H
	EXT_OUT_DEAL X_REG SysLoad_t SysLoadBuf = {0};
	EXT_OUT_DEAL X_REG SysLoad_t SysLoad = {0};
	EXT_OUT_DEAL u8 X_REG u8ShowBuf0 = 0;
	
	EXT_OUT_DEAL u8 B_FstHeatCheck = FALSE;
	EXT_OUT_DEAL Out_t X_REG Out = {0,0,0,0,0,0,0,0};
#else
	//EXT_OUT_DEAL X_REG u16 g_u16TimeToClose_Sec;
	EXT_OUT_DEAL X_REG SysLoad_t SysLoadBuf;
	EXT_OUT_DEAL X_REG SysLoad_t SysLoad;
	EXT_OUT_DEAL u8 X_REG u8ShowBuf0;
	EXT_OUT_DEAL u8 B_FstHeatCheck;
	EXT_OUT_DEAL Out_t X_REG Out;
#endif


EXT_OUT_DEAL void OutPutAllOFF(void);
EXT_OUT_DEAL void OutPutDeal(void);
EXT_OUT_DEAL void FactoryOutPutDeal(void);
EXT_OUT_DEAL void CustomerTestOutPutDeal(void);
#endif

