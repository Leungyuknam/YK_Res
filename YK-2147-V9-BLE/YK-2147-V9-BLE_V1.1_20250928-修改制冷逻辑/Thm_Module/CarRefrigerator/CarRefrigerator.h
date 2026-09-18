/*********************************************************
文 件 名 : CarRefrigerator.H
文件说明 : 
编写作者 : thm
修订作者 : 
编写时间 : 2022-3-18
版 本 号 : V1.0
==========================================================*/
#ifndef _CAR_REFRIGERATOR1_H_
#define _CAR_REFRIGERATOR1_H_ 

/*
Def_Machine_Type = 0  :   单温区
Def_Machine_Type = 1  :   并联双温区，左右各工作10分钟
Def_Machine_Type = 2  :   串联双温区，左箱控温优先
Def_Machine_Type = 3  :   串联双温区，右箱控温优先
*/

#define	    Def_Machine_Type            0	 //机型选择(当前支持0-3机型)
#define	    Def_DevideBoardEn           0	 //1:含间隔板       0：不含间隔板
#define	    Def_BlueToothEn             0	 //1:蓝牙使能       0：蓝牙除能
#define	    Def_BatCommunicateEn        1	 //1:锂电通讯使能   0：锂电通讯除能
#define	    Def_NtcNumbers              0	 //1:双NTC          0：单NTC
#define		Def_AdvancedNum	            0	 //高级设置菜单范围  1:E1-EE   0:E1-E9
#define		Def_AdvancedSet	            0	 //高级设置菜单在设置参数过程中按SET键，  0：退出当前界面并返回上一菜单   1：退出当前界面并直接进入下一菜单
#define		Def_SetKeyToggleTempSetEn	0	 //0：SET键切换温度设置不使能   1：SET键切换温度设置使能
#define		Def_AdvancedSetDisp     	0	 //0：高级设置菜单与参数共屏 1：高级设置菜单与参数分屏

#define		Def_TestMode	            0	 //0：缩时模式除能   1：缩时模式使能

/*
1、D010044 4.02.T36-44，不能关闭左箱，不带蓝牙，Def_CloseLBox_En需配置为0
2、D010042 4.02.T36-23，能关闭左箱，带蓝牙，Def_CloseLBox_En需配置为1
3、D010043 4.02.T36-40，能关闭左箱，不带蓝牙，Def_CloseLBox_En需配置为1
*/
#define		Def_CloseLBox_En	        0	 //0：不允许关闭左箱   1：允许关闭左箱
#define		Def_TempUnit      	        0	 //0：含电量显示，摄氏度出厂   1：无电量显示，华氏度出厂

#if Def_TestMode
#define	    C_Interval_Time            60	 //Def_Machine_Type=1时，左、右箱制冷时间1分钟
#define	    C_FIXED_2000RPM            30    //压缩机每次启动，前30秒的转速固定为2000转
#else
#define	    C_Interval_Time            600	 //Def_Machine_Type=1时，左、右箱制冷时间10分钟
#define	    C_FIXED_2000RPM            120   //压缩机每次启动，前6分钟的转速固定为2000转
#endif


#ifndef     RamDataType
#define     RamDataType     xdata   
#endif


typedef struct
{  
    signed   char   s8RoomTemp_L;        //左箱室温
    signed   char   s8SetTemp_L;         //左箱设定温度
    signed   char   s8SetTemp_Top;       //左箱设定温度上限值
    signed   char   s8SetTemp_Bot;       //左箱设定温度下限值     
    signed   char   s8TempComp_N6_L;     //左箱温度>=-6度补偿值
    signed   char   s8TempComp_N12_L;    //左箱温度>=-12度补偿值
    signed   char   s8TempComp_N13_L;    //左箱温度<=-13度补偿值   
    signed   char   s8StartBias_L;       //左箱启动偏差值
    signed   char   s8StopBias_L;        //左箱停机偏差值
    
    signed   char   s8RoomTemp_R;        //右箱室温
    signed   char   s8SetTemp_R;         //右箱设定温度  
    signed   char   s8TempComp_N6_R;     //右箱温度>=-6度补偿值
    signed   char   s8TempComp_N12_R;    //右箱温度>=-12度补偿值
    signed   char   s8TempComp_N13_R;    //右箱温度<=-13度补偿值
    signed   char   s8StartBias_R;       //左箱启动偏差值
    signed   char   s8StopBias_R;        //左箱停机偏差值
    
    unsigned char   u8OnOff;              //开关机：0-1
    unsigned char   u8TempUnit;           //温度单位：华氏，摄氏
    unsigned char   u8ComProtectTime;     //压缩机停机保护时间：单位分钟
	unsigned char   u8LastComProtectTime; //压缩机停机保护时间：单位分钟
    unsigned char   u8BatProtectLv;       //电池保护等级：High,Mid,Low
    unsigned char   u8SpeedLv;            //转速等级：MAX,ECO
	unsigned char   u8SpeedDrv;           //转速驱动：MAX,ECO
    unsigned char   u8ValveStatus;        //电磁阀状态
    unsigned char   u8BoxEn;              //箱体使能：0：双箱全开，1：开左箱，2：开右箱

    unsigned char   u8NtcToward;          //NTC温度传感器方向：0-1
    unsigned char   u8ValveToward;        //电磁阀方向：0-1
    
	unsigned char   u8PowerSource;		  //电源来源：0：适配器，1：锂电池
	unsigned char   u8Charge;		      //充电状态：0：无充电，1：充电中
	unsigned char   u8CompError;		  //压缩机报错信号：F1-F6
	unsigned char   u8AppLock;		      //APP童锁：0关，1开
	unsigned char   u8BleStatus;		  //蓝牙状态
	unsigned char   u8BleBindTime;		  //蓝牙绑定时间

	unsigned char   u8CloseFlag_L;		  //左箱关闭标志
	unsigned int    u16FullDisp;		  //全亮时长
	
	signed   char   s8DispTemp_L;        
	signed   char   s8DispTemp_R;        
	unsigned char   u8DefrostOnTime;
	unsigned char   u8DefrostOffTime;
}CarRfgrt_TypeDef;

//Def_Machine_Type
#define	    C_Machine_Type_SingleBox                    0
#define	    C_Machine_Type_DoubleBox_DoubleMaster       1
#define	    C_Machine_Type_DoubleBox_LeftMaster         2
#define	    C_Machine_Type_DoubleBox_RightMaster        3

//g_CarRfgrt.u8OnOff
#define	    C_Power_OFF                  0
#define	    C_Power_ON                   1

//g_CarRfgrt.u8TempUnit
#define	    C_Unit_Celsius               0
#define	    C_Unit_Fahrenheit            1

//g_CarRfgrt.u8BatProtectLv
#define	    C_Protect_High               2
#define	    C_Protect_Mid                1
#define	    C_Protect_Low                0

//g_CarRfgrt.u8SpeedLv、g_CarRfgrt.u8SpeedDrv
#define	    C_Speed_MAX                  0
#define	    C_Speed_ECO                  1
#define	    C_Speed_STOP                 2
#define	    C_Speed_MAX4                 3



//g_CarRfgrt.u8BoxEn
#define	    C_BOX_ALL_EN                 0
#define	    C_BOX_ONLY_LEFT_EN           2
#define	    C_BOX_ONLY_RIGHT_EN          1

//g_CarRfgrt.u8NtcToward,  g_CarRfgrt.u8ValveToward
#define	    C_Toward_Forward             0
#define	    C_Toward_Backward            1

//g_CarRfgrt.u8PowerSource
#define	    C_Power_Adaptor              0
#define	    C_Power_Battery              1

//g_CarRfgrt.u8AppLock
#define	    C_APP_LOCK_DISABLE           0
#define	    C_APP_LOCK_ENABLE            1

//g_CarRfgrt.u8BleStatus
#define	    C_BLE_MISS                   0
#define	    C_BLE_IDLE                   1
#define	    C_BLE_ASK                    2
#define	    C_BLE_BIND                   3

#define	    C_SOFT_VER                   10

typedef struct
{
	unsigned int   u16DelayOnTime;	   //压缩机延时启动时间计时
	unsigned char  u8CompRunEn;		   //压缩机运行使能
	unsigned char  u8ValveTurn;		   //电磁阀转向
	unsigned char  u8Speed;		       //压缩机转速
	signed char    s8SetTemp_L;		   //左箱设置温度
	signed char    s8SetTemp_R;		   //右箱设置温度
}Compressor_TypeDef;

typedef struct
{
	u8 u8Status;
	u8 u8Object;
	u8 u8Time;
}Modeset_Typedef;


typedef struct
{
	u8 u8Object;	  //设置对象
	s8 *s8Variate;	  //设置变量
	s8 *s8TopC;		  //摄氏上限值
	s8 *s8BottomC;	  //摄氏下限值
	s8 *s8TopF;		  //华氏上限值
	s8 *s8BottomF;	  //华氏下限值
}TemperSet_Typedef;


typedef struct
{
	u8 u8Object;	   //设置对象
	u8 u8CycOrTb;	   //1：循环加减    0：上下限加减
	u8 u8Top;		   //上限值
	u8 u8Bottom;	   //下限值
	u8 *u8Variate;	   //设置变量
}VariateSet_8Bit_Typedef;


//s_sCompressor.u8ValveTurn、g_CarRfgrt.u8ValveStatus
#define	    C_VALVE_TURN_NONE            0
#define	    C_VALVE_TURN_LEFT            1
#define	    C_VALVE_TURN_RIGHT           2

//s_u8OutBuff
#define	    _SIGNAL_A_ON                 _BS(s_u8OutBuff,0)
#define	    _SIGNAL_A_OFF                _BC(s_u8OutBuff,0)
#define	    _TEST_SIGNAL_A               _BTS(s_u8OutBuff,0)

#define	    _SIGNAL_B_ON                 _BS(s_u8OutBuff,1)
#define	    _SIGNAL_B_OFF                _BC(s_u8OutBuff,1)
#define	    _TEST_SIGNAL_B               _BTS(s_u8OutBuff,1)

//蜂鸣器输出
#define     _BUZZER_SHORT_BEEP				F_BuzzerInput(1,1,5)
#define     _BUZZER_LONG_BEEP				F_BuzzerInput(1,10,5)

//g_sModeSet.u8Status
#define     C_MS_STATUS_IDLE   		   		0

#define     C_MS_STATUS_CTRL_LBOX   		1
#define     C_MS_STATUS_CTRL_RBOX   		2
#define     C_MS_STATUS_SPEED   		   	3
#define     C_MS_STATUS_PROTECT   		  4

#define     C_MS_STATUS_E1   		   	    10
#define     C_MS_STATUS_E2   		   	    11
#define     C_MS_STATUS_E3   		   	    12
#define     C_MS_STATUS_E4   		   	    13
#define     C_MS_STATUS_E5   		   	    14
#define     C_MS_STATUS_E6   		   	    15
#define     C_MS_STATUS_E7   		   	    16
#define     C_MS_STATUS_E8   		   	    17
#define     C_MS_STATUS_E9   		   	    18
#define     C_MS_STATUS_EA   		   	    19
#define     C_MS_STATUS_Eb   		   	    20
#define     C_MS_STATUS_EC   		   	    21
#define     C_MS_STATUS_Ed   		   	    22
#define     C_MS_STATUS_EE   		   	    23

#define     C_MS_STATUS_TpCh     		   	30
#define     C_MS_STATUS_ValveCh     		31
#define     C_MS_SYS_RESET          		32

#if  Def_AdvancedNum
#define     C_MS_STATUS_END   	C_MS_STATUS_EE
#define     C_MS_OBJECT_END   	C_MS_OBJECT_EE
#else
#define     C_MS_STATUS_END   	C_MS_STATUS_Eb
#define     C_MS_OBJECT_END   	C_MS_OBJECT_Eb
#endif


//g_sModeSet.u8Object
#define     C_MS_OBJECT_BIAS   		   		128	   //偏移量

#define     C_MS_OBJECT_NONE    		   	0
#define     C_MS_OBJECT_LBOX   		   		1
#define     C_MS_OBJECT_RBOX   		   		2


#define     C_MS_OBJECT_E1   		   		(C_MS_STATUS_E1 + C_MS_OBJECT_BIAS)
#define     C_MS_OBJECT_E2   		   		(C_MS_STATUS_E2 + C_MS_OBJECT_BIAS)
#define     C_MS_OBJECT_E3   		   		(C_MS_STATUS_E3 + C_MS_OBJECT_BIAS)
#define     C_MS_OBJECT_E4   		   		(C_MS_STATUS_E4 + C_MS_OBJECT_BIAS)
#define     C_MS_OBJECT_E5   		   		(C_MS_STATUS_E5 + C_MS_OBJECT_BIAS)
#define     C_MS_OBJECT_E6   		   		(C_MS_STATUS_E6 + C_MS_OBJECT_BIAS)
#define     C_MS_OBJECT_E7   		   		(C_MS_STATUS_E7 + C_MS_OBJECT_BIAS)
#define     C_MS_OBJECT_E8   		   		(C_MS_STATUS_E8 + C_MS_OBJECT_BIAS)
#define     C_MS_OBJECT_E9   		   		(C_MS_STATUS_E9 + C_MS_OBJECT_BIAS)
#define     C_MS_OBJECT_EA   		   		(C_MS_STATUS_EA + C_MS_OBJECT_BIAS)
#define     C_MS_OBJECT_Eb   		   		(C_MS_STATUS_Eb + C_MS_OBJECT_BIAS)
#define     C_MS_OBJECT_EC   		   		(C_MS_STATUS_EC + C_MS_OBJECT_BIAS)
#define     C_MS_OBJECT_Ed   		   		(C_MS_STATUS_Ed + C_MS_OBJECT_BIAS)
#define     C_MS_OBJECT_EE   		   		(C_MS_STATUS_EE + C_MS_OBJECT_BIAS)

//g_sModeSet.u8Time
#define     C_MS_TIME_0S     		   		0
#define     C_MS_TIME_1S     		   		10
#define     C_MS_TIME_2S     		   		20
#define     C_MS_TIME_3S     		   		30
#define     C_MS_TIME_4S     		   		40
#define     C_MS_TIME_5S     		   		50

//l_u8SetType
#define     C_SET_DECREASE	     	0
#define     C_SET_INCREASE	     	1

//HandleType
#define     C_ADVSET_UP    		   			0
#define     C_ADVSET_DOWN   		   		1
#define     C_ADVSET_SET   		   			2
#define     C_ADVSET_RELOAD   		   	    3



//外部变量声明
extern CarRfgrt_TypeDef RamDataType g_CarRfgrt;
extern Modeset_Typedef RamDataType g_sModeSet;
extern u16 g_u16MemoryTime;
extern ANALOG_PWM_Typedef xdata g_sDefrost;

//函数声明
extern void F_DivideBoard_Test(void);
extern void F_CarRefrigerator_Handle(void);
extern void F_DriveOutput_Handle(void);
extern void F_ParameterProtect(void);
extern void F_MCU_Init(void);
extern void F_Defrost(void);
#endif
