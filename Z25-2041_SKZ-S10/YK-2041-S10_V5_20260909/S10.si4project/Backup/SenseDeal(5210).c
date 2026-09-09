#include "h\AllInclude.h"

S8 xdata g_s8NowTemp = 24;//当前温度
U8 xdata g_u8NowHumi = 60;//当前湿度

U8 xdata g_u8PM25AirQuality = 0;//PM25空气质量
U8 xdata g_u8VocAirQuality = 0;//VOC空气质量
U8 xdata g_u8CH2OAirQuality = 0;//甲醛空气质量

U16 g_u16ScreenRunTime = 0;//滤网使用时间 单位0.1小时
U8 g_u8RunPercent = 0;//滤网使用百分比
U8 g_u8FhFilterPer = 100;//滤网剩余百分比
bit g_bScreenInitFlg = 0;//滤网复位标记

StrByte SENSE = {0};

U8 code t_ScreenRunData[] = {5,7,9,9,10,15,20,25,30};//不同PM25，一小时换算滤网使用时间，扩大10倍


S8 xdata g_s8NowTemp = 24;//当前温度
U8 xdata g_u8NowHumi = 60;//当前湿度
U8 code t_AiModeHumiSetData[5][3] = {{48,45,40},{58,55,50},{68,65,60},{48,45,40},{43,40,35}};
//智能模式湿度区间分界值
U8 xdata g_u8HumiRange = c_HUMI_RANGE_1;
U8 xdata g_u8HumiComp  = 8;
/**************************************************
*函数名称：void      F_AiHumiRangeEvent(void) 
*函数功能：湿度区间计算
*调用时间：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_AiHumiRangeEvent(void) 
{
    static U8 s_u8RefHumi_High = 0; 
    static U8 s_u8RefHumi_Mid  = 0; 
    static U8 s_u8RefHumi_Low  = 0; 
	
    static U8 s_u8AiDelayTime  = 0;		//未达到加湿条件的延时时间
    // 新增：高湿锁定标志位 (static bit 节省内存且断电丢失，符合逻辑)
    static bit s_bHasHitRange4 = 0; 

    U8 u8TempIndex = 0;
    U8 u8FinalHumi = 0;
	if(g_bTurnOn && (MODE_AI == g_u8WorkMode))
	/*每次开机或切换到智能模式，如果未达到加湿条件（区间Ⅳ），中档加湿1分钟，1分钟后执
	行智能模式的控制逻辑。*/
	{
		// 1. 计算带补偿的当前湿度
	    u8FinalHumi = g_u8NowHumi + g_u8HumiComp;
	    if(u8FinalHumi > 99) u8FinalHumi = 99;

	    // 2. 查表获取阈值
	    if(g_s8NowTemp < 18)      u8TempIndex = 0;
	    else if(g_s8NowTemp < 23) u8TempIndex = 1;
	    else if(g_s8NowTemp < 27) u8TempIndex = 2;
	    else if(g_s8NowTemp < 32) u8TempIndex = 3;
	    else                      u8TempIndex = 4;

	    s_u8RefHumi_High = t_AiModeHumiSetData[u8TempIndex][0]; 
	    s_u8RefHumi_Mid  = t_AiModeHumiSetData[u8TempIndex][1]; 
	    s_u8RefHumi_Low  = t_AiModeHumiSetData[u8TempIndex][2]; 

	    // 3. 标准四区间判定 (先按物理状态计算)
	    if(u8FinalHumi >= (s_u8RefHumi_High + c_HUMI_HYSTERESIS))
	    {
	        g_u8HumiRange = c_HUMI_RANGE_4;
	        // 动作：一旦进入区间4，激活锁定标志
	        s_bHasHitRange4 = 1;
	    }
	    else if(u8FinalHumi >= (s_u8RefHumi_Mid + c_HUMI_HYSTERESIS))
	    {
	        g_u8HumiRange = c_HUMI_RANGE_3;
	    }
	    else if(u8FinalHumi >= (s_u8RefHumi_Low + c_HUMI_HYSTERESIS))
	    {
	        g_u8HumiRange = c_HUMI_RANGE_2; 
	    }
	    else if(u8FinalHumi <= (s_u8RefHumi_Low - c_HUMI_HYSTERESIS))
	    {
	        g_u8HumiRange = c_HUMI_RANGE_1;
	    }
	    // else: 死区保持上一次状态 (暂不处理，后面统一钳位)

	    // 4. 执行“高湿锁定”逻辑
	    // 需求：当湿度上升至进入区间4后，湿度下降也只能回到区间3
	    if(s_bHasHitRange4 == 1)
	    {
	        // 如果当前计算出的状态 低于 区间3 (即可能是区间2、1)
	        // 强制钳位在 区间3
	        if(g_u8HumiRange < c_HUMI_RANGE_3)
	        {
	            g_u8HumiRange = c_HUMI_RANGE_3;
	        }
	    }
		else if(!s_u8AiDelayTime)//当前未达到加湿条件，需中档运行1分钟
		{
			s_u8AiDelayTime = 60; 
		}
	}
    
}



/* 全局变量与宏定义 (保持不变) */
// S8 xdata g_s8HumiComp = 0; 
// #define c_HUMI_RANGE_4  4 
// #define c_HUMI_RANGE_3  3 
// #define c_HUMI_RANGE_2  2 
// #define c_HUMI_RANGE_1  1 
#define c_HUMI_HYSTERESIS  2 

/**************************************************
*函数名称：void F_AiHumiRangeEvent(void) 
*函数功能：湿度4区间判定 (含高湿锁定 + 下降2%回差逻辑)
*调用时间：1s
**************************************************/
void F_AiHumiRangeEvent(void) 
{
    static U8 s_u8RefHumi_High = 0; 
    static U8 s_u8RefHumi_Mid  = 0; 
    static U8 s_u8RefHumi_Low  = 0; 
    
    // 静态变量：记录上升曲线过程中的“峰值湿度”
    static U8 s_u8HumiPeakInCurve = 0; 
    // 静态变量：高湿锁定标志
    static bit s_bHasHitRange4 = 0; 

    U8 u8TempIndex = 0; 
    U8 u8FinalHumi = 0;
    
    // 临时变量：存储本次计算出的“理论区间”
    U8 s_u8CalcRange = c_HUMI_RANGE_1; 

	U8 u8DropDelta = 0
	if(g_bTurnOn && (MODE_AI == g_u8WorkMode))
	{
		// 1. 计算带补偿的当前湿度
	    u8FinalHumi = g_u8NowHumi + g_u8HumiComp;
	    if(u8FinalHumi > 99) u8FinalHumi = 99;

	    if(g_s8NowTemp < 18)      u8TempIndex = 0;
	    else if(g_s8NowTemp < 23) u8TempIndex = 1;
	    else if(g_s8NowTemp < 27) u8TempIndex = 2;
	    else if(g_s8NowTemp < 32) u8TempIndex = 3;
	    else                      u8TempIndex = 4;

	    s_u8RefHumi_High = t_AiModeHumiSetData[u8TempIndex][0]; 
	    s_u8RefHumi_Mid  = t_AiModeHumiSetData[u8TempIndex][1]; 
	    s_u8RefHumi_Low  = t_AiModeHumiSetData[u8TempIndex][2]; 

	    // ------------------------------------------------------
	    // 2. 计算“理论区间” (s_u8CalcRange)
	    // ------------------------------------------------------
	    // 这里只计算物理状态，暂不更新全局变量 g_u8HumiRange
	    if(u8FinalHumi >= (s_u8RefHumi_High + c_HUMI_HYSTERESIS))
	    {
	        s_u8CalcRange = c_HUMI_RANGE_4;
	        s_bHasHitRange4 = 1; // 触发高湿锁定标志
	    }
	    else if(u8FinalHumi >= (s_u8RefHumi_Mid + c_HUMI_HYSTERESIS))
	    {
	        s_u8CalcRange = c_HUMI_RANGE_3;
	    }
	    else if(u8FinalHumi >= (s_u8RefHumi_Low + c_HUMI_HYSTERESIS))
	    {
	        s_u8CalcRange = c_HUMI_RANGE_2; 
	    }
	    else if(u8FinalHumi <= (s_u8RefHumi_Low - c_HUMI_HYSTERESIS))
	    {
	        s_u8CalcRange = c_HUMI_RANGE_1;
	    }
	    else
	    {
	        // 处于死区时，理论区间暂时认为等于当前实际区间
	        s_u8CalcRange = g_u8HumiRange; 
	    }

	    // ------------------------------------------------------
	    // 3. 处理“高湿锁定” (区间4 -> 区间3 限制)
	    // ------------------------------------------------------
	    if(s_bHasHitRange4 == 1)
	    {
	        if(s_u8CalcRange < c_HUMI_RANGE_3)
	        {
	            s_u8CalcRange = c_HUMI_RANGE_3;
	        }
	    }

	    // ------------------------------------------------------
	    // 4. 核心逻辑：上升曲线峰值记录 与 下降2%回差判定
	    // ------------------------------------------------------
	    
	    // 情况A：湿度正在上升，或者处于高位 (理论区间 > 当前输出区间)
	    // 此时应当：1. 立即更新区间以响应除湿 2. 更新峰值记录
	    if(s_u8CalcRange > g_u8HumiRange)
	    {
	        g_u8HumiRange = s_u8CalcRange;
	        s_u8HumiPeakInCurve = u8FinalHumi; // 记录新的“起跑线”
	    }
	    // 情况B：区间稳定 (理论 == 输出)
	    // 此时应当：如果湿度还在微涨（例如在区间3内从46涨到47），继续更新峰值
	    else if(s_u8CalcRange == g_u8HumiRange)
	    {
	        if(u8FinalHumi > s_u8HumiPeakInCurve)
	        {
	            s_u8HumiPeakInCurve = u8FinalHumi;
	        }
	    }
	    // 情况C：湿度下降，且理论区间变低 (理论 < 输出)
	    // 此时应当：执行你要求的逻辑——下降幅度 >= 2% 才允许切换
	    else 
	    {
	        // 计算从“上升曲线最高点”跌落了多少  
	        if(s_u8HumiPeakInCurve > u8FinalHumi)
	        {
	            u8DropDelta = s_u8HumiPeakInCurve - u8FinalHumi;
	        }

	        // --- 核心判断 ---
	        if(u8DropDelta >= 2) 
	        {
	            // 下降幅度足够，允许切换到低区间
	            g_u8HumiRange = s_u8CalcRange;
	            
	            // 切换后，将峰值重置为当前值，作为下一轮可能的参考
	            s_u8HumiPeakInCurve = u8FinalHumi;
	        }
	        else
	        {
	            // 下降幅度不足2% (例如只降了1%)
	            // 保持原 g_u8HumiRange 不变
	            // 也不更新 s_u8CalcRange
	        }
	    }
	}
	else
	{
		s_bHasHitRange4 = 0;
	}
}


/**************************************************
*函数名称：void  F_ScreenLifeEvent(void) 
*函数功能：滤网寿命计算
*调用时间：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_ScreenLifeEvent()
{
    static U16 s_u16ScreenTime = 0;
    
    if(g_bTurnOn && g_u8FanSpeed)
    {
        if(g_bQuickTest)
        {
            if(g_u16ScreenRunTime < 0xffff)g_u16ScreenRunTime++;
            if(g_u16ScreenRunTime >= 60)
            {   g_bScreenInitFlg = 1;}
        }
        else if(g_u16ScreenRunTime < DUST_TOTAL_TIM)
        {
            if(++s_u16ScreenTime >= g_u16Time_1h)
            {
                s_u16ScreenTime = 0;
                if(!g_u16Pm25Data)
                {   g_u16ScreenRunTime += t_ScreenRunData[0];}
                else if(g_u16Pm25Data > 450)
                {   g_u16ScreenRunTime += t_ScreenRunData[8];}
                else
                {   g_u16ScreenRunTime += t_ScreenRunData[(g_u16Pm25Data-1) / 50];}
            }
        }
    }
    else
    {   s_u16ScreenTime = 0;}
    
    if(g_u16ScreenRunTime >= DUST_TOTAL_TIM)
    {   g_u8RunPercent = 100;}
    else
    {
        g_u8RunPercent = (U32)g_u16ScreenRunTime * 100 / DUST_TOTAL_TIM;
    }
    g_u8FhFilterPer = 100 - g_u8RunPercent;
    
    if(g_u8FhFilterPer <= 2)
    {   g_bScreenInitFlg = 1;}
}
/**************************************************
*函数名称：void  F_SenseEvent(void) 
*函数功能：传感器事件
*调用时间：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_SenseEvent()
{
    static U8 s_u8TempCnt = 0;
    static U8 s_u8HumiCnt = 0;
        
	if(g_u8MCURunTime <= 5)
	{
        g_s8NowTemp = g_s8TempData;
		g_u8NowHumi = g_u8HumiData;
	}
	else
	{        
        if(g_s8NowTemp != g_s8TempData)
        {
            if(++s_u8TempCnt >= 5)
            {
                s_u8TempCnt = 0;
                g_s8NowTemp = g_s8TempData;
            }
        }
        else
        {   s_u8TempCnt = 0;}
        
        if(g_u8NowHumi != g_u8HumiData)
        {
            if(++s_u8HumiCnt >= 5)
            {
                s_u8HumiCnt = 0;
                g_u8NowHumi = g_u8HumiData;
            }
        }
        else
        {   s_u8HumiCnt = 0;}
	}
}
/**************************************************
*函数名称：void  F_PM25Event(void) 
*函数功能：PM25传感器事件
*调用时间：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_PM25Event()
{
    if(c_AIR_QUALITY_ACTOR == g_u8PM25AirQuality)
    {
        if(g_u16Pm25Data > 35)
        {   g_u8PM25AirQuality = c_AIR_QUALITY_GOOD;}
    }
    else if(c_AIR_QUALITY_GOOD == g_u8PM25AirQuality)
    {
        if(g_u16Pm25Data <= 34)
        {   g_u8PM25AirQuality = c_AIR_QUALITY_ACTOR;}
        else if(g_u16Pm25Data > 75)
        {   g_u8PM25AirQuality = c_AIR_QUALITY_MID;}
    }
    else if(c_AIR_QUALITY_MID == g_u8PM25AirQuality)
    {
        if(g_u16Pm25Data <= 74)
        {   g_u8PM25AirQuality = c_AIR_QUALITY_GOOD;}
        else if(g_u16Pm25Data > 115)
        {   g_u8PM25AirQuality = c_AIR_QUALITY_BAD;}
    }
    else
    {
        if(g_u16Pm25Data <= 114)
        {   g_u8PM25AirQuality = c_AIR_QUALITY_MID;}
    }
}
/**************************************************
*函数名称：void  F_VOCEvent(void) 
*函数功能：VOC传感器事件
*调用时间：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_VOCEvent()
{
    static U16 s_u16UpDelay = 0;
    static U8 s_u8VocAirQuality = 0;
    static U8 s_u8VocAirQualityBak = 0;
    static U16 s_u16VocDataBak = 0;
    
    if(c_AIR_QUALITY_ACTOR == s_u8VocAirQuality)
    {
        if(g_u16VocData >= c_VOC_GOOD)
        {   s_u8VocAirQuality = c_AIR_QUALITY_GOOD;}
    }
    else if(c_AIR_QUALITY_GOOD == s_u8VocAirQuality)
    {
        if(g_u16VocData <= (c_VOC_GOOD - c_VOC_BIAS))
        {   s_u8VocAirQuality = c_AIR_QUALITY_ACTOR;}
        else if(g_u16VocData >= c_VOC_MID)
        {   s_u8VocAirQuality = c_AIR_QUALITY_MID;}
    }
    else if(c_AIR_QUALITY_MID == s_u8VocAirQuality)
    {
        if(g_u16VocData <= (c_VOC_MID - c_VOC_BIAS))
        {   s_u8VocAirQuality = c_AIR_QUALITY_GOOD;}
        else if(g_u16VocData >= c_VOC_BAD)
        {   s_u8VocAirQuality = c_AIR_QUALITY_BAD;}
    }
    else
    {
        if(g_u16VocData <= (c_VOC_BAD - c_VOC_BIAS))
        {   s_u8VocAirQuality = c_AIR_QUALITY_MID;}
    }
    
    if(s_u8VocAirQualityBak != s_u8VocAirQuality)
    {   
        s_u8VocAirQualityBak = s_u8VocAirQuality;
        g_u8VocAirQuality = s_u8VocAirQuality;
        s_u16UpDelay = 0;
    }
    
    if(c_AIR_QUALITY_ACTOR != g_u8VocAirQuality)
    {
        if(s_u16UpDelay < 0xffff)s_u16UpDelay++;
        if(g_u8VocAirQuality != c_AIR_QUALITY_GOOD)
        {
            if(s_u16UpDelay >= 600)
            {   
                s_u16UpDelay = 0;
                g_u8VocAirQuality--;
                s_u16VocDataBak = g_u16VocData;
            }
        }
        else
        {
            if(s_u16UpDelay >= 1800)
            {   
                s_u16UpDelay = 0;
                g_u8VocAirQuality--;
                s_u16VocDataBak = g_u16VocData;
            }
        }
    }
    
    if(g_u8VocAirQuality != s_u8VocAirQuality)
    {
        if(s_u16VocDataBak > g_u16VocData)
        {
            if((s_u16VocDataBak - g_u16VocData) >= 50)
            {
                g_u8VocAirQuality = s_u8VocAirQuality;
                s_u16UpDelay = 0;
            }
        }
        else if(s_u16VocDataBak < g_u16VocData)
        {
            if((g_u16VocData - s_u16VocDataBak) >= 50)
            {
                g_u8VocAirQuality = s_u8VocAirQuality;
                s_u16UpDelay = 0;
            }
        }
    }
}
/**************************************************
*函数名称：void  F_CH2OEvent(void) 
*函数功能：甲醛传感器事件
*调用时间：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_CH2OEvent()
{
    if(g_u16CH2OData <= 60)
    {   g_u8CH2OAirQuality = c_AIR_QUALITY_ACTOR;}
    else if(g_u16CH2OData <= 80)
    {   g_u8CH2OAirQuality = c_AIR_QUALITY_GOOD;}
    else if(g_u16CH2OData <= 120)
    {   g_u8CH2OAirQuality = c_AIR_QUALITY_MID;}
    else
    {   g_u8CH2OAirQuality = c_AIR_QUALITY_BAD;}    
}

void F_mAllSenseDeal(void)
{  
	if(F_GetTimeTick(C_TIME_1s))
	{
        if(g_bSenseLoad)
        {
            F_QueryVocData();
            F_QueryCH2OData();
            F_PM25Event();
            F_VOCEvent();
            F_CH2OEvent();
            F_SenseEvent();
            F_ScreenLifeEvent();
        }
	}
} 