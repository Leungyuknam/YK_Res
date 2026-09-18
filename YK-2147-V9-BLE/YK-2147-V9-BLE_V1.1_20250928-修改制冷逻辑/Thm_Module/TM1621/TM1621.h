/*********************************************************
文 件 名 : TM1621.H
文件说明 : TM1621驱动头文件
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-6
版 本 号 : V1.0
==========================================================*/
#ifndef _TM1621_H_
#define _TM1621_H_ 

//命令
#define     C_TM1621_SYS_DIS           2048              //100 00000 0000，关闭LCD偏压发生器和系统振荡器   
#define     C_TM1621_SYS_EN            2050              //100 00000 0010，打开系统振荡器 
#define     C_TM1621_LCD_OFF           2052              //100 00000 0100，关闭LCD偏压发生器               
#define     C_TM1621_LCD_ON            2054              //100 00000 0110，打开LCD偏压发生器                             
#define     C_TM1621_TIMER_DIS         2056              //100 00000 1000，时基输出失效
#define     C_TM1621_WDT_DIS           2058              //100 00000 1010，看门狗溢出标志输出失效
#define     C_TM1621_TIMER_EN          2060              //100 00000 1100，时基输出生效，
#define     C_TM1621_WDT_EN            2062              //100 00000 1110，看门狗溢出标志输出生效
#define     C_TM1621_TONE_OFF          2064              //100 00001 0000，关闭声音
#define     C_TM1621_TONE_ON           2066              //100 00001 0010，打开声音
#define     C_TM1621_CLR_TIMER         2072              //100 00001 1000，时基发生器清零
#define     C_TM1621_CLR_WDT           2076              //100 00001 1100，清WDT
#define     C_TM1621_XTAL_32K          2088              //100 00010 1000，系统时钟源晶振
#define     C_TM1621_RC_256K           2096              //100 00011 0000，系统时钟源片内RC振荡器
#define     C_TM1621_EXT_256K          2104              //100 00011 1000，系统时钟源外部时钟源
#define     C_TM1621_BIAS_1_2_COM2     2112              //100 00100 0000，LCD 1/2偏压选项:2个公共口
#define     C_TM1621_BIAS_1_2_COM3     2120              //100 00100 1000，LCD 1/2偏压选项:3个公共口
#define     C_TM1621_BIAS_1_2_COM4     2128              //100 00101 0000，LCD 1/2偏压选项:4个公共口
#define     C_TM1621_BIAS_1_3_COM2     2114              //100 00100 0010，LCD 1/3偏压选项:2个公共口
#define     C_TM1621_BIAS_1_3_COM3     2122              //100 00100 1010，LCD 1/3偏压选项:3个公共口
#define     C_TM1621_BIAS_1_3_COM4     2130              //100 00101 0010，LCD 1/3偏压选项:4个公共口
#define     C_TM1621_TONE_4K           2176              //100 01000 0000，声音频率4KHz
#define     C_TM1621_TONE_2K           2240              //100 01100 0000，声音频率2KHz
#define     C_TM1621_IRQ_DIS           2304              //100 10000 0000，使/IRQ 输出失效
#define     C_TM1621_IRQ_EN            2320              //100 10001 0000，使/IRQ 输出有效
#define     C_TM1621_OUT_F1            2368              //100 10100 0000，时基/WDT 时钟输出1Hz
#define     C_TM1621_OUT_F2            2370              //100 10100 0010，时基/WDT 时钟输出2Hz
#define     C_TM1621_OUT_F4            2372              //100 10100 0100，时基/WDT 时钟输出4Hz
#define     C_TM1621_OUT_F8            2374              //100 10100 0110，时基/WDT 时钟输出8Hz
#define     C_TM1621_OUT_F16           2376              //100 10100 1000，时基/WDT 时钟输出16Hz
#define     C_TM1621_OUT_F32           2378              //100 10100 1010，时基/WDT 时钟输出32Hz
#define     C_TM1621_OUT_F64           2380              //100 10100 1100，时基/WDT 时钟输出64Hz
#define     C_TM1621_OUT_F128          2382              //100 10100 1110，时基/WDT 时钟输出128Hz
#define     C_TM1621_TOPT              2496              //100 11100 0000，测试模式
#define     C_TM1621_NORMAL            2502              //100 11100 0110，普通模式

//命令头
#define     C_CMD_WRITE                5
#define     C_CMD_READ                 6

//IO口操作
#define     _IO_TM1621_DATA_PP         Set_P30_pp
#define     _IO_TM1621_DATA_PU         Set_P30_pu
#define     _TM1621_DATA_SET           Set_P30
#define     _TM1621_DATA_CLR           Clr_P30
#define     _TEST_TM1621_DATA          Test_P30

#define     _IO_TM1621_CS_PP           Set_P33_pp
#define     _TM1621_CS_SET             Set_P33
#define     _TM1621_CS_CLR             Clr_P33

#define     _IO_TM1621_WR_PP           Set_P31_pp
#define     _TM1621_WR_SET             Set_P31
#define     _TM1621_WR_CLR             Clr_P31

#define     _IO_TM1621_RD_PP           Set_P32_pp
#define     _TM1621_RD_SET             Set_P32
#define     _TM1621_RD_CLR             Clr_P32

//函数声明
extern void F_TM1621_Init(void);
extern u8 F_TM1621_Read(u8 l_u8RamAddr);    //传入单个RAM地址，得到该地址的数据
extern void F_TM1621_Reflesh_RAM(u8 *l_pu8Table);   //传入一个32个元素表格的起始地址，把表格的参数全部写入RAM
extern void F_TM1621_Write_DATA(u8 l_u8RamAddr, u8 l_u8RamData);  //把数据写入到相应的RAM
extern void F_TM1621_Write_CMD(u16 l_u16Cmd);  //写命令操作，针对TM1621芯片本身

#endif