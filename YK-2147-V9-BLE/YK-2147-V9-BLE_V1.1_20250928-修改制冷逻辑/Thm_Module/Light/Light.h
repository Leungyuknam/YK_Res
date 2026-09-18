/*********************************************************
文 件 名 : LIGHT.H
文件说明 :
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-6
版 本 号 : V1.0
==========================================================*/
#ifndef _LIGHT_H_
#define _LIGHT_H_ 

//事件时间段定义（单位：秒）
#define     C_LIGHT_ONTIME_SET      180		//亮灯持续时间

//灯光缓存操作
//#define     C_LIGHT_OFF            0
//#define     C_LIGHT_ON             1
//#define     _TEST_ALL_LIGHT         _BTS(g_u8LightSetBuff,C_ALL_LIGHT_)
//#define     _LIGHT_ALL_ON           _BS(g_u8LightSetBuff,C_ALL_LIGHT_)
//#define     _LIGHT_ALL_OFF          _BC(g_u8LightSetBuff,C_ALL_LIGHT_)
//#define     _TEST_L_LIGHT           _BTS(g_u8LightSetBuff,C_L_LIGHT_)
//#define     _L_LIGHT_ON             _BS(g_u8LightSetBuff,C_L_LIGHT_)
//#define     _L_LIGHT_OFF            _BC(g_u8LightSetBuff,C_L_LIGHT_)
//#define     _TEST_R_LIGHT           _BTS(g_u8LightSetBuff,C_R_LIGHT_)
//#define     _R_LIGHT_ON             _BS(g_u8LightSetBuff,C_R_LIGHT_)
//#define     _R_LIGHT_OFF            _BC(g_u8LightSetBuff,C_R_LIGHT_)

//灯光事件定义
typedef enum 
{
	C_LIGHT_KEYACK_EVENT = 1,	           //灯光键按下触发
	C_LIGHT_TIMEUP_EVENT = 2,	           //定时到达触发
//	C_LIGHT_LEFT_ON_EVENT = 3,	           //左箱制冷触发
//	C_LIGHT_ALL_ON_EVENT = 4,	           //左右箱同时制冷触发
//	C_LIGHT_ALL_OFF_EVENT = 5	           //停止制冷触发
}
LIGHT_EVENT_TypeDef;
              
//IO口驱动
#define     _IO_R_LIGHT_ON			Set_P22
#define     _IO_R_LIGHT_OFF			Clr_P22
#define     _IO_L_LIGHT_ON			Set_P23
#define     _IO_L_LIGHT_OFF			Clr_P23

#define     _IO_LIGHT_ON			P2 |= 0x0C
#define     _IO_LIGHT_OFF			P2 &= ~0x0C

//外部声明
extern u8	g_u8LightSetBuff;

extern void F_BUFF_LightSetBuff(LIGHT_EVENT_TypeDef l_u8LightEvent);   //需要处理g_u8LightSetBuff的，调用该函数
extern void F_TIME_Light(void);
extern void F_DRIVE_Light(void);
             
#endif