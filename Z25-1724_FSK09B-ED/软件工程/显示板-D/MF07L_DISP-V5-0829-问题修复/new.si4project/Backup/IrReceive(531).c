#include "Include\Include.h"
#include "IrCfg.h"
#include "MyLib\IrReceive\IrReceive.h"
#include <intrins.h>
/*-----------------------------------------------------------
模块：NEC红外遥控接收
作者：谢毅
日期：2022.4.28
-------------------------------------------------------------*/

StrIrData g_sIrRecData = {0};
EnumIr g_enuIrUpdataFlg = c_IrNokey;


/*-----------------------------------------------------------------------------
*函数：F_IrReceive()
*功能：红外遥控信号接收
*调用周期：125us中断服务函数
*输入参数：
*输出参数：
*其他说明：
------------------------------------------------------------------------------*/
void F_IrReceive()
{
	U32 l_u32IrBitData = 0;
	bit l_bRecPinLvl = 0;
	bit l_bEdge = 0;
	U16 l_u16PulseTime = 0;
	static bit s_bLongKeyed = 0;
	static bit s_bIrRecPinPrelvl = 1;
	static U8 s_u8Filter = 0;
	static U8 s_u8HCnt = 0;
	static U8 s_u8LCnt = 0;
	static U16 s_u16ShortKeyRecCntDwn = 0;
	static U16 s_u16LongKeyRecCntDwn = 0;
	static U8 s_u8ReceiveBitCnt = 0;
	static U8 s_u8LongKeyTimeCnt = 0;
	
	l_bRecPinLvl = IrRecPin;
	
	if(l_bRecPinLvl != s_bIrRecPinPrelvl)
	{	if(++s_u8Filter>=2)
		{	s_bIrRecPinPrelvl = l_bRecPinLvl ;
			l_bEdge = 1;
		}
	}
	else
	{	s_u8Filter = 0;
	}

	if(s_bIrRecPinPrelvl)
	{
		if(++s_u8HCnt>250)
		{	s_u8HCnt = 250;
		}
	}
	else
	{	if(++s_u8LCnt>250)
		{	s_u8LCnt = 250;
		}
	}

	if(l_bEdge)
	{	
		if(0 == s_bIrRecPinPrelvl)//下降沿
		{	
			l_u16PulseTime = (U16)s_u8LCnt+ s_u8HCnt;
		
			if(s_u16LongKeyRecCntDwn)//接收长按键
			{	/*
				if((l_u16PulseTime >= c_LongKeyPulseMin)&&(l_u16PulseTime <= c_LongKeyPulseMax))
				{	s_u16LongKeyRecCntDwn = c_KeyWaitTime;	//110ms内接收长按键
					
					if(s_u8LongKeyTimeCnt<255)
					{	s_u8LongKeyTimeCnt++;
						if(s_u8LongKeyTimeCnt >= c_IrLongKeyFirstTime)	//长按键信号110ms产生一次
						{	s_u8LongKeyTimeCnt -= c_IrLongKeyContinueTime;
							g_enuIrUpdataFlg = c_IrLongKey;	
							s_bLongKeyed = 1;
						}
					}	
				}
				*/
			}
			else if(s_u16ShortKeyRecCntDwn)//接收0/1
			{
				if((l_u16PulseTime >= c_Data1PulseMin)&&(l_u16PulseTime <= c_Data1PulseMax))
				{	l_u32IrBitData = 0x80000000;
				}
				else if((l_u16PulseTime >= c_Data0PulseMin)&&(l_u16PulseTime <= c_Data0PulseMax))
				{	l_u32IrBitData = 0;
				}
				else //错误信息
				{	s_u16ShortKeyRecCntDwn = 0;	//忽略这帧信息
					goto jump;	//跳过数据处理
				}
				g_sIrRecData.Dword >>= 1;			//NEC标准，数据从低位到高位传送
				g_sIrRecData.Dword |= l_u32IrBitData;
				if(++s_u8ReceiveBitCnt >= 32)		//按下瞬间
				{	
					s_u8ReceiveBitCnt = 0;
					s_u8LongKeyTimeCnt = 0;
					s_u16LongKeyRecCntDwn = s_u16ShortKeyRecCntDwn+c_LongKeyPulseMax;	//短键后11.5MS内接收长按键
					s_u16ShortKeyRecCntDwn = 0;
					#if (0 == c_IrShortKeyMode)
					g_enuIrUpdataFlg = c_IrShortKey;
					#endif
					
				}else{}	
				jump:;
			}
			else if(c_IrNokey == g_enuIrUpdataFlg)//接收引导码
			{
				if((l_u16PulseTime >= c_LeaderPulseMin)&&(l_u16PulseTime <= c_LeaderPulseMax))
				{	g_sIrRecData.Dword = 0;
					s_u8ReceiveBitCnt = 0;
					s_u16ShortKeyRecCntDwn = c_KeyWaitTime;	//110ms内接收短按键
				}
			}
			
			s_u8HCnt = 0;//脉冲周期计时重新开始	
			s_u8LCnt = 0;
		}
		else//上升沿
		{	
		}
	}		

	if(s_u16ShortKeyRecCntDwn)//限制短按键帧接收时间
	{	s_u16ShortKeyRecCntDwn--;
	}

	if(s_u16LongKeyRecCntDwn)//限制长按键帧接收时间
	{	if(0 == --s_u16LongKeyRecCntDwn)//按键松开
		{	if(s_bLongKeyed)//长键松开
			{	s_bLongKeyed = 0;
			}
			else//短键松开
			{	
				#if c_IrShortKeyMode
				g_enuIrUpdataFlg = c_IrShortKey;
				#endif
			}
		}
	}
}

/*--------------------------------------------------------------------------------
*函数名称：F_IrDataProc()
*功能描述：遥控数据处理
*调用本函数的时间周期：10ms
*输入参数：
*输出参数：
*其他说明：
-------------------------------------------------------------------------------*/
void F_IrDataProc()
{
	U8 l_u8Cnt = 0;
	if(g_enuIrUpdataFlg != c_IrNokey)
	{	
//		BELL_NOR();
//		_nop_();
//		_nop_();
//		if((g_sIrRecData.Bytes.Cmd == ~g_sIrRecData.Bytes.CmdReverse)
//		&&(g_sIrRecData.Bytes.ClientCode == IR_ID_HEAD_L)\
//		&&(g_sIrRecData.Bytes.ClientCodeReverse == IR_ID_HEAD_H))
		{	//if(c_ClientCode == g_sIrRecData.Bytes.ClientCode)
			{	if(c_IrShortKey == g_enuIrUpdataFlg)
				{	
//		BELL_NOR();
//		_nop_();	
					for(l_u8Cnt=0;l_u8Cnt<c_IrShortKeyNum;l_u8Cnt++)
					{	if(g_sIrRecData.Bytes.Cmd == g_sShortKeyToEvent[l_u8Cnt].KeyCode)
						{	(g_sShortKeyToEvent[l_u8Cnt].VoidIrKeyEvent)();
							break;
						}
					}
				}
				else//长按键
				{	
					#if c_IrLongKeyNum
					for(l_u8Cnt=0;l_u8Cnt<c_IrLongKeyNum;l_u8Cnt++)
					{	if(g_sIrRecData.Bytes.Cmd == g_sLongKeyToEvent[l_u8Cnt].KeyCode)
						{	(g_sLongKeyToEvent[l_u8Cnt].VoidIrKeyEvent)();
							break;
						}
					}
					#endif
				}
			}
		}
		g_enuIrUpdataFlg = c_IrNokey;
	}
}

