#ifndef	_DispDrive_IO_Pin_H
#define	_DispDrive_IO_Pin_H

#ifdef _Native_DISPDR_H
   #define	EXT_DISPDR 
#else
   #define	EXT_DISPDR extern	
#endif




#define	COM_MAX_CNT 		7
#ifdef _Native_DISPDR_H
	EXT_DISPDR bit B_DispUpdating = 0;
   
#else
	EXT_DISPDR bit B_DispUpdating;
   
#endif


#define	COM_OFF			1
#define	COM_ON			0
   
#define		P_COM5		P51	//PIN4
#define		P_SEG_1L	P50	//PIN5
#define		P_SEG_7L	P11	//PIN6
#define		P_SEG_6L	P12	//PIN7   tTCK
#define		P_SI		P13	//PIN8	 tDIO
#define		P_SEG_5L	P14	//PIN9
#define		P_SEG_3L	P15	//PIN10
#define		P_SEG_4L	P16	//PIN11
#define		P_COM1		P17	//PIN12
#define		P_COM6		P27	//PIN13
#define		P_COM7		P26	//PIN14
//PIN15~PIN28
#define		P_SEG_2L	P25	//PIN15
#define		P_COM2		P24	//PIN16
#define		P_COM3		P23	//PIN17
#define		P_COM4		P22	//PIN18



#define	COM1			P_COM1
#define COM2			P_COM2
#define	COM3			P_COM3
#define COM4			P_COM4
#define COM5			P_COM5
#define	COM6			P_COM6
#define COM7			P_COM7


#define	COM1_ON()		{P1CON |= BIT7;COM1 = COM_ON;}
#define	COM2_ON()		{P2CON |= BIT4;COM2 = COM_ON;}
#define	COM3_ON()		{P2CON |= BIT3;COM3 = COM_ON;}
#define	COM4_ON()		{P2CON |= BIT2;COM4 = COM_ON;}
#define	COM5_ON()		{P5CON |= BIT1;COM5 = COM_ON;}
#define	COM6_ON()		{P2CON |= BIT7;COM6 = COM_ON;}
#define	COM7_ON()		{P2CON |= BIT6;COM7 = COM_ON;}


#define	SEG_ON			1
#define	SEG_OFF			0

#define	PIN_A			P_SEG_5L
#define	PIN_B			P_SEG_6L
#define	PIN_C			P_SEG_7L
#define	PIN_D			P_SEG_1L
#define	PIN_E			P_SEG_2L
#define	PIN_F			P_SEG_4L
#define	PIN_G			P_SEG_3L
//#define	PIN_DP			P01


#define	SEGA_OFF				PIN_A = SEG_OFF	//SEG0
#define	SEGB_OFF				PIN_B = SEG_OFF	//SEG1
#define	SEGC_OFF				PIN_C = SEG_OFF	//SEG2
#define	SEGD_OFF				PIN_D = SEG_OFF	//SEG3
#define	SEGE_OFF				PIN_E = SEG_OFF	//SEG4
#define	SEGF_OFF				PIN_F = SEG_OFF	//SEG5
#define	SEGG_OFF				PIN_G = SEG_OFF	//SEG6
#define	SEGDP_OFF				PIN_DP = SEG_OFF//SEG7

#define	SEGA_ON				PIN_A = SEG_ON	//SEG0
#define	SEGB_ON				PIN_B = SEG_ON	//SEG1
#define	SEGC_ON				PIN_C = SEG_ON	//SEG2
#define	SEGD_ON				PIN_D = SEG_ON	//SEG3
#define	SEGE_ON				PIN_E = SEG_ON	//SEG4
#define	SEGF_ON				PIN_F = SEG_ON	//SEG5
#define	SEGG_ON				PIN_G = SEG_ON	//SEG6
#define	SEGDP_ON			PIN_DP = SEG_ON	//SEG7
	  
#define	All_COM_OFF() 		{P1CON &= ~BIT7;P2CON &= ~(BIT2+BIT3+BIT4+BIT6+BIT7);P5CON &= ~BIT1;}

								
			
//#define BSEG_0   	0
//#define BSEG_A   	BIT0
//#define BSEG_B   	BIT1
//#define BSEG_C   	BIT2
//#define BSEG_D   	BIT3
//#define BSEG_E   	BIT4
//#define BSEG_F   	BIT5
//#define BSEG_G   	BIT6
//#define BSEG_DP  	BIT7





EXT_DISPDR void DispLedBufSet(u8 *u8Buf);
EXT_DISPDR void DispPIN_DR(void);
#endif
