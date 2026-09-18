#ifndef _RL_STEPMOTOR_H_
#define _RL_STEPMOTOR_H_

//===================================步进电机控制子程序=============================

extern void F_RL_StepMotorDrive( unsigned char r_RL_Way );//步进电机驱动 r_Way:1--顺时针旋转  0--逆时针旋转
extern void F_RL_StepMotorRstControl(void);//步进电机控制=
extern void	F_RL_StepMotorSwing(unsigned int r_RL_Angle1Buf,unsigned int r_RL_Angle2Buf,unsigned char r_RL_SMSpeedBuf,unsigned int r_RL_Angle1Add,unsigned int r_RL_Angle2Add);//摆风
extern void F_RL_StepMotorStop(void);//风门停止
extern void F_RL_StepMotorFixed( unsigned int r_RL_AngleBuf, unsigned char r_RL_SMSpeedBuf );//以一定速度到达指定角度

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
#define 	p_RL_SMA1_on() 		{P_LR1 = 1;}	
#define 	p_RL_SMA1_off() 	{P_LR1 = 0;}	
#define 	p_RL_SMB1_on() 		{P_LR2 = 1;}	
#define 	p_RL_SMB1_off() 	{P_LR2 = 0;}	
#define 	p_RL_SMC1_on() 		{P_LR3 = 1;}	
#define 	p_RL_SMC1_off() 	{P_LR3 = 0;}	
#define 	p_RL_SMD1_on() 		{P_LR4 = 1;}	
#define 	p_RL_SMD1_off() 	{P_LR4 = 0;}	

//=================================================================
extern	unsigned char xdata r_RL_SMOutFlag1;//电机端口输出标志===
#define		b_RL_SMOutA1	r_RL_SMOutFlag1&BIT7	//步进电机A1输出
#define		b_RL_SMOutB1	r_RL_SMOutFlag1&BIT6	//步进电机B1输出
#define		b_RL_SMOutC1	r_RL_SMOutFlag1&BIT5	//步进电机C1输出
#define		b_RL_SMOutD1	r_RL_SMOutFlag1&BIT4	//步进电机D1输出

//输出轴步距角度  3.75/42.5 
//=======================================================================
#define		c_RL_AngleRst		2992//2946	//264*42.5/3.75 = 3400 //上电或关机时复位的角度--2018	 
											//90度，齿轮比12；39  292.5  结构95度 
//复位是0~264（数值增加方向）
//30度摆动数值	85-174
#define		C30D_R				(2992-2028)		//离复位角度的远端角度	(264-174)~(264-85)
#define		C30D_L				(2992-1020)		//离复位角度的近端角度
//50度摆动数值 60-205
#define		C50D_R				(2992-2346)		//(264-205)~(264-57)
#define		C50D_L				(2992-669)
//80度摆动数值 10-252
#define		C80D_R				(2992-2856)		//(264-250)~(264-12)
#define		C80D_L				(2992-159)

#define		c_RL_SMSpeedStop		0 //风门停止
#define		c_RL_SMSpeedRst			8//*1MS //风门复位  步数3410 	半周期15s 则15000/3410=7.33ms
#define		c_RL_SMSpeedSwing		8//*1MS //风门摆风  步数3400	半周期15s 则15000/3400=4.4ms
								  
extern unsigned char xdata r_RL_SMSpeedSet;	//电机设定速度===
extern signed int xdata r_RL_SMAngleNow;		//当前角度=
extern signed int xdata r_RL_SMAngleSet;		//设定角度=

extern unsigned char xdata g_u8RL_SMRstState;
//#define Rst_No		0
//#define Rst_Need	1
//#define Rst_Need2	2
//#define Rst_Ing		3
//#define Rst_Ing2	4
//#define Rst_Ok		5

#endif
