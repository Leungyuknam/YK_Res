#ifndef CodeI2cDrv_H
#define CodeI2cDrv_H

#ifdef	EXTR
#undef	EXTR     
#endif

#ifdef	CodeI2cDrv	
#define EXTR     
#else             
#define EXTR extern
#endif 

#define WIFI_SN_EN			1

//#define P_E2SDA_INMODE()	P40_INMODE()
//#define P_E2SDA_OUTMODE()	P40_OUTMODE()  
//#define P_E2SDA_CLRB()		P40_CLRB()
//#define P_E2SDA_READ()		P40_READ() 
//#define P_E2SDA_SETB()		P40_SETB()

//#define P_E2SCL_INMODE()		P41_INMODE()
//#define P_E2SCL_OUTMODE()	P41_OUTMODE()
//#define P_E2SCL_CLRB()		P41_CLRB()
//#define P_E2SCL_SETB()		P41_SETB()

#define	P_E2SDA_INMODE()	{P4CR &= ~BIT0;}
#define	P_E2SDA_OUTMODE()	{P4CR |= BIT0;}  
#define	P_E2SDA_CLRB()		{P4 &= ~BIT0;}
#define	P_E2SDA_READ()		(P4 & BIT0)
#define	P_E2SDA_SETB()		{P4 |= BIT0;}

#define	P_E2SCL_INMODE()	{P4CR &= ~BIT3;}
#define	P_E2SCL_OUTMODE()	{P4CR |= BIT3;}
#define	P_E2SCL_CLRB()		{P4 &= ~BIT3;}
#define	P_E2SCL_SETB()		{P4 |= BIT3;}

EXTR void CodeWriteProc(void);
EXTR uchar GetFactoryID(uchar idx);
#endif
/******************************************************
					END	
******************************************************/
