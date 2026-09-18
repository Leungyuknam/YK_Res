#ifndef __IRCFG_H__
#define __IRCFG_H__

typedef struct 
{
	U8 KeyCode;
	void (*VoidIrKeyEvent)();

}StrIrCfg;

//---------------------配置部分---------------------------------
#define IrRecPin 					P_REC	
#define c_ClientCode 				0x05	//遥控客户码，8位，不包含反码

#define c_IrShortKeyNum				8		//短键数量
#define c_IrShortKeyMode			0		//短键模式，0 = 按下响应，1= 松开响应（用于区分长短键）		

#define c_IrLongKeyNum				0		//长键数量
#define c_IrLongKeyFirstTime  		14		//长按键首次生效时间，单位110ms
#define c_IrLongKeyContinueTime 	3		//长按键连续生效时间，单位110ms



//按键码对应的执行函数
code StrIrCfg g_sShortKeyToEvent[c_IrShortKeyNum]	=
{	
	{0x01,&F_PowerKey},	  
	{0x04,&F_UpKey},
	{0x06,&F_DownKey},
	{0x03,&F_TimeKey},
	{0x02,&F_OscKey},
	{0x05,&F_ModeKey},
	{0x08,&F_SleepKey},
	{0x07,&F_MuteKey},
//	{0x87,&F_HumiKey},
//	{0x81,&F_IrMuteKey},
//	{0x83,&F_TimeKey},
//	{0x9E,&F_IrLightKey},
//	{0x89,&F_SpeedKey},
};

#if c_IrLongKeyNum
code StrIrCfg g_sLongKeyToEvent[c_IrLongKeyNum] =
{
//	{0x9c,&F_TmrKeyEvent},
	
};
#endif
//----------------------配置结束----------------------------------
#endif

