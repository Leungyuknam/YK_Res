#ifndef _COMMUCATE_H_
#define _COMMUCATE_H_

extern U8 xdata g_u8UartMissCnt;
extern bit g_bUartErrFlg;  //通讯错误标记

extern U8 xdata g_u8DispVersion;//显示板版本

extern U8 MakeEnCode();
extern void DeCodeProc(U8 l_u8ByteNum);

extern void F_CheckCommucate(void);

#endif

