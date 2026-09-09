#include "h\Allinclude.h"

#define CH2OReadBuf UartRXDBuf

U8 xdata g_u8CH2OLinkTime = c_CH2O_LINK_ERR_TIME;//甲醛通讯时间

U16 xdata g_u16CH2OData = 0;//CH2O数值 0~500对应0~5ppm
S8 xdata g_s8TempData = 24;//温度 范围-20~50℃
U8 xdata g_u8HumiData = 60;//湿度 范围15~90%RH

//U8 code t_CH2OTaple[2] = {0x53,0x32};//测量CH2O值命令码
//U8 code t_TempHumiTaple[2] = {0x46,0xFD};//测量温湿度值命令码

unsigned char FucCheckSum(unsigned char *i,unsigned char ln)
{ 
    unsigned char j,tempq=0;
    i+=1;
    for(j=0;j<(ln-2);j++)
    {
        tempq+=*i;
        i++;
    }
    tempq=(~tempq)+1; return(tempq);
}

/******************************************************************
 函数名称：						F_CH2ORecProc()
 功能描述：						甲醛通讯接收
入口参数：                      length:接收长度
 出口参数：                     void
*******************************************************************/
void F_CH2ORecProc(U8 length)
{	
	if((CH2OReadBuf[0] == 0xFF) && (CH2OReadBuf[1] == 0x17) && (CH2OReadBuf[2] == 0x04)
	&&(CH2OReadBuf[length-1] == FucCheckSum(CH2OReadBuf,length)))
	{
		g_u8CH2OLinkTime = c_CH2O_LINK_ERR_TIME;
        g_u16CH2OData = (CH2OReadBuf[4] << 8) | CH2OReadBuf[5];
        g_u16CH2OData *= 1.25;//1ppb=1.25ug/m3
        if(g_u16CH2OData > c_CH2O_DATA_MAX)
        {   g_u16CH2OData = c_CH2O_DATA_MAX;}
	}
}

/**************************************************
*函数名称：void F_QueryVocData(void) 
*函数功能：查询CH2O传感器数据
*调用周期：1s
*入口参数：void
*出口参数：void  
**************************************************/
void F_QueryCH2OData()
{    
    static U8 temp = 3;
    static U8 s_u8DispDataType = 0;
    
    if(g_u8CH2OLinkTime)g_u8CH2OLinkTime--;

    //甲醛显示数据处理
    if(++s_u8DispDataType >= 5)
    {
        s_u8DispDataType = 0;
        temp = (g_u8DispCnt % 3) + 1;
    }
    
    if(g_u16CH2OData < 3)
    {   g_u16DispCH2OData = temp;}
    else
    {   g_u16DispCH2OData = g_u16CH2OData;}
}