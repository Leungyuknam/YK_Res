/*--------------------------------------------------------------------
模块名称：    串口模块
模块作者：    CJB
编制时间：    2016-6-30
功能描述：    实现串口数据重发，数据帧完毕回调
修改记录：    2016-7-9 1：在F_ReceiveInital中增加对两个队列数据的初始化操作
                       2：在F_ClearReadFrameData中清除 r_ReadDataLength = 0;操作
---------------------------------------------------------------------*/
#define		FuncQueue_C

#include "h\Allinclude.h"

#define		FUNC_ARRAY_MAX			32

typedef struct{
	PFun pFunc;
	unsigned int Para;
	unsigned char ReDoCnt;
}TYPE_FuncQueue;

TYPE_FuncQueue LONG_REG CurFunc;
TYPE_FuncQueue LONG_REG FuncArray[FUNC_ARRAY_MAX];
unsigned char LONG_REG ArrayTop;
unsigned char LONG_REG ArrayBottom;

void ClearFunc(PFun Fun)
{
	if((CurFunc.pFunc == Fun)||(NULL_FUNC == Fun))
	{
		CurFunc.pFunc = NULL_FUNC;
		CurFunc.Para = 0;
		CurFunc.ReDoCnt = 0;
	}
}

void AddFunc(PFun Fun,unsigned int Para, unsigned char ReSendTimes)
{
	 unsigned char x;
	 x = ArrayBottom + 1;
	 if( x >= FUNC_ARRAY_MAX)
	 {
		 x = 0;
	 }
	 if( x != ArrayTop)
	 {
		 FuncArray[ArrayBottom].pFunc = Fun;
		 FuncArray[ArrayBottom].Para = Para;
		 FuncArray[ArrayBottom].ReDoCnt = ReSendTimes;
		 ArrayBottom = x;
	 }
}

void InsertFunc(PFun Fun,unsigned int Para, unsigned char ReSendTimes)
{
	unsigned char x;
	x = ArrayTop - 1;
	if( x >= FUNC_ARRAY_MAX )
	{
		x = FUNC_ARRAY_MAX - 1;
	}
	if( x != ArrayBottom)
	{
		ArrayTop = x;
		FuncArray[ArrayTop].pFunc = Fun;
		FuncArray[ArrayTop].Para = Para;
		FuncArray[ArrayTop].ReDoCnt = ReSendTimes;
	}
}

unsigned char CallArrayFunc(void)
{
	unsigned char result;

	result = 0;

	if(CurFunc.pFunc != NULL_FUNC)
	{
		if(0 != CurFunc.ReDoCnt)
		{
			CurFunc.ReDoCnt --;
			result = (*(CurFunc.pFunc))(CurFunc.Para);
		}
		else
		{
			ClearFunc(CurFunc.pFunc);
		}
	}
	else
	{
		if(ArrayTop != ArrayBottom)
		{
			CurFunc.pFunc = FuncArray[ArrayTop].pFunc;
			CurFunc.Para = FuncArray[ArrayTop].Para;
			CurFunc.ReDoCnt = FuncArray[ArrayTop].ReDoCnt;
			FuncArray[ArrayTop].pFunc = NULL_FUNC;
			FuncArray[ArrayTop].ReDoCnt = 0;
			ArrayTop ++;
			if(ArrayTop >= FUNC_ARRAY_MAX)
			{
				ArrayTop = 0;
			}
		}
	}
	return result;
}

void InitArraryFunc(void)
{
	unsigned char i;

	for(i = 0; i < FUNC_ARRAY_MAX; i++)
	{
		FuncArray[i].pFunc = NULL_FUNC;
		FuncArray[i].Para = 0;
		FuncArray[i].ReDoCnt = 0;
	}
	ArrayTop = 0;
	ArrayBottom = 0;
	ClearFunc(NULL_FUNC);
}
