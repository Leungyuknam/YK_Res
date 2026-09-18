/*********************************************************
文 件 名 : CompressorErroTest.H
文件说明 : 
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/
#ifndef _COMPRESSOR_ERRO_TEST_H_
#define _COMPRESSOR_ERRO_TEST_H_ 

/*************************需配置部分***********************/
//检测口
#define     _TEST_IO_ERRO			 (P26)	

/*************************配置结束*************************/

//报错定义
#define     C_ERRO_NULL              0
#define     C_ERRO_F1                1
#define     C_ERRO_F2                2
#define     C_ERRO_F3                3
#define     C_ERRO_F4                4
#define     C_ERRO_F5                5
#define     C_ERRO_F6                6
#define     C_ERRO_F7                7
#define     C_ERRO_F8                8

#define     C_ERRO_OL                0
#define     C_ERRO_OR                1
#define     C_ERRO_RL                0x03

//外部声明
extern u8   xdata  g_u8SysErroStatus;
extern u8   xdata  g_u8NtcErroStatus;

extern void F_CompError_Test(void);

#endif