#include "h\Allinclude.h"

U8 xdata g_u8DispCnt = 0;                   //传感器显示随机数(0~3随机)

U8 xdata g_u8VocDispState = 0;              //VOC RGB灯显示状态
U8 xdata g_u8PM25DispState = 0;             //PM25 RGB灯显示状态

U8 xdata g_u8LockBlinkTime = 0;             //童锁闪烁时间
U8 xdata g_u8DryingBlinkTime = 0;           //风干闪烁时间

U8 xdata g_u8KeyDispTime = C_KEY_DISP_TIME; //按键显示时间
U8 xdata g_u8SleepDispTime = 0;             //睡眠显示时间
bit g_bHalfDispEn = 0;                      //半亮使能标记

U8 xdata g_u8LedBlink = 0;

U16 xdata g_u16DispCH2OData = 0;            //甲醛显示数值
U16 xdata g_u16DispPm25Data = 0;            //PM25显示数值
U16 xdata g_u16DispPm10Data = 0;            //PM10显示数值
U16 xdata g_u16DispPm1Data = 0;             //PM1显示数值

U8 xdata DispBuf[TOTAL_COM] = {0};
U8 xdata ChipDispBuf[TOTAL_COM] = {0};

U8 xdata g_u8DispLastT = 0;
U8 xdata g_u8DispIndex = 0;
U8 xdata g_u8DispBlink = 0;
bit g_bDispMask = 0;

code TYPE_DISP_TASK DispTskTbl[DISP_TOTAL_TASK];

U8 code NumTable[16] = {N_0,N_1,N_2,N_3,N_4,N_5,N_6,N_7,N_8,N_9,N_A,N_B,N_C,N_D,N_E,N_F};

U8 code SegTable_UP[10][3] = 
{               
                c_SEGA,     c_SEGA,     c_SEGA, 
                     0,     c_SEGA,     (c_SEGA|c_SEGB),
                     0,          0,     (c_SEGA|c_SEGB|c_SEGC),
                     0,          0,     (c_SEGB|c_SEGC|c_SEGD),
                     0,     c_SEGD,     (c_SEGC|c_SEGD),
                c_SEGD,     c_SEGD,     c_SEGD,
       (c_SEGD|c_SEGE),     c_SEGD,     0,
(c_SEGD|c_SEGE|c_SEGF),          0,     0,
(c_SEGA|c_SEGE|c_SEGF),          0,     0,
       (c_SEGA|c_SEGF),     c_SEGA,     0,                     
};

                       
U8 code SegTable_DOWN[12][4] = 
{              
                c_SEGA,     c_SEGA,     c_SEGA,      0,
                     0,     c_SEGA,     c_SEGA,      c_SEGA,
                     0,          0,     c_SEGA,      (c_SEGA|c_SEGB),
                     0,          0,     0,           (c_SEGA|c_SEGB|c_SEGC),
                     0,          0,     0,           (c_SEGB|c_SEGC|c_SEGD),
                     0,          0,     c_SEGD,      (c_SEGC|c_SEGD),
                     0,     c_SEGD,     c_SEGD,      c_SEGD,
                c_SEGD,     c_SEGD,     c_SEGD,      0,
       (c_SEGD|c_SEGE),     c_SEGD,          0,      0,
(c_SEGD|c_SEGE|c_SEGF),          0,          0,      0,
(c_SEGA|c_SEGE|c_SEGF),          0,          0,      0,
       (c_SEGA|c_SEGF),     c_SEGA,          0,      0,               
};



//TMS6644专用
U8 xdata g_u8DryingBlinkTime = 0;           //风干闪烁时间
U8 xdata g_u8WifiBlinkTime = 0;             //WiFi闪烁时间
U8 xdata g_u8TimeHBlinkTime = 0;            //定时闪烁时间
U8 xdata g_u8WaterBlinkTime = 0;            //余水闪烁时间

bit RGBEn = 0;								//RGB使能
U8 xdata g_u8RGBDispState = 0;				//RGB显示状态

U8 xdata g_u8DispHumiData = 0;              //湿度显示数值

U8 xdata g_u8GlobalDispState = 0;           //当前显示模式(全亮、息屏)
U8 xdata g_u8WakeDispTime = 0;              //唤醒显示时间

U8 xdata g_u8DispBright = 0;                 // 全局亮度 (0-10)，映射为255级PWM
U8 xdata g_t_u8TMS6644_DispBuf[9][16] = {0};   // 显存 [COM][SEG]
U8 xdata g_u8DispLastT = 0;
U8 xdata g_u8DispIndex = 0;
U8 xdata g_u8DispBlink = 0;
bit g_bDispMask = 0;

code TYPE_DISP_TASK DispTskTbl[DISP_TOTAL_TASK]















// =================================================================================
// 1. 图标库数据 (9x9 示例)
// =================================================================================
// WiFi 图标 (9列 x 9行), LSB=Top
U16 code t_u16IconData_WiFi[] = {
    0x0001, 0x0003, 0x0007, 0x000F, 0x001F, 0x000F, 0x0007, 0x0003, 0x0001 
};

TYPE_ICON_DATA code t_Icon_WiFi = {
    t_u16IconData_WiFi, 9, 9 // 数据, 宽, 高
};

// =================================================================================
// 2. 硬件映射配置 (乱序处理)
// =================================================================================

// 【场景假设】：
// - 数字区域(2位) 占用了部分引脚
// - 图标区域(9x9) 占用了 COM0~8 和 SEG0~8
// - 指示灯 占用了 SEG15 (非常重要：映射表里不能有SEG15)

// --- A. 数字显示区域 (2位数字) ---
// 逻辑行 A1~A9 -> 接 SEG 0~8
U8 code t_u8Num_RowMap[9] = { 0,1,2,3,4,5,6,7,8 }; 
// 逻辑列 B1..D1, Gap, F1..H1 -> 接 COM 0~6
U8 code t_u8Num_ColMap[7] = { 0,1,2, 8, 3,4,5 };  // 中间Gap填了个不用的COM8

TYPE_DIGITAL_AREA code g_Area_Number = {
    t_u8Num_ColMap, t_u8Num_RowMap, 2 // 2位数
};

// --- B. 9x9 图标显示区域 ---
// 逻辑行 0~8 -> 接 SEG 0~8 (注意不要包含LED用的SEG15)
U8 code t_u8Matrix_RowMap[9] = { 0,1,2,3,4,5,6,7,8 };
// 逻辑列 0~8 -> 接 COM 0~8
U8 code t_u8Matrix_ColMap[9] = { 0,1,2,3,4,5,6,7,8 };

TYPE_DIGITAL_AREA code g_Area_Icon9x9 = {
    t_u8Matrix_ColMap, t_u8Matrix_RowMap, 0 // 图标区域不需要DigitCount
};

// --- C. 指示灯定义 (直接物理坐标) ---
#define C_LED_POWER_COM  0
#define C_LED_POWER_SEG  15  // 独立于点阵的SEG

#define C_LED_LOCK_COM   1
#define C_LED_LOCK_SEG   15










// =================================================================================
// 内部常量
// =================================================================================

// Gamma 2.2 亮度表
U8 code t_u8BrightGammaMap[11] = { 0, 4, 8, 16, 28, 45, 68, 95, 128, 180, 255 };

// 3x9 数字字模 (0-9)
// U16类型, Bit0=A1(顶), Bit8=A9(底)
U16 code t_u16PixelFont_3x9[10][3] = {
    {0x01FF, 0x0183, 0x01FF}, // 0
    {0x0000, 0x01FF, 0x0000}, // 1
    {0x0179, 0x0149, 0x014F}, // 2
    {0x0149, 0x0149, 0x01FF}, // 3
    {0x003F, 0x0020, 0x01FF}, // 4
    {0x014F, 0x0149, 0x0179}, // 5
    {0x01FF, 0x0149, 0x0179}, // 6
    {0x0003, 0x0001, 0x01FF}, // 7
    {0x01FF, 0x0149, 0x01FF}, // 8
    {0x004F, 0x0049, 0x01FF}  // 9
};

// =================================================================================
// 内部辅助函数
// =================================================================================

// 获取PWM值
U8 F_GetPWMValue(void)
{
    if (g_u8DispBright > C_DISP_MAX_BRIGHT_LEVEL) return 255;
    return t_u8BrightGammaMap[g_u8DispBright];
}

/**************************************************
* 核心物理映射写入函数 (Private)
* 功能：根据映射表，只修改特定的像素点，保护其他区域(如LED)
**************************************************/
void F_TMS_Mapping_Write(TYPE_DIGITAL_AREA *l_pArea, U8 l_u8LogicCol, U16 l_u16ColData)
{
    U8 l_u8Y;
    U8 l_u8PhyCom, l_u8PhySeg;
    U8 l_u8PwmVal;

    l_u8PwmVal = F_GetPWMValue();

    // 1. 查列映射表: 逻辑列 -> 物理COM
    l_u8PhyCom = l_pArea->pColToComMap[l_u8LogicCol];

    // 2. 遍历高度 (行 A1-A9)
    for (l_u8Y = 0; l_u8Y < C_FONT_HEIGHT; l_u8Y++)
    {
        // 3. 查行映射表: 逻辑行 -> 物理SEG
        l_u8PhySeg = l_pArea->pRowToSegMap[l_u8Y];

        // 4. 安全检查
        if (l_u8PhyCom < 9 && l_u8PhySeg < 16)
        {
            // 仅当映射表中定义了有效的SEG/COM时才写入
            // 这保证了不会误触碰到指示灯的SEG(例如SEG15)
            if (l_u16ColData & (1 << l_u8Y))
            {
                g_t_u8TMS6644_DispBuf[l_u8PhyCom][l_u8PhySeg] = l_u8PwmVal;
            }
            else
            {
                g_t_u8TMS6644_DispBuf[l_u8PhyCom][l_u8PhySeg] = 0;
            }
        }
    }
}

// =================================================================================
// 外部接口实现
// =================================================================================

// 显示数字
void F_TMS_ShowNum(TYPE_DIGITAL_AREA *l_pArea, U16 l_u16Num)
{
    U8 l_u8Cnt, l_u8X;
    U8 l_t_u8DispData[4];
    U8 l_u8LogicColIndex = 0; 

    // 拆分数字
    if (l_pArea->u8DigitCount == 2)
    {
        if (l_u16Num > 99) l_u16Num = 99;
        l_t_u8DispData[0] = l_u16Num / 10;
        l_t_u8DispData[1] = l_u16Num % 10;
    }
    // (可扩展3位逻辑)

    // 循环绘制每一位
    for (l_u8Cnt = 0; l_u8Cnt < l_pArea->u8DigitCount; l_u8Cnt++)
    {
        for (l_u8X = 0; l_u8X < C_FONT_WIDTH; l_u8X++)
        {
            F_TMS_Mapping_Write(l_pArea, 
                              l_u8LogicColIndex + l_u8X, 
                              t_u16PixelFont_3x9[l_t_u8DispData[l_u8Cnt]][l_u8X]);
        }
        // 逻辑列步进: 字宽(3) + 间隔(1)
        l_u8LogicColIndex += (C_FONT_WIDTH + 1); 
    }
}

// 显示图标
void F_TMS_ShowIcon(TYPE_DIGITAL_AREA *l_pArea, TYPE_ICON_DATA *l_pIcon, U8 l_u8Ofst)
{
    U8 l_u8X;
    for (l_u8X = 0; l_u8X < l_pIcon->u8Width; l_u8X++)
    {
        F_TMS_Mapping_Write(l_pArea, 
                          l_u8Ofst + l_u8X, 
                          l_pIcon->pIconData[l_u8X]);
    }
}

// 区域清除 (写0)
void F_TMS_ClearArea(TYPE_DIGITAL_AREA *l_pArea, U8 l_u8Width)
{
    U8 l_u8X;
    for (l_u8X = 0; l_u8X < l_u8Width; l_u8X++)
    {
        F_TMS_Mapping_Write(l_pArea, l_u8X, 0x0000);
    }
}

// 独立控制指示灯 (不走映射表，直接操作BUF)
void F_TMS_SetLed(U8 l_u8Com, U8 l_u8Seg, bit l_bState)
{
    if (l_u8Com < 9 && l_u8Seg < 16)
    {
        if (l_bState)
            g_t_u8TMS6644_DispBuf[l_u8Com][l_u8Seg] = F_GetPWMValue();
        else
            g_t_u8TMS6644_DispBuf[l_u8Com][l_u8Seg] = 0;
    }
}









/**************************************************
*函数名称：void  F_DispNum_Up(void) 
*函数功能：上三八显示
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispNum_Up(U16 l_u16Num)
{
    if(l_u16Num > 999)
    {   l_u16Num = 999;}
    
    if(l_u16Num >= 100)
    {   DispBuf[COM_2] |= NumTable[l_u16Num / 100];}
    
    if(l_u16Num >= 10)
    {   DispBuf[COM_3] |= NumTable[l_u16Num % 100 / 10];}
    
    DispBuf[COM_4] |= NumTable[l_u16Num % 10];
}
/**************************************************
*函数名称：void  F_DispNum_Up_S(void) 
*函数功能：上三八有符号显示
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispNum_Up_S(S8 l_s8Num)
{
    if(l_s8Num > 99)
    {   l_s8Num = 99;}
    else if(l_s8Num < -99)
    {   l_s8Num = -99;}
    
    if(l_s8Num < 0)
    {
        l_s8Num *= -1;
        if(l_s8Num >= 10)
        {   DispBuf[COM_2] |= N__;}
        else
        {   DispBuf[COM_3] |= N__;}
    }
    if(l_s8Num >= 10)
    {   DispBuf[COM_3] |= NumTable[l_s8Num / 10];}
    DispBuf[COM_4] |= NumTable[l_s8Num % 10];
}
/**************************************************
*函数名称：void  F_DispNum_Down(void) 
*函数功能：下四八显示
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispNum_Down(U16 l_u16Num)
{
    if(l_u16Num > 9999)
    {   l_u16Num = 9999;}
    
    DispBuf[COM_6] |= NumTable[l_u16Num / 1000];
    DispBuf[COM_7] |= NumTable[l_u16Num % 1000 / 100];
    DispBuf[COM_8] |= NumTable[l_u16Num % 100 / 10];
    DispBuf[COM_9] |= NumTable[l_u16Num % 10];
}
/**************************************************
*函数名称：void  F_DispPreheat_Up(void) 
*函数功能：PM25数码管显示预热界面
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispPreheat_Up()
{	
    static U8 s_u8Cnt = 0;
    
    if(g_u16SenseEnTime <= 20)
    {
        F_DispNum_Up(g_u8FhFilterPer);
        DIS_BAI_FEN_BI();
        DIS_STRAINER();
        DIS_CLE_PM1_10_25();
        s_u8Cnt = 0;
    }
    else
    {
        if(!(g_u16SenseEnTime % 10))
        {
            if(++s_u8Cnt >= 10)
            {   s_u8Cnt = 0;}
        }
        F_SpecialDis_Up(SegTable_UP[s_u8Cnt][0],SegTable_UP[s_u8Cnt][1],SegTable_UP[s_u8Cnt][2]);
    }
}
/**************************************************
*函数名称：void  F_DispPreheat_Down(void) 
*函数功能：甲醛数码管显示预热界面
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispPreheat_Down()
{	
    static U8 s_u8Cnt = 0;
    
    if(g_u16SenseEnTime <= 20)
    {
        s_u8Cnt = 0;
        DIS_CLE_METHANAL();
    }
    else
    {
        if(!(g_u16SenseEnTime % 10))
        {
            if(++s_u8Cnt >= 12)
            {   s_u8Cnt = 0;}
        }
        F_SpecialDis_Down(SegTable_DOWN[s_u8Cnt][0],SegTable_DOWN[s_u8Cnt][1],SegTable_DOWN[s_u8Cnt][2],SegTable_DOWN[s_u8Cnt][3]);
    }
}
/**************************************************
*函数名称：void  F_DispCH2O(void) 
*函数功能：显示甲醛数值
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispCH2O()
{	    
    DIS_METHANAL();
    if(g_u16SenseEnTime < c_STIVE_PREHEAT_TIME)
    {   F_DispPreheat_Down();}
    else if(g_u8CH2OLinkTime)
    {   
        F_DispNum_Down(g_u16DispCH2OData);
        DIS_DP();
    }
    else
    {   F_SpecialDis_Down(N__,N__,N__,N__);}
}
/**************************************************
*函数名称：void  F_DispPM10(void) 
*函数功能：显示PM10
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispPM10()
{	
    DIS_PM10();
    if(g_u16SenseEnTime < c_STIVE_PREHEAT_TIME)
    {   F_DispPreheat_Up();}
    else if(C_STIVE_ERR == g_u8StiveState)
    {   F_SpecialDis_Up(N__,N__,N__);}
    else
    {   F_DispNum_Up(g_u16DispPm10Data);}
}
/**************************************************
*函数名称：void  F_DispPM1(void) 
*函数功能：显示PM1
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispPM1()
{	
    DIS_PM1();
    if(g_u16SenseEnTime < c_STIVE_PREHEAT_TIME)
    {   F_DispPreheat_Up();}
    else if(C_STIVE_ERR == g_u8StiveState)
    {   F_SpecialDis_Up(N__,N__,N__);}
    else
    {   F_DispNum_Up(g_u16DispPm1Data);}
}
/**************************************************
*函数名称：void  F_DispPM25(void) 
*函数功能：显示PM25
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispPM25()
{	
    DIS_PM25();
    if(g_u16SenseEnTime < c_STIVE_PREHEAT_TIME)
    {   F_DispPreheat_Up();}
    else if(C_STIVE_ERR == g_u8StiveState)
    {   F_SpecialDis_Up(N__,N__,N__);}
    else
    {   F_DispNum_Up(g_u16DispPm25Data);}
}
/**************************************************
*函数名称：void  F_DispTemp(void) 
*函数功能：显示温度
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispTemp()
{	
    if(g_u8CH2OLinkTime)
    {   F_DispNum_Up_S(g_s8NowTemp);}
    else
    {   F_SpecialDis_Up(N__,N__,N__);}
    DIS_TEMP();
}
/**************************************************
*函数名称：void  F_DispHumi(void) 
*函数功能：显示湿度
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispHumi()
{	
    if(g_u8CH2OLinkTime)
    {   F_DispNum_Up(g_u8NowHumi);}
    else
    {   F_SpecialDis_Up(N__,N__,N__);}
    DIS_BAI_FEN_BI();
}
/**************************************************
*函数名称：void  F_DispNormal(void) 
*函数功能：常规显示
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispNormal()
{	     
    if(g_bTurnOn)
    {
        if(g_bAirDryingOn)
        {
            F_DispNum_Up(30 - (g_u16AirDryingTime / 60));
        }
        else
        {   F_DispPM25();}
    }
    else
    {   
    }
}
/**************************************************
*函数名称：void  F_DispQuickTest(void) 
*函数功能：显示快检
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispQuickTest()
{
    if(g_u8DispLastT > 20)
    {   F_SpecialDis_Up(N_U,N_0,N_8);}
    else if(g_u8DispLastT > 10)
    {   F_SpecialDis_Up(N_L,N_1,N_0);}
    else
    {
        DispBuf[COM_2] |= N_U;
        DispBuf[COM_3] |= NumTable[g_u8DispVersion / 10];
        DispBuf[COM_4] |= NumTable[g_u8DispVersion % 10];
    }
}
/**************************************************
*函数名称：void  F_DispShrinkTimeState(void) 
*函数功能：显示缩时状态
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispShrinkTimeState()
{
    if(g_bShrinkTimeFlg)
    {   F_SpecialDis_Up(0,N_o,N_n);}
    else
    {   F_SpecialDis_Up(N_o,N_F,N_F);}
}
/**************************************************
*函数名称：void  F_ReadErrState(void) 
*函数功能：读错误状态
*入口参数：void
*出口参数：1表示有故障  
**************************************************/
U8 F_ReadErrState()
{
    if(g_bSnCodeError)
    {   
        return 1;
    }
    else if(g_bQuickTest && (g_bInFanErr || !g_u8CH2OLinkTime || !g_u8VocLinkTime || !g_u8StiveState || fgBlueError || fgWifiError_AP))
    {
        return 1;
    }   
    else if(g_bUartErrFlg)
    {   
        return 1;
    }
    
    return 0;
}
/**************************************************
*函数名称：void  F_DispErrState(void) 
*函数功能：显示错误状态
*入口参数：void
*出口参数：void
**************************************************/
void F_DispErrState()
{
    if(g_bSnCodeError)
    {   
        F_SpecialDis_Up(0,N_5,N_n);
    }
    else if(g_bQuickTest && (g_bInFanErr || !g_u8CH2OLinkTime || !g_u8VocLinkTime || !g_u8StiveState || fgBlueError || fgWifiError_AP))
    {
        if(fgBlueError)
        {   F_SpecialDis_Up(0,N_F,N_L);}
        else if(fgWifiError_AP)
        {   F_SpecialDis_Up(0,N_F,N_P);}
        else if(g_bInFanErr)
        {   F_SpecialDis_Up(0,N_F,N_5);}
        else if(!g_u8CH2OLinkTime)
        {   F_SpecialDis_Up(0,N_A,N_E);}
        else if(!g_u8VocLinkTime)
        {   F_SpecialDis_Up(0,N_F,N_4);}
        else
        {   F_SpecialDis_Up(0,N_F,N_U);}
    }   
    else if(g_bUartErrFlg)
    {   
        F_SpecialDis_Up(0,N_F,N_7);
    }
}
/**************************************************
*函数名称：void  F_DispLed(void) 
*函数功能：LED显示
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispLed()
{
    static U8 s_u8VocDispStateBak = 0;
    static U8 s_u8PM25DispStateBak = 0;
    
    if(g_bTurnOn)
    {            
        DIS_NETION_KEY();
        DIS_DRYING_KEY();
        DIS_MESSAGE_KEY();
        DIS_LIGHT_KEY();
        DIS_MODE_KEY();
        if(g_u8LockBlinkTime)
        {
            if((g_u8LockBlinkTime % 10) >= 5)
            {   DIS_LOCK_KEY();}
        }
        else
        {   DIS_LOCK_KEY();}
        
        if(F_ReadErrState())
        {   return;}
        
        if(g_bAirDryingOn)
        {   
            if(g_u8DryingBlinkTime)
            {
                if((g_u8DryingBlinkTime % 10) >= 5)
                {   DIS_DRYING();}
            }
            else
            {   DIS_DRYING();}
        }
        else if(MODE_LOW == g_u8WorkMode)
        {   DIS_GEAR_1();}
        else if(MODE_MID == g_u8WorkMode)
        {   DIS_GEAR_2();}
        else if(MODE_HIGH == g_u8WorkMode)
        {   DIS_GEAR_3();}
        else if(MODE_AI == g_u8WorkMode)
        {   DIS_AI();}
        else if(MODE_SLEEP == g_u8WorkMode)
        {   DIS_SLEEP();}
        
        if(g_bLockEn)
        {
            if(g_u8LockBlinkTime)
            {
                if((g_u8LockBlinkTime % 10) >= 5)
                {   DIS_LOCK();}
            }
            else
            {   DIS_LOCK();}
        }
        
        if((LIGHT_OFF == g_u8LightState) && !g_u8KeyDispTime)
        {   return;}
        if((MODE_SLEEP == g_u8WorkMode) && !g_bAirDryingOn && !g_u8SleepDispTime)
        {   return;}
        
        if(g_bNetIonEn)
        {   DIS_NETION();}
        
        if(g_u16SenseEnTime < c_STIVE_PREHEAT_TIME)
        {
            if(0 == (g_u16SenseEnTime % 10))
            {
                if(++s_u8PM25DispStateBak > C_DISP_READ)
                {   s_u8PM25DispStateBak = C_DISP_BLUE;}
            }
            g_u8PM25DispState = s_u8PM25DispStateBak;
        }
        else
        {
            s_u8PM25DispStateBak = 0;
            if(g_u8PM25AirQuality >= g_u8CH2OAirQuality)
            {   g_u8PM25DispState = g_u8PM25AirQuality + 1;}
            else
            {   g_u8PM25DispState = g_u8CH2OAirQuality + 1;}
        }
        
        if(g_bAirDryingOn)
        {   return;}
        
        F_DispCH2O();
//        F_DispPlayHex_Down(InFan.Duty);
        
        if(g_u16SenseEnTime < c_VOC_PREHEAT_TIME)
        {
            if(g_u16SenseEnTime < 100)
            {
                if(0 == (g_u16SenseEnTime % 10))
                {
                    if(++s_u8VocDispStateBak > C_DISP_READ)
                    {   s_u8VocDispStateBak = C_DISP_BLUE;}
                }
            }
            else
            {   s_u8VocDispStateBak = C_DISP_BLUE;}
            g_u8VocDispState = s_u8VocDispStateBak;
        }
        else
        {
            s_u8VocDispStateBak = 0;
            g_u8VocDispState = g_u8VocAirQuality + 1;
        }
        
        if(g_bScreenInitFlg)
        {   
            DIS_STRAINER();
            DIS_RESET();
        }
    }
    else
    {  
        s_u8VocDispStateBak = 0;
        s_u8PM25DispStateBak = 0;
        if(g_bLockEn)
        {
            if(g_u8LockBlinkTime)
            {
                if((g_u8LockBlinkTime % 10) >= 5)
                {   
                    DIS_LOCK_KEY();
                    DIS_LOCK();
                }
            }
            else
            {   
                DIS_LOCK_KEY();
                DIS_LOCK();
            }
        }
    }
    if(S_API_WifiDisp())
    {   DIS_WIFI();}
}
/**************************************************
*函数名称：void  F_DispTime(void) 
*函数功能：显示时间
*入口参数：void
*出口参数：void  
**************************************************/
//void F_DispTime()
//{
//	DispBuf[COM_1] |= NumTable[g_u8TimeData / 10];
//	DispBuf[COM_2] |= NumTable[g_u8TimeData % 10];
//}
/**************************************************
*函数名称：void  F_DispKey(void) 
*函数功能：显示Key
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispKey()
{
//	DispBuf[COM_1] = NumTable[g_u8KeyData >> 4];
//	DispBuf[COM_2] = NumTable[g_u8KeyData & 0X0F];
    F_DispPlayHex_L(g_u8KeyData);
}
//--------------------------不需修改-----------------------------------------
/**************************************************
*函数名称：void  F_DispPowerUp(void) 
*函数功能：全显
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispPowerUp()
{
	F_SetStr(&DispBuf,0xff,TOTAL_COM);
    g_u8VocDispState =  C_DISP_ALL;
    g_u8PM25DispState =  C_DISP_ALL;
}
/**************************************************
*函数名称：void  F_DispClean(void) 
*函数功能：清除
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispClean()
{
	F_SetStr(&DispBuf,0,TOTAL_COM);
    g_u8VocDispState =  C_DISP_OFF;
    g_u8PM25DispState =  C_DISP_OFF;
}
/**************************************************
*函数名称：void  F_mDispInital(void) 
*函数功能：显示初始
*入口参数：void
*出口参数：void  
**************************************************/
void F_mDispInital()
{
	F_DispTskChange(DIPS_POWERUP);
}
/**************************************************
*函数名称：void  F_GetDispType(void) 
*函数功能：获取当前显示状态
*入口参数：void
*出口参数：void  
**************************************************/
U8 F_GetDispType()
{
	return DispTskTbl[g_u8DispIndex].Index;
}
/**************************************************
*函数名称：void  F_DispTskChange(void) 
*函数功能：显示事件切换函数
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispTskChange(unsigned char temp)
{
	unsigned char i;
	for(i = 0; i < DISP_TOTAL_TASK; i ++)
	{
		if(DispTskTbl[i].Index == temp)
		{
			g_u8DispIndex = i;
			g_u8DispLastT = DispTskTbl[i].LastTime;
			g_u8DispBlink = DispTskTbl[i].Blink;
			g_bDispMask = 0;
			return;
		}
	}
}
//------------------------------------------------------------------
/******************************************************************
*函数名称：void  F_UpdateDispBuff(void) 
*函数功能：最终Buf数据刷新
*入口参数：void
*出口参数：void  
*******************************************************************/
void F_UpdateDispBuff()
{   
	ChipDispBuf[0] = DispBuf[COM_1];	
	ChipDispBuf[1] = DispBuf[COM_11];	
	ChipDispBuf[2] = DispBuf[COM_9];
	ChipDispBuf[3] = DispBuf[COM_12];
	ChipDispBuf[4] = DispBuf[COM_8];	
	ChipDispBuf[5] = DispBuf[COM_10];
	ChipDispBuf[6] = DispBuf[COM_6];
	ChipDispBuf[7] = DispBuf[COM_4];
	ChipDispBuf[8] = DispBuf[COM_3];	
	ChipDispBuf[9] = DispBuf[COM_2];
	ChipDispBuf[10] = DispBuf[COM_7];
	ChipDispBuf[11] = DispBuf[COM_5];
}
/**************************************************
*函数名称：void  F_DisplayUpData(void) 
*函数功能：显示事件处理函数
*入口参数：void
*出口参数：void  
**************************************************/
void F_DisplayUpData()
{
    if( g_bTurnOn && ((LIGHT_OFF == g_u8LightState) || (LIGHT_HALF == g_u8LightState)) && !F_ReadErrState())
    {   
        if(g_u8KeyDispTime)g_u8KeyDispTime--;
    }
    else
    {   g_u8KeyDispTime = C_KEY_DISP_TIME;}
    
    if( ( ((MODE_SLEEP == g_u8WorkMode) && !g_bAirDryingOn)  
       || (g_u8LightState != LIGHT_ON) ) && g_bTurnOn )
    {   
        if(!g_u8KeyDispTime || !g_u8SleepDispTime || (LIGHT_ON != g_u8LightState))
        {   g_bHalfDispEn = 1;}
        else
        {   g_bHalfDispEn = 0;}
    }
    else
    {   g_bHalfDispEn = 0;}
    
    if(g_bTurnOn && ((MODE_SLEEP == g_u8WorkMode) && !g_bAirDryingOn) && !F_ReadErrState())
    {
        if(g_u8SleepDispTime)g_u8SleepDispTime--;
    }
    else
    {   g_u8SleepDispTime = C_KEY_DISP_TIME;}
    
    if(g_bLockEn)
    {
        if(g_u8LockBlinkTime)
        {   g_u8LockBlinkTime--;}
    }
    else
    {   g_u8LockBlinkTime = 0;}
    
    if(g_u8DryingBlinkTime)g_u8DryingBlinkTime--;
/***************************************************************************************/
	if(g_u8DispLastT)
	{
		g_u8DispLastT --;
        if((F_GetDispType() >= DIPS_PM10) && (F_GetDispType() <= DIPS_PM25) && g_bAirDryingOn)
        {
            if(g_u8DispLastT <= 20)
            {   F_DispTskChange(DIPS_NORMAL);}
        }
		if(g_u8DispLastT == 0)
		{
			F_DispTskChange(DIPS_NORMAL);
		}
	}
	if(g_u8DispBlink)
	{
		g_u8DispBlink --;
		if(g_u8DispBlink == 0)
		{
			g_u8DispBlink = DispTskTbl[g_u8DispIndex].Blink;
			g_bDispMask = !g_bDispMask;
		}
	}
    
	F_DispClean();
    
	F_DispLed();
    
    if((LIGHT_OFF == g_u8LightState) && !g_u8KeyDispTime)
    {   return;}
    if((MODE_SLEEP == g_u8WorkMode) && !g_bAirDryingOn && !g_u8SleepDispTime)
    {   return;}
    
    if(F_ReadErrState())
    {   
        F_DispErrState();
    }
    else if(g_bDispMask == 0)
	{
		(DispTskTbl[g_u8DispIndex].DispFunc)();
	}
}

/******************************************************************
 函数名称：						DisplayDrive();
 功能描述：						显示数据输出；
 调用本函数的时钟周期：			2ms
 输入参数：						无
 输出参数：						无
*******************************************************************/
/*void F_DisplayDrive()
{	
	static U8 s_u8ComBeat = 0;
    static U8 s_u82msTime = 0;
    
    if(++s_u82msTime >= 16)
    {
        s_u82msTime = 0;
        F_UpdateDispBuff();
        F_ComsOff();

        P_SEG1 = ChipDispBuf[s_u8ComBeat] & BIT0;
        P_SEG2 = ChipDispBuf[s_u8ComBeat] & BIT1;
        P_SEG3 = ChipDispBuf[s_u8ComBeat] & BIT2;
        P_SEG4 = ChipDispBuf[s_u8ComBeat] & BIT3;
        P_SEG5 = ChipDispBuf[s_u8ComBeat] & BIT4;
        P_SEG6 = ChipDispBuf[s_u8ComBeat] & BIT5;
        P_SEG7 = ChipDispBuf[s_u8ComBeat] & BIT6;
        P_SEG8 = ChipDispBuf[s_u8ComBeat] & BIT7;
        
        switch (s_u8ComBeat)
        {	case 0:
                F_Com1On();
                break;
            case 1:
                F_Com2On();
                break;
            case 2:
                F_Com3On();
                break;	
            default:
                break;
        }
        
        if(++s_u8ComBeat > 2)
        {	
            s_u8ComBeat = 0;
        }
    }
}*/

code TYPE_DISP_TASK DispTskTbl[DISP_TOTAL_TASK] = 
{
	// Index,		LastTime,	Blink *100ms,		Function
	{DIPS_NORMAL, 		0,		0,			&F_DispNormal	},
	{DIPS_PM10, 		50,		0,			&F_DispPM10	    },
	{DIPS_PM1, 		    50,		0,			&F_DispPM1  	},
	{DIPS_TEMP, 		50,		0,			&F_DispTemp 	},
	{DIPS_HUMI, 		50,		0,			&F_DispHumi 	},
	{DIPS_PM25, 		50,		0,			&F_DispPM25 	},
    
	{DIPS_QUICK_TEST, 	40,		0,			&F_DispQuickTest},
	{DIPS_SHRINK_TIME, 	40,		0,			&F_DispShrinkTimeState},
    
	{DIPS_POWERUP, 		15,		0,			&F_DispPowerUp	},
};

void F_mDisplayControl(void)
{
	if(F_GetTimeTick(C_TIME_100ms))
	{
		//显示数据刷新
        if(g_u8TestState)
        {   
            F_DispClean();
            F_SelfTestDislayUpData();
        }
        else
        {   F_DisplayUpData();}
        F_UpdateDispBuff();
        g_u8LedBlink++;
	}
}

U8 xdata DZDispBuf[TOTAL_COM][TOTAL_SEG] = {0};

void F_SpecialDis(u8 x,u8 y)
{
	
}