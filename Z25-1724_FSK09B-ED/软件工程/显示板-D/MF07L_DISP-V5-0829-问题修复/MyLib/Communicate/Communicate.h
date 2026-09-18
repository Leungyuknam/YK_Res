#ifndef Communication_h
#define Communication_h


extern unsigned char xdata u8DatSame_Cnt;
extern unsigned char xdata UartMissCnt;
extern unsigned char xdata ControlDisConnect_Cnt; 

extern unsigned char DeCodeProc(unsigned char UartBuf[], unsigned char Length);
extern unsigned char MakeEnCode(unsigned char UartBuf[]);

extern void SendVerDeal();
#endif
