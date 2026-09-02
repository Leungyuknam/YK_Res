#define Communication_C 

#include "h\Allinclude.h"

// ==========================================
// 全局变量区
// ==========================================
// --- 显示相关 ---
unsigned char g_u8DisplayTimeOut = 0; // 接收超时看门狗


// --- 外部接口声明 ---
extern void F_SendToVoiceModule(unsigned char *pData); // 发送给语音模块的串口接口

// 校验和函数（和主机保持一致）
unsigned char F_Calc_MCU_CheckSum(unsigned char *buf, unsigned char len)
{
    unsigned char i, sum = 0;
    for(i = 0; i < len; i++) sum += buf[i];
    return sum;
}




/**************************************************
*函数名称：void DeCodeProc(U8 length)
*函数功能：从机解析主机发来的 23 字节数据，并做语音转发
**************************************************/
void DeCodeProc(U8 length)
{
    unsigned char i;
    
    // 检查长度是否为主机发来的 23 字节
    if(length == 23)
    {
        // 校验整帧数据 (前22个字节的和 == 第23个字节)
        if(F_Calc_MCU_CheckSum(Uart0RXDBuf, 22) == Uart0RXDBuf[22])
        {
            // --- 1. 语音透传拦截 ---
            // 检查前两个字节是否为语音协议帧头 0xA5, 0xFA
            if((Uart0RXDBuf[0] == 0xA5) && (Uart0RXDBuf[1] == 0xFA))
            {
                // 将包含语音指令的前 8 字节，通过独立的语音串口发给语音模块
                F_SendToVoiceModule(&Uart0RXDBuf[0]);
            }

            // --- 2. 提取显示数据 ---
            for(i = 0; i < 12; i++)
            {
                ChipDispBuf[i] = Uart0RXDBuf[8 + i]; // BYTE_TX_DISP_START = 8
            }
            
            // --- 3. 提取亮度和 RGB 颜色索引 ---
            g_u8DisBright = Uart0RXDBuf[20];     // BYTE_TX_BRIGHTNESS = 20
            g_u8RgbColorIndex = Uart0RXDBuf[21]; // BYTE_TX_RGB_INDEX = 21
            
            // --- 4. 喂狗：收到正常数据，重置 300ms 超时看门狗 ---
            g_u8DisplayTimeOut = 30; 
        }
    }
}

/**************************************************
*函数名称：U8 MakeEnCode(void)
*函数功能：从机打包 11 字节数据发送给主机
**************************************************/
U8 MakeEnCode(void)
{
    unsigned char i;

    // --- 1. 填充前 8 个字节 (语音数据段) ---
    if(g_bHasNewVoiceData)
    {
        // 如果有暂存的语音回复数据，填入发送缓冲区
        for(i = 0; i < 8; i++) 
        {
            Uart0TXDBuf[i] = g_u8PendingVoiceBuf[i];
        }
        g_bHasNewVoiceData = 0; // 取出后清除标志位
    }
    else
    {
        // 如果没有语音数据，前 8 个字节全部填 0
        for(i = 0; i < 8; i++) 
        {
            Uart0TXDBuf[i] = 0x00;
        }
    }

    // --- 2. 填充第 9 个字节 (触摸数据) ---
    Uart0TXDBuf[8] = g_u8TouchDataToSend;

    // --- 3. 填充第 10 个字节 (传感器数据) ---
    Uart0TXDBuf[9] = g_u8SensorData; // 如果目前没用传感器，可赋0或你的实际变量名

    // --- 4. 填充第 11 个字节 (整帧校验和) ---
    Uart0TXDBuf[10] = F_Calc_MCU_CheckSum(Uart0TXDBuf, 10);

    // 返回要发送的总长度：11 字节
    return 11; 
}