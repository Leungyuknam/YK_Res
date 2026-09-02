#ifndef _DISPLAY_H_
#define _DISPLAY_H_

extern U8 xdata g_u8DisBright;    //LED灯显示亮度

extern U8 xdata g_u8RgbColorIndex;

extern U8 xdata ChipDispBuf[];


#define TOTAL_COM		12	//显示数  

#define C_RGB_R           0
#define C_RGB_G           1
#define C_RGB_B           2

extern void F_DispKey();
extern void F_DispClean();
extern void F_DispPowerUp();
//extern void F_DisplayDrive();
extern void F_RGBDry_IRQ();


extern void F_RgbTestDispDrv(U8 l_u8DisType);

extern U8 F_GetDispType();
extern void F_mDispInital();
extern void F_DispTskChange(unsigned char temp);

extern void F_mDisplayControl(void);

#endif  
 