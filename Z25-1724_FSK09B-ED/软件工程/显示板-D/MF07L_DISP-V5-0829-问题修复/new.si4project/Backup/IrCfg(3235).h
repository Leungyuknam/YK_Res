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

#define	IR_ID_HEAD_L				0x10
#define	IR_ID_HEAD_H				0xCF


//遥控命令码
//0x0c	0x08
//0x07	0x06
//0x1C  0x15
//0x03	0x05
//按键码对应的执行函数
code StrIrCfg g_sShortKeyToEvent[c_IrShortKeyNum]	=
{	
	{0x0c,&KeyOFFON},		


};

#if c_IrLongKeyNum
code StrIrCfg g_sLongKeyToEvent[c_IrLongKeyNum] =
{
	{0x9c,&KeyOFFON},
	
};
#endif
//----------------------配置结束----------------------------------
#endif

