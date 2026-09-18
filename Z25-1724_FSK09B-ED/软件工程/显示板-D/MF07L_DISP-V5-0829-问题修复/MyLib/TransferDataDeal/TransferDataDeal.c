#include "Include\Include.h"
void F_mTransferDataDeal(void){
    if (GetTimeTick(TIME_10MS))
	{
        //红外数据接收处理
        //外部通讯数据处理
        F_IrDataProc();
    }
    
} 