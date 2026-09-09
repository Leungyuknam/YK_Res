#include "h\Allinclude.h" // 引用你的公共头文件

#define C_RGB_PWM0_PERIOD       3200U  // 24MHz / 3200 = 7.5kHz
#define C_RGB_PWM0_DUTY_STEP      32U  // 3200 / 100
#define C_RGB_PCA_TOP           PWM_PERIOD_ARR
#define C_RGB_PCA_DUTY_STEP       16U  // 1600 / 100，双斜坡频率为7.5kHz

// ------------------- 静态变量/常量 -------------------

// 颜色配置表 (基准值 0-100)
// 对应 E_COLOR_INDEX 的顺序
U8 code t_RgbColorTable[COLOR_MAX][3] = 
{
    {	0,   0,   0},  // OFF
    {	10,  50,  0},  // WHITE
    {	50,  35,  0},  // MIX
    {	50,  10,   0},  // YELLOW
};

// 实例化驱动结构体 (内部使用 static 隐藏，外部只能通过函数操作)
static TYPE_RGB_CTRL s_stRgb = {0};

// ------------------- 内部函数 -------------------

// 将0~100百分比换算为硬件PWM比较值
static void F_RgbSetRawPWM(U8 u8R, U8 u8G, U8 u8B)
{
    U16 u16RedDuty;
    U16 u16GreenCompare;

    if(u8R > 100) u8R = 100;
    if(u8G > 100) u8G = 100;
    if(u8B > 100) u8B = 100;

    // PWM0高电平有效：Duty = Period * 百分比 / 100
    u16RedDuty = (U16)u8R * C_RGB_PWM0_DUTY_STEP;

    // PCA0模块0使用反相输出，比较值 = TOP - TOP * 百分比 / 100
    if(u8G == 0)
    {
        u16GreenCompare = C_RGB_PCA_TOP;
    }
    else if(u8G >= 100)
    {
        u16GreenCompare = 0;
    }
    else
    {
        u16GreenCompare = C_RGB_PCA_TOP - ((U16)u8G * C_RGB_PCA_DUTY_STEP);
    }

    s_stRgb.u8RedDuty   = u8R;
    s_stRgb.u8GreenDuty = u8G;
    s_stRgb.u8BlueDuty  = u8B; // 仅记录请求值，当前硬件方案不使用B通道

    SET_BANK_0();
    PWM0DL = (U8)(u16RedDuty & 0xFF);
    PWM0DH = (U8)(u16RedDuty >> 8);

    SET_BANK_1();
    P0CPH0 = (U8)(u16GreenCompare >> 8); // PCA要求先写高字节
    P0CPL0 = (U8)(u16GreenCompare & 0xFF);
    SET_BANK_0();

    P_RGB_B = 0; // B通道固定关闭
}

// ------------------- 外部接口实现 -------------------

void F_RgbInit(void)
{
    // 先保持三个MOS栅极为低，完成关闭比较值后再开放复用输出
    P_RGB_R = 0;
    P_RGB_G = 0;
    P_RGB_B = 0;

    SET_BANK_0();
    PWM0CON = 0x00;
    PWM0PL = (U8)(C_RGB_PWM0_PERIOD & 0xFF);
    PWM0PH = (U8)(C_RGB_PWM0_PERIOD >> 8);
    PWM0DL = 0;
    PWM0DH = 0;

    SET_BANK_1();
    PWMCR = (PWMCR & 0xF8) | 0x05; // PWM0映射到P3.5
    CEXCR &= ~0x07;                // P0CEX0映射到P3.4，保留模块1映射

    // 模块0：16位相频修正PWM、反相后高电平驱动MOS；先关闭输出
    P0CPM0 = 0xF4;
    P0CPH0 = (U8)(C_RGB_PCA_TOP >> 8);
    P0CPL0 = (U8)(C_RGB_PCA_TOP & 0xFF);
    PCACON |= 0x01;                // 公共PCA计数器保持运行
    P0CPM0 |= 0x08;                // 比较值已为0%后再开放G输出

    SET_BANK_0();
    PWM0CON = 0x81;                // Fsys/1、高电平有效、开放PWM0输出

    s_stRgb.bEnable = 1;
    s_stRgb.bRainbow = 0;
    F_RgbSetRawPWM(0, 0, 0);
}

/**************************************************
* 函数名称：F_RgbControl_Task
* 函数功能：RGB 业务逻辑调度
* 调用周期：10ms
**************************************************/
void F_RgbControl_Task(void)
{
    U8 u8TargetIdx = 0;
    U8 bShouldLightOff = 0; // 标志位：是否需要强制灭灯

    if(F_GetTimeTick(C_TIME_100ms))
    {
        // ==========================================
        // 1. 综合判断：在哪些情况下氛围灯必须强制关闭？
        // ==========================================
        // 核心条件 A：系统关机
        if(!g_stSys.bTurnOn)
        {
            bShouldLightOff = 1;
        }
        // 核心条件 B：用户通过按键手动关闭了氛围灯
        else if(g_stSys.u8LightState == LIGHT_OFF)
        {
            bShouldLightOff = 1;
        }
        // 核心条件 C：屏幕处于物理黑屏状态 (熄屏模式 或 睡眠模式，且倒计时为0)
        else if(g_stSys.bScreenAlwaysOn == 0 && g_stSys.u16Cnt_WakeDisp == 0)
        {
            bShouldLightOff = 1;
        }

        // ==========================================
        // 2. 执行 PWM 下发
        // ==========================================
        if(bShouldLightOff)
        {
            // 满足任何一个强制灭灯条件，下发 0，直接黑掉
            F_RgbSetRawPWM(0, 0, 0);
        }
        else
        {
            // 允许亮灯，获取目标颜色索引 (防止数组越界)
            u8TargetIdx = g_stSys.u8ColorIdx;
            if(u8TargetIdx >= COLOR_MAX) 
            {
                u8TargetIdx = 0; // 默认给 0 (OFF)
            }

            // 直接把颜色表里的数字当作最终占空比下发 (0~100)
            F_RgbSetRawPWM(
                t_RgbColorTable[u8TargetIdx][0], // R通道占空比
                t_RgbColorTable[u8TargetIdx][1], // G通道占空比
                t_RgbColorTable[u8TargetIdx][2]  // B通道占空比
            );
        }
    }
}