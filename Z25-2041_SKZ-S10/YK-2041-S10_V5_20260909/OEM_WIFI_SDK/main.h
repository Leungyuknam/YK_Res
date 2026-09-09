#ifndef Main_h
#define Main_h
#ifdef	EXTR
#undef	EXTR 
#endif 

#ifdef	Main
#define EXTR
#else 
#define EXTR extern 
#endif                  
/************************************************************
以下变量是用户接口函数用的，用户根据自己代码修改，
例如，fgHumAIMode指恒湿功能，用户定义的恒湿标志替代此标志
************************************************************/
EXTR	ram_field unFlagKey1;  
#define fgTimeToClose	unFlagKey1.ram_bit.bit_0		
#define fgTurnOn		unFlagKey1.ram_bit.bit_1
#define fgAnionEn		unFlagKey1.ram_bit.bit_2
#define fgBrightStren	unFlagKey1.ram_bit.bit_3
#define fgBuzEn		unFlagKey1.ram_bit.bit_4
#define fgFanDry		unFlagKey1.ram_bit.bit_5
#define fgTimeToOpen	unFlagKey1.ram_bit.bit_6
#define fgScreenSaver	unFlagKey1.ram_bit.bit_7
/************************************************************
以下变量是用户接口函数用的，用户根据自己代码修改，
例如，ucHumiditySetGrade指湿度档位，用户根据自己代码修改。
************************************************************/
EXTR uint  uiT_TimeToClose;//定时关机时间
EXTR uint  uiT_TimeToOpen;//预约开机时间

EXTR uint  uiFanDryTimSet;//风干时间设置
EXTR uchar ucErrorCodeType;//故障类型变量，参见WifiAPI.h定义
EXTR uchar ucTurnOnOffEn;//0:关机，1:开机
EXTR uchar ucFanDryEn;//风干功能设置，0:关闭风干功能，1:开启风干功能
EXTR uchar ucHumiditySet;//湿度设置,恒湿模式默认60，其他根据情况赋值上传
EXTR uchar ucAnionEn;//净离子负载控制，1:开启净离子负载，0:关闭净离子负载
EXTR uchar ucBuzEn;//蜂鸣器使能变量，1:打开蜂鸣器提示音，0:关闭蜂鸣器提示音
EXTR uchar ucHumModeSet;//模式设置，参见WifiAPI.h定义
EXTR uchar ucBrightStrenSet;//氛围灯设置，0:关闭氛围灯，1:开启氛围灯
EXTR uchar ucScreenSaverGrade;//屏显档位变量
EXTR uchar ucHumFanSpeedSet;//手动模式风机设置档位，参见WifiAPI.h定义
EXTR uchar ucHumFanSpeedRun;//风机运行档位:手动模式运行档位=设定档位，睡眠模式运行档位=U8_S_SPEED，
                //AI模式运行档位随工作区间变化在U8_L_SPEED、U8_M_SPEED、U8_H_SPEED之间选择
                              
EXTR uchar ucWaterStateCheck;//水量状态检测
EXTR uchar ucScreenSaverEn;//熄屏设置，0:开启熄屏，1:亮屏(取消熄屏)
EXTR uchar ucScreenSaverTimSet;//开启熄屏情况下，睡眠模式5秒其他60秒自动熄屏
EXTR uchar ucCheckWater;//水量状态变量 0x00：缺水,0x01~0x63：1－100当前水的状态百分比  0x64:水满
EXTR uchar FanDryTim;//风干时间

EXTR void UART0_Init(uint rate);
#endif  
/************************************************************
					END	        
************************************************************/
