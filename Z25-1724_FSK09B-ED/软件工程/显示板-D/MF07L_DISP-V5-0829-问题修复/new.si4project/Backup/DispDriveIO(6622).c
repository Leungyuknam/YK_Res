#define _Native_DISPDR_H
#include "Include\Include.h"
/**************************************************
*函数名称：void DispPIN_DR(void)	
*函数功能：MCU引脚直接驱动显示
*入口参数：void
*出口参数：void
*描述:包含有使某个灯暗亮的驱动方式
**************************************************/
void DispPIN_DR(void)	
{
	static u8 u8DispCnt,u8ComOn_Cnt;
	u16 u16DispTemp0;
	if(++u8DispCnt >= 4)
	{
		if(FALSE == B_DispUpdating)
		{
			u8DispCnt = 0;	//1MS
			if(++ u8ComOn_Cnt >= COM_MAX_CNT)		
				u8ComOn_Cnt = 0;
			u16DispTemp0 = g_u8DispSeg[u8ComOn_Cnt];  		//高、低亮都会显示的数据
			All_COM_OFF();
			if(u16DispTemp0 & BIT0)
			{
				SEGA_ON;
			}
			else
			{
				SEGA_OFF;
			}
			
			if(u16DispTemp0 & BIT1)
			{
				SEGB_ON;
			}
			else
			{
				SEGB_OFF;
			}

			if(u16DispTemp0 & BIT2)
			{
				SEGC_ON;
			}
			else
			{
				SEGC_OFF;
			}
			
			if(u16DispTemp0 & BIT3)
			{
				SEGD_ON;
			}
			else
			{
				SEGD_OFF;
			}

			if(u16DispTemp0 & BIT4)
			{
				SEGE_ON;
			}
			else
			{
				SEGE_OFF;
			}

			if(u16DispTemp0 & BIT5)
			{
				SEGF_ON;
			}
			else
			{
				SEGF_OFF;
			}
			
			if(u16DispTemp0 & BIT6)
			{
				SEGG_ON;
			}
			else
			{
				SEGG_OFF;
			}

			//if(u16DispTemp0 & BIT7)
			//{
			//	SEGDP_ON;
			//}
			//else
			//{
			//	SEGDP_OFF;
			//}


			switch(u8ComOn_Cnt)
			{
				case 0:
					COM1_ON();
				break;

				case 1:
					COM2_ON();
				break;

				case 2:
					COM3_ON();
				break;

				case 3:
					COM4_ON();
				break;

				case 4:
					COM5_ON();
				break;

				case 5:
					COM6_ON();
				break;

				case 6:
					COM7_ON();
				break;

				default:

				break;
			}
		}
	}
}



void DispLedBufSet(u8 *u8Buf)
{
	u8 i = 0;
	//驱动缓存赋值
	B_DispUpdating = TRUE;
	for (i = 0; i < COM_MAX_CNT; i++)
	{
		g_u8DispSeg[i] = *u8Buf;
		u8Buf ++;
	}
	B_DispUpdating = FALSE;
}

