/*********************************************************
文 件 名 : Buzzer.C
文件说明 : 蜂鸣器事件处理
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/
//头文件
#include "COMMON\AllInclude.h"
#include "Buzzer\Buzzer_C.h"


/*****************************************************
*函数名称：void F_BuzzerOn(void)
*函数功能：蜂鸣器打开
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
static void F_BuzzerOn(void)
{

#if (DEF_BUZZER_RUNTYPE == C_PWM_DRIVE)
	GPIO_Init(GPIO4, GPIO_PIN_1,GPIO_MODE_OUT_PP);	
	PWM_Cmd(ENABLE);
#else

	if (g_u8BuzzerTimerDrive == 0)
	{
//		ADC_ChannelConfig(ADC_CHANNEL_4,DISABLE);
		g_u8BuzzerTimerDrive = 1;
		_SET_IO_BUZZER_PP;
	}

#endif
}

/*****************************************************
*函数名称：void F_BuzzerOFF(void)
*函数功能：蜂鸣器关闭
*输入参数：无
*输出参数：无
*说明：
*****************************************************/
static void F_BuzzerOFF(void)
{
#if (DEF_BUZZER_RUNTYPE == C_PWM_DRIVE)
	PWM_Cmd(DISABLE);
	GPIO_Init(GPIO4, GPIO_PIN_1,GPIO_MODE_IN_HI);
#else
	if (g_u8BuzzerTimerDrive)
	{
//		ADC_ChannelConfig(ADC_CHANNEL_4,ENABLE);
		g_u8BuzzerTimerDrive = 0;
		_SET_IO_BUZZER_IN;
	}
#endif
}

/*****************************************************
*函数名称：void F_BuzzerInput(u8 l_u8BeepCnt,u8 l_u8ContinueTime, u8 l_u8IntervalTime)
*函数功能：蜂鸣器缓存输入
*输入参数：
l_u8BeepCnt 			鸣响次数
l_u8ContinueTime 		单次鸣响持续时间:输入1代表100ms
l_u8IntervalTime		鸣响间隔时间:输入1代表100ms
*说明：旧数据会马上被新数据覆盖
*****************************************************/
void F_BuzzerInput(u8 l_u8BeepCnt,u8 l_u8ContinueTime, u8 l_u8IntervalTime)
{
	BUZ.u8Beep100msCnt = 0;
	BUZ.u8BeepCnt = l_u8BeepCnt;
	BUZ.u8ContinueTime = l_u8ContinueTime;
	if (l_u8BeepCnt == 1)
	{
		BUZ.u8IntervalTime = 0;	
	}
	else
	{
		BUZ.u8IntervalTime = l_u8IntervalTime;
	}	
}


/*****************************************************
*函数名称：u8 F_Buzzer_Buffer(void)
*函数功能：蜂鸣器缓存处理
*调用周期：100ms
*调用资源：BUZ.u8BeepCnt
		   BUZ.u8ContinueTime
		   BUZ.u8IntervalTime
		   BUZ.u8Beep100msCnt

*返回值：  l_u8BeepOutBuff
		   高4位 = 0-------------驱动未完成
		   高4位 = 1-------------驱动完成
		   低4位 = 0-------------蜂鸣器关闭
		   低4位 = 1-------------蜂鸣器开启
*说明：
*****************************************************/
static u8 F_Buzzer_Buffer(void)
{  
	u8	xdata l_u8BeepOutBuff= 0;

    if (BUZ.u8BeepCnt)
	{
		//单周期计算
		if (++BUZ.u8Beep100msCnt > (BUZ.u8ContinueTime + BUZ.u8IntervalTime))
		{
			BUZ.u8Beep100msCnt = 0;
			if (BUZ.u8BeepCnt > 0)
			{
				BUZ.u8BeepCnt--;
			}
		}

		//最后一个周期，蜂鸣器开完马上结束
		if ((BUZ.u8BeepCnt == 1) && (BUZ.u8Beep100msCnt > BUZ.u8ContinueTime))
		{
			BUZ.u8BeepCnt--;
		}

		//输出驱动标志
		if (BUZ.u8Beep100msCnt <= BUZ.u8ContinueTime)
		{   
			if (BUZ.u8BeepCnt) 
			{
				l_u8BeepOutBuff |= C_BUZZER_ON;
			}
			else
			{
				l_u8BeepOutBuff |= C_BUZZER_OFF;			
			}
		}
		else
		{
			l_u8BeepOutBuff |= C_BUZZER_OFF;
		}
	}
	else
	{
		BUZ.u8Beep100msCnt = 0;
		l_u8BeepOutBuff = C_BUZZER_DONE;
	}

	return l_u8BeepOutBuff;
}

/*****************************************************
*函数名称：void F_Buzzer_Handle(void)
*函数功能：蜂鸣器处理
*输入参数：无
*输出参数：无
*调用周期：100ms
*说明：
*****************************************************/
void F_Buzzer_Handle(void)
{
	u8	xdata l_u8BuzzerOut = 0;

	l_u8BuzzerOut = F_Buzzer_Buffer();

	if (_TEST_BUZZER_OUT(l_u8BuzzerOut))
	{
		F_BuzzerOn();
	}
	else
	{
		F_BuzzerOFF();
	}
}



/*****************************************************
*函数名称：void F_DRIVE_Buzzer(void)
*函数功能：蜂鸣器驱动
*输入参数：无
*输出参数：无
*调用周期：125us
*说明：
*****************************************************/

#if (DEF_BUZZER_RUNTYPE == C_TIMER_DRIVE)

void F_DRIVE_Buzzer(void)
{
	static bit s_bBuzzerToggle = 0;

	s_bBuzzerToggle = ~s_bBuzzerToggle;

	if (g_u8BuzzerTimerDrive)
	{
		if (s_bBuzzerToggle)
		{_IO_BUZZER_ON;}
		else
		{_IO_BUZZER_OFF;}
	}
	else
	{
		_IO_BUZZER_OFF;	
	}
}

#endif
