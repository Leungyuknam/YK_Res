#include "h\AllInclude.h"

U8 xdata g_u8SoundTime = 0;//蜂鸣时间
U8 xdata g_u8MuteTime = 0;//静音时间

U8 xdata g_u8SoundTimeBak = 0;//蜂鸣时间备份
U8 xdata g_u8MuteTimeBak = 0;//静音时间备份

U8 xdata g_u8BeepTimes = 0;//蜂鸣次数

U8 xdata g_u8BuzMode = 0;//蜂鸣模式
U8 xdata g_u8BuzType = 0;//蜂鸣切换

bit g_bMuteOn = 0;

U8 xdata g_u8BuzClk = 0;//蜂鸣频率
U16 code t_BuzClkTaple[5] = {0,4444,4000,3478,2857};
    
/**************************************************
*函数名称：void F_BuzzerSet(void) 
*函数功能：蜂鸣设置
*入口参数：void
*出口参数：void  
**************************************************/
void F_BuzzerSet(U8 l_u8Flg)	
{	
    g_u8BuzMode = l_u8Flg;
    g_u8BuzType++;
}
/**************************************************
*函数名称：void F_mBuzProc(void) 
*函数功能：蜂鸣调用
*入口参数：void
*出口参数：void  
**************************************************/
void F_mBuzProc()	
{	
    static U8 s_u8BuzTypeBak = 0;
    static U8 s_u8BuzClkBak = 0;
    static bit s_bMuteOnBak = 0;
    static U8 s_u8Delay = 0;
    
	if(F_GetTimeTick(C_TIME_10ms))
	{
        if(s_bMuteOnBak != g_bMuteOn)
        {   s_bMuteOnBak = g_bMuteOn;}
        else if(g_bMuteOn)
        {   g_u8BuzMode = BUZ_OFF;}
        
        if(s_u8BuzTypeBak != g_u8BuzType)
        {
            s_u8BuzTypeBak = g_u8BuzType;
            s_u8BuzClkBak = 0;
            if(BUZ_POWER == g_u8BuzMode)
            {
                g_u8SoundTime = g_u8SoundTimeBak = 8;
                g_u8MuteTime = g_u8MuteTimeBak = 12;
                g_u8BeepTimes = 3;
                g_u8BuzClk = c_BUZ_CLK_1800;
            }
            else if(BUZ_SHORT == g_u8BuzMode)
            {
                g_u8SoundTime = g_u8SoundTimeBak = 8;
                g_u8MuteTime = g_u8MuteTimeBak = 12;
                g_u8BeepTimes = 0;
                g_u8BuzClk = c_BUZ_CLK_2300;
            }
            else if(BUZ_SHORT_2 == g_u8BuzMode)
            {
                g_u8SoundTime = g_u8SoundTimeBak = 8;
                g_u8MuteTime = g_u8MuteTimeBak = 12;
                g_u8BeepTimes = 1;
                g_u8BuzClk = c_BUZ_CLK_2300;
            }
            else if(BUZ_SHORT_3 == g_u8BuzMode)
            {
                g_u8SoundTime = g_u8SoundTimeBak = 8;
                g_u8MuteTime = g_u8MuteTimeBak = 12;
                g_u8BeepTimes = 2;
                g_u8BuzClk = c_BUZ_CLK_2300;
            }            
            else if(BUZ_UP == g_u8BuzMode)
            {
                g_u8SoundTime = g_u8SoundTimeBak = 5;
                g_u8MuteTime = g_u8MuteTimeBak = 8;
                g_u8BeepTimes = 2;
                g_u8BuzClk = c_BUZ_CLK_2000;
            }
            else if(BUZ_DOWN == g_u8BuzMode)
            {
                g_u8SoundTime = g_u8SoundTimeBak = 5;
                g_u8MuteTime = g_u8MuteTimeBak = 8;
                g_u8BeepTimes = 2;
                g_u8BuzClk = c_BUZ_CLK_2800;
            }
            else if(BUZ_LOCK == g_u8BuzMode)
            {
                g_u8SoundTime = g_u8SoundTimeBak = 8;
                g_u8MuteTime = g_u8MuteTimeBak = 92;
                g_u8BeepTimes = 1;
                g_u8BuzClk = c_BUZ_CLK_2300;
            }            
            else
            {
                g_u8SoundTime = g_u8SoundTimeBak = 0;
                g_u8MuteTime = g_u8MuteTimeBak = 0;
                g_u8BeepTimes = 0;
                g_u8BuzClk = 0;
            }
        }
        
		if(g_u8SoundTime)
		{	g_u8SoundTime--;}
		else if(g_u8MuteTime)
		{	g_u8MuteTime--;}
        else if(g_u8BeepTimes)
		{	
            g_u8BeepTimes--;;
            g_u8SoundTime = g_u8SoundTimeBak;
            g_u8MuteTime = g_u8MuteTimeBak;
            s_u8BuzClkBak = 0;
            switch(g_u8BuzMode)
            {
                case BUZ_POWER:
                    if(2 == g_u8BeepTimes)
                    {   g_u8BuzClk = c_BUZ_CLK_2000;}
                    else if(1 == g_u8BeepTimes)
                    {   g_u8BuzClk = c_BUZ_CLK_2300;}
                    else
                    {   g_u8BuzClk = c_BUZ_CLK_2800;}
                    break;
                
                case BUZ_UP:
                    if(1 == g_u8BeepTimes)
                    {   g_u8BuzClk = c_BUZ_CLK_2000;}
                    else
                    {   g_u8BuzClk = c_BUZ_CLK_2300;}
                    break;

                case BUZ_DOWN:
                    if(1 == g_u8BeepTimes)
                    {   g_u8BuzClk = c_BUZ_CLK_2300;}
                    else
                    {   g_u8BuzClk = c_BUZ_CLK_2000;}
                    break;

                case BUZ_SHORT_2:
                case BUZ_SHORT_3:
                case BUZ_LOCK:
                    g_u8BuzClk = c_BUZ_CLK_2300;
                    break;
                    
                default:
                    break;                
            }
		}
		else
        {   }
        
        if(s_u8BuzClkBak != g_u8BuzClk)
        {
            s_u8BuzClkBak = g_u8BuzClk;
            if(g_u8BuzClk)
            {   Timer4_Set(t_BuzClkTaple[g_u8BuzClk]);}
            s_u8Delay = 100;
        }
        else if(s_u8Delay)
        {   s_u8Delay--;}
        else
        {   
            Time4cli();
            P_BUZPWM = 0;
        }
        
        if(g_u8SoundTime)
        {   P_BUZEN = 1;}
        else
        {   P_BUZEN = 0;}
	}
}
/******************************************************************
 函数名称：						F_Buzzer_IRQ();
 功能描述：						蜂鸣运行；
 调用周期：						125us
 输入参数：						无
 输出参数：						无
*******************************************************************/
void F_Buzzer_IRQ()		
{	
    P_BUZPWM = !P_BUZPWM;
}