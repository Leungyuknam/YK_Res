#ifndef __FUNCTION_H_
#define __FUNCTION_H_

#define     __DEBUG__           0



#ifdef _Native_FUN_H
   #define	EXT_FUN
#else
   #define	EXT_FUN extern	
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

typedef struct
{
    u8 PowRecSIErr	               	: 1;        //接收通信
    u8 DoorState	               	: 1;        //门状态			0打开		1闭合
    u8 PowVer						: 6;		//程序版本号
} PowMessage_t;


typedef struct
{
	u16 	UpDnFarAngleVal;
	u16     UpDnNearAngleVal;
	TYPE_State	UpDnAngleCol;
	
	u16 	LeftRightFarAngleVal;
	u16     LeftRightNearAngleVal;
	TYPE_State	LeftRightAngleCol;
	
	u8	FanSpeed;  
	u8	LRHallState;
	//接收的
	RecAngle_State UpDnAngleState;
	RecAngle_State LeftRightAngleState;
	PowMessage_t PowMessage;
}Out_t;














#ifdef _Native_FUN_H
	EXT_FUN X_REG SysLoad_t SysLoadBuf = {0};
	EXT_FUN X_REG SysLoad_t SysLoad = {0};
	EXT_FUN u8 X_REG u8ShowBuf0 = 0;

	EXT_FUN u8 B_FstHeatCheck = FALSE;
	EXT_FUN Out_t X_REG Out = {0,0,0,0,0,0,0,0};

#else
	//EXT_OUT_DEAL X_REG u16 g_u16TimeToClose_Sec;
	EXT_FUN X_REG SysLoad_t SysLoadBuf;
	EXT_FUN X_REG SysLoad_t SysLoad;
	EXT_FUN u8 X_REG u8ShowBuf0;
	EXT_FUN u8 B_FstHeatCheck;
	EXT_FUN Out_t X_REG Out;
#endif







extern void F_FunCtrl(void);
extern void OutputDeal();
extern void F_ModeWork(void);

#endif



