#include "h\Allinclude.h"

TYPE_DC_FAN InFan = {0};
bit g_bFanPwmEn = 0;

/**************************************************
* 函数名称：void F_PwmInit(void)
* 函数功能：PCA0-模块1 初始化为 16位 PWM
* 硬件连接：P0CEX1 映射到 P5.2
* 频率设定：7.5kHz @ 24MHz（双斜坡）
**************************************************/
void F_PwmInit()
{  
    // --------------------------------------------------------
    // 2. 配置 PCA 寄存器 (Bank 1)
    // --------------------------------------------------------
    SET_BANK_1();

    // 2.1 引脚映射配置 (CEXCR - E7H)
    // P0CEX1 映射控制位是 Bit[6:4]
	CEXCR &= ~0x70;
	CEXCR |= 0x60;
	    // 2.2 配置 PCA 时钟源 (POCMD - 99H)
    // 保持使用 Fsys (24MHz)
	P0CMD = B01000000;
    // 2.3 设置 PWM 周期 (公共周期)
    // 7.5kHz = 24M / (2 * 1600)
    P0TOPH = (PWM_PERIOD_ARR >> 8); 
    P0TOPL = (PWM_PERIOD_ARR & 0xFF);

    // 2.4 配置 模块1 (Module 1) 为 16位 PWM 模式
	P0CPM1 = B11110000;
    // 2.5 初始占空比设为 0 (操作 Module 1 的比较寄存器)
    P0CPH1 = 0;         // 注意这里是 P0CPH1
    P0CPL1 = 0;         // 注意这里是 P0CPL1


    // --------------------------------------------------------
    // 3. 恢复 Bank 0
    // --------------------------------------------------------
    SET_BANK_0();
}

/**************************************************
* 函数名称：void F_FanPwmSet(void)
* 函数功能：更新 PWM 占空比 (操作通道 1)
**************************************************/
void F_FanPwmSet()
{
    static unsigned int s_u16LastDuty = 0xFFFF;

    if(s_u16LastDuty != InFan.CurDuty)
    {
        s_u16LastDuty = InFan.CurDuty;

        SET_BANK_1(); // 切入 Bank 1

        if(InFan.CurDuty == 0)
        {
            // 关断通道 1
            P0CPH1 = 0;
            P0CPL1 = 0;
			P0CPM1 &= ~0x08; // 仅关闭模块1，公共计数器供G通道继续使用
        }
        else if(InFan.CurDuty >= PWM_PERIOD_ARR)
        {
            // 全开
            P0CPH1 = (PWM_PERIOD_ARR >> 8);
            P0CPL1 = (PWM_PERIOD_ARR & 0xFF);
			PCACON |= 0x01;
			P0CPM1 |= 0x08;
        }
        else
        {
            // 正常输出 (操作 POCPH1 / POCPL1)
            // 务必遵守先写高字节，后写低字节的顺序
            P0CPH1 = (InFan.CurDuty >> 8);
            P0CPL1 = (InFan.CurDuty & 0xFF);
			PCACON |= 0x01;
			P0CPM1 |= 0x08;
        }

        SET_BANK_0(); // 恢复 Bank 0
    }
}



/**************************************************
* 函数名称：void F_RampControl(void)
* 函数功能：风机软启动逻辑 (与之前保持一致)
* 调用周期：建议在主循环调用
**************************************************/
void F_RampControl(TYPE_DC_FAN *pFan)
{
	unsigned int timeThreshold;
    // 1. 关机或无使能：立即归零
    if((pFan->AimDuty == 0) || (g_bFanPwmEn == 0))
    {
        pFan->CurDuty = 0;
        pFan->RampTimer = 0;
        return;
    }

    // 已到达目标，无需处理
    if(pFan->CurDuty == pFan->AimDuty)
    {
        pFan->RampTimer = 0;
        return;
    }

    // 2. 时间基准处理 (假设外部提供 F_GetTimeTick)
    if(F_GetTimeTick(C_TIME_10ms)) 
    {
        pFan->RampTimer++; 
        // 3. 确定调整速度 (10ms 或 100ms)
        if(pFan->CurDuty < DUTY_THRESHOLD_10P) // < 10%
        {
            timeThreshold = 1; // 10ms
        }
        else // >= 10%
        {
            timeThreshold = 10; // 100ms
        }

        // 4. 执行渐变
        if(pFan->RampTimer >= timeThreshold)
        {
            pFan->RampTimer = 0;

            if(pFan->CurDuty < pFan->AimDuty)
            {
                // 加速
                if(pFan->AimDuty - pFan->CurDuty >= DUTY_STEP_VAL)
                    pFan->CurDuty += DUTY_STEP_VAL;
                else
                    pFan->CurDuty = pFan->AimDuty;
            }
            else
            {
                // 减速
                if(pFan->CurDuty - pFan->AimDuty >= DUTY_STEP_VAL)
                    pFan->CurDuty -= DUTY_STEP_VAL;
                else
                    pFan->CurDuty = pFan->AimDuty;
            }
        }
    }
}

/**************************************************
* 函数名称：void F_DCFanEn(void)
* 函数功能：电源时序管理
**************************************************/
void F_DCFanEn()
{
    static int s_s16TimeCnt = 0;
    
    if(F_GetTimeTick(C_TIME_100ms))
    {
        if(InFan.AimDuty > 0)
        {
            // 开机序列
            P_FANEN = 1;
            
            if(s_s16TimeCnt < 0) s_s16TimeCnt = 0;
            if(s_s16TimeCnt < 5) // 延时 500ms
            {
                s_s16TimeCnt++;
            }
            else 
            {
                g_bFanPwmEn = 1; // 允许PWM输出
            }
        }
        else
        {
            // 关机序列
            g_bFanPwmEn = 0; // 立即关闭PWM
            
            if(s_s16TimeCnt > 0) s_s16TimeCnt = 0;
            if(s_s16TimeCnt > -50) // 延时 5s 关电
            {
                s_s16TimeCnt--;
            }
            else
            {
                P_FANEN = 0; // 关闭电源
            }
        }
    }
}

/**************************************************
* 函数名称：void DCFanProc(void)
* 函数功能：主循环调用接口
**************************************************/
void DCFanProc()
{
//	InFan.AimDuty = 360;//PWM_MAX_DUTY * 100 / 45;
    F_DCFanEn();            // 电源管理
    F_RampControl(&InFan);  // 软启动计算
    F_FanPwmSet();          // 硬件输出更新
}