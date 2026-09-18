/*********************************************************
文 件 名 : DigitalLedDisplay.H
文件说明 : 数码管显示头文件
编写作者 : thm
修订作者 : 
编写时间 : 2021/1/13
版 本 号 : V1.0
==========================================================*/
#ifndef _DIGITAL_LED_DISPLAY_H_
#define _DIGITAL_LED_DISPLAY_H_ 

/*************************需配置部分***********************/
//数码管类型（0：共阴，1：共阳）
#define     C_DIGITAL_TYPE                     0  
    
//扫描间隔
#define     C_SCAN_INTERVAL_TIME               0
	      
//数码管数字COM口，l_u8LablePos
#define     C_DISPLAY_LENGTH               C_DIGITAL_3
#define     C_FIRST_NUM                        0           
#define     C_SECOND_NUM                       1
#define     C_THIRD_NUM                        2
#define     C_FOURTH_NUM                       3
#define     C_FIFTH_NUM                        4

#define     C_SLAVE1_DISPLAY_LENGTH        C_DIGITAL_3
#define     C_SLAVE1_FIRST_NUM                 3           
#define     C_SLAVE1_SECOND_NUM                4
#define     C_SLAVE1_THIRD_NUM                 5
#define     C_SLAVE1_FOURTH_NUM                3
#define     C_SLAVE1_FIFTH_NUM                 4

#define     C_SLAVE2_DISPLAY_LENGTH        C_DIGITAL_3
#define     C_SLAVE2_FIRST_NUM                 3           
#define     C_SLAVE2_SECOND_NUM                4
#define     C_SLAVE2_THIRD_NUM                 5
#define     C_SLAVE2_FOURTH_NUM                3
#define     C_SLAVE2_FIFTH_NUM                 4


//段IO口配置
#define     DIGITAL_SEG_NUMBERS                7			   //SEG个数（8-16）

#define     GPIO_SEG_A                         P0
#define     PIN_SEG_A                          4
#define     IO_SEG_A                           P04
#define     _SET_PIN_SEGA_IN_HI                Set_P04_in;
#define     _SET_PIN_SEGA_PP                   Set_P04_pp;

#define     GPIO_SEG_B                         P0
#define     PIN_SEG_B                          6
#define     IO_SEG_B                           P06
#define     _SET_PIN_SEGB_IN_HI                Set_P06_in;
#define     _SET_PIN_SEGB_PP                   Set_P06_pp;

#define     GPIO_SEG_C                         P0
#define     PIN_SEG_C                          1
#define     IO_SEG_C                           P01
#define     _SET_PIN_SEGC_IN_HI                Set_P01_in;
#define     _SET_PIN_SEGC_PP                   Set_P01_pp;

#define     GPIO_SEG_D                         P0
#define     PIN_SEG_D                          2
#define     IO_SEG_D                           P02
#define     _SET_PIN_SEGD_IN_HI                Set_P02_in;
#define     _SET_PIN_SEGD_PP                   Set_P02_pp;

#define     GPIO_SEG_E                         P0
#define     PIN_SEG_E                          3
#define     IO_SEG_E                           P03
#define     _SET_PIN_SEGE_IN_HI                Set_P03_in;
#define     _SET_PIN_SEGE_PP                   Set_P03_pp;

#define     GPIO_SEG_F                         P0
#define     PIN_SEG_F                          5
#define     IO_SEG_F                           P05
#define     _SET_PIN_SEGF_IN_HI                Set_P05_in;
#define     _SET_PIN_SEGF_PP                   Set_P05_pp;

#define     GPIO_SEG_G                         P0
#define     PIN_SEG_G                          0
#define     IO_SEG_G                           P00
#define     _SET_PIN_SEGG_IN_HI                Set_P00_pp;
#define     _SET_PIN_SEGG_PP                   Set_P00_pp;

#define     GPIO_SEG_H                         P0
#define     PIN_SEG_H                          0
#define     _SET_PIN_SEGH_IN_HI                
#define     _SET_PIN_SEGH_PP                   

#define     GPIO_SEG_I                         P3
#define     PIN_SEG_I                          3
#define     _SET_PIN_SEGI_IN_HI                
#define     _SET_PIN_SEGI_PP                   
													 
#define     GPIO_SEG_J                         P3
#define     PIN_SEG_J                          3
#define     _SET_PIN_SEGJ_IN_HI                
#define     _SET_PIN_SEGJ_PP                   

#define     GPIO_SEG_K                         P3
#define     PIN_SEG_K                          3
#define     _SET_PIN_SEGK_IN_HI                
#define     _SET_PIN_SEGK_PP                   

#define     GPIO_SEG_L                         P3
#define     PIN_SEG_L                          3
#define     _SET_PIN_SEGL_IN_HI                
#define     _SET_PIN_SEGL_PP                   

#define     GPIO_SEG_M                         P3
#define     PIN_SEG_M                          3
#define     _SET_PIN_SEGM_IN_HI                
#define     _SET_PIN_SEGM_PP                   

#define     GPIO_SEG_N                         P3
#define     PIN_SEG_N                          3
#define     _SET_PIN_SEGN_IN_HI                
#define     _SET_PIN_SEGN_PP                   

#define     GPIO_SEG_O                         P3
#define     PIN_SEG_O                          3
#define     _SET_PIN_SEGO_IN_HI                
#define     _SET_PIN_SEGO_PP                   

#define     GPIO_SEG_P                         P3
#define     PIN_SEG_P                          3
#define     _SET_PIN_SEGP_IN_HI                
#define     _SET_PIN_SEGP_PP                   

//位IO口配置
#define     DIGITAL_COM_NUMBERS                3 			   //COM个数（1-10）

#define     GPIO_COM1                          P0
#define     PIN_COM1                           7
#define     IO_COM1                            P07
#define     _SET_PIN_COM1_IN_HI                Set_P07_in;
#define     _SET_PIN_COM1_IN_PU                Set_P07_pu;
#define     _SET_PIN_COM1_PP                   Set_P07_pp;

#define     GPIO_COM2                          P2
#define     PIN_COM2                           2
#define     IO_COM2                            P22
#define     _SET_PIN_COM2_IN_HI                Set_P22_in;
#define     _SET_PIN_COM2_IN_PU                Set_P22_pu;
#define     _SET_PIN_COM2_PP                   Set_P22_pp;

#define     GPIO_COM3                          P2
#define     PIN_COM3                           3
#define     IO_COM3                            P23
#define     _SET_PIN_COM3_IN_HI                Set_P23_in;
#define     _SET_PIN_COM3_IN_PU                Set_P23_pu;
#define     _SET_PIN_COM3_PP                   Set_P23_pp;

#define     GPIO_COM4                          P0
#define     PIN_COM4                           5
#define     _SET_PIN_COM4_IN_HI                
#define     _SET_PIN_COM4_PP                   

#define     GPIO_COM5                          P0
#define     PIN_COM5                           6
#define     _SET_PIN_COM5_IN_HI                
#define     _SET_PIN_COM5_PP                   

#define     GPIO_COM6                          P0
#define     PIN_COM6                           7
#define     _SET_PIN_COM6_IN_HI                
#define     _SET_PIN_COM6_PP                   

#define     GPIO_COM7                          P5
#define     PIN_COM7                           0
#define     _SET_PIN_COM7_IN_HI                
#define     _SET_PIN_COM7_PP                   

#define     GPIO_COM8                          P5
#define     PIN_COM8                           1
#define     _SET_PIN_COM8_IN_HI                
#define     _SET_PIN_COM8_PP                   

#define     GPIO_COM9                          P5
#define     PIN_COM9                           0
#define     _SET_PIN_COM9_IN_HI                
#define     _SET_PIN_COM9_PP                   

#define     GPIO_COM10                         P5
#define     PIN_COM10                          0
#define     _SET_PIN_COM10_IN_HI               
#define     _SET_PIN_COM10_PP                  


/*************************配置结束*************************/


//COM口数据重定向
#define     g_u16Digital_COM1_Data             g_u16DisPrimaryBuff_TABLE[0]
#define     g_u16Digital_COM2_Data             g_u16DisPrimaryBuff_TABLE[1]
#define     g_u16Digital_COM3_Data             g_u16DisPrimaryBuff_TABLE[2]
#define     g_u16Digital_COM4_Data             g_u16DisPrimaryBuff_TABLE[3]
#define     g_u16Digital_COM5_Data             g_u16DisPrimaryBuff_TABLE[4]
#define     g_u16Digital_COM6_Data             g_u16DisPrimaryBuff_TABLE[5]
#define     g_u16Digital_COM7_Data             g_u16DisPrimaryBuff_TABLE[6]
#define     g_u16Digital_COM8_Data             g_u16DisPrimaryBuff_TABLE[7]
#define     g_u16Digital_COM9_Data             g_u16DisPrimaryBuff_TABLE[8]
#define     g_u16Digital_COM10_Data            g_u16DisPrimaryBuff_TABLE[9]

//数码管数字显示的位数
#define     C_DIGITAL_5                        5           
#define     C_DIGITAL_4                        4
#define     C_DIGITAL_3                        3
#define     C_DIGITAL_2                        2
#define     C_DIGITAL_1                        1

//图标定义，l_u8LableType                      
/*
	P   O 	N 	M   L   K   J   I		H G F E   D C B A
	15  14	13	12	11	10	9   8       7 6 5 4   3 2 1 0
*/ 
#define     C_LABLE_SEGA                       0x0001
#define     C_LABLE_SEGB                       0x0002
#define     C_LABLE_SEGC                       0x0004
#define     C_LABLE_SEGD                       0x0008
#define     C_LABLE_SEGE                       0x0010
#define     C_LABLE_SEGF                       0x0020
#define     C_LABLE_SEGG                       0x0040
#define     C_LABLE_SEGH                       0x0080

#define     C_LABLE_SEGI                       0x0100
#define     C_LABLE_SEGJ                       0x0200
#define     C_LABLE_SEGK                       0x0400
#define     C_LABLE_SEGL                       0x0800
#define     C_LABLE_SEGM                       0x1000
#define     C_LABLE_SEGN                       0x2000
#define     C_LABLE_SEGO                       0x4000
#define     C_LABLE_SEGP                       0x8000

#define     C_LABLE_NUM0                       0x3F
#define     C_LABLE_NUM1                       0x06
#define     C_LABLE_NUM2                       0x5B
#define     C_LABLE_NUM3                       0x4F
#define     C_LABLE_NUM4                       0x66
#define     C_LABLE_NUM5                       0x6D
#define     C_LABLE_NUM6                       0x7D
#define     C_LABLE_NUM7                       0x07
#define     C_LABLE_NUM8                       0x7F
#define     C_LABLE_NUM9                       0x6F

#define     C_LABLE_BLANK                      0x00
#define     C_LABLE_E                          0x79
#define     C_LABLE_F                          0x71
#define     C_LABLE_A                          0x77
#define     C_LABLE_T                          0x31
#define     C_LABLE_P                          0x73
#define     C_LABLE_n                          0x37
#define     C_LABLE_L                          0x38
#define     C_LABLE_H                          0x76
#define     C_LABLE_b                          0x7C
#define     C_LABLE_d                          0x5E
#define     C_LABLE_C                          0x39
#define     C_LABLE_U                          0x3E
#define     C_LABLE_UP_c                       0x61
#define     C_LABLE_DN_c                       0x58

//显示锁存
#define     C_DISP_LOCK                        1
#define     C_DISP_UNLOCK                      0


//数码管操作
#if (C_DIGITAL_TYPE == 0)  //共阴

#define     _IO_SEG_A_ON          _BS(GPIO_SEG_A,PIN_SEG_A)    
#define     _IO_SEG_A_OFF         _BC(GPIO_SEG_A,PIN_SEG_A)
#define     _IO_SEG_B_ON          _BS(GPIO_SEG_B,PIN_SEG_B)    
#define     _IO_SEG_B_OFF         _BC(GPIO_SEG_B,PIN_SEG_B)
#define     _IO_SEG_C_ON          _BS(GPIO_SEG_C,PIN_SEG_C)    
#define     _IO_SEG_C_OFF         _BC(GPIO_SEG_C,PIN_SEG_C)
#define     _IO_SEG_D_ON          _BS(GPIO_SEG_D,PIN_SEG_D)    
#define     _IO_SEG_D_OFF         _BC(GPIO_SEG_D,PIN_SEG_D)
#define     _IO_SEG_E_ON          _BS(GPIO_SEG_E,PIN_SEG_E)    
#define     _IO_SEG_E_OFF         _BC(GPIO_SEG_E,PIN_SEG_E)
#define     _IO_SEG_F_ON          _BS(GPIO_SEG_F,PIN_SEG_F)    
#define     _IO_SEG_F_OFF         _BC(GPIO_SEG_F,PIN_SEG_F)
#define     _IO_SEG_G_ON          _BS(GPIO_SEG_G,PIN_SEG_G)    
#define     _IO_SEG_G_OFF         _BC(GPIO_SEG_G,PIN_SEG_G)
#define     _IO_SEG_H_ON          _BS(GPIO_SEG_H,PIN_SEG_H)    
#define     _IO_SEG_H_OFF         _BC(GPIO_SEG_H,PIN_SEG_H)

#define     _IO_SEG_I_ON          _BS(GPIO_SEG_I,PIN_SEG_I)    
#define     _IO_SEG_I_OFF         _BC(GPIO_SEG_I,PIN_SEG_I)
#define     _IO_SEG_J_ON          _BS(GPIO_SEG_J,PIN_SEG_J)    
#define     _IO_SEG_J_OFF         _BC(GPIO_SEG_J,PIN_SEG_J)
#define     _IO_SEG_K_ON          _BS(GPIO_SEG_K,PIN_SEG_K)    
#define     _IO_SEG_K_OFF         _BC(GPIO_SEG_K,PIN_SEG_K)
#define     _IO_SEG_L_ON          _BS(GPIO_SEG_L,PIN_SEG_L)    
#define     _IO_SEG_L_OFF         _BC(GPIO_SEG_L,PIN_SEG_L)
#define     _IO_SEG_M_ON          _BS(GPIO_SEG_M,PIN_SEG_M)    
#define     _IO_SEG_M_OFF         _BC(GPIO_SEG_M,PIN_SEG_M)
#define     _IO_SEG_N_ON          _BS(GPIO_SEG_N,PIN_SEG_N)    
#define     _IO_SEG_N_OFF         _BC(GPIO_SEG_N,PIN_SEG_N)
#define     _IO_SEG_O_ON          _BS(GPIO_SEG_O,PIN_SEG_O)    
#define     _IO_SEG_O_OFF         _BC(GPIO_SEG_O,PIN_SEG_O)
#define     _IO_SEG_P_ON          _BS(GPIO_SEG_P,PIN_SEG_P)    
#define     _IO_SEG_P_OFF         _BC(GPIO_SEG_P,PIN_SEG_P)

#define     _IO_COM1_ON           _SET_PIN_COM1_PP;IO_COM1 = 0//_BC(GPIO_COM1,PIN_COM1)    
#define     _IO_COM1_OFF          _SET_PIN_COM1_IN_HI
#define     _IO_COM2_ON           _SET_PIN_COM2_PP;IO_COM2 = 0//_BC(GPIO_COM2,PIN_COM2)    
#define     _IO_COM2_OFF          _SET_PIN_COM2_IN_HI
#define     _IO_COM3_ON           _SET_PIN_COM3_PP;IO_COM3 = 0//_BC(GPIO_COM3,PIN_COM3)    
#define     _IO_COM3_OFF          _SET_PIN_COM3_IN_HI
#define     _IO_COM4_ON           _SET_PIN_COM4_PP;_BC(GPIO_COM4,PIN_COM4)    
#define     _IO_COM4_OFF          _SET_PIN_COM4_IN_HI
#define     _IO_COM5_ON           _SET_PIN_COM5_PP;_BC(GPIO_COM5,PIN_COM5)    
#define     _IO_COM5_OFF          _SET_PIN_COM5_IN_HI
#define     _IO_COM6_ON           _SET_PIN_COM6_PP;_BC(GPIO_COM6,PIN_COM6)    
#define     _IO_COM6_OFF          _SET_PIN_COM6_IN_HI
#define     _IO_COM7_ON           _SET_PIN_COM7_PP;_BC(GPIO_COM7,PIN_COM7)    
#define     _IO_COM7_OFF          _SET_PIN_COM7_IN_HI
#define     _IO_COM8_ON           _SET_PIN_COM8_PP;_BC(GPIO_COM8,PIN_COM8)    
#define     _IO_COM8_OFF          _SET_PIN_COM8_IN_HI
#define     _IO_COM9_ON           _SET_PIN_COM9_PP;_BC(GPIO_COM9,PIN_COM9)    
#define     _IO_COM9_OFF          _SET_PIN_COM9_IN_HI
#define     _IO_COM10_ON          _SET_PIN_COM10_PP;_BC(GPIO_COM10,PIN_COM10)    
#define     _IO_COM10_OFF         _SET_PIN_COM10_IN_HI

#else	    //共阳

#define     _IO_SEG_A_ON          _SET_PIN_SEGA_PP;_BC(GPIO_SEG_A,PIN_SEG_A)     
#define     _IO_SEG_A_OFF         _SET_PIN_SEGA_IN_HI
#define     _IO_SEG_B_ON          _SET_PIN_SEGB_PP;_BC(GPIO_SEG_B,PIN_SEG_B)    
#define     _IO_SEG_B_OFF         _SET_PIN_SEGB_IN_HI
#define     _IO_SEG_C_ON          _SET_PIN_SEGC_PP;_BC(GPIO_SEG_C,PIN_SEG_C)    
#define     _IO_SEG_C_OFF         _SET_PIN_SEGC_IN_HI
#define     _IO_SEG_D_ON          _SET_PIN_SEGD_PP;_BC(GPIO_SEG_D,PIN_SEG_D)    
#define     _IO_SEG_D_OFF         _SET_PIN_SEGD_IN_HI
#define     _IO_SEG_E_ON          _SET_PIN_SEGE_PP;_BC(GPIO_SEG_E,PIN_SEG_E)    
#define     _IO_SEG_E_OFF         _SET_PIN_SEGE_IN_HI
#define     _IO_SEG_F_ON          _SET_PIN_SEGF_PP;_BC(GPIO_SEG_F,PIN_SEG_F)    
#define     _IO_SEG_F_OFF         _SET_PIN_SEGF_IN_HI
#define     _IO_SEG_G_ON          _SET_PIN_SEGG_PP;_BC(GPIO_SEG_G,PIN_SEG_G)    
#define     _IO_SEG_G_OFF         _SET_PIN_SEGG_IN_HI
#define     _IO_SEG_H_ON          _SET_PIN_SEGH_PP;_BC(GPIO_SEG_H,PIN_SEG_H)    
#define     _IO_SEG_H_OFF         _SET_PIN_SEGH_IN_HI

#define     _IO_SEG_I_ON          _SET_PIN_SEGI_PP;_BC(GPIO_SEG_I,PIN_SEG_I)     
#define     _IO_SEG_I_OFF         _SET_PIN_SEGI_IN_HI
#define     _IO_SEG_J_ON          _SET_PIN_SEGJ_PP;_BC(GPIO_SEG_J,PIN_SEG_J)    
#define     _IO_SEG_J_OFF         _SET_PIN_SEGJ_IN_HI
#define     _IO_SEG_K_ON          _SET_PIN_SEGK_PP;_BC(GPIO_SEG_K,PIN_SEG_K)    
#define     _IO_SEG_K_OFF         _SET_PIN_SEGK_IN_HI
#define     _IO_SEG_L_ON          _SET_PIN_SEGL_PP;_BC(GPIO_SEG_L,PIN_SEG_L)    
#define     _IO_SEG_L_OFF         _SET_PIN_SEGL_IN_HI
#define     _IO_SEG_M_ON          _SET_PIN_SEGM_PP;_BC(GPIO_SEG_M,PIN_SEG_M)    
#define     _IO_SEG_M_OFF         _SET_PIN_SEGM_IN_HI
#define     _IO_SEG_N_ON          _SET_PIN_SEGN_PP;_BC(GPIO_SEG_N,PIN_SEG_N)    
#define     _IO_SEG_N_OFF         _SET_PIN_SEGN_IN_HI
#define     _IO_SEG_O_ON          _SET_PIN_SEGO_PP;_BC(GPIO_SEG_O,PIN_SEG_O)    
#define     _IO_SEG_O_OFF         _SET_PIN_SEGO_IN_HI
#define     _IO_SEG_P_ON          _SET_PIN_SEGP_PP;_BC(GPIO_SEG_P,PIN_SEG_P)    
#define     _IO_SEG_P_OFF         _SET_PIN_SEGP_IN_HI

#define     _IO_COM1_ON           _BS(GPIO_COM1,PIN_COM1)    
#define     _IO_COM1_OFF          _BC(GPIO_COM1,PIN_COM1)
#define     _IO_COM2_ON           _BS(GPIO_COM2,PIN_COM2)    
#define     _IO_COM2_OFF          _BC(GPIO_COM2,PIN_COM2)
#define     _IO_COM3_ON           _BS(GPIO_COM3,PIN_COM3)    
#define     _IO_COM3_OFF          _BC(GPIO_COM3,PIN_COM3)
#define     _IO_COM4_ON           _BS(GPIO_COM4,PIN_COM4)    
#define     _IO_COM4_OFF          _BC(GPIO_COM4,PIN_COM4)
#define     _IO_COM5_ON           _BS(GPIO_COM5,PIN_COM5)    
#define     _IO_COM5_OFF          _BC(GPIO_COM5,PIN_COM5)
#define     _IO_COM6_ON           _BS(GPIO_COM6,PIN_COM6)    
#define     _IO_COM6_OFF          _BC(GPIO_COM6,PIN_COM6)
#define     _IO_COM7_ON           _BS(GPIO_COM7,PIN_COM7)    
#define     _IO_COM7_OFF          _BC(GPIO_COM7,PIN_COM7)
#define     _IO_COM8_ON           _BS(GPIO_COM8,PIN_COM8)    
#define     _IO_COM8_OFF          _BC(GPIO_COM8,PIN_COM8)
#define     _IO_COM9_ON           _BS(GPIO_COM9,PIN_COM9)    
#define     _IO_COM9_OFF          _BC(GPIO_COM9,PIN_COM9)
#define     _IO_COM10_ON          _BS(GPIO_COM10,PIN_COM10)    
#define     _IO_COM10_OFF         _BC(GPIO_COM10,PIN_COM10)

#endif

extern code unsigned char C_NUMBER_TABLE[10];
extern unsigned int   xdata  g_u16DisPrimaryBuff_TABLE[DIGITAL_COM_NUMBERS];

//开放函数声明
extern      void     F_NUMBER_Discretize(unsigned int l_u16InputValue);
extern      void     F_NUMBER_Discretize_Slave1(unsigned int l_u16InputValue);
extern      void     F_NUMBER_Discretize_Slave2(unsigned int l_u16InputValue);
extern      void     F_LABLE_InsteadSet(unsigned int l_u16LableType, unsigned int *l_u16Digital_COM_Data);
extern      void     F_LABLE_CombineSet(unsigned int l_u16LableType, unsigned char l_u8ComNumber);
extern      void     F_DISPLAY_DataRedirect(void);
extern      void     F_DIGITAL_Drive(unsigned char l_u8DispLevel);
extern      void 	 F_DISP_Clear(void);
extern      void     F_DISP_All(void);

#endif