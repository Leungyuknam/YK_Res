#ifndef _MCUINIT_H__
#define _MCUINIT_H__
/*****************************************
IC型号选择
*****************************************/

enum {
    PIN_GPIO,                   //IO设置成模拟功能
    PIN_ANALOG,                 //IO设置成模拟功能
    PIN_01,                    	//未定义
    PIN_02,                     //未定义
    PIN_CC0                     //Timer2 比较输出通道 
};


extern void F_McuInit(void);
extern void SystemInit(void);
#endif

