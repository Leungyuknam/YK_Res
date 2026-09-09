#include "h\Allinclude.h"

xdata U8	g_u8BellOnTime = 0,g_u8BellOffTime = 0,g_u8BellTimes = 0;
xdata U8	g_u8SetBellOnTime = 0;
xdata U8    g_u8SetBellOffTime = 0;
// ---- beep request latch: F_SetBuz registers, F_BellControl consumes ----
static U8 xdata s_u8ReqTimes   = 0;
static U8 xdata s_u8ReqOnTime  = 0;
static U8 xdata s_u8ReqOffTime = 0;
static U8 xdata g_u8BuzReq     = 0;   // bumped on each request (like g_u8BuzType)


bit g_bMuteOn = 0;
/******************************************************************
 函数名称					：	void F_BellControl()
 功能描述					：	蜂鸣器控制函数
 被本函数调用的函数列表		：	NULL
 调用本函数列表				：	main()
 调用本函数的时钟周期		：	20ms
 本函数访问的数据列表		：	r_BellTimes 鸣叫次数,r_BellOffTime 暂停时间,r_BellOnTime 鸣叫时间,
 输入参数					：	NULL
 输出参数					：	NULL
 其他说明					：	NULL
*******************************************************************/
void F_BellControl()
{
    static bit s_bMuteOnBak  = 0;
    static U8  s_u8BuzReqBak = 0;
    bit bSuppress = 0;

    if(F_GetTimeTick(C_TIME_10ms))
    {
        // 1) toggle-aware mute decision (mirrors your F_mBuzProc)
        if(s_bMuteOnBak != g_bMuteOn)
        {   s_bMuteOnBak = g_bMuteOn;}   // just toggled -> allow the confirmation beep
        else if(g_bMuteOn)
        {   bSuppress = 1;}              // steady muted -> block NEW loads only

        // 2) load a pending request unless suppressed.
        //    never touches an in-flight beep, so the tone plays full length.
        if(s_u8BuzReqBak != g_u8BuzReq)
        {
            s_u8BuzReqBak = g_u8BuzReq;
            if(!bSuppress)
            {
                g_u8BellOnTime     = 0;
                g_u8BellOffTime    = 0;
                g_u8BellTimes      = s_u8ReqTimes;
                g_u8SetBellOnTime  = s_u8ReqOnTime;
                g_u8SetBellOffTime = s_u8ReqOffTime;
            }
        }

        // 3) original beep countdown (unchanged)
        if(g_u8BellOnTime > 0)
        {   g_u8BellOnTime--;}
        else if(g_u8BellOffTime > 0)
        {   g_u8BellOffTime--;}
        else if(g_u8BellTimes > 0)
        {
            g_u8BellTimes--;
            g_u8BellOnTime  = g_u8SetBellOnTime;
            g_u8BellOffTime = g_u8SetBellOffTime;
        }
    }
}
/******************************************************************
 函数名称：						F_BellIrq()
 功能描述：						
 调用本函数的时钟周期：			125us
 输入参数：						
 输出参数：						无
 其他说明：						无
*******************************************************************/
void F_BellIrq()
{
	if(g_u8BellOnTime > 0)
	{
		P_BUZZ = ~P_BUZZ;
	}
	else 
	{
		P_BUZZ = 0;
	}

}


void F_SetBuz(U8 l_u8Times, U8 l_u8SetBellOnTime, U8 l_u8SetBellOffTime)
{
    // Only latch the request. The mute decision is made later in
    // F_BellControl at one fixed point of the main loop, so it never
    // reads a stale g_bMuteOn (which is updated AFTER this call by the
    // WiFi download path).
    s_u8ReqTimes   = l_u8Times;
    s_u8ReqOnTime  = l_u8SetBellOnTime;
    s_u8ReqOffTime = l_u8SetBellOffTime;
    g_u8BuzReq++;
}