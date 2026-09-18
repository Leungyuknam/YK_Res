#ifndef __DISPMCU_H_
#define __DISPMCU_H_



#define TM1640 0
#define TM1650 1
#define TM1628 0
#define TM1668 0
#define TM1629B 0



#if TM1640	 
#define P_DIN 	P11
#define P_SCLK	P10
#define F_SET_DIN()   {	P1 |= (0x01<<1);	}	 
#define F_CLR_DIN()   {	P1 &= ~(0x01<<1);	}	 
#define F_SET_SCLK()  {	P1 |= (0x01<<0);	}	  	
#define F_CLR_SCLK()  {	P1 &= ~(0x01<<0);	}	 

extern void F_1640Driver(unsigned char *l_pu8Buf,unsigned char l_u8Number,unsigned char l_u8Brightness); //1~7逐渐增加亮度			
#endif

#if TM1650
/********************定义控制端口**********************/
#define	SDA			P_DAT
#define	SCL			P_CLK

//#define	SDAIn()			{	P5M = (P5M &0xf7);_nop_();_nop_();	}
#define	SDAOut()		{	P1CON |= BIT1;_nop_();_nop_();	}
#define	SCLOut()		{	P1CON |= BIT2;_nop_();_nop_();	}

extern void F_1650Driver(unsigned char *l_pu8Buf,unsigned char l_u8Brightness); //1~7逐渐增加亮度			

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
#define	P_STB				P04
#define	P_SCK				P03
#define	P_DIO				P02
#define	F_DIO_IN()		{ P0CON &= ~BIT2;}
#define	F_DIO_OUT()		{ P0CON |= BIT2; }

#define F_TEST_DIO()   (P_DIO)

#define F_SET_DIO()   P_DIO = 1;
#define F_CLR_DIO()   P_DIO = 0;
#define F_SET_SCLK()  P_SCK = 1;
#define F_CLR_SCLK()  P_SCK = 0;
#define F_SET_STB()   P_STB = 1;
#define F_CLR_STB()   P_STB = 0;

#define CFG_DATA_CMD_SETTING    0x40 // 0x44为固定地址模式，0x40为自动地址增加模式
#define CFG_DISP_MODE_SETTING   0x01 //  显示命令设置,7位10段(0x03) 6位11段(0x02)	5位12段(0x01) 4位13段(0x00) 

extern void F_1668Driver(unsigned int *l_pu16Buf,unsigned char l_u8Number,unsigned char l_u8Brightness);//1~7逐渐增加亮度			

#endif

/********************TM1629定义控制端口**********************/
#if TM1629B
#define P_1629_STB P51
#define P_1629_SCK P52
#define P_1629_DIO P53
#define	F_1629_DIO_IN()		{ P5CON &= ~BIT3;}
#define	F_1629_DIO_OUT()	{ P5CON |= BIT3; }

#define F_TEST_DIO()   (P_1629_DIO)

#define F_1629_SET_DIO()   P_1629_DIO = 1;
#define F_1629_CLR_DIO()   P_1629_DIO = 0;
#define F_1629_SET_SCLK()  P_1629_SCK = 1;
#define F_1629_CLR_SCLK()  P_1629_SCK = 0;
#define F_1629_SET_STB()   P_1629_STB = 1;
#define F_1629_CLR_STB()   P_1629_STB = 0;

//#define	CFG_READ_BYTE			0x42  	//读取按键命令

extern void F_1629Driver(unsigned int *l_pu16Buf,unsigned char l_u8Number,unsigned char l_u8Brightness);	//1~7逐渐增加亮度	
extern void F_ReadKey(unsigned char *KeyBuf);
#endif

#endif