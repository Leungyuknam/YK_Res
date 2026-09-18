#ifndef	_Out_H
#define	_Out_H

#ifdef _Native_Out_H
   #define	EXT_OUT 
#else
   #define	EXT_OUT extern	
#endif
   
#include "Include\Include.h"


typedef enum				
{	
	BAROMETRIC_LOW_N,
	BAROMETRIC_LOW_Y
}BarometricLow_Enum;

typedef enum				
{	
	SW_CLOSE,
	SW_OPEN
}SW_Status_Enum;





typedef enum				
{	
	BAT_NORMAL,
	BAT_RECHARGING
}BatteryStatus_Enum;

typedef enum				
{	
	BAT_VOL_NOR,
	BAT_VOL_LOW
}BatteryEn_Enum;

typedef enum				
{
	EN_OFF,
	EN_ON
}Out_Enum;

#ifdef _Native_Out_H
	EXT_OUT X_REG Out_Enum mHeatX_OutEn = EN_OFF;
	EXT_OUT X_REG Out_Enum mHeatY_OutEn = EN_OFF;

#else

	EXT_OUT X_REG Out_Enum mHeatX_OutEn;
	EXT_OUT X_REG Out_Enum mHeatY_OutEn;

#endif


EXT_OUT void HeatX_Output(Out_Enum mOutputEn);
EXT_OUT void HeatY_Output(Out_Enum mOutputEn);




#endif
