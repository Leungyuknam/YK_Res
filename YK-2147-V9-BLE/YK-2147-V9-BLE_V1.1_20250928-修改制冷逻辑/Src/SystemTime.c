/*********************************************************
文 件 名 : SystemTime.c
文件说明 : 系统时间计算，各时间标记建立所在地
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/
//头文件
#include "COMMON\AllInclude.h"

//全局变量声明
static	u8		g_u8BaseTimeFlagBuff = 0;
static  u8		g_u8Sys50msCnt = 0;
static  u8		g_u8Sys100msCnt = 0;
static  u8		g_Sys500msCnt = 0;
static  u8		g_Sys1sCnt = 0;
static  u8		g_Sys3sCnt = 0;
static  u8		g_Sys5sCnt = 0;


static  bit 	s_bLastIoStatus = 0;
static  bit 	s_bNowIoStatus = 0;
static  bit 	s_bStarCountFlag = 0;
static  u16 	s_u16FlashCnt = 0;
static  u16 	s_u16RunTimeCnt = 0;
static  u16 	s_u16LastCntValue = 0;
static  u16 	s_u16NormalCnt = 0;

		bit		g_bFlash1HzFlag = 0;
		bit		g_bFlash2HzFlag = 0;
		u8		g_u8SysTimeBuff = 0;


/*****************************************************
*函数名称：void F_Delay_us(u8 u8InputDelayTime)
*函数功能：微秒级延时函数
*入口参数：u8InputDelayTime
*出口参数：void
*说明：仅做粗略延时用
*****************************************************/
void F_Delay_Us(u8 u8InputDelayTime)
{
    u8  l_u8UsCnt = 0;
    u8  l_u8Ns125Cnt = 0;
    
    for (l_u8UsCnt = 0; l_u8UsCnt < u8InputDelayTime; l_u8UsCnt++)
    {
        for (l_u8Ns125Cnt = 0; l_u8Ns125Cnt < 2; l_u8Ns125Cnt++)
        {}
    }
}


/*****************************************************
*函数名称：void F_SYSTEM_Time(void)
*函数功能：系统时间标志处理
*输入参数：无
*输出参数：无
*调用周期：
*说明：
*****************************************************/
void F_SYSTEM_Time(void)
{
	g_u8SysTimeBuff = 0;

	if (_BTS(g_u8BaseTimeFlagBuff,C_BASETIME_10MS_))
	{
		g_u8BaseTimeFlagBuff = 0;

		//10ms标志位
		_BS(g_u8SysTimeBuff,C_SYS_10MS_);

		//50ms标志位
		if (++g_u8Sys50msCnt >= C_SET_50MS)
		{
			g_u8Sys50msCnt = 0;
			_BS(g_u8SysTimeBuff,C_SYS_50MS_);
		}

		if (++g_u8Sys100msCnt >= C_SET_100MS)
		{
			//100ms标志位
			g_u8Sys100msCnt = 0;
			_BS(g_u8SysTimeBuff,C_SYS_100MS_);

			//500ms标志位
			if (++g_Sys500msCnt >= C_SET_500MS)
			{
				g_Sys500msCnt = 0;
				_BS(g_u8SysTimeBuff,C_SYS_500MS_);

				//1Hz标志位
				g_bFlash1HzFlag = ~g_bFlash1HzFlag;
			}

			//1s标志位
			if (++g_Sys1sCnt >= C_SET_1S)
			{	
				g_Sys1sCnt = 0;
				_BS(g_u8SysTimeBuff,C_SYS_1S_);

				//2Hz标志位
				g_bFlash2HzFlag = ~g_bFlash2HzFlag;
			}
			
			//3s标志位
			if (++g_Sys3sCnt >= C_SET_3S)
			{
				g_Sys3sCnt = 0;
				_BS(g_u8SysTimeBuff,C_SYS_3S_);
			}

			//5s标志位
			if (++g_Sys5sCnt >= C_SET_5S)
			{
				g_Sys5sCnt = 0;
				_BS(g_u8SysTimeBuff,C_SYS_5S_);
			}
		}							
	}	
}


/*****************************************************
*函数名称：void F_TIME_Event(void)
*函数功能：时间事件处理
*输入参数：无
*输出参数：无
*调用周期：
*说明：
*****************************************************/
void F_TIME_Event(void)
{

	if (TEST_10MS_FLAG)
	{
		#if USE_BLUETOOTH

		F_UART_ReceiveTimeout();
		F_UART_SendDelay();

		#endif
	}

	if (TEST_100MS_FLAG)
	{
		F_TIME_PrepareOff();
		F_TIME_ModeSet();
		F_TIME_SystemRun();
	}

	if (TEST_1S_FLAG)
	{	
		F_TIME_Compressor();
		F_SELF_Check();
	}
}


/*****************************************************
*函数名称：void F_INT0_Interrupt(void)
*函数功能：定时器0中断处理
*说明：	   中断时间125uS
*****************************************************/
void F_INT0_Interrupt(void)		interrupt 1				
{
//	_BN(P1,3);

	#if def_Flag1ms
	static	u8		s_u8Base1msCnt = 0;
	#endif
	
	#if def_Flag2ms
	static	u8		s_u8Base2msCnt = 0;
	#endif
	
	#if def_Flag3ms
	static	u8		s_u8Base3msCnt = 0;
	#endif
	
	#if def_Flag4ms
	static	u8		s_u8Base4msCnt = 0;
	#endif
	
	#if def_Flag5ms
	static	u8		s_u8Base5msCnt = 0;
	#endif
	
	#if def_Flag10ms
	static	u8		s_u8Base10msCnt = 0;
	#endif


	TCON &= 0XDF;		//clear Interrupt flag

/**********时基处理**********/	
	g_u8BaseTimeFlagBuff = 0;

#if def_Flag1ms
	if (++s_u8Base1msCnt >= 8)
	{
		s_u8Base1msCnt = 0;
		_BS(g_u8BaseTimeFlagBuff,C_BASETIME_1MS_);
	}
#endif

#if def_Flag2ms
	if (++s_u8Base2msCnt >= 16)
	{
		s_u8Base2msCnt = 0;
		_BS(g_u8BaseTimeFlagBuff,C_BASETIME_2MS_);
	}
#endif

#if def_Flag3ms
	if (++s_u8Base3msCnt >= 24)
	{
		s_u8Base3msCnt = 0;
		_BS(g_u8BaseTimeFlagBuff,C_BASETIME_3MS_);
	}
#endif

#if def_Flag4ms
	if (++s_u8Base4msCnt >= 32)
	{
		s_u8Base4msCnt = 0;
		_BS(g_u8BaseTimeFlagBuff,C_BASETIME_4MS_);
	}
#endif

#if def_Flag5ms
	if (++s_u8Base5msCnt >= 40)
	{
		s_u8Base5msCnt = 0;
		_BS(g_u8BaseTimeFlagBuff,C_BASETIME_5MS_);
	}
#endif

#if def_Flag10ms
	if (++s_u8Base10msCnt >= 80)
	{
		s_u8Base10msCnt = 0;
		_BS(g_u8BaseTimeFlagBuff,C_BASETIME_10MS_);
	}
#endif


/**********压机报错检测**********/	
   
    //获取IO口状态
    if (_TEST_IO_ERRO)
    {s_bNowIoStatus = 1;}
    else
    {s_bNowIoStatus = 0;}
    
    //IO口电平跳变
    if (s_bLastIoStatus != s_bNowIoStatus)
    {
        if (s_bStarCountFlag == 0)
        {s_bStarCountFlag = 1;}
        
        if (s_bStarCountFlag == 1)
        {s_u16FlashCnt++;}
        
        //相邻两次跳变间隔时间少于50ms，退出报错检测
        if (((s_u16RunTimeCnt - s_u16LastCntValue) < 400) && (s_u16RunTimeCnt != 0))
        {
            s_u16RunTimeCnt = 0;
            s_bStarCountFlag = 0;
            s_u16FlashCnt = 0;        
        }            
        s_u16LastCntValue = s_u16RunTimeCnt;
    }
    s_bLastIoStatus = s_bNowIoStatus;
    
    //3秒倒计时达到后根据闪烁次数判断报错类型
    if (s_bStarCountFlag == 1)
    {
        if (++s_u16RunTimeCnt >= 24000)
        {
            s_u16RunTimeCnt = 0;
            s_bStarCountFlag = 0;
            
            if (s_u16FlashCnt == 2)
            {g_u8CompressorErro = C_ERRO_F1;}
            else if (s_u16FlashCnt == 4)
            {g_u8CompressorErro = C_ERRO_F2;}
            else if (s_u16FlashCnt == 6)
            {g_u8CompressorErro = C_ERRO_F3;}
            else if (s_u16FlashCnt == 8)
            {g_u8CompressorErro = C_ERRO_F4;}
            else if (s_u16FlashCnt == 10)
            {g_u8CompressorErro = C_ERRO_F5;}
            else if (s_u16FlashCnt == 12)
            {g_u8CompressorErro = C_ERRO_F6;}
            else
            {g_u8CompressorErro = C_ERRO_NULL;}

            s_u16FlashCnt = 0;  
        }
		s_u16NormalCnt = 0;
    }
	else
	{
		if (++s_u16NormalCnt >= 24000)
		{
			s_u16NormalCnt = 0;
			g_u8CompressorErro = C_ERRO_NULL;
		}
	}   
}