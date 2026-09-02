#include "h\Allinclude.h"

unsigned int g_u16RecTimeOut = 0;
unsigned char g_u8TxdBusyTim = 0; // 100ms 心跳定时器
unsigned char g_u8TxdNum = TXD1_BUF_LENGTH;
unsigned char g_u8SendLength = 0;
unsigned char g_u8RecNum = RXD1_BUF_LENGTH;

unsigned char xdata Uart1RXDBuf[RXD1_BUF_LENGTH] = {0};
unsigned char xdata Uart1TXDBuf[TXD1_BUF_LENGTH]= {0};

void F_RXDInit(void)
{
    while(g_u8RecNum)
    {
        g_u8RecNum --;
        Uart1RXDBuf[g_u8RecNum] = 0;
    }
    g_u8RecNum = 0; 
    g_u16RecTimeOut = 60000;
    EN_RXD_INT();
}

void F_RxdDecode(U8 length)
{
    // 将解析任务交给 Voice 模块
    F_VoiceRecProc();
}

void F_IRQ_REC(void)
{
    U8 temp;
    temp = REG1_READD;
    if(g_u16RecTimeOut)
    {
        if(g_u8RecNum < RXD1_BUF_LENGTH)
        {
            g_u16RecTimeOut = UART1_DECODE_DELAY_TIME;
            Uart1RXDBuf[g_u8RecNum] = temp;
            g_u8RecNum ++;
        }
    }
}

void F_UartRxdProc(void)
{
    if(F_GetTimeTick(C_TIME_10ms))
    {
        if(g_u16RecTimeOut)
        {
            g_u16RecTimeOut --;
            if(g_u16RecTimeOut == 0)
            {
                if(g_u8RecNum)
                {
                    F_RxdDecode(g_u8RecNum);
                }
                F_RXDInit();
            }
        }
    }
}

void F_InitUartSend(void)
{
    g_u8TxdNum = 1;
    EN_TXD_INT();
    REG1_SENDD = Uart1TXDBuf[0];
    Uart1TXDBuf[0] = 0;
}

void F_IRQ_TXD(void)
{
    if(g_u8TxdNum < g_u8SendLength)
    {
        REG1_SENDD = Uart1TXDBuf[g_u8TxdNum];
        Uart1TXDBuf[g_u8TxdNum] = 0;
        g_u8TxdNum ++;
        // 注意：这里不再重置 g_u8TxdBusyTim，让其作为独立的 100ms 心跳计时
    }
    else
    {
        DIS_TXD_INT();
    }
}

// ==========================================
// 发送调度逻辑：基于 100ms 心跳
// ==========================================
void F_UartTxdProc(void)
{
    // 1. 每 10ms 滴答一次，递减心跳倒计时
    if(F_GetTimeTick(C_TIME_10ms))
    {
        if(g_u8TxdBusyTim > 0)
        {
            g_u8TxdBusyTim--;
        }
    }

    // 2. 如果倒计时到达 0 (过了100ms) 并且 上一帧已经完全发送完毕
    if((g_u8TxdBusyTim == 0) && (g_u8TxdNum >= g_u8SendLength))
    {
        // 重新装载 100ms 定时器
        g_u8TxdBusyTim = UART1_HEARTBEAT_TIME;
        
        // 尝试从队列中取出语音指令任务
        g_u8SendLength = CallArrayFuncB();
        
        // 如果队列为空 (没有语音指令)，则主动打包没有语音指令的空闲帧
        if(g_u8SendLength == 0)
        {
            g_u8SendLength = F_Send_MCU_Idle_Cmd();
        }
        
        // 触发串口硬件发送
        if(g_u8SendLength)
        {
            if(g_u8SendLength > TXD1_BUF_LENGTH)
            {   
                g_u8SendLength = TXD1_BUF_LENGTH;
            }
            F_InitUartSend();
        }
    }
}

void F_Uart1DuplexProc(void)
{
    F_UartRxdProc();
    F_UartTxdProc();
	if(F_GetTimeTick(C_TIME_100ms)) 
    {
        F_VoiceCommMonitor(); 
    }
}

void F_Uart_Init(unsigned int Freq, unsigned long int baud)  
{       
    OTCON |= 0xC0;                           //串行接口USCI1选择Uart1通信
    US1CON0 = 0x50;                          //设置通信方式为模式一，允许接收
    US1CON1 = Freq * 1000000 / baud;         //波特率低位控制
    US1CON2 = (Freq * 1000000 / baud) >> 8;  //波特率高位控制
    IE2 |= 0x01;                             //开启SSI1中断
}

void UART1_IRQHandler(void) interrupt 15
{
    if(US1CON0&0x02)
    {   
        US1CON0 &= 0xFE;    
        F_IRQ_TXD();
    }   

    if(US1CON0&0x01) 
    {   
        US1CON0 &= 0xFD;
        F_IRQ_REC();
    }
}

void F_SysUart1Init(void)
{
    while(g_u8TxdNum)
    {
        g_u8TxdNum --;
        Uart1TXDBuf[g_u8TxdNum] = 0;
    }
    F_RXDInit();
    F_Uart_Init(XTAL_FREQUENCY,9600);
}