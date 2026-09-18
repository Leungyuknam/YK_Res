#include "h\Allinclude.h"

void F_mTransferDataDeal(void)
{
    if( F_GetTimeTick(C_TIME_10ms ) )
	{
        //红外数据接收处理
        //外部通讯数据处理
        F_IrDataProc();
    }
//    UartDuplexProc();
} 