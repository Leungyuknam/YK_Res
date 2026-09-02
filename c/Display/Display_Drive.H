
#ifndef _DISPLAY_DRIVE_H_
#define _DISPLAY_DRIVE_H_


#define TM1640 1
#define TM1650 0
#define TM1628 0
#define TM1668 0
#define TM1618 0

#ifndef	uint8
#define uint8    unsigned char
#endif

#if TM1640
#define P_DIN 			P30
#define P_SCLK			P31
#define F_SET_DIN()   	P_DIN = 1	 
#define F_CLR_DIN()   	P_DIN = 0	 
#define F_SET_SCLK()  	P_SCLK = 1	  	
#define F_CLR_SCLK()  	P_SCLK = 0	 

extern void F_1640Driver(unsigned char *l_pu8Buf,unsigned char l_u8Number,unsigned char l_u8Brightness); //1~7Öð½¥Ôö¼ÓÁÁ¶È			
#endif

#endif
