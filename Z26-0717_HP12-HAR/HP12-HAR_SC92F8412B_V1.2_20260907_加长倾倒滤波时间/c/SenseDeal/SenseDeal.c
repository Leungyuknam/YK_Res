#include "h\Allinclude.h"

// -----------------------------------------------------------------
// 倾倒输入扫描：10 ms 节拍，2000 ms 去抖
// -----------------------------------------------------------------
static void F_TipOverScan(void)
{
    static U8 s_u8FilterCnt = 0;
    U8 u8Cur = (P_TIP == TIP_ACTIVE) ? 1 : 0;

    if(u8Cur != g_stSys.bIsTipOver)
    {
        if(++s_u8FilterCnt >= 200)       // 200 * 10 ms = 2000 ms
        {
            s_u8FilterCnt = 0;
            g_stSys.bIsTipOver = u8Cur;
        }
    }
    else
    {
        s_u8FilterCnt = 0;
    }
}

// -----------------------------------------------------------------
// PIR 人体感应输入扫描：200 ms 去抖
// -----------------------------------------------------------------
static void F_HumanSenseScan(void)
{
    static U8 s_u8FilterCnt = 0;
    U8 u8Cur = (P_HUMAN == HUMAN_ACTIVE) ? 1 : 0;

    if(u8Cur != g_stSys.bIsHumanDetected)
    {
        if(++s_u8FilterCnt >= 20)
        {
            s_u8FilterCnt = 0;
            g_stSys.bIsHumanDetected = u8Cur;
        }
    }
    else
    {
        s_u8FilterCnt = 0;
    }
}

// -----------------------------------------------------------------
// 主循环入口
// -----------------------------------------------------------------
void F_mAllSenseDeal(void)
{
    if(F_GetTimeTick(C_TIME_10ms))
    {
        F_TipOverScan();
        F_HumanSenseScan();
    }
}
