#ifndef _IO_H_
#define _IO_H_

//上下摇头


//PIN1~PIN8
//GND  //PIN1   //no use LR IO
#define P_LR4		P1_0 //PIN2
#define P_PWM		P2_3 //P1_1 //PIN3  //风扇驱动
#define P_LR1		P2_7 //PIN4
#define P_LR2		P2_6 //PIN5
//烧录口SCK	 //PIN6
//烧录口SDA	 //PIN7
#define P_LR3		P2_3 //PIN8

//PIN9~PIN16
//#define P_VOL_AD	P0_6 //AN6 //PIN9

#define P_UPDN1		P0_0//P0_5 //PIN10
#define P_UPDN2		P0_1//P0_4 //PIN11
#define P_UPDN3		P0_2//P0_3 //PIN12
#define P_UPDN4		P0_3//P0_2 //PIN13
//#define P_TX		P0_1 //PIN14
//#define P_RX		P0_0 //PIN15
//VDD  //PIN16
#endif 