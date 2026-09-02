#ifndef _IAP_INIT_H_
#define _IAP_INIT_H_

#define IapROM     0x00	  //0x00：选择ROM区操作 
#define IapEPPROM  0x02	  //0x00：选择ROM区操作  0x02：选择EEPROM区操作

extern unsigned char IAP_Read(unsigned long Add,unsigned char Iapade);

#endif  
