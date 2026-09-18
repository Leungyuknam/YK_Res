/********************************************************************************************************
说明:
时间片产生模块，产生5ms,10ms,100ms,1s,1min等基本时间片
/*******************************************************************************************************/
#define _Native_Time_Fuction_H
#include "..\Include\Include.h"

static unsigned char TimeFlag = 0;
static unsigned char uc100msCnt = 0;
static unsigned char uc1sCnt = 0;



/********************************************************************************************************
 * @ name   : unsigned char GetTimeTick(unsigned char temp)
 * @ msg    : 时间片查询
 * @ param   (unsigned char) temp
 * @ return  (*)
 ********************************************************************************************************/
unsigned char GetTimeTick(unsigned char temp)
{
	if (temp & TimeFlag)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/********************************************************************************************************
 * @ name   : void TimeBaseTickIRQ(void)
 * @ msg    : 时间中断
 * @ param   (*)
 * @ return  (*)
 ********************************************************************************************************/
void TimeBaseTickIRQ(void)
{
	if(g_u8BaseTime < 255)
		g_u8BaseTime ++;

	if(g_u8SysTime_Cnt < 255)
		g_u8SysTime_Cnt ++;

	if(g_u82MS_Cnt < 255)
		g_u82MS_Cnt ++;

	


	
//	if(g_u8RxSI_Cnt < 255)
//		g_u8RxSI_Cnt ++;
}

/********************************************************************************************************
 * @ name   : void TimeFucRunStart(void)
 * @ msg    : 时间总函数
 * @ param   (*)
 * @ return  (*)
 ********************************************************************************************************/
//void TimeFucRunStart(void)
//{
//	uc100msCnt = 0;
//	uc1sCnt = 0;
//}

//void TCntReset()
//{
//	B_Bright_500MS = 1;
//	B_Bright_1S = 0;
//	B_Bright_OFF_1S = 0;
//	g_u8Bright_500MS_Cnt = 0;
//	B_OneSec_OFFON = 0;
//}



/********************************************************************************************************
 * @ name   : void TimeProc(void)
 * @ msg    : 时间总函数
 * @ param   (*)
 * @ return  (*)
 ********************************************************************************************************/
void TimeProc(void)
{
	static unsigned char uc1minCnt = 0;
	TimeFlag = 0;
	g_TimeSign.Byte &= (BIT_BRIGHT_500MS + BIT_500MS_BRION + BIT_ONESEC_OFFON);
	if (g_u8SysTime_Cnt >= 80)
	{
		g_u8SysTime_Cnt -= 80;
		TimeFlag |= TIME_10MS;
		uc100msCnt++;
		if (uc100msCnt >= 10)
		{
			TimeFlag |= TIME_100MS;
			uc100msCnt = 0;
			
			if(++g_u8Bright_500MS_Cnt >= 5)
			{
				g_u8Bright_500MS_Cnt = 0;
				B_Bright_500MS ^= 1;
				if(B_Bright_500MS)
				{
					B_OneSec_OFFON ^= 1;
					B_Bright_1S = TRUE;
				}		
				else					
					B_Bright_OFF_1S = TRUE;
			}



			uc1sCnt++;
			if (uc1sCnt >= 10)
			{
				TimeFlag |= TIME_1S;
				uc1sCnt = 0;

				uc1minCnt++;
				if (uc1minCnt >= 60)
				{
					TimeFlag |= TIME_1MIN;
					uc1minCnt = 0;
				}
			}
			if((5 == uc1sCnt) || (0 == uc1sCnt))
			{
				B_500MS = TRUE;	
			}

		}

		if(uc1sCnt < 5)
			B_500MS_BriOn = TRUE;
		else
			B_500MS_BriOn = FALSE;		

	}

}


/**
 * @ name   : unsigned char CheckSum(unsigned char *ucPoint,unsigned char	 Count)
 * @ msg    : 计算校验和
 * @ param   (unsigned char) *ucPoint	待校验数组
 * @ param   (unsigned char) Count		数组个数
 * @ return  (unsigned char) temp		校验和
 */
//unsigned char CheckSum(unsigned char *ucPoint, unsigned char Count)
//{
//	unsigned char temp;
//	temp = Count + 0x5A;
//	while (Count)
//	{
//		Count--;
//		temp -= (*ucPoint);
//		ucPoint++;
//	}
//	return temp;
//}

/**
 * @ name   : CopyStr(unsigned char *TargetStr, unsigned char *SourceStr, unsigned char Count)
 * @ msg    : 字符串复制
 * @ param   (unsigned char) *TargetStr		目标字符串指针
 * @ param   (unsigned char) *SourceStr		源字符串指针
 * @ param   (unsigned char) Count			个数
 * @ return  (*)
 */
//void CopyStr(unsigned char *TargetStr, unsigned char *SourceStr, unsigned char Count)
//{
//	while (Count)
//	{
//		Count--;
//		*TargetStr = *SourceStr;
//		TargetStr++;
//		SourceStr++;
//	}
//}

/**
 * @ name   : SetStr(unsigned char *TargetStr, unsigned char Value, unsigned char Count)
 * @ msg    : 字符串赋值
 * @ param   (unsigned char) *TargetStr		目标字符串指针
 * @ param   (unsigned char) Value			源字符串指针
 * @ param   (unsigned char) Count			个数
 * @ return  (*)
 */
//void SetStr(unsigned char *TargetStr, unsigned char Value, unsigned char Count)
//{
//	while (Count)
//	{
//		Count--;
//		*TargetStr = Value;
//		TargetStr++;
//	}
//}


u8 CmpStrAndCopy(unsigned char *TargetStr, unsigned char *SourceStr, unsigned char Count)
{
	u8 u8temp0 = 1;
	while (Count)
	{
		Count--;
		if(*TargetStr != *SourceStr)
		{
			u8temp0 = 0;
		}
		*TargetStr = *SourceStr;
		TargetStr++;
		SourceStr++;
	}
	
	return u8temp0;
}



#if 0
/**
 * @ name   : unsigned char CompareStr(unsigned char *TargetStr, unsigned char *SourceStr, unsigned char Count)
 * @ msg    : 字符串比较
 * @ param   (unsigned char) *TargetStr		目标字符串指针
 * @ param   (unsigned char) *SourceStr		源字符串指针
 * @ param   (unsigned char) Count			个数
 * @ return  (unsigned char) 0				不相同
 * @ return  (unsigned char) 1				相同
 */
unsigned char CompareStr(unsigned char *TargetStr, unsigned char *SourceStr, unsigned char Count)
{
	while (Count)
	{
		Count--;
		if (*TargetStr != *SourceStr)
		{
			return 0;
		}
		TargetStr++;
		SourceStr++;
	}
	return 1;
}
#endif
