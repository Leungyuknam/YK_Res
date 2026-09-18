//*************************************************************
// HP12-HAR 主循环
//*************************************************************
#include "h\Allinclude.h"

void main(void)
{
    cli();
    F_mCpuInital();
    TouchKeyInit();
//    F_UartInit();
    F_mSystemInital();
    F_MemoryRead();// 使用童锁掉电记忆时，在此读取 EEPROM 数据
    F_mDispInital();
    BUZ_POWERON();
    sei();

    while(1)
    {
        WDT_clear();
        F_mSystemTimeDeal();
//        F_mTransferDataDeal();
        F_mDisplayControl();
        F_mAllSenseDeal();
        F_mKeyControl();
        if(g_u8TestState) 
			F_mSelfTestSchdule();
        else              
			F_mFunctionSchdule();
		
        F_BellControl();
    }
}
