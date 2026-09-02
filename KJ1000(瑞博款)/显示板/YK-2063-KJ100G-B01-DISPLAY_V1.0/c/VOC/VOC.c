#include "h\Allinclude.h"

#define SDASet_Input()		{P5CON &= ~BIT0;}
#define SDASet_Output()		{P5CON |= BIT0;}
#define SCLSet_Input()		{P5CON &= ~BIT1;}
#define SCLSet_Output()		{P5CON |= BIT1;}

#define SDASet_H()			{P_VOC_SDA = 1;}
#define SDASet_L()			{P_VOC_SDA = 0;}
#define	GetSDApin()			(P_VOC_SDA)

#define SCLSet_H()			{P_VOC_SCL = 1;}
#define SCLSet_L()			{P_VOC_SCL = 0;}

#define	I2C_ACK_ERR			1
#define	I2C_ACK_OK			0

/* ------------ 新增：本函数内部使用的轻量状态 ------------ */
#define TH_BUF_SIZE        3      /* 温湿均值窗口 */
#define VOC_BUF_SIZE       5      /* VOC均值窗口 */
#define TEMP_FAST_THR      1      /* 温度快速跟随阈值(℃) */
#define HUMI_FAST_THR      3      /* 湿度快速跟随阈值(%RH) */
#define VOC_FAST_THR       10     /* VOC快速跟随阈值(同你PM25逻辑) */
#define DELAY_FAST_SEC     5			/* 快速变化延时值 */
#define DELAY_SLOW_SEC     64			/* 微小变化延时值 */

U8 xdata g_u8VocLinkTime = c_VOC_LINK_ERR_TIME;//VOC通讯时间

U8 xdata g_u8VocReadType = 0;//VOC读取类型

U16 xdata g_u16VocData = 0;//VOC数值 0~500对应0~5ppm
S8 xdata g_s8TempData = 24;//温度 范围-40~85℃
U8 xdata g_u8HumiData = 60;//湿度 范围0~100%RH

S8  xdata g_s8DispTempData = 24;   /* 对外显示/上报温度 */
U8  xdata g_u8DispHumiData = 60;   /* 对外显示/上报湿度 */
U16 xdata g_u16DispVocData = 0;    /* 对外显示/上报VOC */

U8 code t_VocTaple[2] = {0x26,0x0F};//测量VOC值命令码
U8 code t_TempHumiTaple[2] = {0x46,0xFD};//测量温湿度值命令码

U8 xdata VocReadBuf[c_VOC_LENGTH_MAX] = {0};

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
    
//    if(I2C_ACK_OK == l_bAckFg)
//	{   g_u8VocLinkTime = c_VOC_LINK_ERR_TIME;}
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
		if(GetSDApin() == 1)
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
///**************************************************
//*函数名称：void F_QueryVocData(void) 
//*函数功能：查询VOC传感器数据
//*调用周期：1s
//*入口参数：void
//*出口参数：void  
//**************************************************/
//void F_QueryVocData()
//{    
//    U16 l_u16Temp = 0;
//    
//    if(g_u8VocLinkTime)g_u8VocLinkTime--;
//    
////    if(c_VOC_READ_INIT == g_u8VocReadType)
////    {
////        F_WriteEeprom(&t_VocTaple,2);
////        g_u8VocReadType = c_VOC_READ_VOC_DATA;
////    }
////    else
////    {
////        F_SetStr(&VocReadBuf,0,3);
////        F_ReadEeprom(&VocReadBuf,3);
////        F_WriteEeprom(&t_VocTaple,2);
////        if(VocReadBuf[2] == Calc_CRC8(&VocReadBuf,2))
////        {
////            g_u16VocData = (VocReadBuf[0] << 8) | VocReadBuf[1];
////            if(g_u16VocData > c_VOC_DATA_MAX)
////            {   g_u16VocData = c_VOC_DATA_MAX;}
////        }
////        g_u8VocReadType = c_VOC_READ_VOC_DATA;
////    }
//    if(c_VOC_READ_INIT == g_u8VocReadType)
//    {
//        F_WriteEeprom(&t_TempHumiTaple,2);
//        g_u8VocReadType = c_VOC_READ_TEMP_HUMI;
//    }
//    else if(c_VOC_READ_TEMP_HUMI == g_u8VocReadType)
//    {
//        F_SetStr(&VocReadBuf,0,6);
//        F_ReadEeprom(&VocReadBuf,6);
//        F_WriteEeprom(&t_VocTaple,2);
//        if(VocReadBuf[2] == Calc_CRC8(&VocReadBuf,2))
//        {
//            l_u16Temp = ((VocReadBuf[0] << 8) | VocReadBuf[1]);
//            if(l_u16Temp < 32767)
//            {   g_s8TempData = l_u16Temp / 100;}
//            else
//            {   g_s8TempData = (l_u16Temp - 65536) / 100;}
//            if(g_s8TempData > c_TEMP_MAX)
//            {   g_s8TempData = c_TEMP_MAX;}
//            else if(g_s8TempData < c_TEMP_MIN)
//            {   g_s8TempData = c_TEMP_MIN;}
//        }
//        if(VocReadBuf[5] == Calc_CRC8(&VocReadBuf[3],2))
//        {
//            l_u16Temp = ((VocReadBuf[3] << 8) | VocReadBuf[4]);
//            g_u8HumiData = l_u16Temp / 100;
//            if(g_u8HumiData > c_HUMI_MAX)
//            {   g_u8HumiData = c_HUMI_MAX;}
//            else if(g_u8HumiData < c_HUMI_MIN)
//            {   g_u8HumiData = c_HUMI_MIN;}
//        }
//        g_u8VocReadType = c_VOC_READ_VOC_DATA;
//    }
//    else
//    {
//        F_SetStr(&VocReadBuf,0,6);
//        F_ReadEeprom(&VocReadBuf,3);
//        F_WriteEeprom(&t_TempHumiTaple,2);
//        if(VocReadBuf[2] == Calc_CRC8(&VocReadBuf,2))
//        {
//            g_u8VocLinkTime = c_VOC_LINK_ERR_TIME;
//            g_u16VocData = (VocReadBuf[0] << 8) | VocReadBuf[1];
//            if(g_u16VocData > c_VOC_DATA_MAX)
//            {   g_u16VocData = c_VOC_DATA_MAX;}
//        }
//        g_u8VocReadType = c_VOC_READ_TEMP_HUMI;
//    }
//}



///**************************************************
//*函数名称：void F_QueryVocData(void) 
//*函数功能：查询VOC传感器数据 + 温/湿延时显示&缓冲处理
//*调用周期：1s
//*入口参数：void
//*出口参数：void  
//**************************************************/
//void F_QueryVocData()
//{    
//    U16 l_u16Temp = 0;

//    /* 温度缓冲/目标/延时 */
//    static S8   s_Tbuf[TH_BUF_SIZE] = {0};
//    static U32  s_Tsum = 0;
//    static U8   s_Tidx = 0;
//    static bit  s_Tready = 0;
//    static S8   s_Tbak  = 24;
//    static U8   s_Tdly  = 0;

//    /* 湿度缓冲/目标/延时 */
//    static U8   s_Hbuf[TH_BUF_SIZE] = {0};
//    static U32  s_Hsum = 0;
//    static U8   s_Hidx = 0;
//    static bit  s_Hready = 0;
//    static U8   s_Hbak  = 60;
//    static U8   s_Hdly  = 0;

//    /* VOC缓冲/目标/延时 */
//    static U16  s_Vbuf[VOC_BUF_SIZE] = {0};
//    static U32  s_Vsum = 0;
//    static U8   s_Vidx = 0;
//    static bit  s_Vready = 0;
//    static U16  s_Vbak  = 0;
//    static U8   s_Vdly  = 0;

//    /* ------------ 原有链路计时（保留） ------------ */
//    if (g_u8VocLinkTime) g_u8VocLinkTime--;

//    /* ------------ 原有：按状态机访问EEPROM（保留你的顺序和校验） ------------ */
//    // if(c_VOC_READ_INIT == g_u8VocReadType) {...} 的旧分支保留在注释中

//    if (c_VOC_READ_INIT == g_u8VocReadType)
//    {
//        F_WriteEeprom(&t_TempHumiTaple, 2);
//        g_u8VocReadType = c_VOC_READ_TEMP_HUMI;
//    }
//    else if (c_VOC_READ_TEMP_HUMI == g_u8VocReadType)
//    {
//        F_SetStr(&VocReadBuf, 0, 6);
//        F_ReadEeprom(&VocReadBuf, 6);
//        F_WriteEeprom(&t_VocTaple, 2);

//        if (VocReadBuf[2] == Calc_CRC8(&VocReadBuf, 2))
//        {
//            l_u16Temp = ((VocReadBuf[0] << 8) | VocReadBuf[1]);
//            if (l_u16Temp < 32767) { g_s8TempData = l_u16Temp / 100; }
//            else                    { g_s8TempData = (l_u16Temp - 65536) / 100; }

//            if (g_s8TempData > c_TEMP_MAX)      g_s8TempData = c_TEMP_MAX;
//            else if (g_s8TempData < c_TEMP_MIN) g_s8TempData = c_TEMP_MIN;
//        }
//        if (VocReadBuf[5] == Calc_CRC8(&VocReadBuf[3], 2))
//        {
//            l_u16Temp   = ((VocReadBuf[3] << 8) | VocReadBuf[4]);
//            g_u8HumiData = l_u16Temp / 100;
//            if (g_u8HumiData > c_HUMI_MAX)      g_u8HumiData = c_HUMI_MAX;
//            else if (g_u8HumiData < c_HUMI_MIN) g_u8HumiData = c_HUMI_MIN;
//        }
//        g_u8VocReadType = c_VOC_READ_VOC_DATA;
//    }
//    else
//    {
//        F_SetStr(&VocReadBuf, 0, 6);
//        F_ReadEeprom(&VocReadBuf, 3);
//        F_WriteEeprom(&t_TempHumiTaple, 2);
//        if (VocReadBuf[2] == Calc_CRC8(&VocReadBuf, 2))
//        {
//            g_u8VocLinkTime = c_VOC_LINK_ERR_TIME;            /* 链路OK续命 */
//            g_u16VocData    = (VocReadBuf[0] << 8) | VocReadBuf[1];
//            if (g_u16VocData > c_VOC_DATA_MAX) g_u16VocData = c_VOC_DATA_MAX;
//        }
//        g_u8VocReadType = c_VOC_READ_TEMP_HUMI;
//    }

//    /* ================= 显示/上报处理（两档延时 + 均值） ================= */

//    /* A) 通讯错误：温度=24、湿度=60 立即生效；VOC维持上一次显示值 */
//    if (0 == g_u8VocLinkTime)
//    {
//        g_s8DispTempData = 24;
//        g_u8DispHumiData = 60;

//        s_Tbak = 24; s_Hbak = 60;
//        s_Tdly = 0;  s_Hdly = 0;
//        /* VOC 不更新 s_Vbak 与显示延时，保持 g_u16DispVocData 当前值 */
//        return;
//    }

//    /* B) 正常：更新 温度/湿度/VOC 的“目标显示值Bak”（滑动均值） */
//    /* 温度均值 */
//    {
//        S8 t = g_s8TempData;
//        if (s_Tready) {
//            s_Tsum -= (U8)s_Tbuf[s_Tidx];
//            s_Tbuf[s_Tidx] = (U8)t;
//            s_Tsum += (U8)t;
//            s_Tidx = (s_Tidx + 1) % TH_BUF_SIZE;
//            s_Tbak = (S8)(s_Tsum / TH_BUF_SIZE);
//        } else {
//            if (s_Tidx < TH_BUF_SIZE) {
//                s_Tbuf[s_Tidx] = (U8)t;
//                s_Tsum += (U8)t;
//                s_Tidx++;
//                s_Tbak = (S8)(s_Tsum / s_Tidx);
//                if (s_Tidx >= TH_BUF_SIZE) { s_Tidx = 0; s_Tready = 1; }
//            } else {
//                s_Tidx = 0; s_Tready = 1;
//            }
//        }
//    }
//    /* 湿度均值 */
//    {
//        U8 h = g_u8HumiData;
//        if (s_Hready) {
//            s_Hsum -= s_Hbuf[s_Hidx];
//            s_Hbuf[s_Hidx] = h;
//            s_Hsum += h;
//            s_Hidx = (s_Hidx + 1) % TH_BUF_SIZE;
//            s_Hbak = (U8)(s_Hsum / TH_BUF_SIZE);
//        } else {
//            if (s_Hidx < TH_BUF_SIZE) {
//                s_Hbuf[s_Hidx] = h;
//                s_Hsum += h;
//                s_Hidx++;
//                s_Hbak = (U8)(s_Hsum / s_Hidx);
//                if (s_Hidx >= TH_BUF_SIZE) { s_Hidx = 0; s_Hready = 1; }
//            } else {
//                s_Hidx = 0; s_Hready = 1;
//            }
//        }
//    }
//    /* VOC均值 */
//    {
//        U16 v = g_u16VocData;
//        if (s_Vready) {
//            s_Vsum -= s_Vbuf[s_Vidx];
//            s_Vbuf[s_Vidx] = v;
//            s_Vsum += v;
//            s_Vidx = (s_Vidx + 1) % VOC_BUF_SIZE;
//            s_Vbak = (U16)(s_Vsum / VOC_BUF_SIZE);
//        } else {
//            if (s_Vidx < VOC_BUF_SIZE) {
//                s_Vbuf[s_Vidx] = v;
//                s_Vsum += v;
//                s_Vidx++;
//                s_Vbak = (U16)(s_Vsum / s_Vidx);
//                if (s_Vidx >= VOC_BUF_SIZE) { s_Vidx = 0; s_Vready = 1; }
//            } else {
//                s_Vidx = 0; s_Vready = 1;
//            }
//        }
//    }

//    /* C) 两档延时跟随：到点才把“显示值=Bak”，否则保持旧值 */
//    /* 温度 */
//    {
//        S8 a = g_s8DispTempData, b = s_Tbak;
//        U8 diff = (a >= b) ? (a - b) : (b - a);
//        U8 dly  = (diff >= TEMP_FAST_THR) ? DELAY_FAST_SEC : DELAY_SLOW_SEC;
//        if (++s_Tdly >= dly) { s_Tdly = 0; g_s8DispTempData = s_Tbak; }
//    }
//    /* 湿度 */
//    {
//        U8 a = g_u8DispHumiData, b = s_Hbak;
//        U8 diff = (a >= b) ? (a - b) : (b - a);
//        U8 dly  = (diff >= HUMI_FAST_THR) ? DELAY_FAST_SEC : DELAY_SLOW_SEC;
//        if (++s_Hdly >= dly) { s_Hdly = 0; g_u8DispHumiData = s_Hbak; }
//    }
//    /* VOC */
//    {
//        U16 a = g_u16DispVocData, b = s_Vbak;
//        U16 diff = (a >= b) ? (a - b) : (b - a);
//        U8  dly  = (diff >= VOC_FAST_THR) ? DELAY_FAST_SEC : DELAY_SLOW_SEC;
//        if (++s_Vdly >= dly) { s_Vdly = 0; g_u16DispVocData = s_Vbak; }
//    }
//}



/**************************************************
* 函数名称：void F_QueryVocData(void) 
* 功能描述：查询VOC传感器数据 + 温/湿延时显示&缓冲处理
* 调用周期：1s
* 入口参数：void
* 出口参数：void  
**************************************************/
void F_QueryVocData(void)
{
    U16 l_u16Temp = 0;

    /*------------------- 静态变量定义 -------------------*/
    /* 温度 */
//    static S8  s_s8Tbuf[TH_BUF_SIZE] = {0};
    static U8  s_u8Tbuf[TH_BUF_SIZE] = {0};
    static U32 s_u32Tsum = 0;
    static U8  s_u8Tidx = 0;
    static bit s_bTready = 0;
    static S8  s_s8Tbak  = 24;
    static U8  s_u8Tdly  = 0;

    /* 湿度 */
    static U8  s_u8Hbuf[TH_BUF_SIZE] = {0};
    static U32 s_u32Hsum = 0;
    static U8  s_u8Hidx = 0;
    static bit s_bHready = 0;
    static U8  s_u8Hbak  = 60;
    static U8  s_u8Hdly  = 0;

    /* VOC */
    static U16 s_u16Vbuf[VOC_BUF_SIZE] = {0};
    static U32 s_u32Vsum = 0;
    static U8  s_u8Vidx = 0;
    static bit s_bVready = 0;
    static U16 s_u16Vbak = 0;
    static U8  s_u8Vdly  = 0;
		/* VOC 错误显示（参考粉尘：6~9循环） */
    static U8  s_u8VocErrNum  = 6;
    static U8  s_u8VocErrTick = 0;

    /* 临时变量 */
//    S8  l_s8T = 0;
	U8  l_u8T = 0;
    U8  l_u8H = 0;
    U16 l_u16V = 0;
    U8  l_u8Diff = 0;
    U8  l_u8Delay = 0;
    U16 l_u16Diff = 0;

    /*--------------------------------------------------------------
      原有链路计时
    --------------------------------------------------------------*/
    if (g_u8VocLinkTime)
        g_u8VocLinkTime--;

    /*--------------------------------------------------------------
      原有状态机读取逻辑（不改动结构）
    --------------------------------------------------------------*/
    if (c_VOC_READ_INIT == g_u8VocReadType)
    {
        F_WriteEeprom(&t_TempHumiTaple, 2);
        g_u8VocReadType = c_VOC_READ_TEMP_HUMI;
    }
    else if (c_VOC_READ_TEMP_HUMI == g_u8VocReadType)
    {
        F_SetStr(&VocReadBuf, 0, 6);
        F_ReadEeprom(&VocReadBuf, 6);
        F_WriteEeprom(&t_VocTaple, 2);

        if (VocReadBuf[2] == Calc_CRC8(&VocReadBuf, 2))
        {
            l_u16Temp = ((VocReadBuf[0] << 8) | VocReadBuf[1]);
            if (l_u16Temp < 32767)
                g_s8TempData = (S8)(l_u16Temp / 100);
            else
                g_s8TempData = (S8)((l_u16Temp - 65536) / 100);

            if (g_s8TempData > c_TEMP_MAX)
                g_s8TempData = c_TEMP_MAX;
            else if (g_s8TempData < c_TEMP_MIN)
                g_s8TempData = c_TEMP_MIN;
        }
        if (VocReadBuf[5] == Calc_CRC8(&VocReadBuf[3], 2))
        {
            l_u16Temp = ((VocReadBuf[3] << 8) | VocReadBuf[4]);
            g_u8HumiData = (U8)(l_u16Temp / 100);
            if (g_u8HumiData > c_HUMI_MAX)
                g_u8HumiData = c_HUMI_MAX;
            else if (g_u8HumiData < c_HUMI_MIN)
                g_u8HumiData = c_HUMI_MIN;
        }
        g_u8VocReadType = c_VOC_READ_VOC_DATA;
    }
    else
    {
        F_SetStr(&VocReadBuf, 0, 6);
        F_ReadEeprom(&VocReadBuf, 3);
        F_WriteEeprom(&t_TempHumiTaple, 2);
        if (VocReadBuf[2] == Calc_CRC8(&VocReadBuf, 2))
        {
            g_u8VocLinkTime = c_VOC_LINK_ERR_TIME;
            g_u16VocData = (U16)((VocReadBuf[0] << 8) | VocReadBuf[1]);
            if (g_u16VocData > c_VOC_DATA_MAX)
                g_u16VocData = c_VOC_DATA_MAX;
        }
        g_u8VocReadType = c_VOC_READ_TEMP_HUMI;
    }

    /* 通讯错误处理：
       温度=24、湿度=60 立即生效；
       VOC 参考粉尘：显示个位随机值（6~9循环），立即生效 */
    if (0 == g_u8VocLinkTime)
    {
        g_s8DispTempData = 24;
        g_u8DispHumiData = 60;

        s_s8Tbak = 24; s_u8Tdly = 0;
        s_u8Hbak = 60; s_u8Hdly = 0;

        /* 每 8 秒更新一次错误占位数：6~9循环（与粉尘一致） */
        s_u8VocErrTick++;
        if (s_u8VocErrTick >= 8)
        {
            s_u8VocErrTick = 0;
            s_u8VocErrNum  = (U8)((g_u8DispCnt % 5) + 5);
        }

        g_u16DispVocData = s_u8VocErrNum;  /* 立即显示 */
        s_u16Vbak        = s_u8VocErrNum;  /* 目标也置为同值，避免恢复时跳变 */
        s_u8Vdly         = 0;

        return;
    }

    /*--------------------------------------------------------------
      正常情况：温湿度/VOC均值计算
    --------------------------------------------------------------*/
//    /* 温度均值 */
//    l_s8T = g_s8TempData;
//    if (s_bTready)
//    {
//        s_u32Tsum -= (U8)s_s8Tbuf[s_u8Tidx];
//        s_s8Tbuf[s_u8Tidx] = (U8)l_s8T;
//        s_u32Tsum += (U8)l_s8T;
//        s_u8Tidx = (U8)((s_u8Tidx + 1U) % TH_BUF_SIZE);
//        s_s8Tbak = (S8)(s_u32Tsum / TH_BUF_SIZE);
//    }
//    else
//    {
//        if (s_u8Tidx < TH_BUF_SIZE)
//        {
//            s_s8Tbuf[s_u8Tidx] = (U8)l_s8T;
//            s_u32Tsum += (U8)l_s8T;
//            s_u8Tidx++;
//            s_s8Tbak = (S8)(s_u32Tsum / s_u8Tidx);
//            if (s_u8Tidx >= TH_BUF_SIZE)
//            {
//                s_u8Tidx = 0;
//                s_bTready = 1;
//            }
//        }
//        else
//        {
//            s_u8Tidx = 0;
//            s_bTready = 1;
//        }
//    }
    /* 温度均值（g_s8TempData: 0~85） */
	l_u8T = (U8)g_s8TempData;    // 入口统一转一次，后面全按 U8 处理

	if (s_bTready)
	{
	    /* 环形缓冲区：减去旧值，加上新值 */
	    s_u32Tsum -= s_u8Tbuf[s_u8Tidx];
	    s_u8Tbuf[s_u8Tidx] = l_u8T;
	    s_u32Tsum += l_u8T;

	    /* 索引递增并回绕 */
	    if (++s_u8Tidx >= TH_BUF_SIZE)
	    {
	        s_u8Tidx = 0;
	    }

	    /* 已经填满，用固定长度求平均 */
	    s_s8Tbak = (S8)(s_u32Tsum / TH_BUF_SIZE);
	}
	else
	{
	    /* 初始化阶段：未填满时，长度用当前 idx */
	    s_u8Tbuf[s_u8Tidx] = l_u8T;
	    s_u32Tsum += l_u8T;
	    s_u8Tidx++;

	    /* 用当前样本个数求平均 */
	    s_s8Tbak = (S8)(s_u32Tsum / s_u8Tidx);

	    /* 填满一次之后进入稳定状态 */
	    if (s_u8Tidx >= TH_BUF_SIZE)
	    {
	        s_u8Tidx = 0;
	        s_bTready = 1;
	    }
	}

    /* 湿度均值 */
    l_u8H = g_u8HumiData;
    if (s_bHready)
    {
        s_u32Hsum -= s_u8Hbuf[s_u8Hidx];
        s_u8Hbuf[s_u8Hidx] = l_u8H;
        s_u32Hsum += l_u8H;
        s_u8Hidx = (U8)((s_u8Hidx + 1U) % TH_BUF_SIZE);
        s_u8Hbak = (U8)(s_u32Hsum / TH_BUF_SIZE);
    }
    else
    {
        if (s_u8Hidx < TH_BUF_SIZE)
        {
            s_u8Hbuf[s_u8Hidx] = l_u8H;
            s_u32Hsum += l_u8H;
            s_u8Hidx++;
            s_u8Hbak = (U8)(s_u32Hsum / s_u8Hidx);
            if (s_u8Hidx >= TH_BUF_SIZE)
            {
                s_u8Hidx = 0;
                s_bHready = 1;
            }
        }
        else
        {
            s_u8Hidx = 0;
            s_bHready = 1;
        }
    }

    /* VOC均值 */
    l_u16V = g_u16VocData;
    if (s_bVready)
    {
        s_u32Vsum -= s_u16Vbuf[s_u8Vidx];
        s_u16Vbuf[s_u8Vidx] = l_u16V;
        s_u32Vsum += l_u16V;
        s_u8Vidx = (U8)((s_u8Vidx + 1U) % VOC_BUF_SIZE);
        s_u16Vbak = (U16)(s_u32Vsum / VOC_BUF_SIZE);
    }
    else
    {
        if (s_u8Vidx < VOC_BUF_SIZE)
        {
            s_u16Vbuf[s_u8Vidx] = l_u16V;
            s_u32Vsum += l_u16V;
            s_u8Vidx++;
            s_u16Vbak = (U16)(s_u32Vsum / s_u8Vidx);
            if (s_u8Vidx >= VOC_BUF_SIZE)
            {
                s_u8Vidx = 0;
                s_bVready = 1;
            }
        }
        else
        {
            s_u8Vidx = 0;
            s_bVready = 1;
        }
    }

    /*--------------------------------------------------------------
      两档延时跟随逻辑
    --------------------------------------------------------------*/
    /* 温度 */
    l_u8Diff = (g_s8DispTempData >= s_s8Tbak) ?
               (U8)(g_s8DispTempData - s_s8Tbak) :
               (U8)(s_s8Tbak - g_s8DispTempData);
    l_u8Delay = (l_u8Diff >= TEMP_FAST_THR) ? DELAY_FAST_SEC : DELAY_SLOW_SEC;
    if (++s_u8Tdly >= l_u8Delay)
    {
        s_u8Tdly = 0;
        g_s8DispTempData = s_s8Tbak;
    }

    /* 湿度 */
    l_u8Diff = (g_u8DispHumiData >= s_u8Hbak) ?
               (U8)(g_u8DispHumiData - s_u8Hbak) :
               (U8)(s_u8Hbak - g_u8DispHumiData);
    l_u8Delay = (l_u8Diff >= HUMI_FAST_THR) ? DELAY_FAST_SEC : DELAY_SLOW_SEC;
    if (++s_u8Hdly >= l_u8Delay)
    {
        s_u8Hdly = 0;
        g_u8DispHumiData = s_u8Hbak;
    }

    /* VOC */
    l_u16Diff = (g_u16DispVocData >= s_u16Vbak) ?
                (g_u16DispVocData - s_u16Vbak) :
                (s_u16Vbak - g_u16DispVocData);
    l_u8Delay = (l_u16Diff >= VOC_FAST_THR) ? DELAY_FAST_SEC : DELAY_SLOW_SEC;
    if (++s_u8Vdly >= l_u8Delay)
    {
        s_u8Vdly = 0;
        g_u16DispVocData = s_u16Vbak;
    }
}
