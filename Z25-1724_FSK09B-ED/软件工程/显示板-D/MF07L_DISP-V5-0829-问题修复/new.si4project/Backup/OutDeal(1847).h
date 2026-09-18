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
	M_RST_NEED
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
	//接收的
	RecAngle_State UpDnAngleState;
	RecAngle_State LeftRightAngleState;
}Out_t;

//输出轴步距角度  3.75/42.5 
//=======================================================================
#define		c_RL_AngleRst		2992//2946	//264*42.5/3.75 = 3400 //上电或关机时复位的角度--2018	 
											//90度，齿轮比12；39  292.5  结构95度 
//复位是0~264（数值增加方向）
//30度摆动数值	85-174
#define		C30D_R				(2992-2028)		//离复位角度的远端角度	(264-174)~(264-85)
#define		C30D_L				(2992-1020)		//离复位角度的近端角度
//50度摆动数值 60-205
#define		C50D_R				(2992-2346)		//(264-205)~(264-57)
#define		C50D_L				(2992-669)
//80度摆动数值 10-252
#define		C80D_R				(2992-2856)		//(264-250)~(264-12)
#define		C80D_L				(2992-159)

//输出轴步距角度  7.5/85
//=======================================================================
#define		c_AngleRst			1201	//106*85/7.5 = 1201 //上电或关机时复位的角度--1130--全开位	 
	
	//30度摆动数值 85-174
#define		UPDN_UP				(1201-1077)		//离复位角度的远端角度	(106-89)~(106-11)
#define		UPDN_DN				(1201-193)		//离复位角度的近端角度


#ifdef _Native_OutDeal_H
	EXT_OUT_DEAL X_REG SysLoad_t SysLoadBuf = {0};
	EXT_OUT_DEAL X_REG SysLoad_t SysLoad = {0};
	EXT_OUT_DEAL u8 X_REG u8ShowBuf0 = 0;
	
	EXT_OUT_DEAL u8 B_FstHeatCheck = FALSE;
	EXT_OUT_DEAL Out_t X_REG Out = {0,0,0,0,0,0,0};
#else
	//EXT_OUT_DEAL X_REG u16 g_u16TimeToClose_Sec;
	EXT_OUT_DEAL X_REG SysLoad_t SysLoadBuf;
	EXT_OUT_DEAL X_REG SysLoad_t SysLoad;
	EXT_OUT_DEAL u8 X_REG u8ShowBuf0;
	EXT_OUT_DEAL u8 B_FstHeatCheck;
	EXT_OUT_DEAL Out_t X_REG Out;
#endif



EXT_OUT_DEAL void OutPutDeal(void);
EXT_OUT_DEAL void FactoryOutPutDeal(void);
EXT_OUT_DEAL void CustomerTestOutPutDeal(void);
#endif

