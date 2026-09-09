#include "h\Allinclude.h"
#include "ABSACC.H"
#include "cpu.h"

UCHAR ssp_flag;

/*****************************************************************************************************
 *  Function Name: UCHAR EEPromByteRead(UCHAR nAddrH,UCHAR nAddrL)  
 *  Created By:    xiong.zhang
 *  Created Date:  2021-05-31
 *  Input:         nAddrH, nAddrL
 *  Output:        None
 *  Return:        nTemp
 *  Description:   从指定地址处读取1个byte返回
 *  Modified By:   
 *  Modified Date:
 *  History:
 *  Others:                        
 *****************************************************************************************************/ 
UCHAR EEPromByteRead(UCHAR nAddrH,UCHAR nAddrL)  				// 字节读
{
	UCHAR nTemp;
	//=====关闭总中断======
     _push_(IEN0);
     IEN0 &=0x7F;//关总中断	 
	//======访问EEPROM区===========
	FLASHCON = 0x01;
    //=====读取相应地址数据========
	nTemp= CBYTE[nAddrH*256+nAddrL];
    //=======切回Flash区=========
	FLASHCON = 0x00;
	//====操作结束，恢复总中断=====
    _pop_(IEN0);
	return (nTemp);
}

/*****************************************************************************************************
 *  Function Name: void EEPromByteProgram(UCHAR nAddrH,UCHAR nAddrL, UCHAR nData)   
 *  Created By:    xiong.zhang
 *  Created Date:  2021-05-31
 *  Input:         nAddrH, nAddrL，nData
 *  Output:        None
 *  Return:        
 *  Description:   向指定地址处写入1个byte
 *  Modified By:   
 *  Modified Date:
 *  History:
 *  Others:                        
 *****************************************************************************************************/ 
void EEPromByteProgram(UCHAR nAddrH,UCHAR nAddrL, UCHAR nData)  // 字节编程
{
	//=====关闭总中断======
    _push_(IEN0);
    IEN0 &=0x7F;//关总中断	 
    //======访问EEPROM区========
	FLASHCON = 0x01;	
	XPAGE = nAddrH;			 		   //从0x00开始
	IB_OFFSET = nAddrL;
	IB_DATA = nData;                   // 烧写内容
	IB_CON1	 = 0x6E;
	IB_CON2	 = 0x05;
	IB_CON3	 = 0x0A;
	IB_CON4	 = 0x09;
    //=====增加flag判断，加强抗干扰能力======
	if(ssp_flag!=0x5A)
		goto Error;
	IB_CON5	 = 0x06;
	_nop_() ;				
	_nop_() ;
	_nop_() ;
	_nop_() ;	
Error:
	ssp_flag = 0;
	IB_CON1 = 0x00;
	IB_CON2 = 0x00;
	IB_CON3 = 0x00;
	IB_CON4 = 0x00;
	IB_CON5 = 0x00;
	//=========切回Flash区======
	FLASHCON = 0x00;
	//====操作结束，恢复总中断=====
    _pop_(IEN0);
}
/*****************************************************************************************************
 *  Function Name: void EEPromSectorErase(UCHAR nAddrH)   
 *  Created By:    xiong.zhang
 *  Created Date:  2021-05-31
 *  Input:         nAddrH
 *  Output:        None
 *  Return:        
 *  Description:   
 *  Modified By:   
 *  Modified Date:
 *  History:
 *  Others:                        
 *****************************************************************************************************/ 
void EEPromSectorErase(UCHAR nAddrH)// 扇区擦除
{											
	//=====关闭总中断======
     _push_(IEN0);
     IEN0 &=0x7F;//关总中断	 
    //======访问EEPROM区========
	FLASHCON = 0x01;									
	
	XPAGE = nAddrH<<1 ;			 	

	IB_CON1	 = 0xE6;	
	IB_CON2	 = 0x05;
	IB_CON3	 = 0x0A;
	IB_CON4	 = 0x09;
//=====增加flag判断，加强抗干扰能力======
	if(ssp_flag!=0xA5)
		goto Error;
	IB_CON5	 = 0x06;
	_nop_();				
	_nop_();
	_nop_();
	_nop_();	
Error:
	ssp_flag = 0;
	IB_CON1 = 0x00;
	IB_CON2 = 0x00;
	IB_CON3 = 0x00;
	IB_CON4 = 0x00;
	IB_CON5 = 0x00;
	//=========切回Flash区======
	FLASHCON = 0x00;
	//====操作结束，恢复总中断=====
    _pop_(IEN0);
}