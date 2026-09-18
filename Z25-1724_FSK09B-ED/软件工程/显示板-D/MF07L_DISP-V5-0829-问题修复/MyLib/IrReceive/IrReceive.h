#ifndef __IRRECEIVE_H__
#define __IRRECEIVE_H__

typedef enum {c_IrNokey,c_IrShortKey,c_IrLongKey} EnumIr;
/*
//108//13.5MS
#define c_LeaderPulseMin	75		
#define c_LeaderPulseMax	141		

//92//11.5MS
#define c_LongKeyPulseMin	64		
#define c_LongKeyPulseMax	120		

//18//2.25MS
#define c_Data1PulseMin		13	
#define c_Data1PulseMax		24	

//8.96//1.12MS
#define c_Data0PulseMin		6	
#define c_Data0PulseMax		12
*/

//38.5//4.824MS
#define c_LeaderPulseMin	27		
#define c_LeaderPulseMax	50		

//12.6//1.578MS  无
#define c_LongKeyPulseMin	8		
#define c_LongKeyPulseMax	17		

//13.1//1.646MS
#define c_Data1PulseMin		9
#define c_Data1PulseMax		17	

//6.4//0.8MS
#define c_Data0PulseMin		4	
#define c_Data0PulseMax		8


//880//110MS
#define c_KeyWaitTime	1144



typedef union {
    unsigned long Dword;		//51单片机数据存放方式为大端排序
    struct {
        unsigned char CmdReverse;
        unsigned char Cmd;
        unsigned char ClientCodeReverse;
        unsigned char ClientCode; 
    }Bytes;
}StrIrData;



extern StrIrData g_sIrRecData;


void F_IrReceive();			//放125us中断
void F_IrDataProc();		//放10ms时间片


#endif

