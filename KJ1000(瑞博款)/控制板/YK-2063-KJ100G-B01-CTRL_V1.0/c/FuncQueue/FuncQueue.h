
#ifndef FuncQueue_H
#define FuncQueue_H

#ifdef	EXTR
#undef	EXTR                                                                        
#endif
						                                                                                         
#ifdef	FuncQueue_C
#define EXTR                                                                                                         
#else						                                                                                         
#define EXTR	extern                                                                                               
#endif 

#define NULL_FUNC		0

typedef unsigned char (*PFun)(unsigned int);

EXTR void ClearFunc(PFun Fun);
EXTR void AddFunc(PFun Fun,unsigned int Para, unsigned char ReSendTimes);
EXTR void InsertFunc(PFun Fun,unsigned int Para, unsigned char ReSendTimes);
EXTR unsigned char CallArrayFunc(void);
EXTR void InitArraryFunc(void);

#endif
