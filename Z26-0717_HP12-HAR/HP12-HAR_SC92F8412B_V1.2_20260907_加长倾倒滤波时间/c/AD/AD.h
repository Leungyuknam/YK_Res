#ifndef _AD_H_
#define _AD_H_

//------------------------------ADC 配置---------------------------------
// SC92F8413 寄存器定义适配
// ADCCON: b5 = ADCIF (中断标志/完成标志)
// ADCCFG0：b0=VREFS（0=VDD，1=内部 2.4 V）

#define ADC_OK_FLAG             (ADCCON & BIT5)     
#define ADC_VALUE_10BIT         ((ADCVH << 2) | (ADCVL >> 6))

// 启动ADC: 设置通道(低5位) + 开启ADC(b7) + 启动转换(b6)
// 注意：SC92F841x 通常通过 ADCCON = 0xC0 | 通道号启动
#define F_AdcStart(Ch)          { ADCCON = (Ch & 0x1f) | 0xC0; }

// 关闭ADC: 清除 ADEN
#define F_AdcOff()              { ADCCON &= 0xef;ADCCFG0 = 0x00;ADCCFG1 = 0x00; } 

// 清除标志位
#define F_ClrAdcOkFlg()         { ADCCON &= ~BIT5; }

// 初始化 (如果需要特殊配置频率，在此添加)
#define F_AdcInit()             { f_adcInit(); } 

// Vref 配置 (适配 SC92F8413)
// 位 0：0=VDD，1=内部 2.4 V
#define F_SetVRef_Vdd()         { OPINX = 0xc2;OPREG &= ~BIT7; }
#define F_SetVRef_2V4()         { OPINX = 0xc2;OPREG |= BIT7;  }

//------------------------------函数声明---------------------------------
// 返回值：0=进行中，1=平均值完成，2=单次完成（采样次数设为 1 时）
U8 F_ADC_Read(U8 u8Ch, U8 u8Samples, U16 *pu16Result);

#endif