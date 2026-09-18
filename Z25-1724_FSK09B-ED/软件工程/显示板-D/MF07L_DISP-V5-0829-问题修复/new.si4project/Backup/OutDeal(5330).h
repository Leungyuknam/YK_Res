#ifndef _OUTDEAL_H_    
#define _OUTDEAL_H_

#ifdef _Native_OutDeal_H
	#define	EXT_OUT_DEAL
#else
   #define	EXT_OUT_DEAL extern	
#endif

typedef union {
	struct {
		unsigned char TopHeat       : 1;
		unsigned char BotHeat       : 1;
		unsigned char PumpA        : 1;
		unsigned char PumpB        : 1;
		unsigned char PlateHeatX   : 1;
		unsigned char PlateHeatY   : 1;
		unsigned char SleepCommand : 2;
	} Bits;
	unsigned char Byte;
} SysLoad_t;



#ifdef _Native_OutDeal_H
	EXT_OUT_DEAL X_REG SysLoad_t SysLoadBuf = {0};
	EXT_OUT_DEAL X_REG SysLoad_t SysLoad = {0};
	EXT_OUT_DEAL u8 X_REG u8ShowBuf0 = 0;
	
	EXT_OUT_DEAL u8 B_FstHeatCheck = FALSE;

#else
	//EXT_OUT_DEAL X_REG u16 g_u16TimeToClose_Sec;
	EXT_OUT_DEAL X_REG SysLoad_t SysLoadBuf;
	EXT_OUT_DEAL X_REG SysLoad_t SysLoad;
	EXT_OUT_DEAL u8 X_REG u8ShowBuf0;
	EXT_OUT_DEAL u8 B_FstHeatCheck;
#endif



EXT_OUT_DEAL void OutPutDeal(void);
EXT_OUT_DEAL void FactoryOutPutDeal(void);






#endif

