#ifndef  __COMMUNICATE_H__
#define  __COMMUNICATE_H__

#define c_softUartErrTime	30



extern bit g_bSoftUartErr;//??NG

U8 DeCodeProc(U8 l_u8ByteNum);	
U8 MakeEnCode();		

void F_mTransferDataDeal();


#endif


