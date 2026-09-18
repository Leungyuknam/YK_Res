/*********************************************************
文 件 名 : SELFCHECK.H
文件说明 : 自检头文件
编写作者 : thm
修订作者 : 
编写时间 : 2021-2-3
版 本 号 : V1.0
==========================================================*/
#ifndef _SELFCHECK_H_
#define _SELFCHECK_H_ 

#define 	C_MAX_STATE		 2

//外部变量
extern u8  xdata  g_u8SelfCheckFlag;
extern u8  xdata  g_u8ForbidSaveData;
 
//函数声明
extern void F_IntoSelfCheck(void);
extern void F_ManualCheck(void);
extern void F_SELFCHECK_Display(void);
extern void F_SELFCHECK_Output(void);
extern void F_BackOutSelfCheck(void);
extern void F_DispToggle(void);
      
#endif