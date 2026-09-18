/*********************************************************
文 件 名 : DigitalLedDisplay.C
文件说明 : 数码管显示
编写作者 : thm
修订作者 : 
编写时间 : 2021-12-23
版 本 号 : V1.4
==========================================================*/
//头文件
#include "COMMON\AllInclude.h"

//全局变量声明
unsigned int   xdata  g_u16DisPrimaryBuff_TABLE[DIGITAL_COM_NUMBERS] = {0};	//初级缓存
static unsigned int   xdata  g_u16DisDrive_TABLE[DIGITAL_COM_NUMBERS] = {0};	    //驱动数据
static unsigned char  xdata  g_u8DispLock = C_DISP_UNLOCK;

code unsigned char C_NUMBER_TABLE[10] = 
{
    C_LABLE_NUM0,
    C_LABLE_NUM1,
    C_LABLE_NUM2,
    C_LABLE_NUM3,
    C_LABLE_NUM4,
    C_LABLE_NUM5,
    C_LABLE_NUM6,
    C_LABLE_NUM7,
    C_LABLE_NUM8,
    C_LABLE_NUM9    
};

/********************************************************************
*函数名：void NUMBER_Discretize(u16 l_u16InputValue,u8 *pTable)
*输入参数：待分解的数字0-65535
*输出参数：无
*调用周期：无
*函数说明：

    -----       -----       -----       -----       -----
   |     |     |     |     |     |     |     |     |     |
   |     |     |     |     |     |     |     |     |     |
    -----       -----       -----       -----       -----
   |     |     |     |     |     |     |     |     |     |
   |     |     |     |     |     |     |     |     |     |
    -----       -----       -----       -----       -----
    第1位       第2位       第3位       第4位       第5位

第1位数据是数码管最左边第一位的数据
从左往右，依次类推。

高位数据如果为0，则不显示。
/*******************************************************************/
   
void F_NUMBER_Discretize(unsigned int l_u16InputValue)
{
    #if (C_DISPLAY_LENGTH == C_DIGITAL_5)
    {   

        if (l_u16InputValue >= 10000)
        {(g_u16DisPrimaryBuff_TABLE[C_FIRST_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 10000)];}
        else
        {(g_u16DisPrimaryBuff_TABLE[C_FIRST_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 1000)
        {
            if (l_u16InputValue >= 10000)
            {(g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM]) |= C_NUMBER_TABLE[((l_u16InputValue / 1000) % 10)];}
            else
            {(g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 1000)];}
        }
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 100)
        {
            if (l_u16InputValue >= 1000)
            {(g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM]) |= C_NUMBER_TABLE[((l_u16InputValue % 1000) / 100)];}
            else
            {(g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 100)];}
        }
        else
        {(g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {
            if (l_u16InputValue >= 100)
            {(g_u16DisPrimaryBuff_TABLE[C_FOURTH_NUM]) |= C_NUMBER_TABLE[((l_u16InputValue % 100) / 10)];}
            else
            {(g_u16DisPrimaryBuff_TABLE[C_FOURTH_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 10)];}
        }
        else
        {(g_u16DisPrimaryBuff_TABLE[C_FOURTH_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {(g_u16DisPrimaryBuff_TABLE[C_FIFTH_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue % 10)];}
        else
        {(g_u16DisPrimaryBuff_TABLE[C_FIFTH_NUM]) |= C_NUMBER_TABLE[l_u16InputValue];}
    }        
    #endif
    

    #if (C_DISPLAY_LENGTH == C_DIGITAL_4)
    {    
		if (l_u16InputValue > 9999)
		{l_u16InputValue = 9999;}
			       
        if (l_u16InputValue >= 1000)
        {(g_u16DisPrimaryBuff_TABLE[C_FIRST_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 1000)];}
        else
        {(g_u16DisPrimaryBuff_TABLE[C_FIRST_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 100)
        {
            if (l_u16InputValue >= 1000)
            {(g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM]) |= C_NUMBER_TABLE[((l_u16InputValue % 1000) / 100)];}
            else
            {(g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 100)];}
        }
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {
            if (l_u16InputValue >= 100)
            {(g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM]) |= C_NUMBER_TABLE[((l_u16InputValue % 100) / 10)];}
            else
            {(g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 10)];}
        }
        else
        {(g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {(g_u16DisPrimaryBuff_TABLE[C_FOURTH_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue % 10)];}
        else
        {(g_u16DisPrimaryBuff_TABLE[C_FOURTH_NUM]) |= C_NUMBER_TABLE[l_u16InputValue];}
    }        
    #endif
    
    #if (C_DISPLAY_LENGTH == C_DIGITAL_3)
    {           
		if (l_u16InputValue > 999)
		{l_u16InputValue = 999;}

        if (l_u16InputValue >= 100)
        {g_u16DisPrimaryBuff_TABLE[C_FIRST_NUM] |= C_NUMBER_TABLE[((l_u16InputValue / 100))];}
        else
        {g_u16DisPrimaryBuff_TABLE[C_FIRST_NUM] |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {
            if (l_u16InputValue >= 100)
            {g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_NUMBER_TABLE[((l_u16InputValue % 100) / 10)];}
            else
            {g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_NUMBER_TABLE[(l_u16InputValue / 10)];}
        }
        else
        {g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_NUMBER_TABLE[(l_u16InputValue % 10)];}
        else
        {g_u16DisPrimaryBuff_TABLE[C_THIRD_NUM] |= C_NUMBER_TABLE[l_u16InputValue];}
    }        
    #endif

    
    #if (C_DISPLAY_LENGTH == C_DIGITAL_2)
    {             
		if (l_u16InputValue > 99)
		{l_u16InputValue = 99;}
			     
        if (l_u16InputValue >= 10)
        {g_u16DisPrimaryBuff_TABLE[C_FIRST_NUM] |= C_NUMBER_TABLE[(l_u16InputValue / 10)];}
        else
        {g_u16DisPrimaryBuff_TABLE[C_FIRST_NUM] |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_NUMBER_TABLE[(l_u16InputValue % 10)];}
        else
        {g_u16DisPrimaryBuff_TABLE[C_SECOND_NUM] |= C_NUMBER_TABLE[l_u16InputValue];}
    }        
    #endif
    
    
}


/********************************************************************
*函数名：void F_NUMBER_Discretize_Slave1(u16 l_u16InputValue)
*输入参数：待分解的数字0-65535
*输出参数：无
*调用周期：无
*函数说明：副显数字离散化
/*******************************************************************/
void F_NUMBER_Discretize_Slave1(unsigned int l_u16InputValue)
{
    #if (C_SLAVE1_DISPLAY_LENGTH == C_DIGITAL_5)
    {   

        if (l_u16InputValue >= 10000)
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_FIRST_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 10000)];}
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_FIRST_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 1000)
        {
            if (l_u16InputValue >= 10000)
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_SECOND_NUM]) |= C_NUMBER_TABLE[((l_u16InputValue / 1000) % 10)];}
            else
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_SECOND_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 1000)];}
        }
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_SECOND_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 100)
        {
            if (l_u16InputValue >= 1000)
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_THIRD_NUM]) |= C_NUMBER_TABLE[((l_u16InputValue % 1000) / 100)];}
            else
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_THIRD_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 100)];}
        }
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_THIRD_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {
            if (l_u16InputValue >= 100)
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_FOURTH_NUM]) |= C_NUMBER_TABLE[((l_u16InputValue % 100) / 10)];}
            else
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_FOURTH_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 10)];}
        }
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_FOURTH_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_FIFTH_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue % 10)];}
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_FIFTH_NUM]) |= C_NUMBER_TABLE[l_u16InputValue];}
    }        
    #endif
    

    #if (C_SLAVE1_DISPLAY_LENGTH == C_DIGITAL_4)
    {    
		if (l_u16InputValue > 9999)
		{l_u16InputValue = 9999;}
			       
        if (l_u16InputValue >= 1000)
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_FIRST_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 1000)];}
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_FIRST_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 100)
        {
            if (l_u16InputValue >= 1000)
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_SECOND_NUM]) |= C_NUMBER_TABLE[((l_u16InputValue % 1000) / 100)];}
            else
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_SECOND_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 100)];}
        }
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_SECOND_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {
            if (l_u16InputValue >= 100)
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_THIRD_NUM]) |= C_NUMBER_TABLE[((l_u16InputValue % 100) / 10)];}
            else
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_THIRD_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 10)];}
        }
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_THIRD_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_FOURTH_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue % 10)];}
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE1_FOURTH_NUM]) |= C_NUMBER_TABLE[l_u16InputValue];}
    }        
    #endif
    
    #if (C_SLAVE1_DISPLAY_LENGTH == C_DIGITAL_3)
    {           
		if (l_u16InputValue > 999)
		{l_u16InputValue = 999;}

        if (l_u16InputValue >= 100)
        {g_u16DisPrimaryBuff_TABLE[C_SLAVE1_FIRST_NUM] |= C_NUMBER_TABLE[((l_u16InputValue / 100))];}
        else
        {g_u16DisPrimaryBuff_TABLE[C_SLAVE1_FIRST_NUM] |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {
            if (l_u16InputValue >= 100)
            {g_u16DisPrimaryBuff_TABLE[C_SLAVE1_SECOND_NUM] |= C_NUMBER_TABLE[((l_u16InputValue % 100) / 10)];}
            else
            {g_u16DisPrimaryBuff_TABLE[C_SLAVE1_SECOND_NUM] |= C_NUMBER_TABLE[(l_u16InputValue / 10)];}
        }
        else
        {g_u16DisPrimaryBuff_TABLE[C_SLAVE1_SECOND_NUM] |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {g_u16DisPrimaryBuff_TABLE[C_SLAVE1_THIRD_NUM] |= C_NUMBER_TABLE[(l_u16InputValue % 10)];}
        else
        {g_u16DisPrimaryBuff_TABLE[C_SLAVE1_THIRD_NUM] |= C_NUMBER_TABLE[l_u16InputValue];}
    }        
    #endif

    
    #if (C_SLAVE1_DISPLAY_LENGTH == C_DIGITAL_2)
    {             
		if (l_u16InputValue > 99)
		{l_u16InputValue = 99;}
			     
        if (l_u16InputValue >= 10)
        {g_u16DisPrimaryBuff_TABLE[C_SLAVE1_FIRST_NUM] |= C_NUMBER_TABLE[(l_u16InputValue / 10)];}
        else
        {g_u16DisPrimaryBuff_TABLE[C_SLAVE1_FIRST_NUM] |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {g_u16DisPrimaryBuff_TABLE[C_SLAVE1_SECOND_NUM] |= C_NUMBER_TABLE[(l_u16InputValue % 10)];}
        else
        {g_u16DisPrimaryBuff_TABLE[C_SLAVE1_SECOND_NUM] |= C_NUMBER_TABLE[l_u16InputValue];}
    }        
    #endif
   
}


/********************************************************************
*函数名：void F_NUMBER_Discretize_Slave2(u16 l_u16InputValue)
*输入参数：待分解的数字0-65535
*输出参数：无
*调用周期：无
*函数说明：副显数字离散化
/*******************************************************************/
void F_NUMBER_Discretize_Slave2(unsigned int l_u16InputValue)
{
    #if (C_SLAVE2_DISPLAY_LENGTH == C_DIGITAL_5)
    {   

        if (l_u16InputValue >= 10000)
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_FIRST_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 10000)];}
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_FIRST_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 1000)
        {
            if (l_u16InputValue >= 10000)
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_SECOND_NUM]) |= C_NUMBER_TABLE[((l_u16InputValue / 1000) % 10)];}
            else
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_SECOND_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 1000)];}
        }
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_SECOND_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 100)
        {
            if (l_u16InputValue >= 1000)
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_THIRD_NUM]) |= C_NUMBER_TABLE[((l_u16InputValue % 1000) / 100)];}
            else
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_THIRD_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 100)];}
        }
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_THIRD_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {
            if (l_u16InputValue >= 100)
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_FOURTH_NUM]) |= C_NUMBER_TABLE[((l_u16InputValue % 100) / 10)];}
            else
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_FOURTH_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 10)];}
        }
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_FOURTH_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_FIFTH_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue % 10)];}
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_FIFTH_NUM]) |= C_NUMBER_TABLE[l_u16InputValue];}
    }        
    #endif
    

    #if (C_SLAVE2_DISPLAY_LENGTH == C_DIGITAL_4)
    {    
		if (l_u16InputValue > 9999)
		{l_u16InputValue = 9999;}
			       
        if (l_u16InputValue >= 1000)
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_FIRST_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 1000)];}
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_FIRST_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 100)
        {
            if (l_u16InputValue >= 1000)
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_SECOND_NUM]) |= C_NUMBER_TABLE[((l_u16InputValue % 1000) / 100)];}
            else
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_SECOND_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 100)];}
        }
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_SECOND_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {
            if (l_u16InputValue >= 100)
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_THIRD_NUM]) |= C_NUMBER_TABLE[((l_u16InputValue % 100) / 10)];}
            else
            {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_THIRD_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue / 10)];}
        }
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_THIRD_NUM]) |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_FOURTH_NUM]) |= C_NUMBER_TABLE[(l_u16InputValue % 10)];}
        else
        {(g_u16DisPrimaryBuff_TABLE[C_SLAVE2_FOURTH_NUM]) |= C_NUMBER_TABLE[l_u16InputValue];}
    }        
    #endif
    
    #if (C_SLAVE2_DISPLAY_LENGTH == C_DIGITAL_3)
    {           
		if (l_u16InputValue > 999)
		{l_u16InputValue = 999;}

        if (l_u16InputValue >= 100)
        {g_u16DisPrimaryBuff_TABLE[C_SLAVE2_FIRST_NUM] |= C_NUMBER_TABLE[((l_u16InputValue / 100))];}
        else
        {g_u16DisPrimaryBuff_TABLE[C_SLAVE2_FIRST_NUM] |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {
            if (l_u16InputValue >= 100)
            {g_u16DisPrimaryBuff_TABLE[C_SLAVE2_SECOND_NUM] |= C_NUMBER_TABLE[((l_u16InputValue % 100) / 10)];}
            else
            {g_u16DisPrimaryBuff_TABLE[C_SLAVE2_SECOND_NUM] |= C_NUMBER_TABLE[(l_u16InputValue / 10)];}
        }
        else
        {g_u16DisPrimaryBuff_TABLE[C_SLAVE2_SECOND_NUM] |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {g_u16DisPrimaryBuff_TABLE[C_SLAVE2_THIRD_NUM] |= C_NUMBER_TABLE[(l_u16InputValue % 10)];}
        else
        {g_u16DisPrimaryBuff_TABLE[C_SLAVE2_THIRD_NUM] |= C_NUMBER_TABLE[l_u16InputValue];}
    }        
    #endif

    
    #if (C_SLAVE2_DISPLAY_LENGTH == C_DIGITAL_2)
    {             
		if (l_u16InputValue > 99)
		{l_u16InputValue = 99;}
			     
        if (l_u16InputValue >= 10)
        {g_u16DisPrimaryBuff_TABLE[C_SLAVE2_FIRST_NUM] |= C_NUMBER_TABLE[(l_u16InputValue / 10)];}
        else
        {g_u16DisPrimaryBuff_TABLE[C_SLAVE2_FIRST_NUM] |= C_LABLE_BLANK;}
        
        if (l_u16InputValue >= 10)
        {g_u16DisPrimaryBuff_TABLE[C_SLAVE2_SECOND_NUM] |= C_NUMBER_TABLE[(l_u16InputValue % 10)];}
        else
        {g_u16DisPrimaryBuff_TABLE[C_SLAVE2_SECOND_NUM] |= C_NUMBER_TABLE[l_u16InputValue];}
    }        
    #endif
    
    
}

/********************************************************************
*函数名：void F_LABLE_InsteadSet(unsigned int l_u16LableType, unsigned int *l_u16Digital_COM_Data)
*输入参数：l_u16LableType---->图标类型（选型范围参考头文件）
           l_u16Digital_COM_Data----->显示的位置（根据COM来进行配置）
      
*输出参数：无
*调用周期：无
*函数说明：在指定数据位代换显示
/*******************************************************************/
void F_LABLE_InsteadSet(unsigned int l_u16LableType, unsigned int *l_u16Digital_COM_Data)
{
    *l_u16Digital_COM_Data = l_u16LableType;
}


/********************************************************************
*函数名：void F_LABLE_CombineSet(unsigned int l_u16LableType, unsigned char l_u8ComNumber)
*输入参数：l_u16LableType---->图标类型（选型范围参考头文件）
           l_u16Digital_COM_Data----->显示的位置（根据COM来进行配置）
      
*输出参数：无
*调用周期：无
*函数说明：用于组合显示
/*******************************************************************/
void F_LABLE_CombineSet(unsigned int l_u16LableType, unsigned char l_u8ComNumber)
{
	if ((l_u8ComNumber == 0) || (l_u8ComNumber > DIGITAL_COM_NUMBERS))
	{return ;}

	g_u16DisPrimaryBuff_TABLE[l_u8ComNumber - 1] |= l_u16LableType;
}


/*****************************************************
*函数名称: void F_DISP_Clear(void)
*函数功能: 清除显示缓存
*入口参数：void
*出口参数：void 
*****************************************************/
void F_DISP_Clear(void)
{
	unsigned char i = 0;

	for (i = 0; i < DIGITAL_COM_NUMBERS; i++)
	{
		g_u16DisPrimaryBuff_TABLE[i] = 0;
	}
}

/*****************************************************
*函数名称: void F_DISP_All(void)
*函数功能: 全显缓存处理
*入口参数：void
*出口参数：void 
*****************************************************/
void F_DISP_All(void)
{
	unsigned char i = 0;

	for (i = 0; i < DIGITAL_COM_NUMBERS; i++)
	{
		g_u16DisPrimaryBuff_TABLE[i] = 0xFFFF;
	}
}

/********************************************************************
*函数名：void F_DISPLAY_DataRedirect(void)
*输入参数：无    
*输出参数：无
*调用周期：无
*函数说明：拷贝显示数据至输出缓存区
/*******************************************************************/
void F_DISPLAY_DataRedirect(void)
{
	unsigned char i = 0;

	g_u8DispLock = C_DISP_LOCK;

	for (i = 0; i < DIGITAL_COM_NUMBERS; i++)		//DIGITAL_COM_NUMBERS
	{
		g_u16DisDrive_TABLE[i] = g_u16DisPrimaryBuff_TABLE[i];
	}

	g_u8DispLock = C_DISP_UNLOCK;

}

static void F_DISPLAY_DataRedirect_S(void)
{
	unsigned char i = 0;

	for (i = 0; i < DIGITAL_COM_NUMBERS; i++)		//DIGITAL_COM_NUMBERS
	{
		g_u16DisDrive_TABLE[i] = g_u16DisPrimaryBuff_TABLE[i];
	}

}

/********************************************************************
*函数名：void F_DIGITAL_Drive(unsigned char l_u8DispLevel)
*输入参数：pTable显示缓存数组，数组成员个数为COM的个数
		   l_u8DispLevel(亮度调节，数字越小亮度越高)
*输出参数：无
*调用周期：根据COM个数适当调整
*函数说明：数码管驱动
/*******************************************************************/
#if (C_DIGITAL_TYPE == 0)

//共阴驱动
void F_DIGITAL_Drive(unsigned char l_u8DispLevel)
{
    static unsigned char xdata s_u8DigitalDispState = 0;

	//调节扫描时间
#if (C_SCAN_INTERVAL_TIME > 0)
	static unsigned char xdata s_u8IntervalTime = 0;

	if (++s_u8IntervalTime <= C_SCAN_INTERVAL_TIME)
	{return ;}
	else
	{s_u8IntervalTime = 0;}
#endif

	//显示数据未完全刷新
	if (g_u8DispLock == C_DISP_LOCK)
	{F_DISPLAY_DataRedirect_S();}	//缓存数据

    /************************************************/
    
    //熄屏消影	    
    _IO_COM1_OFF;

    #if (DIGITAL_COM_NUMBERS > 1)			  
    _IO_COM2_OFF;
    #endif
    
    #if (DIGITAL_COM_NUMBERS > 2)			  
    _IO_COM3_OFF;
    #endif

    #if (DIGITAL_COM_NUMBERS > 3)
    _IO_COM4_OFF;
    #endif

    #if (DIGITAL_COM_NUMBERS > 4)
    _IO_COM5_OFF;
    #endif
    
    #if (DIGITAL_COM_NUMBERS > 5)
    _IO_COM6_OFF;
    #endif
    
    #if (DIGITAL_COM_NUMBERS > 6)
    _IO_COM7_OFF;
    #endif

    #if (DIGITAL_COM_NUMBERS > 7)
    _IO_COM8_OFF;
    #endif

    #if (DIGITAL_COM_NUMBERS > 8)
    _IO_COM9_OFF;
    #endif

    #if (DIGITAL_COM_NUMBERS > 9)
    _IO_COM10_OFF;
    #endif

//	F_DispKey_Scan();
	
    /************************************************/
    //分步显示
    if (++s_u8DigitalDispState >= (DIGITAL_COM_NUMBERS + l_u8DispLevel))
    {s_u8DigitalDispState = 0;}
		    
    //SEG口输出	 
//    _BCP(GPIO_SEG_A,PIN_SEG_A,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),0);
//    _BCP(GPIO_SEG_B,PIN_SEG_B,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),1);
//    _BCP(GPIO_SEG_C,PIN_SEG_C,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),2);
//    _BCP(GPIO_SEG_D,PIN_SEG_D,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),3);
//    _BCP(GPIO_SEG_E,PIN_SEG_E,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),4);
//    _BCP(GPIO_SEG_F,PIN_SEG_F,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),5);
//    _BCP(GPIO_SEG_G,PIN_SEG_G,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),6);
//    _BCP(GPIO_SEG_H,PIN_SEG_H,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),7);

    _IO_BCP(IO_SEG_A,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),0);
    _IO_BCP(IO_SEG_B,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),1);
    _IO_BCP(IO_SEG_C,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),2);
    _IO_BCP(IO_SEG_D,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),3);
    _IO_BCP(IO_SEG_E,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),4);
    _IO_BCP(IO_SEG_F,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),5);
    _IO_BCP(IO_SEG_G,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),6);
//    _IO_BCP(IO_SEG_H,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),7);
	
	#if (DIGITAL_SEG_NUMBERS > 8)
    _BCP(GPIO_SEG_I,PIN_SEG_I,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),8);
	#endif

	#if (DIGITAL_SEG_NUMBERS > 9)
    _BCP(GPIO_SEG_J,PIN_SEG_J,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),9);
	#endif

	#if (DIGITAL_SEG_NUMBERS > 10)
    _BCP(GPIO_SEG_K,PIN_SEG_K,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),10);
	#endif

	#if (DIGITAL_SEG_NUMBERS > 11)
    _BCP(GPIO_SEG_L,PIN_SEG_L,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),11);
	#endif

	#if (DIGITAL_SEG_NUMBERS > 12)
    _BCP(GPIO_SEG_M,PIN_SEG_M,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),12);
	#endif

	#if (DIGITAL_SEG_NUMBERS > 13)
    _BCP(GPIO_SEG_N,PIN_SEG_N,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),13);
	#endif

	#if (DIGITAL_SEG_NUMBERS > 14)
    _BCP(GPIO_SEG_O,PIN_SEG_O,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),14);
	#endif

	#if (DIGITAL_SEG_NUMBERS > 15)
    _BCP(GPIO_SEG_P,PIN_SEG_P,(g_u16DisDrive_TABLE[s_u8DigitalDispState]),15);
	#endif
	    
    /************************************************/
	    
    //COM口输出
    switch (s_u8DigitalDispState)
    {
        case 0:
        {
            _IO_COM1_ON;
            break;
        }
        
		#if (DIGITAL_COM_NUMBERS > 1)
        case 1:
        {
            _IO_COM2_ON;        
            break;
        }
		#endif

        #if (DIGITAL_COM_NUMBERS > 2)
        case 2:
        {
            _IO_COM3_ON;             
            break;
        }
        #endif
        
        #if (DIGITAL_COM_NUMBERS > 3)
        case 3:
        {
            _IO_COM4_ON;             
            break;
        }
        #endif

        #if (DIGITAL_COM_NUMBERS > 4)
        case 4:
        {
            _IO_COM5_ON;             
            break;
        }
        #endif

        #if (DIGITAL_COM_NUMBERS > 5)
        case 5:
        {
            _IO_COM6_ON;             
            break;
        }
        #endif

        #if (DIGITAL_COM_NUMBERS > 6)
        case 6:
        {
            _IO_COM7_ON;             
            break;
        }
        #endif

        #if (DIGITAL_COM_NUMBERS > 7)
        case 7:
        {
            _IO_COM8_ON;             
            break;
        }
        #endif
        
        #if (DIGITAL_COM_NUMBERS > 8)
        case 8:
        {
            _IO_COM9_ON;             
            break;
        }
        #endif

        #if (DIGITAL_COM_NUMBERS > 9)
        case 9:
        {
            _IO_COM10_ON;             
            break;
        }
        #endif

        default: 
		{   
			break;
    	}	
	}
}

#else	 
//共阳驱动
void F_DIGITAL_Drive(unsigned char l_u8DispLevel)
{
    static unsigned char xdata s_u8DigitalDispState = 0;

	//调节扫描时间
#if (C_SCAN_INTERVAL_TIME > 0)
	static unsigned char xdata s_u8IntervalTime = 0;

	if (++s_u8IntervalTime <= C_SCAN_INTERVAL_TIME)
	{return ;}
	else
	{s_u8IntervalTime = 0;}
#endif

	//显示数据未完全刷新
	if (g_u8DispLock == C_DISP_LOCK)
	{F_DISPLAY_DataRedirect();}

    /************************************************/
    
    //熄屏消影
	_IO_SEG_A_OFF;
	_IO_SEG_B_OFF;
	_IO_SEG_C_OFF;
	_IO_SEG_D_OFF;
	_IO_SEG_E_OFF;
	_IO_SEG_F_OFF;
	_IO_SEG_G_OFF;
	_IO_SEG_H_OFF;
	
    #if (DIGITAL_SEG_NUMBERS > 8)			  
    _IO_SEG_I_OFF;
    #endif
	
    #if (DIGITAL_SEG_NUMBERS > 9)			  
    _IO_SEG_J_OFF;
    #endif
	
    #if (DIGITAL_SEG_NUMBERS > 10)			  
    _IO_SEG_K_OFF;
    #endif	
	
    #if (DIGITAL_SEG_NUMBERS > 11)			  
    _IO_SEG_L_OFF;
    #endif	
	
    #if (DIGITAL_SEG_NUMBERS > 12)			  
    _IO_SEG_M_OFF;
    #endif		
	
    #if (DIGITAL_SEG_NUMBERS > 13)			  
    _IO_SEG_N_OFF;
    #endif
	
    #if (DIGITAL_SEG_NUMBERS > 14)			  
    _IO_SEG_O_OFF;
    #endif
	
    #if (DIGITAL_SEG_NUMBERS > 15)			  
    _IO_SEG_P_OFF;
    #endif		
				    
    /************************************************/
    //分步显示
    if (++s_u8DigitalDispState >= (DIGITAL_SEG_NUMBERS + l_u8DispLevel))
    {s_u8DigitalDispState = 0;}
	
		    
    //COM口输出
	if (s_u8DigitalDispState < DIGITAL_COM_NUMBERS)
	{	 
		_BCP(GPIO_COM1,PIN_COM1,g_u16DisDrive_TABLE[0],s_u8DigitalDispState);
	
		#if (DIGITAL_COM_NUMBERS > 1)
		_BCP(GPIO_COM2,PIN_COM2,g_u16DisDrive_TABLE[1],s_u8DigitalDispState);
		#endif
	
		#if (DIGITAL_COM_NUMBERS > 2)
		_BCP(GPIO_COM3,PIN_COM3,g_u16DisDrive_TABLE[2],s_u8DigitalDispState);
		#endif
	
		#if (DIGITAL_COM_NUMBERS > 3)
		_BCP(GPIO_COM4,PIN_COM4,g_u16DisDrive_TABLE[3],s_u8DigitalDispState);
		#endif
		
		#if (DIGITAL_COM_NUMBERS > 4)
		_BCP(GPIO_COM5,PIN_COM5,g_u16DisDrive_TABLE[4],s_u8DigitalDispState);
		#endif
		
		#if (DIGITAL_COM_NUMBERS > 5)
		_BCP(GPIO_COM6,PIN_COM6,g_u16DisDrive_TABLE[5],s_u8DigitalDispState);
		#endif
		
		#if (DIGITAL_COM_NUMBERS > 6)
		_BCP(GPIO_COM7,PIN_COM7,g_u16DisDrive_TABLE[6],s_u8DigitalDispState);
		#endif			
		
		#if (DIGITAL_COM_NUMBERS > 7)
		_BCP(GPIO_COM8,PIN_COM8,g_u16DisDrive_TABLE[7],s_u8DigitalDispState);
		#endif
		
		#if (DIGITAL_COM_NUMBERS > 8)
		_BCP(GPIO_COM9,PIN_COM9,g_u16DisDrive_TABLE[8],s_u8DigitalDispState);
		#endif
		
		#if (DIGITAL_COM_NUMBERS > 9)
		_BCP(GPIO_COM10,PIN_COM10,g_u16DisDrive_TABLE[9],s_u8DigitalDispState);
		#endif
	}
					    
    /************************************************/
	    
    //COM口输出
    switch (s_u8DigitalDispState)
    {
        case 0:
        {
            _IO_SEG_A_ON;
            break;
        }

        case 1:
        {
            _IO_SEG_B_ON;
            break;
        }
		
        case 2:
        {
            _IO_SEG_C_ON;
            break;
        }
				
        case 3:
        {
            _IO_SEG_D_ON;
            break;
        }
		
        case 4:
        {
            _IO_SEG_E_ON;
            break;
        }				
		
        case 5:
        {
            _IO_SEG_F_ON;
            break;
        }
		
        case 6:
        {
            _IO_SEG_G_ON;
            break;
        }				
		
        case 7:
        {
            _IO_SEG_H_ON;
            break;
        }
		
		#if (DIGITAL_SEG_NUMBERS > 8)
        case 8:
        {
            _IO_SEG_I_ON;
            break;
        }				
		#endif
		
		#if (DIGITAL_SEG_NUMBERS > 9)
        case 9:
        {
            _IO_SEG_J_ON;
            break;
        }				
		#endif		        

		#if (DIGITAL_SEG_NUMBERS > 10)
        case 10:
        {
            _IO_SEG_K_ON;
            break;
        }				
		#endif

		#if (DIGITAL_SEG_NUMBERS > 11)
        case 11:
        {
            _IO_SEG_L_ON;
            break;
        }				
		#endif

		#if (DIGITAL_SEG_NUMBERS > 12)
        case 12:
        {
            _IO_SEG_M_ON;
            break;
        }				
		#endif

		#if (DIGITAL_SEG_NUMBERS > 13)
        case 13:
        {
            _IO_SEG_N_ON;
            break;
        }				
		#endif

		#if (DIGITAL_SEG_NUMBERS > 14)
        case 14:
        {
            _IO_SEG_O_ON;
            break;
        }				
		#endif

		#if (DIGITAL_SEG_NUMBERS > 15)
        case 15:
        {
            _IO_SEG_P_ON;
            break;
        }				
		#endif

        default: 
		{   
			break;
     	}
	}
}

#endif