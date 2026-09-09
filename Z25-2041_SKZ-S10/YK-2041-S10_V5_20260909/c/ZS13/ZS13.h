#ifndef _ZS13_H_
#define _ZS13_H_

// --- ZS13 I2C地址 ---
// 7位地址0x38 -> 8位地址 Write:0x70 Read:0x71
#define c_ZS13_ADDR_W           0x70
#define c_ZS13_ADDR_R           0x71

// --- 状态机状态定义 ---
#define c_HUMI_STATE_INIT       0    // 初始状态 / 触发测量
#define c_HUMI_STATE_READ       1    // 读取数据并触发下一次

// --- 接收数据长度 ---
// Status + Humi(2.5) + Temp(2.5) + CRC
#define c_ZS13_LENGTH_MAX       7 

// --- 超时常量 ---
#define c_HUMI_LINK_ERR_TIME    3   // 通讯超时次数

// --- 上电初始化相关命令 (官方建议，修复上电初始化异常导致一直输出满量程) ---
#define c_ZS13_CMD_RESET        0xBA    // 软复位
#define c_ZS13_OTP_CCP          0x1C    // OTP 地址：CCP/CCN
#define c_ZS13_OTP_AFE          0x1B    // OTP 地址：AFE_CFG
#define c_ZS13_REG_CCP          0xBC    // 回写寄存器：CCP/CCN
#define c_ZS13_REG_AFE          0xBB    // 回写寄存器：AFE_CFG
#define c_ZS13_CMD_CALEN        0xBE    // 使能校准
#define c_ZS13_CAL_INIT_OK      0x18    // 状态字 bit4|bit3：校准参数已正确装载
#define c_ZS13_RAW_HUMI_MAX     0x000FFFFFUL
#define c_ZS13_MAX_RAW_ERR_TIME 5

// --- 对外接口 ---
extern void F_Zs13PowerOnPrepare(void);
extern void F_QueryHumiData(void);
extern void F_Zs13Init(void);   // 上电一次性初始化(自带状态字自检，仅在校准异常时重灌寄存器)

#endif