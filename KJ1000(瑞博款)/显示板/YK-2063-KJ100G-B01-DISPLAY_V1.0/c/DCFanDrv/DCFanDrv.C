#include "h\Allinclude.h"

unsigned int xdata PWMREG[14] _at_ 0x1034;	//PWM占空比调节寄存器

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
			pFan->CalTurnCnt = 30;
			if(pFan->Duty == 0)
			{
				(pFan->Duty) = START_DUTY;
			}
            else if((pFan->AimSpeed) > (pFan->CurSpeed) + 5)
			{
				temp = (pFan->AimSpeed) - (pFan->CurSpeed);
                if(temp <= 15)
				{	temp = 1;}
                else if(temp <= 50)
				{	temp = 2;}
				else if(temp <= 100)
				{	temp = 4;}
				else if(temp <= 200)
				{	temp = 6;}
				else if(temp <= 300)
				{	temp = 8;}
				else
				{	temp = 16;}
				(pFan->Duty) += temp;
			}
			else if((pFan->AimSpeed) + 5 < (pFan->CurSpeed))
			{
				temp = (pFan->CurSpeed) - (pFan->AimSpeed);
                if(temp <= 15)
				{	temp = 1;}
                else if(temp <= 50)
				{	temp = 2;}
				else if(temp <= 100)
				{	temp = 4;}
				else if(temp <= 200)
				{	temp = 6;}
				else if(temp <= 300)
				{	temp = 8;}
				else
				{	temp = 16;}
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
	if(InFan.ErrCnt >= 200)		//超过20秒转速没提升到>50
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
    PWMCON0 = 0x30;         //设置PWM系统时钟fosc/8=4MHz,独立模式，边沿对齐
    PWMCFG = 0x00;          //PWM不反向
    PWMCON1 = PWM4;         //PWM04
    PWMPDL = RUN_CYCLE & 0xff;  //周期配置低8位  频率250Hz 4000/0.25=16000
    PWMPDH = RUN_CYCLE >> 8;    //周期配置高8位
    PWMREG[10] = 0;         //PWM04占空比
    PWMCON0 |= 0x80;        //使能PWM
//    IE1 |= 0x02;          //开启中断标志位

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
            PWMREG[10] = InFan.Duty;
            PWMCON1 = PWM4; //作为PWM04
            PWMCON0 |= 0x80;//使能PWM
		}
	}
	else if(s_u16InfanPwmBck)
	{
        PWMREG[10] = 0;
        PWMCON1 = 0;     //作为IO口
        PWMCON0 &= ~0x80;//失能PWM
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
            P_EN15V = 1;
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
//            {   P_EN15V = 0;}
			{   P_EN15V = 0;}
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
