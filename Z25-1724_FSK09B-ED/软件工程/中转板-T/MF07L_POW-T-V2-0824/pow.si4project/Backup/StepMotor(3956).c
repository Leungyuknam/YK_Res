#include "..\Include\Include.h"

//==========================================================================
void	F_StepMotorSwing(unsigned int r_Angle1Buf,unsigned int r_Angle2Buf,unsigned char r_SMSpeedBuf,unsigned int r_Angle1Add,unsigned int r_Angle2Add);
void	F_StepMotorStop(void);//风门停止
void	F_StepMotorFixed( unsigned char r_AngleBuf, unsigned char r_SMSpeedBuf );//以一定速度到达指定角度
//==========================================================================
unsigned char xdata r_SMOutFlag1 = 0;	//电机端口输出标志===
unsigned char xdata r_SMSpeedSet = 0;	//电机设定速度===
signed int xdata r_SMAngleNow = 0;		//当前角度=
signed int xdata r_SMAngleSet = 0;		//设定角度=

unsigned char xdata g_u8SMRstState = Rst_No;   //复位状态==

#define		b_SMABit		BIT7
#define		b_SMBBit		BIT6	
#define		b_SMCBit		BIT5
#define		b_SMDBit		BIT4
#define		c_SMA			b_SMABit
#define		c_SMAB			b_SMABit|b_SMBBit
#define		c_SMB			b_SMBBit
#define		c_SMBC			b_SMBBit|b_SMCBit
#define		c_SMC			b_SMCBit
#define		c_SMCD			b_SMCBit|b_SMDBit
#define		c_SMD			b_SMDBit
#define		c_SMDA			b_SMDBit|b_SMABit	

//========四相8拍方式====A-AB-B-BC-C-CD-D-DA(顺时针)================
const unsigned char Table_SM1[8] =
{
	c_SMA,c_SMAB,c_SMB,c_SMBC,c_SMC,c_SMCD,c_SMD,c_SMDA  
};
//========四相8拍方式====A-DA-D-CD-C-BC-B-AB(逆时针)================
const unsigned char Table_SM2[8] =
{	
	c_SMA,c_SMDA,c_SMD,c_SMCD,c_SMC,c_SMBC,c_SMB,c_SMAB   
};

/******************************************************************
 函数名称：						void	F_StepMotorOutPut(void)
 功能描述：						步进电机端口输出
 输入参数：						无
 输出参数：						无
*******************************************************************/
//=======================================================================
void F_StepMotorOutPut(void)//步进电机端口输出=====
{
	if( b_SMOutA1 )
	{
		p_SMA1_on();
	}
	else
	{
		p_SMA1_off();
	}
	if( b_SMOutB1 )
	{
		p_SMB1_on();
	}
	else
	{
		p_SMB1_off();
	}
	if( b_SMOutC1 )
	{
		p_SMC1_on();
	}
	else
	{
		p_SMC1_off();
	}
	if( b_SMOutD1 )
	{
		p_SMD1_on();
	}
	else
	{
		p_SMD1_off();
	}
}

/******************************************************************
 函数名称：						void F_StepMotorDrive()
 功能描述：						r_Way:1--顺时针旋转  0--逆时针旋转
 调用本函数的时钟周期：			2ms
 输入参数：						r_Way
 输出参数：						无
*******************************************************************/
//=======================================================================
void F_StepMotorDrive(unsigned char r_Way)//步进电机驱动 r_Way:1--顺时针旋转  0--逆时针旋转
{
	static	unsigned char r_SMStep = 0;		//电机步数===
	static	unsigned char r_SMSpeed = 0;	//电机速度===
	static  unsigned char r_SDelay = 0;		//切换延时

	if( ( r_SMAngleNow == r_SMAngleSet )|| ( r_SMSpeedSet == 0 ) )//电机转到设定角度或者转速要求为0--停止电机
	{
		r_SMOutFlag1=0;
        if(0 == r_SDelay)
		{	r_SDelay = 100;} //100
	}
	else
	{	
        if(r_SDelay > 0)
		{	r_SDelay--;}
		else
		{
			if( r_SMSpeed > 1 )
			{
				r_SMSpeed--;
			}
			else
			{
				r_SMSpeed=r_SMSpeedSet;	
				if( r_Way )//调用顺时针表格
				{
					r_SMOutFlag1=Table_SM1[r_SMStep];				
				}
				else//调用逆时针表格
				{
					r_SMOutFlag1=Table_SM2[r_SMStep];
				}	
							
				if( r_SMAngleNow < r_SMAngleSet )//当前角度小于设定的角度==
				{
					r_SMAngleNow++;
					r_SMStep++;
					if( r_SMStep >= 8 )
					{
						r_SMStep=0;
					}	
				}
				else//当前角度大于设定的角度==反转===等于时停止===
				{
					r_SMAngleNow--;		
					if( r_SMStep == 0 )
					{
						r_SMStep=8;
					}
					r_SMStep--;
				}
			}
        }
	}
	F_StepMotorOutPut();
}


/******************************************************************
 函数名称：						void	F_StepMotorSwing(unsigned int r_Angle1Buf,unsigned int r_Angle2Buf,unsigned char r_SMSpeedBuf,unsigned int r_Angle1Add,unsigned int r_Angle2Add)
 功能描述：						摆风
 调用本函数的时钟周期：			100ms
 输入参数：						r_Angle1Buf,r_Angle2Buf,r_SMSpeedBuf,r_Angle1Add,r_Angle2Add
 输出参数：						无
*******************************************************************/
//摆风角度范围1-2(角度1比角度2小)=========================
void F_StepMotorSwing(unsigned int r_Angle1Buf,unsigned int r_Angle2Buf,unsigned char r_SMSpeedBuf,unsigned int r_Angle1Add,unsigned int r_Angle2Add)
{
	static	unsigned char	r_Swing1Reach=0;	//角度1到达位置标志	
	
	r_SMSpeedSet=r_SMSpeedBuf;
	if( r_Swing1Reach == 0 )//往角度1方向摆动
	{
		r_SMAngleSet=r_Angle1Buf;
		r_SMAngleSet-=r_Angle1Add;//角度1方向多摆动一定的补偿角度
		if( r_SMAngleNow <= r_SMAngleSet )
		{
			r_Swing1Reach=1;
			r_SMAngleNow=r_Angle1Buf;
		}	
	}
	else//往角度2方向摆动
	{
		r_SMAngleSet=r_Angle2Buf;
		r_SMAngleSet+=r_Angle2Add;//角度2方向多摆动一定的补偿角度
		if( r_SMAngleNow >= r_SMAngleSet )
		{
			r_Swing1Reach=0;
			r_SMAngleNow=r_Angle2Buf;
		}
	}
}

/******************************************************************
 函数名称：						void F_StepMotorStop(void)
 功能描述：						风门停止
 输入参数：						无
 输出参数：						无
*******************************************************************/
void F_StepMotorStop(void)//风门停止
{
	r_SMSpeedSet = c_SMSpeedStop;	
}

/******************************************************************
 函数名称：						void	F_StepMotorFixed( unsigned char r_AngleBuf, unsigned char r_SMSpeedBuf )
 功能描述：						以一定速度到达指定角度
 输入参数：						无
 输出参数：						无
*******************************************************************/
//void F_StepMotorFixed( unsigned char r_AngleBuf, unsigned char r_SMSpeedBuf )//以一定速度到达指定角度
//{
//	r_SMSpeedSet = r_SMSpeedBuf;
//	r_SMAngleSet = r_AngleBuf;
//}

/******************************************************************
 函数名称：						void F_StepMotorRstInit(void )
 功能描述：						风门复位启动--初始复位角度和当前角度值
 输入参数：						无
 输出参数：						无
*******************************************************************/
void F_StepMotorRstInit(u16        u16Buf  )//风门复位启动--初始复位角度和当前角度值
{	
	r_SMAngleNow = 0;		
	r_SMAngleSet = u16Buf;//c_AngleRst;
}

/******************************************************************
 函数名称：						void	F_StepMotorRsting(unsigned char r_SMSpeedBuf )
 功能描述：						风门复位中
 调用本函数的时钟周期：			100ms
 输入参数：						
 输出参数：						无
*******************************************************************/
void F_StepMotorRsting()//风门复位中
{	
	r_SMSpeedSet = c_SMSpeedRst;
	if((r_SMAngleNow == r_SMAngleSet) || g_bRstPos)		//已运行指定步数||霍尔触发
	{	
		if(Rst_Ing == g_u8SMRstState)
		{	
			//if(SysSet.fg.TurnOn)
			{	
				g_u8SMRstState = Rst_Ok;
				r_SMAngleNow = r_SMAngleSet;
			}

		}		  
		else
		{	g_u8SMRstState = Rst_Ok;}
	}
}

/******************************************************************
 函数名称：						void F_StepMotorRstControl(void)
 功能描述：						步进电机控制
 调用本函数的时钟周期：			100ms
 输入参数：						无
 输出参数：						无
*******************************************************************/
void F_StepMotorRstControl(void)//步进电机控制=
{
	if(Rst_Need == g_u8SMRstState)//风门需要复位
	{	
		F_StepMotorRstInit(Out.UpDnFarAngleVal);
//		F_StepMotorRstInit(UPDN_UP180);
//		F_StepMotorRstInit(c_AngleRst);
		g_u8SMRstState = Rst_Ing;
	}
	//else if(Rst_Need2 == g_u8SMRstState)
	//{
	//	F_StepMotorFixed(6, c_SMSpeedRst);
	//	g_u8SMRstState = Rst_Ing2;
	//}
	else if(Rst_Ing == g_u8SMRstState )//风门复位中//|| Rst_Ing2 == g_u8SMRstState
	{
		F_StepMotorRsting();
	}
}