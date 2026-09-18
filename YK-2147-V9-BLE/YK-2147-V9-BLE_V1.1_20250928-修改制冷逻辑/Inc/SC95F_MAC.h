/*********************************************************
文 件 名 : SC95F_MAC.H
文件说明 : SC95F系列底层操作宏头文件
编写作者 : thm
编写时间 : 2020-12-5
版 本 号 : V1.0
==========================================================*/
        /*本文件用 SC95F系列 MCU 不建议修改*/
    
#ifndef _SC95F_MAC_H_
#define _SC95F_MAC_H_ 

//typedef bit     bool;
typedef unsigned char   Byte;
typedef unsigned int    Word;
typedef unsigned long   Dword;

#define     u8      unsigned char 
#define     u16     unsigned int 
#define     uchar   unsigned char 
#define     uint    unsigned int 
#define     ulong   unsigned long int
#define	    uint32	unsigned double int
#define     ext     extern
#define     Disable     0
#define     Enable      1
#define     NO          0
#define     YES         1

/*************************************************
*      移位及高低字节有效宏定义
*************************************************/
#define    IoOut(IO_PIN,ADDRESS,BIT)			(IO_PIN = _BTS(ADDRESS,BIT)? 1:0)

#define    _MAKEWORD(v1,v2)     (((Word)(v1)<<8) + (Word)(v2))
#define    _HI8BIT(x)           ((x>>8)&0xFF)            //高8位有效
#define    _LO8BIT(x)           (x&0xFF)                 //低8位有效
#define    _RL(nVal, nPos)      (nVal<<nPos)             //循环左移
#define    _RR(nVal, nPos)      (nVal>>nPos)             //循环右移

#define    _BSR(RAMBIT)         ((_HI8BIT(RAMBIT)) |= (1<<(_LO8BIT(RAMBIT))))
#define    _BS(ADDRESS,BIT)     ((ADDRESS) |=  (1<<(BIT)))
#define    _BC(ADDRESS,BIT)     ((ADDRESS) &= ~(1<<(BIT)))
#define    _BTC(ADDRESS,BIT)    (((ADDRESS)&   (1<<(BIT)))==0)
#define    _BTS(ADDRESS,BIT)    ((ADDRESS) &   (1<<(BIT)))
#define    _BN(ADDRESS,BIT)     ((ADDRESS) ^=  (1<<(BIT)))
#define    _BCP(DST,BITD,SUR,BITS)\
{\
    if(_BTS(SUR,BITS))\
    {_BS(DST,BITD);}\
    else\
    {_BC(DST,BITD);}\
}
#define    __BIN(A,B,C,D,E,F,G,H) (A<<7|B<<6|C<<5|D<<4|E<<3|F<<2|G<<1|H<<0)
    //二进制宏定义
#define LongToBin(n) \
(                    \
((n >> 21) & 0x80) | \
((n >> 18) & 0x40) | \
((n >> 15) & 0x20) | \
((n >> 12) & 0x10) | \
((n >>  9) & 0x08) | \
((n >>  6) & 0x04) | \
((n >>  3) & 0x02) | \
((n      ) & 0x01)   \
)
#define _BIN(n) LongToBin(0x##n##l) //write binary charactor set,exsample : Bin(11111111) = 0xff

//*****************************************************
            //SC95F系列专用宏
#define     _NOP              _nop_();  
#define     _DI               EA = Disable; 
#define     _EI               EA = Enable; 

//看门狗
#define     _WDTC_500MS       WDTCON = 0x10;     //清WDTC，500ms
#define     _WDTC_250MS       WDTCON = 0x11;     //清WDTC，250ms
#define     _WDTC_125MS       WDTCON = 0x12;     //清WDTC，125ms
#define     _WDTC_62MS        WDTCON = 0x13;     //清WDTC，62.5ms
#define     _WDTC_31MS        WDTCON = 0x14;     //清WDTC，31.5ms
#define     _WDTC_15MS        WDTCON = 0x15;     //清WDTC，15.75ms
#define     _WDTC_8MS         WDTCON = 0x16;     //清WDTC，7.88ms
#define     _WDTC_4MS         WDTCON = 0x17;     //清WDTC，3.94ms

//定时器0
#define     _T0_FREQ_SYS              _BS(TMCON,0);                  //定时器0时钟采用系统时钟
#define     _T0_FREQ_SYS_DIV12        _BC(TMCON,0);     			 //定时器0时钟采用系统时钟12分频
#define     _T0_SET_TIMER			  _BC(TMOD,2);		             //T0设为定时器
#define     _T0_SET_COUNTER		      _BS(TMOD,2);		             //T0设为计数器
#define     _T0_TM_MODE13			  {_BC(TMOD,1);_BC(TMOD,0);}     //13位定时器/计数器
#define     _T0_TM_MODE16			  {_BC(TMOD,1);_BS(TMOD,0);}     //16位定时器/计数器
#define     _T0_TM_MODE08			  {_BS(TMOD,1);_BC(TMOD,0);}     //8位自动重载定时器
#define     _T0_CNT_ENABLE			  TR0 = 1;                       //允许计数
#define     _T0_CNT_DISABLE			  TR0 = 0;                       //停止计数
#define     _T0_INT_ENABLE			  ET0 = 1;                       //允许中断
#define     _T0_INT_DISABLE			  ET0 = 0;                       //禁止中断
#define     _T0_INT_PRIORITY_HIGH	  _BS(IP,1);                     //中断优先级为高
#define     _T0_INT_PRIORITY_LOW	  _BC(IP,1);                     //中断优先级为低

//定时器1
#define     _T1_FREQ_SYS              _BS(TMCON,1);                  //定时器1时钟采用系统时钟
#define     _T1_FREQ_SYS_DIV12        _BC(TMCON,1);                  //定时器1时钟采用系统时钟12分频
#define     _T1_SET_TIMER			  _BC(TMOD,6);		             //T0设为定时器
#define     _T1_SET_COUNTER			  _BS(TMOD,6);		             //T0设为计数器
#define     _T1_TM_MODE13			  {_BC(TMOD,5);_BC(TMOD,4);}     //13位定时器/计数器
#define     _T1_TM_MODE16			  {_BC(TMOD,5);_BS(TMOD,4);}     //16位定时器/计数器
#define     _T1_TM_MODE08			  {_BS(TMOD,5);_BC(TMOD,4);}     //8位自动重载定时器
#define     _T1_CNT_ENABLE			  TR1 = 1;                       //允许计数
#define     _T1_CNT_DISABLE			  TR1 = 0;                       //停止计数
#define     _T1_INT_ENABLE			  ET1 = 1;          			 //允许中断
#define     _T1_INT_DISABLE			  ET1 = 0;                       //禁止中断
#define     _T1_INT_PRIORITY_HIGH	  _BS(IP,3);                     //中断优先级为高
#define     _T1_INT_PRIORITY_LOW	  _BC(IP,3);                     //中断优先级为低

//定时器2
#define     _TIMER_POINT_T2			{_BC(TXINX,2);_BS(TXINX,1);_BC(TXINX,0);}            //定时器寄存器指针指向T2
#define     _T2_FREQ_SYS            _BS(TXMOD,7);                                        //定时器2时钟采用系统时钟
#define     _T2_FREQ_SYS_DIV12      _BC(TXMOD,7);                                        //定时器2时钟采用系统时钟12分频
#define     _T2_SET_IN              _BC(TXMOD,1);                                        //定时器2作为时钟输入或IO口
#define     _T2_SET_OUT             _BS(TXMOD,1);                                        //定时器2作为时钟输出
#define     _T2_DC_DISABLE          _BC(TXMOD,0);                                        //定时器2禁止向下计数
#define     _T2_DC_ENABLE           _BS(TXMOD,0);                                        //定时器2允许向下计数
#define     _T2_INT_ENABLE			ET2 = 1;          						             //允许中断
#define     _T2_INT_DISABLE			ET2 = 0;                                             //禁止中断
#define     _T2_INT_PRIORITY_HIGH	_BS(IP,5);                    						 //中断优先级为高
#define     _T2_INT_PRIORITY_LOW	_BC(IP,5);                     						 //中断优先级为低
#define     _T2_CLR_OVERFLOW        _BC(TXCON,7);                     					 //清溢出标志
#define     _T2_CLR_EXFALL          _BC(TXCON,6);                     					 //清外部下降沿标志
#define     _T2_MAKE_RX_BAUDRATE    _BS(TXCON,5);                     					 //定时器2产生UART0接收波特率
#define     _T1_MAKE_RX_BAUDRATE    _BC(TXCON,5);                     					 //定时器1产生UART0接收波特率
#define     _T2_MAKE_TX_BAUDRATE    _BS(TXCON,4);                     					 //定时器2产生UART0发送波特率
#define     _T1_MAKE_TX_BAUDRATE    _BC(TXCON,4);                     					 //定时器1产生UART0发送波特率
#define     _T2_EX_DISABLE          _BC(TXCON,3);                     					 //忽略引脚外部事件
#define     _T2_EX_ENABLE           _BS(TXCON,3);                     					 //定时器2不作为UART0时钟时，每检测到一个下降沿，产生一个捕获或重载
#define     _T2_CNT_DISABLE         _BC(TXCON,2);                     					 //停止计时
#define     _T2_CNT_ENABLE          _BS(TXCON,2);                                        //开始计时
#define     _T2_SET_TIMER           _BC(TXCON,1);                     					 //定时模式，T2引脚作普通IO口
#define     _T2_SET_COUNTER         _BS(TXCON,1);                                        //计数模式
#define     _T2_SET_TC              _BC(TXCON,0);                     					 //16位重载计时/计数器
#define     _T2_SET_TEST            _BS(TXCON,0);                                        //16位重载捕获器，TXEX为定时器2外部捕获信号输入口

//定时器3（16位自动重载）
#define     _TIMER_POINT_T3			{_BC(TXINX,2);_BS(TXINX,1);_BS(TXINX,0);}            //定时器寄存器指针指向T3
#define     _T3_FREQ_SYS            _BS(TXMOD,7);                                        //定时器2时钟采用系统时钟
#define     _T3_FREQ_SYS_DIV12      _BC(TXMOD,7);                                        //定时器2时钟采用系统时钟12分频
#define     _T3_CLR_OVERFLOW        _BC(TXCON,7);                     					 //清溢出标志
#define     _T3_CNT_DISABLE         _BC(TXCON,2);                     					 //停止计时
#define     _T3_CNT_ENABLE          _BS(TXCON,2);                                        //开始计时
#define     _T3_INT_ENABLE			ET3 = 1;          						             //允许中断
#define     _T3_INT_DISABLE			ET3 = 0;                                             //禁止中断
#define     _T3_INT_PRIORITY_HIGH	_BS(IP1,6);                    					     //中断优先级为高
#define     _T3_INT_PRIORITY_LOW	_BC(IP1,6);                     					 //中断优先级为低

//定时器4（16位自动重载）
#define     _TIMER_POINT_T4			{_BS(TXINX,2);_BC(TXINX,1);_BC(TXINX,0);}            //定时器寄存器指针指向T4
#define     _T4_FREQ_SYS            _BS(TXMOD,7);                                        //定时器2时钟采用系统时钟
#define     _T4_FREQ_SYS_DIV12      _BC(TXMOD,7);                                        //定时器2时钟采用系统时钟12分频
#define     _T4_CLR_OVERFLOW        _BC(TXCON,7);                     					 //清溢出标志
#define     _T4_CNT_DISABLE         _BC(TXCON,2);                     					 //停止计时
#define     _T4_CNT_ENABLE          _BS(TXCON,2);                                        //开始计时
#define     _T4_INT_ENABLE			ET4 = 1;          						             //允许中断
#define     _T4_INT_DISABLE			ET4 = 0;                                             //禁止中断
#define     _T4_INT_PRIORITY_HIGH	_BS(IP1,7);                    					     //中断优先级为高
#define     _T4_INT_PRIORITY_LOW	_BC(IP1,7);                     					 //中断优先级为低

//PWM(默认主时钟频率32M)
#define     _PWM_CLK_SYS        	{PWMCFG &= 0xCF;}                   //时钟32M
#define     _PWM_CLK_SYS_DIV2       {PWMCFG &= 0xCF; PWMCFG |= 0x10;}   //时钟32M/2
#define     _PWM_CLK_SYS_DIV4       {PWMCFG &= 0xCF; PWMCFG |= 0x20;}   //时钟32M/4
#define     _PWM_CLK_SYS_DIV8       {PWMCFG &= 0xCF; PWMCFG |= 0x30;}   //时钟32M/8
#define     _PWM_PRD_H4(Perod)      {PWMCFG = (PWMCFG & 0xF0) | (u8)(Perod / 256);}   //周期高4位，Perod数值应该是实际周期-1
#define     _PWM_PRD_L8(Perod)      {PWMCON = (u8)(Perod & 0x00FF);}                  //周期低8位，Perod数值应该是实际周期-1
#define     _PWM_MODE_INDEPENDENT   {_BC(PWMMOD,3);}          //独立模式
#define     _PWM_MODE_COMPREMENTARY   {_BS(PWMMOD,3);}        //互补模式
#define     _PWM_ALIGN_EDGE         {_BC(PWMMOD,2);}          //边沿对齐
#define     _PWM_ALIGN_CENTER       {_BS(PWMMOD,2);}          //中心对齐

//*****************************************************
#define     B7_ 7
#define     B6_ 6
#define     B5_ 5
#define     B4_ 4
#define     B3_ 3
#define     B2_ 2
#define     B1_ 1
#define     B0_ 0
        //共同配置IO定义
#define     _B0_BI   0<<0
#define     _B0_PP   1<<0
#define     _B0_IN   2<<0
#define     _B0_OP   3<<0 
            
#define     _B1_BI   0<<2
#define     _B1_PP   1<<2
#define     _B1_IN   2<<2
#define     _B1_OP   3<<2  
            
#define     _B2_BI   0<<4
#define     _B2_PP   1<<4
#define     _B2_IN   2<<4
#define     _B2_OP   3<<4  

#define     _B3_BI   0<<6
#define     _B3_PP   1<<6
#define     _B3_IN   2<<6
#define     _B3_OP   3<<6

#define     _B4_BI   0<<0
#define     _B4_PP   1<<0
#define     _B4_IN   2<<0
#define     _B4_OP   3<<0 
            
#define     _B5_BI   0<<2
#define     _B5_PP   1<<2
#define     _B5_IN   2<<2
#define     _B5_OP   3<<2  
            
#define     _B6_BI   0<<4
#define     _B6_PP   1<<4
#define     _B6_IN   2<<4
#define     _B6_OP   3<<4  

#define     _B7_BI   0<<6
#define     _B7_PP   1<<6
#define     _B7_IN   2<<6
#define     _B7_OP   3<<6
        //共同配置IO定义结束
        
#define     _B7 (1<<7)
#define     _B6 (1<<6)
#define     _B5 (1<<5)
#define     _B4 (1<<4)
#define     _B3 (1<<3)
#define     _B2 (1<<2)
#define     _B1 (1<<1)
#define     _B0 (1<<0)


       //独立IO引脚类型位定义
    /*
        pu------上拉输入
        pp------推挽输出
        in------高阻输入
    */

/*************************P0*******************************/
//P00状态操作定义
#define  Set_P00_pp           {_BS(P0CON,0);            }      //设置推挽输出模式
#define  Set_P00_in           {_BC(P0CON,0);_BC(P0PH,0);}      //设置高阻输入模式
#define  Set_P00_pu           {_BC(P0CON,0);_BS(P0PH,0);}      //设置上拉输入模式
#define  Test_P00             (P0      & _B0) 
//P00位操作
#define  Set_P00              {_BS(P0,0);}        
#define  Clr_P00              {_BC(P0,0);}         

//P01状态操作定义
#define  Set_P01_pp           {_BS(P0CON,1);            }      //设置推挽输出模式
#define  Set_P01_in           {_BC(P0CON,1);_BC(P0PH,1);}      //设置高阻输入模式
#define  Set_P01_pu           {_BC(P0CON,1);_BS(P0PH,1);}      //设置上拉输入模式
#define  Test_P01             (P0      & _B1) 
//P01位操作
#define  Set_P01              {_BS(P0,1);}        
#define  Clr_P01              {_BC(P0,1);}   

//P02状态操作定义
#define  Set_P02_pp           {_BS(P0CON,2);            }      //设置推挽输出模式
#define  Set_P02_in           {_BC(P0CON,2);_BC(P0PH,2);}      //设置高阻输入模式
#define  Set_P02_pu           {_BC(P0CON,2);_BS(P0PH,2);}      //设置上拉输入模式
#define  Test_P02             (P0      & _B2) 
//P02位操作
#define  Set_P02              {_BS(P0,2);}        
#define  Clr_P02              {_BC(P0,2);} 

//P03状态操作定义
#define  Set_P03_pp           {_BS(P0CON,3);            }      //设置推挽输出模式
#define  Set_P03_in           {_BC(P0CON,3);_BC(P0PH,3);}      //设置高阻输入模式
#define  Set_P03_pu           {_BC(P0CON,3);_BS(P0PH,3);}      //设置上拉输入模式
#define  Test_P03             (P0      & _B3) 
//P03位操作
#define  Set_P03              {_BS(P0,3);}        
#define  Clr_P03              {_BC(P0,3);} 

//P04状态操作定义
#define  Set_P04_pp           {_BS(P0CON,4);            }      //设置推挽输出模式
#define  Set_P04_in           {_BC(P0CON,4);_BC(P0PH,4);}      //设置高阻输入模式
#define  Set_P04_pu           {_BC(P0CON,4);_BS(P0PH,4);}      //设置上拉输入模式
#define  Test_P04             (P0      & _B4) 
//P04位操作
#define  Set_P04              {_BS(P0,4);}        
#define  Clr_P04              {_BC(P0,4);} 

//P05状态操作定义
#define  Set_P05_pp           {_BS(P0CON,5);            }      //设置推挽输出模式
#define  Set_P05_in           {_BC(P0CON,5);_BC(P0PH,5);}      //设置高阻输入模式
#define  Set_P05_pu           {_BC(P0CON,5);_BS(P0PH,5);}      //设置上拉输入模式
#define  Test_P05             (P0      & _B5) 
//P05位操作
#define  Set_P05              {_BS(P0,5);}        
#define  Clr_P05              {_BC(P0,5);} 

//P06状态操作定义
#define  Set_P06_pp           {_BS(P0CON,6);            }      //设置推挽输出模式
#define  Set_P06_in           {_BC(P0CON,6);_BC(P0PH,6);}      //设置高阻输入模式
#define  Set_P06_pu           {_BC(P0CON,6);_BS(P0PH,6);}      //设置上拉输入模式
#define  Test_P06             (P0      & _B6) 
//P06位操作
#define  Set_P06              {_BS(P0,6);}        
#define  Clr_P06              {_BC(P0,6);}

//P07状态操作定义
#define  Set_P07_pp           {_BS(P0CON,7);            }      //设置推挽输出模式
#define  Set_P07_in           {_BC(P0CON,7);_BC(P0PH,7);}      //设置高阻输入模式
#define  Set_P07_pu           {_BC(P0CON,7);_BS(P0PH,7);}      //设置上拉输入模式
#define  Test_P07             (P0      & _B7) 
//P07位操作
#define  Set_P07              {_BS(P0,7);}        
#define  Clr_P07              {_BC(P0,7);}
 
/*************************P1*******************************/
//P10状态操作定义
#define  Set_P10_pp           {_BS(P1CON,0);            }      //设置推挽输出模式
#define  Set_P10_in           {_BC(P1CON,0);_BC(P1PH,0);}      //设置高阻输入模式
#define  Set_P10_pu           {_BC(P1CON,0);_BS(P1PH,0);}      //设置上拉输入模式
#define  Test_P10             (P1      & _B0) 
//P10位操作
#define  Set_P10              {_BS(P1,0);}        
#define  Clr_P10              {_BC(P1,0);}         

//P11状态操作定义
#define  Set_P11_pp           {_BS(P1CON,1);            }      //设置推挽输出模式
#define  Set_P11_in           {_BC(P1CON,1);_BC(P1PH,1);}      //设置高阻输入模式
#define  Set_P11_pu           {_BC(P1CON,1);_BS(P1PH,1);}      //设置上拉输入模式
#define  Test_P11             (P1      & _B1)
//P11位操作
#define  Set_P11              {_BS(P1,1);}        
#define  Clr_P11              {_BC(P1,1);}   

//P12状态操作定义
#define  Set_P12_pp           {_BS(P1CON,2);            }      //设置推挽输出模式
#define  Set_P12_in           {_BC(P1CON,2);_BC(P1PH,2);}      //设置高阻输入模式
#define  Set_P12_pu           {_BC(P1CON,2);_BS(P1PH,2);}      //设置上拉输入模式
#define  Test_P12             (P1      & _B2)
//P12位操作
#define  Set_P12              {_BS(P1,2);}        
#define  Clr_P12              {_BC(P1,2);} 

//P13状态操作定义
#define  Set_P13_pp           {_BS(P1CON,3);            }      //设置推挽输出模式
#define  Set_P13_in           {_BC(P1CON,3);_BC(P1PH,3);}      //设置高阻输入模式
#define  Set_P13_pu           {_BC(P1CON,3);_BS(P1PH,3);}      //设置上拉输入模式
#define  Test_P13             (P1      & _B3)
//P13位操作
#define  Set_P13              {_BS(P1,3);}        
#define  Clr_P13              {_BC(P1,3);} 

//P14状态操作定义
#define  Set_P14_pp           {_BS(P1CON,4);            }      //设置推挽输出模式
#define  Set_P14_in           {_BC(P1CON,4);_BC(P1PH,4);}      //设置高阻输入模式
#define  Set_P14_pu           {_BC(P1CON,4);_BS(P1PH,4);}      //设置上拉输入模式
#define  Test_P14             (P1      & _B4)
//P14位操作
#define  Set_P14              {_BS(P1,4);}        
#define  Clr_P14              {_BC(P1,4);} 

//P15状态操作定义
#define  Set_P15_pp           {_BS(P1CON,5);            }      //设置推挽输出模式
#define  Set_P15_in           {_BC(P1CON,5);_BC(P1PH,5);}      //设置高阻输入模式
#define  Set_P15_pu           {_BC(P1CON,5);_BS(P1PH,5);}      //设置上拉输入模式
#define  Test_P15             (P1      & _B5)
//P15位操作
#define  Set_P15              {_BS(P1,5);}        
#define  Clr_P15              {_BC(P1,5);} 

//P16状态操作定义
#define  Set_P16_pp           {_BS(P1CON,6);            }      //设置推挽输出模式
#define  Set_P16_in           {_BC(P1CON,6);_BC(P1PH,6);}      //设置高阻输入模式
#define  Set_P16_pu           {_BC(P1CON,6);_BS(P1PH,6);}      //设置上拉输入模式
#define  Test_P16             (P1      & _B6)
//P16位操作
#define  Set_P16              {_BS(P1,6);}        
#define  Clr_P16              {_BC(P1,6);}

//P17状态操作定义
#define  Set_P17_pp           {_BS(P1CON,7);            }      //设置推挽输出模式
#define  Set_P17_in           {_BC(P1CON,7);_BC(P1PH,7);}      //设置高阻输入模式
#define  Set_P17_pu           {_BC(P1CON,7);_BS(P1PH,7);}      //设置上拉输入模式
#define  Test_P17             (P1      & _B7)
//P17位操作
#define  Set_P17              {_BS(P1,7);}        
#define  Clr_P17              {_BC(P1,7);}

/*************************P2*******************************/
//P20状态操作定义
#define  Set_P20_pp           {_BS(P2CON,0);            }      //设置推挽输出模式
#define  Set_P20_in           {_BC(P2CON,0);_BC(P2PH,0);}      //设置高阻输入模式
#define  Set_P20_pu           {_BC(P2CON,0);_BS(P2PH,0);}      //设置上拉输入模式
#define  Test_P20             (P2      & _B0)
//P20位操作
#define  Set_P20              {_BS(P2,0);}        
#define  Clr_P20              {_BC(P2,0);}         

//P21状态操作定义
#define  Set_P21_pp           {_BS(P2CON,1);            }      //设置推挽输出模式
#define  Set_P21_in           {_BC(P2CON,1);_BC(P2PH,1);}      //设置高阻输入模式
#define  Set_P21_pu           {_BC(P2CON,1);_BS(P2PH,1);}      //设置上拉输入模式
#define  Test_P21             (P2      & _B1)
//P21位操作
#define  Set_P21              {_BS(P2,1);}        
#define  Clr_P21              {_BC(P2,1);}   

//P22状态操作定义
#define  Set_P22_pp           {_BS(P2CON,2);            }      //设置推挽输出模式
#define  Set_P22_in           {_BC(P2CON,2);_BC(P2PH,2);}      //设置高阻输入模式
#define  Set_P22_pu           {_BC(P2CON,2);_BS(P2PH,2);}      //设置上拉输入模式
#define  Test_P22             (P2      & _B2)
//P22位操作
#define  Set_P22              {_BS(P2,2);}        
#define  Clr_P22              {_BC(P2,2);} 

//P23状态操作定义
#define  Set_P23_pp           {_BS(P2CON,3);            }      //设置推挽输出模式
#define  Set_P23_in           {_BC(P2CON,3);_BC(P2PH,3);}      //设置高阻输入模式
#define  Set_P23_pu           {_BC(P2CON,3);_BS(P2PH,3);}      //设置上拉输入模式
#define  Test_P23             (P2      & _B3)
//P23位操作
#define  Set_P23              {_BS(P2,3);}        
#define  Clr_P23              {_BC(P2,3);} 

//P24状态操作定义
#define  Set_P24_pp           {_BS(P2CON,4);            }      //设置推挽输出模式
#define  Set_P24_in           {_BC(P2CON,4);_BC(P2PH,4);}      //设置高阻输入模式
#define  Set_P24_pu           {_BC(P2CON,4);_BS(P2PH,4);}      //设置上拉输入模式
#define  Test_P24             (P2      & _B4)
//P24位操作
#define  Set_P24              {_BS(P2,4);}        
#define  Clr_P24              {_BC(P2,4);} 

//P25状态操作定义
#define  Set_P25_pp           {_BS(P2CON,5);            }      //设置推挽输出模式
#define  Set_P25_in           {_BC(P2CON,5);_BC(P2PH,5);}      //设置高阻输入模式
#define  Set_P25_pu           {_BC(P2CON,5);_BS(P2PH,5);}      //设置上拉输入模式
#define  Test_P25             (P2      & _B5)
//P25位操作
#define  Set_P25              {_BS(P2,5);}        
#define  Clr_P25              {_BC(P2,5);} 

//P26状态操作定义
#define  Set_P26_pp           {_BS(P2CON,6);            }      //设置推挽输出模式
#define  Set_P26_in           {_BC(P2CON,6);_BC(P2PH,6);}      //设置高阻输入模式
#define  Set_P26_pu           {_BC(P2CON,6);_BS(P2PH,6);}      //设置上拉输入模式
#define  Test_P26             (P2      & _B6)
//P26位操作
#define  Set_P26              {_BS(P2,6);}        
#define  Clr_P26              {_BC(P2,6);}

//P27状态操作定义
#define  Set_P27_pp           {_BS(P2CON,7);            }      //设置推挽输出模式
#define  Set_P27_in           {_BC(P2CON,7);_BC(P2PH,7);}      //设置高阻输入模式
#define  Set_P27_pu           {_BC(P2CON,7);_BS(P2PH,7);}      //设置上拉输入模式
#define  Test_P27             (P2      & _B7)
//P27位操作
#define  Set_P27              {_BS(P2,7);}        
#define  Clr_P27              {_BC(P2,7);}

/*************************P3*******************************/
//P30状态操作定义
#define  Set_P30_pp           {_BS(P3CON,0);            }      //设置推挽输出模式
#define  Set_P30_in           {_BC(P3CON,0);_BC(P3PH,0);}      //设置高阻输入模式
#define  Set_P30_pu           {_BC(P3CON,0);_BS(P3PH,0);}      //设置上拉输入模式
#define  Test_P30             (P3      & _B0)
//P30位操作
#define  Set_P30              {_BS(P3,0);}        
#define  Clr_P30              {_BC(P3,0);} 

//P31状态操作定义
#define  Set_P31_pp           {_BS(P3CON,1);            }      //设置推挽输出模式
#define  Set_P31_in           {_BC(P3CON,1);_BC(P3PH,1);}      //设置高阻输入模式
#define  Set_P31_pu           {_BC(P3CON,1);_BS(P3PH,1);}      //设置上拉输入模式
#define  Test_P31             (P3      & _B1)
//P31位操作
#define  Set_P31              {_BS(P3,1);}        
#define  Clr_P31              {_BC(P3,1);} 

//P32状态操作定义
#define  Set_P32_pp           {_BS(P3CON,2);            }      //设置推挽输出模式
#define  Set_P32_in           {_BC(P3CON,2);_BC(P3PH,2);}      //设置高阻输入模式
#define  Set_P32_pu           {_BC(P3CON,2);_BS(P3PH,2);}      //设置上拉输入模式
#define  Test_P32             (P3      & _B2)
//P32位操作
#define  Set_P32              {_BS(P3,2);}        
#define  Clr_P32              {_BC(P3,2);} 

//P33状态操作定义
#define  Set_P33_pp           {_BS(P3CON,3);            }      //设置推挽输出模式
#define  Set_P33_in           {_BC(P3CON,3);_BC(P3PH,3);}      //设置高阻输入模式
#define  Set_P33_pu           {_BC(P3CON,3);_BS(P3PH,3);}      //设置上拉输入模式
#define  Test_P33             (P3      & _B3)
//P33位操作
#define  Set_P33              {_BS(P3,3);}        
#define  Clr_P33              {_BC(P3,3);} 

//P34状态操作定义
#define  Set_P34_pp           {_BS(P3CON,4);            }      //设置推挽输出模式
#define  Set_P34_in           {_BC(P3CON,4);_BC(P3PH,4);}      //设置高阻输入模式
#define  Set_P34_pu           {_BC(P3CON,4);_BS(P3PH,4);}      //设置上拉输入模式
#define  Test_P34             (P3      & _B4)
//P34位操作
#define  Set_P34              {_BS(P3,4);}        
#define  Clr_P34              {_BC(P3,4);} 

//P35状态操作定义
#define  Set_P35_pp           {_BS(P3CON,5);            }      //设置推挽输出模式
#define  Set_P35_in           {_BC(P3CON,5);_BC(P3PH,5);}      //设置高阻输入模式
#define  Set_P35_pu           {_BC(P3CON,5);_BS(P3PH,5);}      //设置上拉输入模式
#define  Test_P35             (P3      & _B5)
//P35位操作
#define  Set_P35              {_BS(P3,5);}        
#define  Clr_P35              {_BC(P3,5);} 

//P36状态操作定义
#define  Set_P36_pp           {_BS(P3CON,6);            }      //设置推挽输出模式
#define  Set_P36_in           {_BC(P3CON,6);_BC(P3PH,6);}      //设置高阻输入模式
#define  Set_P36_pu           {_BC(P3CON,6);_BS(P3PH,6);}      //设置上拉输入模式
#define  Test_P36             (P3      & _B6)
//P36位操作
#define  Set_P36              {_BS(P3,6);}        
#define  Clr_P36              {_BC(P3,6);} 

//P37状态操作定义
#define  Set_P37_pp           {_BS(P3CON,7);            }      //设置推挽输出模式
#define  Set_P37_in           {_BC(P3CON,7);_BC(P3PH,7);}      //设置高阻输入模式
#define  Set_P37_pu           {_BC(P3CON,7);_BS(P3PH,7);}      //设置上拉输入模式
#define  Test_P37             (P3      & _B7)
//P37位操作
#define  Set_P37              {_BS(P3,7);}        
#define  Clr_P37              {_BC(P3,7);} 

/*************************P4*******************************/
//P40状态操作定义
#define  Set_P40_pp           {_BS(P4CON,0);            }      //设置推挽输出模式
#define  Set_P40_in           {_BC(P4CON,0);_BC(P4PH,0);}      //设置高阻输入模式
#define  Set_P40_pu           {_BC(P4CON,0);_BS(P4PH,0);}      //设置上拉输入模式
#define  Test_P40             (P4      & _B0)
//P40位操作
#define  Set_P40              {_BS(P4,0);}        
#define  Clr_P40              {_BC(P4,0);} 

//P41状态操作定义
#define  Set_P41_pp           {_BS(P4CON,1);            }      //设置推挽输出模式
#define  Set_P41_in           {_BC(P4CON,1);_BC(P4PH,1);}      //设置高阻输入模式
#define  Set_P41_pu           {_BC(P4CON,1);_BS(P4PH,1);}      //设置上拉输入模式
#define  Test_P41             (P4      & _B1)
//P41位操作
#define  Set_P41              {_BS(P4,1);}        
#define  Clr_P41              {_BC(P4,1);} 

//P42状态操作定义
#define  Set_P42_pp           {_BS(P4CON,2);            }      //设置推挽输出模式
#define  Set_P42_in           {_BC(P4CON,2);_BC(P4PH,2);}      //设置高阻输入模式
#define  Set_P42_pu           {_BC(P4CON,2);_BS(P4PH,2);}      //设置上拉输入模式
#define  Test_P42             (P4      & _B2)
//P42位操作
#define  Set_P42              {_BS(P4,2);}        
#define  Clr_P42              {_BC(P4,2);} 

//P43状态操作定义
#define  Set_P43_pp           {_BS(P4CON,3);            }      //设置推挽输出模式
#define  Set_P43_in           {_BC(P4CON,3);_BC(P4PH,3);}      //设置高阻输入模式
#define  Set_P43_pu           {_BC(P4CON,3);_BS(P4PH,3);}      //设置上拉输入模式
#define  Test_P43             (P4      & _B3)
//P43位操作
#define  Set_P43              {_BS(P4,3);}        
#define  Clr_P43              {_BC(P4,3);} 

//P44状态操作定义
#define  Set_P44_pp           {_BS(P4CON,4);            }      //设置推挽输出模式
#define  Set_P44_in           {_BC(P4CON,4);_BC(P4PH,4);}      //设置高阻输入模式
#define  Set_P44_pu           {_BC(P4CON,4);_BS(P4PH,4);}      //设置上拉输入模式
#define  Test_P44             (P4      & _B4)
//P44位操作
#define  Set_P44              {_BS(P4,4);}        
#define  Clr_P44              {_BC(P4,4);} 

//P45状态操作定义
#define  Set_P45_pp           {_BS(P4CON,5);            }      //设置推挽输出模式
#define  Set_P45_in           {_BC(P4CON,5);_BC(P4PH,5);}      //设置高阻输入模式
#define  Set_P45_pu           {_BC(P4CON,5);_BS(P4PH,5);}      //设置上拉输入模式
#define  Test_P45             (P4      & _B5)
//P45位操作
#define  Set_P45              {_BS(P4,5);}        
#define  Clr_P45              {_BC(P4,5);} 

//P46状态操作定义
#define  Set_P46_pp           {_BS(P4CON,6);            }      //设置推挽输出模式
#define  Set_P46_in           {_BC(P4CON,6);_BC(P4PH,6);}      //设置高阻输入模式
#define  Set_P46_pu           {_BC(P4CON,6);_BS(P4PH,6);}      //设置上拉输入模式
#define  Test_P46             (P4      & _B6)
//P46位操作
#define  Set_P46              {_BS(P4,6);}        
#define  Clr_P46              {_BC(P4,6);} 

//P47状态操作定义
#define  Set_P47_pp           {_BS(P4CON,7);            }      //设置推挽输出模式
#define  Set_P47_in           {_BC(P4CON,7);_BC(P4PH,7);}      //设置高阻输入模式
#define  Set_P47_pu           {_BC(P4CON,7);_BS(P4PH,7);}      //设置上拉输入模式
#define  Test_P47             (P4      & _B7)
//P47位操作
#define  Set_P47              {_BS(P4,7);}        
#define  Clr_P47              {_BC(P4,7);} 

/*************************P5*******************************/
//P50状态操作定义
#define  Set_P50_pp           {_BS(P5CON,0);            }      //设置推挽输出模式
#define  Set_P50_in           {_BC(P5CON,0);_BC(P5PH,0);}      //设置高阻输入模式
#define  Set_P50_pu           {_BC(P5CON,0);_BS(P5PH,0);}      //设置上拉输入模式
#define  Test_P50             (P5      & _B0)
//P50位操作
#define  Set_P50              {_BS(P5,0);}        
#define  Clr_P50              {_BC(P5,0);}         

//P51状态操作定义
#define  Set_P51_pp           {_BS(P5CON,1);            }      //设置推挽输出模式
#define  Set_P51_in           {_BC(P5CON,1);_BC(P5PH,1);}      //设置高阻输入模式
#define  Set_P51_pu           {_BC(P5CON,1);_BS(P5PH,1);}      //设置上拉输入模式
#define  Test_P51             (P5      & _B1)
//P51位操作
#define  Set_P51              {_BS(P5,1);}        
#define  Clr_P51              {_BC(P5,1);} 

//P52状态操作定义
#define  Set_P52_pp           {_BS(P5CON,2);            }      //设置推挽输出模式
#define  Set_P52_in           {_BC(P5CON,2);_BC(P5PH,2);}      //设置高阻输入模式
#define  Set_P52_pu           {_BC(P5CON,2);_BS(P5PH,2);}      //设置上拉输入模式
#define  Test_P52             (P5      & _B2)
//P52位操作
#define  Set_P52              {_BS(P5,2);}        
#define  Clr_P52              {_BC(P5,2);} 

//P53状态操作定义
#define  Set_P53_pp           {_BS(P5CON,3);            }      //设置推挽输出模式
#define  Set_P53_in           {_BC(P5CON,3);_BC(P5PH,3);}      //设置高阻输入模式
#define  Set_P53_pu           {_BC(P5CON,3);_BS(P5PH,3);}      //设置上拉输入模式
#define  Test_P53             (P5      & _B3)
//P53位操作
#define  Set_P53              {_BS(P5,3);}        
#define  Clr_P53              {_BC(P5,3);} 

#endif