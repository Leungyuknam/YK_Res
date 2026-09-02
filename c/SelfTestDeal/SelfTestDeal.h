#ifndef _SELF_TEST_DEAL_H_
#define _SELF_TEST_DEAL_H_

extern U8 xdata g_u8TestState;
#define C_TestOff           0
#define C_SelfTest          1
#define C_Quick_Test        3

extern bit g_bShrinkTimeFlg;    //Àı ±±Íº«

extern void F_SelfTestKey();
extern void F_QuickTestKey();

extern void F_SelfTestDislayUpData(void);
extern void F_mSelfTestSchdule(void);

#endif