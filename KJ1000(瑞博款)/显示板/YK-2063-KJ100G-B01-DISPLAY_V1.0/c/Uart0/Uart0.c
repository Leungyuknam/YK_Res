#define UART0_C

#include "h\Allinclude.h"
 
unsigned int idata g_u16RecTimeOut = 0;
unsigned char idata g_u8TxdBusyTim = 0;
unsigned char idata g_u8TxdNum = TXD0_BUF_LENGTH;
unsigned char idata g_u8SendLength = 0;
unsigned char idata g_u8RecNum = RXD0_BUF_LENGTH;

unsigned char xdata Uart0RXDBuf[RXD0_BUF_LENGTH] = {0};
unsigned char xdata Uart0TXDBuf[TXD0_BUF_LENGTH]= {0};

void F_RXDInit(void)
{
    while(g_u8RecNum)
    {
        g_u8RecNum --;
        Uart0RXDBuf[g_u8RecNum] = 0;
    }
    g_u8RecNum = 0; 
    g_u16RecTimeOut = 60000;
    EN_RXD_INT();
}

void F_RxdDecode(U8 length)
{
    // 调用从机的业务解析函数，解析主机发来的 23 个字节
    DeCodeProc(length);
}

void F_IRQ_REC(void)
{
    U8 temp;
    temp = REG0_READD;
    if(g_u16RecTimeOut)
    {
        if(g_u8RecNum < RXD0_BUF_LENGTH)
        {
            g_u16RecTimeOut = UART0_DECODE_DELAY_TIME;
            Uart0RXDBuf[g_u8RecNum] = temp;
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
    REG0_SENDD = Uart0TXDBuf[0];
    Uart0TXDBuf[0] = 0;
}

void F_IRQ_TXD(void)
{
    if(g_u8TxdNum < g_u8SendLength)
    {
        REG0_SENDD = Uart0TXDBuf[g_u8TxdNum];
        Uart0TXDBuf[g_u8TxdNum] = 0;
        g_u8TxdNum ++;
        g_u8TxdBusyTim = UART0_SEND_DELAY_TIME;
    }
    else
    {
        DIS_TXD_INT();
    }
}

void F_UartTxdProc(void)
{
    if(g_u8TxdBusyTim == 0)
    {
        // 调用从机的业务打包函数，将 11 个字节填入 Uart0TXDBuf
        g_u8SendLength = MakeEnCode();
        if(g_u8SendLength)
        {
            g_u8TxdBusyTim = UART0_SEND_DELAY_TIME;
            F_InitUartSend();
        }
    }
    else
    {
        if(F_GetTimeTick(C_TIME_10ms)) g_u8TxdBusyTim--;
    }
}

void F_UartDuplexProc(void)
{
    // F_CheckCommucate(); // 如果不需要通讯检测，可以注释掉
    F_UartRxdProc();
    F_UartTxdProc();
}

/*****************************************************
*函数名称：void Uart0_Init(uint Freq,unsigned long int baud)
*函数功能：Uart0中断初始化
*入口参数：Freq-主频，baud-波特率
*出口参数：void
*****************************************************/
void Uart0_Init(uint Freq,unsigned long int baud)    //选择Timer1作为波特率信号发生器
{
    P1CON &= 0xF3;   //TX/RX设置为输入带上拉
    P1PH  |= 0x0C;
    
    SCON  |= 0X50;   //设置通信方式为模式一，允许接收
    TMCON |= 0X02;
    TMOD  |= 0X20;
    PCON  |= 0X80;  //SMOD=1
    TH1 = (Freq*1000000/baud)>>8;     //波特率为T1的溢出时间；
    TL1 = Freq*1000000/baud;
    TR1 = 0;
    ET1 = 0;
    EUART = 1;     //开启Uart0中断
    EA = 1;
}

/*****************************************************
*函数名称：void UartInt(void) interrupt 4
*函数功能：Uart0中断函数
*入口参数：void
*出口参数：void
*****************************************************/
void UartInt(void) interrupt 4
{
    if(TI)
    {
        TI = 0; 
        F_IRQ_TXD();        
    }
    if(RI)
    {
        RI = 0; 
        F_IRQ_REC(); 
    }   
}

void F_SYSUartInit(void)
{
    while(g_u8TxdNum)
    {
        g_u8TxdNum --;
        Uart0TXDBuf[g_u8TxdNum] = 0;
    }
    F_RXDInit();
    Uart0_Init(24,9600);
}