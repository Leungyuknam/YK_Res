/*********************************************************
文 件 名 : TMS6644_Drive.C
文件说明 : TMS6644 LED驱动 (模拟IIC)
编写作者 : 
适配芯片 : SH79F9463A (24MHz)
版 本 号 : V1.1 (修正中文编码)
==========================================================*/
#include "h\Allinclude.h"
#include "TMS6644.h"

// 简单的短延时，适配24M主频 (约1us-2us)
#define IIC_DELAY()  {_nop_(); _nop_(); _nop_(); _nop_();}

/*********************************************************************************
函数名称：F_IIC_Start
功能描述：发送IIC起始信号
*********************************************************************************/
void F_IIC_Start(void)
{
    F_TMS_SDA_H();
    F_TMS_SCL_H();
    IIC_DELAY();
    F_TMS_SDA_L(); // SCL高电平时，SDA由高变低
    IIC_DELAY();
    F_TMS_SCL_L(); // 钳住总线，准备发送数据
}

/*********************************************************************************
函数名称：F_IIC_Stop
功能描述：发送IIC停止信号
*********************************************************************************/
void F_IIC_Stop(void)
{
    F_TMS_SDA_L();
    F_TMS_SCL_H();
    IIC_DELAY();
    F_TMS_SDA_H(); // SCL高电平时，SDA由低变高
    IIC_DELAY();
}

/*********************************************************************************
函数名称：F_IIC_WriteByte
功能描述：发送一个字节数据
输入参数：r_Data 要发送的数据
*********************************************************************************/
void F_IIC_WriteByte(uint8 r_Data)
{
    uint8 i;
    
    // 发送8位数据 (高位在前 MSB First)
    for(i = 0; i < 8; i++)
    {
        if(r_Data & 0x80)
        {
            F_TMS_SDA_H();
        }
        else
        {
            F_TMS_SDA_L();
        }
        
        r_Data <<= 1;
        
        IIC_DELAY();
        F_TMS_SCL_H(); // 拉高时钟，从机采样
        IIC_DELAY();
        F_TMS_SCL_L(); // 拉低时钟，准备下一位
    }
    
    // 处理ACK位 (第9个时钟)
    F_TMS_SDA_H();     // 释放SDA线
    IIC_DELAY();
    F_TMS_SCL_H();     // 产生ACK时钟
    IIC_DELAY();
    // 如需严格判断ACK，可在此读取SDA电平
    F_TMS_SCL_L();     // 结束ACK位
}

/*********************************************************************************
函数名称：F_TMS6644_WriteCmd
功能描述：写单个寄存器
*********************************************************************************/
void F_TMS6644_WriteCmd(uint8 r_RegAddr, uint8 r_Data)
{
    F_IIC_Start();
    F_IIC_WriteByte(TMS6644_ADDR_WR); // 器件写地址，通常为 0xA0
    F_IIC_WriteByte(r_RegAddr);       
    F_IIC_WriteByte(r_Data);          
    F_IIC_Stop();
}

/*********************************************************************************
函数名称：F_TMS6644_Unlock_Page
功能描述：解锁并切换寄存器页
输入参数：r_PageID (0~3)
*********************************************************************************/
void F_TMS6644_Unlock_Page(uint8 r_PageID)
{
    // 1. 发送解锁命令：寄存器 0xFE 写入 0xC5
    F_TMS6644_WriteCmd(0xFE, 0xC5);
    
    // 2. 切换页码：寄存器 0xFD 写入目标 Page ID
    F_TMS6644_WriteCmd(0xFD, r_PageID);
}

/*********************************************************************************
函数名称：F_TMS6644_Init
功能描述：芯片初始化
*********************************************************************************/
void F_TMS6644_Init(void)
{
    uint8 i;
	TMS_EN = 1;
    // 1. 设置 Page 3：配置全局参数
    F_TMS6644_Unlock_Page(3); 
    
    // 1.1 设置全局电流 (GCC)，0xFF 为最大电流
    F_TMS6644_WriteCmd(0x01, 0xFF); 

    // 1.2 设置配置寄存器，使能芯片 (SSD=1 正常工作)
    F_TMS6644_WriteCmd(0x00, 0x01); 

    // 2. 设置 Page 0：打开 LED 开关
    F_TMS6644_Unlock_Page(0);
    
    F_IIC_Start();
    F_IIC_WriteByte(TMS6644_ADDR_WR);
    F_IIC_WriteByte(0x00); // 从第一个开关寄存器开始
    for(i = 0; i < 24; i++) // 覆盖 24 个控制位
    {
        F_IIC_WriteByte(0xFF); // 全部打开
    }
    F_IIC_Stop();

    // 3. 最后切回 Page 1 (PWM数据页)，方便后续主循环刷新亮度
    F_TMS6644_Unlock_Page(1);
	
	TMS_EN = 1;                   // 开启屏幕电源使能
}

/*********************************************************************************
函数名称：F_TMS6644_Display_PWM
功能描述：刷新全屏 PWM 亮度数据
输入参数：l_pu8Buf 显存指针, l_u8Number 长度
*********************************************************************************/
void F_TMS6644_Display_PWM(uint8 *l_pu8Buf, uint8 l_u8Number)
{
    uint8 i;
    
    // 确保当前处于 Page 1
    F_TMS6644_Unlock_Page(1);
    
    // 使用 I2C 地址自增模式连续写入
    F_IIC_Start();
    F_IIC_WriteByte(TMS6644_ADDR_WR); 
    F_IIC_WriteByte(0x00); // PWM 数据起始地址
    
    for(i = 0; i < l_u8Number; i++)
    {
        F_IIC_WriteByte(l_pu8Buf[i]); 
    }
    
    F_IIC_Stop();
}
