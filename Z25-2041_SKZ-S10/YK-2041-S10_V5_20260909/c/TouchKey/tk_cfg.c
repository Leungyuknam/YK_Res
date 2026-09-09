//===============================================================================================
//        中颖 SH79F328系列 触摸库 V2.4
//       -------------------------------
//===============================================================================================

#include <intrins.h>
#include <ABSACC.H>
#include "STRING.H"
//#include "SH79F9463A.h"
//#include "tk.h"
//#include "tk_check.h"
#include "h\Allinclude.h"
#include "h\SH79F9463A.H"
#include "c\TouchKey\tk.h"
#include "c\TouchKey\tk_check.h"



U8      xdata       gBKeyValidFg;
U16     xdata       gWRestT;
U16     xdata       gWFailT;
U16     xdata       gWrCurKey;
U8      xdata       gBLatchErrFg;

U16     xdata       Tab_TKMax[MAX_KEYS*2];
U16	    xdata       HF_DIV[MAX_KEYS];				    //高频放大倍数
U16	    xdata       LF_DIV[MAX_KEYS];				 	//低频放大倍数

#if HAND_TK_ENABLE
    #pragma message "打开支持手直接摸功能"
#else
    #pragma message "关闭支持手直接摸功能"
#endif

//内部基准电压选择位
#define     TK_VREF_SEL_2V          0x00
#define     TK_VREF_SEL_1P5V        0x40
#define     TK_VREF_SEL_1V          0x80
#define     TK_VREF_SEL_0P6V        0xC0
// 去抖动时间选择位
#define     TK_CMP_Delay_8SCK       0x00
#define     TK_CMP_Delay_16SCK      0x10
#define     TK_CMP_Delay_32SCK      0x20
#define     TK_CMP_Delay_64SCK      0x30
// OP输出电压选择位
#define     TK_OP_SEL_4V            0x00
#define     TK_OP_SEL_3V            0x04
#define     TK_OP_SEL_2V            0x08
#define     TK_OP_SEL_1P2V          0x0C
// 放电时间调整选择位
#define     TK_Dischg_256SCK        0x00
#define     TK_Dischg_384SCK        0x01
#define     TK_Dischg_512SCK        0x02
#define     TK_Dischg_640SCK        0x03

// 触摸扫描频率
#define     _setTKFreq(n)       ((U8)((TK_CLOCK_FREQ)/(n)))
#define     FREQ_3M             _setTKFreq(3)
#define     FREQ_2_4M           _setTKFreq(2.4)
#define     FREQ_2M             _setTKFreq(2)
#define     FREQ_1_7M           _setTKFreq(1.7)
#define     FREQ_1_5M           _setTKFreq(1.5)
#define     FREQ_1_33M          _setTKFreq(1.33)
#define     FREQ_1_2M           _setTKFreq(1.2)
#define     FREQ_1_09M          _setTKFreq(1.09)
#define     FREQ_1M             _setTKFreq(1)
#define     FREQ_923K           _setTKFreq(0.923)
#define     FREQ_857K           _setTKFreq(0.857)

//触摸通道设置，板子有多少隔触摸通道，则设置多少 ,对应通道号TKn-1
#define TK(n)       (n-1)


//打 *** 可以改
// 触摸通道: 可建议设置触摸通道错开填写配置，比如板子上的顺序 1，2，3，4， 可错开填写配置成1，3，2，4
Struct_TK_Para code TK_Para =
{
    4,     // 按键个数     这个数要<=MAX_KEYS
    {TK(5),TK(6),TK(7),TK(8),
	 0, 0, 0, 0,},  // 通道   0表示TK1....23表示TK24
    /* 低频放大倍数 */    //(***)
    {
        LF_DIV0, LF_DIV1, LF_DIV2,  LF_DIV3,  LF_DIV4,  LF_DIV5,  LF_DIV6,  LF_DIV7,
        LF_DIV8, LF_DIV9, LF_DIV10, LF_DIV11, LF_DIV12, LF_DIV13, LF_DIV14, LF_DIV15,
    },
    /* (***)高频放大倍数 */   //(***)
    {
        HF_DIV0, HF_DIV1, HF_DIV2,  HF_DIV3,  HF_DIV4,  HF_DIV5,  HF_DIV6,  HF_DIV7,
        HF_DIV8, HF_DIV9, HF_DIV10, HF_DIV11, HF_DIV12, HF_DIV13, HF_DIV14, HF_DIV15,
    },
    /* 按键按下的最大变化量 */
    {
        //低频设置阀值：//(***)
        LF_MAX0, LF_MAX1, LF_MAX2,  LF_MAX3,  LF_MAX4,  LF_MAX5,  LF_MAX6,  LF_MAX7,
        LF_MAX8, LF_MAX9, LF_MAX10, LF_MAX11, LF_MAX12, LF_MAX13, LF_MAX14, LF_MAX15,

        // 高频设置阀，一般可跟高频一样即可 //(***)
        HF_MAX0, HF_MAX1, HF_MAX2,  HF_MAX3,  HF_MAX4,  HF_MAX5,  HF_MAX6,  HF_MAX7,
        HF_MAX8, HF_MAX9, HF_MAX10, HF_MAX11, HF_MAX12, HF_MAX13, HF_MAX14, HF_MAX15,
    },
    /* 原始纹波值 */    //(***)
    {
        // 低频原始纹波值：//(***)
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,

        // 高频原始纹波值 //(***)
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    },

    FREQ_923K,              // 低频率
    TK_VREF_SEL_1V + TK_CMP_Delay_64SCK +  TK_OP_SEL_4V + TK_Dischg_640SCK, // 低频触摸电压 要求: VREF < VOP < VDD
    FREQ_2M,                // 高频率
    TK_VREF_SEL_2V + TK_CMP_Delay_64SCK +  TK_OP_SEL_4V + TK_Dischg_640SCK, // 高频触摸电压 要求: VREF < VOP < VDD
    2,                      // 按键确认次数 去抖时间=扫描周期*2(高低双频)*2次    （不可以写0） //(***)
    20,                     // 原始数据去抖
    100,                    // 原始数据去抖上限次数
    1,                      // 键值输出条件更严格（0：单频，1：双频）

    /*--------------------------------------------------------------*/
    /*		以下部分仅限中颖原厂调试使用							*/
    /*	note ：禁止修改！！！！										*/
    /*--------------------------------------------------------------*/
    1,                      // 补丁1允许位                   (填1支持,填0不支持)
    5,                      // 补丁1参数：有5个按键忽大/忽小表示把板子从桌子上举起来再放下去
    1,                      // 补丁2允许位：手直接摸一个键然后才通5V补丁 (填1支持,填0不支持)
    HAND_TK_ENABLE,         // 补丁3允许位: 支持手直接摸弹簧 (填1支持,填0不支持)
    2,                      // CSI异常
    0xA1,                   // 补丁4
    0,                      // 放电时间（通道间隔时间）
};

/* 人手参数 */
// 补丁3参数: 手直接触摸的参数
// TK_Para.patch3为1时本表格有效
#define TK_HAND_K   (2.4)
U16 code TK_HandPara[MAX_KEYS*2]=
{
    //低频：//(***)
    LF_MAX0*TK_HAND_K,  LF_MAX1*TK_HAND_K,  LF_MAX2*TK_HAND_K,  LF_MAX3*TK_HAND_K,
    LF_MAX4*TK_HAND_K,  LF_MAX5*TK_HAND_K,  LF_MAX6*TK_HAND_K,  LF_MAX7*TK_HAND_K,
    LF_MAX8*TK_HAND_K,  LF_MAX9*TK_HAND_K,  LF_MAX10*TK_HAND_K, LF_MAX11*TK_HAND_K,
    LF_MAX12*TK_HAND_K, LF_MAX13*TK_HAND_K, LF_MAX14*TK_HAND_K, LF_MAX15*TK_HAND_K,

    // 高频 //(***)
    HF_MAX0*TK_HAND_K,  HF_MAX1*TK_HAND_K,  HF_MAX2*TK_HAND_K,  HF_MAX3*TK_HAND_K,
    HF_MAX4*TK_HAND_K,  HF_MAX5*TK_HAND_K,  HF_MAX6*TK_HAND_K,  HF_MAX7*TK_HAND_K,
    HF_MAX8*TK_HAND_K,  HF_MAX9*TK_HAND_K,  HF_MAX10*TK_HAND_K, HF_MAX11*TK_HAND_K,
    HF_MAX12*TK_HAND_K, HF_MAX13*TK_HAND_K, HF_MAX14*TK_HAND_K, HF_MAX15*TK_HAND_K,
};

/**
 * @brief  触摸初始化
 *
 */
// 触摸引脚初始化
#define     KEY1_OUT_L      {P1_0 = 0; P1CR  |=  b0;}
#define     KEY2_OUT_L      {P1_1 = 0; P1CR  |=  b1;}
#define     KEY3_OUT_L      {P1_2 = 0; P1CR  |=  b2;}
#define     KEY4_OUT_L      {P1_3 = 0; P1CR  |=  b3;}
#define     KEY5_OUT_L      {P1_4 = 0; P1CR  |=  b4;}
#define     KEY6_OUT_L      {P1_5 = 0; P1CR  |=  b5;}
#define     KEY7_OUT_L      {P1_6 = 0; P1CR  |=  b6;}
#define     KEY8_OUT_L      {P1_7 = 0; P1CR  |=  b7;}
#define     KEY9_OUT_L      {P0_0 = 0; P0CR  |=  b0;}
#define     KEY10_OUT_L     {P0_1 = 0; P0CR  |=  b1;}
#define     KEY11_OUT_L     {P0_2 = 0; P0CR  |=  b2;}
#define     KEY12_OUT_L     {P0_3 = 0; P0CR  |=  b3;}
#define     KEY13_OUT_L     {P0_4 = 0; P0CR  |=  b4;}
#define     KEY14_OUT_L     {P0_5 = 0; P0CR  |=  b5;}
#define     KEY15_OUT_L     {P0_6 = 0; P0CR  |=  b6;}
#define     KEY16_OUT_L     {INSCON |= 0x40; P5_0 = 0;P5CR |=  b0; INSCON &= 0xBF; }
#define     KEY17_OUT_L     {INSCON |= 0x40; P5_1 = 0;P5CR |=  b1; INSCON &= 0xBF; }
#define     KEY18_OUT_L     {INSCON |= 0x40; P5_2 = 0;P5CR |=  b2; INSCON &= 0xBF; }
#define     KEY19_OUT_L     {INSCON |= 0x40; P5_3 = 0;P5CR |=  b3; INSCON &= 0xBF; }
#define     KEY20_OUT_L     {P4_5 = 0; P4CR  |=  b5;}
#define     KEY21_OUT_L     {P4_4 = 0; P4CR  |=  b4;}
#define     KEY22_OUT_L     {P4_3 = 0; P4CR  |=  b3;}
#define     KEY23_OUT_L     {P4_2 = 0; P4CR  |=  b2;}
#define     KEY24_OUT_L     {P4_1 = 0; P4CR  |=  b1;}
#define     CX_OUT_L        {P0_7 = 0; P0CR   |=  b7;}
void InitKey(void)
{
    unsigned char i;

    // 触摸IO设置输出低电平
    for (i=0; i<TK_Para.num; i++)
    {
        switch (TK_Para.chn[i])
        {
            case 0:  KEY1_OUT_L   break;     //TK1--P1.0
            case 1:  KEY2_OUT_L   break;     //TK2--P1.1
            case 2:  KEY3_OUT_L   break;     //TK3--P1.2
            case 3:  KEY4_OUT_L   break;     //TK4--P1.3
            case 4:  KEY5_OUT_L   break;     //TK5--P1.4
            case 5:  KEY6_OUT_L   break;     //TK6--P1.5
            case 6:  KEY7_OUT_L   break;     //TK7--P1.6
            case 7:  KEY8_OUT_L   break;     //TK8--P1.7
//          case 8:  KEY9_OUT_L   break;     //TK9 --P0.0
//          case 9:  KEY10_OUT_L  break;     //TK10--P0.1
//          case 10: KEY11_OUT_L  break;     //TK11--P0.2
//			case 11: KEY12_OUT_L  break;     //TK12--P0.3
//			case 12: KEY13_OUT_L  break;     //TK13--P0.4
//			case 13: KEY14_OUT_L  break;     //TK14--P0.5
//          case 14: KEY15_OUT_L  break;     //TK15--P0.6
            case 15: KEY16_OUT_L  break;     //TK16--P5.0
            case 16: KEY17_OUT_L  break;     //TK17--P5.1
            case 17: KEY18_OUT_L  break;     //TK18--P5.2
//			case 18: KEY19_OUT_L  break;     //TK19--P5.3
//			case 19: KEY20_OUT_L  break;     //TK20--P4.5
            case 20: KEY21_OUT_L  break;     //TK21--P4.4
            case 21: KEY22_OUT_L  break;     //TK22--P4.3
            case 22: KEY23_OUT_L  break;     //TK23--P4.2
            case 23: KEY24_OUT_L  break;     //TK24--P4.1
            default: break;
        }
    }
    // C管脚设置输出低电平
    CX_OUT_L

//隐藏寄存器配置，隐藏邋IO配置输出低电平

#if (PACKAGE == SH79F9463A_32PIN) 
#pragma message "芯片封装:SH79F9463A----32PIN"
    P0  &= (~(b0+b1+b2+b3+b4+b5+b6));
    P0CR |=  (b0+b1+b2+b3+b4+b5+b6);

    P2  &= (~(b3+b4+b5));
    P2CR |=  (b3+b4+b5);

    P4  &= (~(b5+b6+b7));
    P4CR |=  (b5+b6+b7);

    INSCON |= 0x40;
    P5  &= (~(b3+b4+b5+b6+b7));
    P5CR |=  (b3+b4+b5+b6+b7);
    INSCON &= 0xBF;
#endif

#if (PACKAGE == SH79F9463A_28PIN)
#pragma message "芯片封装:SH79F9463A----28PIN"
    P0  &= (~(b0+b1+b2+b3+b4+b5+b6+b7));
    P0CR |=  (b0+b1+b2+b3+b4+b5+b6+b7);

    P2  &= (~(b1+b2+b3+b4+b5));
    P2CR |=  (b1+b2+b3+b4+b5);

    P4  &= (~(b4+b5+b6+b7));
    P4CR |=  (b4+b5+b6+b7);

    INSCON |= 0x40;
    P5  &= (~(b2+b3+b4+b5+b6+b7));
    P5CR |=  (b2+b3+b4+b5+b6+b7);
    INSCON &= 0xBF;
#endif


    // 放大系数和变化量初始化
    memcpy(LF_DIV, TK_Para.LFDiv, sizeof(LF_DIV));
    memcpy(HF_DIV, TK_Para.HFDiv, sizeof(HF_DIV));
    memcpy(Tab_TKMax, TK_Para.TKMax, sizeof(Tab_TKMax));
    tk_base_update_config(16, 1, 0);

    // 触摸复位
    reset_TK();

#ifdef TEST_TK_DATA
    tk_debug_uart_init(); // 触摸调试，初始化串口调试
#endif
}

/**
 * @brief  获取触摸库的键值并做有效性判断
 *
 * @return U16
 */
U16 Key_DataInput(void)
{
    U16 val;

    gBKeyValidFg = 1;

    val = key_value;
    switch (val)
    {
        //支持的按键值
        case 0x0000:
            gBLatchErrFg = 0;     //等到按键释放后清除无效键标志
        case 0x0001:
        case 0x0002:
        case 0x0004:
        case 0x0008:
        case 0x0010:
        case 0x0020:
        case 0x0040:
        case 0x0080:
        case 0x0100:
        case 0x0200:
        case 0x0400:
        case 0x0800:
        case 0x1000:
        case 0x2000:
        case 0x4000:
        case 0x8000:

        // TBD: 支持的组合值
        //case 0x0002|0x0010:     // TK1+TK2组合键
		case 0x0004|0x0002:
		case 0x0004|0x0001:
		case 0x0004|0x0008:
			
		case 0x0008|0x0002:
		case 0x0008|0x0001:
			
		case 0x0002|0x0001:
			
		case 0x0004|0x0002|0x0001:
		case 0x0008|0x0002|0x0001:
		case 0x0008|0x0004|0x0002:
		case 0x0008|0x0004|0x0001:
		
		case 0x0004|0x0001|0x0008|0x0002:
            if ( gBLatchErrFg == 0 )
            {
                return val;
            }
            else
            {
                return 0xffff;
            }
        default:
            gBKeyValidFg = 0;
            gBLatchErrFg = 1;
            return 0xffff;
    }
}


/**
 * @brief  触摸防卡死
 *
 *         防卡测函数，一般检测到非法键，或长按36S，复位一次触摸数据。 客户可根据需要是否打开
 *
 * @return U16
 */
bit Two_one = 0;
bit no_allow=0 ;
U16 catchKey(void)
{
    U8  i,j,k;
    U16 KeyRead;

    //-----------------------------------------------------------
    KeyRead =  Key_DataInput();

    //-----------------------------------------------------------
    if (gBKeyValidFg == 0)//非法键复位
    {
        gWRestT = 0;
        if ( gBTouchReason == 0 )
        {
            if ((++gWFailT)>=(1800/TK_SCAN_PERIOD))   //1.8s
            {
                gWFailT = 0;
                reset_TK();
                KeyRead = 0;
            }
        }
        else
        {
            //手直接摸
            if ((++gWFailT)>=(10000/TK_SCAN_PERIOD) ) //10s
            {
                gWFailT = 0;
                reset_TK();
                KeyRead = 0;
            }
        }
    }
    else if (KeyRead!=0)  //Cancel long button press reset KeyRead
    {
        //长按36s复位
        gWFailT = 0;

        if ((++gWRestT)>=(36000/TK_SCAN_PERIOD)) //36s
        {
            gWRestT = 0;
            reset_TK();
            KeyRead = 0;
        }
    }
    else
    {
        //无键
        gWRestT = 0;
        gWFailT = 0;
    }

    //-----------------------------------------------------------
    // 判断输出键值个数
    for (i=0,j=1,k=0;i<TK_Para.num;i++,j+=j)
    {
        if ( KeyRead & j)
        {
            k++;
        }
    }
    //判断双键情况，当有组合键时，需要等按键都释放，才能再次输出有效键值
    no_allow = 0;
    if (k > 1)              // 双键正常输出，
    {
        Two_one = 1;
    }
    else if (k==1)          // 单键情况
    {
        if (Two_one == 1)   // 从双键变成单键，输出无效键
        {
            no_allow = 1;
        }
    }
    else
    {
        Two_one = 0;        // 手离开，清0 双键变单键标志
    }
    // 多键还没有释放，不输出键值
    if (no_allow)
    {
        KeyRead = 0;
    }

    //-----------------------------------------------------------
    return KeyRead;
}


/**
 * @brief  触摸扫描和处理函数
 *
 *         建议主循环一直调用
 */
bit gbFlag1ms_ScanTK;
void ScanTKPress(void)
{
    static U8 idata counterTKPeriod=0;

    if(F_GetTimeTick(C_TIME_10ms))                 // gbFlag1ms_ScanTK每1ms 置1
    {
//        if ((++counterTKPeriod) >= (TK_SCAN_PERIOD)/8) // 周期处理
//        {
            counterTKPeriod = 0;
            stop_scan_tk();				            //一个周期调用一次，重新启动TK扫描
            #ifdef	TEST_TK_DATA
                tk_debug_uart_txd();		        //串口发送数据
            #endif
            gWrCurKey = catchKey();                 //获取键值，非法键判断
//        }
    }

	//主循环一直调用该函数。 一个触摸扫描周期保证调用SanTk()次数大于 触摸按键数
    SanTk();	//触摸后台程序
	SanTk();	//触摸后台程序
	SanTk();	//触摸后台程序
	SanTk();	//触摸后台程序
    //	一个周期多出的触摸按键数 SanTk调用，底层会直接返回，不做其它处理
}



/**
 * @brief  触摸中断服务程序
 *
 */
void TK_Interrupt (void) interrupt  1
{
    TK_Isr ();
}

