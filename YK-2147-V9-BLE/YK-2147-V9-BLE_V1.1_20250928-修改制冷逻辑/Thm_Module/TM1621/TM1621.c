/*********************************************************
文 件 名 : TM1621.C
文件说明 : TM1621液晶显示芯片驱动文件
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-6
版 本 号 : V1.0
==========================================================*/
//头文件
#include "ALLInclude.h"

//函数声明
static void F_Write_Head(u8 l_u8CmdType);
static void F_Write_Addr(u8 l_u8Addr);
static void F_Write_Data(u8 l_u8Data);

void F_TM1621_Init(void);
u8 F_TM1621_Read(u8 l_u8RamAddr);
void F_TM1621_Reflesh_RAM(u8 *l_pu8Table);
void F_TM1621_Write_DATA(u8 l_u8RamAddr, u8 l_u8RamData);
void F_TM1621_Write_CMD(u16 l_u16Cmd);

/*****************************************************
*函数名称: F_TM1621_Init(void)
*函数功能: TM1621初始化函数
*入口参数：void
*出口参数：void 
*****************************************************/
void F_TM1621_Init(void)
{	
	_IO_TM1621_CS_PP;
	_IO_TM1621_WR_PP;
	_IO_TM1621_RD_PP;
	_IO_TM1621_DATA_PP;
    F_TM1621_Write_CMD(C_TM1621_SYS_EN);
    F_TM1621_Write_CMD(C_TM1621_LCD_ON);
    F_TM1621_Write_CMD(C_TM1621_TIMER_DIS);
    F_TM1621_Write_CMD(C_TM1621_WDT_DIS);
    F_TM1621_Write_CMD(C_TM1621_BIAS_1_2_COM4);
    F_TM1621_Write_CMD(C_TM1621_IRQ_DIS);
//	F_TM1621_Write_CMD(C_TM1621_SYS_DIS); 
}

/*****************************************************
*函数名称：void F_Delay_Us(u8 u8InputDelayTime)
*函数功能：微秒级延时函数
*入口参数：u8InputDelayTime
*出口参数：void
*说明：仅做粗略延时用
*****************************************************/
//static void F_Delay_Us(u8 l_u8InputDelayTime)
//{
//    u8  l_u8UsCnt = 0;
//    u8  l_u8Ns125Cnt = 0;
//    
//    for (l_u8UsCnt = 0; l_u8UsCnt < l_u8InputDelayTime; l_u8UsCnt++)
//    {
//        for (l_u8Ns125Cnt = 0; l_u8Ns125Cnt < 8; l_u8Ns125Cnt++)
//        {}
//    }
//}

/*****************************************************
*函数名称：void F_Write_Head(u8 l_u8CmdType)
*函数功能：写命令头部的命令类型
*入口参数：l_u8CmdType（命令的类型）
*出口参数：void
*说明：
*****************************************************/
static void F_Write_Head(u8 l_u8CmdType)
{
    u8  l_u8HeadCnt = 0;
    
    //片选选中TM1621，配置各线初始状态
    _IO_TM1621_DATA_PP;
    _TM1621_DATA_SET;
    _TM1621_WR_SET; 
    _TM1621_RD_SET;
    _TM1621_CS_SET;
    F_Delay_Us(4);
    _TM1621_CS_CLR;
    F_Delay_Us(4);
    
    for (l_u8HeadCnt = 0; l_u8HeadCnt < 3; l_u8HeadCnt++)
    {

        //写命令头101
        if (C_CMD_WRITE == l_u8CmdType)             
        {
            if ((0 == l_u8HeadCnt) || (2 == l_u8HeadCnt))
            {
                _TM1621_DATA_SET;
            }
            else
            {
                _TM1621_DATA_CLR;
            }            
        }
        //读命令头110
        else
        {
            if (l_u8HeadCnt < 2)
            {
                _TM1621_DATA_SET;
            }
            else
            {
                _TM1621_DATA_CLR;
            }
        }        
        
        F_Delay_Us(2);
        _TM1621_WR_CLR;
        F_Delay_Us(2);
        _TM1621_WR_SET;
        F_Delay_Us(2);
    }
}
    
/*****************************************************
*函数名称：void F_Write_Addr(u8 l_u8Addr)
*函数功能：写入RAM地址，低6位有效
*入口参数：l_u8Addr（RAM地址）
*出口参数：void
*说明：
*****************************************************/
static void F_Write_Addr(u8 l_u8Addr)
{
    u8  l_u8WriteCnt = 0;
    
    //写RAM地址，先发高位再发低位
    l_u8Addr <<= 2;
    for (l_u8WriteCnt = 0; l_u8WriteCnt < 6; l_u8WriteCnt++)
    {
        if (l_u8Addr & 0x80)
        {
            _TM1621_DATA_SET;
        }
        else
        {
            _TM1621_DATA_CLR;
        }
        l_u8Addr <<= 1;
        
        F_Delay_Us(2);
        _TM1621_WR_CLR;
        F_Delay_Us(2);
        _TM1621_WR_SET;
        F_Delay_Us(2);        
    }
}

/*****************************************************
*函数名称：void F_Write_Data(u8 l_u8Data)
*函数功能：写入一个地址的数据
*入口参数：l_u8Data
*出口参数：void
*说明：
*****************************************************/
static void F_Write_Data(u8 l_u8Data)
{
    u8  l_u8SentCnt = 0;
    
    for (l_u8SentCnt = 0; l_u8SentCnt < 4; l_u8SentCnt++)
    {
        if (l_u8Data & 0x01)
        {
            _TM1621_DATA_SET;
        }
        else
        {
            _TM1621_DATA_CLR;
        }
        l_u8Data >>= 1;
        
        F_Delay_Us(1);
        _TM1621_WR_CLR;
        F_Delay_Us(1);
        _TM1621_WR_SET;
        F_Delay_Us(1);
    }
}   

/*****************************************************
*函数名称：u8 F_TM1621_Read(u8 l_u8RamAddr)
*函数功能：读RAM数据函数
*入口参数：l_u8RamAddr（RAM地址）
*出口参数：l_u8ReturnData
*说明：一次只能读一个RAM地址的数据
*****************************************************/
//u8 F_TM1621_Read(u8 l_u8RamAddr)
//{
//    u8  l_u8ReadCnt = 0;
//    u8  l_u8ReturnData = 0;
//    
//    //写命令头及RAM地址
//    F_Write_Head(C_CMD_READ);
//    F_Write_Addr(l_u8RamAddr);
//    
//    //接收数据
//    _IO_TM1621_DATA_PU;
//    F_Delay_Us(1);
//    for (l_u8ReadCnt = 0; l_u8ReadCnt < 4; l_u8ReadCnt++)
//    {
//        _TM1621_RD_CLR;
//        F_Delay_Us(1);
//        _TM1621_RD_SET;
//        
//        l_u8ReturnData >>= 1;
//        if (_TEST_TM1621_DATA)
//        {
//            l_u8ReturnData |= 0x80;
//        }
//        else
//        {
//        
//        }
//        
//        F_Delay_Us(1);
//    }
//	l_u8ReturnData >>= 4;
//
//    //释放TM1621
//    _TM1621_CS_CLR;
//    F_Delay_Us(2);  
//    
//    return l_u8ReturnData;
//}
//    

/*****************************************************
*函数名称：void F_TM1621_Write(u8 l_u8RamAddr, u8 l_u8RamData)
*函数功能：写RAM数据
*入口参数：l_u8RamAddr（RAM地址），l_u8RamData（显示的数据）
*出口参数：void
*说明：一次只能写一个地址的数据
*****************************************************/
//void F_TM1621_Write_DATA(u8 l_u8RamAddr, u8 l_u8RamData)
//{ 
//    //写命令头及RAM地址
//    F_Write_Head(C_CMD_WRITE);
//    F_Write_Addr(l_u8RamAddr);
//    
//    //写数据
//    F_Write_Data(l_u8RamData);
//    
//    //释放TM1621
//    _TM1621_CS_CLR;
//    F_Delay_Us(2);    
//}

/*****************************************************
*函数名称：void F_TM1621_Reflesh_RAM(u8 l_u8RamData)
*函数功能：刷新全部RAM区数据
*入口参数：*l_pu8Table（数据表格的起始地址）
*出口参数：void
*说明：
*****************************************************/
void F_TM1621_Reflesh_RAM(u8 *l_pu8Table)
{
    u8  l_u8RamAddrCnt = 0;
    
    //写命令头及起始地址0
    F_Write_Head(C_CMD_WRITE);
    F_Write_Addr(0); 

    //写相应的数据
    for (l_u8RamAddrCnt = 0; l_u8RamAddrCnt < 32; l_u8RamAddrCnt++)
    {              
        F_Write_Data(*l_pu8Table);
     
        if (l_u8RamAddrCnt < 31)
        {
            l_pu8Table++;
        }
        else
        {
        
        }
    }
    
    //释放TM1621
    _TM1621_CS_CLR;
    F_Delay_Us(2);
}

/*****************************************************
*函数名称：void F_TM1621_Write_CMD(u16 l_u16Cmd)
*函数功能：写单条命令
*入口参数：l_u16Cmd
*出口参数：void
*说明：
*****************************************************/
void F_TM1621_Write_CMD(u16 l_u16Cmd)
{
    u8  l_u8CmdCnt = 0;
    
    //片选选中TM1621，配置各线初始状态
    _IO_TM1621_DATA_PP;
    _TM1621_DATA_SET;
    _TM1621_WR_SET; 
    _TM1621_RD_SET;
    _TM1621_CS_SET;
    F_Delay_Us(4);
    _TM1621_CS_CLR;
    F_Delay_Us(4);

    //写命令
    l_u16Cmd <<= 4;
    for (l_u8CmdCnt = 0; l_u8CmdCnt < 13; l_u8CmdCnt++)
    {
        if (l_u16Cmd & 0x8000)
        {
            _TM1621_DATA_SET;
        }
        else
        {
            _TM1621_DATA_CLR;
        }
        l_u16Cmd <<= 1;
        
        F_Delay_Us(2);
        _TM1621_WR_CLR;
        F_Delay_Us(2);
        _TM1621_WR_SET;
        F_Delay_Us(2);        
    }
    
    //释放TM1621
    _TM1621_CS_CLR;
    F_Delay_Us(2);
}