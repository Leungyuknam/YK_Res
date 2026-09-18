#include "h\Allinclude.h"
#define C_ADC_RESTART_TIME  10  // 异常超时时间

/**************************************************
* 函数名称：F_ADC_Read
* 函数功能：非阻塞ADC采样状态机
* 入口参数：u8Ch:通道, u8Samples:采样次数(求平均), *pu16Result:结果指针
* 返回参数：0:采样中, 1:采样完成
**************************************************/
U8 F_ADC_Read(U8 u8Ch, U8 u8Samples, U16 *pu16Result)
{   
    static xdata U8 s_u8TimeoutCnt = C_ADC_RESTART_TIME;
    static xdata U8 s_u8SampleCnt = 0;
    static bit s_bIsRunning = 0;

    // 如果采样次数为0，关闭ADC并复位状态
    if(u8Samples == 0)
    {   
        F_AdcOff();
        s_u8SampleCnt = 0;
        s_u8TimeoutCnt = 0;
        s_bIsRunning = 0;
        return 0;
    }

    // 状态机处理
    if(s_u8SampleCnt == 0) // --- 阶段1：启动第一次转换 ---
    {   
        if(!s_bIsRunning) 
        {
            F_AdcInit();
            s_bIsRunning = 1;
        }
        
        F_ClrAdcOkFlg();
        F_AdcStart(u8Ch);
        
        *pu16Result = 0; // 清空累加器
        s_u8TimeoutCnt = 0;
        s_u8SampleCnt = 1; 
    }
    else if(ADC_OK_FLAG) // --- 阶段2：转换完成，读取数据 ---
    {   
        F_ClrAdcOkFlg();
        s_u8TimeoutCnt = 0; // 重置超时
        
        // 累加数据
        (*pu16Result) += ADC_VALUE_10BIT;
        
        // 检查是否完成所有采样
        if(++s_u8SampleCnt > u8Samples)
        {   
            // 计算平均值
            *pu16Result = (*pu16Result) / u8Samples;
            
            // 复位状态，准备下一次调用
            s_u8SampleCnt = 0; 
            s_bIsRunning = 0;
            return 1; // 返回完成
        }
        else
        {   
            // 继续下一次采样
            F_AdcStart(u8Ch);
        }
    }
    else // --- 阶段3：等待中/超时检查 ---
    {   
        if(++s_u8TimeoutCnt >= C_ADC_RESTART_TIME)
        {   
            // 超时异常处理：重置状态机
            s_u8SampleCnt = 0;
        }
    }
    
    return 0; // 返回进行中
}