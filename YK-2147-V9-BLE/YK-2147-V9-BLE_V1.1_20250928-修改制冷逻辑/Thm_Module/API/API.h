///*********************************************************
//文 件 名 : API.H
//文件说明 :
//编写作者 : thm
//修订作者 : 
//编写时间 : 2020-12-6
//版 本 号 : V1.0
//==========================================================*/
//#ifndef _API_H_
//#define _API_H_ 
//
////带蓝牙1，无蓝牙0
//#define		Def_BLUETOOTH	            0 
//
////存储数据个数
//#define     SYSTEM_DATA_NUMBERS			20
//
//
////g_u8ModeSetTime
//#define     C_MODESET_TIME_3S		   30
//#define     C_MODESET_TIME_5S		   50
//
//

//
////g_u8SystemStatus
//#define     C_SYSTEM_TURNOFF		   0		//关机
//#define     C_SYSTEM_TURNON  		   1		//开机
//
////g_u8SetBox
//#define     C_NONE          		   0
//#define     C_LBOX          		   1
//#define     C_RBOX          		   2
//
////g_u8BingDingFlag
//#define     C_BINGDING_IDLE            0
//#define     C_BINGDING_ASK             1
//#define     C_BINGDING_DONE            2
//
////g_u8SysLock
//#define     C_SYSYLOCK_UNLOCK          0
//#define     C_SYSYLOCK_LOCK            1
//
////g_u8SpeedSetValue
//#define     C_SPEED_ECO		           1
//#define     C_SPEED_MAX		           0
//
////g_u8BatteryProSetValue
//#define     C_CELL_LOW		           0
//#define     C_CELL_MID		           1
//#define     C_CELL_HIGH		           2
//
////g_u8UnitConversion
//#define     C_FAHRENHEIT_F			   1     //华氏
//#define     C_CENTIGRADE_C			   0     //摄氏 
//
////g_u8BoxRunStatus
//#define     C_BOX_ALLOFF			   0
//#define     C_BOX_LEFT_ON    		   1
//#define     C_BOX_RIGHT_ON			   2
//
////g_u8BoxLockSet
//#define     C_BOX_ALL_UNLOCK		   0
//#define     C_BOX_LEFT_LOCK    		   1
//#define     C_BOX_RIGHT_LOCK		   2
//
////g_bBatteryType
//#define     C_BATTERY_LEAD   		   0	//铅酸电池
//#define     C_BATTERY_LITHIUM    	   1	//锂电池
//
////g_bChargeStatus
//#define     C_IN_CHARGE   	   		   1	 	
//#define     C_NO_CHARGE    	           0
//
////g_u8BoxEnable
//#define     C_ONLY_RBOX_ENABLE   	   2	 	
//#define     C_ONLY_LBOX_ENABLE    	   1
//#define     C_ALL_BOX_ENABLE    	   0
//
////g_u8RunningBox
//#define     C_RBOX_RUNNING       	   2
//#define     C_LBOX_RUNNING       	   1
//#define     C_NOBOX_RUNNING       	   0
//
////g_u8MakeColdStatus
//#define     C_COLD_LBOX_DONE       	   0x01
//#define     C_COLD_RBOX_DONE       	   0x02
//#define     C_COLD_ALLBOX_DONE         0x03
//#define     C_COLD_NOBOX_DONE          0x00
//
//
//
//
//
//
//
//
////g_u8OutPutBuff位定义
//#define     C_RMP3000_	     		   1
//#define     C_RMP2000_	     		   2
//#define     C_PROTECT_MED_	     	   3
//#define     C_PROTECT_LOW_	     	   4
//#define     C_BACKLIGHT_	     	   5
//#define     C_DCF_A_	     	       6
//#define     C_DCF_B_	     	   	   7
//
////驱动位操作
//#define     _RMP3000_ON		           _BS(g_u8OutPutBuff,C_RMP3000_)
//#define     _RMP3000_OFF		       _BC(g_u8OutPutBuff,C_RMP3000_)
//#define     _RMP2000_ON		           _BS(g_u8OutPutBuff,C_RMP2000_)
//#define     _RMP2000_OFF		       _BC(g_u8OutPutBuff,C_RMP2000_)
//#define     _LCD_BACKLIGHT_ON		   _BS(g_u8OutPutBuff,C_BACKLIGHT_)
//#define     _LCD_BACKLIGHT_OFF		   _BC(g_u8OutPutBuff,C_BACKLIGHT_)
//
//
//#define     _DCF_A_ON		           _BS(g_u8OutPutBuff,C_DCF_A_)
//#define     _DCF_A_OFF		           _BC(g_u8OutPutBuff,C_DCF_A_)
//#define     _DCF_B_ON		       	   _BS(g_u8OutPutBuff,C_DCF_B_)
//#define     _DCF_B_OFF		           _BC(g_u8OutPutBuff,C_DCF_B_)
//
//
//
////外部变量声明
//
//
//
////外部函数声明
//
//
////extern	void F_TIME_Compressor(void);
////extern	void F_COMPRESSOR_Handle(void);
////extern	void F_BATTERY_PRT_Handle(void);
////extern	void F_DRIVE_Output(void);
////
////extern	void F_TIME_Ask(void);
////extern	void F_LOCK_Handle(void);
////extern	void F_ValveDrive(void);
//        
//#endif