#ifndef _IAP_INIT_H_
#define _IAP_INIT_H_

#include "cpu.h"

extern UCHAR ssp_flag;

extern UCHAR EEPromByteRead(UCHAR nAddrH,UCHAR nAddrL); 
extern void EEPromByteProgram(UCHAR nAddrH,UCHAR nAddrL,UCHAR nData);
extern void EEPromSectorErase(UCHAR nAddrH);

#endif  
