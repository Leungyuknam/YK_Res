

#ifndef __ALLINCLUDE_H_
#define __ALLINCLUDE_H_

//#define		_DEMO_TEST_			//底层检测
//#define		_SHOW_TEMP_AD_		//显示AD与温度
//#define	 _RUN_HEAT_TEST_			//显示烧的温度曲线步骤
//#define	_RUN_HEAT_DOOR_DIS			//无论箱门状态，加热可运行


//#define _TO_CUSTOM_LIMIT_
//#define	_TEST_DEBUG_
//#define _TEST_CAP_AD_   //显示电容AD 或者1/4 VDD 的AD
//#define _ERR_IGNORE_

//#define	_FAST_TEST_

//#define __Uart_DEBUG__


#define HEAT_M_ONE		0
#define HEAT_M_TWO		1

#define HEAT_MODE		HEAT_M_ONE

#define	MAIN_10MS_CNT			10
#define	MAIN_100MS_CNT			100

#include "h\SC92F841X_C.h"

#include "Include\Typedef.h"
#include "MyLib\Common\Common.h"
#include "MyLib\Interrupt\Interrupt.h"
#include "MyLib\McuInit\McuInit.h"
#include "MyLib\Function\Function.h"
#include "MyLib\KeyEvent\KeyEvent.h"
#include "MyLib\Key\Key.h"
#include "MyLib\Display\Display.h"
//#include "MyLib\Communicate\Communicate.h"
//#include "MyLib\Uart_Analog\Uart_Analog.h"
#include "MyLib\DispDriveIO\DispDriveIO.h"
#include "MyLib\beep\beep.h"
#include "MyLib\OutDeal\OutDeal.h"



#include "MyLib\IO.h"

//#include "MyLib\AD\AD.h"


//#define		TPIN1()			P00^=1
#endif
