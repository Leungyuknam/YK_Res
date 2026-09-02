
#ifndef _FuncQueueB_H_
#define _FuncQueueB_H_

#ifdef	EXTR
#undef	EXTR                                                                        
#endif
						                                                                                         
#ifdef	_FuncQueueB_C_
#define EXTR                                                                                                         
#else						                                                                                         
#define EXTR	extern                                                                                               
#endif 

#define NULL_FUNC		0

//typedef unsigned char (*PFun)(unsigned int);

EXTR void ClearFuncB(PFun Fun);
EXTR void AddFuncB(PFun Fun,unsigned int ParaB, unsigned char ReSendTimes);
EXTR void InsertFuncB(PFun Fun,unsigned int ParaB, unsigned char ReSendTimes);
EXTR unsigned char CallArrayFuncB(void);
EXTR void InitArraryFuncB(void);

#endif
