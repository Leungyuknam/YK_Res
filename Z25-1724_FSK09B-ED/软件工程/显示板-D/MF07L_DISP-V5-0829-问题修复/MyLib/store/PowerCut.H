#ifndef POWERCUT_H					// Judge the "DisDrv_h" has been defined or not,make sure this file only include once 
#define POWERCUT_H					// define "MCU_8513_H" 
#ifdef	EXTR				        	//                                                                               
#undef	EXTR                        	// undefine "EXTR"                                                               
#endif
						                                                                                         
#ifdef	POWERCUT_C			    		// redefine "EXTR",make sure EXTR defined according to the constant of "main".   
#define EXTR                                                                                                         
#else						                                                                                         
#define EXTR	extern                                                                                               
#endif 

#define	c_SAVE_DELAY_TIME		20



EXTR void F_PowerCutOffProc(void);
EXTR void F_ReadFlashData(void);
//EXTR void EppromProc(void);


#endif

