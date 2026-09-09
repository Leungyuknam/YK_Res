#define  Main 
#include "Define.h"

#define value_time_10ms		80

static RAM_REGION uchar ucR_Time_cycle;
uchar ucTempRoom;
uchar ucHumSampVal;
/*******************************************************
Function Name : S_INT_125us()
Description   : 125us定时中断程序
Input         : null
Return        : null
*******************************************************/
void s_Timer0_ISR(void) interrupt INT_TIMER_125US_VECTOR
{ 
	Push_Context();
	INT_Timer_125us_Reset();
	ucR_Time_cycle ++;
	if(ucR_Time_cycle >= value_time_10ms)
	{
		ucR_Time_cycle = 0;
		fgWifi10msTimbase = 1;	
	}
	Pop_Context();
}
/*******************************************************
Function Name : main()
Description   : 主函数程序入口
Input		: null
Return	: null
*******************************************************/
void main(void)
{
	INT_Disable();
	CodeWriteProc();
	INT_Timer_125us_init();
	S_API_WifiDataInit();
	S_API_WifiCommInit();	
	OTAOkCheck();//OTA相关接口
	INT_Enable();
	while(1)
	{
		WatchDog_Reset();
		S_API_WifiUartDeal();
		S_API_Wifi_main();
		
		//此部分根据实际情况调用对应的函数，放在需要的调用的地方
		S_API_Key_Tuch();//有按键操作时调用
		S_API_AP_Tuch();//触发AP配网时调用
		S_API_WIFI_RST_Tuch();//触发WIFI模组复位时调用
		S_API_CANCEL_WIFI_Tuch();//触发WIFI模组解绑时调用
		S_API_WIFI_FastCheck_Tuch();//触发自检时调用
		S_API_HumFanSpeed_Set(ucHumFanSpeedSet);
		S_API_HumFanSpeed_Run(ucHumFanSpeedRun);
		S_API_HumMode_Set(ucHumModeSet);
		S_API_S_API_OnOff_Set(ucTurnOnOffEn);
		S_API_TimetoOpen_Set(1);//0无预约开机功能，1有预约开机功能
		S_API_TimetoClose_Set(1);//0无定时关机功能，1有定时关机功能
		S_API_TimetoClose_TimSet(uiT_TimeToClose);
		S_API_TimetoOpen_TimSet(uiT_TimeToOpen);
		S_API_Room_Temp(ucTempRoom);//参数变量根据实际需要定义
		S_API_Hum_Temp(ucHumSampVal);//参数变量根据实际需要定义
		S_API_Err_Report(ucErrorCodeType);
		S_API_WifiDisp();
		S_API_Humidity_Set(ucHumiditySet);
		S_API_FanDry_Set(ucFanDryEn);
		//S_API_FanDryTimSet(uiFanDryTimSet);
		//S_API_CheckWater(ucWaterStateCheck);
		S_API_Buz_Set(ucBuzEn);
		S_API_Anion_Set(ucAnionEn);
		S_API_BrightSet();
		S_API_ScreenSaver_Set(ucScreenSaverEn);
		S_API_ScreenSaver_TimSet(ucScreenSaverTimSet);
		OTAProcesingTimeDeal();//OTA相关接口
		//此部分根据实际情况调用对应的函数，放在需要的调用的地方
	}
}
/**********************************************************************
            				 END
**********************************************************************/
