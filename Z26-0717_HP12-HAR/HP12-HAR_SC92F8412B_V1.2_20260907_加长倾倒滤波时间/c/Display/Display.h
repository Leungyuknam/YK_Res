#ifndef _DISPLAY_H_
#define _DISPLAY_H_

// =================================================================
// 4 SEG × 2 COM 矩阵，共驱动 8 个独立指示灯
// =================================================================
#define COM_MAX_CNT     2
#define SEG_MAX_CNT     4
#define COM_1           0
#define COM_2           1

// 缓冲区（应用层写 DispBuf，中断扫描读取 DriveDispBuf）
extern U8 xdata DispBuf[COM_MAX_CNT];
extern U8 xdata DriveDispBuf[COM_MAX_CNT];
extern bit B_DispUpdating;

// 每个 DispBuf[COM] 内的 SEG 位映射
#define BIT_SEG1        BIT0
#define BIT_SEG2        BIT1
#define BIT_SEG3        BIT2
#define BIT_SEG4        BIT3

// -----------------------------------------------------------------
// 逻辑指示灯到矩阵位置的映射
// COM1 行：开关/人体感应/低档/高档
// COM2 行：童锁/定时1H/定时2H/定时4H
// （请根据 PCB 丝印核对 SEGx 到指示灯的映射；如有差异，
//  仅需调整以下 8 个宏的顺序。）
// -----------------------------------------------------------------
#define DIS_POWER()     (DispBuf[COM_1] |= BIT_SEG4)
#define DIS_HUMAN()     (DispBuf[COM_1] |= BIT_SEG1)
#define DIS_LOW()       (DispBuf[COM_1] |= BIT_SEG3)
#define DIS_HIGH()      (DispBuf[COM_2] |= BIT_SEG3)
#define DIS_LOCK()      (DispBuf[COM_2] |= BIT_SEG4)
#define DIS_TIME_1H()   (DispBuf[COM_2] |= BIT_SEG1)
#define DIS_TIME_2H()   (DispBuf[COM_2] |= BIT_SEG2)
#define DIS_TIME_4H()   (DispBuf[COM_1] |= BIT_SEG2)

// =================================================================
// 显示任务表
// =================================================================
enum DISP_TASK_NUM
{
    DIPS_NORMAL = 0,        // 根据整机状态渲染指示灯
    DIPS_POWERUP,           // 上电后全亮 1 秒
    DISP_TOTAL_TASK,
};

typedef struct _TYPE_DISP_TASK
{
    unsigned char Index;
    unsigned int  LastTime;     // 自动退出倒计时，单位 100 ms
    unsigned char Blink;        // 当前未使用，为兼容保留
    void (*DispFunc)(void);
} TYPE_DISP_TASK;

extern U8 xdata DispLastT;

// -----------------------------------------------------------------
// 接口
// -----------------------------------------------------------------
void F_mDispInital(void);
void F_DispTskChange(unsigned char temp);
U8   F_GetDispType(void);
void F_mDisplayControl(void);       // 每 100 ms 将整机状态更新到 DispBuf
void F_DispClean(void);
void F_DisplayDrive_IRQ(void);      // 每 4 ms 扫描矩阵（由定时器 0 调用）
void F_SyncDispBuf(void);
void F_SelfTestDislayUpData(void);

#endif
