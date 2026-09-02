/*--------------------------------------------------------------------
模块名称：    按键扫描分发模块
模块作者：    CJB
功能描述：    通过配置表格实现按键扫描，处理函数分发
修改记录：    2016-5-17 1:增加按键码前端滤波，增加抗干扰能力
                        2:直接更改为函数指针调用处理函数
---------------------------------------------------------------------*/
#include "h\Allinclude.h"
#include "keyCfg.h"

U16 xdata r_NoKeyTime = 0;
U16 xdata r_KeyCNT = 0;
#define C_KEY_CNT_MAX 65530

U32 exKeyValueFlag = 0;		//当前轮按键标志
U16 xdata g_u16KeyState = 0;
// ==============================================================
// 串口触摸数据位定义 (请根据中间MCU实际发送的协议调整对应关系)
// ==============================================================
#define BIT_KEY_POWER   0x01  // BIT0: 电源键
#define BIT_KEY_LOCK    0x02  // BIT1: 童锁键
#define BIT_KEY_MODE    0x04  // BIT2: 模式键
#define BIT_KEY_ION     0x08  // BIT3: 负离子键
#define BIT_KEY_TIME    0x10  // BIT4: 定时键
#define BIT_KEY_WIFI    0x20  // BIT5: WIFI键

/**************************************************
*函数名称：U16 F_KeyScan(void)
*函数功能：从串口缓存读取按键，并映射为系统的逻辑按键码
**************************************************/
U16 F_KeyScan(void)
{
    U16 l_u16KeyState = 0;
    
    // 读取最新的串口触摸数据
    U8 currentTouch = g_u8TouchData; 

    // *组合按键判断
    // 原 TK19(TIME) & TK18(WIFI)
    if((currentTouch & BIT_KEY_TIME) && (currentTouch & BIT_KEY_WIFI))
    {
        l_u16KeyState |= C_KEY_CHANGE_RGB;  
    }
    // 原 TK24(MODE) & TK25(LOCK)
    else if((currentTouch & BIT_KEY_MODE) && (currentTouch & BIT_KEY_LOCK))
    {
        l_u16KeyState |= c_KEY_FILTER;  
    }
    // 原 TK24(MODE) & TK23(ION)
    else if((currentTouch & BIT_KEY_MODE) && (currentTouch & BIT_KEY_ION))
    {
        l_u16KeyState |= C_KEY_QUICK_TEST;  
    }
	else if((currentTouch & BIT_KEY_POWER) && (currentTouch & BIT_KEY_LOCK))
    {
        l_u16KeyState |= C_KEY_TEST;  
    }
    
    // *单一按键判断
    else if(currentTouch & BIT_KEY_POWER)
    {
        l_u16KeyState |= c_KEY_POWER;
    }
    else if(currentTouch & BIT_KEY_LOCK)
    {
        l_u16KeyState |= c_KEY_LOCK;            
    }
    else if(currentTouch & BIT_KEY_MODE)
    {
        l_u16KeyState |= c_KEY_MODE;            
    }
    else if(currentTouch & BIT_KEY_ION)
    {
        l_u16KeyState |= c_KEY_ION;         
    }
    else if(currentTouch & BIT_KEY_TIME)
    {
        l_u16KeyState |= c_KEY_TIME;            
    }
    else if(currentTouch & BIT_KEY_WIFI)
    {
        l_u16KeyState |= c_KEY_WIFI;            
    }
    
    g_u16KeyState = l_u16KeyState;
    return l_u16KeyState;
}

//--------------之后的内容不需要你更改---------------- 
//----------------------------------------------------
//  Functiob Name : F_NoKeyTimeing 
//  Input         : NULL
//  Output        : NULL
//  Influence Data: NULL
//  Description   : 无按键计时函数
//----------------------------------------------------
/*void F_NoKeyTimeing(void){
         if( r_NoKeyTime <= C_NO_KEY_TIME_MAX ){
             r_NoKeyTime++;
         }
}*/
//----------------------------------------------------
//  Functiob Name : F_ResetNoKeyTime 
//  Input         : NULL
//  Output        : NULL
//  Influence Data: NULL
//  Description   : 清零无按键计时
//----------------------------------------------------
void F_ResetNoKeyTime(void){
     r_NoKeyTime = 0;
}
//----------------------------------------------------
//  Functiob Name : F_GetNoKeyTime 
//  Input         : NULL
//  Output        : r_NoKeyTime
//  Influence Data: NULL
//  Description   : 获取目前无按键的持续时间
//----------------------------------------------------
/*U16 F_GetNoKeyTime(void){
     return r_NoKeyTime;
}
U16 F_GetKeyKeepingTime(void){
    return r_KeyCNT;
}*/

//----------------------------------------------------
//  Functiob Name : F_KeyDeal 
//  Input         : NULL
//  Output        : NULL
//  Influence Data: NULL
//  Description   : 处理按键码并获得分发按键号
//
//----------------------------------------------------
#define C_KEY_CODE_FILTER_CNT 3
void F_KeyDealControl(void){
    
     static U16 xdata r_KeyCodeBuf = 0;
     static U8  xdata r_KeyCodeFilterCNT = 0;
     static U16 xdata r_KeyBuf = 0;
     static bit s_bKeyFlg = 0;
     U8         xdata i= 0;
     U16        xdata x;

     x = F_KeyScan();

     if( r_KeyCodeBuf != x ){
         if( ++r_KeyCodeFilterCNT >= C_KEY_CODE_FILTER_CNT){
             r_KeyCodeBuf = x;
             r_KeyCodeFilterCNT = 0;
         }
     }else{
         r_KeyCodeFilterCNT = 0;
     }
     
     x = r_KeyCodeBuf;
     
     if( x != 0 ){
        if( x != r_KeyBuf ){
            if(!s_bKeyFlg){
                r_KeyBuf = x;
            }else{   
                r_KeyBuf = 0;
            }
            r_KeyCNT = 0;
        }else{
            if( r_KeyCNT <= C_KEY_CNT_MAX ){
                r_KeyCNT++;
            }
            //处理单次响应的部分
			 
            for( i = 0 ; ; i++ ){
                 if( T_passKeyTable[i].keyCode == 0 )break;
                 if( T_passKeyTable[i].keyCode ==  r_KeyBuf ){
                     if( T_passKeyTable[i].lopKey == 0 ){
                         if( r_KeyCNT == T_passKeyTable[i].KeyConfrimTime ){
                             s_bKeyFlg = 1;
							 (*T_passKeyTable[i].passKeyFun)();   
                             F_ResetNoKeyTime(); 
                         }
                     }else{
                         if( r_KeyCNT == T_passKeyTable[i].KeyConfrimTime ){  
                                s_bKeyFlg = 1;
								(*T_passKeyTable[i].passKeyFun)();
								r_KeyCNT -= T_passKeyTable[i].lopKeyInnerTime;
                         }
                     } 
                 }
            }
       }
    }else{
       if(  r_KeyBuf != 0 ){
            for( i = 0 ; ; i++ ){
                 if( T_releaseKeyTable[i].keyCode == 0 )break;
                 if( T_releaseKeyTable[i].keyCode  == r_KeyBuf ){
                     if(  (r_KeyCNT >=  T_releaseKeyTable[i].releaseMixTime)
                        &&(r_KeyCNT <=  T_releaseKeyTable[i].releaseMaxTime) ){
                                (*T_releaseKeyTable[i].releaseFun)();
                     }
                     F_ResetNoKeyTime(); 
                 }
            }
       }
       s_bKeyFlg = 0;
       r_KeyBuf = 0;
       r_KeyCNT = 0;
    } 
}

void F_mKeyControl(void)
{
    if(F_GetTimeTick(C_TIME_10ms))
    {
        F_KeyDealControl();
    }
}

////////////////////////////////////////////////