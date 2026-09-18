//************************************************************
//  Copyright (c) YINGKE
//	文件名称	: init.c
//	作者		: thm
//	模块功能	: 初始化C文件
//  最后更正日期: 2020-12-5
// 	版本		: V1.0 
//  说明        ：  
//*************************************************************
//头文件
#include "COMMON\AllInclude.h"

static void F_GPIO_ParallelPortSet(void)
{
	EA=0;
	OPINX=0x86;
	OPREG&=(~0x08);
	EA=1;
}

void Uart0_Init(uint Freq,unsigned long int baud)
{
	P2CON &= 0xFC;    //TX/RX设置为输入带上拉
	P2PH  |= 0x03;
	
	OTCON |= 0x30;    //串行接口SSI0选择Uart0通信
	US0CON0 = 0x50;   //设置通信方式为模式一，允许接收
	US0CON1 = Freq*1000000/baud;   //波特率低位控制
	US0CON2 = (Freq*1000000/baud)>>8;   //波特率高位控制
	IE1 |= 0x01;      //开启SSI0中断
//    EA = 1;	
}


/*****************************************************
*函数名称：void Uart0_Init(uint Freq,unsigned long int baud)
*函数功能：Uart0中断初始化
*入口参数：Freq-主频，baud-波特率
*出口参数：void
*****************************************************/
void F_Uart0_Init(uint Freq,unsigned long int baud)
{
	P2CON &= 0xFC;    //TX/RX设置为输入带上拉
	P2PH  |= 0x03;
	
	OTCON |= 0x30;    //串行接口USCI0选择Uart0通信
	US0CON0 = 0x50;   //设置通信方式为模式一，允许接收
	US0CON1 = Freq*1000000/baud;   //波特率低位控制
	US0CON2 = (Freq*1000000/baud)>>8;   //波特率高位控制
	IE1 |= 0x01;      //开启USCI0中断	
}


/*****************************************************
*函数名称：void Uart2_Init(uint Freq,unsigned long int baud)
*函数功能：Uart2中断初始化
*入口参数：Freq-主频，baud-波特率
*出口参数：void
*****************************************************/
void F_USCI2_Init(uint Freq,unsigned long int baud)
{
	P1CON &= 0xCF;    //TX/RX设置为输入带上拉
	P1PH  |= 0x30;
	
	TMCON |= 0xC0;    //串行接口USCI2选择Uart2通信
	US2CON0 = 0x40;   //设置通信方式为模式一，允许接收
	US2CON1 = Freq*1000000/baud;   //波特率低位控制
	US2CON2 = (Freq*1000000/baud)>>8;   //波特率高位控制
	IE2 |= 0x02;      //开启USCI2中断
}


void F_TIMER0_Init(void)
{
//	TMCON = 0X00;    //------111 ;Timer0、Tiemr1和Tiemr2选择时钟Fsys/12
	
	//T0设置，现象：P00输出周期为1ms的方波(主频：32M)
	TMOD |= 0x02;                 //0000 0001;Timer0设置工作方式2
	TL0 = 89;    //时钟为Fsys，则溢出时间=16000*（1/Fsys）;
	TH0 = 89;
	TR0 = 0;
	ET0 = 1;//定时器0允许
	TR0 = 1;//打开定时器0
	
	_BS(IP,1);
}

/*****************************************************
*函数名称: SC_GPIO_Init
*函数功能: GPIO初始化函数
*入口参数：void
*出口参数：void
*****************************************************/
void SC_GPIO_Init(void)
{
	//IO口配置
	P0 = 0x00;
	P2 = 0x00;
	P1 = 0x00;
	P5 = 0x00;
	
	Set_P16_pp;
	Set_P27_pp;
	
	Set_P50_pu;
	Set_P51_pu;
	Set_P11_pu;

	Set_P17_in;
	Set_P26_in;	
	
	Set_P00_pp;	
	Set_P01_pp;	
	Set_P02_pp;	
	Set_P03_pp;
	Set_P04_pp;
	Set_P05_pp;
	Set_P06_pp;	
	
	Set_P07_in;	
	Set_P22_in;	
	Set_P23_in;	
	
	P0 = 0x00;
	P2 = 0x00;
	P1 = 0x00;
	P5 = 0x00;
	
}

/*****************************************************
*函数名称: SC_Init
*函数功能: MCU初始化函数
*入口参数：void
*出口参数：void
*****************************************************/
void F_MCU_Init(void)
{
	F_GPIO_ParallelPortSet();
	SC_GPIO_Init();
	//F_Uart0_Init(16,57600);
		F_USCI2_Init(16,9600);
	F_TIMER0_Init();
}




