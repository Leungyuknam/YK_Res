
#ifndef _UART_APP_H_
#define _UART_APP_H_

//帧数据
#define 	C_FRAME_HEAD          	  0xFE	    

//CMD
#define 	C_CMD_BIND         		  0x00
#define 	C_CMD_CHECK               0x01
#define 	C_CMD_SET                 0x02
#define 	C_CMD_RESET               0x04
#define 	C_CMD_SET_TP_L            0x05
#define 	C_CMD_SET_TP_R            0x06

extern u8 xdata g_u8AppSn;

//必用函数
extern void F_UpLoad_Bind(void);
extern void F_UartApp_Handle(void);

#endif