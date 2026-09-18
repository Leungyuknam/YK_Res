#ifndef	_AD_H											
#define	_AD_H





#define	c_AD_NORMAL						0		//AD值正常
#define	c_AD_OVERSIZE					1		//AD值超过最大设定值
#define	c_AD_LESS_THAN					2		//AD值小于最大设定值


#define c_TempIn_Channel                2
#define c_TempOut_Channel               3
#define c_PowerAC_Channel               8




extern  U8  xdata g_u8TempIn,g_u8TempOut,g_u8TempOut_Bak,g_u8TempOut_TestBak;
extern  U16 xdata g_u16PowerAC_Ad;

extern 	code U16 t_u16table_temp[104];

/*****************************************************************/
#define	c_GETAD_AVERAGE_TIMER			18		//读c_GETAD_AVERAGE_TIMER次后取一次平均

#define	c_AD_ERROR_CHECK_TIMES			5		//检测到AD超过最大或最小值时,报错的判断次数


#define		FIND_TABLE_ENABLE			1	//使用查表功能

extern unsigned int  xdata AD_Value;

extern uint LONG_REG 	PowADValue;


extern void ADProc(void);
extern void F_GetCapAD();
extern void CAP_SetInPULL_ON();

#endif


