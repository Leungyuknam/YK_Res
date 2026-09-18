
#ifndef _DISPLAY_DRIVE_H_
#define _DISPLAY_DRIVE_H_


#define TM1640 0
#define TM1650 1
#define TM1628 0
#define TM1668 0
#define TM1618 0

#ifndef	uint8
#define uint8    unsigned char
#endif

#if TM1640
#define P_DIN 			P06
#define P_SCLK			P05
#define F_SET_DIN()   	P_DIN = 1	 
#define F_CLR_DIN()   	P_DIN = 0	 
#define F_SET_SCLK()  	P_SCLK = 1	  	
#define F_CLR_SCLK()  	P_SCLK = 0	 

extern void F_1640Driver(unsigned char *l_pu8Buf,unsigned char l_u8Number,unsigned char l_u8Brightness); //1~7逐渐增加亮度			
#endif

#if TM1650
/********************定义控制端口**********************/
#define	SDA_1			P41
#define	SCL_1			P40

//#define	SDAIn()			{	P5M = (P5M &0xf7);_nop_();_nop_();	}
#define	SDAOut_1()		{	P4CON |= (0x01<<1);_nop_();_nop_();	}
#define	SCLOut_1()		{	P4CON|= (0x01<<0);_nop_();_nop_();	}

extern void F_1650Driver_1(unsigned char *l_pu8Buf,unsigned char l_u8Brightness); //1~7逐渐增加亮度			

#define	SDA_2			P15
#define	SCL_2			P14

//#define	SDAIn()			{	P5M = (P5M &0xf7);_nop_();_nop_();	}
#define	SDAOut_2()		{	P1CON |= (0x01<<5);_nop_();_nop_();	}
#define	SCLOut_2()		{	P1CON|= (0x01<<4);_nop_();_nop_();	}

extern void F_1650Driver_2(unsigned char *l_pu8Buf,unsigned char l_u8Brightness); //1~7逐渐增加亮度	

#endif


#if TM1628
/********************定义控制端口**********************/

#define P_STB P3_1
#define P_DIO P4_1
#define P_SCK P4_2
#define	F_DIO_IN()		{ P4M0 |= (0x01<<1); P4M1 &= ~(0x01<<1);}
#define	F_DIO_OUT()		{ P4M0 &= ~(0x01<<1); P4M1 |= (0x01<<1); }

#define F_SET_DIO()   P_DIO = 1;
#define F_CLR_DIO()   P_DIO = 0;
#define F_SET_SCLK()  P_SCK = 1;
#define F_CLR_SCLK()  P_SCK = 0;
#define F_SET_STB()   P_STB = 1;
#define F_CLR_STB()   P_STB = 0;

//#define	CFG_READ_BYTE			0x42  	//读取按键命令

#define CFG_DISP_MODE_SETTING   0x00	// 显示命令设置,7位10段(0x03) 6位11段(0x02)	5位12段(0x01) 4位13段(0x00) 

extern void F_1628Driver(unsigned int *l_pu16Buf,unsigned char l_u8Number,unsigned char l_u8Brightness);	//1~7逐渐增加亮度	

#endif


#if TM1668
#define	P_STB				P21
#define	P_SCK				P05
#define	P_DIO				P20

#define F_SET_DIO()         P_DIO = 1;
#define F_CLR_DIO()   		P_DIO = 0;
#define F_SET_SCLK()  		P_SCK = 1;
#define F_CLR_SCLK()  		P_SCK = 0;
#define F_SET_STB()   		P_STB = 1;
#define F_CLR_STB()   		P_STB = 0;

#define CFG_DATA_CMD_SETTING    0x40 // 0x44为固定地址模式，0x40为自动地址增加模式
#define CFG_DISP_MODE_SETTING   0x03 //  显示命令设置,7位10段(0x03) 6位11段(0x02)	5位12段(0x01) 4位13段(0x00) 

extern void F_1668Driver(unsigned int *l_pu16Buf,unsigned char l_u8Number,unsigned char l_u8Brightness);//1~7逐渐增加亮度			

#endif

#if TM1618
#define	P_STB				P27
#define	P_SCK				P14
#define	P_DIO				P15

#define F_SET_DIO()   P_DIO = 1;
#define F_CLR_DIO()   P_DIO = 0;
#define F_SET_SCLK()  P_SCK = 1;
#define F_CLR_SCLK()  P_SCK = 0;
#define F_SET_STB()   {P_STB = 1;}
#define F_CLR_STB()   {P_STB = 0;}

#define CFG_DATA_CMD_SETTING    0x40 // 0x44为固定地址模式，0x40为自动地址增加模式
#define CFG_DISP_MODE_SETTING   0x00 //  显示命令设置,7位5段(0x03) 6位6段(0x02)	5位7段(0x01) 4位8段(0x00) 

extern void F_1618Driver(unsigned int *l_pu16Buf,unsigned char l_u8Number,unsigned char l_u8Brightness);//1~7逐渐增加亮度			

#endif

#endif
