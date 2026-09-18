

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
	DISP_TIME_OFF_SET,			//时间设置显示
	DISP_TEMP_SET,				//温度设置显示
	DISP_TIME,					//显示当前时间
    _DISP_TOTAL_,
};








#define	DISP_LENGTH 	3// //比COM（COM_MAX_CNT）多一组
#define	KEY_DISP		2	//按键组的显示






#ifdef _Native_Display_H
	static I_REG unsigned int DispLastTime = 0;
	static I_REG unsigned char DispIndex = 0;
	static I_REG unsigned char DispBlink = 0;
	//static I_REG unsigned char GobalBlink = 0;
	EXT_DISP u8 I_REG DispRunSec = 0;

	EXT_DISP I_REG  DispFlag DispFg;
	EXT_DISP I_REG u8 DispBuf[4];	
	EXT_DISP u8 g_DispSegBuf[DISP_LENGTH];
	EXT_DISP I_REG u8 g_DispSeg[DISP_LENGTH];


	//产测
	EXT_DISP X_REG u8 FactoryMode_Sec = 0;
	EXT_DISP X_REG u8 FactoryStep = 0;
	//EXT_DISP X_REG u8 FactoryRunT = 0;
	EXT_DISP X_REG u8 FactoryKeyNum = 0;
	//EXT_DISP X_REG u8 FactoryKeyPressFlag = 0;



#else
	EXT_DISP u8 I_REG DispRunSec;
	EXT_DISP  u8  g_DispSegBuf[DISP_LENGTH];
	EXT_DISP I_REG u8 g_DispSeg[DISP_LENGTH];

	EXT_DISP I_REG  DispFlag DispFg;
	//产测
	EXT_DISP X_REG u8 FactoryStep;
	EXT_DISP X_REG u8 FactoryKeyNum;
	EXT_DISP X_REG u8 FactoryMode_Sec;
	//EXT_DISP X_REG u8 FactoryKeyPressFlag;
	//EXT_DISP X_REG u8 UserRunSec;


#endif

#define	 DISP_FAC_T		3//*100MS


//#define	PSEG1	0x20  A
//#define	PSEG2	0x10  b
//#define	PSEG3	0x08  c
//#define	PSEG4	0x04  d
//#define	PSEG5	0x100 e
//#define	PSEG6	0x80  f
//#define	PSEG7	0x40  g
//#define	PSEG8	0x02

#define BSEG_0   	0
#define BSEG_A   		BIT5
#define BSEG_B   		BIT6
#define BSEG_C   		BIT9
#define BSEG_D   		BIT10
#define BSEG_E   		BIT8
#define BSEG_F   		BIT7
#define BSEG_G   		BIT13
#define BSEG_DP  	0

#define DISP_0  BSEG_A|BSEG_B|BSEG_C|BSEG_D|BSEG_E|BSEG_F|BSEG_0
#define DISP_1  BSEG_B|BSEG_C
#define DISP_2  BSEG_A|BSEG_B|BSEG_D|BSEG_E|BSEG_G   
#define DISP_3  BSEG_A|BSEG_B|BSEG_C|BSEG_D|BSEG_G
#define DISP_4  BSEG_B|BSEG_C|BSEG_F|BSEG_G
#define DISP_5  BSEG_A|BSEG_0|BSEG_C|BSEG_D|BSEG_0|BSEG_F|BSEG_G
#define DISP_6  BSEG_A|BSEG_0|BSEG_C|BSEG_D|BSEG_E|BSEG_F|BSEG_G
#define DISP_7  BSEG_A|BSEG_B|BSEG_C|BSEG_0|BSEG_0|BSEG_0|BSEG_0
#define DISP_8  BSEG_A|BSEG_B|BSEG_C|BSEG_D|BSEG_E|BSEG_F|BSEG_G
#define DISP_9  BSEG_A|BSEG_B|BSEG_C|BSEG_D|BSEG_0|BSEG_F|BSEG_G
#define DISP_A  BSEG_A|BSEG_B|BSEG_C|BSEG_0|BSEG_E|BSEG_F|BSEG_G
#define DISP_b  BSEG_0|BSEG_0|BSEG_C|BSEG_D|BSEG_E|BSEG_F|BSEG_G
#define DISP_C  BSEG_A|BSEG_0|BSEG_0|BSEG_D|BSEG_E|BSEG_F|BSEG_0
#define DISP_d  BSEG_0|BSEG_B|BSEG_C|BSEG_D|BSEG_E|BSEG_0|BSEG_G
#define DISP_E  BSEG_A|BSEG_0|BSEG_0|BSEG_D|BSEG_E|BSEG_F|BSEG_G
#define DISP_F 	BSEG_A|BSEG_0|BSEG_0|BSEG_0|BSEG_E|BSEG_F|BSEG_G

#define DISP_g  BSEG_A|BSEG_B|BSEG_C|BSEG_D|BSEG_F|BSEG_G
#define DISP_o  BSEG_0|BSEG_0|BSEG_C|BSEG_D|BSEG_E|BSEG_0|BSEG_G
#define DISP_P  BSEG_A|BSEG_B|BSEG_0|BSEG_0|BSEG_E|BSEG_F|BSEG_G
#define DISP_c  BSEG_0|BSEG_0|BSEG_0|BSEG_D|BSEG_E|BSEG_0|BSEG_G
#define DISP_L	BSEG_0|BSEG_0|BSEG_0|BSEG_D|BSEG_E|BSEG_F|BSEG_0
#define DISP_S	BSEG_A|BSEG_C|BSEG_D|BSEG_F|BSEG_G
#define DISP_C  BSEG_A|BSEG_0|BSEG_0|BSEG_D|BSEG_E|BSEG_F|BSEG_0


#define DISP_N  BSEG_A|BSEG_B|BSEG_C|BSEG_E|BSEG_F|BSEG_0
#define DISP_H  BSEG_B|BSEG_C|BSEG_E|BSEG_F|BSEG_G

#define DISP_r  BSEG_E|BSEG_G
#define DISP_t  BSEG_D|BSEG_E|BSEG_F|BSEG_G
#define DISP_n  BSEG_C|BSEG_E|BSEG_G
#define DISP_U  BSEG_B|BSEG_C|BSEG_D|BSEG_E|BSEG_F
#define DISP_u  BSEG_C|BSEG_D|BSEG_E


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
