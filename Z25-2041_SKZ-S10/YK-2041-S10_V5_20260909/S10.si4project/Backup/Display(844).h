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
// l_u8LedIndex: 枚举索引 (例如 L_WIFI)
// l_bState:     1=亮, 0=灭
void F_TMS_SetLed(U8 l_u8LedIndex, bit l_bState);


enum DISP_TASK_NUM
{
	DIPS_NORMAL = 0,	  
	DIPS_POWERUP,
	DISP_TOTAL_TASK,
};


typedef struct _TYPE_DISP_TASK
{
	unsigned char		Index;
	unsigned int		LastTime;
	unsigned char		Blink;
	void (*DispFunc)(void);	
}TYPE_DISP_TASK;


typedef struct
{
    U8 u8Com;   // 物理 COM 口
    U8 u8Seg;   // 物理 SEG 口
} TYPE_LED_MAP;


extern U8 xdata g_u8DryingBlinkTime;           //风干闪烁时间
extern U8 xdata g_u8WifiBlinkTime;            //WiFi闪烁时间
extern U8 xdata g_u8TimeHBlinkTime;            //定时闪烁时间
extern U8 xdata g_u8WaterBlinkTime;            //余水闪烁时间
                                   
extern U8 xdata g_u8GlobalDispState;           //当前显示模式(全亮、息屏)
extern U8 xdata g_u8WakeDispTime;              //唤醒显示时间



extern void F_DispKey();
extern void F_DispClean();
extern void F_DispPowerUp();
extern void F_DisplayDrive();

extern U8 F_GetDispType();
extern void F_mDispInital();
extern void F_DispTskChange(unsigned char temp);

extern void F_mDisplayControl(void);



#endif
