#include <intrins.h>
//#include "SH79F9463A.h"
//#include "tk.h"

#include "h\SH79F9463A.H"
#include "c\TouchKey\tk.h"
#include "c\TouchKey\tk_check.h"

#ifndef TEST_TK_DATA
    #pragma message "关闭串口调试"
#else

/* 以下18个定义与芯片有关，这里仅限SH79F326A/328A/6481A使用 */
#define UART0_P00           (0)             /* Uart0 */
#define UART0_P01           (1)
#define UART0_P02           (2)
#define UART0_P03           (3)
#define UART0_P26           (4)
#define UART0_P27           (5)
#define UART0_P52           (6)
#define UART0_P53           (7)
#define UART1_P26           (8+0)           /* Uart1 */
#define UART1_P27           (8+1)
#define UART1_P40           (8+2)
#define UART1_P41           (8+3)
#define UART1_P42           (8+4)
#define UART1_P43           (8+5)
#define UART1_P50           (8+6)
#define UART1_P51           (8+7)
#define UART2_P20           (16+1)          /* Uart2 此引脚只能作为Rxd */
#define UART2_P21           (16+0)          /* Uart2 此引脚只能作为Txd */


#pragma message "打开串口调试,波特率115200,在这里选择串口引脚"
#define	UARTx_TXD_PIN		UART1_P40		/*!< 选择使用哪个引脚发送触摸数据到PC */
#define	UARTx_RXD_PIN		UART1_P43		/*!< 选择使用哪个引脚接收PC发送的指令 */

/* 波特率设置 */
#define UARTx_SYS_CLOCK     (24000000)
#define UARTx_BAUDRATE      (115200)
#define UARTx_BRT           ((unsigned int)(UARTx_SYS_CLOCK / UARTx_BAUDRATE / 16))
#define UARTx_FINE          ((UARTx_SYS_CLOCK / UARTx_BAUDRATE) - (UARTx_BRT * 16))





/* 以下为自适应定义，不用修改 */
#if		(UARTx_TXD_PIN < 8)
    #define     UARTx_RI                RI
    #define     UARTx_TI                TI
    #define     UARTx_REN               REN
    #define     UARTx_SBUF              SBUF
    #define     UARTx_SFINE             SFINE
    #define     UARTx_SBRTL             SBRTL
    #define     UARTx_SBRTH             SBRTH
    #define     UARTx_SCON              SCON
    #define     UARTx_PCON              PCON
    #define     UARTx_CR                UART0CR
    #define     UARTx_VECTOR            4
    #define     UARTx_INT_EN()          IEN0 |= (1<<4)
    #define     UARTx_BANK_SET()
    #define     UARTx_BANK_CLR()

#elif 	(UARTx_TXD_PIN < 16)
    #define     UARTx_RI                RI1
    #define     UARTx_TI                TI1
    #define     UARTx_REN               REN1
    #define     UARTx_SBUF              SBUF1
    #define     UARTx_SFINE             SFINE1
    #define     UARTx_SBRTL             SBRTL1
    #define     UARTx_SBRTH             SBRTH1
    #define     UARTx_SCON              SCON1
    #define     UARTx_PCON              PCON1
    #define     UARTx_CR                UART1CR
    #define     UARTx_VECTOR            15
    #define     UARTx_INT_EN()          IEN2 |= (1<<0)
    #define     UARTx_BANK_SET()        INSCON |= 0x40
    #define     UARTx_BANK_CLR()        INSCON &= 0xBF

#else
    #define     UARTx_RI                RI2
    #define     UARTx_TI                TI2
    #define     UARTx_REN               REN2
    #define     UARTx_SBUF              SBUF2
    #define     UARTx_SFINE             SFINE2
    #define     UARTx_SBRTL             SBRTL2
    #define     UARTx_SBRTH             SBRTH2
    #define     UARTx_SCON              SCON2
    #define     UARTx_PCON              PCON2
//  #define     UARTx_CR                UART2CR         // Uart2无LCM功能
    #define     UARTx_VECTOR            16
    #define     UARTx_INT_EN()          IEN2 |= (1<<1)
    #define     UARTx_BANK_SET()        INSCON |= 0x40
    #define     UARTx_BANK_CLR()        INSCON &= 0xBF
#endif



bit                     gbTKUartTran;
U8      xdata           gBTKUartSendFg;
U8      xdata           gBTKUartLen;
U8      xdata           gBTKUartTotalByte;
#if (TEST_TK_DATA_TYPE == TEST_ALL_DATA)
U8      xdata           gBTKUartBuf[(MAX_KEYS*2+1)*8+3]; //再增加2个假通道
#else
U8      xdata           gBTKUartBuf[(MAX_KEYS+1)*8+3]; //再增加2个假通道
#endif

/* 参数校准 */
#ifdef	ADJUST_TK_PARA
U16     xdata           gWTKDebugDiff1[MAX_KEYS*2];     // 存放初始的最大差值
U16     xdata           gWTKDebugDiff2[MAX_KEYS*2];     // 存放校验后的最大差值
U8      xdata           gBTKUartRxCmd;                  // 串口接收到的命令
U8      xdata           gBTKUartRxProcess;              // 串口接收进程处理
U8      xdata           gBTKAdjStep;                    // 串口接收到的确认命令个数（用于指示阶段）
U8      xdata           gBTKAdjStepFlag;                // 串口发送状态 - 标志（每个阶段是否已经发送对应的状态）
U8      xdata           gBTKUartTxStatus;               // 串口发送状态
U8      xdata           gBTKUartTxPara;                 // 串口发送参数
U8      xdata           gBTKUartTxParaCnt;              // 串口发送参数计数
U8      xdata           gBTKUartTxParaType;             // 串口发送参数类型
U8      xdata           gBTKAdjFreq;                    // 调整频率
#endif

//----------------------------------------------------------------------------
//				串口发送脚设置为输出高
//----------------------------------------------------------------------------
#define	OUTx_H(port, bitn)                  \
        {                                   \
            if (port > 4)                   \
            {                               \
                INSCON |= 0x40;             \
            }                               \
            P##port |= (1<<(bitn));         \
            P##port##CR |= (1<<(bitn));     \
            if (port > 4)                   \
            {                               \
                INSCON &= 0xBF;             \
            }                               \
        }
#define	Uartx_PinH()                                    \
{                                                       \
         if(UARTx_TXD_PIN == UART0_P00) {OUTx_H(0, 0);} \
    else if(UARTx_TXD_PIN == UART0_P01) {OUTx_H(0, 1);} \
    else if(UARTx_TXD_PIN == UART0_P02) {OUTx_H(0, 2);} \
    else if(UARTx_TXD_PIN == UART0_P03) {OUTx_H(0, 3);} \
    else if(UARTx_TXD_PIN == UART0_P26) {OUTx_H(2, 6);} \
    else if(UARTx_TXD_PIN == UART0_P27) {OUTx_H(2, 7);} \
    else if(UARTx_TXD_PIN == UART0_P52) {OUTx_H(5, 2);} \
    else if(UARTx_TXD_PIN == UART0_P53) {OUTx_H(5, 3);} \
    else if(UARTx_TXD_PIN == UART1_P26) {OUTx_H(2, 6);} \
    else if(UARTx_TXD_PIN == UART1_P27) {OUTx_H(2, 7);} \
    else if(UARTx_TXD_PIN == UART1_P40) {OUTx_H(4, 0);} \
    else if(UARTx_TXD_PIN == UART1_P41) {OUTx_H(4, 1);} \
    else if(UARTx_TXD_PIN == UART1_P42) {OUTx_H(4, 2);} \
    else if(UARTx_TXD_PIN == UART1_P43) {OUTx_H(4, 3);} \
    else if(UARTx_TXD_PIN == UART1_P50) {OUTx_H(5, 0);} \
    else if(UARTx_TXD_PIN == UART1_P51) {OUTx_H(5, 1);} \
    else if(UARTx_TXD_PIN == UART2_P21) {OUTx_H(2, 1);} \
}

//----------------------------------------------------------------------------
//				串口接收脚设置为输入上拉
//----------------------------------------------------------------------------
#define	INx_PULLUP(port, bitn)              \
        {                                   \
            if (port > 4)                   \
            {                               \
                INSCON |= 0x40;             \
            }                               \
            P##port |= (1<<(bitn));         \
            P##port##PCR |= (1<<(bitn));    \
             P##port##CR &= ~(1<<(bitn));   \
            if (port > 4)                   \
            {                               \
                INSCON &= 0xBF;             \
            }                               \
        }
#define	Uartx_PinInPull()                                   \
{                                                           \
         if(UARTx_RXD_PIN == UART0_P00) {INx_PULLUP(0, 0);} \
    else if(UARTx_RXD_PIN == UART0_P01) {INx_PULLUP(0, 1);} \
    else if(UARTx_RXD_PIN == UART0_P02) {INx_PULLUP(0, 2);} \
    else if(UARTx_RXD_PIN == UART0_P03) {INx_PULLUP(0, 3);} \
    else if(UARTx_RXD_PIN == UART0_P26) {INx_PULLUP(2, 6);} \
    else if(UARTx_RXD_PIN == UART0_P27) {INx_PULLUP(2, 7);} \
    else if(UARTx_RXD_PIN == UART0_P52) {INx_PULLUP(5, 2);} \
    else if(UARTx_RXD_PIN == UART0_P53) {INx_PULLUP(5, 3);} \
    else if(UARTx_RXD_PIN == UART1_P26) {INx_PULLUP(2, 6);} \
    else if(UARTx_RXD_PIN == UART1_P27) {INx_PULLUP(2, 7);} \
    else if(UARTx_RXD_PIN == UART1_P40) {INx_PULLUP(4, 0);} \
    else if(UARTx_RXD_PIN == UART1_P41) {INx_PULLUP(4, 1);} \
    else if(UARTx_RXD_PIN == UART1_P42) {INx_PULLUP(4, 2);} \
    else if(UARTx_RXD_PIN == UART1_P43) {INx_PULLUP(4, 3);} \
    else if(UARTx_RXD_PIN == UART1_P50) {INx_PULLUP(5, 0);} \
    else if(UARTx_RXD_PIN == UART1_P51) {INx_PULLUP(5, 1);} \
    else if(UARTx_RXD_PIN == UART2_P20) {INx_PULLUP(2, 1);} \
}


/**
 * @brief  触摸调试串口初始化
 *
 */
void tk_debug_uart_init(void)
{
    /* Txd引脚设置为输出高 */
    Uartx_PinH();

    /* 配置对应的串口Txd映射到指定引脚 */
#if	(UARTx_TXD_PIN < 16 )
    INSCON |= 0x40;
    UARTx_CR &= 0x0F;
    UARTx_CR |= ((UARTx_TXD_PIN&0x07) << 4);
    INSCON &= 0xBF;
#endif

#ifdef	ADJUST_TK_PARA
    /* Rxd引脚设置为输入上拉 */
    Uartx_PinInPull();

    /* 配置对应的串口Rxd映射到指定引脚 */
    #if	(UARTx_RXD_PIN < 16 )
        INSCON |= 0x40;
        UARTx_CR &= 0xF0;
        UARTx_CR |= (UARTx_RXD_PIN&0x07);
        INSCON &= 0xBF;
    #endif
#endif  /*ADJUST_TK_PARA */

    UARTx_BANK_SET();
    UARTx_PCON  &= ~0x40;               // SSTAT=0  SMx0-SMx1-SMx2有效
    UARTx_SCON  = 0x40;
    UARTx_SFINE = UARTx_FINE;
    UARTx_SBRTL = (32768 - UARTx_BRT) & 0xff;
    UARTx_SBRTH = (((32768 - UARTx_BRT) >> 8) & 0xff) | 0x80;
#ifdef	ADJUST_TK_PARA
    UARTx_REN   = 1;                    // 有参数校准，要接收
#else
    UARTx_REN   = 0;                    // 无参数校准，不接收
#endif
    UARTx_BANK_CLR();

    UARTx_INT_EN();                     // 使能中断
}


/**
 * @brief  触摸调试串口中断服务程序
 *
 */
void tk_debug_uart_handler(void)    interrupt   UARTx_VECTOR
{
#ifdef	ADJUST_TK_PARA
           unsigned char data  rxBuf;
    static unsigned char xdata rxQueue[5];
#endif

    //---------------------------------------------------------------------------
    _push_(INSCON);
    INSCON = 0x00;
    _push_(FLASHCON);
    FLASHCON = 0x00;
    //---------------------------------------------------------------------------

    UARTx_BANK_SET();

	/* 接收 */
#ifdef	ADJUST_TK_PARA
    if (UARTx_RI)
    {
        UARTx_RI= 0;
        rxBuf = UARTx_SBUF;

        rxQueue[0] = rxQueue[1];
        rxQueue[1] = rxQueue[2];
        rxQueue[2] = rxQueue[3];
        rxQueue[3] = rxQueue[4];
        rxQueue[4] = rxBuf;
        if(    (rxQueue[0] == 0x69)             // 第1字节0x69
            && (rxQueue[1] == 0x03)             // 第2字节0x03
            && (rxQueue[2] == 0x41)             // 第3字节0x41
            && ((rxQueue[0]+rxQueue[1]+rxQueue[2]+rxQueue[3]) == rxBuf) )  // 校验和
        {
            gBTKUartRxCmd = rxQueue[3];         // 命令
        }
    }
#else
//  if( UARTx_RI )
    {
        UARTx_RI= 0;
    }
#endif

    /* 发送 */
    if (UARTx_TI)
    {
        UARTx_TI = 0;
        if (gbTKUartTran)
        {
            if (gBTKUartLen < gBTKUartTotalByte)
            {
                UARTx_SBUF = gBTKUartBuf[gBTKUartLen];
                gBTKUartLen++;
            }
            else
            {
                gbTKUartTran = 0;
            }
        }
    }

    //---------------------------------------------------------------------------
    INSCON = 0x00;
    _pop_(FLASHCON);
    _pop_(INSCON);
    //---------------------------------------------------------------------------
}

/************************************************************************************************************
 * @brief  :    参数调整通讯过程与参数锁存
 *
 ************************************************************************************************************/
#ifdef  ADJUST_TK_PARA
// 校准的状态
unsigned char   const    Tab_TKDebugStatus[]=
{
    0x07,		// 0- 第1次校准完成，去第1次基准
    0x08,		// 1- 第1次基准完成，去第1次差值
    0x0C,		// 2- 第1次差值完成，去第2次校准
    0x0D,		// 3- 第2次校准完成，去第2次差值
    0x0B,		// 4- 失败：校准/基准/差值  任一项未通过都认为是失败
    0x09,		// 5- 成功：校准/基准/差值  全部通过才认为是成功
};

// 调整/保存放大倍数
void  _AdjTKDiv(U16 *diff, U16 *div)
{
    U8      i;
    U16     v16;
    U32     v32;

    for (i=0; i<TK_Para.num; i++)
    {
        v16 = *diff;
        if (v16 >= 65)
        {
            v32 = *div;             // 读出放大系数
            v32 *= TK_DIFF_TARGET;  // 放大系数调整
            v16 = v32 /v16;

            if (v16 >= 0x600)
            {
                v16 = 0x600;
            }
            *div = v16;             // 保存放大系数
        }
        div++;
        diff++;
    }
}

void AdjTKDiv(void)
{
    // 低频
    _AdjTKDiv(&gWTKDebugDiff1[0], &LF_DIV[0]);

    // 高频
    _AdjTKDiv(&gWTKDebugDiff1[MAX_KEYS], &HF_DIV[0]);
}
// 校验/调整/保存阈值
U8 _checkDiff(U16 *diff, U16 *max)
{
    U8      i;
    U8      retVal=1;               // 默认校验成功
    U16     v16;

    for (i=0; i<TK_Para.num; i++)
    {
        v16 = *diff;
        if ((v16 >= TK_DIFF_MIN) && (v16<=TK_DIFF_MAX))
        {
            *max = v16;             // 保存最大变化量
        }
        else
        {
            retVal = 0;             // 失败
        }
        max++;
        diff++;
    }
    return  retVal;
}
U8 checkTKDiff(void)
{
    U8 retVal=1;               // 默认校验成功

    // 低频
    if (_checkDiff(&gWTKDebugDiff2[0], &Tab_TKMax[0]) == 0)
    {
        retVal = 0;
    }

    // 高频
    if (_checkDiff(&gWTKDebugDiff2[MAX_KEYS], &Tab_TKMax[MAX_KEYS]) == 0)
    {
        retVal = 0;
    }

    return  retVal;
}


// 处理接收到的命令
void tk_debug_rxd_cmd(void)
{
    U8 i;

    if (gBTKUartRxCmd)
    {
        if (gBTKUartRxCmd == PC_ADJUST_TK)              /* 校准 */
        {
            reset_TK();                                 // 触摸复位
            stop_scan_tk();
            gBTKUartRxProcess = 1;                      // 进入串口接收处理阶段
            gBTKAdjStep = 0;                            // 确认计数初始化（阶段）
            gBTKAdjStepFlag = 0;                        // 各个阶段已经发送状态标志

            for (i=0; i<(sizeof(gWTKDebugDiff1)/sizeof(*gWTKDebugDiff1)); i++)
            {
                gWTKDebugDiff1[i] = 0;
                gWTKDebugDiff2[i] = 0;
            }
        }
        else if (gBTKUartRxCmd == PC_ACK_OK)           /* 确认 */
        {
            gBTKAdjStep ++;
            if (gBTKAdjStep == 2)						//>> 第二次校准
            {
                reset_TK();                             // 触摸复位
                stop_scan_tk();
                AdjTKDiv();                             // 放大倍数调整
            }
            else if (gBTKAdjStep == 4) 				 //>> 校准结束，发送参数
            {
                gBTKUartTxPara = 1;
                gBTKUartTxParaCnt = 0;
                gBTKUartTxParaType = 0;
            }
        }
        else if (gBTKUartRxCmd == PC_FREQ_SWITCH)      /* 频率切换 */
        {

        }

        gBTKUartRxCmd = 0;                              // 清除接收到的命令
    }
}

// 状态处理
extern U8	xdata HF_init_Done;				 	//初始化完成
extern U8	xdata LF_init_Done;
void tk_debug_status(void)
{
    if (gBTKUartRxProcess)                             // 串口调试中
    {
        if (   ((gBTKAdjStep == 1)&&((gBTKAdjStepFlag&0x02) == 0))     // 在调试阶段1，且当前阶段状态未发送
            || ((gBTKAdjStep == 3)&&((gBTKAdjStepFlag&0x08) == 0)))    // 在调试阶段3，且当前阶段状态未发送
        {
            gBTKAdjStepFlag |= (0x01<<gBTKAdjStep);
            gBTKUartTxStatus = Tab_TKDebugStatus[gBTKAdjStep];
        }
        else if (  ((gBTKAdjStep == 0)&&((gBTKAdjStepFlag&0x01) == 0))  // 在调试阶段0（校准中），且当前阶段状态未发送
                || ((gBTKAdjStep == 2)&&((gBTKAdjStepFlag&0x04) == 0))) // 在调试阶段2（校准中），且当前阶段状态未发送
        {
            if ((HF_init_Done) && (LF_init_Done))
            {
                gBTKAdjStepFlag |= (0x01<<gBTKAdjStep);
                gBTKUartTxStatus = Tab_TKDebugStatus[gBTKAdjStep];
            }
        }
        else if ((gBTKAdjStep == 4)&&((gBTKAdjStepFlag&0x10) == 0))    // 在调试阶段4，且当前阶段状态未发送
        {
            gBTKAdjStepFlag |= 0x10;
            if (checkTKDiff())                                     // 检查调整后的阈值是否合理
            {
                gBTKUartTxStatus = Tab_TKDebugStatus[5];            // 默认成功
            }
            else
            {
                gBTKUartTxStatus = Tab_TKDebugStatus[4];            // 失败
            }
        }
    }
}

// 保存差值
void tk_debug_save_diff(void)
{
    U8 i, k;
    U16 diff_val;
    U16 xdata *diff_buf;

    if (gBTKAdjStep == 1)
    {
        diff_buf = gWTKDebugDiff1;
    }
    else if (gBTKAdjStep == 3)
    {
        diff_buf = gWTKDebugDiff2;
    }
    else
    {
        return;
    }

    for (i=0,k=MAX_KEYS; i<TK_Para.num; i++,k++)
    {
        diff_val = (key_pre[i] > key_baseline[i]) ? (key_pre[i] - key_baseline[i]) : 0;
        if (diff_buf[i] < diff_val)
        {
            diff_buf[i] = diff_val;
        }

        diff_val = (key_pre[k] > key_baseline[k]) ? (key_pre[k] - key_baseline[k]) : 0;
        if (diff_buf[k] < diff_val)
        {
            diff_buf[k] = diff_val;
        }
    }
}

#endif  /* ADJUST_TK_PARA */


/**
 * @brief  触摸调试串口发送数据打包
 *
 */
void tk_debug_uart_txd(void)
{
    unsigned char i,j,k,x,len;
    unsigned int sample,base;

    #ifdef  ADJUST_TK_PARA			// 校准
		tk_debug_rxd_cmd();         // 命令接收处理
        tk_debug_status();          // 状态处理
        tk_debug_save_diff();       // 保存差值
	#endif

#if TEST_TK_DATA_TYPE == TEST_HIGH_DATA
    // 串口发送高频数据
    if (freqForDataMsg != MAX_KEYS) //MAX_KEYS表示高频
    {
        return;
    }
#elif TEST_TK_DATA_TYPE == TEST_LOW_DATA
    // 串口发送低频数据
    if (freqForDataMsg != 0)        //MAX_KEYS表示高频
    {
        return;
    }
#else
    // 串口发送双频数据
    if (freqForDataMsg != 0)
    {
        return;
    }
#endif

    if (gbTKUartTran == 0)
    {
    #ifdef  ADJUST_TK_PARA
        x = 0;
        if (gBTKUartRxProcess)
        {
            if (gBTKUartTxPara)                         /* 1. 发送参数 */
            {
                gBTKUartBuf[0] = 0x69;                  // 引导码
                gBTKUartBuf[1] = 0x05;                  // 长度
                if (gBTKUartTxParaType == 0)            // 高频放大倍数
                {
                    gBTKUartBuf[2] = 0x32;
                    gBTKUartBuf[3] = 0x00;
                    gBTKUartBuf[4] = HF_DIV[gBTKUartTxParaCnt]>>8;
                    gBTKUartBuf[5] = HF_DIV[gBTKUartTxParaCnt];
                }
                else if ( gBTKUartTxParaType == 2 )     // 低频放大倍数
                {
                    gBTKUartBuf[2] = 0x32;
                    gBTKUartBuf[3] = 0x00;
                    gBTKUartBuf[4] = LF_DIV[gBTKUartTxParaCnt]>>8;
                    gBTKUartBuf[5] = LF_DIV[gBTKUartTxParaCnt];
                }
                else if (gBTKUartTxParaType == 1)       // 高频阈值
                {
                    gBTKUartBuf[2] = 0x22;
                    gBTKUartBuf[3] = gBTKUartTxParaCnt;
                    gBTKUartBuf[4] = (gWTKDebugDiff2[gBTKUartTxParaCnt+MAX_KEYS]>>8)&0xff;
                    gBTKUartBuf[5] = (gWTKDebugDiff2[gBTKUartTxParaCnt+MAX_KEYS])&0xff;
                }
                else if (gBTKUartTxParaType == 3)       // 低频阈值
                {
                    gBTKUartBuf[2] = 0x22;
                    gBTKUartBuf[3] = gBTKUartTxParaCnt;
                    gBTKUartBuf[4] = (gWTKDebugDiff2[gBTKUartTxParaCnt]>>8)&0xff;
                    gBTKUartBuf[5] = (gWTKDebugDiff2[gBTKUartTxParaCnt])&0xff;
                }

                if (++gBTKUartTxParaCnt >= TK_Para.num)
                {
                    gBTKUartTxParaCnt = 0;
                    if (++gBTKUartTxParaType > 3)
                    {
                        gBTKUartTxPara = 0;
                    }
                }

                x = gBTKUartBuf[1] + 1;                 // 待发送的长度（不包含校验码）
            }
            else if (gBTKUartTxStatus)                  /* 2. 发送状态 */
            {
                gBTKUartBuf[0] = 0x69;                  // 引导码
                gBTKUartBuf[1] = 0x04;                  // 长度
                gBTKUartBuf[2] = 0x51;                  // 不知道是什么
                gBTKUartBuf[3] = gBTKUartTxStatus;      // 状态
                gBTKUartBuf[4] = 0x00;                  // tk error number
                x = gBTKUartBuf[1] + 1;                 // 待发送的长度（不包含校验码）
                gBTKUartTxStatus = 0;

                if (gBTKAdjStep == 4)                   // 全部参数和状态都发送完成了，退出接收模式
                {
                    gBTKUartRxProcess = 0;
                }
            }
        }
        else
    #endif
        {
            x = 0;
            gBTKUartBuf[x++] = 0x69;
        #if ((TEST_TK_DATA_TYPE == TEST_HIGH_DATA) || (TEST_TK_DATA_TYPE == TEST_LOW_DATA))
            len = TK_Para.num;
        #else
            len = TK_Para.num*2;
        #endif
            len ++;                                 // 增加一个假通道，发送键值
            gBTKUartBuf[x++] = (len<<3)+1;
            for (k=0; k<len; k++)
            {
            #if ((TEST_TK_DATA_TYPE == TEST_HIGH_DATA) || (TEST_TK_DATA_TYPE == TEST_LOW_DATA))
                if (k == TK_Para.num)
                {
                    sample =key_value;              // 当前发的这一笔数据属于哪一个频率
                    base =  freqForKeyJudge;        // 当前用哪个频率确认按键
                }
                else
                {
                    i = k+freqForDataMsg;
                    sample = key_pre[i];            // 触摸原始数据
                    base   = key_baseline[i];       // 触摸基准值
                    if (base == 0)
                    {
                        sample = 0;
                    }
                }
            #else
                if (k == (TK_Para.num*2))
                {
                    sample = key_value;             // 当前发的这一笔数据属于哪一个频率
                    base = freqForKeyJudge;         // 当前用哪个频率确认按键
                }
                else
                {
                    i = (k < TK_Para.num) ? k : (k + MAX_KEYS - TK_Para.num);
                    sample = key_pre[i];            // 触摸原始数据
                    base   = key_baseline[i]  ;     // 触摸基准值
                    if (base == 0)
                    {
                        sample = 0;
                    }
                }
            #endif

                gBTKUartBuf[x++] = 0x02;
                gBTKUartBuf[x++] = k;
                gBTKUartBuf[x++] = (base>>8)&0xff;
                gBTKUartBuf[x++] = (base)&0xff;

                gBTKUartBuf[x++] = 0x12;
                gBTKUartBuf[x++] = k;
                gBTKUartBuf[x++] = (sample>>8)&0xff;
                gBTKUartBuf[x++] = (sample)&0xff;
            }
        }

        for (i=0,j=0; i<x; i++)
        {
            j += gBTKUartBuf[i];
        }
        gBTKUartBuf[x++] = j;
        gBTKUartLen = 0;
        gBTKUartTotalByte = x;
        gbTKUartTran = 1;

        // 启动发送
        UARTx_BANK_SET();
        UARTx_TI = 1;
        UARTx_BANK_CLR();
    }
}

#endif
