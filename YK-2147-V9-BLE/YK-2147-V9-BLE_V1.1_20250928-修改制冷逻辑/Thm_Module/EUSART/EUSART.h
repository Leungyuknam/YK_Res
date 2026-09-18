/*********************************************************
文 件 名 : EUSART.H
文件说明 : 串口通讯头文件
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/
#ifndef _EUSART_H_
#define _EUSART_H_    

//数据类型
#define		C_TYPE_BINGDING		0x00	//绑定
#define		C_TYPE_CHECK		0x01	//查询
#define		C_TYPE_SET  		0x02	//设置
#define		C_TYPE_RESET		0x04	//恢复出厂设置
#define		C_TYPE_LBOX_SET		0x05	//左箱温度
#define		C_TYPE_RBOX_SET		0x06	//右箱温度

//发送标志位g_u8SendFlag
#define		SEND_NULL_       	0	//无接收
#define		SEND_BINGDING_		1	//绑定
#define		SEND_CHECK_			2	//查询
#define		SEND_SET_  			3	//设置
#define		SEND_RESET_			4	//恢复出厂设置
#define		SEND_LBOX_SET_		5	//左箱温度
#define		SEND_RBOX_SET_		6	//右箱温度
#define		SEND_BLUE_CHECK_    7	//蓝牙检测

//FRAME
#define    C_FRAME_HEAD          0xFE
#define    C_FRAME_BINGDING      0x00
#define    C_FRAME_NACK_CHECK    0x01
#define    C_FRAME_NACK_SET      0x02
#define    C_FRAME_NACK_RESET    0x04
#define    C_FRAME_LBOX_TEMP     0x05
#define    C_FRAME_RBOX_TEMP     0x06


//外部函数声明
extern void F_UART_ReceiveTimeout(void);
extern void F_UART_SendDelay(void);
extern void F_RECEIVE_Control(void);
extern void F_SEND_Control(void);
extern u16 F_CHECKSUM_Calculate(u8 *pTable,u8 l_u8TableLength);
extern void F_APP_Request(void);


#endif