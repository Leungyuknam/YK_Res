

#ifndef __ALLINCLUDE_H_
#define __ALLINCLUDE_H_


#include "h\SC92F841X_C.h"

#include "Include\Typedef.h"
//#include "c\main.h"

//#include "MyLib\McuInit\McuInit.h"
#include "MyLib\Common\Common.h"

//#include "MyLib\Uart_Analog\Uart_Analog.h"
#include "MyLib\Interrupt\Interrupt.h"
#include "MyLib\McuInit\McuInit.h"
//#include "MyLib\DispDriveIO\DispDriveIO.h"

#include "MyLib\Function\Function.h"
#include "MyLib\beep\beep.h"
#include "MyLib\KeyEvent\KeyEvent.h"
#include "MyLib\Key\Key.h"
#include "MyLib\Display\Display.h"
#include "MyLib\OutDeal\OutDeal.h"


#include "MyLib\Communicate.h"
#include "MyLib\TM1668Drv.h"
#include "MyLib\AD\AD.h"
#include "MyLib\IAP_Init.h"
#include "MyLib\PowerCut.h"

#define	_TEST_DEBUG_
#define	_STONE_EXIST_ 
//#define _TEST_CAP_AD_
//#define _ERR_IGNORE_

//#define _TO_CUSTOM_LIMIT_

#define	MAIN_10MS_CNT			10
#define	MAIN_100MS_CNT			100

#define		TPIN1()			P00^=1
#define		TPIN2()			P01^=1
#endif
