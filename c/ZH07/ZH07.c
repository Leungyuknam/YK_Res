#include "h\Allinclude.h"

#define StiveSendBuf UartTXDBuf
#define StiveRecBuf UartRXDBuf

U8 xdata g_u8StiveLinkTime = 10;
U8 xdata g_u8StiveState = STIVE_STATE_OFF;//粉尘传感器状态

U8 xdata g_u8StiveSendState = 0;//粉尘传感器通信发送状态

U16 xdata g_u16Pm25Data = 0;//PM25数值 单位ug/m3
U16 xdata g_u16Pm10Data = 0;//PM10数值 单位ug/m3
U16 xdata g_u16Pm1Data = 0; //PM1.0数值 单位ug/m3 

StiveData_t xdata g_sPm25 = {0};
StiveData_t xdata g_sPm10 = {0};
StiveData_t xdata g_sPm1 = {0};

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
	if((StiveRecBuf[HEAD_FIRST] == 0x16)
	&& (StiveRecBuf[length-1] == StiveCheckSum(StiveRecBuf,length-1)))
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
/******************************************************************
 函数名称：					F_GetStiveData()
 功能描述：					粉尘数据获取
入口参数：                  l_sStiveData:显示数据 l_u8NowData:当前粉尘数据 l_RandomNum:随机数
 出口参数：                 void
*******************************************************************/
void F_GetStiveData(StiveData_t *l_sStiveData,U16 l_u16NowData,U8 l_RandomNum)
{
    U8 l_u8DelayTimeNum = 0;
    
    //实际显示数值赋值判断
    if(l_sStiveData->u16DispData >= l_sStiveData->u16DispDataBak)
    {
        if(l_sStiveData->u16DispData - l_sStiveData->u16DispDataBak >= 10)
        {   l_u8DelayTimeNum = 5;}
        else
        {   l_u8DelayTimeNum = 64;}
    }
    else 
    {
        if(l_sStiveData->u16DispDataBak - l_sStiveData->u16DispData >= 10)
        {   l_u8DelayTimeNum = 5;}
        else
        {   l_u8DelayTimeNum = 64;}
    }
    if(++l_sStiveData->u8DelayTime >= l_u8DelayTimeNum)
    {
        l_sStiveData->u8DelayTime = 0;
        l_sStiveData->u16DispData = l_sStiveData->u16DispDataBak;
    }
    //实际显示数值为0时,默认为5
    if(0 == l_sStiveData->u16DispData)
    {   l_sStiveData->u16DispData = 5;}
    //要求显示数值判断
    if(l_u16NowData < 5)//数值小于5,按随机数显示
    {   l_sStiveData->u16DispDataBak = l_RandomNum;}
    else //if(l_u16NowData < 20)//数值小于20,直接赋值
    {   l_sStiveData->u16DispDataBak = l_u16NowData;}
//    else
//    {//数值进行8次平均
//        if(l_sStiveData->bGetSumFlg)
//        {
//            l_sStiveData->u16DataSum -= l_sStiveData->u16DataBuf[l_sStiveData->u8Index];
//            l_sStiveData->u16DataBuf[l_sStiveData->u8Index] = l_u16NowData;
//            l_sStiveData->u16DataSum += l_u16NowData;
//            l_sStiveData->u8Index = (l_sStiveData->u8Index + 1) % c_STIVE_BUF_SIZE;
//            l_sStiveData->u16DispDataBak = l_sStiveData->u16DataSum / c_STIVE_BUF_SIZE;
//        }
//        else
//        {   
//            if(l_sStiveData->u8Index < c_STIVE_BUF_SIZE)
//            {
//                l_sStiveData->u16DataBuf[l_sStiveData->u8Index] = l_u16NowData;
//                l_sStiveData->u16DataSum += l_u16NowData;
//                l_sStiveData->u8Index++;
//                l_sStiveData->u16DispDataBak = l_sStiveData->u16DataSum / l_sStiveData->u8Index;
//                if(l_sStiveData->u8Index >= c_STIVE_BUF_SIZE)
//                {   
//                    l_sStiveData->u8Index = 0;
//                    l_sStiveData->bGetSumFlg = 1;
//                }
//            }
//            else
//            {
//                l_sStiveData->u8Index = 0;
//                l_sStiveData->bGetSumFlg = 1;
//            }
//        }
//    }
}

void F_StiveCommucate()
{
    static U8 s_u8RandomNumBak = 3;
    static U8 s_u8RandomDealy = 0;
    static U8 s_u8ErrNum = 8;
    static U8 s_u8DispErrDataType = 0;
    
    if(F_GetTimeTick(C_TIME_1s))
    {
        if(g_bSenseLoad)
        {
            if(g_u8StiveLinkTime)
            {   g_u8StiveLinkTime --;}
            else
            {   g_u8StiveState = STIVE_STATE_ERR;}
            
            if(STIVE_STATE_ON == g_u8StiveState)
            {   F_StiveTask(STIVE_CMD_READ);}
            else
            {   F_StiveTask(STIVE_CMD_ON);}
            
            //粉尘显示数据处理               
            if(STIVE_STATE_ERR == g_u8StiveState)
            {//通信故障时,数值按6~9随机数,间隔8s刷新
                if(++s_u8DispErrDataType >= 8)
                {
                    s_u8DispErrDataType = 0;
                    s_u8ErrNum = (g_u8RandomNum % 4) + 6;
                }
                g_sPm25.u16DispData = g_sPm25.u16DispDataBak = s_u8ErrNum;
                g_sPm10.u16DispData = g_sPm10.u16DispDataBak = s_u8ErrNum;
                g_sPm1.u16DispData = g_sPm1.u16DispDataBak = s_u8ErrNum;
            }
            else
            {
                //随机数5s获取
                if(++s_u8RandomDealy >= 5)
                {
                    s_u8RandomDealy = 0;
                    s_u8RandomNumBak = (g_u8RandomNum % 5) + 1;
                }
                
                F_GetStiveData(&g_sPm25,g_u16Pm25Data,s_u8RandomNumBak);
                F_GetStiveData(&g_sPm10,g_u16Pm10Data,s_u8RandomNumBak);
                F_GetStiveData(&g_sPm1,g_u16Pm1Data,s_u8RandomNumBak);
            }                
        }
    }
}