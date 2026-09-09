#ifndef _DISPLAY_H_
#define _DISPLAY_H_

// =================================================================================
// 1. 物理引脚映射宏 (对应 PCB)
// =================================================================================
#define PCOM1 0
#define PCOM2 1
#define PCOM3 2
#define PCOM4 3
#define PCOM5 4
#define PCOM6 5
#define PCOM7 6
#define PCOM8 7
#define PCOM9 8

#define PSEG1 0
#define PSEG2 1
#define PSEG3 2
#define PSEG4 3
#define PSEG5 4
#define PSEG6 5
#define PSEG7 6
#define PSEG8 7
#define PSEG9 8
#define PSEG10 9
#define PSEG11 10
#define PSEG12 11
#define PSEG13 12
#define PSEG14 13
#define PSEG15 14
#define PSEG16 15

// =================================================================================
// 2. 显示常量定义
// =================================================================================
#define C_DISP_BRIGHT_MAX       10      
#define C_MATRIX_BRIGHT_PCT     30      /* dot-matrix brightness percent vs keys; lower = dimmer */      
#define C_FONT_WIDTH            4       
#define C_FONT_HEIGHT           9       

// 故障字符宏定义
#define CH_F 0
#define CH_H 1
#define CH_L 2
#define CH_P 3
#define CH_S 4
#define CH_n 5

// =================================================================================
// 3. 数据结构定义 (HAL 层结构体)
// =================================================================================
// 区域映射对象
typedef struct
{
    U8 code *pColToComMap;   // 列 -> COM
    U8 code *pRowToSegMap;   // 行 -> SEG
    U8 u8DigitCount;         // 位数
} TYPE_DIGITAL_AREA;

// 图标数据对象
typedef struct
{
    U16 code *pIconData;     
    U8 u8Width;              
    U8 u8Height;             
} TYPE_ICON_DATA;

// 单个像素点坐标
typedef struct
{
    U8 u8Com;
    U8 u8Seg;
} TYPE_LED_PIXEL;

// LED 组合映射 (一对多)
typedef struct
{
    U8 u8Count;              // 灯珠数量
    TYPE_LED_PIXEL code *pList; // 坐标数组指针
} TYPE_LED_MAP;

// =================================================================================
// 4. 逻辑索引与任务枚举 (业务层使用)
// =================================================================================
typedef enum
{
    L_LIGHT_KEY = 0,    // 灯光键灯
    L_TIME_KEY,         // 定时键灯
    L_MODE_KEY,         // 模式键灯
    L_POWER_KEY,        // 开关键灯
    L_TIME,             // 定时图标 (H)
    L_AIR_DRY,          // 风干图标
    L_WATER_LACK,       // 缺水图标
    L_WIFI,             // WiFi图标
    L_CLEAN_REMIND,     // 余水图标
    L_HUMI_PERCENT,     // 湿度百分号 (%)
    _LED_TOTAL_         // 总数
} LIST_LED_ENUM;

enum DISP_TASK_NUM
{
    DIPS_NORMAL = 0,    
    DIPS_POWERUP,
    DIPS_TIME_SET,
    DIPS_WATER_LACK,    // 无限循环的缺水任务
    DIPS_WATER_FULL,    // 存活3秒的满水任务
    DIPS_AIR_DRY,       // 无限循环的风干任务
    DIPS_CLEAN_REMIND,
    DIPS_QUICK_TEST,
    DIPS_TANK_REMOVED,
    DIPS_MODE_SET,      // mode/gear setting state: 3s hold frame + one-shot anim
    DISP_TOTAL_TASK
};

// =================================================================================
// 5. 外部接口声明
// =================================================================================
// 系统调度 API
void F_mDispInital(void);
void F_mDisplayControl(void);
void F_DispTskChange(unsigned char u8TaskIndex);
U8 F_GetDispType(void);
void F_DispEnterModeSet(void);   // mode key first press: enter setting state
void F_DispModeSwitched(void);
void F_DispSetTimeModified(void);   // mode key cycle inside setting: mark switched
U8 F_ReadErrState(void);

// 外部依赖 API
extern void F_TMS6644_Display_PWM(U8 *pBuf, U8 u8Len);
extern void F_DispResetAnim(void);

// 底层绘图 API (开放给 SelfTest.c 等使用)
extern void F_DispClean(void);
extern void F_TMS_SetLed(U8 l_u8LedIndex, bit l_bState);
extern void F_TMS_SetLed_Ex(U8 l_u8LedIndex, U8 u8BrightLevel);
extern void F_TMS_ShowNum(TYPE_DIGITAL_AREA *l_pArea, U16 l_u16Num);
extern void F_TMS_ShowErrChars(U8 u8Char1, U8 u8Char2);
extern void F_TMS_Mapping_Write_Ex(TYPE_DIGITAL_AREA *l_pArea, U8 l_u8LogicCol, U16 l_u16ColData, U8 u8BrightLevel);

// 全局变量与开放结构体
extern U8 xdata g_t_u8TMS6644_DispBuf[9][16];
extern TYPE_LED_MAP code t_stLedMap[_LED_TOTAL_];
extern TYPE_DIGITAL_AREA code g_Area_Number;
extern TYPE_DIGITAL_AREA code g_Area_Icon9x9;

#endif