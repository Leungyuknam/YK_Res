#include "h\Allinclude.h"
//TMS6644专用
U8 xdata g_u8DryingBlinkTime = 0;           //风干闪烁时间
U8 xdata g_u8WifiBlinkTime = 0;             //WiFi闪烁时间
U8 xdata g_u8TimeHBlinkTime = 0;            //定时闪烁时间
U8 xdata g_u8WaterBlinkTime = 0;            //余水闪烁时间

U8 xdata g_u8GlobalDispState = 0;           //当前显示模式(全亮、息屏)
U8 xdata g_u8WakeDispTime = 0;              //唤醒显示时间

U8 xdata g_u8DispBright = 0;                 // 全局亮度 (0-10)，映射为255级PWM
U8 xdata g_t_u8TMS6644_DispBuf[9][16] = {0};   // 显存 [COM][SEG]
U8 xdata g_u8DispLastT = 0;
U8 xdata g_u8DispIndex = 0;
U8 xdata g_u8DispBlink = 0;
bit g_bDispMask = 0;

code TYPE_DISP_TASK DispTskTbl[DISP_TOTAL_TASK];


// =================================================================================
// 1. 图标库数据 (9x9 示例)
// =================================================================================
// WiFi 图标 (9列 x 9行), LSB=Top
U16 code t_u16IconData_WiFi[] = {
    0x0001, 0x0003, 0x0007, 0x000F, 0x001F, 0x000F, 0x0007, 0x0003, 0x0001 
};

code TYPE_ICON_DATA t_Icon_WiFi = {
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
U8 code t_u8Num_RowMap[9] = { 11,10,9,8,7,6,5,4,3 }; 
// 逻辑列 B1..D1, Gap, F1..H1 -> 接 COM 0~6
U8 code t_u8Num_ColMap[7] = { 1,2,3, 15, 5,6,7 };  // 中间Gap填了个不用的COM8

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
//U16 code t_u16PixelFont_3x9[10][3] = {
//    {0x01FF, 0x0183, 0x01FF}, // 0
//    {0x0000, 0x01FF, 0x0000}, // 1
//    {0x0179, 0x0149, 0x014F}, // 2
//    {0x0149, 0x0149, 0x01FF}, // 3
//    {0x003F, 0x0020, 0x01FF}, // 4
//    {0x014F, 0x0149, 0x0179}, // 5
//    {0x01FF, 0x0149, 0x0179}, // 6
//    {0x0003, 0x0001, 0x01FF}, // 7
//    {0x01FF, 0x0149, 0x01FF}, // 8
//    {0x004F, 0x0049, 0x01FF}  // 9
//};

// =================================================================================
// 3x9 数字字模 (0-9) - 极细版
// U16类型, Bit0=A1(顶), Bit4=中间, Bit8=A9(底)
// 完美适配 9行高度，横竖均为单像素
// =================================================================================
U16 code t_u16PixelFont_3x9[10][3] = {
    // 0: 中间镂空，只亮顶(Bit0)和底(Bit8) -> 0x0101
    {0x01FF, 0x0101, 0x01FF}, 

    // 1: 简单的竖线 (放在中间列或右边列均可，这里放中间)
    {0x0000, 0x01FF, 0x0000}, 

    // 2: 真正的 3x9 比例
    // 左列: Top(0) + Mid(4) + Bot(8) + 左下半竖(5,6,7) -> 0x1F1
    // 中列: Top(0) + Mid(4) + Bot(8) -> 0x111 (极细横线)
    // 右列: Top(0) + Mid(4) + Bot(8) + 右上半竖(1,2,3) -> 0x11F
    {0x01F1, 0x0111, 0x011F}, 

    // 3: 
    // 左/中列: 三横线 -> 0x111
    // 右列: 全竖线 -> 0x1FF
    {0x0111, 0x0111, 0x01FF}, 

    // 4: 
    // 左列: 上半竖(0,1,2,3) + 中横(4) -> 0x01F
    // 中列: 中横(4) -> 0x010
    // 右列: 全竖线 -> 0x1FF
    {0x001F, 0x0010, 0x01FF}, 

    // 5: 
    // 左列: Top + Mid + Bot + 左上半竖 -> 0x11F
    // 中列: 三横线 -> 0x111
    // 右列: Top + Mid + Bot + 右下半竖 -> 0x1F1
    {0x011F, 0x0111, 0x01F1}, 

    // 6: 
    // 左列: 全竖线 -> 0x1FF
    // 中列: 三横线 -> 0x111
    // 右列: Top + Mid + Bot + 右下半竖 -> 0x1F1
    {0x01FF, 0x0111, 0x01F1}, 

    // 7: 
    // 左/中列: 只有顶横 -> 0x001
    // 右列: 全竖线 -> 0x1FF
    {0x0001, 0x0001, 0x01FF}, 

    // 8: 全亮框
    // 左/右列: 全竖线
    // 中列: 三横线
    {0x01FF, 0x0111, 0x01FF}, 

    // 9: 
    // 左列: Top + Mid + 左上半竖 -> 0x01F
    // 中列: 三横线 -> 0x111
    // 右列: 全竖线 -> 0x1FF
    {0x011F, 0x0111, 0x01FF}  
};

#define PCOM0 0
#define PCOM1 1
#define PCOM2 2
#define PCOM3 3

#define PSEG0 0
#define PSEG1 1
#define PSEG2 2

typedef enum
{
    L_LR = 0,       // 0
    L_UP_DN,        // 1
    L_TOFF_ONE,     // 2
    L_TOFF_TWO,
    L_TOFF_FOUR,
    L_FAN1,
    L_FAN2,
    L_FAN3,
    L_WIFI,         // 示例：新增
    L_LOCK,         // 示例：新增
    
    _LED_TOTAL_     // 这是一个计数器，代表LED总数
} LIST_LED_ENUM;

// =================================================================================
// LED 物理映射表
// 下标顺序必须严格对应 LIST_LED_ENUM 的顺序！
// =================================================================================
TYPE_LED_MAP code t_stLedMap[_LED_TOTAL_] =
{
    // Index(隐式)      Com         Seg
    /* L_LR */        { PCOM1,      PSEG2 },
    /* L_UP_DN */     { PCOM1,      PSEG2 },
    /* L_TOFF_ONE */  { PCOM1,      PSEG2 },
    /* L_TOFF_TWO */  { PCOM1,      PSEG2 },
    /* L_TOFF_FOUR */ { PCOM2,      PSEG2 },
    /* L_FAN1 */      { PCOM2,      PSEG2 },
    /* L_FAN2 */      { PCOM3,      PSEG2 },
    /* L_FAN3 */      { PCOM1,      PSEG2 },
    /* L_WIFI */      { PCOM0,      PSEG2 }, // 假设
    /* L_LOCK */      { PCOM1,      PSEG2 }  // 假设
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

/**************************************************
* 函数名称：F_TMS_SetLed
* 函数功能：根据枚举索引设置LED (查表法)
* 入口参数：
* l_u8LedIndex: LED在枚举中的位置 (0, 1, 2...)
* l_bState:     1=点亮 (使用当前亮度), 0=熄灭
**************************************************/
void F_TMS_SetLed(U8 l_u8LedIndex, bit l_bState)
{
    U8 l_u8PhyCom;
    U8 l_u8PhySeg;

    // 1. 查表获取物理坐标
    // 直接用 Index 访问 code 区数组，效率极高
    l_u8PhyCom = t_stLedMap[l_u8LedIndex].u8Com;
    l_u8PhySeg = t_stLedMap[l_u8LedIndex].u8Seg;

    // 2. 安全检查 & 写入显存
    if (l_u8PhyCom < 9 && l_u8PhySeg < 16)
    {
        if (l_bState)
        {
            // 点亮时使用全局亮度 PWM 值
            g_t_u8TMS6644_DispBuf[l_u8PhyCom][l_u8PhySeg] = F_GetPWMValue();
        }
        else
        {
            g_t_u8TMS6644_DispBuf[l_u8PhyCom][l_u8PhySeg] = 0;
        }
    }
}






/**************************************************
*函数名称：void  F_DispNormal(void) 
*函数功能：常规显示
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispNormal()
{	     
	
}
/**************************************************
*函数名称：void  F_DispQuickTest(void) 
*函数功能：显示快检
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispQuickTest()
{

}
/**************************************************
*函数名称：void  F_DispShrinkTimeState(void) 
*函数功能：显示缩时状态
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispShrinkTimeState()
{

}
/**************************************************
*函数名称：void  F_ReadErrState(void) 
*函数功能：读错误状态
*入口参数：void
*出口参数：1表示有故障  
**************************************************/
U8 F_ReadErrState()
{
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

}
/**************************************************
*函数名称：void  F_DispLed(void) 
*函数功能：LED显示
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispLed()
{ 
	if(g_bTurnOn)
    {            

    }
    else
    {  
    }
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
//    F_DispPlayHex_L(g_u8KeyData);
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
//	F_SetStr(&DispBuf,0xff,TOTAL_COM);
	F_SetStr(&g_t_u8TMS6644_DispBuf[0][0], 0xff, 144);
}
/**************************************************
*函数名称：void  F_DispClean(void) 
*函数功能：清除
*入口参数：void
*出口参数：void  
**************************************************/
void F_DispClean()
{
//	F_SetStr(&DispBuf,0,TOTAL_COM);
	F_SetStr(&g_t_u8TMS6644_DispBuf[0][0], 0, 144);
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

}
/**************************************************
*函数名称：void  F_DisplayUpData(void) 
*函数功能：显示事件处理函数
*入口参数：void
*出口参数：void  
**************************************************/
void F_DisplayUpData()
{
//    if( g_bTurnOn && ((LIGHT_OFF == g_u8LightState) || (LIGHT_HALF == g_u8LightState)) && !F_ReadErrState())
//    {   
//        if(g_u8KeyDispTime)g_u8KeyDispTime--;
//    }
//    else
//    {   g_u8KeyDispTime = C_KEY_DISP_TIME;}
    
//    if( ( ((MODE_SLEEP == g_u8WorkMode) && !g_bAirDryingOn)  
//       || (g_u8LightState != LIGHT_ON) ) && g_bTurnOn )
//    {   
//        if(!g_u8KeyDispTime || !g_u8SleepDispTime || (LIGHT_ON != g_u8LightState))
//        {   g_bHalfDispEn = 1;}
//        else
//        {   g_bHalfDispEn = 0;}
//    }
//    else
//    {   g_bHalfDispEn = 0;}
    
//    if(g_bTurnOn && ((MODE_SLEEP == g_u8WorkMode) && !g_bAirDryingOn) && !F_ReadErrState())
//    {
//        if(g_u8SleepDispTime)g_u8SleepDispTime--;
//    }
//    else
//    {   g_u8SleepDispTime = C_KEY_DISP_TIME;}
    
//    if(g_bLockEn)
//    {
//        if(g_u8LockBlinkTime)
//        {   g_u8LockBlinkTime--;}
//    }
//    else
//    {   g_u8LockBlinkTime = 0;}
    
//    if(g_u8DryingBlinkTime)g_u8DryingBlinkTime--;
/***************************************************************************************/
	if(g_u8DispLastT)
	{
		g_u8DispLastT --;
//        if((F_GetDispType() >= DIPS_PM10) && (F_GetDispType() <= DIPS_PM25) && g_bAirDryingOn)
//        {
//            if(g_u8DispLastT <= 20)
//            {   F_DispTskChange(DIPS_NORMAL);}
//        }
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
    
//	F_DispLed();
    
//    if((LIGHT_OFF == g_u8LightState) && !g_u8KeyDispTime)
//    {   return;}
//    if((MODE_SLEEP == g_u8WorkMode) && !g_bAirDryingOn && !g_u8SleepDispTime)
//    {   return;}
    
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
//	{DIPS_PM10, 		50,		0,			&F_DispPM10	    },
//	{DIPS_PM1, 		    50,		0,			&F_DispPM1  	},
//	{DIPS_TEMP, 		50,		0,			&F_DispTemp 	},
//	{DIPS_HUMI, 		50,		0,			&F_DispHumi 	},
//	{DIPS_PM25, 		50,		0,			&F_DispPM25 	},
    
//	{DIPS_QUICK_TEST, 	40,		0,			&F_DispQuickTest},
//	{DIPS_SHRINK_TIME, 	40,		0,			&F_DispShrinkTimeState},
    
	{DIPS_POWERUP, 		15,		0,			&F_DispPowerUp	},
};

void F_mDisplayControl(void)
{
	static U8 s_u8TestSeg = 0;
	if(F_GetTimeTick(C_TIME_100ms))
	{
//		//显示数据刷新
//        if(g_u8TestState)
//        {   
//            F_DispClean();
//            F_SelfTestDislayUpData();
//        }
//        else
//        {   
			F_DisplayUpData();
//		}
        F_UpdateDispBuff();
//		F_SetStr(&g_t_u8TMS6644_DispBuf[0][0], 0x78, 144);
		F_SetStr(&g_t_u8TMS6644_DispBuf[0][0], 0x00, 144);
		g_u8DispBright = 10;
		F_TMS_ShowNum(&g_Area_Number,s_u8TestSeg);

		

        
//        // 1. 清屏
//        F_SetStr(&g_t_u8TMS6644_DispBuf[0][0], 0x00, 144);

//        // 2. 测试模式：固定 COM0，轮询 SEG 0-15
//        // 观察板子：当 s_u8TestSeg 变到几的时候，数码管的最上面那一段（A1）亮了？
//        // 那个值就是 t_u8Num_RowMap[0] 应该填的值。
//        
//        g_t_u8TMS6644_DispBuf[0][s_u8TestSeg] = 0xFF; // 点亮 [COM0, 当前SEG]

//        // 3. 切换下一个SEG
        s_u8TestSeg++;
        if(s_u8TestSeg > 99) s_u8TestSeg = 0;
//		F_SetStr(&g_t_u8TMS6644_DispBuf[0][0], 0xFF, 144);
		F_TMS6644_Display_PWM(&g_t_u8TMS6644_DispBuf[0][0],144);
	}
}