#ifndef __FUNCTION_H_
#define __FUNCTION_H_

#define     __DEBUG__           0



#ifdef _Native_FUN_H
   #define	EXT_FUN
#else
   #define	EXT_FUN extern	
#endif


typedef enum				
{	
	SW_OPEN,
	SW_CLOSE,
}SW_Status_Enum;








//AC同步电机
typedef enum				
{	
	AC_NONE,
	AC_CLOCK_WISE,
	AC_AN_CLK_WISE,
	AC_ALL_WISE
}MotorAC_Enum;


typedef struct
{
    u8 Heat	               			: 1;        //加热
	u8 Fan							: 1;		//风扇
	u8 Light						: 1;		//指示灯
	u8 WarmHeat						: 1;		//较低功率加热
	u8 None							: 4;		//None
} Load_t;

typedef struct
{
    u8 PowRecSIErr	               	: 1;        //接收通信
    u8 DoorState	               	: 1;        //门状态			0打开		1闭合
    u8 PowVer						: 6;		//程序版本号
} PowMessage_t;



typedef struct
{  
	Load_t Load;					
	PowMessage_t	PowMessage;				
}Sys_TypeDef;

/*************************需配置部分***********************/
//检测口         
#define 	HEAT_ON()             			{P_HEAT = 1;}
#define 	HEAT_OFF()            			{P_HEAT = 0;}

#define 	WARM_HEAT_ON()             		{P_WarmHeat = 1;}
#define 	WARM_HEAT_OFF()            		{P_WarmHeat = 0;}

#define 	LIGHT_ON()             			{P_Light = 1;}
#define 	LIGHT_OFF()            			{P_Light = 0;}

#define 	FAN_ON()             			{P_FAN = 1;}
#define 	FAN_OFF()            			{P_FAN = 0;}


//s_u8OutBuff
#define    _BSR(RAMBIT)         ((_HI8BIT(RAMBIT)) |= (1<<(_LO8BIT(RAMBIT))))
#define    _BS(ADDRESS,BIT)     ((ADDRESS) |=  (1<<(BIT)))
#define    _BC(ADDRESS,BIT)     ((ADDRESS) &= ~(1<<(BIT)))
#define    _BTC(ADDRESS,BIT)    (((ADDRESS)&   (1<<(BIT)))==0)
#define    _BTS(ADDRESS,BIT)    ((ADDRESS) &   (1<<(BIT)))
#define    _BN(ADDRESS,BIT)     ((ADDRESS) ^=  (1<<(BIT)))



#define	    _SIGNAL_A_ON                 _BS(s_u8OutBuff,0)
#define	    _SIGNAL_A_OFF                _BC(s_u8OutBuff,0)
#define	    _TEST_SIGNAL_A               _BTS(s_u8OutBuff,0)

#define	    _SIGNAL_B_ON                 _BS(s_u8OutBuff,1)
#define	    _SIGNAL_B_OFF                _BC(s_u8OutBuff,1)
#define	    _TEST_SIGNAL_B               _BTS(s_u8OutBuff,1)




#define PIN_SW1								P_Door   //标SW1


#define SW_CHANGE_TIME						10//20	//*10MS	微动开关检测时间

#define SW_CHANGE_OPEN_TIME					50		//*10MS	微动开关检测时间

#define SW_STATUS_CLOSE_CHANGE_TIME			10//140	//*10MS	到装水位置微动开关检测时间

/*************************配置结束*************************/

#define		MAX_DISCONECT_TIME				500 //*10MS


//报错定义
#define     C_ERRO_NULL              0
#define     C_ERRO_F1                1
#define     C_ERRO_F2                2
#define     C_ERRO_F3                3
#define     C_ERRO_F4                4
#define     C_ERRO_F5                5
#define     C_ERRO_F6                6
#define     C_ERRO_F7                7
#define     C_ERRO_F8                8



extern xdata BatCharge_Cnt;


#define	STATUS_COUNT		20
#define	POW_COUNT			5



extern X_REG SW_Status_Enum mSwStatus;
extern X_REG Sys_TypeDef  Sys;






extern void F_FunCtrl(void);
extern void OutputDeal();
extern void F_ModeWork(void);

#endif



