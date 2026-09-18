/*--------------------------------------------------------------------
模块名称：    按键扫描分发模块
模块作者：    CJB
功能描述：    通过配置表格实现按键扫描，处理函数分发
修改记录：    2016-5-17 1:增加按键码前端滤波，增加抗干扰能力
                        2:直接更改为函数指针调用处理函数
              2018-10-22
                        1：更改KeyCfg.h 使配置集中   祖铭阳
---------------------------------------------------------------------*/
 

#include "Include\Typedef.h"
#include "MyLib\KeyEvent\KeyEvent.h"
#include "MyLib\Key\keyCfg.h"
#include "MyLib\Key\Key.h"

u16 X_REG r_NoKeyTime = 0;
u16 X_REG r_KeyCNT = 0;
u16 X_REG r_KeySameRepeat_Cnt = 0;
X_REG Key_Enum  mKeyStatus = KEY_RELEASE;


#define C_KEY_CNT_MAX 65530







//--------------之后的内容不需要你更改----------------
//----------------------------------------------------
//  Functiob Name : F_NoKeyTimeing 
//  Input         : NULL
//  Output        : NULL
//  Influence Data: NULL
//  Description   : 无按键计时函数
//----------------------------------------------------
//void F_NoKeyTimeing(void){
//         if( r_NoKeyTime <= C_NO_KEY_TIME_MAX ){
//             r_NoKeyTime++;
//         }
//}
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
//U16 F_GetNoKeyTime(void){
//     return r_NoKeyTime;
//}
//U16 F_GetKeyKeepingTime(void){
//    return r_KeyCNT;
//}



//----------------------------------------------------
//  Functiob Name : F_KeyDeal 
//  Input         : NULL
//  Output        : NULL
//  Influence Data: NULL
//  Description   : 处理按键码并获得分发按键号
//
//----------------------------------------------------
//为了适配电源键，长短按功能，改动了下持续长按的处理。	2023、06、14
#define C_KEY_CODE_FILTER_CNT 3
void F_KeyDealControl(void){
    
     static U32 X_REG r_KeyCodeBuf = 0;
     static U8 X_REG r_KeyCodeFilterCNT = 0;
     static U32 X_REG r_KeyBuf = 0;
	  static U8 X_REG r_Keyrelease_Cnt = 0;
     U8 X_REG i= 0;
     U32 X_REG x;
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
	 	r_Keyrelease_Cnt = 0;
        if( x != r_KeyBuf ){
            r_KeyBuf = x;
            r_KeyCNT = 0;
			r_KeySameRepeat_Cnt = 0;
        }else{
            if( r_KeyCNT <= C_KEY_CNT_MAX ){
                r_KeyCNT++;
            }
            //处理单次响应的部分
            for( i = 0 ; ; i++ ){
                 if( KeyEventDealCode[i].KeyValue == 0 )break;
                 if( KeyEventDealCode[i].KeyValue ==  r_KeyBuf ){
                     if( KeyEventDealCode[i].IfRepeat == 0 ){
                         if( r_KeyCNT == KeyEventDealCode[i].FirstTriggerTime ){
							 (KeyEventDealCode[i].voidKeyEvent)();                             
                             F_ResetNoKeyTime(); 
                         }
                     }else{
                         if( r_KeyCNT == KeyEventDealCode[i].FirstTriggerTime ){   
                             (KeyEventDealCode[i].voidKeyEvent)();
							 r_KeySameRepeat_Cnt = 0;
                         }
						 else if( r_KeyCNT > KeyEventDealCode[i].FirstTriggerTime )
						 {
							if(r_KeySameRepeat_Cnt < C_KEY_CNT_MAX)
								r_KeySameRepeat_Cnt ++;

							if(r_KeySameRepeat_Cnt == KeyEventDealCode[i].RepeatTriggerTime)
							{
								(KeyEventDealCode[i].voidKeyEvent)();
								r_KeySameRepeat_Cnt = 0;
							}
						 }
						
                     } 
                 }
            }
       }
    }else{
       if(  r_KeyBuf != 0 ){
            for( i = 0 ; ; i++ ){
                 if( ReleseKeyEventDealCode[i].KeyValue == 0 )break;
                 if( ReleseKeyEventDealCode[i].KeyValue == r_KeyBuf ){
                     if(  (r_KeyCNT >=  ReleseKeyEventDealCode[i].MinReleseTime)
                        &&(r_KeyCNT <=  ReleseKeyEventDealCode[i].MaxReleseTime) ){
                           (ReleseKeyEventDealCode[i].voidReleseKeyEvent)();
                        }
                        F_ResetNoKeyTime(); 
                 }
            }
       }
	   if(r_Keyrelease_Cnt <255)
	   		r_Keyrelease_Cnt ++;
       r_KeyBuf = 0;
       r_KeyCNT = 0;
    } 
}



////////////////////////////////////////////////





