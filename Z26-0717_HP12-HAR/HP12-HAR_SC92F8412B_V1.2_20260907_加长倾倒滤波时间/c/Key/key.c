/*--------------------------------------------------------------------
Ä£ï¿½ï¿½ï¿½ï¿½ï¿½Æ£ï¿½    ï¿½ï¿½ï¿½ï¿½É¨ï¿½ï¿½Ö·ï¿½Ä£ï¿½ï¿?
Ä£ï¿½ï¿½ï¿½ï¿½ï¿½ß£ï¿½    CJB
ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½    Í¨ï¿½ï¿½ï¿½ï¿½ï¿½Ã±ï¿½ï¿½ï¿½Êµï¿½Ö°ï¿½ï¿½ï¿½É¨ï¿½è£¬ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö·ï¿½
ï¿½Þ¸Ä¼ï¿½Â¼ï¿½ï¿½    2016-5-17 1:ï¿½ï¿½ï¿½Ó°ï¿½ï¿½ï¿½ï¿½ï¿½Ç°ï¿½ï¿½ï¿½Ë²ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ó¿ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
                        2:Ö±ï¿½Ó¸ï¿½ï¿½ï¿½Îªï¿½ï¿½ï¿½ï¿½Ö¸ï¿½ï¿½ï¿½ï¿½Ã´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
---------------------------------------------------------------------*/
#include "h\Allinclude.h"
#include "keyCfg.h"
U16 xdata r_NoKeyTime = 0;
U16 xdata r_KeyCNT = 0;
#define C_KEY_CNT_MAX 65530
#define TK(x)  ((U32)0x01 << x)

U32 xdata exKeyValueFlag = 0;		//ï¿½ï¿½Ç°ï¿½Ö°ï¿½ï¿½ï¿½ï¿½ï¿½Ö¾
U8 xdata g_u8KeyData = 0;		    //ï¿½ï¿½Ç°ï¿½Ö°ï¿½ï¿½ï¿½ï¿½ï¿½Öµ

/**************************************************
*ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ£ï¿½void ChangeTouchKeyvalue(void) 
*ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü£ï¿½×ªï¿½ï¿½TKï¿½ï¿½Öµ
*ï¿½ï¿½Ú²ï¿½ï¿½ï¿½ï¿½ï¿½void
*ï¿½ï¿½ï¿½Ú²ï¿½ï¿½ï¿½ï¿½ï¿½void
**************************************************/
// HP12-HAR£ºTK0¡«TK4 Í¨µÀ¶ÔÓ¦ 5 ¸ö´¥Ãþ°´¼ü
// TK0=ÈËÌå¸ÐÓ¦£¬TK1=¶¨Ê±£¬TK2=µµÎ»£¬TK3=Í¯Ëø£¬TK4=¿ª¹Ø
void ChangeTouchKeyvalue(void)
{
    g_u8KeyData = 0;
    if(exKeyValueFlag & TK(8)) g_u8KeyData |= c_KEY_HUMAN;
    if(exKeyValueFlag & TK(9)) g_u8KeyData |= c_KEY_TIME;
    if(exKeyValueFlag & TK(12)) g_u8KeyData |= c_KEY_GEAR;
    if(exKeyValueFlag & TK(13)) g_u8KeyData |= c_KEY_LOCK;
    if(exKeyValueFlag & TK(18)) g_u8KeyData |= c_KEY_POWER;
}

/**************************************************
*ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ£ï¿½void  Sys_Scan(void) 
*ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü£ï¿½É¨ï¿½ï¿½TKï¿½ï¿½ï¿½ï¿½Ê¾
*ï¿½ï¿½Ú²ï¿½ï¿½ï¿½ï¿½ï¿½void
*ï¿½ï¿½ï¿½Ú²ï¿½ï¿½ï¿½ï¿½ï¿½void  
**************************************************/
void Sys_Scan(void)
{   		
	if(SOCAPI_TouchKeyStatus&0x80)	    //ï¿½ï¿½Òªï¿½ï¿½ï¿½ï¿½2:  ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½É¨ï¿½ï¿½Ò»ï¿½Ö±ï¿½Ö¾ï¿½ï¿½ï¿½Ç·ï¿½ï¿½ï¿½ï¿½TouchKeyScan()Ò»ï¿½ï¿½Òªï¿½ï¿½ï¿½Ý´Ë±ï¿½Ö¾Î»ï¿½ï¿½ï¿½ï¿½ï¿?
	 {	 
		SOCAPI_TouchKeyStatus &= 0x7f;	//ï¿½ï¿½Òªï¿½ï¿½ï¿½ï¿½3: ï¿½ï¿½ï¿½ï¿½ï¿½Ö¾Î»ï¿½ï¿?ï¿½ï¿½Òªï¿½â²¿ï¿½ï¿½ï¿½ï¿½ï¿?												    
		exKeyValueFlag = TouchKeyScan();//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½   
		ChangeTouchKeyvalue();   		//×ªï¿½ï¿½ï¿½ï¿½Öµ
		TouchKeyRestart();				//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ò»ï¿½ï¿½×ªï¿½ï¿½																														 			
	}		  	   
}

U8 F_KeyScan()
{
    static xdata U8 s_u8ReadKeyData = 0;
	static xdata U8 s_u8ReadKeyDataBuff = 0;
	
	s_u8ReadKeyData = g_u8KeyData;
	if(0 == s_u8ReadKeyData)
	{
		s_u8ReadKeyDataBuff = 0;
	}
	else if(s_u8ReadKeyData >= s_u8ReadKeyDataBuff)
	{
		s_u8ReadKeyDataBuff = s_u8ReadKeyData;
	}
	else
    {
	}
	return s_u8ReadKeyDataBuff;
}


//--------------Ö®ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý²ï¿½ï¿½ï¿½Òªï¿½ï¿½ï¿½ï¿½ï¿½---------------- 
//----------------------------------------------------
//  º¯ÊýÃû³Æ      £ºF_NoKeyTimeing 
//  ÊäÈë²ÎÊý      £ºÎÞ
//  Êä³ö²ÎÊý      £ºÎÞ
//  Ó°ÏìÊý¾Ý      £ºÎÞ
//  ¹¦ÄÜÃèÊö      £º ï¿½Þ°ï¿½ï¿½ï¿½ï¿½ï¿½Ê±ï¿½ï¿½ï¿½ï¿½
//----------------------------------------------------
/*void F_NoKeyTimeing(void){
         if( r_NoKeyTime <= C_NO_KEY_TIME_MAX ){
             r_NoKeyTime++;
         }
}*/
//----------------------------------------------------
//  º¯ÊýÃû³Æ      £ºF_ResetNoKeyTime 
//  ÊäÈë²ÎÊý      £ºÎÞ
//  Êä³ö²ÎÊý      £ºÎÞ
//  Ó°ÏìÊý¾Ý      £ºÎÞ
//  ¹¦ÄÜÃèÊö      £º ï¿½ï¿½ï¿½ï¿½ï¿½Þ°ï¿½ï¿½ï¿½ï¿½ï¿½Ê±
//----------------------------------------------------
void F_ResetNoKeyTime(void){
     r_NoKeyTime = 0;
}
//----------------------------------------------------
//  º¯ÊýÃû³Æ      £ºF_GetNoKeyTime 
//  ÊäÈë²ÎÊý      £ºÎÞ
//  Êä³ö²ÎÊý      £ºr_NoKeyTime
//  Ó°ÏìÊý¾Ý      £ºÎÞ
//  ¹¦ÄÜÃèÊö      £º ï¿½ï¿½È¡Ä¿Ç°ï¿½Þ°ï¿½ï¿½ï¿½ï¿½Ä³ï¿½ï¿½ï¿½Ê±ï¿½ï¿½
//----------------------------------------------------
/*U16 F_GetNoKeyTime(void){
     return r_NoKeyTime;
}
U16 F_GetKeyKeepingTime(void){
    return r_KeyCNT;
}*/

//----------------------------------------------------
//  º¯ÊýÃû³Æ      £ºF_KeyDeal 
//  ÊäÈë²ÎÊý      £ºÎÞ
//  Êä³ö²ÎÊý      £ºÎÞ
//  Ó°ÏìÊý¾Ý      £ºÎÞ
//  ¹¦ÄÜÃèÊö      £º ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ë²¢ï¿½ï¿½Ã·Ö·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
//
//----------------------------------------------------
#define C_KEY_CODE_FILTER_CNT 3
void F_KeyDealControl(void){
    
     static xdata U8  r_KeyCodeBuf = 0;
     static xdata U8   r_KeyCodeFilterCNT = 0;
     static xdata U8  r_KeyBuf = 0;
     xdata U8          i= 0;
     xdata U8         x;
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
            r_KeyBuf = x;
            r_KeyCNT = 0;
        }else{
            if( r_KeyCNT <= C_KEY_CNT_MAX ){
                r_KeyCNT++;
            }
            //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ó¦ï¿½Ä²ï¿½ï¿½ï¿½
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
                             r_KeyCNT -= KeyEventDealCode[i].RepeatTriggerTime;
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
       r_KeyBuf = 0;
       r_KeyCNT = 0;
    } 
}

void F_mKeyControl(void)
{
    if(F_GetTimeTick(C_TIME_10ms))
    {
		Sys_Scan();
        F_KeyDealControl();
    }
}