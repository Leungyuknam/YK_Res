/**************************************************************************
**************************************************************************/
#ifndef	_COMMUNICATE_H_
#define	_COMMUNICATE_H_

typedef union
{
	unsigned long all;
	unsigned char one[4];
}CommType;

typedef union
{
    unsigned char  all;
    struct
    {
       unsigned char b0:1;
       unsigned char b1:1;
       unsigned char b2:1;
       unsigned char b3:1;
       unsigned char b4:1;
       unsigned char b5:1;
	   unsigned char b6:1;
       unsigned char b7:1;
    }one;
}bits;

typedef struct
{
	unsigned char radix;		//低八位为小数
	unsigned char integer;		//高八位为整数
}TypeFloat;
//-------------------------------------------------------------------------
typedef union
{
	unsigned int all;
	TypeFloat flo;
}VolElecType;

#define	SetBit(x,y)			x |= (1<<y)			//将寄存器x的第y位置1
#define ClrBit(x,y)			x &= ~(1<<y)		//将寄存器x的第y位清0

#define TestOne(Ram,Bit)	(Ram & (1<<Bit))	//测试寄存器Ram的Bit位是否为1
#define TestZero(Ram,Bit)	!(Ram & (1<<Bit))	//测试寄存器Ram的Bit位是否为0

extern volatile unsigned char X_REG Status1_Work,Status2_Work; 

extern bits X_REG					Flag1;

#define	Flag1_Work					Flag1.all		//硬件故障标志1

#define B_Heat_En					Flag1.one.b0	//加热使能
#define B_Fan_En					Flag1.one.b1	//风扇使能
#define B_Buzz_En					Flag1.one.b2	//蜂鸣器使能
#define B_BuzzShort					Flag1.one.b3	//蜂鸣器短鸣1声（200ms）
#define B_BuzzLong					Flag1.one.b4	//蜂鸣器长鸣1声（400ms）
#define B_HeatStop_CheckPan			Flag1.one.b5	//不加热但检锅
#define B_HeatStop_NoCheck			Flag1.one.b6	//不加热不检锅
#define B_PPGDP_Dis					Flag1.one.b7	//PPG抖频禁止

extern volatile bits X_REG					Flag2;
#define	Flag2_Work					Flag2.all		
#define B_PanOver_Dis				Flag2.one.b0	//锅底NTC超温判断禁止
#define B_PanFail_Dis				Flag2.one.b1	//锅底NTC失效判断禁止	
#define B_PanTempReportEn			Flag2.one.b2	//锅底NTC温度快速上报



#define B_PPGDP_Can					Flag2.one.b7	//PPG抖频强制使能


extern volatile bits X_REG				ErrorF1;

#define	Flag1_Error			ErrorF1.all		//硬件故障标志1
#define B_EVolLow			ErrorF1.one.b0	//市电电压过低
#define B_EVolHigh			ErrorF1.one.b1	//市电电压过高
#define B_EIgbtOpen			ErrorF1.one.b2	//IGBT-NTC开路
#define B_EIgbtClose		ErrorF1.one.b3	//IGBT-NTC短路
#define B_EIgbtOver			ErrorF1.one.b4	//IGBT-NTC超温
#define B_EPanOpen			ErrorF1.one.b5	//锅底NTC开路
#define B_EPanClose			ErrorF1.one.b6	//锅底NTC短路
#define B_EPanOver			ErrorF1.one.b7	//锅底NTC超温


extern volatile bits X_REG				ErrorF2;

#define	Flag2_Error			ErrorF2.all		//硬件故障标志2
#define B_EPanFail			ErrorF2.one.b0	//锅底NTC失效
#define B_ETopOpen			ErrorF2.one.b1	//顶部NTC开路
#define B_ETopClose			ErrorF2.one.b2	//顶部NTC短路
#define B_ETopFail			ErrorF2.one.b3	//顶部NTC失效
#define B_Comm_Error		ErrorF2.one.b7	//通信故障

extern volatile  unsigned char X_REG AD_TmprIgbt;			//IGBT温度AD值
extern volatile  unsigned char X_REG AD_TmprPan;			//锅底温度AD值
extern volatile  unsigned char X_REG AD_TmprTop;			//顶部温度AD值
extern volatile  unsigned char X_REG PowerWork;				//工作功率值
//extern volatile  unsigned char X_REG SlopeCurr_Adjust;		//电流斜率调节值 


/**************************************************************************
--------------------------------- 函数声明 --------------------------------
**************************************************************************/ 
void Communicate();  						 
void Deal_CommData();					 
 

/**************************************************************************
--------------------------------- 引脚定义 --------------------------------
**************************************************************************/

#define Pin_CommIn				P12

#define Pin_CommOut				P13
#define C_TimeUS_Int			125//US
//参数名称：主循环时间（ms）	 
#define 	C_TimeMS_Main		5				 

/**************************************************************************
**************************************************************************/
#endif

