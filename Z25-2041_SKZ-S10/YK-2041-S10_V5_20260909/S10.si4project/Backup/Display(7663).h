#ifndef _DISPLAY_H_
#define _DISPLAY_H_

extern U8 xdata g_u8DispCnt;      //传感器显示随机数(0~3随机)

extern U8 xdata g_u8LockBlinkTime;//童锁闪烁时间
#define C_LOCK_BLINK_TIME       30//单位100ms
extern U8 xdata g_u8DryingBlinkTime;//风干闪烁时间
#define C_DRYING_BLINK_TIME     50//单位100ms

extern U8 xdata g_u8KeyDispTime;  //按键显示时间
extern U8 xdata g_u8SleepDispTime;//睡眠显示时间
#define C_KEY_DISP_TIME         50//单位100ms

extern bit g_bHalfDispEn;         //半亮使能标记

extern U8 xdata g_u8VocDispState;
extern U8 xdata g_u8PM25DispState;
#define C_DISP_OFF        0
#define C_DISP_BLUE       1
#define C_DISP_GREEN      2
#define C_DISP_ORANGE     3
#define C_DISP_READ       4
#define C_DISP_ALL        5

extern U16 xdata g_u16DispCH2OData;            //甲醛显示数值
extern U16 xdata g_u16DispPm25Data;            //PM25显示数值
extern U16 xdata g_u16DispPm10Data;            //PM10显示数值
extern U16 xdata g_u16DispPm1Data;             //PM1显示数值

extern U8 code NumTable[];
extern U8 xdata DispBuf[];
extern U8 xdata ChipDispBuf[];
#define TOTAL_COM		12	//显示数  
#define COM_1			0
#define COM_2			1
#define COM_3			2
#define COM_4			3
#define COM_5			4
#define COM_6			5
#define COM_7			6
#define COM_8			7
#define COM_9			8
#define COM_10			9
#define COM_11			10
#define COM_12			11

#define F_SpecialDis_Up(x,y,z) 	 {  DispBuf[COM_2] |= x;\
                                    DispBuf[COM_3] |= y;\
                                    DispBuf[COM_4] |= z;}

#define F_SpecialDis_Down(x,y,z,h) {  DispBuf[COM_6] |= x;\
                                      DispBuf[COM_7] |= y;\
                                      DispBuf[COM_8] |= z;\
                                      DispBuf[COM_9] |= h;}

#define F_DispPlayHex_L(x)  {DispBuf[COM_2] |= NumTable[x >> 8];\
                             DispBuf[COM_3] |= NumTable[(x >> 4) & 0x0f];\
                             DispBuf[COM_4] |= NumTable[x & 0x0f];}

#define F_DispPlayHex_Down(x)  {DispBuf[COM_6] |= NumTable[x >> 12];\
                                DispBuf[COM_7] |= NumTable[(x >> 8) & 0x0f];\
                                DispBuf[COM_8] |= NumTable[(x >> 4) & 0x0f];\
                                DispBuf[COM_9] |= NumTable[x & 0x0f];}

enum DISP_TASK_NUM
{
	DIPS_NORMAL = 0,	
	DIPS_PM10,
	DIPS_PM1,
	DIPS_TEMP,
	DIPS_HUMI,
	DIPS_PM25,
    
	DIPS_QUICK_TEST,
	DIPS_SHRINK_TIME,
    
	DIPS_POWERUP,
	DISP_TOTAL_TASK,
};

/****PM1.0左方两个灯*****/                             
#define DIS_PM1_L()         DispBuf[COM_1] |= B00000011

#define DIS_PM1()           DispBuf[COM_1] |= B00011100        
#define DIS_PM10()          DispBuf[COM_1] |= B11100000        

#define DIS_PM25()          {DispBuf[COM_2] |= BIT0;\
                             DispBuf[COM_3] |= BIT0;\
                             DispBuf[COM_4] |= BIT0;}      


#define DIS_CLE_PM1_10_25() {DispBuf[COM_1] &= ~B11111100;\
                             DispBuf[COM_2] &= ~BIT0;\
                             DispBuf[COM_3] &= ~BIT0;\
                             DispBuf[COM_4] &= ~BIT0;}      

#define DIS_TEMP()          DispBuf[COM_5] |= BIT0
#define DIS_BAI_FEN_BI()    DispBuf[COM_5] |= BIT1
#define DIS_MESSAGE_KEY()   DispBuf[COM_5] |= BIT2
#define DIS_RESET()         DispBuf[COM_5] |= B00111000
/****风干右边灯,预留*****/                             
#define DIS_DRYING_R()      DispBuf[COM_5] |= BIT6
                             
#define DIS_DRYING()        DispBuf[COM_5] |= BIT7
                             
/****甲醛*****/                             
#define DIS_METHANAL()      {DispBuf[COM_6] |= BIT0;\
                             DispBuf[COM_7] |= BIT0;\
                             DispBuf[COM_8] |= BIT0;}

#define DIS_CLE_METHANAL()  {DispBuf[COM_6] &= ~BIT0;\
                             DispBuf[COM_7] &= ~BIT0;\
                             DispBuf[COM_8] &= ~BIT0;}

/****滤网*****/                             
#define DIS_STRAINER()       DispBuf[COM_9] |= BIT0

#define DIS_LOCK_KEY()       DispBuf[COM_10] |= B00001111  //按键灯和长按3s提示灯
#define CLR_DIS_LOCK_KEY()   DispBuf[COM_10] &= ~B00001111  //按键灯和长按3s提示灯
#define DIS_NETION_KEY()     DispBuf[COM_10] |= BIT4
#define DIS_DRYING_KEY()     DispBuf[COM_10] |= BIT5
#define DIS_DP()             DispBuf[COM_10] |= BIT6
#define DIS_WIFI()           DispBuf[COM_10] |= BIT7
                             
#define DIS_LIGHT_KEY()      DispBuf[COM_11] |= BIT0
#define DIS_MODE_KEY()       DispBuf[COM_11] |= BIT1
#define DIS_LACK()           DispBuf[COM_11] |= BIT2
#define DIS_SLEEP()          DispBuf[COM_11] |= BIT3
#define DIS_HUMI()           DispBuf[COM_11] |= BIT4
#define DIS_AI()             DispBuf[COM_11] |= BIT5
#define DIS_LOCK()           DispBuf[COM_11] |= BIT6
#define DIS_GEAR_3()         DispBuf[COM_11] |= BIT7
                             
#define DIS_NETION()         DispBuf[COM_12] |= BIT0
#define DIS_GEAR_2()         DispBuf[COM_12] |= BIT1
#define DIS_GEAR_1()         DispBuf[COM_12] |= BIT2

//-------------------------------------------------------------------
typedef struct _TYPE_DISP_TASK
{
	unsigned char		Index;
	unsigned int		LastTime;
	unsigned char		Blink;
	void (*DispFunc)(void);	
}TYPE_DISP_TASK;

#define c_SEGA		BIT1
#define c_SEGB		BIT6
#define c_SEGC		BIT5
#define c_SEGD		BIT4
#define c_SEGE		BIT3
#define c_SEGF		BIT2
#define c_SEGG		BIT7
#define c_SEGDP		BIT0

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
#define N_I		(c_SEGE|c_SEGF)
#define N_L		(c_SEGD|c_SEGE|c_SEGF)
#define N_n		(c_SEGC|c_SEGE|c_SEGG)
#define N_o		(c_SEGC|c_SEGD|c_SEGE|c_SEGG)
#define N_P		(c_SEGA|c_SEGB|c_SEGE|c_SEGF|c_SEGG)
#define N_U		(c_SEGB|c_SEGC|c_SEGD|c_SEGE|c_SEGF)
#define N_t		(c_SEGD|c_SEGE|c_SEGF|c_SEGG)

extern void F_DispKey();
extern void F_DispClean();
extern void F_DispPowerUp();
extern void F_DisplayDrive();

extern U8 F_GetDispType();
extern void F_mDispInital();
extern void F_DispTskChange(unsigned char temp);

extern void F_mDisplayControl(void);

#endif  


#ifndef _DISPLAY_H_
#define _DISPLAY_H_
// =================================================================================
// 宏定义
// =================================================================================
#define C_DISP_MAX_BRIGHT_LEVEL  10   
#define C_FONT_WIDTH             3    // 数字字模宽度 (3列)
#define C_FONT_HEIGHT            9    // 字高9行 (A1-A9)

// =================================================================================
// 数据结构定义
// =================================================================================

// 1. 区域映射对象 (硬件抽象层 HAL)
// 核心作用：将 逻辑坐标(x,y) 转换为 物理坐标(COM,SEG)
// 只要这个表填对了，画图就不会覆盖到指示灯
typedef struct
{
    // [X轴] 列映射表: 逻辑列Index -> 物理COM号
    U8 code *pColToComMap;   

    // [Y轴] 行映射表: 逻辑行Index -> 物理SEG号
    U8 code *pRowToSegMap;   

    U8 u8DigitCount;         // 该区域包含几个数字 (仅显示数字时使用)
} TYPE_DIGITAL_AREA;

// 2. 图标数据对象
typedef struct
{
    // 图像数据指针 (U16数组，纵向取模)
    // LSB(Bit0)=A1(顶), Bit8=A9(底)
    U16 code *pIconData; 
    
    U8 u8Width;          // 图像宽度 (占用多少列)
    U8 u8Height;         // 图像高度 (通常固定为9)
} TYPE_ICON_DATA;

// =================================================================================
// 外部变量
// =================================================================================
extern U8 xdata g_u8DispBright;                 // 全局亮度 (0-10)
extern U8 xdata g_t_u8TMS6644_DispBuf[9][16];   // 显存 [COM][SEG]

// =================================================================================
// 接口函数
// =================================================================================

// 1. 区域初始化/清除 (只清除映射表覆盖的像素，不灭指示灯)
void F_TMS_ClearArea(TYPE_DIGITAL_AREA *l_pArea, U8 l_u8Width);

// 2. 显示数字 (自动处理 2位/3位 和 间隔)
void F_TMS_ShowNum(TYPE_DIGITAL_AREA *l_pArea, U16 l_u16Num);

// 3. 显示图标 (支持 9x9 或任意尺寸)
// l_u8Ofst: 在该区域内的起始列偏移
void F_TMS_ShowIcon(TYPE_DIGITAL_AREA *l_pArea, TYPE_ICON_DATA *l_pIcon, U8 l_u8Ofst);

// 4. 独立控制指示灯 (直接操作物理坐标)
// l_bState: 1=亮, 0=灭
void F_TMS_SetLed(U8 l_u8Com, U8 l_u8Seg, bit l_bState);

#endif
