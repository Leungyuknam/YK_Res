#define	_Native_BEEP_H
#include "Include\Include.h"

#define	PIN_PWM1_EN			b1
#define	PWM_CYC				1000
#define	PWM_DUTY			800

enum{
	YIN1,
	YIN2,
	YIN3,
	YIN4,
	YIN5,
	YIN6,
	YIN7,
	YIN_H1,
};

void PWM_ON();
void PWM_OFF();

unsigned char r_BuzzerOnTime = 0,r_BuzzerOffTime = 0,r_BuzzerType = 0,r_BuzzerCount = c_MAX_BUZZER_TIMES;

xdata U8 r_BuzTimeBuff	= 0;


xdata U8 r_u8BuzErrFlag = 0;
xdata U8 r_u8BuzTime = 0;
bit B_20MS=0;


xdata U16 g_u16BeepData=0;
xdata U16 g_u16PWM_DUTY=0;


U16 xdata r_VoiceBuf = 0;


void BuzzSet(u8 Type)
{
	BuzzSt.Set.Type = Type;
	BuzzSt.Set.Id ++;
}


void BeepSet(u8 Cnt,u8 OnT,u8 OffT,u8 type)
{
    Bper.Cnt = Cnt; 
    Bper.OnT = OnT; 
    Bper.OffT = OffT; 
	Bper.GoT = OnT + OffT; 
    Bper.Btype = type;
	//SET_CON_OUT;  //使IO输出
}


void BeepDeal(void)
{
    if (GetTimeTick(TIME_10MS) == FALSE)
		return;
    if(Bper.Cnt)
    {
        if(++Bper.GoT == Bper.OnT)     		
        {
			B_BuzzEn = FALSE;
		}
        else if(Bper.GoT >= (Bper.OnT+Bper.OffT))	
        {
        	if(Bper.Cnt > 0)
			{
                Bper.Cnt  --;
				BuzzSet(Bper.Btype);
				/*
                Buzz.Set.Type = Bper.Btype;  //长鸣
                if (++Buzz.Set.Id >= 4)
                {
                    Buzz.Set.Id = 0;
                }
                Buzz_Cnt = 0;
				B_BuzzEn = TRUE;
				*/
			}
            Bper.GoT = 0;
        }
    }
}




/*
#define	PWM_CLK				0x40
void PWM_Buzz_Init(u16 u16PWM_Cyc,u16 u16PWM_Duty)
{
 	PWMCFG  = 0xc0;		//设置PWM系统时钟:24M/8，控制输出反向  
	PWMCON  = 0X02;		//控制是否输出到IO口 
	PWMDTYA = (u16PWM_Cyc&0x03) << 6 ;		//周期与PWM0/1/2占空比的低2位设置
 	PWMPRD  = u16PWM_Cyc>>2;		//周期高8位设置	
	PWMDTY0 = 0X00;     //PWM0占空比高8位设置
	PWMDTY1 = 0x00;     //PWM1占空比高8位设置
	PWMDTY2 = 0x28;     //PWM2占空比高8位设置
	PWMDTYB = (u16PWM_Duty&0x03) << 2;     //PWM3/4/5周期与占空比的低2位设置  
	PWMDTY3 = 0X14;     //PWM3占空比高8位设置
	PWMDTY4 = u16PWM_Duty>>2;		//PWM4占空比高8位设置
	PWMDTY5 = 0X14;     //PWM5占空比高8位设置
	PWMCON |= 0x80;		//打开PWM使能端		
	//IE1 |= 0x02;
}

void PWM_Buzz_OFF()
{
	PWMCON &= ~BIT4;
	P_BuzzH = 0;
	P_BuzzL = 0; 
}
*/

//////////////////////////**********使用4K——IO驱动************////////////
//;┃(11)P2.5	 	<-->|NTC-L      [AIN3]      |BUZZ-PWM[PWM3]
//;┃(18)P0.2		<-->|FAN-L                  |BUZZ-IO[PWM2]*

//;┃(09)P2.7		<-->|||BUZZ-PWM[PWM5]     
//;┃(10)P2.6		<-->|||BUZZ-IO[PWM4]  *    
void PWM_Init(void)
{
 	PWMCFG  = 0xc0;		//设置PWM系统时钟，控制输出反向///32/foc
	PWMCON  = 0X10;		//控制是否输出到IO口
	PWMDTYA = 0xea;		//周期与PWM0/1/2占空比的低2位设置
 	PWMPRD  = 0x4f;		//周期高8位设置	
	PWMDTY0 = 0x28;     //PWM0占空比高8位设置
	PWMDTY1 = 0x28;     //PWM1占空比高8位设置
	PWMDTY2 = 0x28;     //PWM2占空比高8位设置
    ///////////////////////////////////////////
	PWMDTYB = 0X00;     //PWM3/4/5周期与占空比的低2位设置  
	PWMDTY3 = 0X14;     //PWM3占空比高8位设置
	PWMDTY4 = 0X14;		//PWM4占空比高8位设置
	PWMDTY5 = 0X14;     //PWM5占空比高8位设置
	PWMCON |= 0x80;		//打开PWM使能端
	IE1 |= 0x02;
    IP1 |= 0X02;////优先级高
	EA = 1;
}
//;┃(11)P2.5	 	<-->|NTC-L      [AIN3]      |BUZZ-PWM[PWM3]
////16M==32/Fsy=500kHZ
////500HZ-45KHZ
#define 	PWM_HZ        500UL*1000//系统频率

/******************************配置蜂鸣器频率并打开频率输出*******************************/		 
void F_FrequencyInit(unsigned int Freq)//修改为定时器模拟PWM
{
	unsigned int FreBuf;
    U16 Duty_num=0;
    U16 temp=0;
    U16 Period_Num=0;
    
    
	if( Freq == 0 )
	{	PWM_OFF();
		return ;
	}
	FreBuf  =  MAIN_FREQUENCY/TIMER_PRESCALER/Freq;
    
    Period_Num=FreBuf;///周期
    
    Duty_num=Period_Num/2;//占空比
    g_u16PWM_DUTY=Duty_num;
    
    temp=Period_Num&0x03;///PWM0-5周期SET
    PWMDTYA=temp<<6;     ///PWM0-5周期SET
    PWMPRD=Period_Num>>2;///PWM0-5周期SET
    
    temp=Duty_num&0x03;     ////占空比SET  低2位
    PWMDTYB|=temp<<2;          ////PWM3/4/5周期与占空比的低2位设置  
    PWMDTY4=Duty_num>>2;    ////占空比SET  高8位   
    
	PWM_ON(); 			//打开频率输出
}

                                        //单位是Hz
code unsigned int FrequencyTable[c_BUZZER_ALL_TYPE][c_MAX_BUZZER_TIMES] = {	//配置蜂鸣器鸣叫类型及次数
	{C_Voice_1,	C_Voice_3,	C_Voice_5,      C_Voice_H1,	C_Voice_End,},//单位是Hz
	{C_Voice_3,	C_Voice_5,	C_Voice_End,    C_Voice_End,C_Voice_End,},
	{C_Voice_5,	C_Voice_1,	C_Voice_End,    C_Voice_End,C_Voice_End,},
	{C_Voice_1,	C_Voice_3,	C_Voice_5,      C_Voice_End,C_Voice_End,},
	{C_Voice_5,	C_Voice_3,	C_Voice_1,      C_Voice_End,C_Voice_End,},
	{C_Voice_H1,C_Voice_5,	C_Voice_3,      C_Voice_1,	C_Voice_End,},
    {C_Voice_5,	C_Voice_5,	C_Voice_End,    C_Voice_End,C_Voice_End,},///6
    {C_Voice_5,	C_Voice_3,	C_Voice_1,      C_Voice_End,C_Voice_End,},
    {C_Voice_3,	C_Voice_End,C_Voice_End,    C_Voice_End,C_Voice_End,},////8
    {C_Voice_H1,C_Voice_5,  C_Voice_3,      C_Voice_1,  C_Voice_End,},////9-off
};
////20ms
code unsigned char NotePowerOnOffTime[c_BUZZER_ALL_TYPE][c_MAX_BUZZER_TIMES][2] = {	//配置蜂鸣器鸣叫时间以及间隔时间
	{	{2,5},	{3,5},	{8,0},  {8,200},  {0,0}	},
	{	{2,5},	{3,5},	{8,250},  {0,0},  {0,0}	},
	{{2,250},   {0,0},	{0,0},  {0,0},  {0,0}	},
	{	{2,50},	{2,50},	{2,250},  {0,0},  {0,0}	},
	{	{3,3},	{0,0},	{0,0},  {0,0},  {0,0}	},
	{	{3,3},	{0,0},	{0,0},  {0,0},  {0,0}	},
    {	{3,3},	{0,0},	{0,0},  {0,0},  {0,0}	},//6
    {	{3,3},	{0,0},	{0,0},  {0,0},  {0,0}	},
    {	{50,0},	{0,0},	{0,0},  {0,0},  {0,0}	},
    {	{50,0},	{0,0},	{0,0},  {0,0},  {0,0}	},///9-off
};
/****************************************************************************************/

/******************************************下面请不要更改*********************************************/
void PWM_ON()
{	
	P_FrequenceOn();
}

void PWM_OFF()	//停止频率
{	
	P_FrequenceOff();
}

void F_SetBuzzerType(unsigned char Type)
{
	r_BuzzerType = Type;
	r_BuzzerCount = 0;
	r_BuzzerOnTime = 0;
	r_BuzzerOffTime = 0;
}


void F_BuzzerControl(void)///20ms
{	
    U16 xdata x= 0;
    U8  xdata i= 0;
    U16 xdata temp= 0;
    if(g_u16BeepData)
    {
        for(i=0;i<16;i++)
        {
            temp=0x01<<i;
            if(temp&g_u16BeepData)
            {r_VoiceBuf=temp;}
        }
        g_u16BeepData=0;
        switch(r_VoiceBuf)
        {
            case C_beep_power:
                F_SetBuzzerType(C_power);
            break;
            case C_beep_keypress:
                F_SetBuzzerType(C_keypress);
            break;
            case C_beep_error:
                F_SetBuzzerType(C_error);
            break;
            case C_beep_start:
                F_SetBuzzerType(C_start);
            break;
            case C_beep_stop:
                F_SetBuzzerType(C_stop);
            break;
            case C_beep_ECO:
                F_SetBuzzerType(C_ECO);
            break;
            case C_beep_NOPRESS:
                F_SetBuzzerType(C_NOPRESS);
            break;
            case C_beep_TIPS:
                F_SetBuzzerType(C_TIPS);
            break;
            case C_beep_NOdo:
                F_SetBuzzerType(C_NOdo);
            break;
            case C_beep_OFF:
                F_SetBuzzerType(C_Off);
            break;
        }
    }
/////////////////////////////////////////////////////////////////////
	if( r_BuzzerOnTime )
	{	--r_BuzzerOnTime;
        P_BuzzerPowerOff();
	}
	else if( r_BuzzerOffTime )
	{	--r_BuzzerOffTime;
		P_BuzzerPowerOn();
	}
	else if( r_BuzzerCount < c_MAX_BUZZER_TIMES )
	{
		r_BuzzerOnTime  = NotePowerOnOffTime[r_BuzzerType][r_BuzzerCount][0];
		r_BuzzerOffTime = NotePowerOnOffTime[r_BuzzerType][r_BuzzerCount][1];
		F_FrequencyInit(FrequencyTable[r_BuzzerType][r_BuzzerCount]);
		++r_BuzzerCount;
	}
	else
	{
		PWM_OFF();	 
	}
}



/****************************************************************************************/




void F_mVoiceControl(void)
{
    if( GetTimeTick(TIME_10MS ))
    {
        B_20MS^=0x01;
        if(B_20MS)
        {
            F_BuzzerControl();
        }
    }
}








