#include "h\Allinclude.h"

/* ========================================== */
/* 全局变量定义                 */
/* ========================================== */
U8  xdata g_u8FanSpeed     = 0;         // 当前风速档位
U8  xdata g_u8AiFanSpeed   = 0;         // AI模式计算的风速
U16 xdata g_u16SenseEnTime = 0;         // 传感器使能时间计数
U8  xdata g_u8AirQuality   = 0;         // 综合空气质量等级
U8  xdata g_u8VoiceCntTime = 0;         // 语音播报间隔计数
U8  xdata g_u8VoiceFilterBeepCnt = 0;
U8  xdata g_u8LocalTimeIsNight = 0;     // 早晚标志位 0:白天 1:夜晚
StrByte LOAD = {0};                     // 负载状态寄存器

// 风机各档位对应的目标转速 (RPM)
// 索引: 0=OFF, 1=SLEEP, 2=LOW, 3=MID, 4=HI
U16 code t_FanSpeed[5] = {0, 300, 500, 650, 800}; 

/*****************************************************************************
* 函数名称：void F_LoadDry(void) 
* 函数功能：硬件负载底层输出执行
* 入口参数：无
* 出口参数：无  
******************************************************************************/
void F_LoadDry()
{
    // 1. 传感器供电控制 (注意引脚逻辑)
    if(g_bSenseLoad)
    {   P_PM25_EN = 0;}
    else
    {   P_PM25_EN = 1;}

    // 2. 负离子发生器控制
    if(g_bIonLoad)
    {   P_ION = 1;}
    else
    {   P_ION = 0;}
    
    // 3. 风机目标转速赋值
    if(g_u8FanSpeed)
    {   InFan.AimSpeed = t_FanSpeed[g_u8FanSpeed];}
    else
    {   InFan.AimSpeed = 0;}
}

/*****************************************************************************
* 函数名称：void F_WorkChenge(void) 
* 函数功能：工作状态(开/关机、亮度)跳变处理
* 入口参数：无
* 出口参数：无  
******************************************************************************/
void F_WorkChenge()
{
    static bit s_bTurnOnBak = 0;
    static U8 s_u8LightStateBak = 0;
        
    // --- 开关机状态跳变处理 ---
    if(s_bTurnOnBak != g_bTurnOn)
    {
        s_bTurnOnBak = g_bTurnOn;
        F_TimeType(0);             // 取消定时
        g_bInFanErr = 0;           // 清除风机故障
        
        if(!g_bTurnOn)
        {   
            // 关机动作
            g_bDLZEn = c_OFF;
            g_bLockEn = 0;
            g_u8PowerOnDispTime = 0;
        }
        else
        {
            // 开机动作
            g_bDLZEn = c_ON;
            g_u8WorkMode = MODE_MID;
            g_u8PowerOnDispTime = C_POWERON_DISP_TIME;
            if(g_bScreenInitFlg)
            {   
				g_u8VoiceFilterBeepCnt = 2; // 设为 2 次
                g_u8VoiceCntTime = 0;       // 立即开始第一声
			} // 滤网到期语音提示使能
        }
    }
        
    // --- 亮度模式跳变处理 ---
    if(s_u8LightStateBak != g_u8LightState)
    {
        s_u8LightStateBak = g_u8LightState;
        if(LIGHT_OFF == g_u8LightState)
        {   g_u8KeyDispTime = 0;}
        else if(LIGHT_HALF == g_u8LightState)
        {   
            g_u8KeyDispTime = 0;
            g_bHalfDispEn = 1;
        }
    }
}

/*****************************************************************************
* 函数名称：void F_AiModeEvent(void) 
* 函数功能：AI(自动)模式风速决策逻辑
* 功能描述：取 甲醛/PM2.5/VOC 中污染最严重的一项，决定风机档位
******************************************************************************/
void F_AiModeEvent()
{
    // 寻找三种污染物中，等级最差的一个
    g_u8AirQuality = g_u8PM25AirQuality;
    
    if(g_u8VocAirQuality > g_u8AirQuality)
        g_u8AirQuality = g_u8VocAirQuality;
        
    if(g_u8CH2OAirQuality > g_u8AirQuality)
        g_u8AirQuality = g_u8CH2OAirQuality;

    // 根据最差空气质量，匹配风速档位
    if(AIR_QUALITY_ACTOR == g_u8AirQuality)
    {   g_u8FanSpeed = FAN_SLEEP;}
    else if(AIR_QUALITY_GOOD == g_u8AirQuality)
    {   g_u8FanSpeed = FAN_LOW;}
    else if(AIR_QUALITY_MID == g_u8AirQuality)
    {   g_u8FanSpeed = FAN_MID;}
    else
    {   g_u8FanSpeed = FAN_HI;}
    
    g_u8AiFanSpeed = g_u8FanSpeed;
}

/*****************************************************************************
* 函数名称：void F_FanEvent(void) 
* 函数功能：风机总体调度逻辑
* 入口参数：无
* 出口参数：无  
******************************************************************************/
void F_FanEvent()
{
    static U8 s_u8FanInitTime = 0;
    
    // 如果开机且无任何报错(风机报错/倾倒/门盖/通讯异常)
    if(g_bTurnOn && !g_bInFanErr && !g_bQDErr && !g_bDoorOpenLeft && !g_bDoorOpenRight && !g_bUartErr)
    {
        // 刚开机前 30*100ms=3秒，强制中风启动
        if(s_u8FanInitTime)
        {
            s_u8FanInitTime--;
            g_u8FanSpeed = FAN_MID;
        }
        // 根据当前模式分配风速
        else if(MODE_AI == g_u8WorkMode)
        {   F_AiModeEvent();}
        else if(MODE_LOW == g_u8WorkMode)
        {   g_u8FanSpeed = FAN_LOW;}
        else if(MODE_MID == g_u8WorkMode)
        {   g_u8FanSpeed = FAN_MID;}
        else if(MODE_HIGH == g_u8WorkMode)
        {   g_u8FanSpeed = FAN_HI;}
        else // 睡眠模式
        {   g_u8FanSpeed = FAN_SLEEP;}
    }
    else
    {
        // 关机或有故障时，复位开机强风计数器，停止风机
        s_u8FanInitTime = 30;
        g_u8FanSpeed = FAN_OFF;
    }
}

/*****************************************************************************
* 函数名称：void F_IonEvent(void) 
* 函数功能：等离子(负离子)发生器控制逻辑
* 入口参数：无
* 出口参数：无  
******************************************************************************/
void F_IonEvent()
{
    // 开机 & 用户开启了等离子 & 非睡眠模式 & 门盖未开 & 未倾倒
    if(g_bTurnOn && g_bDLZEn && (MODE_SLEEP != g_u8WorkMode) && 
      (!g_bDoorOpenRight) && (!g_bDoorOpenLeft) && (!g_bQDErr))
    {
        g_bIonLoad = 1;
    }
    else
    {   
        g_bIonLoad = 0;
    }
}

/**************************************************
*函数名称：void  F_SenseEnEvent(void) 
*函数功能：传感器使能5V事件
*调用时间：100ms
*入口参数：void
*出口参数：void  
**************************************************/
void F_SenseEnEvent()
{
    static U16 s_u16DelayOff = 0;
    
    if(g_bTurnOn)
    {   s_u16DelayOff = 600;}
    else if(s_u16DelayOff)
    {   
        s_u16DelayOff--;
        g_u16SenseEnTime = 0xffff;
    }
    
    if(g_bTurnOn || s_u16DelayOff || (g_u8MCURunTime < 60))
    {   g_bSenseLoad = 1;}
    else
    {   g_bSenseLoad = 0;}
    
    if(g_bSenseLoad)
    {
        if((g_u16SenseEnTime < 0xffff) && g_bTurnOn)
        {   g_u16SenseEnTime++;}
    }
    else
    {   g_u16SenseEnTime = 0;}
}

/*****************************************************************************
* 函数名称：void F_ModeWork(void) 
* 函数功能：综合模式动作执行合集
* 调用时间：100ms
******************************************************************************/
void F_ModeWork(void)
{
    F_FanEvent();
    F_IonEvent();
    F_SenseEnEvent();
    F_LoadDry();
}

/*****************************************************************************
* 函数名称：void F_TimeEvent(void) 
* 函数功能：定时开/关机控制逻辑
* 调用时间：1s
******************************************************************************/
void F_TimeEvent()
{
    if(g_u8TimeData)
    {
        // 如果开启了缩时测试 (1分钟当1小时)
        if(g_bShrinkTimeFlg)
        {
            if(++g_u16TimeCountDown >= 60)
            {
                g_u16TimeCountDown = 0;
                g_u8TimeData--;
                if(!g_u8TimeData)
                {
                    g_bTurnOn = !g_bTurnOn; // 定时时间到，翻转开关机状态
                    if(!g_bTurnOn)
                    {   g_bLockEn = 0;}
                    F_DispTskChange(DIPS_NORMAL);
                }
            }
        }
        // 常规正常定时
        else
        {
            if(++g_u16TimeCountDown >= c_Time_1h)
            {
                g_u16TimeCountDown = 0;
                g_u8TimeData--;
                if(!g_u8TimeData)
                {
                    g_bTurnOn = !g_bTurnOn;
                    if(!g_bTurnOn)
                    {   g_bLockEn = 0;}
                    F_DispTskChange(DIPS_NORMAL);
                }
            }
        }
    }
    else
    {   
        g_u16TimeCountDown = 0;
    }
}

/*****************************************************************************
* 函数名称：void F_VoiceEvent(void) 
* 函数功能：滤网到期语音播报控制
* 调用时间：100ms
******************************************************************************/
void F_VoiceEvent(void)
{
    // 只要计数器大于 0，就说明有播报任务
    if(g_u8VoiceFilterBeepCnt > 0)
    {
        // 倒计时为 0，执行播报
        if(g_u8VoiceCntTime == 0)
        {
            F_VoiceFliter();             // 触发串口下发语音指令 (0x0202)
            g_u8VoiceFilterBeepCnt--;    // 剩余次数减 1
            
            // 如果还有剩余次数（比如开机要求报 2 次），则装载 3 秒间隔
            if(g_u8VoiceFilterBeepCnt > 0)
            {
                g_u8VoiceCntTime = 30;   // 30 * 100ms = 3 秒后报下一次
            }
        }
        else
        {
            g_u8VoiceCntTime--;          // 倒计时递减
        }
    }
}

/*****************************************************************************
* 函数名称：void F_mSystemInital(void) 
* 函数功能：系统业务层变量默认初始化
* 入口参数：无
* 出口参数：无  
******************************************************************************/
void F_mSystemInital(void)
{
    g_bTurnOn = 0;
    g_bLockEn = 0;
    
    g_u8WorkMode   = MODE_AI;       // 默认开机 AI 模式
    g_u8LightState = LIGHT_ON;      // 默认屏幕全亮
    g_bDLZEn       = c_OFF;         // 默认关闭等离子
    
    g_u16ScreenRunTime = 0; 
    g_bEepromEn = 1;
    g_bShrinkTimeFlg = 0;
    g_bRgbDispEn = 1;
}

/*****************************************************************************
* 函数名称：void F_mFunctionSchdule(void)
* 函数功能：业务逻辑总调度 (从 main 循环调用)
* 入口参数：无
* 出口参数：无
******************************************************************************/
void F_mFunctionSchdule(void)
{       
    F_WorkChenge();                 // 开关机跳变拦截
    
    if(F_GetTimeTick(C_TIME_100ms))
    {
        F_ModeWork();               // 核心硬件工作处理
        F_VoiceEvent();             // 语音播报处理
    } 
    
    if(F_GetTimeTick(C_TIME_1s))
    {                        
        F_TimeEvent();              // 定时开/关机处理
    }
}