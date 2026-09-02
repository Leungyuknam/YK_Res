#ifndef _DISPLAY_H_
#define _DISPLAY_H_

extern bit g_bRgbDispEn;          //外接RGB显示使能

extern U8 xdata g_u8LockBlinkTime;//童锁闪烁时间
#define C_LOCK_BLINK_TIME       20//单位100ms

extern U8 xdata g_u8TimeHBlinkTime;//定时闪烁时间
#define C_TIMEH_BLINK_TIME       50//单位100ms

extern U8 xdata g_u8PowerOnDispTime;  //开机动画的显示时间
#define C_POWERON_DISP_TIME            89//单位100ms

extern U8 xdata g_u8KeyDispTime;  //按键显示时间
extern U8 xdata g_u8SleepDispTime;//睡眠显示时间
#define C_KEY_DISP_TIME         50//单位100ms

extern U8 xdata g_u8RGBDispMode;

extern U8 xdata g_u8RGBManualDispTime;//手动切换RGB类型时的显示时间
#define C_RGB_MANUAL_DISP_TIME         100//单位100ms

extern U8 xdata g_u8RGBAutoDispTime;  //自动切换RGB类型时的显示时间
#define C_RGB_AUTO_DISP_TIME            50//单位100ms


extern bit g_bHalfDispEn;         //半亮使能标记

extern U8 xdata g_u8DisBright;    //LED灯显示亮度

extern U8 xdata g_u8VocDispState;
extern U8 xdata g_u8PM25DispState;
extern U8 xdata g_u8CH2ODispState;
extern U8 xdata g_u8FilterDispState;

extern bit g_bVoiceFilterEn;

extern U8 xdata g_u8RgbColorIndex;
extern U8 xdata g_u8RGBDispType;
#define C_DISPTYPE_NONE       0
#define C_DISPTYPE_CH2O       1
#define C_DISPTYPE_PM25       2
#define C_DISPTYPE_VOC        3
	


#define C_DISP_OFF        0
#define C_DISP_BLUE       1
#define C_DISP_GREEN      2
#define C_DISP_YELLOW     3
#define C_DISP_RED        4
#define C_DISP_ALL        5

#define C_RGB_R           0
#define C_RGB_G           1
#define C_RGB_B           2

#define C_RGB_DISMODE_AUTO    0
#define C_RGB_DISMODE_MANUAL  1

//extern U16 xdata g_sPm25.u16DispData;            //PM25显示数值
extern U16 xdata g_u16DispPm10Data;            //PM10显示数值
extern U16 xdata g_u16DispPm1Data;             //PM1显示数值

extern U8 code NumTable[];
extern U8 xdata DispBuf[];
extern U8 code NumTableTimeOnes[];
extern U8 code NumTableTimeTens[];
extern U8 code NumTableTempHumi[];

#define TOTAL_COM		12	//显示数  
#define COM_1				0
#define COM_2				6
#define COM_3				8
#define COM_4				7
#define COM_5				5
#define COM_6				10
#define COM_7				2
#define COM_8				3
#define COM_9				4
#define COM_10			9
#define COM_11			11
#define COM_12			1

#define F_SpecialDis(x,y,z) 	 	 {  DispBuf[COM_7] |= 0;\
																		DispBuf[COM_8] |= x;\
																		DispBuf[COM_9] |= y;\
																		DispBuf[COM_10] |= z;}


																		
#define F_SpecialDisFour(x,y,z,j) 	 {  DispBuf[COM_7] |= x;\
																				DispBuf[COM_8] |= y;\
																				DispBuf[COM_9] |= z;\
																				DispBuf[COM_10] |= j;}

#define F_SpecialDisThree(x,y,z) 	 {  DispBuf[COM_1] |= x;\
																			DispBuf[COM_2] |= y;\
																			DispBuf[COM_3] |= z;}																			
																		

#define F_DispPlayHex(x)    {DispBuf[COM_1] |= NumTable[x >> 8];\
                             DispBuf[COM_2] |= NumTable[(x >> 4) & 0x0f];\
                             DispBuf[COM_3] |= NumTable[x & 0x0f];}				

#define F_SpecialDisTime(x,y) 	 	 {  DispBuf[COM_3] |= x;\
																			DispBuf[COM_4] |= y;}		

#define F_SpecialDisTempHumi(x,y) 	 	 {  DispBuf[COM_1] |= x;\
																					DispBuf[COM_2] |= y;}	

														 
enum DISP_TASK_NUM
{
	DIPS_NORMAL = 0,		
  DIPS_FAN,
	DIPS_TIME,
	DIPS_POWERUP,
	DISP_TOTAL_TASK,
};

#define DIS_PM25()         DispBuf[COM_4] |= BIT0



#define DIS_FAN()          DispBuf[COM_4] |= BIT7
#define DIS_SLEEP()        DispBuf[COM_4] |= BIT6
#define DIS_PET()          DispBuf[COM_4] |= BIT5
#define DIS_AI()           DispBuf[COM_4] |= BIT2

#define DIS_LOCK_KEY()			DispBuf[COM_5] 	|= BIT7
#define DIS_MODE_KEY()			DispBuf[COM_12] |= BIT0
#define DIS_ION_KEY()				DispBuf[COM_12] |= BIT1
#define DIS_WIFI_KEY()			DispBuf[COM_11] |= BIT0
#define DIS_POWER_KEY()			DispBuf[COM_11] |= BIT2
#define DIS_TIME_KEY()     	DispBuf[COM_11] |= BIT1

/****甲醛*****/
#define DIS_CH2O_LOGO()			DispBuf[COM_5] |= (BIT0|BIT1)

/****VOC*****/
#define DIS_VOC_LOGO()		  DispBuf[COM_5] |= (BIT5|BIT6)

/****PM25*****/
#define DIS_PM25_LOGO()			DispBuf[COM_5] |= (BIT2|BIT3|BIT4)

/****单位*****/
#define DIS_UG()						DispBuf[COM_11] |= (BIT5|BIT6)
#define DIS_MG()						DispBuf[COM_11] |= (BIT3|BIT4)

/****定时指示灯 H+时钟*****/
#define DIS_TIME_LOGO()			DispBuf[COM_3] |= BIT7
#define DIS_TIME_H()				DispBuf[COM_4] |= BIT0

/****温度单位  温度计+C*****/
#define DIS_TEMP_C() 				do { \
														DispBuf[COM_1] |= (BIT0); \
														DispBuf[COM_6] |= (BIT0); \
														} while (0)


/****湿度单位	 水滴+%*****/
#define DIS_HUMI_BFB()			do { \
														DispBuf[COM_2] |= (BIT0); \
														DispBuf[COM_6] |= (BIT1); \
														} while (0)

														
#define DIS_ION()			DispBuf[COM_6] |= BIT4
//#define DIS_WATER()			DispBuf[COM_6] |= BIT5
#define DIS_LOCK()          DispBuf[COM_6] |= BIT3
#define DIS_WIFI()          DispBuf[COM_6] |= BIT2
#define DIS_WATERBOX()		DispBuf[COM_12] |= BIT5
#define DIS_WATER()			DispBuf[COM_12] |= BIT6

#define DIS_FILTER()        DispBuf[COM_6] |= (BIT6|BIT7)								
														
#define DIS_MODE_HI()		DispBuf[COM_10] |= BIT7
#define DIS_MODE_MID()		DispBuf[COM_9]  |= BIT7
#define DIS_MODE_LOW()		DispBuf[COM_8]  |= BIT7
#define DIS_MODE_AI()		DispBuf[COM_12] |= BIT2
#define DIS_MODE_SLEEP()	DispBuf[COM_11] |= BIT7
														
														
#define DIS_DP()			DispBuf[COM_7] |= BIT7


//-------------------------------------------------------------------
typedef struct _TYPE_DISP_TASK
{
	unsigned char		Index;
	unsigned int		LastTime;
	unsigned char		Blink;
	void (*DispFunc)(void);	
}TYPE_DISP_TASK;

//中间四位数码管
#define c_SEGA		BIT0
#define c_SEGB		BIT5
#define c_SEGC		BIT4
#define c_SEGD		BIT3
#define c_SEGE		BIT2
#define c_SEGF		BIT1
#define c_SEGG		BIT6
#define c_SEGDP		BIT7

#define N_0		(c_SEGA|c_SEGB|c_SEGC|c_SEGD|c_SEGE|c_SEGF)
#define N_1		(c_SEGB|c_SEGC)
#define N_2		(c_SEGA|c_SEGB|c_SEGD|c_SEGE|c_SEGG)
#define N_3		(c_SEGA|c_SEGB|c_SEGC|c_SEGD|c_SEGG)
#define N_4		(c_SEGB|c_SEGC|c_SEGF|c_SEGG)
#define N_5		(c_SEGA|c_SEGC|c_SEGD|c_SEGF|c_SEGG)
#define N_6		(c_SEGA|c_SEGC|c_SEGD|c_SEGE|c_SEGF|c_SEGG)
#define N_7		(c_SEGA|c_SEGB|c_SEGC)
#define N_8		(c_SEGA|c_SEGB|c_SEGC|c_SEGD|c_SEGE|c_SEGF|c_SEGG)
#define N_9		(c_SEGA|c_SEGB|c_SEGC|c_SEGD|c_SEGF|c_SEGG)
#define N__		(c_SEGG)
#define N_A		(c_SEGA|c_SEGB|c_SEGC|c_SEGE|c_SEGF|c_SEGG)
#define N_B		(c_SEGC|c_SEGD|c_SEGE|c_SEGF|c_SEGG)
#define N_C		(c_SEGA|c_SEGD|c_SEGE|c_SEGF)
#define N_D		(c_SEGB|c_SEGC|c_SEGD|c_SEGE|c_SEGG)
#define N_E		(c_SEGA|c_SEGD|c_SEGE|c_SEGF|c_SEGG)
#define N_F		(c_SEGA|c_SEGE|c_SEGF|c_SEGG)
#define N_H		(c_SEGB|c_SEGC|c_SEGE|c_SEGF|c_SEGG)
#define N_h		(c_SEGC|c_SEGE|c_SEGF|c_SEGG)
#define N_I		(c_SEGE|c_SEGF)
#define N_L		(c_SEGD|c_SEGE|c_SEGF)
#define N_n		(c_SEGC|c_SEGE|c_SEGG)
#define N_o		(c_SEGC|c_SEGD|c_SEGE|c_SEGG)
#define N_P		(c_SEGA|c_SEGB|c_SEGE|c_SEGF|c_SEGG)
#define N_U		(c_SEGB|c_SEGC|c_SEGD|c_SEGE|c_SEGF)
#define N_t		(c_SEGD|c_SEGE|c_SEGF|c_SEGG)



//左上角温湿度数码管
#define c_TEMP_SEGA		BIT1
#define c_TEMP_SEGB		BIT6
#define c_TEMP_SEGC		BIT5
#define c_TEMP_SEGD		BIT4
#define c_TEMP_SEGE		BIT3
#define c_TEMP_SEGF		BIT2
#define c_TEMP_SEGG		BIT7

#define N_TEMP_0		(c_TEMP_SEGA|c_TEMP_SEGB|c_TEMP_SEGC|c_TEMP_SEGD|c_TEMP_SEGE|c_TEMP_SEGF)
#define N_TEMP_1		(c_TEMP_SEGB|c_TEMP_SEGC)
#define N_TEMP_2		(c_TEMP_SEGA|c_TEMP_SEGB|c_TEMP_SEGD|c_TEMP_SEGE|c_TEMP_SEGG)
#define N_TEMP_3		(c_TEMP_SEGA|c_TEMP_SEGB|c_TEMP_SEGC|c_TEMP_SEGD|c_TEMP_SEGG)
#define N_TEMP_4		(c_TEMP_SEGB|c_TEMP_SEGC|c_TEMP_SEGF|c_TEMP_SEGG)
#define N_TEMP_5		(c_TEMP_SEGA|c_TEMP_SEGC|c_TEMP_SEGD|c_TEMP_SEGF|c_TEMP_SEGG)
#define N_TEMP_6		(c_TEMP_SEGA|c_TEMP_SEGC|c_TEMP_SEGD|c_TEMP_SEGE|c_TEMP_SEGF|c_TEMP_SEGG)
#define N_TEMP_7		(c_TEMP_SEGA|c_TEMP_SEGB|c_TEMP_SEGC)
#define N_TEMP_8		(c_TEMP_SEGA|c_TEMP_SEGB|c_TEMP_SEGC|c_TEMP_SEGD|c_TEMP_SEGE|c_TEMP_SEGF|c_TEMP_SEGG)
#define N_TEMP_9		(c_TEMP_SEGA|c_TEMP_SEGB|c_TEMP_SEGC|c_TEMP_SEGD|c_TEMP_SEGF|c_TEMP_SEGG)
#define N_TEMP__		(c_TEMP_SEGG)
#define N_TEMP_A		(c_TEMP_SEGA|c_TEMP_SEGB|c_TEMP_SEGC|c_TEMP_SEGE|c_TEMP_SEGF|c_TEMP_SEGG)
#define N_TEMP_B		(c_TEMP_SEGC|c_TEMP_SEGD|c_TEMP_SEGE|c_TEMP_SEGF|c_TEMP_SEGG)
#define N_TEMP_C		(c_TEMP_SEGA|c_TEMP_SEGD|c_TEMP_SEGE|c_TEMP_SEGF)
#define N_TEMP_D		(c_TEMP_SEGB|c_TEMP_SEGC|c_TEMP_SEGD|c_TEMP_SEGE|c_TEMP_SEGG)
#define N_TEMP_E		(c_TEMP_SEGA|c_TEMP_SEGD|c_TEMP_SEGE|c_TEMP_SEGF|c_TEMP_SEGG)
#define N_TEMP_F		(c_TEMP_SEGA|c_TEMP_SEGE|c_TEMP_SEGF|c_TEMP_SEGG)
#define N_TEMP_H		(c_TEMP_SEGB|c_TEMP_SEGC|c_TEMP_SEGE|c_TEMP_SEGF|c_TEMP_SEGG)
#define N_TEMP_h		(c_TEMP_SEGC|c_TEMP_SEGE|c_TEMP_SEGF|c_TEMP_SEGG)
#define N_TEMP_I		(c_TEMP_SEGE|c_TEMP_SEGF)
#define N_TEMP_L		(c_TEMP_SEGD|c_TEMP_SEGE|c_TEMP_SEGF)
#define N_TEMP_n		(c_TEMP_SEGC|c_TEMP_SEGE|c_TEMP_SEGG)
#define N_TEMP_o		(c_TEMP_SEGC|c_TEMP_SEGD|c_TEMP_SEGE|c_TEMP_SEGG)
#define N_TEMP_P		(c_TEMP_SEGA|c_TEMP_SEGB|c_TEMP_SEGE|c_TEMP_SEGF|c_TEMP_SEGG)
#define N_TEMP_U		(c_TEMP_SEGB|c_TEMP_SEGC|c_TEMP_SEGD|c_TEMP_SEGE|c_TEMP_SEGF)
#define N_TEMP_t		(c_TEMP_SEGD|c_SEGE|c_TEMP_SEGF|c_TEMP_SEGG)



/************ 右上角定时数码管：个位 ONES ************/
#define c_ONES_SEGA   BIT1
#define c_ONES_SEGB   BIT6
#define c_ONES_SEGC   BIT5
#define c_ONES_SEGD   BIT4
#define c_ONES_SEGE   BIT3
#define c_ONES_SEGF   BIT2
#define c_ONES_SEGG   BIT7

#define N_ONES_0      (c_ONES_SEGA|c_ONES_SEGB|c_ONES_SEGC|c_ONES_SEGD|c_ONES_SEGE|c_ONES_SEGF)
#define N_ONES_1      (c_ONES_SEGB|c_ONES_SEGC)
#define N_ONES_2      (c_ONES_SEGA|c_ONES_SEGB|c_ONES_SEGD|c_ONES_SEGE|c_ONES_SEGG)
#define N_ONES_3      (c_ONES_SEGA|c_ONES_SEGB|c_ONES_SEGC|c_ONES_SEGD|c_ONES_SEGG)
#define N_ONES_4      (c_ONES_SEGB|c_ONES_SEGC|c_ONES_SEGF|c_ONES_SEGG)
#define N_ONES_5      (c_ONES_SEGA|c_ONES_SEGC|c_ONES_SEGD|c_ONES_SEGF|c_ONES_SEGG)
#define N_ONES_6      (c_ONES_SEGA|c_ONES_SEGC|c_ONES_SEGD|c_ONES_SEGE|c_ONES_SEGF|c_ONES_SEGG)
#define N_ONES_7      (c_ONES_SEGA|c_ONES_SEGB|c_ONES_SEGC)
#define N_ONES_8      (c_ONES_SEGA|c_ONES_SEGB|c_ONES_SEGC|c_ONES_SEGD|c_ONES_SEGE|c_ONES_SEGF|c_ONES_SEGG)
#define N_ONES_9      (c_ONES_SEGA|c_ONES_SEGB|c_ONES_SEGC|c_ONES_SEGD|c_ONES_SEGF|c_ONES_SEGG)
#define N_ONES__      (c_ONES_SEGG)

#define N_ONES_A      (c_ONES_SEGA|c_ONES_SEGB|c_ONES_SEGC|c_ONES_SEGE|c_ONES_SEGF|c_ONES_SEGG)
#define N_ONES_B      (c_ONES_SEGC|c_ONES_SEGD|c_ONES_SEGE|c_ONES_SEGF|c_ONES_SEGG)
#define N_ONES_C      (c_ONES_SEGA|c_ONES_SEGD|c_ONES_SEGE|c_ONES_SEGF)
#define N_ONES_D      (c_ONES_SEGB|c_ONES_SEGC|c_ONES_SEGD|c_ONES_SEGE|c_ONES_SEGG)
#define N_ONES_E      (c_ONES_SEGA|c_ONES_SEGD|c_ONES_SEGE|c_ONES_SEGF|c_ONES_SEGG)
#define N_ONES_F      (c_ONES_SEGA|c_ONES_SEGE|c_ONES_SEGF|c_ONES_SEGG)
#define N_ONES_H      (c_ONES_SEGB|c_ONES_SEGC|c_ONES_SEGE|c_ONES_SEGF|c_ONES_SEGG)
#define N_ONES_h      (c_ONES_SEGC|c_ONES_SEGE|c_ONES_SEGF|c_ONES_SEGG)
#define N_ONES_I      (c_ONES_SEGE|c_ONES_SEGF)
#define N_ONES_L      (c_ONES_SEGD|c_ONES_SEGE|c_ONES_SEGF)
#define N_ONES_n      (c_ONES_SEGC|c_ONES_SEGE|c_ONES_SEGG)
#define N_ONES_o      (c_ONES_SEGC|c_ONES_SEGD|c_ONES_SEGE|c_ONES_SEGG)
#define N_ONES_P      (c_ONES_SEGA|c_ONES_SEGB|c_ONES_SEGE|c_ONES_SEGF|c_ONES_SEGG)
#define N_ONES_U      (c_ONES_SEGB|c_ONES_SEGC|c_ONES_SEGD|c_ONES_SEGE|c_ONES_SEGF)
#define N_ONES_t      (c_ONES_SEGD|c_ONES_SEGE|c_ONES_SEGF|c_ONES_SEGG)

/************ 右上角定时数码管：十位 TENS ************/
#define c_TENS_SEGA   BIT0
#define c_TENS_SEGB   BIT5
#define c_TENS_SEGC   BIT4
#define c_TENS_SEGD   BIT3
#define c_TENS_SEGE   BIT2
#define c_TENS_SEGF   BIT1
#define c_TENS_SEGG   BIT6

#define N_TENS_0      (c_TENS_SEGA|c_TENS_SEGB|c_TENS_SEGC|c_TENS_SEGD|c_TENS_SEGE|c_TENS_SEGF)
#define N_TENS_1      (c_TENS_SEGB|c_TENS_SEGC)
#define N_TENS_2      (c_TENS_SEGA|c_TENS_SEGB|c_TENS_SEGD|c_TENS_SEGE|c_TENS_SEGG)
#define N_TENS_3      (c_TENS_SEGA|c_TENS_SEGB|c_TENS_SEGC|c_TENS_SEGD|c_TENS_SEGG)
#define N_TENS_4      (c_TENS_SEGB|c_TENS_SEGC|c_TENS_SEGF|c_TENS_SEGG)
#define N_TENS_5      (c_TENS_SEGA|c_TENS_SEGC|c_TENS_SEGD|c_TENS_SEGF|c_TENS_SEGG)
#define N_TENS_6      (c_TENS_SEGA|c_TENS_SEGC|c_TENS_SEGD|c_TENS_SEGE|c_TENS_SEGF|c_TENS_SEGG)
#define N_TENS_7      (c_TENS_SEGA|c_TENS_SEGB|c_TENS_SEGC)
#define N_TENS_8      (c_TENS_SEGA|c_TENS_SEGB|c_TENS_SEGC|c_TENS_SEGD|c_TENS_SEGE|c_TENS_SEGF|c_TENS_SEGG)
#define N_TENS_9      (c_TENS_SEGA|c_TENS_SEGB|c_TENS_SEGC|c_TENS_SEGD|c_TENS_SEGF|c_TENS_SEGG)
#define N_TENS__      (c_TENS_SEGG)

#define N_TENS_A      (c_TENS_SEGA|c_TENS_SEGB|c_TENS_SEGC|c_TENS_SEGE|c_TENS_SEGF|c_TENS_SEGG)
#define N_TENS_B      (c_TENS_SEGC|c_TENS_SEGD|c_TENS_SEGE|c_TENS_SEGF|c_TENS_SEGG)
#define N_TENS_C      (c_TENS_SEGA|c_TENS_SEGD|c_TENS_SEGE|c_TENS_SEGF)
#define N_TENS_D      (c_TENS_SEGB|c_TENS_SEGC|c_TENS_SEGD|c_TENS_SEGE|c_TENS_SEGG)
#define N_TENS_E      (c_TENS_SEGA|c_TENS_SEGD|c_TENS_SEGE|c_TENS_SEGF|c_TENS_SEGG)
#define N_TENS_F      (c_TENS_SEGA|c_TENS_SEGE|c_TENS_SEGF|c_TENS_SEGG)
#define N_TENS_H      (c_TENS_SEGB|c_TENS_SEGC|c_TENS_SEGE|c_TENS_SEGF|c_TENS_SEGG)
#define N_TENS_h      (c_TENS_SEGC|c_TENS_SEGE|c_TENS_SEGF|c_TENS_SEGG)
#define N_TENS_I      (c_TENS_SEGE|c_TENS_SEGF)
#define N_TENS_L      (c_TENS_SEGD|c_TENS_SEGE|c_TENS_SEGF)
#define N_TENS_n      (c_TENS_SEGC|c_TENS_SEGE|c_TENS_SEGG)
#define N_TENS_o      (c_TENS_SEGC|c_TENS_SEGD|c_TENS_SEGE|c_TENS_SEGG)
#define N_TENS_P      (c_TENS_SEGA|c_TENS_SEGB|c_TENS_SEGE|c_TENS_SEGF|c_TENS_SEGG)
#define N_TENS_U      (c_TENS_SEGB|c_TENS_SEGC|c_TENS_SEGD|c_TENS_SEGE|c_TENS_SEGF)
#define N_TENS_t      (c_TENS_SEGD|c_TENS_SEGE|c_TENS_SEGF|c_TENS_SEGG)


extern void F_DispKey();
extern void F_DispClean();
extern void F_DispPowerUp();
//extern void F_DisplayDrive();
extern void F_RGBDry_IRQ();


extern void F_RgbTestDispDrv(U8 l_u8DisType);

extern U8 F_GetDispType();
extern void F_mDispInital();
extern void F_DispTskChange(unsigned char temp);

extern void F_mDisplayControl(void);

#endif  
 