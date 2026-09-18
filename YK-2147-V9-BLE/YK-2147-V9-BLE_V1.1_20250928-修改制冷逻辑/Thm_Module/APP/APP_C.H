/*********************************************************
文 件 名 : APP_C.H
文件说明 : wifi串口通讯头文件
编写作者 : thm
修订作者 : 
编写时间 : 2021-04-19
版 本 号 : V1.0
==========================================================*/
#ifndef _APP_C_H_
#define _APP_C_H_ 

/*************************需配置部分***********************/
#define		C_APP_RECEIVE_MAX_NUM	    5       //接收最大字节数（含校验字节）
#define		C_RECEIVE_TIMEOUT	        4        //接收超时时间（单位：*10mS）

//UART寄存器配置
//#define		REGISTER_UART			SBUF		    
//#define		_TEST_UART_TI		    (SCON & 0X02)		//发送中断判断
//#define		_CLR_UART_TI		    SCON &= (~0X02)		//清发送中断
//#define		_TEST_UART_RI		    (SCON & 0X01)	    //接收中断判断
//#define		_CLR_UART_RI		    SCON &= (~0X01)		//清接收中断

#define		REGISTER_UART			US1CON3		    
#define		_TEST_UART_TI		    (US1CON0 & 0X02)		//发送中断判断
#define		_CLR_UART_TI		    US1CON0 &= (~0X02)		//清发送中断
#define		_TEST_UART_RI		    (US1CON0 & 0X01)	    //接收中断判断
#define		_CLR_UART_RI		    US1CON0 &= (~0X01)		//清接收中断

/*************************配置结束*************************/
//UART状态
#define		C_REC_ENABLE	    	0     
#define		C_REC_DISABLE	    	1
 
#define		C_SEND_DONE 	    	0     
#define		C_SEND_ING  	    	1
#define		C_SEND_DELAY 	    	2    


#endif
