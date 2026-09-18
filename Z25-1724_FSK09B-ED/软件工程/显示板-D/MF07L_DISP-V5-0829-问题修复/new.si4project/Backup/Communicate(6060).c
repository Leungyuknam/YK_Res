/**************************************************************************
**************************************************************************/
#include "Include\Include.h"

	/*
	bit B_Pan_Error = 0;
	bit B_Pan_Have = 0;	
	bit B_Pan_Steel = 0;	
 	bit B_Pan_Iron = 0;	

 	bit B_Zero_60Hz = 0;

	
 	bit B_Protect_Stop = 0;	
 	bit B_PPGStop_CheckPan = 0;	
 	bit B_PPGStop_NoCheck = 0;
	*/




	volatile  VolElecType X_REG Vol;		
	
	
/**************************************************************************
--------------------------------- 变量定义 --------------------------------
**************************************************************************/
volatile unsigned char I_REG Buf_DataSend[4];
volatile unsigned char I_REG Buf_DataRece[4];

//volatile unsigned char X_REG Flag1_Work,Flag2_Work;
volatile unsigned char X_REG Status1_Work,Status2_Work; 
volatile unsigned char X_REG LastStatus1_Work,LastStatus2_Work,u8SameWorkCnt; 

//Status1_Work
//		if(B_Pan_Error)				SetBit(i,0); 	//检锅故障（未接线盘）	
//		if(B_Pan_No)				SetBit(i,1); 	//无锅
//		if(B_Pan_Have)				SetBit(i,2); 	//有锅	
//		if(B_Pan_Steel)				SetBit(i,3); 	//钢锅			
//		if(B_Pan_Iron)				SetBit(i,4); 	//铁锅
//		if(B_Zero_No)				SetBit(i,5); 	//无过零
//		if(B_Zero_60Hz)				SetBit(i,6); 	//过零频率 = 60Hz
//		if(B_Protect_BackPress)		SetBit(i,7); 	//过压保护	
volatile bit B_Deal_DataRece,B_Refrsh_DataSend;

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
 bits X_REG					Flag1;
/*
#define B_Heat_En					Flag1.one.b0	//加热使能
#define B_Fan_En					Flag1.one.b1	//风扇使能
#define B_Buzz_En					Flag1.one.b2	//蜂鸣器使能
#define B_BuzzShort					Flag1.one.b3	//蜂鸣器短鸣1声（200ms）
#define B_BuzzLong					Flag1.one.b4	//蜂鸣器长鸣1声（400ms）
#define B_HeatStop_CheckPan			Flag1.one.b5	//不加热但检锅
#define B_HeatStop_NoCheck			Flag1.one.b6	//不加热不检锅
#define B_PPGDP_Dis					Flag1.one.b7	//PPG抖频禁止
*/
volatile bits X_REG					Flag2;
volatile bits X_REG					ErrorF1;
volatile bits X_REG					ErrorF2;
//volatile bits X_REG			OutPutF;
//#define Flag_OutPut			OutPutF.all		//输出控制标志
//#define B_Fan_En			OutPutF.one.b0	//风扇使能
//#define B_Buzz_En			OutPutF.one.b1	//蜂鸣器使能
//#define B_Buzz_Out			OutPutF.one.b2	//蜂鸣器输出
//#define B_HeatStop_CheckPan	OutPutF.one.b4	//不加热但检锅				
//#define B_HeatStop_NoCheck	OutPutF.one.b5	//不加热不检锅

volatile unsigned char X_REG LastErrorF1,LastErrorF2,u8SameErrorCnt; 


volatile  unsigned char X_REG Status_PPGStop;		//PPG停止状态
//volatile  unsigned int  X_REG AD_Volatage;			//市电电压AD值
//volatile  unsigned int  X_REG AD_VolatageBuf;		//市电电压AD值缓存

//volatile  unsigned int  X_REG AD_Current;			//工作电流AD值
//volatile  unsigned int  X_REG AD_CurrentBuf;		//工作电流AD值缓存

//volatile  unsigned int  X_REG AD_Volatage;			//市电电压AD值
volatile  unsigned char X_REG AD_TmprIgbt;			//IGBT温度AD值
volatile  unsigned char X_REG AD_TmprPan;			//锅底温度AD值
//volatile  unsigned char X_REG AD_TmprTop;			//顶部温度AD值
//volatile  unsigned int  X_REG AD_VRef;				//内部基准电压AD值

//volatile  unsigned int  X_REG Delay_CheckPan;			//检锅延时时间
//volatile  unsigned int  X_REG Time_Syn;				//同步脉宽时间 
//volatile  unsigned char X_REG Count_Syn;				//同步次数 


//volatile  unsigned int  X_REG PowerReal;				//实际输出功率 
//volatile  unsigned int  X_REG PowerSet;				//目标输出功率 


//volatile  unsigned int  X_REG Buf_PPGTMR;				//PPG导通缓存值

//volatile  unsigned int  X_REG Count_BackPress;			//1级过压保护计数
//volatile  unsigned int  X_REG Count_IgbtProtect;		//IGBT硬件保护计数

//volatile  unsigned char X_REG Bak_Num_LxLowOn;


volatile  unsigned char X_REG PowerWork;			//工作功率值
//volatile  unsigned char X_REG SlopeCurr_Adjust;		//电流斜率调节值 

volatile  VolElecType X_REG Elec;					//电量计算结果

/**************************************************************************
* 函数名称：Communicate
* 函数功能：芯片通信
* 入口参数：无
* 出口参数：无 
* 备    注：中断调用
**************************************************************************/
void Communicate()
{
	//收码相关寄存器定义
	static unsigned char TimeRece_High,TimeRece_Low,CountRece;
	static CommType DataRece;
	static bit B_Rece_Old;
	
	//发码相关寄存器定义
	static unsigned char TimeSend,CountSend;
	static CommType DataSend;
	static bit B_Send_Start,B_Send_Bak; 		
	
    volatile unsigned char i;
						 
	/**********************************************************************
									收码	
	**********************************************************************/ 

	{			
		if(!Pin_CommIn)
		{
			TimeRece_Low++;							//低电平计时
			
			if(!B_Rece_Old)
			{
				TimeRece_High = 0;
			}
			else
			{										//下降沿接收数据
				B_Rece_Old = 0;						//收码电平旧值（低）
				
				if(1 == TimeRece_Low)
				{									//第一次下降沿
					CountRece = 32;
				}
				else
				{									//非第一次下降沿
					DataRece.all <<= 1;	
										
					if(TimeRece_Low >= TimeRece_High) 
					{								//低电平>高电平为1，否则为0
						DataRece.one[3] |= 0x01;
					}

					TimeRece_High = 0;
					TimeRece_Low = 1;
					
					if(0 == --CountRece)
					{
						B_Deal_DataRece = 1;		//收码数据处理
						
						Buf_DataRece[3] = DataRece.one[0];	  	 
						Buf_DataRece[2] = DataRece.one[1];			 
						Buf_DataRece[1] = DataRece.one[2];		
						Buf_DataRece[0] = DataRece.one[3];						
					}
				}
			}
		}
		else if(Pin_CommIn)
		{
			B_Rece_Old = 1;							//收码电平旧值（高）
			
			if(TimeRece_High >= 5000/C_TimeUS_Int)	//5ms
			{										//长时间高电平，重新收码
				TimeRece_Low = 0;					 
				CountRece = 32;
			}
			else
			{
				TimeRece_High++;					//高电平计时
			}
		}
	}
	
	/**********************************************************************
	发码	
	**********************************************************************/ 

	{
		if(B_Send_Start)
		{
			if(0 == --TimeSend)
			{										//已发完一位码
				Pin_CommOut = 0;					//输出低电平
			
				if(0 == CountSend)
				{								
 					B_Send_Start = 0;				//发码完成	
					TimeSend = 0;										
				}
				else
				{									//发送下一位数据
					CountSend--;
					TimeSend = 2000/C_TimeUS_Int;	//2ms
					
					if(DataSend.one[0] & 0x80)
					{
						B_Send_Bak = 1;				//发1
					}
					else
					{
						B_Send_Bak = 0;				//发0
					}	
								 		
					DataSend.all <<= 1;		
				}				
			}	
			else
			{										//还没有发完一位码
				if(B_Send_Bak)
				{
					i = 500/C_TimeUS_Int;			//0.5ms
				}
				else
				{
					i = 1500/C_TimeUS_Int;			//1.5ms
				}		
				
				if(TimeSend > i)
				{
					Pin_CommOut = 0;				//输出低电平						
				}
				else
				{
					Pin_CommOut = 1;				//输出高电平			
				}
			}
		}
		else
		{
			if(TimeSend >= 10000/C_TimeUS_Int)		//10ms
			{
				B_Send_Start = 1;					//发码开始
												
				TimeSend = 1;	
				CountSend = 32;						//发码数据位数：32bit
		
				DataSend.one[3] = Buf_DataSend[0];	  	 
				DataSend.one[2] = Buf_DataSend[1];			 
				DataSend.one[1] = Buf_DataSend[2];	
				DataSend.one[0] = Buf_DataSend[3];	
		
				B_Refrsh_DataSend = 1;				//发码数据刷新 							
			}
			else
			{
				TimeSend++;
			}
			
			if(TimeSend >= 500/C_TimeUS_Int)		//0.5ms		
			{			
				Pin_CommOut = 1;					//输出高电平			
			}		
			else		
			{
				Pin_CommOut = 0;					//输出低电平	
			}
		}
	}
}
 
/**************************************************************************
* 函数名称：Deal_CommData
* 函数功能：通信数据处理
* 入口参数：无
* 出口参数：无 
* 备    注：主循环调用
**************************************************************************/
void Deal_CommData()
{			
	static unsigned int  Time_NoComm;
	static unsigned char Num_CommSend;	
		
	volatile unsigned char i,j;
	
	/**********************************************************************
	发码数据处理	
	**********************************************************************/				
	if(B_Refrsh_DataSend)
	{												//发码数据刷新		
		/*
		i = 0;			
		if(B_Pan_Error)				SetBit(i,0); 	//检锅故障（未接线盘）	
		if(B_Pan_No)				SetBit(i,1); 	//无锅
		if(B_Pan_Have)				SetBit(i,2); 	//有锅	
		if(B_Pan_Steel)				SetBit(i,3); 	//钢锅			
		if(B_Pan_Iron)				SetBit(i,4); 	//铁锅
		if(B_Zero_No)				SetBit(i,5); 	//无过零
		if(B_Zero_60Hz)				SetBit(i,6); 	//过零频率 = 60Hz
		if(B_Protect_BackPress)		SetBit(i,7); 	//过压保护	
						
		Status1_Work = i;							//刷新工作状态1
		//-----------------------------------------------------------------	
		i = 0;					
		if(B_Protect_Stop)			SetBit(i,0); 	//保护停止加热
		if(B_PPGStop_CheckPan)		SetBit(i,1); 	//加热暂停但检锅	
		if(B_PPGStop_NoCheck)		SetBit(i,2); 	//加热暂停不检锅
		if(Status_PPGDP & 0x02)		SetBit(i,3); 	//抖频开启
		if(Status_LxLow & 0x02)		SetBit(i,4); 	//连续低功率开启		

		Status2_Work = i;							//刷新工作状态2		
		*/
		//-----------------------------------------------------------------	
		if((TRUE	 == B_BuzzShort) || (TRUE	 == B_BuzzLong)   )
			Num_CommSend = 	0;
		switch(Num_CommSend)
		{	
			case 0:		
				Buf_DataSend[3] = 0xB0;				//命令字			
				Buf_DataSend[2] = Flag1_Work;//Flag1_Work;		//工作标志1				
				Buf_DataSend[1] = Flag2_Work;		//工作标志2	
				B_BuzzShort = FALSE;
				B_BuzzLong = FALSE;
				B_Buzz_En = FALSE;
			break;	
			case 1:		
				Buf_DataSend[3] = 0xB1;				//命令字			
				Buf_DataSend[2] = PowerWork;		//加热功率值				
				Buf_DataSend[1] = 0x00;				//--	
			break;				
			case 2:
				Buf_DataSend[3] = 0xB2;				//命令字
				Buf_DataSend[2] = Num_LxLowOn;		//连续低功率开个数		
				Buf_DataSend[1] = Num_LxLowOff;		 //连续低功率关个数		
			break;			
			case 3:
				if(SysMode == SysMX_Adjust)
				{
					Buf_DataSend[3] = 0xD0;				//命令字
					Buf_DataSend[2] = Sys.ComSlopeCurr_Adjust;				//电流斜率调整数
					Buf_DataSend[1] = ~Sys.ComSlopeCurr_Adjust;				//电流斜率调整数反码
				}

			break;	
			
			default:

			break;	
		}
		i = (unsigned)Buf_DataSend[1] + Buf_DataSend[2] + Buf_DataSend[3];
		Buf_DataSend[0] = (unsigned)~i;				//校验和	
		//-----------------------------------------------------------------		
		B_Refrsh_DataSend = 0;						//发码数据刷新完成	
				
		if(++Num_CommSend >= 4)						//发码切换
		{							
			Num_CommSend = 0;	
		}							 
	}
	
	/**********************************************************************
	收码数据处理	
	**********************************************************************/
	if(B_Deal_DataRece)					
	{	
		B_Deal_DataRece = 0;						//收码数据处理完成 			
								
		i = (unsigned)Buf_DataRece[1] + Buf_DataRece[2] + Buf_DataRece[3];
		i = (unsigned)~i;
		j = (unsigned)Buf_DataRece[3] & 0xF0;
 
		if((i == Buf_DataRece[0]) && (0xA0 == j))	//数据校验
		{								
			B_Comm_Error = 0;						//清通信故障标志					
			Time_NoComm = 0;						//清无通信计时				
 				 
			if(0xA0 == Buf_DataRece[3])		
			{			
				if(LastStatus1_Work != Buf_DataRece[2] || LastStatus2_Work != Buf_DataRece[1])
				{
					LastStatus1_Work = Buf_DataRece[2];
					LastStatus2_Work = Buf_DataRece[1];
					u8SameWorkCnt = 0;
				}
				else
				{
					if(u8SameWorkCnt < 255)
						u8SameWorkCnt++;
					if(2 == u8SameWorkCnt )
					{
						Status1_Work = Buf_DataRece[2]; 	//工作状态1
						Status2_Work = Buf_DataRece[1]; 	//工作状态2 			
					}
				}
			}		
			else if(0xA1 == Buf_DataRece[3])		
			{	
				if(LastErrorF1 != Buf_DataRece[2] || LastErrorF2 != Buf_DataRece[1])
				{
					LastErrorF1 = Buf_DataRece[2];
					LastErrorF2 = Buf_DataRece[1];
					u8SameErrorCnt = 0;
				}
				else
				{
					if(u8SameErrorCnt < 255)
						u8SameErrorCnt++;
					if(2 == u8SameErrorCnt )
					{
						Flag1_Error = Buf_DataRece[2];		//硬件故障标志1	
						Flag2_Error = Buf_DataRece[1];		//硬件故障标志2				
					}
				}
			}	
			else if(0xA2 == Buf_DataRece[3])		
			{			
				Status_PPGStop = Buf_DataRece[2];				//保护状态	
				AD_TmprPan = Buf_DataRece[1];					//锅底温度AD
			}	
			else if(0xA4 == Buf_DataRece[3])		
			{	
				AD_TmprIgbt = Buf_DataRece[2];					//IGBT温度AD
				//AD_VolatageBuf = (Buf_DataRece[1] & 0xF0)<<4;	//市电电压AD（高4位）
				//AD_CurrentBuf = (Buf_DataRece[1] & 0x0f)<<8;	//工作电流AD（高4位）	
			}	
			else if(0xAC == Buf_DataRece[3])		
			{	
				i = (unsigned)~Buf_DataRece[1];
				if(Buf_DataRece[2] == i)
				{
					Sys.ShowEppSlope = Buf_DataRece[2];
					if(SysMode != SysMX_Adjust)
						Sys.ComSlopeCurr_Adjust = Buf_DataRece[2];
				}
			}	

			
			/*
			
			else if(0xA3 == Buf_DataRece[3])		
			{	
				AD_VolatageBuf &= 0x0f00;
				AD_VolatageBuf |= Buf_DataRece[2];				//市电电压AD（低8位）
				AD_Volatage = AD_VolatageBuf;

				AD_CurrentBuf &= 0x0f00;
				AD_CurrentBuf |= Buf_DataRece[1];				//工作电流AD（低8位）
				AD_Current = AD_CurrentBuf;
			}	
			else if(0xA4 == Buf_DataRece[3])		
			{	
				AD_TmprIgbt = Buf_DataRece[2];					//IGBT温度AD
				AD_VolatageBuf = (Buf_DataRece[1] & 0xF0)<<4;	//市电电压AD（高4位）
				AD_CurrentBuf = (Buf_DataRece[1] & 0x0f)<<8;	//工作电流AD（高4位）	
			}	
			else if(0xA5 == Buf_DataRece[3])		
			{	
				AD_TmprTop = Buf_DataRece[2];					//顶部温度AD
				AD_VRef = Buf_DataRece[1];						//内部基准电压（1.2V）AD	
			}	
			else if(0xA6 == Buf_DataRece[3])		
			{	
				Vol.flo.integer = Buf_DataRece[2];				//市电电压 - 80V
				Count_Syn = Buf_DataRece[1];					//检锅振荡个数	
			}	
			else if(0xA7 == Buf_DataRece[3])		
			{	
				Time_Syn = (u16)(Buf_DataRece[2]<<8) + Buf_DataRece[1];		//检锅振荡脉宽
			}
			else if(0xA8 == Buf_DataRece[3])		
			{	
				PowerReal = (u16)(Buf_DataRece[2]<<8) + Buf_DataRece[1];	//实际输出功率
			}	
			else if(0xA9 == Buf_DataRece[3])		
			{	
				Buf_PPGTMR = (u16)(Buf_DataRece[2]<<8) + Buf_DataRece[1];	//PPG导通值
			}	
			else if(0xAA == Buf_DataRece[3])		
			{	
				Count_BackPress = (u16)(Buf_DataRece[2]<<8) + Buf_DataRece[1];	//1级过压保护计数
			}	
			else if(0xAB == Buf_DataRece[3])		
			{	
				Count_IgbtProtect = (u16)(Buf_DataRece[2]<<8) + Buf_DataRece[1];	//IGBT硬件保护计数
			}	
			else if(0xAC == Buf_DataRece[3])		
			{	
				Elec.flo.integer = Buf_DataRece[2];					//电量高位
				Elec.flo.radix = Buf_DataRece[1];					//电量低位	
			}
			*/	
		}
		//-----------------------------------------------------------------									
	}
	B_EIgbtOver = FALSE; //使不存在E5 高温故障显示
	/**********************************************************************
	通信状态确认	
	**********************************************************************/		
												//已确认有通信
	if(++Time_NoComm > 2000/C_TimeMS_Main)		//2s
	{
		Time_NoComm = 0;
		
		B_Comm_Error = 1;						//通信故障	
		//Buzz_Short_One();						//蜂鸣器短鸣1声		
	}
	
}

/**************************************************************************
**************************************************************************/ 

			
