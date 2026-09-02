#define UART_DUPLEX_C

#include "h\Allinclude.h"

// =========================================================
// 语音透传专属变量
// =========================================================
unsigned int  idata g_u16VoiceRecTimeOut = 0;
unsigned char idata g_u8VoiceTxdBusyTim = 0;
unsigned char idata g_u8VoiceTxdNum = TXD1_VOICE_BUF_LENGTH;
unsigned char idata g_u8VoiceSendLength = 0;
unsigned char idata g_u8VoiceRecNum = RXD1_VOICE_BUF_LENGTH;

unsigned char xdata Uart1RXDBuf[RXD1_VOICE_BUF_LENGTH] = {0};
unsigned char xdata Uart1TXDBuf[TXD1_VOICE_BUF_LENGTH] = {0};

// --- 对外提供的接收缓存和标志 ---
unsigned char g_u8PendingVoiceBuf[8] = {0}; 
bit g_bHasNewVoiceData = 0;                 

// --- 接收外部发送请求的标志 ---
bit g_bVoiceTxReady = 0; 

// =========================================================
// 接收逻辑区
// =========================================================
void F_VoiceRXDInit(void)
{
    while(g_u8VoiceRecNum)
    {
        g_u8VoiceRecNum --;
        Uart1RXDBuf[g_u8VoiceRecNum] = 0;
    }
    g_u8VoiceRecNum = 0; 
    g_u16VoiceRecTimeOut = 60000;
    EN_RXD1_VOICE_INT();
}

void F_VoiceUartRxdDecode(void)
{
    unsigned char i, checkSum = 0;
    
    // 计算前6字节的校验和
    for(i = 0; i < 6; i++) checkSum += Uart1RXDBuf[i];
    
    // 检查语音模块的帧头(0xA5 0xFA)、校验和、帧尾(0xFB)
    if( (0xA5 == Uart1RXDBuf[0]) 
     && (0xFA == Uart1RXDBuf[1]) 
     && (checkSum == Uart1RXDBuf[6])
     && (0xFB == Uart1RXDBuf[7]) )
    {
        // 校验通过，暂存进转发缓冲区，等待 Communication.c 打包
        for(i = 0; i < 8; i++)
        {
            g_u8PendingVoiceBuf[i] = Uart1RXDBuf[i];
        }
        g_bHasNewVoiceData = 1; 
    }
}

void F_Voice_IRQ_REC(void)
{
    U8 temp;
    temp = REG1_READD_VOICE;
    if(g_u16VoiceRecTimeOut)
    {
        if(g_u8VoiceRecNum < RXD1_VOICE_BUF_LENGTH)
        {
            g_u16VoiceRecTimeOut = UART1_VOICE_DECODE_DELAY_TIME;
            Uart1RXDBuf[g_u8VoiceRecNum] = temp;
            g_u8VoiceRecNum ++;
        }
    }
}

void F_VoiceUartRxdProc(void)
{
    if(F_GetTimeTick(C_TIME_10ms))
    {
        if(g_u16VoiceRecTimeOut)
        {
            g_u16VoiceRecTimeOut --;
            if(g_u16VoiceRecTimeOut == 0)
            {
                if(g_u8VoiceRecNum == 8) // 语音包固定8字节
                {
                    F_VoiceUartRxdDecode();
                }
                F_VoiceRXDInit();
            }
        }
    }
}

// =========================================================
// 发送逻辑区 (彻底修复了变量名不统一的问题)
// =========================================================

// 外部调用的触发接口：只搬运数据，置位请求
void F_SendToVoiceModule(unsigned char *pData)
{
    unsigned char i;
	
	// 【新增逻辑锁】：如果上一包还没发完，或者正准备发，直接丢弃本次的新指令，保护发送区不被撕裂
    if(g_bVoiceTxReady || (g_u8VoiceTxdNum < g_u8VoiceSendLength))
    {
        return; 
    }
	
    for(i = 0; i < 8; i++)
    {
        Uart1TXDBuf[i] = pData[i];
    }
    g_bVoiceTxReady = 1; 
}

// 底层触发硬件发送的函数
void F_VoiceInitUartSend(void)
{
    g_u8VoiceTxdNum = 1;           // 使用统一的 Voice 变量名
    EN_TXD1_VOICE_INT();           // 开启发送中断
    REG1_SENDD_VOICE = Uart1TXDBuf[0];
    Uart1TXDBuf[0] = 0;
}

// 发送状态机轮询
void F_VoiceUartTxdProc(void)
{
    if(g_u8VoiceTxdBusyTim == 0)
    {
        if(g_bVoiceTxReady)
        {
            g_bVoiceTxReady = 0;           
            g_u8VoiceSendLength = 8;       // 发送8字节
            g_u8VoiceTxdBusyTim = UART1_VOICE_SEND_DELAY_TIME; 
            
            F_VoiceInitUartSend();         
        }
    }
    else
    {
        if(F_GetTimeTick(C_TIME_10ms)) 
        {
            g_u8VoiceTxdBusyTim--;
        }
    }
}

void F_Voice_IRQ_TXD(void)
{
    if(g_u8VoiceTxdNum < g_u8VoiceSendLength)
    {
        REG1_SENDD_VOICE = Uart1TXDBuf[g_u8VoiceTxdNum];
        Uart1TXDBuf[g_u8VoiceTxdNum] = 0;
        g_u8VoiceTxdNum ++;
    }
    else
    {
        DIS_TXD1_VOICE_INT();
    }
}

// =========================================================
// 主调度与初始化
// =========================================================
void F_Uart1DuplexProc(void)
{
    F_VoiceUartRxdProc(); 
    F_VoiceUartTxdProc(); // 按顺序执行
}

void F_Uart_Init(unsigned int Freq, unsigned long int baud)  
{       
    OTCON |= 0xC0;                           //串行接口USCI1选择Uart1通信
    SSCON0 = 0x10;                           //设置通信方式为模式一，允许接收
    SSCON1 = Freq * 1000000 / baud;          //波特率低位控制
    SSCON2 = (Freq * 1000000 / baud) >> 8;   //波特率高位控制
    IE1 |= 0x01;                             //开启SSI1中断
}

void UART1_IRQHandler(void) interrupt 7
{
    // 已修复中断清零掩码的安全问题
    if(SSCON0 & 0x02)
    {   
        SSCON0 &= ~0x02;
        F_Voice_IRQ_TXD();
    }   

    if(SSCON0 & 0x01) 
    {   
        SSCON0 &= ~0x01;
        F_Voice_IRQ_REC();
    }
}

void F_SysUart1Init(void)
{
    while(g_u8VoiceTxdNum)
    {
        g_u8VoiceTxdNum --;
        Uart1TXDBuf[g_u8VoiceTxdNum] = 0;
    }
    F_VoiceRXDInit();
    F_Uart_Init(XTAL_FREQUENCY, 9600);
}