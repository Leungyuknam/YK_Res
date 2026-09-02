#ifndef VOICE_H
#define VOICE_H

extern bit g_bVoiceOff;
extern bit g_bVoiceEn;

//=============================================================================
// 业务数据变量声明 (供你的其他业务逻辑读写)
//=============================================================================

extern unsigned char g_u8TouchData;       // 接收到的触摸数据
extern unsigned char g_u8SensorData;      // 接收到的传感器数据

//=============================================================================
// 语音任务枚举
//=============================================================================
enum VOICE_TASK_NUM
{
    VOICE_CMD_POWER = 0x02,
    VOICE_CMD_FAN_WORD,
    VOICE_CMD_FAN_NUM,
    VOICE_CMD_ION,
    VOICE_CMD_MODE,
    VOICE_CMD_TIME_ON,
    VOICE_CMD_TIME_OFF,
    VOICE_CMD_TIME_CANCEL,
    VOICE_CMD_ONOFF = 0x0A,
    VOICE_CMD_WAKE = 0x42,
    VOICE_CMD_ALL,
};

//=============================================================================
// 发送帧字节顺序 (MCU_A -> MCU_B, 共23字节)
//=============================================================================
#define         BYTE_HEAD_0           0
#define         BYTE_HEAD_1           1
#define         BYTE_ID               2
#define         BYTE_CMD              3
#define         BYTE_DATA0            4
#define         BYTE_DATA1            5
#define         BYTE_CHECKSUM         6  // 语音指令自身的校验和
#define         BYTE_TAIL             7  // 语音指令的帧尾 (0xFB)
#define         BYTE_TX_DISP_START    8  // 显示数据起始位 (占12字节, 8~20)
#define         BYTE_TX_BRIGHTNESS    20 // 亮度数据位置 (第21字节)
#define         BYTE_TX_RGB_INDEX     21 // RGB映射位置 (第22字节)
#define         BYTE_TX_MCU_CHECKSUM  22 // 主控通信的自定义校验和 (第23字节)

//=============================================================================
// 接收帧字节顺序 (MCU_B -> MCU_A, 共11字节)
//=============================================================================
#define         BYTE_RX_TOUCH         8  // 触摸数据位置
#define         BYTE_RX_SENSOR        9  // 传感器数据位置
#define         BYTE_RX_MCU_CHECKSUM  10 // 主控通信的自定义校验和位置

//=============================================================================
// 基础数据定义
//=============================================================================
#define         HEARDER_0             0xA5
#define         HEARDER_1             0xFA
#define         ID                    0x00
#define         CMD_REC               0x81
#define         CMD_SEND              0x03
#define         MSG_TAIL              0xFB // 帧尾

extern bit g_bUartErr;

//=============================================================================
// 外部函数声明
//=============================================================================
extern void F_VoiceCommMonitor(void);
extern void F_VoiceRecProc();
extern void F_VoiceChange(unsigned int Command);
extern void F_VoiceFliter(void);
extern unsigned char F_Send_MCU_Idle_Cmd(void); 

#endif