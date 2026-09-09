/*--------------------------------------------------------------------------
REG79F328A.h

Header file for generic SH79xx series microcontroller.
Copyright (c) 1996-2007 Sionwealth Electronic and Sinowealth Software, Inc.
All rights reserved.
--------------------------------------------------------------------------*/

#ifndef REG79F328A_H
#define REG79F328A_H

/* ------------------- BYTE Register-------------------- */
/* CPU */
sfr ACC    = 0xE0;
sfr B      = 0xF0;
sfr AUXC   = 0xF1;
sfr PSW    = 0xD0;
sfr SP     = 0x81;
sfr DPL    = 0x82;
sfr DPH    = 0x83;
sfr DPL1   = 0x84;
sfr DPH1   = 0x85;
sfr INSCON = 0x86;

/*Power*/
sfr PCON  = 0x87;
sfr SUSLO = 0x8E;

/* FLASH */
sfr IB_CON1   = 0xF2;
sfr IB_CON2   = 0xF3;
sfr IB_CON3   = 0xF4;
sfr IB_CON4   = 0xF5;
sfr IB_CON5   = 0xF6;	
sfr XPAGE     = 0xF7;
sfr IB_OFFSET = 0xFB;
sfr IB_DATA   = 0xFC;
sfr FLASHCON  = 0xA7;

/*WDT*/             
sfr RSTSTAT = 0xB1;

/* SYSTEM CLOCK*/
sfr CLKCON = 0xB2;

/* INTERRUPT */
sfr IEN0  = 0xA8;
sfr IEN1  = 0xA9;
sfr IEN2  = 0xAA;
sfr IPL0  = 0xB8;
sfr IPL1  = 0xB9;
sfr IPH0  = 0xB4;
sfr IPH1  = 0xB5;
sfr EXF0  = 0xE8;
sfr EXF1  = 0xD8;
sfr ELEDCON = 0xEF;
sfr IENC = 0xBA;
sfr EXCON = 0x8B;
sfr TCON = 0x88;

/* TWI */
sfr TWICON  = 0xC8;
sfr TWISTA  = 0xDF;
sfr TWIBR   = 0x8A;
sfr TWIADR  = 0x8C;
sfr TWIDAT  = 0x8D;
sfr TWIAMR  = 0x8F;
sfr TWITOUT = 0xFE;
sfr TWTFREE = 0x89;

/* PORT */
sfr P0    = 0x80;
sfr P1    = 0x90;
sfr P2    = 0xA0;
sfr P3    = 0xB0;
sfr P4	  = 0xC0;
sfr P5    = 0x80;
sfr P0CR  = 0xE1;
sfr P1CR  = 0xE2;
sfr P2CR  = 0xE3;
sfr P3CR  = 0xE4;
sfr P4CR  = 0xE5;
sfr P5CR  = 0xE1;
sfr P0PCR = 0xE9;
sfr P1PCR = 0xEA;
sfr P2PCR = 0xEB;
sfr P3PCR = 0xEC;
sfr P4PCR = 0xED;
sfr P5PCR = 0xE9;
sfr UTOS = 0xEE;

/* TIMER3 */
sfr T3CON = 0x88;
sfr TL3   = 0x8C;
sfr TH3   = 0x8D;

/*TIMER4*/
sfr T4CON  = 0xC8;
sfr TL4  = 0xCC;
sfr TH4  = 0xCD;

/*TIMER5*/
sfr T5CON = 0xC0;
sfr TL5 = 0xCE;
sfr TH5 = 0xCF;
 
/* PCA0 */
sfr PCACON  = 0xD8;
sfr P0CF    = 0x98;
sfr P0CMD   = 0x99;
sfr P0CPM0  = 0x9A;
sfr P0CPM1  = 0x9B;
sfr P0CPL0  = 0x9C;
sfr P0CPH0  = 0x9D;
sfr P0TOPL  = 0x9E;
sfr P0TOPH  = 0x9F;

sfr P0CPL1  = 0xAC;
sfr P0CPH1  = 0xAD;
sfr P0FORCE = 0xDC;

/* EUART  */
sfr SCON  = 0x98;
sfr SBUF  = 0x99;
sfr SADDR = 0x9A;
sfr SADEN = 0x9B;
sfr SBRTL = 0x9C;
sfr SBRTH = 0x9D;
sfr SFINE = 0x9E;

/* EUART1  */
sfr SCON1  = 0xA0;
sfr SBUF1  = 0xA1;
sfr SADDR1 = 0xA2;
sfr SADEN1 = 0xA3;
sfr SBRTL1 = 0xA4;
sfr SBRTH1 = 0xA5;
sfr SFINE1 = 0xA6;
sfr PCON1  = 0xA7;

/* EUART2 */
sfr SCON2 = 0x90;
sfr SBUF2 = 0x91;
sfr SADDR2 = 0x92;
sfr SADEN2 = 0x93;
sfr SBRTL2 = 0x94;
sfr SBRTH2 = 0x95;
sfr SFINE2 = 0x96;
sfr PCON2 = 0x97;

/*SPI*/
sfr SPSTA = 0xF8;
sfr SPCON = 0xA2;
sfr SPDAT = 0xA3; 

/* ADC */
sfr ADCON1 = 0x93;
sfr ADCON2  = 0x92;
sfr SEQCON = 0x91;
sfr SEQCHX = 0x9F;
sfr ADCH1 = 0x95;
sfr ADCH2 = 0xA5;
sfr ADDXL = 0x96;
sfr ADDXH = 0x97;
sfr ADT     = 0x94;


/*LED*/
sfr LEDST = 0xD5;
sfr LEDCON = 0xD6;
sfr DISCOM = 0xAC;
sfr LEDDZ = 0xD7;
sfr SEG01 = 0xCC;
sfr SEG02 = 0xCD;
sfr LEDCOM = 0xAB;

/*PWM0*/
sfr PWM0CON = 0xD9;
sfr PWM0DH = 0xDD;
sfr PWM0DL = 0xDC;
sfr PWM0PH = 0xDB;
sfr PWM0PL = 0xDA;

/*PWM1*/
sfr PWM1CON = 0xC1;
sfr PWM1PH = 0xC3;
sfr PWM1PL = 0xC2;
sfr PWM1DH = 0xC5;
sfr PWM1DL = 0xC4;

/* LPD */
sfr LPDCON = 0xB3;
sfr LPDSEL = 0xBB;

/* CRC */
sfr CRCCON = 0xFD;
sfr CRCDL = 0xF9;
sfr CRCDH = 0xFA;

/* TOUCHKEY */
sfr TKCON1 = 0xC1;
sfr TKF0 = 0xC7;
sfr TKU1 = 0xC9;
sfr TKU2 = 0xCA;
sfr TKU3 = 0xCB;
sfr TKDIV01 = 0xC3;
sfr TKDIV02 = 0xC4;
sfr TKDIV03 = 0xC5;
sfr TKDIV04 = 0xC6;
sfr TKVREF = 0xCE;
sfr TKST = 0xCF;
sfr TKRANDOM = 0xC2;
sfr TKW = 0xB7;
sfr P0SS = 0xBC;
sfr P1SS = 0xBD;
sfr P4SS = 0xBE;
sfr P5SS = 0xBF;

/* LCM */
sfr UART0CR = 0xE2;
sfr UART1CR = 0xE3;
sfr TWICR = 0xE5;
sfr PWMCR = 0xE6;
sfr CEXCR = 0xE7;
sfr ECICR = 0xEA;
sfr LCMEN = 0xEB;
/*----BIT Register ----*/

/* SPSTA */
sbit SPEN  = 0xFF;
sbit SPIF  = 0xFE;
sbit MODF  = 0xFD;
sbit WCOL  = 0xFC;
sbit RXOV  = 0xFB;

/* B */
/*  EXF0  */
sbit IT41 = 0xEF; 
sbit IT40 = 0xEE;
sbit IT31 = 0xED;
sbit IT30 = 0xEC;
sbit IT21 = 0xEB;
sbit IT20 = 0xEA;
sbit IE3  = 0xE9;
sbit IE2  = 0xE8;
/* ACC */

/*  EXF1  */
sbit IF47 = 0xDF;
sbit IF46 = 0xDE;
sbit IF45 = 0xDD;
sbit IF44 = 0xDC; 
sbit IF43 = 0xDB;
sbit IF42 = 0xDA;
sbit IF41 = 0xD9;
sbit IF40 = 0xD8;

/*  PSW   */
sbit CY  = 0xD7;
sbit AC  = 0xD6;
sbit F0  = 0xD5;
sbit RS1 = 0xD4;
sbit RS0 = 0xD3;
sbit OV  = 0xD2;
sbit F1  = 0xD1;
sbit P   = 0xD0;

/*TWICON*/
sbit TOUT  = 0xCF;
sbit ENTWI = 0xCE;
sbit STA   = 0xCD;
sbit STO   = 0xCC;
sbit TWINT = 0xCB;
sbit AA    = 0xCA;
sbit TFREE = 0xC9;
sbit EFREE = 0xC8;

/* P5 */
sbit P5_0 = P5^0;
sbit P5_1 = P5^1;
sbit P5_2 = P5^2;
sbit P5_3 = P5^3;
sbit P5_4 = P5^4;
sbit P5_5 = P5^5;
sbit P5_6 = P5^6;
sbit P5_7 = P5^7;

/* P4 */
sbit P4_0 = P4^0;
sbit P4_1 = P4^1;
sbit P4_2 = P4^2;
sbit P4_3 = P4^3;
sbit P4_4 = P4^4;
sbit P4_5 = P4^5;
sbit P4_6 = P4^6;
sbit P4_7 = P4^7;

/*  IPL0   */
sbit PINTL   = 0xBF; 
sbit PADCL   = 0xBE;
sbit PT3L    = 0xBD;
sbit PSL    = 0xBC;
sbit PT5L   = 0xBB;
sbit PX1L   = 0xBA;
sbit PTKL   = 0xB9;
sbit PX0L   = 0xB8;

/* P3 */
sbit P3_0 = P3^0;
sbit P3_1 = P3^1;
sbit P3_2 = P3^2;
sbit P3_3 = P3^3;
sbit P3_4 = P3^4;
sbit P3_5 = P3^5;
sbit P3_6 = P3^6;
sbit P3_7 = P3^7;

/*  IEN0   */
sbit EA     = 0xAF;
sbit EADC   = 0xAE;
sbit ET3    = 0xAD;
sbit ES0    = 0xAC;
sbit ET5   = 0xAB;
sbit EX1  = 0xAA;
sbit ETK  = 0xA9;
sbit EX0  = 0xA8;

/* P2 */
sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;
sbit P2_4 = P2^4;
sbit P2_5 = P2^5;
sbit P2_6 = P2^6;
sbit P2_7 = P2^7; 

/*  SCON  */
sbit SM0_FE    = 0x9F;
sbit SM1_RXOV  = 0x9E;
sbit SM2_TXCOL = 0x9D;
sbit REN       = 0x9C;
sbit TB8       = 0x9B;
sbit RB8       = 0x9A;
sbit TI        = 0x99;
sbit RI        = 0x98;

/* P1 */
sbit P1_0 = P1^0;
sbit P1_1 = P1^1;
sbit P1_2 = P1^2;
sbit P1_3 = P1^3;
sbit P1_4 = P1^4;
sbit P1_5 = P1^5;
sbit P1_6 = P1^6;
sbit P1_7 = P1^7;

/*  TCON  */

sbit IE1  = 0x8B;
sbit IT1  = 0x8A;
sbit IE0  = 0x89;
sbit IT0 = 0x88;

/* P0 */
sbit P0_0 = P0^0;
sbit P0_1 = P0^1;
sbit P0_2 = P0^2;
sbit P0_3 = P0^3;
sbit P0_4 = P0^4;
sbit P0_5 = P0^5;
sbit P0_6 = P0^6;
sbit P0_7 = P0^7;

/*  PCACON  */
sbit PR0 = 0xD8;

/*  T4CON  */
sbit TF4 = 0xCF;
sbit TC4 = 0xCE;
sbit T4PS1  = 0xCD;
sbit T4PS0  = 0xCC;
sbit T4M1  = 0xCB;
sbit T4M0   = 0xCA;
sbit TR4  = 0xC9;
sbit T4CLKS = 0xC8;

/*  T5CON  */
sbit TF5    = 0xC7;
sbit T5PS1    = 0xC5;
sbit T5PS0   = 0xC4;
sbit TR5 = 0xC1;

/*  SCON1  */          
sbit SM10_FE1    = 0xA7;
sbit SM11_RXOV1  = 0xA6;
sbit SM12_TXCOL1 = 0xA5;
sbit REN1        = 0xA4;
sbit TB81        = 0xA3;
sbit RB81        = 0xA2;
sbit TI1         = 0xA1;
sbit RI1         = 0xA0;

/*  P0CF  */          
sbit CF0    = 0x9F;
sbit P0CCF1 = 0x99;
sbit P0CCF0 = 0x98;

/*  SCON2  */          
sbit SM20_FE2    = 0x97;
sbit SM21_RXOV2  = 0x96;
sbit SM22_TXCOL2 = 0x95;
sbit REN2        = 0x94;
sbit TB82        = 0x93;
sbit RB82        = 0x92;
sbit TI2         = 0x91;
sbit RI2         = 0x90;

/*  T3CON  */
sbit TF3    = 0x8F;
sbit T3PS1   = 0x8D;
sbit T3PS0  = 0x8C;
sbit TR3    = 0x8A;
sbit T3CLKS1   = 0x89;
sbit T3CLKS0 = 0x88;

extern unsigned char volatile xdata SINO_LEDRAM_1 ;
extern unsigned char volatile xdata SINO_LEDRAM_2 ;
extern unsigned char volatile xdata SINO_LEDRAM_3 ;
extern unsigned char volatile xdata SINO_LEDRAM_4 ;
extern unsigned char volatile xdata SINO_LEDRAM_5 ;
extern unsigned char volatile xdata SINO_LEDRAM_6 ;
extern unsigned char volatile xdata SINO_LEDRAM_7 ;
extern unsigned char volatile xdata SINO_LEDRAM_8 ;
extern unsigned char volatile xdata SINO_LEDRAM_9 ;
extern unsigned char volatile xdata SINO_LEDRAM_10;
extern unsigned char volatile xdata SINO_LEDRAM_11;
extern unsigned char volatile xdata SINO_LEDRAM_12;
extern unsigned char volatile xdata SINO_LEDRAM_13;
extern unsigned char volatile xdata SINO_LEDRAM_14;
extern unsigned char volatile xdata SINO_LEDRAM_15;
extern unsigned char volatile xdata SINO_LEDRAM_16;

extern unsigned char volatile xdata SINO_TKRAM_1;
extern unsigned char volatile xdata SINO_TKRAM_2;
extern unsigned char volatile xdata SINO_TKRAM_3;
extern unsigned char volatile xdata SINO_TKRAM_4;
extern unsigned char volatile xdata SINO_TKRAM_5;
extern unsigned char volatile xdata SINO_TKRAM_6;
extern unsigned char volatile xdata SINO_TKRAM_7;
extern unsigned char volatile xdata SINO_TKRAM_8;
extern unsigned char volatile xdata SINO_TKRAM_9;
extern unsigned char volatile xdata SINO_TKRAM_10;
extern unsigned char volatile xdata SINO_TKRAM_11;
extern unsigned char volatile xdata SINO_TKRAM_12;
extern unsigned char volatile xdata SINO_TKRAM_13;
extern unsigned char volatile xdata SINO_TKRAM_14;
extern unsigned char volatile xdata SINO_TKRAM_15;
extern unsigned char volatile xdata SINO_TKRAM_16;
extern unsigned char volatile xdata SINO_TKRAM_17;
extern unsigned char volatile xdata SINO_TKRAM_18;
extern unsigned char volatile xdata SINO_TKRAM_19;
extern unsigned char volatile xdata SINO_TKRAM_20;
extern unsigned char volatile xdata SINO_TKRAM_21;
extern unsigned char volatile xdata SINO_TKRAM_22;
extern unsigned char volatile xdata SINO_TKRAM_23;
extern unsigned char volatile xdata SINO_TKRAM_24;

extern unsigned char volatile xdata SINO_TKRAM_25;
extern unsigned char volatile xdata SINO_TKRAM_26;
extern unsigned char volatile xdata SINO_TKRAM_27;
extern unsigned char volatile xdata SINO_TKRAM_28;
extern unsigned char volatile xdata SINO_TKRAM_29;
extern unsigned char volatile xdata SINO_TKRAM_30;
extern unsigned char volatile xdata SINO_TKRAM_31;
extern unsigned char volatile xdata SINO_TKRAM_32;
extern unsigned char volatile xdata SINO_TKRAM_33;
extern unsigned char volatile xdata SINO_TKRAM_34;
extern unsigned char volatile xdata SINO_TKRAM_35;
extern unsigned char volatile xdata SINO_TKRAM_36;
extern unsigned char volatile xdata SINO_TKRAM_37;
extern unsigned char volatile xdata SINO_TKRAM_38;
extern unsigned char volatile xdata SINO_TKRAM_39;
extern unsigned char volatile xdata SINO_TKRAM_40;
extern unsigned char volatile xdata SINO_TKRAM_41;
extern unsigned char volatile xdata SINO_TKRAM_42;
extern unsigned char volatile xdata SINO_TKRAM_43;
extern unsigned char volatile xdata SINO_TKRAM_44;
extern unsigned char volatile xdata SINO_TKRAM_45;
extern unsigned char volatile xdata SINO_TKRAM_46;
extern unsigned char volatile xdata SINO_TKRAM_47;
extern unsigned char volatile xdata SINO_TKRAM_48;
//#message "XRAM π”√≥¨1280,«Îº”‘ÿSH79F328A_XDATA.H"
#endif

