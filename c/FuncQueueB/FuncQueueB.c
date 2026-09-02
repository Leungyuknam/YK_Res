/*--------------------------------------------------------------------
模块名称：    串口模块
模块作者：    CJB
编制时间：    2016-6-30
功能描述：    实现串口数据重发，数据帧完毕回调
修改记录：    2016-7-9 1：在F_ReceiveInital中增加对两个队列数据的初始化操作
                       2：在F_ClearReadFrameData中清除 r_ReadDataLength = 0;操作
---------------------------------------------------------------------*/
#define	_FuncQueueB_C_

#include "h\Allinclude.h"

#define	FUNC_ARRAY_MAX_B			32

typedef struct{
	PFun pFuncB;
	unsigned int ParaB;
	unsigned char ReDoCntB;
}TYPE_FuncQueueB;

TYPE_FuncQueueB LONG_REG CurFuncB;
TYPE_FuncQueueB LONG_REG FuncArrayB[FUNC_ARRAY_MAX_B];
unsigned char LONG_REG ArrayTopB;
unsigned char LONG_REG ArrayBottomB;

void ClearFuncB(PFun Fun)
{
	if((CurFuncB.pFuncB == Fun)||(NULL_FUNC == Fun))
	{
		CurFuncB.pFuncB = NULL_FUNC;
		CurFuncB.ParaB = 0;
		CurFuncB.ReDoCntB = 0;
	}
}

void AddFuncB(PFun Fun,unsigned int ParaB, unsigned char ReSendTimes)
{
	 unsigned char x;
	 x = ArrayBottomB + 1;
	 if( x >= FUNC_ARRAY_MAX_B)
	 {
		 x = 0;
	 }
	 if( x != ArrayTopB)
	 {
		 FuncArrayB[ArrayBottomB].pFuncB = Fun;
		 FuncArrayB[ArrayBottomB].ParaB = ParaB;
		 FuncArrayB[ArrayBottomB].ReDoCntB = ReSendTimes;
		 ArrayBottomB = x;
	 }
}

void InsertFuncB(PFun Fun,unsigned int ParaB, unsigned char ReSendTimes)
{
	unsigned char x;
	x = ArrayTopB - 1;
	if( x >= FUNC_ARRAY_MAX_B )
	{
		x = FUNC_ARRAY_MAX_B - 1;
	}
	if( x != ArrayBottomB)
	{
		ArrayTopB = x;
		FuncArrayB[ArrayTopB].pFuncB = Fun;
		FuncArrayB[ArrayTopB].ParaB = ParaB;
		FuncArrayB[ArrayTopB].ReDoCntB = ReSendTimes;
	}
}

unsigned char CallArrayFuncB(void)
{
	unsigned char result;

	result = 0;

	if(CurFuncB.pFuncB != NULL_FUNC)
	{
		if(0 != CurFuncB.ReDoCntB)
		{
			CurFuncB.ReDoCntB --;
			result = (*(CurFuncB.pFuncB))(CurFuncB.ParaB);
		}
		else
		{
			ClearFuncB(CurFuncB.pFuncB);
		}
	}
	else
	{
		if(ArrayTopB != ArrayBottomB)
		{
			CurFuncB.pFuncB = FuncArrayB[ArrayTopB].pFuncB;
			CurFuncB.ParaB = FuncArrayB[ArrayTopB].ParaB;
			CurFuncB.ReDoCntB = FuncArrayB[ArrayTopB].ReDoCntB;
			FuncArrayB[ArrayTopB].pFuncB = NULL_FUNC;
			FuncArrayB[ArrayTopB].ReDoCntB = 0;
			ArrayTopB ++;
			if(ArrayTopB >= FUNC_ARRAY_MAX_B)
			{
				ArrayTopB = 0;
			}
		}
	}
	return result;
}

void InitArraryFuncB(void)
{
	unsigned char i;

	for(i = 0; i < FUNC_ARRAY_MAX_B; i++)
	{
		FuncArrayB[i].pFuncB = NULL_FUNC;
		FuncArrayB[i].ParaB = 0;
		FuncArrayB[i].ReDoCntB = 0;
	}
	ArrayTopB = 0;
	ArrayBottomB = 0;
	ClearFuncB(NULL_FUNC);
}
