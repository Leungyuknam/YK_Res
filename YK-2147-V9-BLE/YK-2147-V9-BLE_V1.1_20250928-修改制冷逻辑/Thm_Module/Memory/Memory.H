/*********************************************************
文 件 名 : MEMORY.H
文件说明 : 存储头文件
编写作者 : thm
修订作者 : 
编写时间 : 2020-12-7
版 本 号 : V1.0
==========================================================*/
#ifndef _MEMORY_H_
#define _MEMORY_H_ 

/*************************需配置部分***********************/

#define		C_MEMORY_LENGTH	     19      //存储的字节数
#define		Def_MemoryType	     1		 //1：Flash   0：EEPROM
#define		Def_FlashSize	     32		 //Flash空间，32 = 32K ， 64 = 64K


/*************************配置结束*************************/

#if (Def_MemoryType == 1)

//IAP起始地址
#if (Def_FlashSize == 64)
#define     C_IAP_ADDR_STAR	           0x00
#elif (Def_FlashSize == 32)      
#define     C_IAP_ADDR_STAR	           0X7E01
#endif

#define     C_IAP_LIMIT_TIME	       0x42
#define     C_IAP_MEMTYPE	       	   0x00
#define     C_IAP_BACKUP_BIAS	       100		 //备份区地址偏移量

#endif


//外部函数声明
extern void F_MEMORY_AllWrite(void);
extern void F_MEMORY_AllRead(u16 l_u16Addr);
extern u8   F_CHECKOUT_MemoryData(u8 *pTable,u8 l_u8TableLength);
extern u8   F_CHECKSUM_MemoryCalculate(u8 *pTable,u8 l_u8TableLength);

extern void F_REFILL_MemoryTable(void);
extern void F_TAKE_MemoryTable(void);
extern void F_Memory_Hanle(void);

#endif