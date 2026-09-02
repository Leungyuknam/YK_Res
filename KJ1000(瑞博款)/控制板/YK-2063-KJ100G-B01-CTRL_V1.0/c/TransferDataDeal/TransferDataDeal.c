#include "h\Allinclude.h"

void F_mTransferDataDeal(void)
{
//    if( F_GetTimeTick(C_TIME_10ms ) )
//	{

//    }
//    F_Uart1DuplexProc();
//    F_CheckCommucate();
    
    F_UartDuplexProc();
    F_StiveCommucate();
	F_Uart1DuplexProc();
    F_WifiUartDuplexProc();
    TuyaWifiProc();
} 