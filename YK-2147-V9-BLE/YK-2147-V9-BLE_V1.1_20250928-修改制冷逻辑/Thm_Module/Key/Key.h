 
#ifndef _KEY_H_
#define _KEY_H_

#define C_NO_KEY_TIME_MAX 7210

extern void F_KEY_Handle(void);

#if (KEY_NUMBERS > 16)
extern u32 F_KeyScan(void); 
#elif (KEY_NUMBERS > 8)
extern u16 F_KeyScan(void);
#else
extern u8 F_KeyScan(void);
#endif

#endif


