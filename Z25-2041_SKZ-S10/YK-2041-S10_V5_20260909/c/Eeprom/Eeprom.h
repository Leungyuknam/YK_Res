#ifndef _EEPROM_H_
#define _EEPROM_H_

#define c_u8SizeBuf 	2

#define EEPRON_TEXT_IO_SET      {}
#define EEPRON_TEXT_IO_CLR      {}
//#define EEPRON_TEXT_IO_SET      {P0_4 = 1;}
//#define EEPRON_TEXT_IO_CLR      {P0_4 = 0;}

#define IO_OFF() {P0 = 0;P1 = 0x40;P2 = 0;P3 = 0;}

extern void F_IapWriteCtrl();	   //主函数中调用
extern void F_EepromReadCtrl();    //上电0.5s读，注意屏蔽上电0.5s内的操作
extern void F_EepromWriteCtrl();   //写数据

#endif