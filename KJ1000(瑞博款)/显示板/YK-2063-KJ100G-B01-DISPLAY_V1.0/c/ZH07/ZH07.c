#include "h\Allinclude.h"

#define StiveSendBuf UartTXDBuf
#define StiveRecBuf UartRXDBuf

U8 xdata g_u8StiveLinkTime = 10;
U8 xdata g_u8StiveState = C_STIVE_OFF;//粉尘传感器状态

U8 xdata g_u8StiveSendState = 0;//粉尘传感器通信发送状态

U16 xdata g_u16Pm25Data = 0;//PM25数值 单位ug/m3
U16 xdata g_u16Pm10Data = 0;//PM10数值 单位ug/m3
U16 xdata g_u16Pm1Data = 0; //PM1.0数值 单位ug/m3 

bit g_bGetSumFlg = 0;          //已获取总和标记
U8 xdata g_u8BufIndex = 0;     //缓存索引
U16 xdata g_u8Pm25Sum = 0;     //平均数总和
U16 xdata t_Pm25DataBuf[PM25_BUF_SIZE] = 0;//PM25数值BUF

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
	if(  (StiveRecBuf[HEAD_FIRST] == 0x16)
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
    g_u8StiveSendState = Command;
}
/******************************************************************
 函数名称：						F_StiveSendCode()
 功能描述：						粉尘串口发送
 入口参数：                     void
 出口参数：                     发送长度
*******************************************************************/
U8 F_StiveSendCode()
{
    U8 temp = 0;
    
	switch(g_u8StiveSendState)
    {
        case STIVE_CMD_READ:
            temp = F_READ_STIVE_DATA();
            break;

        case STIVE_CMD_ON:
            temp = F_ON_STIVE_SCAN();
            break;

        case STIVE_CMD_OFF:
            temp = F_OFF_STIVE_SCAN();
            break;

        case STIVE_CMD_QUERY:
            temp = F_QUERY_STIVE_STATE();
            break;

        default:
            temp = 0;
            break;
    }
    g_u8StiveSendState = 0;
    
    return temp;
}

void F_StiveCommucate()
{
    static U8 temp = 3;
    static U8 s_u8ErrNum = 8;
    static U8 s_u8DispDataType = 0;
    static U8 s_u8DispErrDataType = 0;
    static U16 g_u16DispPm25DataBak = 5;
    static U8 s_u8DispDelay = 0;
    static U8 s_u8DelayTime = 0;
    
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
            if(C_STIVE_ERR == g_u8StiveState)
            {
                if(++s_u8DispErrDataType >= 8)
                {
                    s_u8DispErrDataType = 0;
                    s_u8ErrNum = (g_u8DispCnt % 4) + 6;
                }
                g_u16DispPm25Data = g_u16DispPm25DataBak = s_u8ErrNum;
                g_u16DispPm25Data = s_u8ErrNum;
                g_u16DispPm10Data = s_u8ErrNum;
                g_u16DispPm1Data = s_u8ErrNum;
            }
            else
            {
                //随机数5s获取
                if(++s_u8DispDataType >= 5)
                {
                    s_u8DispDataType = 0;
                    temp = (g_u8DispCnt % 5) + 1;
                }
                //实际显示数值赋值判断
                if(g_u16DispPm25Data >= g_u16DispPm25DataBak)
                {
                    if(g_u16DispPm25Data - g_u16DispPm25DataBak >= 10)
                    {   s_u8DelayTime = 5;}
                    else
                    {   s_u8DelayTime = 64;}
                }
                else 
                {
                    if(g_u16DispPm25DataBak - g_u16DispPm25Data >= 10)
                    {   s_u8DelayTime = 5;}
                    else
                    {   s_u8DelayTime = 64;}
                }
                if(++s_u8DispDelay >= s_u8DelayTime)
                {
                    s_u8DispDelay = 0;
                    g_u16DispPm25Data = g_u16DispPm25DataBak;
                }
                //要求显示数值判断
                if(g_u16Pm25Data < 5)
                {   g_u16DispPm25DataBak = temp;}
                else if(g_u16Pm25Data < 20)
                {   g_u16DispPm25DataBak = g_u16Pm25Data;}
                else
                {
                    if(g_bGetSumFlg)
                    {
                        g_u8Pm25Sum -= t_Pm25DataBuf[g_u8BufIndex];
                        t_Pm25DataBuf[g_u8BufIndex] = g_u16Pm25Data;
                        g_u8Pm25Sum += g_u16Pm25Data;
                        g_u8BufIndex = (g_u8BufIndex + 1) % PM25_BUF_SIZE;
                        g_u16DispPm25DataBak = g_u8Pm25Sum / PM25_BUF_SIZE;
                    }
                    else
                    {   
                        if(g_u8BufIndex < PM25_BUF_SIZE)
                        {
                            t_Pm25DataBuf[g_u8BufIndex] = g_u16Pm25Data;
                            g_u8Pm25Sum += g_u16Pm25Data;
                            g_u8BufIndex++;
                            g_u16DispPm25DataBak = g_u8Pm25Sum / g_u8BufIndex;
                            if(g_u8BufIndex >= PM25_BUF_SIZE)
                            {   
                                g_u8BufIndex = 0;
                                g_bGetSumFlg = 1;
                            }
                        }
                        else
                        {
                            g_u8BufIndex = 0;
                            g_bGetSumFlg = 1;
                        }
                    }
                }
                //其它显示
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
}