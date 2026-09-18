/*********************************************************
文 件 名 : CRC_Check.H
文件说明 : CRC校验头文件
编写作者 : thm
修订作者 : 
编写时间 : 2021-2-24
版 本 号 : V1.0
==========================================================*/
#ifndef _CRC_CHECK_H_
#define _CRC_CHECK_H_ 

#define			Def_CRC16			1

//CRC初始值(0x0000,0xFFFF)
#define			C_INIT			    0xFFFF

//多项式(0x8005,0x1021,0x3D65)
#define			C_POLY			    0x8005

//多项式值是否按位反转(1反转,0不反转)
#define			Def_REFPOLY			0

//待测数据的每个字节是否按位反转(1反转,0不反转)
#define			Def_REFIN			0

//在计算之后，异或输出之前，整个数据是否按位反转(1反转,0不反转)
#define			Def_REFOUT			0

//CRC值是否需异或输出(1使能,0不使能)
#define			Def_NEEDXOR			1

//计算结果与此参数异或后得到最终的CRC值(0x0000,0xFFFF)
#define			C_XOROUT			0x0000


//函数声明
extern u8 F_CRC16_Check(u8 *pTable, u8 l_u8CheckLength);
   
#endif