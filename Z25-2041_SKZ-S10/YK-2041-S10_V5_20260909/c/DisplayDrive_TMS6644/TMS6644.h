/*********************************************************
文 件 名 : TMS6644_Drive.h
文件说明 : TMS6644 LED驱动头文件
编写作者 : 
适配芯片 : SH79F9463A
版 本 号 : V1.1
==========================================================*/
#ifndef __TMS6644_DRIVE_H__
#define __TMS6644_DRIVE_H__

// 根据 SH79F9463A 的标准库定义包含对应的头文件
// #include "SH79F9463A.h" 


//---------------------------------------------------------
// IIC 引脚定义 (请根据您的实际电路原理图修改管脚)
//---------------------------------------------------------
/* 示例：假设 P1.0 是 SCL, P1.1 是 SDA */
// sbit TMS_SCL = P1^0;
// sbit TMS_SDA = P1^1;

/* 宏定义操作：请在此处替换为实际的 IO 操作命令 */
#define F_TMS_SCL_H()    {TMS_SCL = 1;}  // 时钟线拉高
#define F_TMS_SCL_L()    {TMS_SCL = 0;}  // 时钟线拉低
#define F_TMS_SDA_H()    {TMS_SDA = 1;}  // 数据线拉高
#define F_TMS_SDA_L()    {TMS_SDA = 0;}  // 数据线拉低

/* 如果需要读取 SDA (判断ACK), 请定义此宏 */
#define F_READ_SDA()     (TMS_SDA)       

//---------------------------------------------------------
// TMS6644 器件参数定义
//---------------------------------------------------------
#define TMS6644_ADDR_WR  0xA0   // 器件写地址 (Slave Address)

// 特殊功能寄存器地址
#define REG_PAGE_UNLOCK  0xFE   // 写锁寄存器 (写入 0xC5 解锁 FDh)
#define REG_PAGE_SELECT  0xFD   // 页码选择寄存器 (0~3)

//---------------------------------------------------------
// 函数声明
//---------------------------------------------------------

/* 底层 IIC 协议函数 */
void F_IIC_Start(void);
void F_IIC_Stop(void);
void F_IIC_WriteByte(uint8 r_Data);

/* TMS6644 应用层函数 */
void F_TMS6644_WriteCmd(uint8 r_RegAddr, uint8 r_Data);
void F_TMS6644_Unlock_Page(uint8 r_PageID);
void F_TMS6644_Init(void);
void F_TMS6644_Display_PWM(uint8 *l_pu8Buf, uint8 l_u8Number);

#endif