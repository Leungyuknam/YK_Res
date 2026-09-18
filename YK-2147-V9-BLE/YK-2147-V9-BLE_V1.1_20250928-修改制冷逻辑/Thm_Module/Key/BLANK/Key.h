 
#ifndef _KEY_H_
#define _KEY_H_

#define C_NO_KEY_TIME_MAX 7210

extern void F_KEY_DealControl(void);

#if (KEY_VALUE_LENGTH == LENGTH_32BIT)  
extern u32 F_KeyScan(void); 
#elif (KEY_VALUE_LENGTH == LENGTH_16BIT)
extern u16 F_KeyScan(void);
#elif (KEY_VALUE_LENGTH == LENGTH_8BIT)
extern u8 F_KeyScan(void);
#endif

#endif


