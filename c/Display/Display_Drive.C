/*********************************************************
文 件 名 : Display_Drive.C
文件说明 : 显示驱动
编写作者 : thm
修订作者 : 
编写时间 : 2021-3-12
版 本 号 : V1.0
==========================================================*/
//头文件
//#include "COMMON\AllInclude.h"
#include "h\Allinclude.h"
//V1.0 TM1640 TM1650
//V1.1 TM1628
//V1.2 TM1668
//V1.3 TM1618

#if TM1640	 

/*********************************************************************************
函数名称				：		F_SendData(unsigned char r_Data)
功能描述				：		发送数据到TM芯片
被本函数调用的函数列表	：		Delay(unsigned char t) 
调用本函数的函数列表	：		F_TM1640_SendCommand(),F_TM1640_Switch(unsigned char r_OnDisplay)，
								F_TM1640_AutoSendAddressData(unsigned char r_Address,unsigned char r_Number)，	
								F_TM1640_SendAddressData(unsigned char r_Address, unsigned char r_Data)
本函数访问的数据表格列表：		NULL
调用本函数的时钟周期	：		
输入参数				：		r_Data
输出参数				：		NULL
返回值					：		return NULL
其他说明				：
*********************************************************************************/
void F_SendData(unsigned char r_Data)
{
    unsigned char i;
    F_CLR_SCLK();									//把时钟线拉低，准备发送数据
    for (i = 0x01; i != 0x00; i <<= 1) 
	{          										//判断是否已经移了8位数据                   
        if( r_Data & i ) 
		{											//判断数据第i位是1还是0	
            F_SET_DIN();							//如果是1，拉高数据线
        }
        else 
		{
            F_CLR_DIN();							 //如果是0，释放数据线
        }
        F_SET_SCLK();									//产生时钟上升沿
        F_SET_SCLK();	
        F_SET_SCLK();	
        F_SET_SCLK();	
        F_CLR_SCLK();							 		//再次把时钟线拉低，准备发送下一位数据	
        F_CLR_SCLK();		
        F_CLR_SCLK();		
        F_CLR_SCLK();		
    }
}

/*********************************************************************************
函数名称				：		F_ENDStart()
功能描述				：		TM1640显示的起始位或者结束位标志
被本函数调用的函数列表	：		
调用本函数的函数列表	：		F_DisplayDrive()
本函数访问的数据表格列表：		NULL
调用本函数的时钟周期	：		
输入参数				：		r_OnDisplay
输出参数				：		NULL
返回值					：		return NULL
其他说明				：
*********************************************************************************/
void F_ENDStart()
{		   
	F_CLR_DIN(); 		
	F_CLR_DIN(); 		
	F_CLR_DIN(); 		
    F_SET_SCLK();						//拉高时钟线
    F_SET_SCLK();			
    F_SET_SCLK();			
    F_SET_SCLK();			
    F_SET_DIN();	 					//再时钟线高电平的时候产生一个下降沿，表示开始传送数据
    F_SET_DIN();	 				
    F_SET_DIN();	 				
    F_SET_DIN();	 				
	F_CLR_DIN(); 		
	F_CLR_DIN(); 		
	F_CLR_DIN(); 		
	F_CLR_DIN(); 
    F_CLR_SCLK();						//释放时钟线
}

/*********************************************************************************
函数名称				：		F_TM1640_SendCommand()
功能描述				：		发送命令到TM芯片
被本函数调用的函数列表	：		F_SendData() 
调用本函数的函数列表	：		F_DisplayDrive()
本函数访问的数据表格列表：		NULL
调用本函数的时钟周期	：		100ms
输入参数				：		NULL
输出参数				：		NULL
返回值					：		return NULL
其他说明				：
*********************************************************************************/
void F_TM1640_SendCommand()
{								  
    F_CLR_SCLK();		
    F_CLR_SCLK();		
    F_CLR_SCLK();		
    F_SendData(0x40);  // 发送数据命令设置	 
    F_CLR_SCLK();		
    F_CLR_SCLK();							    
    F_CLR_SCLK();		
    F_CLR_SCLK();							    
    F_CLR_SCLK();	
}

/*********************************************************************************
函数名称				：		F_TM1640_Switch(unsigned char r_Brightness)
功能描述				：		1640显示开启的开关，在数据发送完毕的后面执行该语句
被本函数调用的函数列表	：		F_SendData()
调用本函数的函数列表	：		F_DisplayDrive()
本函数访问的数据表格列表：		NULL
调用本函数的时钟周期	：		100ms
输入参数				：		r_OnDisplay
输出参数				：		NULL
返回值					：		return NULL
其他说明				：	
*********************************************************************************/
void F_TM1640_Switch(unsigned char r_Brightness)   //为1时开显示，为0时不显示
{   
	F_CLR_SCLK();		
	F_CLR_SCLK();		
    F_SendData(0x88|r_Brightness);	    //设置好显示的亮度，并且打开或者关闭显示
	F_CLR_SCLK();		
	F_CLR_SCLK();		
	F_CLR_SCLK();		
	F_CLR_SCLK();		
	F_CLR_SCLK();		
	F_CLR_SCLK();		
	F_CLR_SCLK();		
	F_CLR_SCLK();		
} // F_TM1640_Switch(unsigned char)


void F_TM1640_AutoSendAddressData(unsigned char r_Address,unsigned char *l_pu8Buf,unsigned char r_Number)
{   
   unsigned char i;
   //发送起始地址
   F_CLR_SCLK();	   
   F_CLR_SCLK();							   
   F_SendData(0xC0 + r_Address);		   //发送连续数据存放的首地址
   ////////////////////////
   F_CLR_SCLK();	   
   F_CLR_SCLK();	   
   /////////////////////////////////////
   //发送需要显示的数据，采用循环发送的方式
   for(i=0;i<r_Number;i++)
   {
	  F_SendData(l_pu8Buf[i]);	   //连续发送i个显示缓冲区数据到芯片内部地址
   }
   F_CLR_SCLK();	   
   F_CLR_SCLK();	   
   F_CLR_SCLK();	   
   F_CLR_SCLK();	   
   F_CLR_SCLK();	   
   F_CLR_SCLK();	   
   F_CLR_SCLK();	   
   F_CLR_SCLK();	   
   //等待显示的开关打开	  
}

void F_1640Driver(unsigned char *l_pu8Buf,unsigned char l_u8Number,unsigned char l_u8Brightness) 
{
	F_ENDStart();						
	F_TM1640_SendCommand();	  
	F_ENDStart();
	F_TM1640_AutoSendAddressData(0,l_pu8Buf,l_u8Number);
	F_ENDStart();
	F_TM1640_Switch(l_u8Brightness);	  
	F_ENDStart();	

}

#endif


