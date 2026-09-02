#include "h\Allinclude.h"

#define SDASet_Input()		{P2CON &= ~BIT7;}
#define SDASet_Output()		{P2CON |= BIT7;}
#define SCLSet_Input()		{P1CON &= ~BIT1;}
#define SCLSet_Output()		{P1CON |= BIT1;}

#define SDASet_H()			{P_CH2O_SDA = 1;}
#define SDASet_L()			{P_CH2O_SDA = 0;}
#define	GetSDApin()			(P_CH2O_SDA)

#define SCLSet_H()			{P_CH2O_SCL = 1;}
#define SCLSet_L()			{P_CH2O_SCL = 0;}

#define	I2C_ACK_ERR			1
#define	I2C_ACK_OK			0

U8 xdata g_u8CH2OLinkTime = 5;      //甲醛通讯时间

U8 xdata g_u8CH2OReadType = 0;      //CH2O读取类型

U16 xdata g_u16CH2OData = 0;        //CH2O数值 0~500对应0~5ppm
U16 xdata g_u16DispCH2OData = 0;    //甲醛显示数值

S8 xdata g_s8TempData = 24;         //温度 范围-20~50℃
U8 xdata g_u8HumiData = 60;         //湿度 范围15~90%RH

U8 code t_CH2OTaple[2] = {0x53,0x32};       //测量CH2O值命令码
U8 code t_TempHumiTaple[2] = {0x46,0xFD};   //测量温湿度值命令码

U8 xdata CH2OReadBuf[c_CH2O_LENGTH_MAX] = {0};

/**************************************************
*函数名称：void  F_Delay(void)
*函数功能：延时(单位1us)
*入口参数：void
*出口参数：void
**************************************************/
static void F_Delay(U8 l_u8Cnt)
{
     do
	 {
	 	NOP();NOP();NOP();NOP();NOP();
	 	NOP();NOP();NOP();NOP();NOP();
	 	NOP();NOP();NOP();NOP();NOP();
	 	NOP();NOP();NOP();
	 }while(--l_u8Cnt);
     WDT_clear();
}
/**************************************************
*函数名称：void  F_StartBit(void)
*函数功能：IIC起始位
*入口参数：void
*出口参数：void
**************************************************/
static void F_StartBit()
{
	SCLSet_Output();
	SDASet_Output();
	SCLSet_H();
	SDASet_H();
	F_Delay(7);
	SDASet_L();
	F_Delay(7);
	SCLSet_L();
	F_Delay(7);
}
/**************************************************
*函数名称：void  F_StopBit(void)
*函数功能：IIC停止位
*入口参数：void
*出口参数：void
**************************************************/
static void F_StopBit()
{
	SDASet_L();
	F_Delay(7);
	SCLSet_H();
	F_Delay(7);
	SDASet_H();
	F_Delay(7);
	SCLSet_Input();
	SDASet_Input();
}
/**************************************************
*函数名称：void  F_mAck(void)
*函数功能：应答
*入口参数：void
*出口参数：void
**************************************************/
static void F_mAck()
{
	SDASet_L();
	F_Delay(7);
	SCLSet_H();
	F_Delay(7);
	SCLSet_L();
	F_Delay(7);
	SDASet_H();
	F_Delay(7);
}
/**************************************************
*函数名称：void F_mNack(void)
*函数功能：结束ACK
*入口参数：void
*出口参数：void
**************************************************/
static void F_mNack(void)
{
	SDASet_H();
	F_Delay(7);
	SCLSet_H();
	F_Delay(7);
	SCLSet_L();
	F_Delay(7);
	SDASet_L();
	F_Delay(7);
}
/**************************************************
*函数名称：void  F_CAck(void)
*函数功能：应答检测
*入口参数：void
*出口参数：void
**************************************************/
static void F_CAck()
{
	bit l_bAckFg = 0;
    U8 l_u8Cnt = 20;

	SDASet_H();
	SDASet_Input();
	F_Delay(7);
	SCLSet_H();
	F_Delay(7);
    while(l_u8Cnt)
    {
        l_u8Cnt--;
        if(!GetSDApin())
        {
            l_bAckFg = I2C_ACK_OK;
            break;
        }
        else if(!l_u8Cnt)
        {
            l_bAckFg = I2C_ACK_ERR;
            break;
        }
    }
    SCLSet_L();
	F_Delay(7);
	SDASet_Output();

    if(I2C_ACK_OK == l_bAckFg)
	{   g_u8CH2OLinkTime = c_CH2O_LINK_ERR_TIME;}
}
/**************************************************
*函数名称：void F_Write8Bit_Voc(void)
*函数功能：写一个字节
*入口参数：ch:写入数据
*出口参数：void
**************************************************/
static void F_Write8Bit(U8 ch)
{
	U8 uctemp;

	for (uctemp = 0; uctemp < 8; uctemp++ )
	{
		if (ch & 0x80)
		{
			SDASet_H();
		}
		else
		{
			SDASet_L();
		}
		ch = ch << 1;
        F_Delay(7);
		SCLSet_H();
        F_Delay(7);
		SCLSet_L();
        F_Delay(7);
	}
}
/**************************************************
*函数名称：void F_Read8Bit_Voc(void)
*函数功能：读入一个字节
*入口参数：*p存放该字节的指针
*出口参数：void
**************************************************/
static void F_Read8Bit(U8 *p)
{
	U8 uctemp;

	SDASet_Input();
	for (uctemp = 0; uctemp < 8; uctemp++)
	{
		*p = *p << 1;
		SCLSet_H();
        F_Delay(7);
		if (GetSDApin() == 1)
		{
			*p = *p | 0x01;
		}
		SCLSet_L();
        F_Delay(7);
	}
	SDASet_Output();
}
/**************************************************
*函数名称：void F_ReadEeprom(void)
*函数功能：连续读入多个字节到指定地址
*入口参数：*pData:数据,ucNum:长度
*出口参数：void
**************************************************/
static void F_ReadEeprom(U8 *pData,U8 ucNum)
{
	F_StartBit();
  	F_Write8Bit(0xB3);
	F_CAck();
	for ( ; ucNum != 1; ucNum--)
	{
		F_Read8Bit(pData);
		pData++;
		F_mAck();
	}
	F_Read8Bit(pData);
	F_mNack();
	F_StopBit();
}
/**************************************************
*函数名称：void WriteEeprom(void)
*函数功能：连续写入多个字节到指定地址
*入口参数：*pData:数据,ucNum:长度
*出口参数：void
**************************************************/
static void F_WriteEeprom(U8 *pData,U8 ucNum)
{
	F_StartBit();
  	F_Write8Bit(0xB2);
	F_CAck();
	for ( ; ucNum != 0; ucNum--)
	{
		F_Write8Bit(*pData);
		F_CAck();
		pData++;
	}
	F_StopBit();
}
/**************************************************
*函数名称：void Calc_CRC8(void)
*函数功能：校验和计算
*入口参数：*message:数据,Num:长度
*出口参数：void
**************************************************/
static U8 Calc_CRC8(U8 *message, U8 Num)
{
    U8 i;
    U8 byte;
    U8 crc = 0xFF;

    for(byte = 0; byte < Num; byte++)
    {
        crc ^= (message[ byte ]);
        for(i = 8; i > 0; --i)
        {
            if(crc & 0x80)
            {   crc = ( crc << 1 ) ^ 0x31;}
            else
            {   crc = ( crc << 1 );}
        }
    }
    return crc;
}
/**************************************************
*函数名称：void F_QueryCH2OData(void)
*函数功能：查询CH2O传感器数据 + 显示缓变 + 强降算法
*调用周期：1s
*入口参数：void
*出口参数：void
**************************************************/
void F_QueryCH2OData()
{
    U16 l_u16Temp = 0;
    U16 l_u16Diff = 0;
    static U8  s_u8RandomNumBak = 3;
    static U8  s_u8RandomDealy  = 0;
    static U8  s_u8CH2ODataState = 0;   //CH2O_DATA_STATE_NORMAL
    static U16 s_u16CH2ODataTime = 0;   //计时:常规阶段=触发计时;强降阶段=步进计时
    static U16 s_u16CH2ODataBase = 0;   //强降到底(0.040)后真实数据最低值(新污染判定基准)
    static U8  s_u8Delay    = 0;        //显示缓变刷新计数
    static U8  s_u8DelaySet = 0;        //显示缓变刷新周期

    if(g_u8CH2OLinkTime)g_u8CH2OLinkTime--;

    if(c_CH2O_READ_INIT == g_u8CH2OReadType)
    {
        F_WriteEeprom(&t_TempHumiTaple,2);
        g_u8CH2OReadType = c_CH2O_READ_TEMP_HUMI;
    }
    else if(c_CH2O_READ_TEMP_HUMI == g_u8CH2OReadType)
    {
        F_SetStr(&CH2OReadBuf,0,6);
        F_ReadEeprom(&CH2OReadBuf,6);
        F_WriteEeprom(&t_CH2OTaple,2);
        if(CH2OReadBuf[2] == Calc_CRC8(&CH2OReadBuf,2))
        {
            l_u16Temp = ((CH2OReadBuf[0] << 8) | CH2OReadBuf[1]);
            if(l_u16Temp < 32767)
            {   g_s8TempData = l_u16Temp / 100;}
            else
            {   g_s8TempData = (l_u16Temp - 65536) / 100;}
            if(g_s8TempData > c_TEMP_MAX)
            {   g_s8TempData = c_TEMP_MAX;}
            else if(g_s8TempData < c_TEMP_MIN)
            {   g_s8TempData = c_TEMP_MIN;}
        }
        if(CH2OReadBuf[5] == Calc_CRC8(&CH2OReadBuf[3],2))
        {
            l_u16Temp = ((CH2OReadBuf[3] << 8) | CH2OReadBuf[4]);
            g_u8HumiData = l_u16Temp / 100;
            if(g_u8HumiData > c_HUMI_MAX)
            {   g_u8HumiData = c_HUMI_MAX;}
            else if(g_u8HumiData < c_HUMI_MIN)
            {   g_u8HumiData = c_HUMI_MIN;}
        }
        g_u8CH2OReadType = c_CH2O_READ_CH2O_DATA;
    }
    else
    {
        F_SetStr(&CH2OReadBuf,0,6);
        F_ReadEeprom(&CH2OReadBuf,3);
        F_WriteEeprom(&t_TempHumiTaple,2);
        if(CH2OReadBuf[2] == Calc_CRC8(&CH2OReadBuf,2))
        {
            g_u16CH2OData = (CH2OReadBuf[0] << 8) | CH2OReadBuf[1];
            g_u16CH2OData *= 1.25;//1ppb=1.25ug/m3
            if(g_u16CH2OData > c_CH2O_DATA_MAX)
            {   g_u16CH2OData = c_CH2O_DATA_MAX;}
        }
        g_u8CH2OReadType = c_CH2O_READ_TEMP_HUMI;
    }

    //甲醛显示数据处理
    if(++s_u8RandomDealy >= 5)
    {//随机数间隔5s获取
        s_u8RandomDealy = 0;
        s_u8RandomNumBak = (g_u8RandomNum % 3) + 1;
    }

    if(g_u16CH2OData < 3)
    {//采集数据小于3,按1~3随机数显示
        g_u16DispCH2OData = s_u8RandomNumBak;
        s_u16CH2ODataTime = 0;
        s_u8Delay = 0;
        s_u8CH2ODataState = CH2O_DATA_STATE_NORMAL;
    }
    else if(!g_bSenseLoad)
    {//关传感器,低功耗阶段,直接将采集的数据作为显示的数据
        g_u16DispCH2OData = g_u16CH2OData;
        s_u16CH2ODataTime = 0;
        s_u8Delay = 0;
        s_u8CH2ODataState = CH2O_DATA_STATE_NORMAL;
    }
    else if(CH2O_DATA_STATE_NORMAL == s_u8CH2ODataState)
    {//常规阶段:显示值缓变跟随采集值,并检测>0.080持续30min触发强降
        //----显示缓变:差值大时快刷新(5s),差值小时慢刷新(64s)----
        if(g_u16DispCH2OData >= g_u16CH2OData)
        {   l_u16Diff = g_u16DispCH2OData - g_u16CH2OData;}
        else
        {   l_u16Diff = g_u16CH2OData - g_u16DispCH2OData;}

        s_u8DelaySet = (l_u16Diff >= CH2O_FAST_THR) ? DELAY_FAST_SEC : DELAY_SLOW_SEC;
        if(++s_u8Delay >= s_u8DelaySet)
        {
            s_u8Delay = 0;
            g_u16DispCH2OData = g_u16CH2OData;
        }

        //----甲醛>0.080(80ug/m3)持续30min,进入强降----
        if(g_u16CH2OData > c_CH2O_DECLINE_TRIG)
        {
            if(++s_u16CH2ODataTime >= (30 * c_Time_1min))
            {
                s_u16CH2ODataTime = 0;
                s_u8CH2ODataState = CH2O_DATA_STATE_DECLINE;
            }
        }
        else
        {   s_u16CH2ODataTime = 0;}
    }
    else
    {//强降阶段:显示值每1min强制降0.002到0.040;只有降到0.040后才检测新污染上升
        if(g_u16DispCH2OData > c_CH2O_DECLINE_FLOOR)
        {//下降途中:只管强制下降,不响应真实值变化
            if(++s_u16CH2ODataTime >= c_Time_1min)
            {
                s_u16CH2ODataTime = 0;
                if(g_u16DispCH2OData >= (c_CH2O_DECLINE_FLOOR + c_CH2O_DECLINE_STEP))
                {   g_u16DispCH2OData -= c_CH2O_DECLINE_STEP;}
                else
                {   g_u16DispCH2OData = c_CH2O_DECLINE_FLOOR;}

                if(g_u16DispCH2OData <= c_CH2O_DECLINE_FLOOR)
                {//刚降到0.040,以当前真实值初始化新污染判定基准
                    g_u16DispCH2OData = c_CH2O_DECLINE_FLOOR;
                    s_u16CH2ODataBase = g_u16CH2OData;
                }
            }
        }
        else
        {//已降到0.040:维持强降,仅此阶段检测新污染上升
            s_u16CH2ODataTime = 0;
            //跟踪真实值最低点作为基准(空气净化后基准随之下降)
            if(g_u16CH2OData < s_u16CH2ODataBase)
            {   s_u16CH2ODataBase = g_u16CH2OData;}
            //新污染上升:真实值相对最低值回升≥阈值,退出强降,交由缓变跟随上升
            if(g_u16CH2OData >= (s_u16CH2ODataBase + c_CH2O_RISE_THR))
            {
                s_u8CH2ODataState = CH2O_DATA_STATE_NORMAL;
                s_u8Delay = 0;
            }
        }
    }
}
