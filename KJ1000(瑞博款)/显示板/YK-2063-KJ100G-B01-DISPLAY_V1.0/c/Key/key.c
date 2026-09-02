/*--------------------------------------------------------------------
模块名称：    按键扫描分发模块
模块作者：    CJB
功能描述：    通过配置表格实现按键扫描，处理函数分发
修改记录：    2016-5-17 1:增加按键码前端滤波，增加抗干扰能力
                        2:直接更改为函数指针调用处理函数
---------------------------------------------------------------------*/
#include "h\Allinclude.h"

// ====================================================================
// 新增：发送至主机的 8 位全局变量
// ====================================================================
unsigned char g_u8TouchDataToSend = 0; // 请在你的从机串口发送组包时读取这个变量

// ====================================================================
// 宏定义 (根据你的最新通道注释)
// ====================================================================
#define BIT_KEY_POWER   0x01  // BIT0: 电源键 TK11
#define BIT_KEY_LOCK    0x02  // BIT1: 童锁键 TK2
#define BIT_KEY_MODE    0x04  // BIT2: 模式键 TK0
#define BIT_KEY_ION     0x08  // BIT3: 负离子键 TK1
#define BIT_KEY_TIME    0x10  // BIT4: 定时键 TK13
#define BIT_KEY_WIFI    0x20  // BIT5: WIFI键 TK12

#define KEY_TK0		0X0001
#define KEY_TK1		0X0002
#define KEY_TK2		0X0004
#define KEY_TK3		0X0008
#define KEY_TK4		0X0010
#define KEY_TK5		0X0020
#define KEY_TK6		0X0040
#define KEY_TK7		0X0080
#define KEY_TK8		0X0100
#define KEY_TK9		0X0200
#define KEY_TK10	0X0400
#define KEY_TK11	0X0800
#define KEY_TK12	0X1000
#define KEY_TK13	0X2000
#define KEY_TK14	0X4000
#define KEY_TK15	0X8000
#define KEY_TK16	0X010000
#define KEY_TK17	0X020000
#define KEY_TK18	0X040000
#define KEY_TK19	0X080000
#define KEY_TK20	0X100000
#define KEY_TK21	0X200000
#define KEY_TK22	0X400000


U32 exKeyValueFlag = 0;     // 当前轮按键标志 (底层库返回的原始32位数据)
unsigned char temp_test=0;

/**************************************************
*函数名称：void  F_UpdateTouchDataToSend(void)
*函数功能：将底层的 32 位物理通道映射为 8 位通信字节
**************************************************/
void F_UpdateTouchDataToSend(void)
{
    unsigned char l_u8TouchTemp = 0;

    // 注意：这里全部使用独立的 if，不使用 else if
    // 这样当多个按键同时按下时，相应的 BIT 位都能被同时置为 1 (完美支持主机端的组合键)
    if (exKeyValueFlag & KEY_TK11) l_u8TouchTemp |= BIT_KEY_POWER;
    if (exKeyValueFlag & KEY_TK2)  l_u8TouchTemp |= BIT_KEY_LOCK;
    if (exKeyValueFlag & KEY_TK0)  l_u8TouchTemp |= BIT_KEY_MODE;
    if (exKeyValueFlag & KEY_TK1)  l_u8TouchTemp |= BIT_KEY_ION;
    if (exKeyValueFlag & KEY_TK13) l_u8TouchTemp |= BIT_KEY_TIME;
    if (exKeyValueFlag & KEY_TK12) l_u8TouchTemp |= BIT_KEY_WIFI;

    // 更新全局变量，供串口模块打包发送
    g_u8TouchDataToSend = l_u8TouchTemp;
}

/**************************************************
*函数名称：void  Sys_Scan(void) 
*函数功能：扫描TK并映射数据
**************************************************/
void Sys_Scan(void)
{                   
    if(SOCAPI_TouchKeyStatus & 0x80)    // 重要步骤2: 触摸键扫描一轮标志
    {                                                                   
        SOCAPI_TouchKeyStatus &= 0x7f;  // 重要步骤3: 清除标志位                                                    
        exKeyValueFlag = TouchKeyScan();// 按键数据处理函数    
        
        if(exKeyValueFlag > 0){
            temp_test++;
        }
        TouchKeyRestart();              // 启动下一轮转换      
        
        // --- 核心改动：底层扫描完一轮后，立即更新通信数据 ---
        F_UpdateTouchDataToSend();
    }               
}


void F_mKeyControl(void)
{
    if(F_GetTimeTick(C_TIME_10ms))
    {
		Sys_Scan();
    }
}

////////////////////////////////////////////////