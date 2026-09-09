#include "h\Allinclude.h"
#include "ZS13.h"
//#include "KeyFunction.h" // 引用全局变量

// --- 变量定义 ---
U8 code t_Zs13Cmd[3] = {0xAC, 0x33, 0x00};
U8 xdata Zs13ReadBuf[c_ZS13_LENGTH_MAX];
static bit s_bZs13PowerOnInited = 0;    // 上电一次性初始化标志(0=尚未执行)
static U8 s_u8Zs13MaxRawCnt = 0;

// --- IO口定义 (请根据实际原理图修改) ---
// 假设使用P_HUMI_SDA和P_HUMI_SCL
#define SDASet_Input()      {P1CR &= ~BIT0;}
#define SDASet_Output()     {P1CR |= BIT0;}
#define SCLSet_Input()      {P2CR &= ~BIT2;}
#define SCLSet_Output()     {P2CR |= BIT2;}

#define GetSDApin()         (P_HUMI_SDA)

#define SDASet_L()      { SDASet_Output(); P_HUMI_SDA = 0; }
#define SDASet_H()      { SDASet_Input(); }

#define SCLSet_L()      { SCLSet_Output(); P_HUMI_SCL = 0; }
#define SCLSet_H()      { SCLSet_Input(); }
#define I2C_ACK_ERR         1
#define I2C_ACK_OK          0

/* ================== 基础I2C时序函数 ================== */

static void F_Delay(U8 l_u8Cnt) 
{
    do {
        NOP();NOP();NOP();NOP();NOP();
        NOP();NOP();NOP();NOP();NOP();
		NOP();NOP();NOP();NOP();NOP();
		NOP();NOP();NOP();NOP();NOP();
    } while(--l_u8Cnt);
    WDT_clear(); 
}

static void F_StartBit(void)
{
    SCLSet_H(); SDASet_H(); F_Delay(7);
    SDASet_L(); F_Delay(7);
    SCLSet_L(); F_Delay(7);
}

static void F_StopBit(void)
{
    SDASet_L(); F_Delay(7);
    SCLSet_H(); F_Delay(7);
    SDASet_H(); F_Delay(7);
    SCLSet_Input(); SDASet_Input();
}

static void F_mAck(void)
{
    SDASet_L(); F_Delay(7);
    SCLSet_H(); F_Delay(7);
    SCLSet_L(); F_Delay(7);
    SDASet_H(); F_Delay(7);
}

static void F_mNack(void)
{
    SDASet_H(); F_Delay(7);
    SCLSet_H(); F_Delay(7);
    SCLSet_L(); F_Delay(7);
    SDASet_L(); F_Delay(7);
}

static void F_CAck(void)
{
    U8 l_u8Cnt = 20;
    SDASet_H(); SDASet_Input(); F_Delay(7);
    SCLSet_H(); F_Delay(7);
    while(l_u8Cnt)
    {
        l_u8Cnt--;
        if(!GetSDApin()) break;
    }
    SCLSet_L(); F_Delay(7);
//    SDASet_Output();
}

static void F_Write8Bit(U8 ch)
{
    U8 uctemp;
    for (uctemp = 0; uctemp < 8; uctemp++ )
    {
        if (ch & 0x80) { SDASet_H(); }
        else           { SDASet_L(); }
        ch = ch << 1;
        F_Delay(7);
        SCLSet_H(); F_Delay(7);
        SCLSet_L(); F_Delay(7);
    }
}

static void F_Read8Bit(U8 *p)
{
    U8 uctemp;
    SDASet_Input();
    for (uctemp = 0; uctemp < 8; uctemp++)
    {
        *p = *p << 1;            
        SCLSet_H(); F_Delay(7);
        if(GetSDApin() == 1) { *p = *p | 0x01; }
        SCLSet_L(); F_Delay(7);
    }
//    SDASet_Output();
}

/* ================== ZS13 协议函数 ================== */

static U8 Calc_CRC8(U8 *message, U8 Num)
{
    U8 i, byte, crc = 0xFF;
    for(byte = 0; byte < Num; byte++)
    {
        crc ^= (message[byte]);
        for(i = 8; i > 0; --i)
        {
            if(crc & 0x80) crc = (crc << 1) ^ 0x31;
            else crc = (crc << 1);
        }
    }
    return crc;
}

/* ================== 上电初始化 (官方流程) ================== */
/* 说明：传感器存在上电初始化异常的缺陷，异常时校准参数未正确装载，
 *       会一直输出满量程。官方做法是上电后先读状态字自检，若校准位
 *       (bit4|bit3)未置位，则软复位并把 OTP 中的 CCP/CCN、AFE_CFG 重新
 *       装载到工作寄存器，再使能校准。本流程独立于下方的测量状态机，
 *       仅在上电后执行一次。                                            */

// 约 1.25ms 延时(实测原 8 次循环约 2.5ms，改为 4 次后从宽保留余量)
static void F_Zs13DelayMs(U8 l_u8Ms)
{
    U8 l_u8i;
    while(l_u8Ms--)
    {
        for(l_u8i = 0; l_u8i < 4; l_u8i++) F_Delay(255);
    }
}

// 读状态字(发读地址 -> 读1字节 -> 主机NACK -> 停止)
static U8 F_Zs13ReadStatus(void)
{
    U8 l_u8Status = 0;
    F_StartBit();
    F_Write8Bit(c_ZS13_ADDR_R); F_CAck();
    F_Read8Bit(&l_u8Status);
    F_mNack();
    F_StopBit();
    return l_u8Status;
}

// 读 OTP：先写 OTP 地址命令，再回读 2 字节(首字节为状态字，丢弃)
static void F_Zs13ReadOtp(U8 l_u8Addr, U8 *p_u8B1, U8 *p_u8B2)
{
    U8 l_u8Dummy = 0;
    F_StartBit();
    F_Write8Bit(c_ZS13_ADDR_W); F_CAck();
    F_Write8Bit(l_u8Addr);      F_CAck();
    F_StopBit();
    F_Zs13DelayMs(1);
    F_StartBit();
    F_Write8Bit(c_ZS13_ADDR_R); F_CAck();
    F_Read8Bit(&l_u8Dummy); F_mAck();   // 状态字，丢弃
    F_Read8Bit(p_u8B1);     F_mAck();
    F_Read8Bit(p_u8B2);     F_mNack();
    F_StopBit();
}

// 写寄存器：命令 + 2 字节
static void F_Zs13WriteReg(U8 l_u8Cmd, U8 l_u8B1, U8 l_u8B2)
{
    F_StartBit();
    F_Write8Bit(c_ZS13_ADDR_W); F_CAck();
    F_Write8Bit(l_u8Cmd);       F_CAck();
    F_Write8Bit(l_u8B1);        F_CAck();
    F_Write8Bit(l_u8B2);        F_CAck();
    F_StopBit();
}

// 上电一次性初始化：状态字自检，仅在校准异常时重灌寄存器
void F_Zs13Init(void)
{
    U8 l_u8B1, l_u8B2;

//    // 状态字 &0x18 == 0x18 表示校准已正确装载，无需初始化
//    if((F_Zs13ReadStatus() & c_ZS13_CAL_INIT_OK) == c_ZS13_CAL_INIT_OK)
//        return;
	F_Zs13ReadStatus();
	F_Zs13DelayMs(1);
    // 1. 软复位 0xBA
    F_StartBit();
    F_Write8Bit(c_ZS13_ADDR_W);     F_CAck();
    F_Write8Bit(c_ZS13_CMD_RESET);  F_CAck();
    F_StopBit();
    F_Zs13DelayMs(20);              // 复位后至少等待 20ms

    // 2. 读 OTP 的 CCP/CCN(0x1C)，回写工作寄存器(0xBC)
    F_Zs13ReadOtp(c_ZS13_OTP_CCP, &l_u8B1, &l_u8B2);
    F_Zs13DelayMs(1);
    F_Zs13WriteReg(c_ZS13_REG_CCP, l_u8B1, l_u8B2);
    F_Zs13DelayMs(1);

    // 3. 读 OTP 的 AFE_CFG(0x1B)，回写工作寄存器(0xBB)
    F_Zs13ReadOtp(c_ZS13_OTP_AFE, &l_u8B1, &l_u8B2);
    F_Zs13DelayMs(1);
    F_Zs13WriteReg(c_ZS13_REG_AFE, l_u8B1, l_u8B2);
    F_Zs13DelayMs(1);

    // 4. 使能校准 0xBE 0x08 0x00
    F_Zs13WriteReg(c_ZS13_CMD_CALEN, 0x08, 0x00);
    F_Zs13DelayMs(10);
}

// 触发一次湿温度测量
static void F_Zs13StartMeasure(void)
{
    F_StartBit();
    F_Write8Bit(c_ZS13_ADDR_W); F_CAck();
    F_Write8Bit(t_Zs13Cmd[0]);  F_CAck();
    F_Write8Bit(t_Zs13Cmd[1]);  F_CAck();
    F_Write8Bit(t_Zs13Cmd[2]);  F_CAck();
    F_StopBit();
}

void F_Zs13PowerOnPrepare(void)
{
    if(!s_bZs13PowerOnInited)
    {
        F_Zs13Init();
        F_Zs13StartMeasure();
        g_stSys.u8HumiState = c_HUMI_STATE_READ;
        s_bZs13PowerOnInited = 1;
    }
}

// --- 驱动主逻辑 ---
void F_QueryHumiData(void)
{   
    U32 xdata u32RawHumi = 0;
    U32 xdata u32RawTemp = 0;
    
    // 1. 喂狗与超时检测
    if(g_stSys.u8HumiLinkTime > 0) 
        g_stSys.u8HumiLinkTime--;
    else 
        g_stSys.bHumiErr = 1;

    // 2. 上电一次性初始化兜底：正常由500ms时基提前完成
    if(!s_bZs13PowerOnInited)
    {
        F_Zs13PowerOnPrepare();
        return;
    }


    // 3. 状态机
    if(g_stSys.u8HumiState == c_HUMI_STATE_INIT)
    {
        // 第一次运行时，触发测量
        F_Zs13StartMeasure();
        g_stSys.u8HumiState = c_HUMI_STATE_READ; // 跳转到读取状态
    }
    else 
    {
        // --- 步骤A: 读取数据 ---
        F_SetStr(Zs13ReadBuf, 0, c_ZS13_LENGTH_MAX);
        
        F_StartBit();
        F_Write8Bit(c_ZS13_ADDR_R); F_CAck();
        
        // 读取前6字节 (ACK)
        for (u32RawTemp = 0; u32RawTemp < (c_ZS13_LENGTH_MAX - 1); u32RawTemp++)
        {
            F_Read8Bit(&Zs13ReadBuf[u32RawTemp]);
            F_mAck();
        }
        // 读取最后1字节 (NACK)
        F_Read8Bit(&Zs13ReadBuf[6]);
        F_mNack();
        F_StopBit();

        // --- 步骤B: 校验与解析 ---
        if(Zs13ReadBuf[6] == Calc_CRC8(Zs13ReadBuf, 6))
        {
            // Bit7必须为0 (Idle)
            if((Zs13ReadBuf[0] & 0x80) == 0) 
            {
                g_stSys.u8HumiLinkTime = c_HUMI_LINK_ERR_TIME; // 重置超时
                
                // 拼接
                u32RawHumi = ((U32)Zs13ReadBuf[1] << 12) | ((U32)Zs13ReadBuf[2] << 4) | ((Zs13ReadBuf[3] & 0xF0) >> 4);
                u32RawTemp = ((U32)(Zs13ReadBuf[3] & 0x0F) << 16) | ((U32)Zs13ReadBuf[4] << 8) | (U32)Zs13ReadBuf[5];
				
                // 湿度满量程是传感器校准异常的典型输出，需连续确认后再报错
                if(u32RawHumi >= c_ZS13_RAW_HUMI_MAX)
                {
                    // 未达到次数前只累计，不更新湿度，也不主动清除已有错误
                    if(s_u8Zs13MaxRawCnt < c_ZS13_MAX_RAW_ERR_TIME) 
						s_u8Zs13MaxRawCnt++;
                    if(s_u8Zs13MaxRawCnt >= c_ZS13_MAX_RAW_ERR_TIME) 
						g_stSys.bHumiErr = 1;
                }
                else
                {
                    // 读到一帧有效且非满量程数据，认为传感器恢复正常
                    s_u8Zs13MaxRawCnt = 0;
                    g_stSys.bHumiErr = 0;
                    
                    // 存入全局 (保留x10精度)
                    g_stSys.u16HumiRaw10 = (U16)((u32RawHumi * 1000) >> 20);
                    g_stSys.bHumiDataValid = 1;
                    g_stSys.s16TempRaw10 = (S16)((u32RawTemp * 2000) >> 20) - 500;
                }
            }
            else
            {
                s_u8Zs13MaxRawCnt = 0;
            }
        }
        else
        {
            s_u8Zs13MaxRawCnt = 0;
        }
        
        // --- 步骤C: 立即再次触发下一次测量 ---
        F_Zs13StartMeasure();
        
        // 状态维持在 READ
    }
}