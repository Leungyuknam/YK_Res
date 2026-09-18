#ifndef _PERIPHERAL_H_
#define _PERIPHERAL_H_

#define cli() {EA = 0;}
#define sei() {EA = 1;}

#define Time2cli()  T2CON &= ~0x04;\	
                    ET2 = 0; 

#define Time2sei()  T2CON |= 0x04;\	
                    ET2 = 1; 

extern void Timer2_Set(U16 temp);
extern void WDT_clear();             	// 清看门狗计时器
extern void F_mCpuInital(void);
extern void f_adcInit();
void f_iapWrite(U8 l_u8IapArea,U16 l_u16Addr,U8 *l_pu8Data,U8 l_u8Length);//l_u8IapArea,0X00=ROM,0X02=EE
void f_iapRead(U8 l_u8IapArea,U16 l_u16Addr,U8 *l_pu8Data,U8 l_u8Length);

#endif