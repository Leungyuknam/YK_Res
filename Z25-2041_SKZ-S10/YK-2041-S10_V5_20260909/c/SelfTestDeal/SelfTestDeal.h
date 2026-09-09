#ifndef _SELF_TEST_DEAL_H_
#define _SELF_TEST_DEAL_H_

extern U8 xdata g_u8TestState;
#define C_TestOff           0
#define C_SelfTest          1

extern void F_SelfTestKey();
extern void F_ClientTestKey();

extern void F_SelfTestDislayUpData(void);
extern void F_mSelfTestSchdule(void);

#endif