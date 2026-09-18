#include "h\Allinclude.h"

// -----------------------------------------------------------------
// 缓冲区与状态
// -----------------------------------------------------------------
U8 xdata DispBuf[COM_MAX_CNT] = {0};
U8 xdata DriveDispBuf[COM_MAX_CNT] = {0};
bit B_DispUpdating = 0;

U8 xdata DispLastT = 0;
U8 xdata DispIndex = 0;

// 人感待机使用的 1 Hz 闪烁（亮 500 ms、灭 500 ms）
static U8  xdata s_u8BlinkCnt = 0;
static bit       s_bBlinkPhase = 1;     // 1=显示，0=隐藏

// 显示任务表所需的函数前置声明
static void F_DispNormal(void);
static void F_DispPowerUp(void);

code TYPE_DISP_TASK DispTskTbl[DISP_TOTAL_TASK] =
{
    { DIPS_NORMAL,  0,  0, &F_DispNormal  },
    { DIPS_POWERUP, 10, 0, &F_DispPowerUp },     // 10 * 100 ms = 全亮 1 秒
};

// =================================================================
// 底层矩阵扫描
// =================================================================
static void F_AllSegOff(void)
{
    P_SEG1 = SEG_OFF;
    P_SEG2 = SEG_OFF;
    P_SEG3 = SEG_OFF;
    P_SEG4 = SEG_OFF;
}

static void F_AllComOff(void)
{
    P_COM1 = COM_OFF;
    P_COM2 = COM_OFF;
}

// 由定时器 0 中断每 4 ms 调用一次。
// 2 个 COM 构成 8 ms 一帧，刷新率为 125 Hz。
void F_DisplayDrive_IRQ(void)
{
    static U8 s_u8ComStep = 0;
    U8 u8Data;

    if(B_DispUpdating) return;

    F_AllComOff();
    F_AllSegOff();

    u8Data = DriveDispBuf[s_u8ComStep];
    if(u8Data & BIT_SEG1) P_SEG1 = SEG_ON;
    if(u8Data & BIT_SEG2) P_SEG2 = SEG_ON;
    if(u8Data & BIT_SEG3) P_SEG3 = SEG_ON;
    if(u8Data & BIT_SEG4) P_SEG4 = SEG_ON;

    if(s_u8ComStep == COM_1) P_COM1 = COM_ON;
    else                     P_COM2 = COM_ON;

    s_u8ComStep++;
    if(s_u8ComStep >= COM_MAX_CNT) s_u8ComStep = 0;
}

void F_SyncDispBuf(void)
{
    U8 i;
    B_DispUpdating = 1;
    for(i = 0; i < COM_MAX_CNT; i++) DriveDispBuf[i] = DispBuf[i];
    B_DispUpdating = 0;
}

void F_DispClean(void)
{
    U8 i;
    for(i = 0; i < COM_MAX_CNT; i++) DispBuf[i] = 0;
}

// =================================================================
// 显示任务
// =================================================================

// 上电动画：8 个指示灯全亮 1 秒
static void F_DispPowerUp(void)
{
    DispBuf[COM_1] = 0x0F;       // 点亮 SEG1～SEG4
    DispBuf[COM_2] = 0x0F;
}

// 根据整机状态渲染 DIPS_NORMAL
static void F_DispNormal(void)
{
    // 倾倒或关机时熄灭全部指示灯。
    if(g_stSys.bIsTipOver) return;
    if(!g_stSys.bTurnOn) return;

    // ---- 工作状态 ----
    DIS_POWER();

    // 档位
    if(g_stSys.u8WorkGear == GEAR_LOW)     DIS_LOW();
    else if(g_stSys.u8WorkGear == GEAR_HI) DIS_HIGH();

    // 人感指示灯：正常启用时常亮，待机时以 1 Hz 闪烁
    if(g_stSys.bHumanSenseEn)
    {
        if(g_stSys.bHumanSenseStandby)
        {
            if(s_bBlinkPhase) DIS_HUMAN();
        }
        else
        {
            DIS_HUMAN();
        }
    }

    // 童锁
    if(g_stSys.bChildLockEn) DIS_LOCK();

    // 定时：将向上取整后的剩余小时数显示为 1H/2H/4H 位组合
    {
        U8 u8Bits = F_GetTimerHourBits();
        if(u8Bits & TMR_1H) DIS_TIME_1H();
        if(u8Bits & TMR_2H) DIS_TIME_2H();
        if(u8Bits & TMR_4H) DIS_TIME_4H();
    }
}

// =================================================================
// 任务控制
// =================================================================
void F_DispTskChange(unsigned char temp)
{
    U8 i;
    for(i = 0; i < DISP_TOTAL_TASK; i++)
    {
        if(DispTskTbl[i].Index == temp)
        {
            DispIndex = i;
            DispLastT = DispTskTbl[i].LastTime;
            return;
        }
    }
}

U8 F_GetDispType(void)
{
    return DispTskTbl[DispIndex].Index;
}

// 每 100 ms 将整机状态更新到 DispBuf
static void F_DisplayUpData(void)
{
    // 任务自动退出
    if(DispLastT > 0)
    {
        DispLastT--;
        if(DispLastT == 0)
        {
            if(F_GetDispType() == DIPS_POWERUP)
                F_DispTskChange(DIPS_NORMAL);
        }
    }

    // 1 Hz 闪烁相位计时（5 * 100 ms = 500 ms 半周期）
    if(++s_u8BlinkCnt >= 5)
    {
        s_u8BlinkCnt = 0;
        s_bBlinkPhase = !s_bBlinkPhase;
    }

    F_DispClean();
    (DispTskTbl[DispIndex].DispFunc)();
}

void F_mDisplayControl(void)
{
	if(F_GetTimeTick(C_TIME_4ms))
    {
		F_DisplayDrive_IRQ();
	}
    if(F_GetTimeTick(C_TIME_100ms))
    {
        if(g_u8TestState == C_SelfTest)
        {
            F_DispClean();
            F_SelfTestDislayUpData();
        }
        else
        {
            F_DisplayUpData();
        }
        F_SyncDispBuf();
    }
}

void F_mDispInital(void)
{
    F_AllSegOff();
    F_AllComOff();
    F_DispTskChange(DIPS_POWERUP);
}
