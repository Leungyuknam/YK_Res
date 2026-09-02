#define	Communication_C	

#include "h\Allinclude.h"

U8 xdata g_u8RecOKTime = 0;
U8 xdata g_u8UartMissCnt = 0;
bit g_bUartErrFlg = 0;  //通讯错误标记

U8 xdata g_u8DispVersion = 0;//显示板版本

unsigned char CheckSum(unsigned char *ucPoint,unsigned char	 Count)
{
	unsigned char temp ;
	temp = Count + 0x5A;
	while(Count)
	{
		Count --;
		temp -= (*ucPoint);
		ucPoint ++;
	}
	return	temp;
}
/******************************************************************
 函数名称：						F_CopyNotBuf();
 功能描述：						获取取反后的BUF
 输入：						    u8Buf：存取BUF ，u8Length：实际数据长度
 其他说明：						无
*******************************************************************/
void F_CopyNotBuf(U8 *u8Buf,U8 u8Length)
{
    U8 i = 0;
        
    for(i = 0; i < u8Length;i++)
    {
        u8Buf[u8Length + i] = ~u8Buf[i];
    }
}
/******************************************************************
 函数名称：						F_CheckSumBuf();
 功能描述：						校验BUF是否相同
 输入：						    u8Buf：获取的BUF ，u8Length：实际数据长度
 其他说明：						无
*******************************************************************/
U8 F_CheckSumBuf(U8 *u8Buf,U8 u8Length)
{
    U8 i = 0;
    
    u8Length = u8Length / 2;
    for(i = 0; i < u8Length;i++)
    {
        u8Buf[u8Length + i] = ~u8Buf[u8Length + i];
        if(u8Buf[u8Length + i] != u8Buf[i])
        {
            return 0;
        }
    }
    return 1;
}
/******************************************************************
 函数名称：						MakeEnCode();
 功能描述：						发送的数据处理
 其他说明：						无
*******************************************************************/
U8 MakeEnCode()
{	
	/***实际数据***/
    F_CopyStr(&Uart1TXDBuf,&DispBuf,TOTAL_COM);
    Uart1TXDBuf[7] = g_u8DisBright;
    /***校验数据***/
    F_CopyNotBuf(Uart1TXDBuf,((TXD1_BUF_LENGTH - 1) / 2));     
	Uart1TXDBuf[TXD1_BUF_LENGTH-1] = CheckSum(Uart1TXDBuf,TXD1_BUF_LENGTH-1);
    
	return TXD1_BUF_LENGTH;
}

/******************************************************************
 函数名称：						F_DecCode();
 功能描述：						接收的数据处理
 其他说明：						无
*******************************************************************/
void DeCodeProc(U8 l_u8ByteNum)
{    
	if(l_u8ByteNum == RXD1_BUF_LENGTH)
	{	
        if(Uart1RXDBuf[RXD1_BUF_LENGTH-1] == CheckSum(Uart1RXDBuf,RXD1_BUF_LENGTH-1))	
        {
            if(F_CheckSumBuf(Uart1RXDBuf,RXD1_BUF_LENGTH-1))
            {
                g_u8DispVersion = Uart1RXDBuf[0] & 0x7f;
                if(Uart1RXDBuf[0] & 0x80)
                {   g_bDispUartErr = 1;}
                else
                {   g_bDispUartErr = 0;}
                
                g_u8KeyData = Uart1RXDBuf[1];
                
                g_u8RecOKTime = 0;
                return;
            }
        }
    }
    g_u8UartMissCnt++;
}

void F_CheckCommucate(void)
{
	if(F_GetTimeTick(C_TIME_1s))
	{
		if(g_u8RecOKTime != 0xff)g_u8RecOKTime ++;
		if(g_u8RecOKTime >= 120)
		{
            g_bUartErrFlg = 1;
		}
        else
        {   
            g_bUartErrFlg = 0;
        }
   	}
}
