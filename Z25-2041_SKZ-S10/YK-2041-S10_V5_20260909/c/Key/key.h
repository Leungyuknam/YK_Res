#ifndef _KEY_H_
#define _KEY_H_

#define C_NO_KEY_TIME_MAX 7210
//模块使用指南
//  1：在主程序的10MS片段中调用 F_KeyDealControl（）函数
//  2: 根据F_KeyDealControl（）调用的频率配置好 #define C_TimeStandard 定义
//  3: 根据按键组合将每个按键的扫描码配置好，如下在key.c文件中
//       ----------下面是按键的码值-----------------------------
//       #define K_C_OFF        (1<<7)
//       #define K_C_MODE      (1<<2)
//       #define K_C_SPEED       (1<<1)
//       #define K_C_SET_TIME   (1<<3) //P02
//       #define K_C_MARK       K_C_SPEED|K_C_MODE
//  4:根据需要将配置表格U16 code KeyCodeTable[][4] 填写好
//    对应的Fan code KeyEventDealTable[] 也填写好
//    配置表格各个单元意义如下
//    按键码        按键相应时间   重复响应否	 后续再次响应时间


/*
     例如，如果如下配置框架动作解释
	U16 code KeyCodeTable[][4] = {
	//按键码        按键相应时间   重复响应否	 后续再次响应时间
	 {K_C_OFF,	     C_KEY_50MS,       0	   ,   0       },	  //1
	 {K_C_MODE,	     C_KEY_50MS,	   0	   ,   0       },	  //2
	 {K_C_LOP,       C_Key_1S,         1       ,   C_KEY_50MS }
	 {0,0,0,0},
	};	
	Fan code KeyEventDealTable[] = {
		&F_OffKey,     //1
		&F_ModeKey,    //2 
		&F_LopKey,     //3
	}
     如果KeyScan函数返回的数据是 K_C_OFF，并且时间持续了50MS,系统会
	 主动调用F_OffKey() 这个函数。
	 
	 如果KeyScan函数返回的数据是 K_C_MODE, 并且持续时间是50MS,系统会
	 主动调用F_Modekey() 这个函数。

     如果KeyScan函数返回的数据是 K_C_LOP, 第一响应是持续1秒钟的时候，后面
	 会每隔50MS执行一次 F_LopKey() 这个函数。
	 可以利用 F_GetKeyKeepingTime（）函数的返回值判断此时是第一次响应还是
	 后面的连续响应。

*/
/*  
     5：抬起响应的时候配置同按下响应的一致
		U16 code KeyReleseCodeTable[][3] = {
		  //按键码         抬起最小时间  抬起最大时间
		   {K_C_SET_TIME,  C_KEY_50MS,    C_KEY_1S},
		   {K_C_SPEED,     C_KEY_50MS,    C_KEY_1S},
		   {0,0,0} 
		};
		U8 code ReleseKeyEventTable[] = {
		   &F_SetAlarmTimeKey,
		   &F_SpeedKey, 
		};
	  6：实现好ReleseKeyEventTable[] 
	        和 KeyEventDealTable[] 表格中的函数
	  7:注意KeyCodeTable，KeyReleseCodeTable，要以0元素做为结尾。
*/ 

extern void F_KeyDealControl();  //周期性调用
extern U32 F_KeyScan(); //需要你自己编写的函数，此函数返回32位的按键码此函数模块已自动调用，不需要你调用执行。
//extern U16 F_GetKeyKeepingTime();//函数返回按键持续时间计数器的值。主要用来判断单按键还是连续按键。
//extern U16 F_GetNoKeyTime();//获取没有按键的时间计数。
extern void F_mKeyControl();
 
#endif