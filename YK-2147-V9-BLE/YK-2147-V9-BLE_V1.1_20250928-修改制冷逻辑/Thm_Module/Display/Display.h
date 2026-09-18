/*********************************************************
文 件 名 : Display.H
文件说明 : 显示处理头文件
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-11
版 本 号 : V1.0
==========================================================*/
#ifndef   __DISPLAY_H__
#define   __DISPLAY_H__

#define		C_SET_LEFT                1
#define		C_SET_RIGHT               0

//组合显示预定义
#define     _BLUE_DISP	        
#define     _L_UNIT_F_DISP	    g_u16DisPrimaryBuff_TABLE[1] |= C_LABLE_F;
#define     _L_UNIT_C_DISP	    g_u16DisPrimaryBuff_TABLE[1] |= C_LABLE_C;
#define     _R_UNIT_F_DISP	    g_u16DisPrimaryBuff_TABLE[1] |= C_LABLE_F;
#define     _R_UNIT_C_DISP	    g_u16DisPrimaryBuff_TABLE[1] |= C_LABLE_C;

#define     _ICON_H_DISP	    g_u16DisPrimaryBuff_TABLE[0] |= 0x49;\
								g_u16DisPrimaryBuff_TABLE[1] |= 0x49;\
								g_u16DisPrimaryBuff_TABLE[2] |= 0x49;
#define     _ICON_M_DISP	    g_u16DisPrimaryBuff_TABLE[0] |= 0x48;\
								g_u16DisPrimaryBuff_TABLE[1] |= 0x48;\
								g_u16DisPrimaryBuff_TABLE[2] |= 0x48;
#define     _ICON_L_DISP	    g_u16DisPrimaryBuff_TABLE[0] |= 0x08;\
								g_u16DisPrimaryBuff_TABLE[1] |= 0x08;\
								g_u16DisPrimaryBuff_TABLE[2] |= 0x08;

#define     _POWER_DOT_DISP	    
#define     _POWER_V_DISP	    
#define     _POWER_MAIN_0_DISP	

#define     _ICON_ECO_DISP		{g_u16DisPrimaryBuff_TABLE[C_FIRST_NUM] |= C_LABLE_E;\
	                             g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_C;\
								 g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM]  |= C_LABLE_NUM0;}
#define     _ICON_MAX_DISP		{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_H;\
								 g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM]  |= C_LABLE_H;}

#define     _POWER_SHAPE_DISP	

#define     _POWER_LV1_DISP	
#define     _POWER_LV2_DISP	
#define     _POWER_LV3_DISP		
#define     _POWER_LV4_DISP	
#define     _POWER_LV5_DISP		

#define     _APP_DISP		    {g_u16DisPrimaryBuff_TABLE[C_FIRST_NUM]  |= C_LABLE_A;\
								 g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_P;\
								 g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM]  |= C_LABLE_P;}
																					   
#define     _E1_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_E;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM1;}
#define     _E2_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_E;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM2;}
#define     _E3_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_E;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM3;}
#define     _E4_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_E;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM4;}
#define     _E5_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_E;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM5;}
#define     _E6_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_E;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM6;}
#define     _E7_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_E;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM7;}
#define     _E8_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_E;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM8;}
#define     _E9_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_E;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM9;}
#define     _EA_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_E;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_A;}
#define     _Eb_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_E;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_b;}
#define     _EC_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_E;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_C;}
#define     _Ed_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_E;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_d;}
#define     _EE_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_E;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_E;}


#define     _L_F1_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_F;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM1;}
#define     _L_F2_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_F;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM2;}
#define     _L_F3_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_F;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM3;}
#define     _L_F4_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_F;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM4;}
#define     _L_F5_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_F;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM5;}
#define     _L_F6_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_F;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM6;}
#define     _L_F7_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_F;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM7;}
#define     _L_F8_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_F;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM8;}
#define     _L_FF_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_F;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_F;}


#define     _R_F1_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_F;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM1;}
#define     _R_F2_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_F;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM2;}
#define     _R_F3_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_F;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM3;}
#define     _R_F4_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_F;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM4;}
#define     _R_F5_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_F;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM5;}
#define     _R_F6_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_F;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM6;}
#define     _R_F7_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_F;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM7;}
#define     _R_F8_DISP			{g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_F;g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_LABLE_NUM8;}


#define     _L_DIG1_G_DISP	    g_u16DisPrimaryBuff_TABLE[C_FIRST_NUM]  |= C_LABLE_SEGG;
#define     _L_DIG2_G_DISP	    g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM]  |= C_LABLE_SEGG;
#define     _L_DIG3_G_DISP	    g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM]  |= C_LABLE_SEGG;

#define     _R_DIG1_G_DISP	    g_u16DisPrimaryBuff_TABLE[C_FIRST_NUM]  |= C_LABLE_SEGG;
#define     _R_DIG2_G_DISP	    g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM]  |= C_LABLE_SEGG;
#define     _R_DIG3_G_DISP	    g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM]  |= C_LABLE_SEGG;


extern 	u8 xdata g_tu8Tm1650Buff[3];
extern 	u8 xdata g_u8VersionDispTime;
extern 	u8 xdata g_u8DispLv;
//外部声明
extern 	void F_SetStaticDispTime(void);
extern	void F_Display_Handle(void);
extern	void F_DISP_Clear(void);
extern	void F_LBOX_Display(u8 l_u8InputValue);
extern	void F_POWER_VALUE_Display(void);
extern	void F_BLUE_Display(void);
extern	void F_POWER_LEVEL_Display(void);
extern	void F_DISP_All(void);
extern	void F_TempValue_Display(s8 l_s8InputValue,u8 l_u8RightOrLeft);

extern	void F_AllDisplay_1650(void);
extern	void F_PowerValue_Display(void);

#endif