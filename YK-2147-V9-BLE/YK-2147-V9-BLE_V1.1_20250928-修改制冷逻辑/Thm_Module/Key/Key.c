/*--------------------------------------------------------------------
模块名称：    按键扫描分发模块
模块作者：    CJB
功能描述：    通过配置表格实现按键扫描，处理函数分发
修改记录：    2016-5-17 1:增加按键码前端滤波，增加抗干扰能力
                        2:直接更改为函数指针调用处理函数
              2018-10-22
                        1：更改KeyCfg.h 使配置集中   祖铭阳
---------------------------------------------------------------------*/
//头文件
#include "COMMON\AllInclude.h"
#include "Key\KeyCfg.h"


u16 xdata g_u16NoKeyTime  = 0;

#define C_KEY_CNT_MAX 65530

//----------------------------------------------------
//  Functiob Name : F_ResetNoKeyTime 
//  Input         : NULL
//  Output        : NULL
//  Influence Data: NULL
//  Description   : 清零无按键计时
//----------------------------------------------------
static void F_ResetNoKeyTime(void)
{
     g_u16NoKeyTime  = 0;
}
//----------------------------------------------------
//  Functiob Name : F_KEY_DealControl 
//  Input         : NULL
//  Output        : NULL
//  Influence Data: NULL
//  Description   : 处理按键码并获得分发按键号
//
//----------------------------------------------------
#define C_KEY_CODE_FILTER_CNT 5

void F_KEY_Handle(void)
{
    
#if (KEY_NUMBERS > 16)	
     static u32 xdata s_NowKeyScanBuff = 0;
     u32        xdata l_KeyScanBuff = 0;
     static u32 xdata s_KeyValue = 0;

#elif (KEY_NUMBERS > 8)	
     static u16 xdata s_NowKeyScanBuff = 0;
     u16        xdata l_KeyScanBuff = 0;
     static u16 xdata s_KeyValue = 0;

#else
     static u8 xdata s_NowKeyScanBuff = 0;
     u8        xdata l_KeyScanBuff = 0;
     static u8 xdata s_KeyValue = 0;
#endif

     static u8  xdata s_u8KeyCodeFilterCnt = 0;
     static u16 xdata s_u16KeyDownCnt = 0;
     u8         xdata i= 0;

	 //扫描键值
	 l_KeyScanBuff = F_KeyScan();
//	 g_u8DispKeyBuff = l_KeyScanBuff;
     
	 //滤波
     if( s_NowKeyScanBuff != l_KeyScanBuff )
	 {
         if( ++s_u8KeyCodeFilterCnt >= C_KEY_CODE_FILTER_CNT)
		 {
             s_NowKeyScanBuff = l_KeyScanBuff;
             s_u8KeyCodeFilterCnt = 0;
			 g_CarRfgrt.u16FullDisp = 22500;
         }
     }
	 else
	 {
         s_u8KeyCodeFilterCnt = 0;
     }
     
     l_KeyScanBuff = s_NowKeyScanBuff;
     
	 //有按键按下，则进入键值处理
     if( l_KeyScanBuff != 0 )
	 {
	 	//键值改变，重置按下时间
        if( l_KeyScanBuff != s_KeyValue )
		{
            s_KeyValue = l_KeyScanBuff;
            s_u16KeyDownCnt = 0;
        }

		//键值持续按下
		else
		{
			//按键持续按下的时间
            if( s_u16KeyDownCnt <= C_KEY_CNT_MAX )
			{
                s_u16KeyDownCnt++;
            }

            //处理单次响应的部分
            for( i = 0 ; ; i++ )
			{
				//全部事件遍历完成，退出
				if( KeyEventDealCode[i].KeyValue == 0 )
				{
					break;
				}

				//根据键值处理相应的按键事件
				if( KeyEventDealCode[i].KeyValue == s_KeyValue )
				{
					 //只执行一次的按键事件
					 if( KeyEventDealCode[i].IfRepeat == 0 )
					 {
					     if( s_u16KeyDownCnt == KeyEventDealCode[i].FirstTriggerTime )
						 {
							 (KeyEventDealCode[i].voidKeyEvent)();                             
					         F_ResetNoKeyTime(); 
					     }
					 }
					 
					 //重复执行的按键事件
					 else
					 {
					     if( s_u16KeyDownCnt == KeyEventDealCode[i].FirstTriggerTime )
						 {   
					         (KeyEventDealCode[i].voidKeyEvent)();
					         s_u16KeyDownCnt -= KeyEventDealCode[i].RepeatTriggerTime;
					     }
					 } 
				}

            }
       }
    }

	else
	{
	   //按键释放
       if(  s_KeyValue != 0 )
	   {
            for( i = 0 ; ; i++ )
			{
                 if( ReleseKeyEventDealCode[i].KeyValue == 0 )
				 {
				 	break;
				 }

				 //根据键值和持续按下的时间处理相应事件
                 if( ReleseKeyEventDealCode[i].KeyValue == s_KeyValue )
				 {
					if(  (s_u16KeyDownCnt >=  ReleseKeyEventDealCode[i].MinReleseTime)
					&&(s_u16KeyDownCnt <=  ReleseKeyEventDealCode[i].MaxReleseTime) )
					{
						(ReleseKeyEventDealCode[i].voidReleseKeyEvent)();
					}
					F_ResetNoKeyTime(); 
                 }
            }
       }
       s_KeyValue = 0;
       s_u16KeyDownCnt = 0;
	   g_u8UpKeyRepeat = 0;
    } 
}