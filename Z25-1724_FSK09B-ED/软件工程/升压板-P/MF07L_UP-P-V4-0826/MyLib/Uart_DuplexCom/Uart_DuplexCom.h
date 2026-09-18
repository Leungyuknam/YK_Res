#ifndef _COMMUCATE_H_
#define _COMMUCATE_H_


extern U8 xdata ControlDisConnect_Cnt;




#define	DIS_WORK_TIME  5


extern U8 MakeEnCode();
extern void DeCodeProc(U8 l_u8ByteNum);
extern void F_CheckCommucate(void);



//0825
extern U8 MakeEnCode2();
extern void DeCode2Proc(U8 l_u8ByteNum);
extern void F_CheckCommucate2(void);

extern U8 xdata ControlDisConnect2_Cnt;

#endif

