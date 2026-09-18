/*********************************************************
文 件 名 : CPU_CONFIG.H
文件说明 : 芯片配置宏
编写作者 : thm
编写时间 : 2020-12-8
版 本 号 : V1.0
==========================================================*/ 
#ifndef _CPU_CONFIG_H_
#define _CPU_CONFIG_H_

//PWM
#define     C_PWM_SYS               0x30            //Fhrc:0x00  Fhrc/2:0x10  Fhrc/4:0x20  Fhrc/8:0x30
#define     C_PWM_CYCLE             1000            //0-4095
#define     C_PWM_PORT              1000            //PWM40:0X01    PWM41:0X02  PWM42:0X04  PWM43:0X08  PWM50:0X10  PWM51:0X20  PWM52:0X40  PWM53:0X80


#endif