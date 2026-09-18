/*********************************************************
文 件 名 : AllInclude.H
文件说明 : 系统头文件集合
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/

#ifndef _AllInclude_H
#define _AllInclude_H

#define SC95F8x2x 1

#include "intrins.H"
#include "SC_Init.h"	//MCU初始化头文件，包含固件库所有头文件
//#include "SC_it.h"
		 
//#include "cms80f231x.h"
//#include "adc.h"
#include "AnalogPwm\AnalogPwm.h"
#include "CarRefrigerator\CarRefrigerator.h"
#include "COMMON\MY_MAC.h"
#include "COMMON\IO_Define.h"
#include "System_Time\SystemTime.h"
#include "ADC\ADC1.h"
			   
//#include "EUSART\EUSART.h"
#include "Mcu_Init\Mcu_Init.h"
#include "Buzzer\Buzzer.h"	  
#include "Celsius_Fahrenheit\Celsius_Fahrenheit.h"		  
#include "DigitalLedDisplay\DigitalLedDisplay.h"
#include "Display\Display.h"
#include "Key\KeyEvent.h"							
#include "Key\Key.h"
#include "Memory\Memory.h"
#include "SelfCheck\SelfCheck.h"
#include "CompressorErroTest\CompressorErroTest.h"		 
#include "SC95F_Touch\SensorMethod.h"
#include "ADC\AdEven.h"
			 		 
#include "PowerCalculate\PowerCalculate.h"
#include "Uart_Analog\Uart_Analog.h"

#include "APP\APP.h"
//#include "GPIO_Output\GPIO_Output.h"
#include "Display_Drive\Display_Drive.h"
#include "uart\uart.h"
#include "uart\uart_App.h"
#include "uart1\uart_1.h"
#include "uart1\uart_App_1.h"	
//#include "SOC_DebugTouchKey_Lib\SOC_DebugTouchKey.h"		  
//#include "TM1621\TM1621.h"

			  
#endif