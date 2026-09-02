#include "h\Allinclude.h"

U8 xdata g_u8DisBright = 5;//LED灯显示亮度

U8 xdata g_u8RgbColorIndex = 0;

U8 xdata DispBuf[TOTAL_COM] = {0};
U8 xdata ChipDispBuf[TOTAL_COM] = {0};


// 颜色表
U8 code RgbOnTable[6][3] = {
    /*R,G,B*/
    {0,0,0}, // 0 OFF
	{0,0,1}, // 2 BLUE
    {0,1,0}, // 1 GREEN
    {1,1,0}, // 3 ORANGE/YELLOW
    {1,0,0}, // 4 RED
    {1,1,1}, // 5 WHITE
};

/**************************************************
*函数名称：void F_RgbDispDrv(void) 
*函数功能：根据主机的索引直接输出 RGB 颜色
**************************************************/
void F_RgbDispDrv(void)
{ 
    U8 l_u8DisR = 0;
    U8 l_u8DisG = 0;
    U8 l_u8DisB = 0;

    // 防止数组越界保护
    if(g_u8RgbColorIndex <= 5)
    {
        l_u8DisR = RgbOnTable[g_u8RgbColorIndex][C_RGB_R];
        l_u8DisG = RgbOnTable[g_u8RgbColorIndex][C_RGB_G];
        l_u8DisB = RgbOnTable[g_u8RgbColorIndex][C_RGB_B];
    }
    
    P_RGB_R = (l_u8DisR ? 0 : 1);
    P_RGB_G = (l_u8DisG ? 0 : 1);
    P_RGB_B = (l_u8DisB ? 0 : 1);
	
//	P_RGB_R = 1;
//	P_RGB_G = 1;
//	P_RGB_B = 1;	
}

/******************************************************************
 函数名称：						DisplayDrive();
 功能描述：						显示数据输出；
 调用本函数的时钟周期：			2ms
 输入参数：						无
 输出参数：						无
*******************************************************************/
void F_DisplayDrive()
{	
	F_1640Driver(ChipDispBuf,TOTAL_COM,g_u8DisBright);	
}


void F_mDisplayControl(void)
{
	unsigned char i = 0;
	if(F_GetTimeTick(C_TIME_10ms))
	{
		// --- 通讯超时看门狗处理 ---
        if(g_u8DisplayTimeOut > 0)
        {
            g_u8DisplayTimeOut--;
            if(g_u8DisplayTimeOut == 0)
            {
//                // 超时断线：清空数码管显示和 RGB 颜色
//                for(i = 0; i < 12; i++)
//                {
//                    ChipDispBuf[i] = 0x00; 
//                }
//                g_u8RgbColorIndex = 0; 
            }
        }
		
		F_RgbDispDrv();
	}
	if(F_GetTimeTick(C_TIME_100ms))
	{
		F_DisplayDrive();
	}
}
