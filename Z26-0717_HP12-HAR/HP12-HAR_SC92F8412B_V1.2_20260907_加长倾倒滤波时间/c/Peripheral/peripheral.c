#include "h\Allinclude.h"

void port_init()
{
    OPINX = 0x86;
    OPREG &= (~0x08);       // 关闭 READ-IO 功能

    P0    = B00100000;
    P0CON = B00101111;      
    P0PH  = B00000000;

    P1    = B00101100;
    P1CON = B00111100;
    P1PH  = B00000000;


    P2    = B11110011;
    P2CON = B11110011;
    P2PH  = B11000000;

    SC92F8412B_NIO_Init();
}

void Timer0_init()
{
    TMCON = 0x00;           // 系统时钟 12 分频
    TMOD  = 0x02;           // 8 位自动重装

    TL0 = 256 - 250;        // 125us
    TH0 = 256 - 250;

    TR0 = 0;
    ET0 = 1;
    TR0 = 1;

    IP |= 0x02;
}

void f_iapWrite(U8 l_u8IapArea, U16 l_u16Addr, U8 *l_pu8Data, U8 l_u8Length)
{
    bit l_bEa = 0;
    l_bEa = EA;
    EA = 0;
    IAPADE = l_u8IapArea;
    while(l_u8Length)
    {
        l_u8Length--;
        IAPADH = (l_u16Addr + l_u8Length) >> 8;
        IAPADL = l_u16Addr + l_u8Length;
        IAPDAT = l_pu8Data[l_u8Length];
        IAPKEY = 240;
        IAPCTL = 0x0a;
    }
    _nop_(); _nop_(); _nop_(); _nop_();
    _nop_(); _nop_(); _nop_(); _nop_();
    IAPADE = 0x00;
    EA = l_bEa;
}

void f_iapRead(U8 l_u8IapArea, U16 l_u16Addr, U8 *l_pu8Data, U8 l_u8Length)
{
    U8 code *c_pu8Pt = 0x00;
    bit l_bEa = 0;
    l_bEa = EA;
    EA = 0;
    while(l_u8Length)
    {
        l_u8Length--;
        IAPADE = l_u8IapArea;
        l_pu8Data[l_u8Length] = c_pu8Pt[l_u16Addr + l_u8Length];
        IAPADE = 0x00;
    }
    EA = l_bEa;
}

void f_adcInit()
{	
	// 选项配置参考内部 VDD
	//IO输入

	//使能通道端口，如检测1/4VDD直接选择0x1f通道转换即可
	ADCCFG0 = 0x00;
	ADCCFG1 = 0x00;
	
	ADCCFG2 = 0x09;		//采样时间=(36+14)*24/Fosc, 74=75US,84=50US
//    ADCCFG2 = 0x03;
	ADCCON = 0x80;		//启动ADC电源
}

void WDT_clear()
{
    WDTCON = 0x10;          // 约 524 ms 看门狗，在代码选项中使能
}

void F_mCpuInital(void)
{
    WDT_clear();
    port_init();
    Timer0_init();
}
