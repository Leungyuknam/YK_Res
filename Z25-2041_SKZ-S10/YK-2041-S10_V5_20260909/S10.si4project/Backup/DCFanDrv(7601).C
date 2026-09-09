#include "h\Allinclude.h"

TYPE_DC_FAN InFan = {0};

bit g_bFanPwmEn = 0;        //风机PWM输出使能
bit g_bInFanErr = 0;        //风机故障标志

/*void InFanDrvIRQ()
{
    static U8 s_u8InFanCyc = 0;

	s_u8InFanCyc ++;
	if(s_u8InFanCyc >= MAX_DUTY)s_u8InFanCyc = 0;
	if(s_u8InFanCyc < InFan.Duty)
		PO_INFAN_DRV = 0;
	else
		PO_INFAN_DRV = 1;
}*/
/**************************************************
*函数名称：void  F_InFanBckIRQ(void) 
*函数功能：风机转速检测
*入口参数：void
*出口参数：void  
**************************************************/
void F_InFanBckIRQ()
{
    static bit s_bInfanBckPIn = 0;
    static S8 s_s8InFanBckFilter = 0;

	if(InFan.TimeCnt < 0xffff)InFan.TimeCnt ++;
	if(P_IN_BCK)
	{
		if(s_s8InFanBckFilter < 0)
		{	s_s8InFanBckFilter = 0;}
		if(s_s8InFanBckFilter < 2)
		{	s_s8InFanBckFilter ++;}
		else
		{	s_bInfanBckPIn = 1;}
	}
	else
	{
		if(s_s8InFanBckFilter > 0)
		{	s_s8InFanBckFilter = 0;}
		if(s_s8InFanBckFilter > (-2))
		{	s_s8InFanBckFilter --;}
		else
		{
			if(s_bInfanBckPIn)
			{
				s_bInfanBckPIn = 0;
				InFan.PulseCnt ++;
				if((InFan.PulseCnt >= CAL_CYCLE)&&(InFan.TimeCnt > 228))
				{
					if(InFan.SpeedTime == 0)InFan.SpeedTime = InFan.TimeCnt;
					InFan.PulseCnt = 0;
					InFan.TimeCnt = 0;
				}
			}
		}
	}
}
/**************************************************
*函数名称：void  F_CalDuty(void) 
*函数功能：风机占空比输出判断
*入口参数：void
*出口参数：void  
**************************************************/
#if 0
void F_CalDuty(TYPE_DC_FAN *pFan)
{
	if(pFan->AimSpeed)
	{
		if(pFan->CalTurnCnt)
		{
			if(GetTimeTick(TIME_10ms))
			{
				(pFan->CalTurnCnt) --;
			}
		}
		else
		{
			pFan->CalTurnCnt = 20;
			if(pFan->Duty == 0)
			{
				(pFan->Duty) = START_DUTY;
			}
			else if((pFan->AimSpeed) > (pFan->CurSpeed))
			{
				if((pFan->AimSpeed) - (pFan->CurSpeed) >= 2)
				{
					(pFan->Duty) += 1;
				}
			}
			else if((pFan->AimSpeed) < (pFan->CurSpeed))
			{
				if((pFan->CurSpeed) - (pFan->AimSpeed) >= 2)
				{
					(pFan->Duty) -= 1;
				}
			}
			if((pFan->Duty) > MAX_DUTY)(pFan->Duty) = MAX_DUTY;
			else if((pFan->Duty) < MIN_DUTY)(pFan->Duty) = MIN_DUTY;
		}
	}
	else
	{
		pFan->Duty = 0;
	}
}
#else
void F_CalDuty(TYPE_DC_FAN *pFan)
{
	U16 temp;
	
	if(pFan->AimSpeed && g_bFanPwmEn)
	{
		if(pFan->CalTurnCnt)
		{
			if(F_GetTimeTick(C_TIME_10ms))
			{
				(pFan->CalTurnCnt) --;
			}
		}
		else
		{
			pFan->CalTurnCnt = 50;
			if(pFan->Duty == 0)
			{
				(pFan->Duty) = START_DUTY;
			}
            else if((pFan->AimSpeed) > (pFan->CurSpeed) + 5)
			{
				temp = (pFan->AimSpeed) - (pFan->CurSpeed);
                if(temp <= 50)
				{	temp = 1;}
				else if(temp <= 100)
				{	temp = 2;}
				else if(temp <= 200)
				{	temp = 8;}
				else if(temp <= 300)
				{	temp = 32;}
				else
				{	temp = 64;}
				(pFan->Duty) += temp;
			}
			else if((pFan->AimSpeed) + 5 < (pFan->CurSpeed))
			{
				temp = (pFan->CurSpeed) - (pFan->AimSpeed);
                if(temp <= 50)
				{	temp = 1;}
				else if(temp <= 100)
				{	temp = 2;}
				else if(temp <= 200)
				{	temp = 8;}
				else if(temp <= 300)
				{	temp = 32;}
				else
				{	temp = 64;}
				(pFan->Duty) -= temp;
			}
//			else 
//            {   pFan->Duty = pFan->AimSpeed;}
            
			if((pFan->Duty) > MAX_DUTY)(pFan->Duty) = MAX_DUTY;
			else if((pFan->Duty) < MIN_DUTY)(pFan->Duty) = MIN_DUTY;
		}
	}
	else
	{
		pFan->Duty = 0;
		pFan->CalTurnCnt = 0;
	}
}
#endif
/**************************************************
*函数名称：void  F_GetDcTurn(void) 
*函数功能：获取风机转速
*入口参数：void
*出口参数：void  
**************************************************/
void F_GetDcTurn(TYPE_DC_FAN *pFan)
{
	if(pFan->TimeCnt == 0xffff)
	{
		pFan->CurSpeed = 0;
		pFan->PulseCnt = 0;
		pFan->TimeCnt = 0;
	}
	else
	{
		if(pFan->SpeedTime)
		{
			pFan->CurSpeed = 960000/(U32)(pFan->SpeedTime);
			pFan->CalTurnCnt = 0;
			pFan->SpeedTime = 0;
		}
	}

	if(pFan->AimSpeed)
	{
		if(pFan->CurSpeed < 50)
		{
			if(F_GetTimeTick(C_TIME_100ms))
			{
				if(pFan->ErrCnt < 255)pFan->ErrCnt ++;
			}
		}
		else
		{
			pFan->ErrCnt = 0;
		}
	}
	else
	{
		pFan->ErrCnt = 0;
	}
    
    if(!g_bFanPwmEn)
    {
        pFan->SpeedTime = 0;
        pFan->CurSpeed = 0;
		pFan->PulseCnt = 0;
        pFan->ErrCnt = 0;
    }
}
/**************************************************
*函数名称：void  F_DCFanErr(void) 
*函数功能：风机故障检测
*入口参数：void
*出口参数：void  
**************************************************/
void F_DCFanErr()//风扇错误判断
{
#ifdef DEBUG_FAN
	#pragma message "---------屏蔽电机故障检测---------"
#else
	if(InFan.ErrCnt >= 100)
	{	g_bInFanErr = 1;}
#endif
}
/**************************************************
*函数名称：void  F_PwmInit(void) 
*函数功能：PWM初始化
*入口参数：void
*出口参数：void  
**************************************************/
void F_PwmInit()
{
    SET_BANK_1();
    
//    CEXCR = B00100100;//P0CEX0映射到P24
    CEXCR = B00100000; //P0CEX0映射到P34   
    P0CMD = 0x41;//双沿 时钟为系统时钟的1/12
    P0CPM0 = B11110000;//PWM模式，16位相频修正PWM方式，正向输出波形(Duty为高电平有效)
    
    P0TOPH = RUN_CYCLE >> 8;
    P0TOPL = RUN_CYCLE & 0xff;

//    P0CPH0 = MAX_DUTY >> 8;
//    P0CPL0 = MAX_DUTY & 0xff;
    
//    PCACON = 0x01;//PCACON定时器使能
//    P0CPM0 |= 0x08;//使能比较捕捉模块
    
    SET_BANK_0();
}
/**************************************************
*函数名称：void  F_FanPwmSet(void) 
*函数功能：风机PWM设置
*入口参数：void
*出口参数：void  
**************************************************/
void F_FanPwmSet()
{    
    static U16 s_u16InfanPwmBck = 0;

	if(InFan.Duty)
	{
		if(s_u16InfanPwmBck != InFan.Duty)
		{
			s_u16InfanPwmBck = InFan.Duty;
            SET_BANK_1();
			P0CPH0 = InFan.Duty >> 8;
			P0CPL0 = InFan.Duty & 0xff;
            PCACON |= 0x01;
			P0CPM0 |= 0x08;
            SET_BANK_0();
		}
	}
	else if(s_u16InfanPwmBck)
	{
        SET_BANK_1();
        P0CPH0 = 0;
        P0CPL0 = 0;
		P0CPM0 &= ~0x08;
        PCACON &= ~0x01;
        SET_BANK_0();
		P_FAN_PWM = 0;
		s_u16InfanPwmBck = 0;
	}
}

void F_DCFanEn()
{
    static S16 s_s16TimeCnt = 0;
    
    if(F_GetTimeTick(C_TIME_100ms))
	{
        if(InFan.AimSpeed)
        {
            P_EN15V = 0;
            if(s_s16TimeCnt < 0)
            {   s_s16TimeCnt = 0;}
            if(s_s16TimeCnt < 5)   //开机延时500ms开驱动
            {   s_s16TimeCnt++;}
            else 
            {   g_bFanPwmEn = 1;}
        }
        else
        {
            g_bFanPwmEn = 0;
            if(s_s16TimeCnt > 0)
            {   s_s16TimeCnt = 0;}
            if(s_s16TimeCnt > -200)  //关机延时20s关电源
            {   s_s16TimeCnt--;}
            else
            {   P_EN15V = 1;}
        }
    }
}

void DCFanIRQ()
{
	//InFanDrvIRQ();
	F_InFanBckIRQ();
}

void DCFanProc()
{
    F_DCFanEn();
	F_GetDcTurn(&InFan);
	F_DCFanErr();
	F_CalDuty(&InFan);
	F_FanPwmSet();
}
