#include "h\Allinclude.h"

#define StiveSendBuf UartTXDBuf
#define StiveRecBuf UartRXDBuf

U8 xdata g_u8StiveLinkTime = 10;
U8 xdata g_u8StiveState = C_STIVE_OFF;//粉尘传感器状态

U16 xdata g_u16Pm25Data = 0;//PM25数值 单位ug/m3
U16 xdata g_u16Pm10Data = 0;//PM10数值 单位ug/m3
U16 xdata g_u16Pm1Data = 0; //PM1.0数值 单位ug/m3 

U8 StiveCheckSum(U8 *ucPoint,U8 length)
{
	U8 i,j = 0;
	for (i = 0; i < length; i++)
	{
		j += ucPoint[i];
	}
    j = (~j) + 1;
    
	return	j;
}
/******************************************************************
 函数名称：						F_READ_STIVE_DATA()
 功能描述：						读取粉尘测量结果
 入口参数：                     void
 出口参数：                     数据长度  
*******************************************************************/
U8 F_READ_STIVE_DATA()
{
    StiveSendBuf[0] = 0x11;
    StiveSendBuf[1] = 0x02;
    StiveSendBuf[2] = 0x0B;
    StiveSendBuf[3] = 0x01;
//    StiveSendBuf[4] = 0xE1;
    StiveSendBuf[4] = StiveCheckSum(StiveSendBuf,4);
    return 5;
}
/******************************************************************
 函数名称：						F_ON_STIVE_SCAN()
 功能描述：						开启粉尘测量
 入口参数：                     void
 出口参数：                     数据长度  
*******************************************************************/
U8 F_ON_STIVE_SCAN()
{
    StiveSendBuf[0] = 0x11;
    StiveSendBuf[1] = 0x03;
    StiveSendBuf[2] = 0x0C;
    StiveSendBuf[3] = 0x02;
    StiveSendBuf[4] = 0x1E;
//    StiveSendBuf[5] = 0xC0;
    StiveSendBuf[5] = StiveCheckSum(StiveSendBuf,5);
    return 6;
}
/******************************************************************
 函数名称：						F_OFF_STIVE_SCAN()
 功能描述：						关闭粉尘测量
 入口参数：                     void
 出口参数：                     数据长度  
*******************************************************************/
U8 F_OFF_STIVE_SCAN()
{
    StiveSendBuf[0] = 0x11;
    StiveSendBuf[1] = 0x03;
    StiveSendBuf[2] = 0x0C;
    StiveSendBuf[3] = 0x01;
    StiveSendBuf[4] = 0x1E;
    StiveSendBuf[5] = StiveCheckSum(StiveSendBuf,5);
    return 6;
}
/******************************************************************
 函数名称：						F_QUERY_STIVE_STATE()
 功能描述：						查询粉尘测量状态
 入口参数：                     void
 出口参数：                     数据长度  
*******************************************************************/
U8 F_QUERY_STIVE_STATE()
{
    StiveSendBuf[0] = 0x11;
    StiveSendBuf[1] = 0x01;
    StiveSendBuf[2] = 0x0C;
    StiveSendBuf[3] = StiveCheckSum(StiveSendBuf,3);
    return 4;
}
/******************************************************************
 函数名称：						F_StiveRecProc()
 功能描述：						粉尘通讯接收
入口参数：                      length:接收长度
 出口参数：                     void
*******************************************************************/
void F_StiveRecProc(U8 length)
{	
	if(  (StiveRecBuf[HEAD_FIRST] == FRAME_FIRST)
	   &&(StiveRecBuf[length-1] == StiveCheckSum(StiveRecBuf,length-1)))
	{
		g_u8StiveLinkTime = c_STIVE_LINK_ERR_TIME;
        if((0x02 == StiveRecBuf[FRAME_TYPE_H]) && (0x0C == StiveRecBuf[FRAME_TYPE_L]))
        {
            g_u8StiveState = StiveRecBuf[FRAME_DATA];
        }
        else if((0x11 == StiveRecBuf[FRAME_TYPE_H]) && (0x0B == StiveRecBuf[FRAME_TYPE_L]))
        {
            g_u16Pm25Data = (StiveRecBuf[5] << 8) | StiveRecBuf[6];
            g_u16Pm10Data = (StiveRecBuf[9] << 8) | StiveRecBuf[10];
            g_u16Pm1Data = (StiveRecBuf[13] << 8) | StiveRecBuf[14];
            if(g_u16Pm25Data > c_STIVE_DATA_MAX)
            {   g_u16Pm25Data = c_STIVE_DATA_MAX;}
            if(g_u16Pm10Data > c_STIVE_DATA_MAX)
            {   g_u16Pm10Data = c_STIVE_DATA_MAX;}
            if(g_u16Pm1Data > c_STIVE_DATA_MAX)
            {   g_u16Pm1Data = c_STIVE_DATA_MAX;}            
        }
	}
}
/******************************************************************
 函数名称：						F_StiveTask()
 功能描述：						粉尘发送任务
入口参数：                      Command:指令
 出口参数：                     void
*******************************************************************/
void F_StiveTask(U8 Command)
{
	switch(Command)
    {
        case STIVE_CMD_READ:
            AddFunc(&F_READ_STIVE_DATA,0,1);
            break;

        case STIVE_CMD_ON:
            AddFunc(&F_ON_STIVE_SCAN,0,1);
            break;

        case STIVE_CMD_OFF:
            AddFunc(&F_OFF_STIVE_SCAN,0,1);
            break;

        case STIVE_CMD_QUERY:
            AddFunc(&F_QUERY_STIVE_STATE,0,1);
            break;

        default:
            break;
    }
}
void F_StiveCommucate()
{
    static U8 temp = 3;
    static U8 s_u8DispDataType = 0;
    
    if(F_GetTimeTick(C_TIME_1s))
    {
        if(g_bSenseLoad)
        {
            if(g_u8StiveLinkTime)
            {   g_u8StiveLinkTime --;}
            else
            {   g_u8StiveState = C_STIVE_ERR;}
            if(C_STIVE_ON == g_u8StiveState)
            {   F_StiveTask(STIVE_CMD_READ);}
            else
            {   F_StiveTask(STIVE_CMD_ON);}
            //粉尘显示数据处理
            if(++s_u8DispDataType >= 5)
            {
                s_u8DispDataType = 0;
                temp = (g_u8DispCnt % 5) + 1;
            }
            
            if(g_u16Pm25Data < 5)
            {   g_u16DispPm25Data = temp;}
            else
            {   g_u16DispPm25Data = g_u16Pm25Data;}
            
            if(g_u16Pm10Data < 5)
            {   g_u16DispPm10Data = temp;}
            else
            {   g_u16DispPm10Data = g_u16Pm10Data;}
            
            if(g_u16Pm1Data < 5)
            {   g_u16DispPm1Data = temp;}
            else
            {   g_u16DispPm1Data = g_u16Pm1Data;}            
        }
    }
}