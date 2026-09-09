#ifndef __TK_CHECK_H
#define __TK_CHECK_H

/*-----------------------------------------------------
        先包含参数头文件
-----------------------------------------------------*/
//#include "tk_conf.h"
#include "c\TouchKey\tk_conf.h"
/*-----------------------------------------------------
        配置参数
-----------------------------------------------------*/

// 设置灵敏度方式 
// 0：表示正常使用人手变化量来 来调试灵敏度阀值， 
// 1：表示使用客户金手指工装测试变化量 来调试灵敏度阀值
#define  TEST_TK_SENSOR_TYPE        0

//整机情况下测试灵敏度阀值宏定义
#if (TEST_TK_SENSOR_TYPE == 0)
    // 使用人手操作面板的变化量设置灵敏度阀值:
    // 宏定义填写值： =人手操作的变化量K1 *1.25 ；
    // 需要注意的: 人手操作的变化量K1 建议大于 260，如果变化量小于260，建议调节增大放大倍数设置  HF_DIV  LF_DIV
    #define     DIFF_MUL            (1.25)
#else
    // 使用金手指工装变化量设置灵敏度阀值 客户标准: 直径4mm金手指不允许响应，直径10mm金手指允许响应
    // 宏定义填写值： = K/0.4 ；其中 K=  (直径4mm金手指操作变化量 +直径10mm金手指操作变化量)/2；
    // 需要注意的: K 值建议大于140，如果变化量小于140，建议调节增大放大倍数设置  HF_DIV  LF_DIV
    #define     DIFF_MUL            (2.0)
#endif


/*-----------------------------------------------------
        再进行校验
-----------------------------------------------------*/

/* 未使用的按键，放大倍数也要填满表格 */
#define     LF_DIV_DEF          (3)             /* 低频默认放大倍数 */
#define     HF_DIV_DEF          (LF_DIV_DEF)    /* 高频默认放大倍数 */

// 低频放大倍数
#ifndef  LF_DIV0
    #define LF_DIV0             (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV1
    #define LF_DIV1             (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV2
    #define LF_DIV2             (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV3
    #define LF_DIV3             (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV4
    #define LF_DIV4             (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV5
    #define LF_DIV5             (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV6
    #define LF_DIV6             (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV7
    #define LF_DIV7             (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV8
    #define LF_DIV8             (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV9
    #define LF_DIV9             (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV10
    #define LF_DIV10            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV11
    #define LF_DIV11            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV12
    #define LF_DIV12            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV13
    #define LF_DIV13            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV14
    #define LF_DIV14            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV15
    #define LF_DIV15            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV16
    #define LF_DIV16            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV17
    #define LF_DIV17            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV18
    #define LF_DIV18            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV19
    #define LF_DIV19            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV20
    #define LF_DIV20            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV21
    #define LF_DIV21            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV22
    #define LF_DIV22            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV23
    #define LF_DIV23            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV24
    #define LF_DIV24            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV25
    #define LF_DIV25            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV26
    #define LF_DIV26            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV27
    #define LF_DIV27            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV28
    #define LF_DIV28            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV29
    #define LF_DIV29            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV30
    #define LF_DIV30            (LF_DIV_DEF*256)
#endif
#ifndef  LF_DIV31
    #define LF_DIV31            (LF_DIV_DEF*256)
#endif

// 高频放大倍数
#ifndef  HF_DIV0
    #define HF_DIV0             (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV1
    #define HF_DIV1             (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV2
    #define HF_DIV2             (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV3
    #define HF_DIV3             (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV4
    #define HF_DIV4             (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV5
    #define HF_DIV5             (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV6
    #define HF_DIV6             (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV7
    #define HF_DIV7             (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV8
    #define HF_DIV8             (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV9
    #define HF_DIV9             (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV10
    #define HF_DIV10            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV11
    #define HF_DIV11            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV12
    #define HF_DIV12            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV13
    #define HF_DIV13            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV14
    #define HF_DIV14            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV15
    #define HF_DIV15            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV16
    #define HF_DIV16            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV17
    #define HF_DIV17            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV18
    #define HF_DIV18            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV19
    #define HF_DIV19            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV20
    #define HF_DIV20            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV21
    #define HF_DIV21            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV22
    #define HF_DIV22            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV23
    #define HF_DIV23            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV24
    #define HF_DIV24            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV25
    #define HF_DIV25            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV26
    #define HF_DIV26            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV27
    #define HF_DIV27            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV28
    #define HF_DIV28            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV29
    #define HF_DIV29            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV30
    #define HF_DIV30            (HF_DIV_DEF*256)
#endif
#ifndef  HF_DIV31
    #define HF_DIV31            (HF_DIV_DEF*256)
#endif


/* 未使用的按键，阈值也要填满表格 */
#define  TK_Diff_undefined      (300)
// 低频
#ifndef  LF_diff0
    #define LF_diff0            (TK_Diff_undefined)
#endif
#ifndef  LF_diff1
    #define LF_diff1            (TK_Diff_undefined)
#endif
#ifndef  LF_diff2
    #define LF_diff2            (TK_Diff_undefined)
#endif
#ifndef  LF_diff3
    #define LF_diff3            (TK_Diff_undefined)
#endif
#ifndef  LF_diff4
    #define LF_diff4            (TK_Diff_undefined)
#endif
#ifndef  LF_diff5
    #define LF_diff5            (TK_Diff_undefined)
#endif
#ifndef  LF_diff6
    #define LF_diff6            (TK_Diff_undefined)
#endif
#ifndef  LF_diff7
    #define LF_diff7            (TK_Diff_undefined)
#endif
#ifndef  LF_diff8
    #define LF_diff8            (TK_Diff_undefined)
#endif
#ifndef  LF_diff9
    #define LF_diff9            (TK_Diff_undefined)
#endif
#ifndef  LF_diff10
    #define LF_diff10           (TK_Diff_undefined)
#endif
#ifndef  LF_diff11
    #define LF_diff11           (TK_Diff_undefined)
#endif
#ifndef  LF_diff12
    #define LF_diff12           (TK_Diff_undefined)
#endif
#ifndef  LF_diff13
    #define LF_diff13           (TK_Diff_undefined)
#endif
#ifndef  LF_diff14
    #define LF_diff14           (TK_Diff_undefined)
#endif
#ifndef  LF_diff15
    #define LF_diff15           (TK_Diff_undefined)
#endif
#ifndef  LF_diff16
    #define LF_diff16           (TK_Diff_undefined)
#endif
#ifndef  LF_diff17
    #define LF_diff17           (TK_Diff_undefined)
#endif
#ifndef  LF_diff18
    #define LF_diff18           (TK_Diff_undefined)
#endif
#ifndef  LF_diff19
    #define LF_diff19           (TK_Diff_undefined)
#endif
#ifndef  LF_diff20
    #define LF_diff20           (TK_Diff_undefined)
#endif
#ifndef  LF_diff21
    #define LF_diff21           (TK_Diff_undefined)
#endif
#ifndef  LF_diff22
    #define LF_diff22           (TK_Diff_undefined)
#endif
#ifndef  LF_diff23
    #define LF_diff23           (TK_Diff_undefined)
#endif
#ifndef  LF_diff24
    #define LF_diff24           (TK_Diff_undefined)
#endif
#ifndef  LF_diff25
    #define LF_diff25           (TK_Diff_undefined)
#endif
#ifndef  LF_diff26
    #define LF_diff26           (TK_Diff_undefined)
#endif
#ifndef  LF_diff27
    #define LF_diff27           (TK_Diff_undefined)
#endif
#ifndef  LF_diff28
    #define LF_diff28           (TK_Diff_undefined)
#endif
#ifndef  LF_diff29
    #define LF_diff29           (TK_Diff_undefined)
#endif
#ifndef  LF_diff30
    #define LF_diff30           (TK_Diff_undefined)
#endif
#ifndef  LF_diff31
    #define LF_diff31           (TK_Diff_undefined)
#endif

// 高频
#ifndef  HF_diff0
    #define HF_diff0            (TK_Diff_undefined)
#endif
#ifndef  HF_diff1
    #define HF_diff1            (TK_Diff_undefined)
#endif
#ifndef  HF_diff2
    #define HF_diff2            (TK_Diff_undefined)
#endif
#ifndef  HF_diff3
    #define HF_diff3            (TK_Diff_undefined)
#endif
#ifndef  HF_diff4
    #define HF_diff4            (TK_Diff_undefined)
#endif
#ifndef  HF_diff5
    #define HF_diff5            (TK_Diff_undefined)
#endif
#ifndef  HF_diff6
    #define HF_diff6            (TK_Diff_undefined)
#endif
#ifndef  HF_diff7
    #define HF_diff7            (TK_Diff_undefined)
#endif
#ifndef  HF_diff8
    #define HF_diff8            (TK_Diff_undefined)
#endif
#ifndef  HF_diff9
    #define HF_diff9            (TK_Diff_undefined)
#endif
#ifndef  HF_diff10
    #define HF_diff10           (TK_Diff_undefined)
#endif
#ifndef  HF_diff11
    #define HF_diff11           (TK_Diff_undefined)
#endif
#ifndef  HF_diff12
    #define HF_diff12           (TK_Diff_undefined)
#endif
#ifndef  HF_diff13
    #define HF_diff13           (TK_Diff_undefined)
#endif
#ifndef  HF_diff14
    #define HF_diff14           (TK_Diff_undefined)
#endif
#ifndef  HF_diff15
    #define HF_diff15           (TK_Diff_undefined)
#endif
#ifndef  HF_diff16
    #define HF_diff16           (TK_Diff_undefined)
#endif
#ifndef  HF_diff17
    #define HF_diff17           (TK_Diff_undefined)
#endif
#ifndef  HF_diff18
    #define HF_diff18           (TK_Diff_undefined)
#endif
#ifndef  HF_diff19
    #define HF_diff19           (TK_Diff_undefined)
#endif
#ifndef  HF_diff20
    #define HF_diff20           (TK_Diff_undefined)
#endif
#ifndef  HF_diff21
    #define HF_diff21           (TK_Diff_undefined)
#endif
#ifndef  HF_diff22
    #define HF_diff22           (TK_Diff_undefined)
#endif
#ifndef  HF_diff23
    #define HF_diff23           (TK_Diff_undefined)
#endif
#ifndef  HF_diff24
    #define HF_diff24           (TK_Diff_undefined)
#endif
#ifndef  HF_diff25
    #define HF_diff25           (TK_Diff_undefined)
#endif
#ifndef  HF_diff26
    #define HF_diff26           (TK_Diff_undefined)
#endif
#ifndef  HF_diff27
    #define HF_diff27           (TK_Diff_undefined)
#endif
#ifndef  HF_diff28
    #define HF_diff28           (TK_Diff_undefined)
#endif
#ifndef  HF_diff29
    #define HF_diff29           (TK_Diff_undefined)
#endif
#ifndef  HF_diff30
    #define HF_diff30           (TK_Diff_undefined)
#endif
#ifndef  HF_diff31
    #define HF_diff31           (TK_Diff_undefined)
#endif

/* 灵敏度、阈值 */
// 低频
#define     LF_MAX0            (LF_diff0  * DIFF_MUL)
#define     LF_MAX1            (LF_diff1  * DIFF_MUL)
#define     LF_MAX2            (LF_diff2  * DIFF_MUL)
#define     LF_MAX3            (LF_diff3  * DIFF_MUL)
#define     LF_MAX4            (LF_diff4  * DIFF_MUL)
#define     LF_MAX5            (LF_diff5  * DIFF_MUL)
#define     LF_MAX6            (LF_diff6  * DIFF_MUL)
#define     LF_MAX7            (LF_diff7  * DIFF_MUL)
#define     LF_MAX8            (LF_diff8  * DIFF_MUL)
#define     LF_MAX9            (LF_diff9  * DIFF_MUL)
#define     LF_MAX10           (LF_diff10 * DIFF_MUL)
#define     LF_MAX11           (LF_diff11 * DIFF_MUL)
#define     LF_MAX12           (LF_diff12 * DIFF_MUL)
#define     LF_MAX13           (LF_diff13 * DIFF_MUL)
#define     LF_MAX14           (LF_diff14 * DIFF_MUL)
#define     LF_MAX15           (LF_diff15 * DIFF_MUL)
#define     LF_MAX16           (LF_diff16 * DIFF_MUL)
#define     LF_MAX17           (LF_diff17 * DIFF_MUL)
#define     LF_MAX18           (LF_diff18 * DIFF_MUL)
#define     LF_MAX19           (LF_diff19 * DIFF_MUL)
#define     LF_MAX20           (LF_diff20 * DIFF_MUL)
#define     LF_MAX21           (LF_diff21 * DIFF_MUL)
#define     LF_MAX22           (LF_diff22 * DIFF_MUL)
#define     LF_MAX23           (LF_diff23 * DIFF_MUL)
#define     LF_MAX24           (LF_diff24 * DIFF_MUL)
#define     LF_MAX25           (LF_diff25 * DIFF_MUL)
#define     LF_MAX26           (LF_diff26 * DIFF_MUL)
#define     LF_MAX27           (LF_diff27 * DIFF_MUL)
#define     LF_MAX28           (LF_diff28 * DIFF_MUL)
#define     LF_MAX29           (LF_diff29 * DIFF_MUL)
#define     LF_MAX30           (LF_diff30 * DIFF_MUL)
#define     LF_MAX31           (LF_diff31 * DIFF_MUL)

// 高频
#define     HF_MAX0            (HF_diff0  * DIFF_MUL)
#define     HF_MAX1            (HF_diff1  * DIFF_MUL)
#define     HF_MAX2            (HF_diff2  * DIFF_MUL)
#define     HF_MAX3            (HF_diff3  * DIFF_MUL)
#define     HF_MAX4            (HF_diff4  * DIFF_MUL)
#define     HF_MAX5            (HF_diff5  * DIFF_MUL)
#define     HF_MAX6            (HF_diff6  * DIFF_MUL)
#define     HF_MAX7            (HF_diff7  * DIFF_MUL)
#define     HF_MAX8            (HF_diff8  * DIFF_MUL)
#define     HF_MAX9            (HF_diff9  * DIFF_MUL)
#define     HF_MAX10           (HF_diff10 * DIFF_MUL)
#define     HF_MAX11           (HF_diff11 * DIFF_MUL)
#define     HF_MAX12           (HF_diff12 * DIFF_MUL)
#define     HF_MAX13           (HF_diff13 * DIFF_MUL)
#define     HF_MAX14           (HF_diff14 * DIFF_MUL)
#define     HF_MAX15           (HF_diff15 * DIFF_MUL)
#define     HF_MAX16           (HF_diff16 * DIFF_MUL)
#define     HF_MAX17           (HF_diff17 * DIFF_MUL)
#define     HF_MAX18           (HF_diff18 * DIFF_MUL)
#define     HF_MAX19           (HF_diff19 * DIFF_MUL)
#define     HF_MAX20           (HF_diff20 * DIFF_MUL)
#define     HF_MAX21           (HF_diff21 * DIFF_MUL)
#define     HF_MAX22           (HF_diff22 * DIFF_MUL)
#define     HF_MAX23           (HF_diff23 * DIFF_MUL)
#define     HF_MAX24           (HF_diff24 * DIFF_MUL)
#define     HF_MAX25           (HF_diff25 * DIFF_MUL)
#define     HF_MAX26           (HF_diff26 * DIFF_MUL)
#define     HF_MAX27           (HF_diff27 * DIFF_MUL)
#define     HF_MAX28           (HF_diff28 * DIFF_MUL)
#define     HF_MAX29           (HF_diff29 * DIFF_MUL)
#define     HF_MAX30           (HF_diff30 * DIFF_MUL)
#define     HF_MAX31           (HF_diff31 * DIFF_MUL)


#endif
