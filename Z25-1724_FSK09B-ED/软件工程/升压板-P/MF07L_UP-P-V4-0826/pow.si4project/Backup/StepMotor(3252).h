#ifndef _STEPMOTOR_H_
#define _STEPMOTOR_H_

//===================================步进电机控制子程序=============================

extern void F_StepMotorDrive(unsigned char r_Way);//步进电机驱动 r_Way:1--顺时针旋转  0--逆时针旋转
extern void F_StepMotorRstControl(void);//步进电机控制=
extern void	F_StepMotorSwing(unsigned int r_Angle1Buf,unsigned int r_Angle2Buf,unsigned char r_SMSpeedBuf,unsigned int r_Angle1Add,unsigned int r_Angle2Add);//摆风
extern void F_StepMotorStop(void);//风门停止
extern void F_StepMotorFixed(unsigned char r_AngleBuf, unsigned char r_SMSpeedBuf );//以一定速度到达指定角度

//==================================================================================

/*==================================================================================
步进电机工作说明:
常用家电类产品步进电机一般为四相八拍:例如空调类产品
四相八拍: A-AB-B-BC-C-CD-D-DA  ====顺时针旋转===============
四相八拍: D-CD-C-BC-B-AB-A-DA  ====逆时针旋转===============
步进电机端口定义:通常红色线为12V电源端，靠近红色线端依次为：12V-D-C-B-A。 
步距角度:步进电机每走一步旋转的角度,可查看规格书，一般为5.625°/64，例如要选转100°，则运行的步数:100*64/5.625
驱动步进电机的时序要求: 1ms走一步时间太短，会造成步进电机无法转动，至少2ms.一般4ms以上
==================================================================================*/


//=======步进电机端口定义--==========
#define 	p_SMA1_on() 		{P_UPDN1 = 1;}	
#define 	p_SMA1_off() 		{P_UPDN1 = 0;}
#define 	p_SMB1_on() 		{P_UPDN2 = 1;}	
#define 	p_SMB1_off() 		{P_UPDN2 = 0;}
#define 	p_SMC1_on() 		{P_UPDN3 = 1;}	
#define 	p_SMC1_off() 		{P_UPDN3 = 0;}
#define 	p_SMD1_on() 		{P_UPDN4 = 1;}	
#define 	p_SMD1_off() 		{P_UPDN4 = 0;}	

//=================================================================
extern	unsigned char xdata r_SMOutFlag1;//电机端口输出标志===
#define		b_SMOutA1		r_SMOutFlag1&BIT7	//步进电机A1输出
#define		b_SMOutB1		r_SMOutFlag1&BIT6	//步进电机B1输出
#define		b_SMOutC1		r_SMOutFlag1&BIT5	//步进电机C1输出
#define		b_SMOutD1		r_SMOutFlag1&BIT4	//步进电机D1输出

//输出轴步距角度  7.5/85
//=======================================================================
#define		c_AngleRst			1201	//106*85/7.5 = 1201 //上电或关机时复位的角度--1130--全开位	 

//30度摆动数值	85-174
#define		UPDN_UP				(1201-1077)		//离复位角度的远端角度	(106-89)~(106-11)
#define		UPDN_DN				(1201-193)		//离复位角度的近端角度

#define		c_SMSpeedStop		0 //风门停止
#define		c_SMSpeedRst		4//*2MS //风门复位  71.4Hz
#define		c_SMSpeedSwing		4//*2MS //风门摆风

								  
extern unsigned char xdata r_SMSpeedSet;	//电机设定速度===
extern signed int xdata r_SMAngleNow;//当前角度=
extern signed int xdata r_SMAngleSet;//设定角度=

extern unsigned char xdata g_u8SMRstState;
#define Rst_No		0
#define Rst_Need	1
#define Rst_Ing		2
#define Rst_Ok		3

#endif