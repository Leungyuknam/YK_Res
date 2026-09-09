#ifndef _KEYFUNCTION_H_
#define _KEYFUNCTION_H_

//// =================================================================================
//// 1. 模式定义 (扁平化管理，共5种)
//// 循环顺序：高(0)→智(1)→睡(2)→低(3)→中(4) (规格书 3. 按键说明)
//// =================================================================================
//#define MODE_HIGH           0       // 高档模式
//#define MODE_SMART          1       // 智能模式
//#define MODE_SLEEP          2       // 睡眠模式
//#define MODE_LOW            3       // 低档模式
//#define MODE_MID            4       // 中档模式

//#define MODE_MAX_COUNT      5       // 模式总数

//// =================================================================================
//// 2. 硬件输出等级 (风速/PWM等级) -> 对应 t_FanDutyMap 的下标
//// =================================================================================
//#define SPEED_STOP          0       // 停转 (0%)
//#define SPEED_SLEEP         1       // 睡眠档 (10%)
//#define SPEED_LOW           2       // 低档 (28%)
//#define SPEED_MID           3       // 中档 (37%)
//#define SPEED_HIGH          4       // 高档 (45%)


// =================================================================================
// 1. 模式定义 (扁平化管理，共6种)
// 规格书循环顺序：1档(0)→2档(1)→3档(2)→4档(3)→睡眠(4)→智能(5) 
// 这样在按键函数里直接 if(++g_stSys.u8WorkMode >= MODE_MAX_COUNT) g_stSys.u8WorkMode = 0; 即可
// =================================================================================
#define MODE_MANUAL_1       0       // 手动模式1档
#define MODE_MANUAL_2       1       // 手动模式2档
#define MODE_MANUAL_3       2       // 手动模式3档
#define MODE_MANUAL_4       3       // 手动模式4档
#define MODE_SLEEP          4       // 睡眠模式
#define MODE_SMART          5       // ai湿随温变模式

#define MODE_MAX_COUNT      6       // 模式总数

// =================================================================================
// 2. 硬件输出等级 (Speed Level) -> 对应 t_FanDutyMap 的下标
// =================================================================================
#define SPEED_STOP          0       // 停转 (0%)
#define SPEED_SLEEP         1       // 睡眠档 (30%)
#define SPEED_GEAR_1        2       // 1档 (40%)
#define SPEED_GEAR_2        3       // 2档 (60%)
#define SPEED_GEAR_3        4       // 3档 (80%)
#define SPEED_GEAR_4        5       // 4档 (100%)

// =================================================================================
// 3. 辅助状态宏
// =================================================================================
#define LIGHT_OFF           0       
#define LIGHT_ON            1       

#define WIFI_DISCONNECT     0       
#define WIFI_CONNECTING     1       
#define WIFI_AP_MODE        2       
#define WIFI_CONNECTED      3       

#define C_TIME_MAX          12      // 定时最大12小时
#define C_DISP_WAKE_TIME    300      // 唤醒亮屏时间 (30秒)
#define C_WATER_REMIND_MIN	4320

// --- 传感器显示限幅 ---
#define TEMP_DISP_MAX       99      
#define TEMP_DISP_MIN       1       
#define HUMI_DISP_MAX       99      
#define HUMI_DISP_MIN       1 

// =================================================================================
// 4. 系统全量状态结构体 (ST_SYSTEM_DATA)
// =================================================================================
typedef struct 
{
    // ==========================================
    // [1] 核心状态与模式 (Core & Mode)
    // ==========================================
    U8 bTurnOn;             // 开关机标志 (1:开, 0:关)
    U8 u8WorkMode;          // 工作模式 (高/中/低/睡眠/智能)
    U8 u8FanSpeed;          // 当前实际风速档位 (STOP/SLEEP/LOW/MID/HIGH)
    
    // ==========================================
    // [2] 附加功能与负载 (Loads & Features)
    // ==========================================
    U8 bIsAirDrying;        // 是否正在风干 (1:风干中)
    U8 bUvOn;               // UV杀菌灯开关
    U8 bAnionOn;            // 负离子开关

    // ==========================================
    // [3] 定时器与倒计时 (Timers & Counters)
    // ==========================================
    U8 u8TimeSetData;       // 定时设置值 (0, 2, 4... 12小时)
    U16 u16TimeCnt;         // 定时剩余时间 (秒)
    U16 u16Cnt_AirDry;        // 风干剩余时间 (秒)

    // ==========================================
    // [4] 显示与 UI 交互 (Display & UI)
    // ==========================================
    U8 bScreenAlwaysOn;     // 屏幕常亮标志 (1:常亮, 0:熄屏模式)
    U16 u16Cnt_WakeDisp;    // 息屏唤醒倒计时 (100ms基准)
    U8 u8Cnt_TimeSet;       // 定时设置界面闪烁倒计时
    U8 u8DispBright;        // 当前屏幕全局亮度 (建议 0~8 等级)
    U8 u8LightState;        // 氛围灯用户开关状态 (ON/OFF)
    U8 u8ColorIdx;          // 氛围灯当前颜色索引 (OFF/橘/白/黄)
	U8 u8AirDryWarnTick;
    // ==========================================
    // [5] 保护与报警标志 (Protections & Alarms)
    // ==========================================
    U8 bWaterLack;          // 缺水物理标志 (1:缺水)
    U8 bHumiErr;            // 温湿度传感器故障标志 (1:故障)
	U8 bTankRemoved;
	U8 bCleanRemind;        // 清洁提醒激活标志 (1:正在提醒)
    U16 u16StandbyTimerMin; // 待机累计时间 (分钟)，72H = 4320分钟
    // ==========================================
    // [6] 温湿度传感器数据 (Sensor Data)
    // ==========================================
    U8 u8HumiState;         // I2C读取状态机流转标志
    U8 u8HumiLinkTime;      // I2C通讯超时看门狗计时
    U16 u16HumiRaw10;       // 湿度底层原始读取值 (放大10倍)
    U8 bHumiDataValid;      // 已读取到有效湿度数据
    S16 s16TempRaw10;       // 温度底层原始读取值 (放大10倍)
    U8 u8DispHumi;          // 处理后的应用层湿度 (含补偿及模拟走字)
    S8 s8DispTemp;          // 处理后的应用层温度 (四舍五入并平滑)
	
	U8 bEepromEn;			//掉电记忆使能
	U8 bEepromWriteBan;		//写禁止控制
	
    // ==========================================
    // [7] 自检/快检相关
    // ==========================================	
	U8 bQuickTest;		//快检标志位
	
	// ==========================================
    // [8] 内部逻辑辅助标志
    // ==========================================    
    U8 bAiJustEntered;    // 智能模式初次切入标志

} ST_SYSTEM_DATA;
extern ST_SYSTEM_DATA xdata g_stSys;

extern unsigned char unFlagKey1;

extern U8 xdata g_u8KeyData;
#define c_KEY_POWER 	BIT1
#define c_KEY_MODE		BIT0
#define c_KEY_TIME		BIT2
#define c_KEY_LIGHT		BIT3

#define c_KEY_SELFTEST      (c_KEY_LIGHT|c_KEY_POWER)
#define c_KEY_WIFI_RST   	(c_KEY_TIME|c_KEY_POWER) //WIFI复位
#define c_KEY_WIFI_CANCEL	(c_KEY_TIME|c_KEY_MODE) //WIFI解绑
#define c_KEY_QUICKTEST		(c_KEY_LIGHT|c_KEY_POWER|c_KEY_MODE|c_KEY_TIME)

// =================================================================================
// 5. 函数声明
// =================================================================================
//短按
extern void F_PowerKey();
extern void F_ModeKey();
extern void F_TimeKey();
extern void F_FWLightKey();

//长按
extern void F_QuickTestKey();
extern void F_WifiKey();
extern void F_ScreenLightKey();
extern void F_WifiCancelKey();
extern void F_WifiResetKey();
#endif
