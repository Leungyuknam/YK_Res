#ifndef	_TK_H
#define	_TK_H

//===============================================================================================
//								中颖 SH79F326/328/6481 触摸库 V2
//							--------------------------------------
//	触摸按键子程序接口
//
//===============================================================================================


//-------------------------------------------------------------------------------------
//								配置区
// 打***可以改
//-------------------------------------------------------------------------------------

/* 触摸时钟频率：与系统时钟一样 */
#define     TK_CLOCK_FREQ           (24)                //（***）  触摸外设时钟频率，单位 MHz


// 封装: 后缀带A和不带A封装一样
#define     SH79F9463A_28PIN        0
#define     SH79F9463A_32PIN        1					
#define     PACKAGE                 SH79F9463A_32PIN      // (***) 封装选择

//扫描周期:
#define     TK_SCAN_PERIOD          (16)                // (***) 触摸扫描周期，单位ms

// 手摸
#define     HAND_TK_ENABLE          0                   // (***) 是否支持手直接摸弹簧(填1支持,填0不支持)

// 调试
//#define     TEST_TK_DATA                                // (***) 使用串口调试输出触摸参数
// 参数输出3选1（仅在TEST_TK_DATA有效时生效）
#define     TEST_LOW_DATA           0               // (这是类型定义不要改) 调试输出低频参数
#define     TEST_HIGH_DATA          1               // (这是类型定义不要改) 调试输出高频参数
#define     TEST_ALL_DATA           2               // (这是类型定义不要改) 调试输出双频参数 （例如8个按键时，通道0~7低频数据，通道8~15是高频数据）
#define     TEST_TK_DATA_TYPE       TEST_ALL_DATA   // 在这里选择 (***)


// 触摸参数调整
//#define     ADJUST_TK_PARA          // (***)	是否使用PC对触摸进行参数调整 TEST_TK_DATA有效时ADJUST_TK_PARA才会有效*/

//-------------------------------------------------------------------------------------
//								校准用
//      由ADJUST_TK_PARA使能
//-------------------------------------------------------------------------------------
// STouchKey下行的指令
#define     PC_ADJUST_TK            0x01        // 校准
#define     PC_ACK_OK               0x02        // 确定（中间过程弹窗时点的确定）
#define     PC_FREQ_SWITCH          0x07        // 频率切换

// 校准过程的差值线限定
#define     TK_DIFF_TARGET          (300)       // 目标差值是
#define     TK_DIFF_MIN             (TK_DIFF_TARGET - 100)  // 最小差值线
#define     TK_DIFF_MAX             (TK_DIFF_TARGET + 100)  // 最大差值线


//-------------------------------------------------------------------------------------
//								简化定义
//-------------------------------------------------------------------------------------
#define     b0          0x01
#define     b1          0x02
#define     b2          0x04
#define     b3          0x08
#define     b4          0x10
#define     b5          0x20
#define     b6          0x40
#define     b7          0x80

#define     U8          unsigned char
#define     U16         unsigned int
#define     U32         unsigned long

//-------------------------------------------------------------------------------------
//								配置
//-------------------------------------------------------------------------------------
#define     MAX_KEYS                (16)        // （禁止修改）当前触摸库支持的最大按键数量
typedef struct
{
    U8  num;                        // 按键个数     这个数要<=MAX_KEYS
    U8  chn[MAX_KEYS];              // 通道         0表示TK1....23表示TK24
    U16 LFDiv[MAX_KEYS];            // 低频放大倍数
    U16 HFDiv[MAX_KEYS];            // 高频放大倍数
    U16 TKMax[MAX_KEYS*2];          // 按下的最大变化量（不是填写的阈值）
    U16 ripple[MAX_KEYS*2];         // 原始纹波


    U8  LFValue;                    // 低频率
    U8  LFVref;                     // 低频触摸电压
    U8  HFValue;                    // 高频率
    U8  HFVref;                     // 高频触摸电压

    U8  outputFilter;               // 按键确认次数（这个数不能写0） 周期*2(高低双频)*(outputFilter)
    U8  inputFilter;                // 原始数据去抖
    U8  inputFilterMax;             // 原始数据去抖上限次数
    U8  keyOutStrict;               // 键值输出条件更严格（0：单频，1：双频）

    //自动重新适应环境补丁:
    U8  patch1;                     // 补丁1: 环境突变导致按键采集时间变化了，则自动重新适应(填1有效,填0此补丁无效)
    U8  patch1_Para;                // 参数:有N个按键忽大/忽小表示把板子从桌子上举起来再放下去
                                    // 加入原因: 把面板突然拿走，或者把面板重新盖上去
                                    //
    U8  patch2;                     // 补丁2: (填1有效,填0此补丁无效)
    U8  patch3;                     // 补丁3: 是否支持手直接摸弹簧(填1支持,填0不支持)
    U8  CSICounter;                 // CSI异常去抖次数
    U8  patch4;                     // 补丁4：顶线索引
    U16 dischargeTime;              // 放电时间（通道间隔时间）
} Struct_TK_Para;

extern Struct_TK_Para code TK_Para;
extern U16 code TK_HandPara[MAX_KEYS*2];


//-------------------------------------------------------------------------------------
//								应用程序接口
//-------------------------------------------------------------------------------------
//接口
void InitKey(void);                 // 初始化
void stop_scan_tk(void);            // 一个扫描周期开始的时候调用一次
U8 SanTk(void);                     // 在一个周期内至少调用N次 (N=触摸按键个数)
void TK_Isr(void);                  // 触摸中断接口
void tk_base_update_config(U8 period, U8 step, U8 disableTime);

//一次性发送模式
extern U16  xdata key_baseline[MAX_KEYS*2]; // 基准值
extern U16  xdata key_pre[MAX_KEYS*2];      // 基准值
extern U8   xdata freqForDataMsg;           // 当前处理数据对应的频率
extern U8   xdata freqForKeyJudge;          // 当前用于判断按键使用的频率
extern U16  xdata key_value;                // 结果
extern U16  xdata Tab_TKMax[MAX_KEYS*2];    // 灵敏度
extern U16  xdata HF_DIV[MAX_KEYS];         // 高频放大倍数
extern U16  xdata LF_DIV[MAX_KEYS];         // 低频放大倍数

//结果
U16 Key_DataInput(void);                    // 返回按键值(带无效按键判断功能)
U16 catchKey(void);                         // 返回按键值(带防卡键功能)
extern U16  xdata key_value;                // 返回按键值(原始结果)
extern U8   xdata gBTouchReason;

extern bit gbFlag1ms_ScanTK;
//手动重新适应环境补丁
void reset_TK(void);                        // note1:(如发现按键按下后一直没有释放，可以调用一次)
void ScanTKPress(void);

//调试用
void tk_debug_uart_init(void);
void tk_debug_uart_txd(void);

extern  U16     xdata       gWrCurKey;

#endif
