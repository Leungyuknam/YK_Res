#ifndef _OUTDEAL_H_    
#define _OUTDEAL_H_

#ifdef _Native_OutDeal_H
	#define	EXT_OUT_DEAL
#else
   #define	EXT_OUT_DEAL extern	
#endif

typedef union {
	struct {
		u8 Heat 						: 1;		//加热
		u8 Fan							: 1;		//风扇
		u8 Light						: 1;		//指示灯
		u8 WarmHeat 					: 1;		//较低功率加热
		u8 None 						: 4;		//None
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
EXT_OUT_DEAL void CustomerTestOutPutDeal(void);
#endif

