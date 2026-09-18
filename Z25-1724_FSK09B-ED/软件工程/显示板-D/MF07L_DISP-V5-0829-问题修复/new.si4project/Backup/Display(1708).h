

#ifndef	_Display_H
#define	_Display_H

#include "Include\Typedef.h"




#ifdef _Native_Display_H
   #define	EXT_DISP 
#else
   #define	EXT_DISP extern	
#endif



typedef enum				
{
	SHOW_NONE,
	SHOW_TIME,
}ForceShow_Enum;
	

typedef enum				
{
	SHOW_CH2O,
	SHOW_PM25,
}SenseShow_Enum;


typedef struct
{
	ForceShow_Enum Mode;
	u8 TCnt;
	u8 KeepTCnt;
}ForceDisp_t;


typedef struct
{
	SenseShow_Enum Mode;
	u8 Sec;
}SensorDisp_t;


typedef struct
{
	unsigned char 		Index;
	unsigned char		Com;
	u16					Val;
} LedTask_t;


typedef enum				
{
	SHOW_BRI_EN,
	SHOW_ALL_ON,
//	SHOW_T_RUN
}ShowM_Enum;


#define	PWM_LED_ON					30    //*125us 亮起来的时间，限制最大值为80

typedef struct
{
	unsigned char       Index;
	unsigned int        LastTime;
	unsigned char       Blink;
	void (*DispFunc)(void);	
} DispTask_t;

typedef struct
{
    u8 Bri               			: 1;        //闪缩显示标志
    u8 BreathSet               		: 1; 
    u8 DNone						: 6;		//None
} DispFlag;




enum DISP_TASK_NUM
{
    DISP_NORMAL         = 0,
	  DISP_POW_LOW,			//显示电压低
	//DISP_TIME_OFF_SET,			//定时关机设置显示
	//DISP_ANGLE_SET,				//角度
    _DISP_TOTAL_,
};








#define	DISP_LENGTH 	4// //比COM（COM_MAX_CNT）多一组
#define	KEY_DISP		2	//按键组的显示





	static I_REG unsigned int DispLastTime = 0;
	static I_REG unsigned char DispIndex = 0;
	static I_REG unsigned char DispBlink = 0;
	//static I_REG unsigned char GobalBlink = 0;
	EXT_DISP u8 I_REG DispRunSec = 0;

	EXT_DISP I_REG  DispFlag DispFg;
	EXT_DISP I_REG u8 DispBuf[4];	
	EXT_DISP I_REG u8 g_DispSegBuf[DISP_LENGTH];
	EXT_DISP I_REG u8 g_DispSeg[DISP_LENGTH];
	
	EXT_DISP I_REG u8 g_DispSegHalfBuf[DISP_LENGTH];
	EXT_DISP I_REG u8 g_DispSegHalf[DISP_LENGTH];

	//产测
	EXT_DISP X_REG u8 FactoryMode_Sec = 0;
	EXT_DISP X_REG u8 FactoryStep = 0;
	//EXT_DISP X_REG u8 FactoryRunT = 0;
	EXT_DISP X_REG u8 FactoryKeyNum = 0;
	//EXT_DISP X_REG u8 FactoryKeyPressFlag = 0;



#define	 DISP_FAC_T		3//*100MS


#define SHOW_KEEP_TIME			5   //*1S 维持一段显示的时间

#define	PRODUCT_VERSION					1



//七段数码的序号
#define DIG1			0
#define DIG2			1 

#define DIG_MAX			4
#define PWM_CON			7


#define	PWM_MAX		1020 




#define	LED_ON()			{P00 = 0;}
#define	LED_OFF()			{P00 = 1;}


EXT_DISP void F_DispTaskChange(unsigned char temp);
EXT_DISP void FactoryDisplay_Value_Set(void);
EXT_DISP void Display_Value_Set(void);
EXT_DISP unsigned char GetDispType(void);
EXT_DISP void FactoryKeyNumFuc(u8 Dat);
EXT_DISP void SetFactoryMode();




#endif
