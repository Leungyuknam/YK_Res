#ifndef TM1668_H
#define TM1668_H

#ifdef	EXTR
#undef	EXTR                        	// undefine "EXTR"                                                               
#endif
						                                                                                         
#ifdef	TM1668_C			    		// redefine "EXTR",make sure EXTR defined according to the constant of "main".   
#define EXTR                                                                                                         
#else						                                                                                         
#define EXTR	extern                                                                                               
#endif 

#define	TM_1668M_LENGTH		14

//EXTR uint Read1668Key(void);
EXTR void TM1668_Set(uchar pBuf[],uchar BrightLevel);
EXTR void TM1668_SetBright(uchar level);
 
#endif

