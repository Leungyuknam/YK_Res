#include "..\Include\Include.h"

//==========================================================================
void	F_RL_StepMotorSwing(unsigned int r_RL_Angle1Buf,unsigned int r_RL_Angle2Buf,unsigned char r_RL_SMSpeedBuf,unsigned int r_RL_Angle1Add,unsigned int r_RL_Angle2Add);
void	F_RL_StepMotorStop(void);//风门停止
void	F_RL_StepMotorFixed( unsigned int r_RL_AngleBuf, unsigned char r_RL_SMSpeedBuf );//以一定速度到达指定角度
void	F_RL_StepMotorControl(void);//水平摆风电机控制=
//==========================================================================
unsigned char xdata r_RL_SMOutFlag1=0;//电机端口输出标志===
unsigned char xdata r_RL_SMSpeedSet;//电机设定速度===
signed int xdata r_RL_SMAngleNow=0;//当前角度=
signed int xdata r_RL_SMAngleSet=0;//设定角度=

unsigned char xdata g_u8RL_SMRstState = Rst_No;   //复位状态==

#define		b_RL_SMABit		BIT7
#define		b_RL_SMBBit		BIT6	
#define		b_RL_SMCBit		BIT5
#define		b_RL_SMDBit		BIT4
#define		c_RL_SMA		b_RL_SMABit
#define		c_RL_SMAB		b_RL_SMABit|b_RL_SMBBit
#define		c_RL_SMB		b_RL_SMBBit
#define		c_RL_SMBC		b_RL_SMBBit|b_RL_SMCBit
#define		c_RL_SMC		b_RL_SMCBit
#define		c_RL_SMCD		b_RL_SMCBit|b_RL_SMDBit
#define		c_RL_SMD		b_RL_SMDBit
#define		c_RL_SMDA		b_RL_SMDBit|b_RL_SMABit	

//========四相8拍方式====A-AB-B-BC-C-CD-D-DA(顺时针)================
const unsigned char Table_RL_SM1[8] =
{
	c_RL_SMA,c_RL_SMAB,c_RL_SMB,c_RL_SMBC,c_RL_SMC,c_RL_SMCD,c_RL_SMD,c_RL_SMDA  
};
//========四相8拍方式====A-DA-D-CD-C-BC-B-AB(逆时针)================
const unsigned char Table_RL_SM2[8] =
{	
	c_RL_SMA,c_RL_SMDA,c_RL_SMD,c_RL_SMCD,c_RL_SMC,c_RL_SMBC,c_RL_SMB,c_RL_SMAB   
};

/******************************************************************
 函数名称：						void	F_StepMotorOutPut(void)
 功能描述：						步进电机端口输出
 输入参数：						无
 输出参数：						无
*******************************************************************/
void F_RL_StepMotorOutPut(void)//步进电机端口输出=====
{
	if( b_RL_SMOutA1 )
	{
		p_RL_SMA1_on();
	}
	else
	{
		p_RL_SMA1_off();
	}
	if( b_RL_SMOutB1 )
	{
		p_RL_SMB1_on();
	}
	else
	{
		p_RL_SMB1_off();
	}
	if( b_RL_SMOutC1 )
	{
		p_RL_SMC1_on();
	}
	else
	{
		p_RL_SMC1_off();
	}
	if( b_RL_SMOutD1 )
	{
		p_RL_SMD1_on();
	}
	else
	{
		p_RL_SMD1_off();
	}
}

/******************************************************************
 函数名称：						void	F_StepMotorDrive( unsigned char r_Way )
 功能描述：						步进电机驱动 r_Way:1--顺时针旋转  0--逆时针旋转
 调用本函数的时钟周期：			2ms
 输入参数：						r_Way
 输出参数：						无
*******************************************************************/
void F_RL_StepMotorDrive( unsigned char r_RL_Way )//步进电机驱动 r_Way:1--顺时针旋转  0--逆时针旋转
{
	static	unsigned char r_RL_SMStep=0;	//电机步数===
	static	unsigned char r_RL_SMSpeed=0;	//电机速度===
	static  unsigned char r_RL_SDelay = 0;	//切换延时

	if( ( r_RL_SMAngleNow == r_RL_SMAngleSet )|| ( r_RL_SMSpeedSet == 0 ) )//电机转到设定角度或者转速要求为0--停止电机
	{
		r_RL_SMOutFlag1=0;
        if(0 == r_RL_SDelay)
		{	r_RL_SDelay = 200;}
	}
	else
	{	
		if(r_RL_SDelay > 0)
		{	r_RL_SDelay--;}
		else
		{
			if( r_RL_SMSpeed > 1 )
			{
				r_RL_SMSpeed--;
			}
			else
			{
				r_RL_SMSpeed=r_RL_SMSpeedSet;	
				if( r_RL_Way )//调用顺时针表格
				{
					r_RL_SMOutFlag1=Table_RL_SM1[r_RL_SMStep];				
				}
				else//调用逆时针表格
				{
					r_RL_SMOutFlag1=Table_RL_SM2[r_RL_SMStep];
				}	
							
				if( r_RL_SMAngleNow < r_RL_SMAngleSet )//当前角度小于设定的角度==
				{
					r_RL_SMAngleNow++;
					r_RL_SMStep++;
					if( r_RL_SMStep >= 8 )
					{
						r_RL_SMStep=0;
					}	
				}
				else//当前角度大于设定的角度==反转===等于时停止===
				{
					r_RL_SMAngleNow--;		
					if( r_RL_SMStep == 0 )
					{
						r_RL_SMStep=8;
					}
					r_RL_SMStep--;
				}
			}
        }
	}
	F_RL_StepMotorOutPut();
}

/******************************************************************
 函数名称：						void	F_StepMotorSwing(unsigned int r_Angle1Buf,unsigned int r_Angle2Buf,unsigned char r_SMSpeedBuf,unsigned int r_Angle1Add,unsigned int r_Angle2Add)
 功能描述：						摆风
 调用本函数的时钟周期：			100ms
 输入参数：						r_Angle1Buf,r_Angle2Buf,r_SMSpeedBuf,r_Angle1Add,r_Angle2Add
 输出参数：						无
*******************************************************************/
//摆风角度范围1-2(角度1比角度2小)=========================
void F_RL_StepMotorSwing(unsigned int r_RL_Angle1Buf,unsigned int r_RL_Angle2Buf,unsigned char r_RL_SMSpeedBuf,unsigned int r_RL_Angle1Add,unsigned int r_RL_Angle2Add)
{
	static unsigned char r_RL_Swing1Reach = 0;	//角度1到达位置标志	
	
	r_RL_SMSpeedSet = r_RL_SMSpeedBuf;
	if( r_RL_Swing1Reach == 0 )//往角度1方向摆动
	{
		r_RL_SMAngleSet = r_RL_Angle1Buf;
		r_RL_SMAngleSet -= r_RL_Angle1Add;//角度1方向多摆动一定的补偿角度
		if( r_RL_SMAngleNow <= r_RL_SMAngleSet )
		{
			r_RL_Swing1Reach = 1;
			r_RL_SMAngleNow = r_RL_Angle1Buf;
		}	
	}
	else//往角度2方向摆动
	{
		r_RL_SMAngleSet = r_RL_Angle2Buf;
		r_RL_SMAngleSet += r_RL_Angle2Add;//角度2方向多摆动一定的补偿角度
		if( r_RL_SMAngleNow >= r_RL_SMAngleSet )
		{
			r_RL_Swing1Reach = 0;
			r_RL_SMAngleNow = r_RL_Angle2Buf;
		}
	}
}


/******************************************************************
 函数名称：						void	F_StepMotorStop(void)
 功能描述：						风门停止
 输入参数：						无
 输出参数：						无
*******************************************************************/
void F_RL_StepMotorStop(void)//风门停止
{
	r_RL_SMSpeedSet = c_RL_SMSpeedStop;	
}

/******************************************************************
 函数名称：						void F_StepMotorFixed(unsigned char r_AngleBuf, unsigned char r_SMSpeedBuf )
 功能描述：						以一定速度到达指定角度
 输入参数：						无
 输出参数：						无
*******************************************************************/
//void F_RL_StepMotorFixed( unsigned int r_RL_AngleBuf, unsigned char r_RL_SMSpeedBuf )//以一定速度到达指定角度
//{
//	r_RL_SMSpeedSet = r_RL_SMSpeedBuf;
//	r_RL_SMAngleSet = r_RL_AngleBuf;
//}

/******************************************************************
 函数名称：						void	F_StepMotorRstInit(void )
 功能描述：						风门复位启动--初始复位角度和当前角度值
 输入参数：						无
 输出参数：						无
*******************************************************************/
void F_RL_StepMotorRstInit(u16       	u16Buf)//风门复位启动--初始复位角度和当前角度值
{	
	r_RL_SMAngleNow = 0;		
	r_RL_SMAngleSet = u16Buf;	//c_RL_AngleRst;
}

/******************************************************************
 函数名称：						void	F_StepMotorRsting(unsigned char r_SMSpeedBuf )
 功能描述：						风门复位中
 输入参数：						
 输出参数：						无
*******************************************************************/
void F_RL_StepMotorRsting()//风门复位中
{	
	r_RL_SMSpeedSet = c_RL_SMSpeedRst;
	if(r_RL_SMAngleNow == r_RL_SMAngleSet)
	{	
		if(Rst_Ing == g_u8RL_SMRstState)
		{	
			//if(SysSet.fg.TurnOn)
			{	g_u8RL_SMRstState = Rst_Ok;}
		}		  
		else
		{	g_u8RL_SMRstState = Rst_Ok;}
	}
}

/******************************************************************
 函数名称：						void	F_StepMotorControl(void)
 功能描述：						步进电机控制
 调用本函数的时钟周期：			100ms
 输入参数：						无
 输出参数：						无
*******************************************************************/
void F_RL_StepMotorRstControl(void)//步进电机控制=
{
	if(Rst_Need == g_u8RL_SMRstState)//风门需要复位
	{	
		F_RL_StepMotorRstInit(Out.LeftRightFarAngleVal);
		g_u8RL_SMRstState = Rst_Ing;
	}
	else if(Rst_Ing == g_u8RL_SMRstState )//风门复位中 //|| Rst_Ing2 == g_u8RL_SMRstState
	{
		F_RL_StepMotorRsting();
	}
}