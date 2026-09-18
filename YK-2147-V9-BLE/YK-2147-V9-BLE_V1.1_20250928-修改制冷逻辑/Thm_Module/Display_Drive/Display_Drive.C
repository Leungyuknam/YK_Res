/*********************************************************
文 件 名 : Display_Drive.C
文件说明 : 显示驱动
编写作者 : thm
修订作者 : 
编写时间 : 2021-3-12
版 本 号 : V1.0
==========================================================*/
//头文件
#include "COMMON\AllInclude.h"

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


#if TM1650//2020.7.31双显示

void delay_1(uint8 n)
{	while(n--){	_nop_();_nop_();	} 
}

/************ START信号*******************************/
void TM1650_START_1()
{
	SDAOut_1();
	SCLOut_1();
	delay_1(1);
	SCL_1=1;
	delay_1(1);
	SDA_1=1;
	delay_1(5);
	SDA_1=0;
	delay_1(1);
}
/******************** STOP信号************************/
void TM1650_STOP_1()
{
	SDAOut_1();
	SCLOut_1();
	SCL_1=1;
	SDA_1=0;
	delay_1(5);
	SDA_1=1;
	delay_1(1);
	SCL_1=1;
}
/****************写1个字节给TM1650********************/
void write_8bit_1(uint8 dat)
{
 	uint8 i;
	SCL_1=0;
	SDAOut_1();
	SCLOut_1();
	delay_1(1);
	for(i=0;i<8;i++)
	{	if(dat&0x80)
		{
			SDA_1=1;
			delay_1(1);
			SCL_1=1;
			delay_1(5);
			SCL_1=0;	 
		}
		else
		{
			SDA_1=0;
			delay_1(1);
			SCL_1=1;
			delay_1(5);
			SCL_1=0;
		}	
		dat<<=1;	 
	}
	SDA_1=0;			//ACK信号
	delay_1(3);
	SCL_1=1;
	delay_1(5);
	SCL_1=0;
	delay_1(1);	 
}

///**********************读8bit**************************/
//uint8 read_8bit()
//{
//	uint8 dat,i;
//
//	SDAOut();
//	SCLOut();
//	delay(1);
//	SDA=1;
//	SDAIn();
//	delay(1);
//	for(i=0;i<8;i++)
//	{
//		SCL=1;                        //时钟上沿
//		delay(1);
//		dat<<=1;
//		if(P_SDA)
//		{	dat |= 0x01;
//		}
//		SCL=0;
//		delay(5);
//	}
//	SDA=0;			    //ACK信号
//	delay(2);
//	SCL=1;
//	delay(5);
//	SCL=0;
//	delay(1);
//	
//	return dat ;
//} 
// 
//// /*******************读按键命令************************/
//uint8 TM1650_read()
//{
//	uint8 key;
//	TM1650_START();
//    write_8bit(c_READ_KEY_MODE);//读按键指令	
//	key = read_8bit();
//	TM1650_STOP();
//	return key;
//} 

/*****************发送命令信号***********************/
void TM1650_send_1(uint8 date1,uint8 date2)
{
 	TM1650_START_1();
	write_8bit_1(date1);
	write_8bit_1(date2);
	TM1650_STOP_1();
}


void F_1650Driver_1(unsigned char *l_pu8Buf,unsigned char l_u8Brightness)
{
	
#define	c_DISPLAY_MODE				0x48	//显示模式命令
#define	c_READ_KEY_MODE				0x4f	//按键读取模式命令	 
	
#define	c_CFG_DISP_MODE_SETTING		0x00	//0x00代表8段显示	0x08代表7段显示		
#define	c_CFG_DISP_SWITCH			0x01	//0x01代表开显示	0x00代表关显示

#define c_DISPLAY_ADDRESS1			0x68
#define c_DISPLAY_ADDRESS2			0x6A
#define c_DISPLAY_ADDRESS3			0x6C
#define c_DISPLAY_ADDRESS4			0x6E

	TM1650_send_1(c_DISPLAY_MODE,(l_u8Brightness<<4)|(c_CFG_DISP_MODE_SETTING<<3)|(c_CFG_DISP_SWITCH<<0));
	TM1650_send_1(c_DISPLAY_ADDRESS1,l_pu8Buf[0]);
	TM1650_send_1(c_DISPLAY_ADDRESS2,l_pu8Buf[1]);
	TM1650_send_1(c_DISPLAY_ADDRESS3,l_pu8Buf[2]);
	TM1650_send_1(c_DISPLAY_ADDRESS4,l_pu8Buf[3]); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//2020-12-9 新增  tang
void delay_2(uint8 n)
{	while(n--){	_nop_();_nop_();	} 
}

/************ START信号*******************************/
void TM1650_START_2()
{
	SDAOut_2();
	SCLOut_2();
	delay_2(1);
	SCL_2=1;
	delay_2(1);
	SDA_2=1;
	delay_2(5);
	SDA_2=0;
	delay_2(1);
}
/******************** STOP信号************************/
void TM1650_STOP_2()
{
	SDAOut_2();
	SCLOut_2();
	SCL_2=1;
	SDA_2=0;
	delay_2(5);
	SDA_2=1;
	delay_2(1);
	SCL_2=1;
}
/****************写1个字节给TM1650********************/
void write_8bit_2(uint8 dat)
{
 	uint8 i;
	SCL_2=0;
	SDAOut_2();
	SCLOut_2();
	delay_2(1);
	for(i=0;i<8;i++)
	{	if(dat&0x80)
		{
			SDA_2=1;
			delay_2(1);
			SCL_2=1;
			delay_2(5);
			SCL_2=0;	 
		}
		else
		{
			SDA_2=0;
			delay_2(1);
			SCL_2=1;
			delay_2(5);
			SCL_2=0;
		}	
		dat<<=1;	 
	}
	SDA_2=0;			//ACK信号
	delay_2(3);
	SCL_2=1;
	delay_2(5);
	SCL_2=0;
	delay_2(1);	 
}

/*****************发送命令信号***********************/
void TM1650_send_2(uint8 date1,uint8 date2)
{
 	TM1650_START_2();
	write_8bit_2(date1);
	write_8bit_2(date2);
	TM1650_STOP_2();
}


void F_1650Driver_2(unsigned char *l_pu8Buf,unsigned char l_u8Brightness)
{
	
#define	c_DISPLAY_MODE				0x48	//显示模式命令
#define	c_READ_KEY_MODE				0x4f	//按键读取模式命令	 
	
#define	c_CFG_DISP_MODE_SETTING		0x00	//0x00代表8段显示	0x08代表7段显示		
#define	c_CFG_DISP_SWITCH			0x01	//0x01代表开显示	0x00代表关显示

#define c_DISPLAY_ADDRESS1			0x68
#define c_DISPLAY_ADDRESS2			0x6A
#define c_DISPLAY_ADDRESS3			0x6C
#define c_DISPLAY_ADDRESS4			0x6E

	TM1650_send_2(c_DISPLAY_MODE,(l_u8Brightness<<4)|(c_CFG_DISP_MODE_SETTING<<3)|(c_CFG_DISP_SWITCH<<0));
	TM1650_send_2(c_DISPLAY_ADDRESS1,l_pu8Buf[0]);
	TM1650_send_2(c_DISPLAY_ADDRESS2,l_pu8Buf[0]);
	TM1650_send_2(c_DISPLAY_ADDRESS3,l_pu8Buf[2]);
	TM1650_send_2(c_DISPLAY_ADDRESS4,l_pu8Buf[2]);
	
}


#endif


#if TM1628
void F_SendData(unsigned char r_Data)
{
    unsigned char i;
    F_CLR_SCLK();											//把时钟线拉低，准备发送数据
    for( i = 0x01; i != 0x00; i <<= 1 )
	{               							 			//判断是否已经移了8位数据
         if( r_Data & i )
		 {													//判断数据第i位是1还是0
             F_SET_DIO();									//如果是1，拉高数据线
         }
         else
		 {
             F_CLR_DIO();									//如果是0，释放数据线
         }
         F_SET_SCLK();										//产生时钟上升沿
         F_SET_SCLK();										//延时
		 F_SET_SCLK();										//延时
         F_CLR_SCLK();										//再次把时钟线拉低，准备发送下一位数据
		 F_CLR_SCLK();										//延时
		 F_CLR_SCLK();										//延时
    }
}

void F_TM1628_SendCommand()
{
    F_SET_STB();								//置位片选管脚，再清零，产生一个下降沿，初始化串行接口
    F_SET_STB();								//延时
	F_SET_STB();								//延时
    F_CLR_STB();								//释放片选管脚，表示开始发送数据
	F_CLR_STB();								//延时
	F_CLR_STB();								//延时
    F_SendData(CFG_DISP_MODE_SETTING);    // 发送显示模式设置
    F_SET_STB();
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();							   	//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
    F_CLR_STB();
    F_SendData(CFG_DATA_CMD_SETTING);  	// 发送数据命令设置
    F_SET_STB();
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();							   	//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
}

void F_TM1628_AutoSendAddressData(unsigned char r_Address,unsigned int *l_pu16Buf,unsigned char r_Number)
{
    unsigned char i;
    //发送起始地址
    F_SET_STB();				   			//置位片选管脚，再清零，产生一个下降沿，初始化串行接口
	F_SET_STB();								//延时
	F_SET_STB();								//延时
    F_CLR_STB();				  	 		//释放片选管脚，表示开始发送数据
    F_SendData(0xC0 + r_Address);			//发送连续数据存放的首地址
    ////////////////////////
	F_CLR_SCLK();										//延时
	F_CLR_SCLK();										//延时
    /////////////////////////////////////
    //发送需要显示的数据，采用循环发送的方式
    for( i = 0; i < r_Number; i++ )
    {    F_SendData(l_pu16Buf[i]);		//连续发送i个显示缓冲区数据到芯片内部地址
    	 F_SendData((l_pu16Buf[i]>>8));
	}
    /////////////////////////////////////
	F_SET_STB();								//回到忙状态
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();							   	//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
    //等待显示的开关打开
}

void F_TM1628_Switch(unsigned char r_OnDisplay)
{
    F_SET_STB();
	F_SET_STB();								//延时
	F_SET_STB();							   	//延时
    F_CLR_STB();
    F_SendData((0x80 + r_OnDisplay) | (1 << 3));//设置好显示的亮度，并且打开或者关闭显示
    F_SET_STB();
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();							   	//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
} 


//void F_ReadKey(unsigned char *KeyBuf)
//{
//	unsigned char i,j;
//	F_SET_STB();								//置位片选管脚，再清零，产生一个下降沿，初始化串行接口
//    F_SET_STB();								//延时
//	F_SET_STB();								//延时
//    F_CLR_STB();								//释放片选管脚，表示开始发送数据
//	F_CLR_STB();								//延时
//	F_CLR_STB();								//延时
//	F_SendData(0x42);							//发送读取按键命令
//	
//	F_SET_DIO();
//	F_DIO_IN();	  
//	F_CLR_SCLK();							
//	F_CLR_SCLK();							//延时	
//
//
//	F_CLR_SCLK();							 
//	F_CLR_SCLK();
//	F_CLR_SCLK();							 
//	F_CLR_SCLK();
//	
//	
//	for(j=0;j<5;j++)
//	{
//		KeyBuf[j] = 0;
//		for(i=0;i<8;i++)
//		{
//			KeyBuf[j] = KeyBuf[j]>>1;
//			if( F_TEST_DIO() )
//			{
//				KeyBuf[j] = KeyBuf[j]|0x80;	
//			}	
//			F_SET_SCLK();							//产生时钟上升沿
//			F_SET_SCLK();							//延时	
//			F_SET_SCLK();							//延时	 
//			F_SET_SCLK();							//延时	 
//			F_CLR_SCLK();							 
//			F_CLR_SCLK();							 
//			F_CLR_SCLK();							
//			F_CLR_SCLK();							//延时	
//		}
//	}
//	F_DIO_OUT();
//    F_SET_STB();
//	F_SET_STB();								//延时
//    F_SET_STB();
//	F_SET_STB();								//延时
//
//}

void F_1628Driver(unsigned int *l_pu16Buf,unsigned char l_u8Number,unsigned char l_u8Brightness)
{

	F_TM1628_SendCommand(); 						//发送显示模式命令
	F_TM1628_AutoSendAddressData(0x00,l_pu16Buf,l_u8Number);			//送数据到显示数据区
	F_TM1628_Switch(l_u8Brightness);

}

#endif


#if TM1668	
/*********************************************************************************
函数名称				：		F_SendData(unsigned char r_Data)
功能描述				：		发送数据到TM芯片
被本函数调用的函数列表	：
调用本函数的函数列表	：		F_TM1668_SendCommand(),F_TM1668_Switch(unsigned char r_OnDisplay)，
								F_TM1668_AutoSendAddressData(unsigned char r_Address,unsigned char r_Number)，
								F_TM1668_SendAddressData(unsigned char r_Address, unsigned char r_Data)
本函数访问的数据表格列表：		NULL
调用本函数的时钟周期	：
输入参数				：		r_Data
输出参数				：		NULL
返回值					：		return NULL
其他说明				：		无需改动
*********************************************************************************/
static void F_SendData(unsigned char r_Data)
{
    unsigned char i;
    F_CLR_SCLK();											//把时钟线拉低，准备发送数据
    for( i = 0x01; i != 0x00; i <<= 1 )
	{               							 			//判断是否已经移了8位数据
         if( r_Data & i )
		 {													//判断数据第i位是1还是0
             F_SET_DIO();									//如果是1，拉高数据线
         }
         else
		 {
             F_CLR_DIO();									//如果是0，释放数据线
         }
         F_SET_SCLK();										//产生时钟上升沿
         F_SET_SCLK();										//延时
		 F_SET_SCLK();										//延时
         F_CLR_SCLK();										//再次把时钟线拉低，准备发送下一位数据
		 F_CLR_SCLK();										//延时
		 F_CLR_SCLK();										//延时
    }
}

/*********************************************************************************
函数名称				：		F_TM1668B_SendCommand()
功能描述				：		发送命令到TM芯片
被本函数调用的函数列表	：		F_SendData()
调用本函数的函数列表	：		F_DisplayDrive()
本函数访问的数据表格列表：		NULL
调用本函数的时钟周期	：		100ms
输入参数				：		NULL
输出参数				：		NULL
返回值					：		return NULL
其他说明				：		函数内部无需改动，如果发送显示模式有变，只需要再上面改动：
								CFG_DISP_MODE_SETTING和CFG_DATA_CMD_SETTING	  两个宏定义常量
*********************************************************************************/
void F_TM1668_SendCommand()
{
    F_SET_STB();								//置位片选管脚，再清零，产生一个下降沿，初始化串行接口
    F_SET_STB();								//延时
	F_SET_STB();								//延时
    F_CLR_STB();								//释放片选管脚，表示开始发送数据
	F_CLR_STB();								//延时
	F_CLR_STB();								//延时
    F_SendData(CFG_DISP_MODE_SETTING);    // 发送显示模式设置
    F_SET_STB();
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();							   	//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
    F_CLR_STB();
    F_SendData(CFG_DATA_CMD_SETTING);  	// 发送数据命令设置
    F_SET_STB();
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();							   	//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
}

/*********************************************************************************
函数名称				：		F_TM1668_AutoSendAddressData(unsigned char r_Address,unsigned char r_Number)
功能描述				：		先发送再连续发送地址到TM芯片，地址自动加一
被本函数调用的函数列表	：		F_SendData()
调用本函数的函数列表	：		F_DisplayDrive()
本函数访问的数据表格列表：		r_DisplayBUF
调用本函数的时钟周期	：		100ms
输入参数				：		r_Address,r_Number
输出参数				：		NULL
返回值					：		return NULL
其他说明				：		发送地址加N个连续数据，都无需改动函数内容
*********************************************************************************/
void F_TM1668_AutoSendAddressData(unsigned char r_Address,unsigned int *l_pu16Buf,unsigned char r_Number)
{
    unsigned char i;
    //发送起始地址
    F_SET_STB();				   			//置位片选管脚，再清零，产生一个下降沿，初始化串行接口
	F_SET_STB();								//延时
	F_SET_STB();								//延时
    F_CLR_STB();				  	 		//释放片选管脚，表示开始发送数据
    F_SendData(0xC0 + r_Address);			//发送连续数据存放的首地址
    ////////////////////////
	F_CLR_SCLK();										//延时
	F_CLR_SCLK();										//延时
    /////////////////////////////////////
    //发送需要显示的数据，采用循环发送的方式
    for( i = 0; i < r_Number; i++ )
    {    F_SendData(l_pu16Buf[i]);		//连续发送i个显示缓冲区数据到芯片内部地址
    	 F_SendData((l_pu16Buf[i]>>8));
    }
    /////////////////////////////////////
	F_SET_STB();								//回到忙状态
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();							   	//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
    //等待显示的开关打开
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

/*********************************************************************************
函数名称				：		F_TM1668_Switch(unsigned char r_OnDisplay)
功能描述				：		1668显示开启的开关，在数据发送完毕的后面执行该语句
被本函数调用的函数列表	：		F_SendData()
调用本函数的函数列表	：		F_DisplayDrive()
本函数访问的数据表格列表：		NULL
调用本函数的时钟周期	：		100ms
输入参数				：		r_OnDisplay
输出参数				：		NULL
返回值					：		return NULL
其他说明				：		当r_OnDisplay为1时打开显示，为0时关闭显示，如果需要调整亮度，
								只需要改变宏定义常量：CFG_BRIGHTNESS   不过一般不设置成最亮
*********************************************************************************/
void F_TM1668_Switch(unsigned char r_OnDisplay)
{
    F_SET_STB();
	F_SET_STB();								//延时
	F_SET_STB();							   	//延时
    F_CLR_STB();
    F_SendData((0x80 + r_OnDisplay) | (1 << 3));//设置好显示的亮度，并且打开或者关闭显示
    F_SET_STB();
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();							   	//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
} // TM1668_Switch(unsigned char)
//////////////////////////////////////////////////////////////////



void F_1668Driver(unsigned int *l_pu16Buf,unsigned char l_u8Number,unsigned char l_u8Brightness)
{

	F_TM1668_SendCommand(); 						//发送显示模式命令
	F_TM1668_AutoSendAddressData(0x00,l_pu16Buf,l_u8Number);			//送数据到显示数据区
	F_TM1668_Switch(l_u8Brightness);

}

#endif



#if TM1618	
/*********************************************************************************
函数名称				：		F_SendData(unsigned char r_Data)
功能描述				：		发送数据到TM芯片
被本函数调用的函数列表	：
调用本函数的函数列表	：		F_TM1668_SendCommand(),F_TM1668_Switch(unsigned char r_OnDisplay)，
								F_TM1668_AutoSendAddressData(unsigned char r_Address,unsigned char r_Number)，
								F_TM1668_SendAddressData(unsigned char r_Address, unsigned char r_Data)
本函数访问的数据表格列表：		NULL
调用本函数的时钟周期	：
输入参数				：		r_Data
输出参数				：		NULL
返回值					：		return NULL
其他说明				：		无需改动
*********************************************************************************/
static void F_SendData(unsigned char r_Data)
{
    unsigned char i;
    F_CLR_SCLK();											//把时钟线拉低，准备发送数据
    for( i = 0x01; i != 0x00; i <<= 1 )
	{               							 			//判断是否已经移了8位数据
         if( r_Data & i )
		 {													//判断数据第i位是1还是0
             F_SET_DIO();									//如果是1，拉高数据线
         }
         else
		 {
             F_CLR_DIO();									//如果是0，释放数据线
         }
         F_SET_SCLK();										//产生时钟上升沿
         F_SET_SCLK();										//延时
		 F_SET_SCLK();										//延时
         F_CLR_SCLK();										//再次把时钟线拉低，准备发送下一位数据
		 F_CLR_SCLK();										//延时
		 F_CLR_SCLK();										//延时
    }
}

/*********************************************************************************
函数名称				：		F_TM1668B_SendCommand()
功能描述				：		发送命令到TM芯片
被本函数调用的函数列表	：		F_SendData()
调用本函数的函数列表	：		F_DisplayDrive()
本函数访问的数据表格列表：		NULL
调用本函数的时钟周期	：		100ms
输入参数				：		NULL
输出参数				：		NULL
返回值					：		return NULL
其他说明				：		函数内部无需改动，如果发送显示模式有变，只需要再上面改动：
								CFG_DISP_MODE_SETTING和CFG_DATA_CMD_SETTING	  两个宏定义常量
*********************************************************************************/
void F_TM1618_SendCommand()
{
    F_SET_STB();								//置位片选管脚，再清零，产生一个下降沿，初始化串行接口
    F_SET_STB();								//延时
	F_SET_STB();								//延时
    F_CLR_STB();								//释放片选管脚，表示开始发送数据
	F_CLR_STB();								//延时
	F_CLR_STB();								//延时
    F_SendData(CFG_DISP_MODE_SETTING);    // 发送显示模式设置
    F_SET_STB();
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();							   	//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
    F_CLR_STB();
    F_SendData(CFG_DATA_CMD_SETTING);  	// 发送数据命令设置
    F_SET_STB();
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();							   	//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
}

/*********************************************************************************
函数名称				：		F_TM1668_AutoSendAddressData(unsigned char r_Address,unsigned char r_Number)
功能描述				：		先发送再连续发送地址到TM芯片，地址自动加一
被本函数调用的函数列表	：		F_SendData()
调用本函数的函数列表	：		F_DisplayDrive()
本函数访问的数据表格列表：		r_DisplayBUF
调用本函数的时钟周期	：		100ms
输入参数				：		r_Address,r_Number
输出参数				：		NULL
返回值					：		return NULL
其他说明				：		发送地址加N个连续数据，都无需改动函数内容
*********************************************************************************/
void F_TM1618_AutoSendAddressData(unsigned char r_Address,unsigned int *l_pu16Buf,unsigned char r_Number)
{
    unsigned char i;
    //发送起始地址
    F_SET_STB();				   			//置位片选管脚，再清零，产生一个下降沿，初始化串行接口
	F_SET_STB();								//延时
	F_SET_STB();								//延时
    F_CLR_STB();				  	 		//释放片选管脚，表示开始发送数据
    F_SendData(0xC0 + r_Address);			//发送连续数据存放的首地址
    ////////////////////////
	F_CLR_SCLK();										//延时
	F_CLR_SCLK();										//延时
    /////////////////////////////////////
    //发送需要显示的数据，采用循环发送的方式
    for( i = 0; i < r_Number; i++ )
    {    F_SendData(l_pu16Buf[i]);		//连续发送i个显示缓冲区数据到芯片内部地址
    	 F_SendData((l_pu16Buf[i]>>8));
    }
    /////////////////////////////////////
	F_SET_STB();								//回到忙状态
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();							   	//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
    //等待显示的开关打开
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

/*********************************************************************************
函数名称				：		F_TM1668_Switch(unsigned char r_OnDisplay)
功能描述				：		1668显示开启的开关，在数据发送完毕的后面执行该语句
被本函数调用的函数列表	：		F_SendData()
调用本函数的函数列表	：		F_DisplayDrive()
本函数访问的数据表格列表：		NULL
调用本函数的时钟周期	：		100ms
输入参数				：		r_OnDisplay
输出参数				：		NULL
返回值					：		return NULL
其他说明				：		当r_OnDisplay为1时打开显示，为0时关闭显示，如果需要调整亮度，
								只需要改变宏定义常量：CFG_BRIGHTNESS   不过一般不设置成最亮
*********************************************************************************/
void F_TM1618_Switch(unsigned char r_OnDisplay)
{
    F_SET_STB();
	F_SET_STB();								//延时
	F_SET_STB();							   	//延时
    F_CLR_STB();
    F_SendData((0x80 + r_OnDisplay) | (1 << 3));//设置好显示的亮度，并且打开或者关闭显示
    F_SET_STB();
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();							   	//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
	F_SET_STB();								//延时
} // TM1668_Switch(unsigned char)
//////////////////////////////////////////////////////////////////



void F_1618Driver(unsigned int *l_pu16Buf,unsigned char l_u8Number,unsigned char l_u8Brightness)
{

	F_TM1618_SendCommand(); 						//发送显示模式命令
	F_TM1618_AutoSendAddressData(0x00,l_pu16Buf,l_u8Number);			//送数据到显示数据区
	F_TM1618_Switch(l_u8Brightness);

}

#endif

