#include "h\Allinclude.h"

void F_mTransferDataDeal(void)
{
    if( F_GetTimeTick(C_TIME_10ms ) )
	{
        fgWifi10msTimbase = 1;
    }
//    F_Uart1DuplexProc();
//    F_CheckCommucate();
//    
//    F_UartDuplexProc();
//    F_StiveCommucate();
} 