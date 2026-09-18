#ifndef	_DispDrive_IO_Pin_H
#define	_DispDrive_IO_Pin_H

#ifdef _Native_DISPDR_H
   #define	EXT_DISPDR 
#else
   #define	EXT_DISPDR extern	
#endif




#define	COM_MAX_CNT 		4
#ifdef _Native_DISPDR_H
	EXT_DISPDR bit B_DispUpdating = 0;
   
#else
	EXT_DISPDR bit B_DispUpdating;
   
#endif


#define	COM_OFF			1
#define	COM_ON			0
   





#define	SEG_ON			1
#define	SEG_OFF			0

#define	PIN_A			P_SEG1
#define	PIN_B			P_SEG2
#define	PIN_C			P_SEG3
#define	PIN_D			P_SEG4
#define	PIN_E			P_SEG5

#define	PIN_F			P_SEG6
//#define	PIN_F			P_SEG_4L
//#define	PIN_G			P_SEG_3L
//#define	PIN_DP			P01


#define	SEGA_OFF				PIN_A = SEG_OFF	//SEG0
#define	SEGB_OFF				PIN_B = SEG_OFF	//SEG1
#define	SEGC_OFF				PIN_C = SEG_OFF	//SEG2
#define	SEGD_OFF				PIN_D = SEG_OFF	//SEG3
#define	SEGE_OFF				PIN_E = SEG_OFF	//SEG4

#define	SEGF_OFF				PIN_F = SEG_OFF	//SEG5
//#define	SEGF_OFF				PIN_F = SEG_OFF	//SEG5
//#define	SEGG_OFF				PIN_G = SEG_OFF	//SEG6
//#define	SEGDP_OFF				PIN_DP = SEG_OFF//SEG7

#define	SEGA_ON				PIN_A = SEG_ON	//SEG0
#define	SEGB_ON				PIN_B = SEG_ON	//SEG1
#define	SEGC_ON				PIN_C = SEG_ON	//SEG2
#define	SEGD_ON				PIN_D = SEG_ON	//SEG3
#define	SEGE_ON				PIN_E = SEG_ON	//SEG4

#define	SEGF_ON				PIN_F = SEG_ON	//SEG5
//#define	SEGF_ON				PIN_F = SEG_ON	//SEG5
//#define	SEGG_ON				PIN_G = SEG_ON	//SEG6
//#define	SEGDP_ON			PIN_DP = SEG_ON	//SEG7

#define	COM1			P_COM1
#define COM2			P_COM2
#define	COM3			P_COM3
#define COM4			P_COM4
//#define COM5			P_COM5
//#define	COM6			P_COM6
//#define COM7			P_COM7


#define	COM1_ON()		{P0CON |= BIT3;COM1 = COM_ON;}
#define	COM2_ON()		{P2CON |= BIT4;COM2 = COM_ON;}
#define	COM3_ON()		{P0CON |= BIT1;COM3 = COM_ON;}
#define	COM4_ON()		{P2CON |= BIT2;COM4 = COM_ON;}
//#define	COM5_ON()		{P5CON |= BIT1;COM5 = COM_ON;}
//#define	COM6_ON()		{P2CON |= BIT7;COM6 = COM_ON;}
//#define	COM7_ON()		{P2CON |= BIT6;COM7 = COM_ON;}
	  
#define	All_COM_OFF() 		{P0CON &= ~(BIT1+BIT3);P2CON &= ~BIT2;P2CON &= ~BIT4;}








EXT_DISPDR void DispLedBufSet(u8 *u8HalfBuf, u8 *u8Buf);
EXT_DISPDR void DispPIN_DR(void);
#endif
