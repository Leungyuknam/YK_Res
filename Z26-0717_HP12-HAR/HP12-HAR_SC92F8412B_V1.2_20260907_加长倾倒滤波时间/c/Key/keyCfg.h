// HP12-HAR 按键配置（5 个触摸键）
// 按住处理表：KeyEventDealCode，用于长按和组合键操作
// 松手处理表：ReleseKeyEventDealCode，用于短按操作

#define C_TimeStandard      10      // 每 10 ms 调用一次 F_KeyDealControl

#define C_KEY_60MS          (60 / C_TimeStandard)
#define C_KEY_200MS         (200 / C_TimeStandard)
#define C_KEY_500MS         (500 / C_TimeStandard)
#define C_KEY_1S            (1000 / C_TimeStandard)
#define C_KEY_2S            (2000 / C_TimeStandard)
#define C_KEY_3S            (3000 / C_TimeStandard)

struct KeyEventDeal
{
    unsigned char KeyValue;
    unsigned int  FirstTriggerTime;
    unsigned char IfRepeat;
    unsigned int  RepeatTriggerTime;
    void (*voidKeyEvent)(void);
};

struct ReleseKeyEventDeal
{
    unsigned char KeyValue;
    unsigned int  MinReleseTime;
    unsigned int  MaxReleseTime;
    void (*voidReleseKeyEvent)(void);
};

// -----------------------------------------------------------------
// 按住/长按处理表（组合键条目必须放在单键条目之前，
// 以便调度器优先匹配组合键）。
// -----------------------------------------------------------------
code struct KeyEventDeal KeyEventDealCode[] =
{
    // 上电前 15 秒内同时长按开机键和人体感应键 3 秒，进入自检
    { c_KEY_POWER | c_KEY_HUMAN, C_KEY_3S, 0, C_KEY_500MS, &F_SelfTestKey },
    // 单独长按童锁键 2 秒，切换童锁状态
    { c_KEY_LOCK,               C_KEY_2S, 0, C_KEY_500MS, &F_LockKey     },
    { 0, 0, 0, 0, 0 }
};

// -----------------------------------------------------------------
// 松手（短按）处理表
// -----------------------------------------------------------------
code struct ReleseKeyEventDeal ReleseKeyEventDealCode[] =
{
    { c_KEY_POWER, C_KEY_60MS, C_KEY_500MS, &F_PowerKey },
    { c_KEY_GEAR,  C_KEY_60MS, C_KEY_500MS, &F_GearKey  },
    { c_KEY_TIME,  C_KEY_60MS, C_KEY_500MS, &F_TimerKey },
    { c_KEY_HUMAN, C_KEY_60MS, C_KEY_500MS, &F_HumanKey },
    { 0, 0, 0, 0 }
};
