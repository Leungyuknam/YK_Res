#ifndef __IRCFG_H__
#define __IRCFG_H__

typedef struct 
{
	U8 KeyCode;
	void (*VoidIrKeyEvent)();

}StrIrCfg;

//---------------------配置部分---------------------------------
#define IrRecPin 					P_IRREC	
#define c_ClientCode 				0x01	//遥控客户码，8位，不包含反码

#define c_IrShortKeyNum				8		//短键数量
#define c_IrShortKeyMode			0		//短键模式，0 = 按下响应，1= 松开响应（用于区分长短键）		

#define c_IrLongKeyNum				0		//长键数量
#define c_IrLongKeyFirstTime  		14		//长按键首次生效时间，单位110ms
#define c_IrLongKeyContinueTime 	3		//长按键连续生效时间，单位110ms

#define	IR_ID_HEAD_L				0x06
#define	IR_ID_HEAD_H				0xA3


//遥控命令码
//0xE0  0x00
//    0x20
//0x80  0xA0
//   0x60
//0xC0  0x40

//按键码对应的执行函数
code StrIrCfg g_sShortKeyToEvent[c_IrShortKeyNum]	=
{	
	{0x40,&F_SetTimeOff},	
	{0xC0,&F_SetTimeOn},	
	{0x60,&F_WingLevelDec},	
	{0xA0,&F_UpDnOscKey},	
	{0x80,&F_RLOSC_AngleKey},	
	{0x20,&F_WingLevelInc},	
	{0x00,&F_WingIR},	//
	{0xE0,&KeyOFFON},		


};

#if c_IrLongKeyNum
code StrIrCfg g_sLongKeyToEvent[c_IrLongKeyNum] =
{
	{0x9c,&KeyOFFON},
	
};
#endif
//----------------------配置结束----------------------------------
#endif

