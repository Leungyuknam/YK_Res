//#include "h\Allinclude.h"

//TYPE_DC_FAN InFan = {0};

//bit g_bFanPwmEn = 0;        //风机PWM输出使能
//bit g_bInFanErr = 0;        //风机故障标志

///*void InFanDrvIRQ()
//{
//    static U8 s_u8InFanCyc = 0;

//	s_u8InFanCyc ++;
//	if(s_u8InFanCyc >= MAX_DUTY)s_u8InFanCyc = 0;
//	if(s_u8InFanCyc < InFan.Duty)
//		PO_INFAN_DRV = 0;
//	else
//		PO_INFAN_DRV = 1;
//}*/
///**************************************************
//*函数名称：void  F_InFanBckIRQ(void) 
//*函数功能：风机转速检测
//*入口参数：void
//*出口参数：void  
//**************************************************/
//void F_InFanBckIRQ()
//{
//    static bit s_bInfanBckPIn = 0;
//    static S8 s_s8InFanBckFilter = 0;

//	if(InFan.TimeCnt < 0xffff)InFan.TimeCnt ++;
//	if(P_IN_BCK)
//	{
//		if(s_s8InFanBckFilter < 0)
//		{	s_s8InFanBckFilter = 0;}
//		if(s_s8InFanBckFilter < 2)
//		{	s_s8InFanBckFilter ++;}
//		else
//		{	s_bInfanBckPIn = 1;}
//	}
//	else
//	{
//		if(s_s8InFanBckFilter > 0)
//		{	s_s8InFanBckFilter = 0;}
//		if(s_s8InFanBckFilter > (-2))
//		{	s_s8InFanBckFilter --;}
//		else
//		{
//			if(s_bInfanBckPIn)
//			{
//				s_bInfanBckPIn = 0;
//				InFan.PulseCnt ++;
//				if((InFan.PulseCnt >= CAL_CYCLE)&&(InFan.TimeCnt > 228))
//				{
//					if(InFan.SpeedTime == 0)InFan.SpeedTime = InFan.TimeCnt;
//					InFan.PulseCnt = 0;
//					InFan.TimeCnt = 0;
//				}
//			}
//		}
//	}
//}
///**************************************************
//*函数名称：void  F_CalDuty(void) 
//*函数功能：风机占空比输出判断
//*入口参数：void
//*出口参数：void  
//**************************************************/
//#if 0
//void F_CalDuty(TYPE_DC_FAN *pFan)
//{
//	if(pFan->AimSpeed)
//	{
//		if(pFan->CalTurnCnt)
//		{
//			if(GetTimeTick(TIME_10ms))
//			{
//				(pFan->CalTurnCnt) --;
//			}
//		}
//		else
//		{
//			pFan->CalTurnCnt = 20;
//			if(pFan->Duty == 0)
//			{
//				(pFan->Duty) = START_DUTY;
//			}
//			else if((pFan->AimSpeed) > (pFan->CurSpeed))
//			{
//				if((pFan->AimSpeed) - (pFan->CurSpeed) >= 2)
//				{
//					(pFan->Duty) += 1;
//				}
//			}
//			else if((pFan->AimSpeed) < (pFan->CurSpeed))
//			{
//				if((pFan->CurSpeed) - (pFan->AimSpeed) >= 2)
//				{
//					(pFan->Duty) -= 1;
//				}
//			}
//			if((pFan->Duty) > MAX_DUTY)(pFan->Duty) = MAX_DUTY;
//			else if((pFan->Duty) < MIN_DUTY)(pFan->Duty) = MIN_DUTY;
//		}
//	}
//	else
//	{
//		pFan->Duty = 0;
//	}
//}
//#else
//void F_CalDuty(TYPE_DC_FAN *pFan)
//{
//	U16 temp;
	
//	if(pFan->AimSpeed && g_bFanPwmEn)
//	{
//		if(pFan->CalTurnCnt)
//		{
//			if(F_GetTimeTick(C_TIME_10ms))
//			{
//				(pFan->CalTurnCnt) --;
//			}
//		}
//		else
//		{
//			pFan->CalTurnCnt = 50;
//			if(pFan->Duty == 0)
//			{
//				(pFan->Duty) = START_DUTY;
//			}
//            else if((pFan->AimSpeed) > (pFan->CurSpeed) + 5)
//			{
//				temp = (pFan->AimSpeed) - (pFan->CurSpeed);
//                if(temp <= 50)
//				{	temp = 1;}
//				else if(temp <= 100)
//				{	temp = 2;}
//				else if(temp <= 200)
//				{	temp = 8;}
//				else if(temp <= 300)
//				{	temp = 32;}
//				else
//				{	temp = 64;}
//				(pFan->Duty) += temp;
//			}
//			else if((pFan->AimSpeed) + 5 < (pFan->CurSpeed))
//			{
//				temp = (pFan->CurSpeed) - (pFan->AimSpeed);
//                if(temp <= 50)
//				{	temp = 1;}
//				else if(temp <= 100)
//				{	temp = 2;}
//				else if(temp <= 200)
//				{	temp = 8;}
//				else if(temp <= 300)
//				{	temp = 32;}
//				else
//				{	temp = 64;}
//				(pFan->Duty) -= temp;
//			}
////			else 
////            {   pFan->Duty = pFan->AimSpeed;}
            
//			if((pFan->Duty) > MAX_DUTY)(pFan->Duty) = MAX_DUTY;
//			else if((pFan->Duty) < MIN_DUTY)(pFan->Duty) = MIN_DUTY;
//		}
//	}
//	else
//	{
//		pFan->Duty = 0;
//		pFan->CalTurnCnt = 0;
//	}
//}
//#endif
///**************************************************
//*函数名称：void  F_GetDcTurn(void) 
//*函数功能：获取风机转速
//*入口参数：void
//*出口参数：void  
//**************************************************/
//void F_GetDcTurn(TYPE_DC_FAN *pFan)
//{
//	if(pFan->TimeCnt == 0xffff)
//	{
//		pFan->CurSpeed = 0;
//		pFan->PulseCnt = 0;
//		pFan->TimeCnt = 0;
//	}
//	else
//	{
//		if(pFan->SpeedTime)
//		{
//			pFan->CurSpeed = 960000/(U32)(pFan->SpeedTime);
//			pFan->CalTurnCnt = 0;
//			pFan->SpeedTime = 0;
//		}
//	}

//	if(pFan->AimSpeed)
//	{
//		if(pFan->CurSpeed < 50)
//		{
//			if(F_GetTimeTick(C_TIME_100ms))
//			{
//				if(pFan->ErrCnt < 255)pFan->ErrCnt ++;
//			}
//		}
//		else
//		{
//			pFan->ErrCnt = 0;
//		}
//	}
//	else
//	{
//		pFan->ErrCnt = 0;
//	}
    
//    if(!g_bFanPwmEn)
//    {
//        pFan->SpeedTime = 0;
//        pFan->CurSpeed = 0;
//		pFan->PulseCnt = 0;
//        pFan->ErrCnt = 0;
//    }
//}
///**************************************************
//*函数名称：void  F_DCFanErr(void) 
//*函数功能：风机故障检测
//*入口参数：void
//*出口参数：void  
//**************************************************/
//void F_DCFanErr()//风扇错误判断
//{
//#ifdef DEBUG_FAN
//	#pragma message "---------屏蔽电机故障检测---------"
//#else
//	if(InFan.ErrCnt >= 100)
//	{	g_bInFanErr = 1;}
//#endif
//}
///**************************************************
//*函数名称：void  F_PwmInit(void) 
//*函数功能：PWM初始化
//*入口参数：void
//*出口参数：void  
//**************************************************/
//void F_PwmInit()
//{
//    SET_BANK_1();
    
////    CEXCR = B00100100;//P0CEX0映射到P24
//    CEXCR = B00100000; //P0CEX0映射到P34   
//    P0CMD = 0x41;//双沿 时钟为系统时钟的1/12
//    P0CPM0 = B11110000;//PWM模式，16位相频修正PWM方式，正向输出波形(Duty为高电平有效)
    
//    P0TOPH = RUN_CYCLE >> 8;
//    P0TOPL = RUN_CYCLE & 0xff;

////    P0CPH0 = MAX_DUTY >> 8;
////    P0CPL0 = MAX_DUTY & 0xff;
    
////    PCACON = 0x01;//PCACON定时器使能
////    P0CPM0 |= 0x08;//使能比较捕捉模块
    
//    SET_BANK_0();
//}
///**************************************************
//*函数名称：void  F_FanPwmSet(void) 
//*函数功能：风机PWM设置
//*入口参数：void
//*出口参数：void  
//**************************************************/
//void F_FanPwmSet()
//{    
//    static U16 s_u16InfanPwmBck = 0;

//	if(InFan.Duty)
//	{
//		if(s_u16InfanPwmBck != InFan.Duty)
//		{
//			s_u16InfanPwmBck = InFan.Duty;
//            SET_BANK_1();
//			P0CPH0 = InFan.Duty >> 8;
//			P0CPL0 = InFan.Duty & 0xff;
//            PCACON |= 0x01;
//			P0CPM0 |= 0x08;
//            SET_BANK_0();
//		}
//	}
//	else if(s_u16InfanPwmBck)
//	{
//        SET_BANK_1();
//        P0CPH0 = 0;
//        P0CPL0 = 0;
//		P0CPM0 &= ~0x08;
//        PCACON &= ~0x01;
//        SET_BANK_0();
//		P_FAN_PWM = 0;
//		s_u16InfanPwmBck = 0;
//	}
//}

//void F_DCFanEn()
//{
//    static S16 s_s16TimeCnt = 0;
    
//    if(F_GetTimeTick(C_TIME_100ms))
//	{
//        if(InFan.AimSpeed)
//        {
//            P_EN15V = 0;
//            if(s_s16TimeCnt < 0)
//            {   s_s16TimeCnt = 0;}
//            if(s_s16TimeCnt < 5)   //开机延时500ms开驱动
//            {   s_s16TimeCnt++;}
//            else 
//            {   g_bFanPwmEn = 1;}
//        }
//        else
//        {
//            g_bFanPwmEn = 0;
//            if(s_s16TimeCnt > 0)
//            {   s_s16TimeCnt = 0;}
//            if(s_s16TimeCnt > -200)  //关机延时20s关电源
//            {   s_s16TimeCnt--;}
//            else
//            {   P_EN15V = 1;}
//        }
//    }
//}

//void DCFanIRQ()
//{
//	//InFanDrvIRQ();
//	F_InFanBckIRQ();
//}

//void DCFanProc()
//{
//    F_DCFanEn();
//	F_GetDcTurn(&InFan);
//	F_DCFanErr();
//	F_CalDuty(&InFan);
//	F_FanPwmSet();
//}



#include "h\Allinclude.h"

TYPE_DC_FAN InFan = {0};
bit g_bFanPwmEn = 0;

/**************************************************
* 函数名称：void F_PwmInit(void)
* 函数功能：PCA0-模块1 初始化为 16位 PWM
* 硬件连接：P0CEX1 映射到 P5.2
* 频率设定：15kHz @ 24MHz
**************************************************/
void F_PwmInit()
{
    // --------------------------------------------------------
    // 1. 配置 IO 口 (Bank 0)
    // --------------------------------------------------------
    SET_BANK_0();
    
    // 配置 P5.2 为输出模式
    // 注意：P5CR 寄存器通常在 Bank 0
    P5CR |= 0x04;       // Bit2 = 1 (P5.2 Output)
    
    // --------------------------------------------------------
    // 2. 配置 PCA 寄存器 (Bank 1)
    // --------------------------------------------------------
    SET_BANK_1();

    // 2.1 引脚映射配置 (CEXCR - E7H)
    // P0CEX1 映射控制位是 Bit[5:3]
    // 000:P3.5, 001:P3.7, 010:P1.5, 011:P5.2 ...
    // 我们需要设置 Bit[5:3] = 011 (二进制)
    CEXCR &= ~0x38;     // 清除 Bit 5-3 (0011 1000)
    CEXCR |= 0x18;      // 设置 Bit 5-3 为 011 (0001 1000) -> P5.2

    // 2.2 配置 PCA 时钟源 (POCMD - 99H)
    // 保持使用 Fsys (24MHz)
    P0CMD &= ~0x07;     // Bit2-0 = 000 (Fsys)

    // 2.3 设置 PWM 周期 (公共周期)
    // 15kHz = 24M / 1600
    P0TOPH = (PWM_PERIOD_ARR >> 8); 
    P0TOPL = (PWM_PERIOD_ARR & 0xFF);

    // 2.4 配置 模块1 (Module 1) 为 16位 PWM 模式
    // 寄存器变更为 P0CPM1 (9BH)
    // Bit7(POSMP1)=1, Bit6(POSMN1)=0, Bit5(P0FSP1)=0, Bit4(P0FSN1)=1 -> 16位 PWM
    // Bit3(P0ECOM1)=1 -> 允许比较器
    P0CPM1 = 0x98;      // Binary: 1001 1000

    // 2.5 初始占空比设为 0 (操作 Module 1 的比较寄存器)
    P0CPH1 = 0;         // 注意这里是 P0CPH1
    P0CPL1 = 0;         // 注意这里是 P0CPL1

    // 2.6 启动 PCA 计数器 (如果尚未启动)
    PCACON |= 0x01;     // Bit0 (PRO) Run

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
        }
        else if(InFan.CurDuty >= PWM_PERIOD_ARR)
        {
            // 全开
            P0CPH1 = (PWM_PERIOD_ARR >> 8);
            P0CPL1 = (PWM_PERIOD_ARR & 0xFF);
        }
        else
        {
            // 正常输出 (操作 POCPH1 / POCPL1)
            // 务必遵守先写高字节，后写低字节的顺序
            P0CPH1 = (InFan.CurDuty >> 8);
            P0CPL1 = (InFan.CurDuty & 0xFF);
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
    }
    else
    {
        return; 
    }

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
            P_FANEN = 1; // 打开电源 (假设低有效)
            
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
            if(s_s16TimeCnt > -200) // 延时 20s 关电
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
	InFan.AimDuty = PWM_MAX_DUTY / 2;
    F_DCFanEn();            // 电源管理
    F_RampControl(&InFan);  // 软启动计算
    F_FanPwmSet();          // 硬件输出更新
}