#include "h\Allinclude.h"

// =================================================================
// 软件 UART 传输模块，沿用自上一工程。
// 数据载荷已适配 HP12-HAR 的新状态；若 HP12-HAR
// 实际未使用该通信链路，可移除 main() 中的
// 对应调用。
// =================================================================

bit g_bSoftUartErr = 0;
U8  g_u8SoftUartErrCntDwn = c_softUartErrTime;

// 将当前整机状态编码到发送缓冲区
unsigned char MakeEnCode(void)
{
    unsigned char i, u8CheckSum = 0;
    U8 u8Flags = 0;

    if(g_stSys.bTurnOn)             u8Flags |= BIT0;
    if(g_stSys.bChildLockEn)        u8Flags |= BIT1;
    if(g_stSys.bHumanSenseEn)       u8Flags |= BIT2;
    if(g_stSys.bHumanSenseStandby)  u8Flags |= BIT3;
    if(g_stSys.bIsTipOver)          u8Flags |= BIT5;
    if(g_stSys.bIsHumanDetected)    u8Flags |= BIT6;

    g_u8UartBuf[0] = 0xAA;
    g_u8UartBuf[1] = (g_stSys.u8WorkGear & 0x0F) | (u8Flags & 0xF0);
    g_u8UartBuf[2] = u8Flags;
    g_u8UartBuf[3] = DispBuf[0];
    g_u8UartBuf[4] = DispBuf[1];
    g_u8UartBuf[5] = F_GetTimerHourBits();

    for(i = 1; i <= 5; i++) u8CheckSum += g_u8UartBuf[i];
    g_u8UartBuf[6] = u8CheckSum;

    return 7;
}

// 解码接收帧（HP12-HAR 中保留为空操作；整机不再
// 接收远端传感器板的控制命令）。
unsigned char DeCodeProc(unsigned char len)
{
    unsigned char u8CheckSum = 0;

    if(len != 4 || g_u8UartBuf[0] != 0x55) return C_DECODE_FAIL;

    u8CheckSum = g_u8UartBuf[1] + g_u8UartBuf[2];
    if(g_u8UartBuf[3] != u8CheckSum) return C_DECODE_FAIL;

    g_u8SoftUartErrCntDwn = c_softUartErrTime;
    return C_DECODE_PASS;
}

void f_SoftUartErrDetect()
{
    if(g_u8SoftUartErrCntDwn)
    {
        g_u8SoftUartErrCntDwn--;
        g_bSoftUartErr = 0;
    }
    else
    {
        g_bSoftUartErr = 1;
    }
}

void F_mTransferDataDeal()
{
    if(F_GetTimeTick(C_TIME_100ms))
    {
        f_SoftUartErrDetect();
    }
    if(F_GetTimeTick(C_TIME_10ms))
    {
        F_UartProtocol();
    }
}
