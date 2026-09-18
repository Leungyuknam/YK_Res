#define	TM1668_C

#include "Include\Include.h"
#include <intrins.h>

#define	PO_SDA_1668		P20
#define	PO_STB_1668		P01
#define	PO_CLK_1668		P05
#define	CLK_H()			{PO_CLK_1668 = 1;}
#define	CLK_L()			{PO_CLK_1668 = 0;}
#define	IODelay()		{_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}//_nop_();_nop_();}

#define	DIO_H()			{PO_SDA_1668 = 1;}
#define	DIO_L()			{PO_SDA_1668 = 0;}
#define	DIO_Input()		{P1CON &= ~BIT1;}
#define	DIO_Output()	{P1CON  |= BIT1;}
#define	Get_DIO()		(PO_SDA_1668)
#define STB_H()			{PO_STB_1668 = 1;}
#define	STB_L()			{PO_STB_1668 = 0;}

uchar Keybuf[5] = {0};

void Write8bit_1668(uchar buf)
{
	uchar i;

	DIO_Output();
	i = 0; 
	while(i < 8)
	{
		if(buf & 0x01)
		{
			DIO_H();
		}
		else
		{
			DIO_L();
		}
		CLK_L();
		buf = buf >> 1;
		i++ ;
		CLK_H();
	}			
}

/*
uchar Read8bit_1668(void)
{
	uchar i;
	uchar Result;
	DIO_Input();
	i = 0; 
	Result = 0;
	while(i < 8)
	{
		CLK_L();
		IODelay();
		Result = Result >> 1;
		if(Get_DIO())
		{
			Result |= 0x80;
		}
		CLK_H();
		i++ ;
		IODelay();
	}			
	return Result;
}*/

void TM1668_DispSet(uchar pBuf[])
{
	uchar i;
	STB_L();
	Write8bit_1668(b1100_0000);
	for(i = 0;i < TM_1668M_LENGTH ; i++)
	{
		Write8bit_1668(pBuf[i]);
		IODelay();
	}
	STB_H();
}

void TM1668_SetBright(uchar level)
{	
	level &= b1000_1111;
	STB_L();
	Write8bit_1668(level );//| b1000_1000
	STB_H();
}

/*uint Read1668Key(void)
{
	uchar i;
	uint temp;
	temp = 0;
	STB_L();
	Write8bit_1668(b0100_0010);
	IODelay();
	for(i = 0;i < 4 ; i++)
	{
		temp = temp << 4;
		Keybuf[i] = Read8bit_1668();
		temp |= Keybuf[i] & b0000_1011;
		if(Keybuf[i] & b0001_0000)temp |= b0000_0100;
	}
	STB_H();
	return temp;
}*/

void TM1668_Set(uchar pBuf[],uchar BrightLevel)
{
	STB_H();
	IODelay();
	STB_L();
	Write8bit_1668(b0000_0011);
	STB_H();
	IODelay();
	STB_L();
	Write8bit_1668(b0100_0000);
	STB_H();
	IODelay();
	STB_L();
	TM1668_DispSet(pBuf );
	STB_H();
	IODelay();
	STB_L();
	TM1668_SetBright(BrightLevel | b1000_1000);
	STB_H();
}
