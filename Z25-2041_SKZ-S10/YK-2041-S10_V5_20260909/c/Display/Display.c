#include "h\Allinclude.h"

// =================================================================================
// ZONE 1: 全局与静态变量区 (RAM)
// =================================================================================
U8 xdata g_t_u8TMS6644_DispBuf[9][16] = {0}; // 显存核心数组

static U8 xdata s_u8DispLastT = 0;
static U8 xdata s_u8DispIndex = 0;
static U8 xdata s_u8DispBlink = 0;
static bit s_bDispMask = 0;
static U8 xdata s_u8NormalTick = 0;
static U8 xdata s_u8WLackTick = 0;   /* water-lack anim tick (point5) */
static bit s_bTimeModified = 0; // 记录定时期间是否发生了修改

// 常规显示任务：状态机变量
#define NSTATE_CONFIRM  0  // 状态0：切换模式后定格最后一帧 (3秒)
#define NSTATE_ANIM     1  // 状态1：以 300ms/帧 播放动画
#define NSTATE_HUMI     2  // 状态2：显示湿度 (3秒)

static U8 xdata s_u8NormalState = NSTATE_CONFIRM; 
static U16 xdata s_u16NormalTimer = 0; 


// =================================================================================
// ZONE 2: 硬件映射数据 (HAL ROM DATA)
// =================================================================================
// --- 1. 区域映射表 ---
U8 code t_u8Num_RowMap[9] = { 11,10,9,8,7,6,5,4,3 }; 
U8 code t_u8Num_ColMap[9] = { 0,1,2,3, 4, 5,6,7,8 };  
TYPE_DIGITAL_AREA code g_Area_Number = { t_u8Num_ColMap, t_u8Num_RowMap, 2 };

U8 code t_u8Matrix_RowMap[9] = { 11,10,9,8,7,6,5,4,3 }; 
U8 code t_u8Matrix_ColMap[9] = { 0,1,2,3,4,5,6,7,8 };  
TYPE_DIGITAL_AREA code g_Area_Icon9x9 = { t_u8Matrix_ColMap, t_u8Matrix_RowMap, 0 };

// --- 2. LED 物理坐标数组 ---
TYPE_LED_PIXEL code t_Pix_LightKey[] = {
    {PCOM1, PSEG15}, {PCOM1, PSEG14}, {PCOM1, PSEG16}, {PCOM1, PSEG13}, {PCOM2, PSEG13}, {PCOM2, PSEG14}
};
TYPE_LED_PIXEL code t_Pix_TimeKey[] = {
    {PCOM2, PSEG15}, {PCOM2, PSEG16}
};
TYPE_LED_PIXEL code t_Pix_ModeKey[] = {
    {PCOM3, PSEG13}, {PCOM3, PSEG14}, {PCOM3, PSEG15}, {PCOM3, PSEG16}, {PCOM4, PSEG13}, {PCOM4, PSEG14}
};
TYPE_LED_PIXEL code t_Pix_PowerKey[] = {
    {PCOM4, PSEG15}, {PCOM4, PSEG16}
};
TYPE_LED_PIXEL code t_Pix_Time[] = {
    {PCOM1, PSEG3}, {PCOM2, PSEG3}, {PCOM3, PSEG3}, {PCOM4, PSEG3}
};
TYPE_LED_PIXEL code t_Pix_AirDry[] = {
    {PCOM1, PSEG2}, {PCOM2, PSEG2}, {PCOM3, PSEG2}, {PCOM4, PSEG2}
};
TYPE_LED_PIXEL code t_Pix_WaterLack[] = {
    {PCOM1, PSEG1}, {PCOM2, PSEG1}, {PCOM3, PSEG1}, {PCOM4, PSEG1}
};
TYPE_LED_PIXEL code t_Pix_Wifi[] = {
    {PCOM5, PSEG3}, {PCOM6, PSEG3}, {PCOM7, PSEG3}, {PCOM8, PSEG3}
};
TYPE_LED_PIXEL code t_Pix_WaterRemain[] = {
    {PCOM5, PSEG2}, {PCOM6, PSEG2}, {PCOM7, PSEG2}, {PCOM8, PSEG2}
};
TYPE_LED_PIXEL code t_Pix_Humipercent[] = {
    {PCOM5, PSEG1}, {PCOM6, PSEG1}, {PCOM7, PSEG1}, {PCOM8, PSEG1}
};

// --- 3. 映射总表 ---
TYPE_LED_MAP code t_stLedMap[_LED_TOTAL_] =
{
    { 6,  t_Pix_LightKey      },
    { 2,  t_Pix_TimeKey       },
    { 6,  t_Pix_ModeKey       },
    { 2,  t_Pix_PowerKey      },
    { 4,  t_Pix_Time          },
    { 4,  t_Pix_AirDry        },
    { 4,  t_Pix_WaterLack     },
    { 4,  t_Pix_Wifi          },
    { 4,  t_Pix_WaterRemain   },
    { 4,  t_Pix_Humipercent   }
};

// --- 4. 亮度查表 ---
//U8 code t_u8BrightGammaMap[11] = { 0, 4, 8, 16, 28, 45, 68, 95, 128, 180, 255 };
U8 code t_u8BrightGammaMap[11] = { 0, 4, 8, 16, 28, 45, 50, 50, 60, 80, 100 };

// =================================================================================
// ZONE 3: 字模与动画数据区 (ROM)
// =================================================================================
// --- 1. 基础与专用字模 ---
U16 code t_Font_h_3x6[3] = {0x01F8, 0x0040, 0x01C0}; 
U16 code t_Font_Timer_3x9[3][3] = {
    {0x00FE, 0x0101, 0x00FE}, // 0: 紧凑版 '0'
    {0x0000, 0x0000, 0x0000}, // 1: 占位不用
    {0x01E1, 0x0111, 0x011F}  // 2: 紧凑版 '2'
};

U16 code t_u16PixelFont_4x9[10][4] = {
    {0x00FE, 0x0101, 0x0101, 0x00FE}, // 0
    {0x0000, 0x0002, 0x0002, 0x01FF}, // 1
    {0x01E1, 0x0111, 0x0111, 0x010F}, // 2
    {0x0111, 0x0111, 0x0111, 0x00EE}, // 3
    {0x000F, 0x0010, 0x0010, 0x01FF}, // 4
    {0x011F, 0x0111, 0x0111, 0x00E1}, // 5
    {0x00FE, 0x0111, 0x0111, 0x00E2}, // 6
    {0x0001, 0x01F1, 0x0009, 0x0007}, // 7
    {0x00FE, 0x0111, 0x0111, 0x00FE}, // 8
    {0x008E, 0x0111, 0x0111, 0x00FE}  // 9
};

U16 code t_u16ErrFont_4x9[6][4] = {
    {0x01FF, 0x0011, 0x0011, 0x0001}, // 0: 'F'
    {0x01FF, 0x0010, 0x0010, 0x01FF}, // 1: 'H'
    {0x01FF, 0x0100, 0x0100, 0x0100}, // 2: 'L'
    {0x01FF, 0x0011, 0x0011, 0x000E}, // 3: 'P'
    {0x010E, 0x0111, 0x0111, 0x00E1}, // 4: 'S'
    {0x01F8, 0x0008, 0x0008, 0x01F0}  // 5: 'n'
};

// --- 2. 模式动画：基础档位与睡眠 (全矩阵渲染) ---
//U16 code t_Anim_L4_Bright[90] = {   /* 10 frames x 9 cols, shift-up + bottom wrap (9-bit rotate) */
//    0x000C, 0x001E, 0x001F, 0x001E, 0x018C, 0x01C0, 0x01E0, 0x01C0, 0x0180,  /* f0 */
//    0x0006, 0x000F, 0x010F, 0x000F, 0x00C6, 0x01E0, 0x01F0, 0x01E0, 0x00C0,  /* f1 */
//    0x0003, 0x0107, 0x0187, 0x0107, 0x0063, 0x00F0, 0x00F8, 0x00F0, 0x0060,  /* f2 */
//    0x0101, 0x0183, 0x01C3, 0x0183, 0x0131, 0x0078, 0x007C, 0x0078, 0x0030,  /* f3 */
//    0x0180, 0x01C1, 0x01E1, 0x01C1, 0x0198, 0x003C, 0x003E, 0x003C, 0x0018,  /* f4 */
//    0x00C0, 0x01E0, 0x01F0, 0x01E0, 0x00CC, 0x001E, 0x001F, 0x001E, 0x000C,  /* f5 */
//    0x0060, 0x00F0, 0x00F8, 0x00F0, 0x0066, 0x000F, 0x010F, 0x000F, 0x0006,  /* f6 */
//    0x0030, 0x0078, 0x007C, 0x0078, 0x0033, 0x0107, 0x0187, 0x0107, 0x0003,  /* f7 */
//    0x0018, 0x003C, 0x003E, 0x003C, 0x0119, 0x0183, 0x01C3, 0x0183, 0x0101,  /* f8 */
//    0x000C, 0x001E, 0x001F, 0x001E, 0x018C, 0x01C1, 0x01E1, 0x01C1, 0x0180   /* f9 = f0 (full cycle) */
//};

U16 code t_Anim_L4_Bright[90] = {   /* 10 frames x 9 cols, 10-bit rotate (1 row hidden gap) */
    0x000C, 0x001E, 0x001F, 0x001E, 0x018C, 0x01C0, 0x01E0, 0x01C0, 0x0180,  /* f0 */
    0x0006, 0x000F, 0x000F, 0x000F, 0x00C6, 0x01E0, 0x01F0, 0x01E0, 0x00C0,  /* f1 */
    0x0003, 0x0007, 0x0107, 0x0007, 0x0063, 0x00F0, 0x00F8, 0x00F0, 0x0060,  /* f2 */
    0x0001, 0x0103, 0x0183, 0x0103, 0x0031, 0x0078, 0x007C, 0x0078, 0x0030,  /* f3 */
    0x0100, 0x0181, 0x01C1, 0x0181, 0x0118, 0x003C, 0x003E, 0x003C, 0x0018,  /* f4 */
    0x0180, 0x01C0, 0x01E0, 0x01C0, 0x018C, 0x001E, 0x001F, 0x001E, 0x000C,  /* f5 */
    0x00C0, 0x01E0, 0x01F0, 0x01E0, 0x00C6, 0x000F, 0x000F, 0x000F, 0x0006,  /* f6 */
    0x0060, 0x00F0, 0x00F8, 0x00F0, 0x0063, 0x0007, 0x0107, 0x0007, 0x0003,  /* f7 */
    0x0030, 0x0078, 0x007C, 0x0078, 0x0031, 0x0103, 0x0183, 0x0103, 0x0001,  /* f8 */
    0x0018, 0x003C, 0x003E, 0x003C, 0x0118, 0x0181, 0x01C1, 0x0181, 0x0100   /* f9 */
};

// =========================================================
// L3 手动档位动画数组 (10帧 上升 底部循环)
// =========================================================
U16 code t_Anim_L3_Bright[90] = {   
    0x000C, 0x001E, 0x001D, 0x001E, 0x018C, 0x01C0, 0x01A0, 0x01C0, 0x0180,  /* f0 */
    0x0006, 0x000F, 0x000E, 0x000F, 0x00C6, 0x01E0, 0x01D0, 0x01E0, 0x00C0,  /* f1 */
    0x0003, 0x0007, 0x0107, 0x0007, 0x0063, 0x00F0, 0x00E8, 0x00F0, 0x0060,  /* f2 */
    0x0001, 0x0103, 0x0083, 0x0103, 0x0031, 0x0078, 0x0074, 0x0078, 0x0030,  /* f3 */
    0x0100, 0x0181, 0x0141, 0x0181, 0x0118, 0x003C, 0x003A, 0x003C, 0x0018,  /* f4 */
    0x0180, 0x01C0, 0x01A0, 0x01C0, 0x018C, 0x001E, 0x001D, 0x001E, 0x000C,  /* f5 */
    0x00C0, 0x01E0, 0x01D0, 0x01E0, 0x00C6, 0x000F, 0x000E, 0x000F, 0x0006,  /* f6 */
    0x0060, 0x00F0, 0x00E8, 0x00F0, 0x0063, 0x0007, 0x0107, 0x0007, 0x0003,  /* f7 */
    0x0030, 0x0078, 0x0074, 0x0078, 0x0031, 0x0103, 0x0083, 0x0103, 0x0001,  /* f8 */
    0x0018, 0x003C, 0x003A, 0x003C, 0x0118, 0x0181, 0x0141, 0x0181, 0x0100   /* f9 */
};

// =========================================================
// L2 手动档位动画数组 (10帧 上升 底部循环)
// =========================================================
U16 code t_Anim_L2_Bright[90] = {   
    0x000C, 0x001A, 0x0019, 0x001A, 0x018C, 0x0140, 0x0120, 0x0140, 0x0180,  /* f0 */
    0x0006, 0x000D, 0x000C, 0x000D, 0x00C6, 0x01A0, 0x0190, 0x01A0, 0x00C0,  /* f1 */
    0x0003, 0x0006, 0x0106, 0x0006, 0x0063, 0x00D0, 0x00C8, 0x00D0, 0x0060,  /* f2 */
    0x0001, 0x0103, 0x0083, 0x0103, 0x0031, 0x0068, 0x0064, 0x0068, 0x0030,  /* f3 */
    0x0100, 0x0081, 0x0041, 0x0081, 0x0118, 0x0034, 0x0032, 0x0034, 0x0018,  /* f4 */
    0x0180, 0x0140, 0x0120, 0x0140, 0x018C, 0x001A, 0x0019, 0x001A, 0x000C,  /* f5 */
    0x00C0, 0x01A0, 0x0190, 0x01A0, 0x00C6, 0x000D, 0x000C, 0x000D, 0x0006,  /* f6 */
    0x0060, 0x00D0, 0x00C8, 0x00D0, 0x0063, 0x0006, 0x0106, 0x0006, 0x0003,  /* f7 */
    0x0030, 0x0068, 0x0064, 0x0068, 0x0031, 0x0103, 0x0083, 0x0103, 0x0001,  /* f8 */
    0x0018, 0x0034, 0x0032, 0x0034, 0x0118, 0x0081, 0x0041, 0x0081, 0x0100   /* f9 */
};

// =========================================================
// L1 手动档位动画数组 (10帧 上升 底部循环)
// =========================================================
U16 code t_Anim_L1_Bright[90] = {   
    0x000C, 0x0012, 0x0011, 0x0012, 0x018C, 0x0240, 0x0220, 0x0240, 0x0180,  /* f0 */
    0x0006, 0x0009, 0x0008, 0x0009, 0x00C6, 0x0120, 0x0110, 0x0120, 0x00C0,  /* f1 */
    0x0003, 0x0004, 0x0104, 0x0004, 0x0063, 0x0090, 0x0088, 0x0090, 0x0060,  /* f2 */
    0x0001, 0x0102, 0x0082, 0x0102, 0x0031, 0x0048, 0x0044, 0x0048, 0x0030,  /* f3 */
    0x0100, 0x0081, 0x0041, 0x0081, 0x0118, 0x0024, 0x0022, 0x0024, 0x0018,  /* f4 */
    0x0180, 0x0240, 0x0220, 0x0240, 0x018C, 0x0012, 0x0011, 0x0012, 0x000C,  /* f5 */
    0x00C0, 0x0120, 0x0110, 0x0120, 0x00C6, 0x0009, 0x0008, 0x0009, 0x0006,  /* f6 */
    0x0060, 0x0090, 0x0088, 0x0090, 0x0063, 0x0004, 0x0104, 0x0004, 0x0003,  /* f7 */
    0x0030, 0x0048, 0x0044, 0x0048, 0x0031, 0x0102, 0x0082, 0x0102, 0x0001,  /* f8 */
    0x0018, 0x0024, 0x0022, 0x0024, 0x0118, 0x0081, 0x0041, 0x0081, 0x0100   /* f9 */
};

// ===== Static "hold" frame (Pic1 gear/mode display): one frame x 9 cols, fill in =====
U16 code t_Frame_L1[9]    = {0x0070, 0x0088, 0x0184, 0x0182, 0x0181, 0x0182, 0x0184, 0x0088, 0x0070,};
U16 code t_Frame_L2[9]    = {0x0070, 0x00E8, 0x01E4, 0x01E2, 0x01E1, 0x01E2, 0x01E4, 0x00E8, 0x0070,};
U16 code t_Frame_L3[9]    = {0x0070, 0x00F8, 0x01FC, 0x01FA, 0x01F9, 0x01FA, 0x01FC, 0x00F8, 0x0070,};
U16 code t_Frame_L4[9]    = {0x0070, 0x00F8, 0x01FC, 0x01FE, 0x01FF, 0x01FE, 0x01FC, 0x00F8, 0x0070,};
U16 code t_Frame_Sleep[9] = {0x0064, 0x00CE, 0x01C4, 0x01C0, 0x01C0, 0x01E1, 0x01FF, 0x00FE, 0x007C,};
U16 code t_Frame_Smart[9] = {0x007C, 0x0082, 0x0131, 0x0179, 0x017D, 0x0179, 0x0131, 0x0082, 0x007C,};

U16 code t_Anim_Sleep_Bright[72] = {
    0x0060, 0x00C0, 0x0140, 0x0140, 0x0140, 0x0121, 0x011F, 0x0082, 0x007C, 
	0x0064, 0x00CE, 0x0144, 0x0140, 0x0140, 0x0121, 0x011F, 0x0082, 0x007C, 
    0x0060, 0x00C0, 0x01C0, 0x01C0, 0x0140, 0x0121, 0x011F, 0x0082, 0x007C, 
    0x0064, 0x00CE, 0x01C4, 0x01C0, 0x01C0, 0x01A1, 0x011F, 0x0082, 0x007C, 
    0x0060, 0x00C0, 0x01C0, 0x01C0, 0x01C0, 0x01E1, 0x01DF, 0x0082, 0x007C, 
    0x0064, 0x00CE, 0x01C4, 0x01C0, 0x01C0, 0x01E1, 0x01FF, 0x00E2, 0x007C, 
    0x0060, 0x00C0, 0x01C0, 0x01C0, 0x01C0, 0x01E1, 0x01FF, 0x00FE, 0x007C,
	0x0064, 0x00CE, 0x01C4, 0x01C0, 0x01C0, 0x01E1, 0x01FF, 0x00FE, 0x007C,
};

// --- 3. 模式动画：智能模式 (混合图层) ---
U16 code t_Anim_Smart_Bright[81] = { 
    0x0040, 0x0080, 0x0130, 0x0179, 0x017D, 0x0179, 0x0131, 0x0082, 0x007C, 
    0x0078, 0x0080, 0x0130, 0x0178, 0x017C, 0x0178, 0x0131, 0x0082, 0x007C, 
    0x007C, 0x0082, 0x0131, 0x0178, 0x017C, 0x0178, 0x0130, 0x0080, 0x0078, 
    0x007C, 0x0082, 0x0131, 0x0179, 0x017D, 0x0179, 0x0130, 0x0080, 0x0040, 
    0x007C, 0x0082, 0x0131, 0x0179, 0x017D, 0x0179, 0x0031, 0x0002, 0x0004, 
    0x007C, 0x0082, 0x0131, 0x0079, 0x007D, 0x0079, 0x0031, 0x0002, 0x003C, 
    0x003C, 0x0002, 0x0031, 0x0079, 0x007D, 0x0079, 0x0131, 0x0082, 0x007C,  
    0x0004, 0x0002, 0x0031, 0x0179, 0x017D, 0x0179, 0x0131, 0x0082, 0x007C,
    0x007C, 0x0082, 0x0131, 0x0179, 0x017D, 0x0179, 0x0131, 0x0082, 0x007C // 确认帧
};

U8 code t_Smart_Dim_F0[] = {0x03, 0x04, 0x05, 0xFF}; 
U8 code t_Smart_Dim_F1[] = {0x02, 0x11, 0x20, 0xFF}; 
U8 code t_Smart_Dim_F2[] = {0x30, 0x40, 0x50, 0xFF}; 
U8 code t_Smart_Dim_F3[] = {0x60, 0x71, 0x82, 0xFF}; 
U8 code t_Smart_Dim_F4[] = {0x83, 0x84, 0x85, 0xFF}; 
U8 code t_Smart_Dim_F5[] = {0x68, 0x77, 0x86, 0xFF}; 
U8 code t_Smart_Dim_F6[] = {0x38, 0x48, 0x58, 0xFF}; 
U8 code t_Smart_Dim_F7[] = {0x06, 0x17, 0x28, 0xFF}; 

U8 code * code t_Anim_Smart_Dim1_Sparse[8] = {
    t_Smart_Dim_F0, t_Smart_Dim_F1, t_Smart_Dim_F2, t_Smart_Dim_F3,
    t_Smart_Dim_F4, t_Smart_Dim_F5, t_Smart_Dim_F6, t_Smart_Dim_F7
};

U8 code * code t_Anim_Smart_Dim2_Sparse[8] = {
    t_Smart_Dim_F1, t_Smart_Dim_F2, t_Smart_Dim_F3, t_Smart_Dim_F4,
    t_Smart_Dim_F5, t_Smart_Dim_F6, t_Smart_Dim_F7, t_Smart_Dim_F0
};

// --- 4. 满水与风干 (混合图层压缩) ---
U16 code t_Anim_WaterFull_Bright[27] = { 
    0x00FF, 0x01FC, 0x01FE, 0x01FE, 0x01FC, 0x01FC, 0x01FE, 0x01FE, 0x00FF, 
    0x00FF, 0x01FC, 0x01FE, 0x01FE, 0x01FC, 0x01FC, 0x01FE, 0x01FE, 0x00FF, 
    0x00FF, 0x01FE, 0x01FF, 0x01FF, 0x01FE, 0x01FE, 0x01FF, 0x01FF, 0x00FF,  
};
U8 code t_WaterFull_Dim_F0[] = {0xFF}; 
U8 code t_WaterFull_Dim_F1[] = {0x11, 0x20, 0x30, 0x41, 0x51, 0x60, 0x70, 0xFF}; 
U8 code t_WaterFull_Dim_F2[] = {0x10, 0x40, 0x50, 0xFF}; 
U8 code * code t_Anim_WaterFull_Dim1_Sparse[3] = { t_WaterFull_Dim_F0, t_WaterFull_Dim_F1, t_WaterFull_Dim_F2 };

#define AIR_DRY_ANIM_FRAME_CNT        7
#define AIR_DRY_ANIM_TICKS_PER_FRAME  3

U16 code t_Anim_AirDry_Bright[AIR_DRY_ANIM_FRAME_CNT * 9] = {
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0080, 0x0100, 0x0000, 0x0080, 0x0100, 0x0000, 0x0080, 0x0100,
    0x0040, 0x00A0, 0x0100, 0x0040, 0x00A0, 0x0100, 0x0040, 0x00A0, 0x0100,
    0x0040, 0x00A8, 0x0110, 0x0040, 0x00A8, 0x0110, 0x0040, 0x00A8, 0x0110,
    0x0044, 0x00AA, 0x0110, 0x0044, 0x00AA, 0x0110, 0x0044, 0x00AA, 0x0110,
    0x0044, 0x00AA, 0x0111, 0x0044, 0x00AA, 0x0111, 0x0044, 0x00AA, 0x0111,
};
U8 code t_AirDry_Dim1_F0[] = {0xFF};
U8 code t_AirDry_Dim1_F1[] = {0x17, 0x28, 0x47, 0x58, 0x77, 0x88, 0xFF};
U8 code t_AirDry_Dim1_F2[] = {0x06, 0x15, 0x36, 0x45, 0x66, 0x75, 0xFF};
U8 code t_AirDry_Dim1_F3[] = {0x13, 0x24, 0x43, 0x54, 0x73, 0x84, 0xFF};
U8 code t_AirDry_Dim1_F4[] = {0x02, 0x11, 0x32, 0x41, 0x62, 0x71, 0xFF};
U8 code t_AirDry_Dim1_F5[] = {0x20, 0x50, 0x80, 0xFF};
U8 code t_AirDry_Dim1_F6[] = {0xFF};
U8 code t_AirDry_Dim2_F0[] = {0x17, 0x28, 0x47, 0x58, 0x77, 0x88, 0xFF};
U8 code t_AirDry_Dim2_F1[] = {0x06, 0x15, 0x36, 0x45, 0x66, 0x75, 0xFF};
U8 code t_AirDry_Dim2_F2[] = {0x13, 0x24, 0x43, 0x54, 0x73, 0x84, 0xFF};
U8 code t_AirDry_Dim2_F3[] = {0x02, 0x11, 0x32, 0x41, 0x62, 0x71, 0xFF};
U8 code t_AirDry_Dim2_F4[] = {0x20, 0x50, 0x80, 0xFF};
U8 code t_AirDry_Dim2_F5[] = {0xFF};
U8 code t_AirDry_Dim2_F6[] = {0xFF};
U8 code * code t_Anim_AirDry_Dim1_Sparse[AIR_DRY_ANIM_FRAME_CNT] = { t_AirDry_Dim1_F0, t_AirDry_Dim1_F1, t_AirDry_Dim1_F2, t_AirDry_Dim1_F3, t_AirDry_Dim1_F4, t_AirDry_Dim1_F5, t_AirDry_Dim1_F6 };
U8 code * code t_Anim_AirDry_Dim2_Sparse[AIR_DRY_ANIM_FRAME_CNT] = { t_AirDry_Dim2_F0, t_AirDry_Dim2_F1, t_AirDry_Dim2_F2, t_AirDry_Dim2_F3, t_AirDry_Dim2_F4, t_AirDry_Dim2_F5, t_AirDry_Dim2_F6 };
// --- 5. 其他状态动画 (缺水/清洁/水箱提离) ---
U16 code t_Anim_WaterLack_Bright[27] = { 
    0x00FF, 0x0180, 0x01C0, 0x01C0, 0x0180, 0x0180, 0x01C0, 0x01C0, 0x00FF, 
    0x00FF, 0x0100, 0x0180, 0x0180, 0x0100, 0x0100, 0x0180, 0x0180, 0x00FF, 
    0x00FF, 0x0100, 0x0180, 0x0180, 0x015F, 0x0100, 0x0180, 0x0180, 0x00FF,  
};

U16 code t_Anim_Clean_Bright[18] = { 
    0x0000, 0x00FE, 0x0101, 0x0101, 0x0000, 0x01FF, 0x0100, 0x0100, 0x0000, 
    0x0022, 0x0020, 0x0070, 0x01FC, 0x0070, 0x0020, 0x0022, 0x0007, 0x0002,   
};

U16 code t_Anim_Tank_F1[9] = {0x0000, 0x013F, 0x0140, 0x0140, 0x0140, 0x0140, 0x0140, 0x013F, 0x0000}; 
U16 code t_Anim_Tank_F2[9] = {0x0000, 0x010F, 0x0110, 0x0110, 0x0110, 0x0110, 0x0110, 0x010F, 0x0000}; 
U16 code t_Anim_Tank_F3[9] = {0x0000, 0x0103, 0x0104, 0x0104, 0x0104, 0x0104, 0x0104, 0x0103, 0x0000}; 
U16 code t_Anim_Tank_F4[9] = {0x0000, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0000}; 
U16 code t_Anim_Tank_Shared_A[9] = {0x0000, 0x013F, 0x0140, 0x0140, 0x0157, 0x0140, 0x0140, 0x013F, 0x0000}; 
U16 code t_Anim_Tank_Shared_B_Bright[9] = {0x0000, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0000}; 
U16 code t_Anim_Tank_Shared_B_Dim1[9]   = {0x0000, 0x003F, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x003F, 0x0000};


// =================================================================================
// ZONE 4: 内部函数声明与任务生命周期表
// =================================================================================
static void F_DispNormal(void);
static void F_DispTimeSet(void);
static void F_DispPowerUp(void);
static void F_DisplayUpData(void);
static void F_DispWaterLack(void);
static void F_DispWaterFull(void);
static void F_DispCleanRemind(void);
static void F_DispAirDry(void);
static void F_DispTankRemoved(void);
static void F_DispQuickTest(void);
static void F_DispErrState(void);

static void F_TMS_ShowTimerUI(U8 u8Hour, U8 u8Bright);
static void F_TMS_ClearArea(TYPE_DIGITAL_AREA *l_pArea, U8 l_u8Width);

typedef struct 
{
    unsigned char Index;
    unsigned int  LastTime;
    unsigned char Blink;
} TYPE_DISP_TASK;

code TYPE_DISP_TASK DispTskTbl[DISP_TOTAL_TASK] = 
{
    {DIPS_NORMAL,      0,    0}, 
    {DIPS_POWERUP,     11,   0},
    {DIPS_TIME_SET,    30,   0}, 
    {DIPS_WATER_LACK,  0,    0}, 
    {DIPS_WATER_FULL,  100,  0}, 
    {DIPS_AIR_DRY,     0,    0}, 
    {DIPS_CLEAN_REMIND,50,   0},
    {DIPS_QUICK_TEST,  30,   0},
    {DIPS_TANK_REMOVED, 0,   0},
    {DIPS_MODE_SET,     0,   0},   // 3s hold + one-shot anim, managed inside F_DispModeSet
};


// =================================================================================
// ZONE 5: 底层绘图 API (HAL Primitives)
// =================================================================================
U8 F_GetPWMValue(void)
{
    if (g_stSys.u8DispBright > C_DISP_BRIGHT_MAX) return 255;
    return t_u8BrightGammaMap[g_stSys.u8DispBright];
}

void F_DispClean(void)
{
    U8 i, j;
    for(i=0; i<9; i++)
        for(j=0; j<16; j++) g_t_u8TMS6644_DispBuf[i][j] = 0;
}


void F_TMS_Mapping_Write(TYPE_DIGITAL_AREA *l_pArea, U8 l_u8LogicCol, U16 l_u16ColData)
{
    U8 l_u8Y, l_u8PhyCom, l_u8PhySeg;
    U8 l_u8PwmVal = F_GetPWMValue();
    l_u8PwmVal = (U8)(((U16)l_u8PwmVal * C_MATRIX_BRIGHT_PCT) / 100);  /* dim dot-matrix */

    l_u8PhyCom = l_pArea->pColToComMap[l_u8LogicCol];

    for (l_u8Y = 0; l_u8Y < C_FONT_HEIGHT; l_u8Y++)
    {
        l_u8PhySeg = l_pArea->pRowToSegMap[l_u8Y];
        if (l_u8PhyCom < 9 && l_u8PhySeg < 16)
        {
            if (l_u16ColData & (1 << l_u8Y))
                g_t_u8TMS6644_DispBuf[l_u8PhyCom][l_u8PhySeg] = l_u8PwmVal;
            else
                g_t_u8TMS6644_DispBuf[l_u8PhyCom][l_u8PhySeg] = 0;
        }
    }
}

static void F_TMS_ClearArea(TYPE_DIGITAL_AREA *l_pArea, U8 l_u8Width)
{
    U8 x;
    for (x = 0; x < l_u8Width; x++)
        F_TMS_Mapping_Write(l_pArea, x, 0x0000);
}

void F_TMS_Mapping_Write_Ex(TYPE_DIGITAL_AREA *l_pArea, U8 l_u8LogicCol, U16 l_u16ColData, U8 u8BrightLevel)
{
    U8 l_u8Y, l_u8PhyCom, l_u8PhySeg;
    U8 l_u8PwmVal;

    if(u8BrightLevel > C_DISP_BRIGHT_MAX) u8BrightLevel = C_DISP_BRIGHT_MAX;
    l_u8PwmVal = t_u8BrightGammaMap[u8BrightLevel];
    l_u8PwmVal = (U8)(((U16)l_u8PwmVal * C_MATRIX_BRIGHT_PCT) / 100);  /* dim dot-matrix */

    l_u8PhyCom = l_pArea->pColToComMap[l_u8LogicCol];

    for (l_u8Y = 0; l_u8Y < C_FONT_HEIGHT; l_u8Y++)
    {
        l_u8PhySeg = l_pArea->pRowToSegMap[l_u8Y];
        if (l_u8PhyCom < 9 && l_u8PhySeg < 16)
        {
            if (l_u16ColData & (1 << l_u8Y))
            {
                g_t_u8TMS6644_DispBuf[l_u8PhyCom][l_u8PhySeg] = l_u8PwmVal;
            }
        }
    }
}

void F_TMS_DrawSparse(TYPE_DIGITAL_AREA *l_pArea, U8 code *pSparseData, U8 u8BrightLevel)
{
    U8 i = 0;
    U8 col, row, phyCom, phySeg;
    U8 u8PwmVal;
    
    if(u8BrightLevel > C_DISP_BRIGHT_MAX) u8BrightLevel = C_DISP_BRIGHT_MAX;
    u8PwmVal = t_u8BrightGammaMap[u8BrightLevel];
    u8PwmVal = (U8)(((U16)u8PwmVal * C_MATRIX_BRIGHT_PCT) / 100);  /* dim dot-matrix */

    while(pSparseData[i] != 0xFF)
    {
        col = pSparseData[i] >> 4;   
        row = pSparseData[i] & 0x0F; 
        
        phyCom = l_pArea->pColToComMap[col];
        phySeg = l_pArea->pRowToSegMap[row];
        
        if (phyCom < 9 && phySeg < 16)
        {
            g_t_u8TMS6644_DispBuf[phyCom][phySeg] = u8PwmVal;
        }
        i++;
    }
}

void F_TMS_SetLed(U8 l_u8LedIndex, bit l_bState)
{
    U8 i, u8Count, u8Com, u8Seg, u8Pwm;
    if (l_u8LedIndex >= _LED_TOTAL_) return;

    u8Pwm = l_bState ? F_GetPWMValue() : 0;
    u8Count = t_stLedMap[l_u8LedIndex].u8Count;
	if(l_u8LedIndex > L_POWER_KEY && u8Pwm)
	{
		u8Pwm = (U8)(((U16)u8Pwm * C_MATRIX_BRIGHT_PCT) / 100); 
	}
	
    for(i = 0; i < u8Count; i++)
    {
        u8Com = t_stLedMap[l_u8LedIndex].pList[i].u8Com;
        u8Seg = t_stLedMap[l_u8LedIndex].pList[i].u8Seg;
        if (u8Com < 9 && u8Seg < 16)
            g_t_u8TMS6644_DispBuf[u8Com][u8Seg] = u8Pwm;
    }
}

void F_TMS_SetLed_Ex(U8 l_u8LedIndex, U8 u8BrightLevel)
{
    U8 i, u8Count, u8Com, u8Seg, u8Pwm;
    if (l_u8LedIndex >= _LED_TOTAL_) return;

    if(u8BrightLevel > C_DISP_BRIGHT_MAX) u8BrightLevel = C_DISP_BRIGHT_MAX;
    u8Pwm = t_u8BrightGammaMap[u8BrightLevel];
	if(l_u8LedIndex > L_POWER_KEY && u8Pwm)
	{
		u8Pwm = (U8)(((U16)u8Pwm * C_MATRIX_BRIGHT_PCT) / 100); 
	}
    u8Count = t_stLedMap[l_u8LedIndex].u8Count;

    for(i = 0; i < u8Count; i++)
    {
        u8Com = t_stLedMap[l_u8LedIndex].pList[i].u8Com;
        u8Seg = t_stLedMap[l_u8LedIndex].pList[i].u8Seg;
        if (u8Com < 9 && u8Seg < 16)
        {
            g_t_u8TMS6644_DispBuf[u8Com][u8Seg] = u8Pwm;
        }
    }
}

void F_TMS_ShowNum(TYPE_DIGITAL_AREA *l_pArea, U16 l_u16Num)
{
    U8 i, x, u8LogicCol = 0;
    U8 t_Data[4];

    if (l_pArea->u8DigitCount == 2)
    {
        if (l_u16Num > 99) l_u16Num = 99;
        t_Data[0] = l_u16Num / 10;
        t_Data[1] = l_u16Num % 10;
    }

    for (i = 0; i < l_pArea->u8DigitCount; i++)
    {
        for (x = 0; x < C_FONT_WIDTH; x++)
            F_TMS_Mapping_Write(l_pArea, u8LogicCol + x, t_u16PixelFont_4x9[t_Data[i]][x]);
        u8LogicCol += (C_FONT_WIDTH + 1); 
    }
}

static void F_TMS_ShowHumi(U8 l_u8Humi)
{
    U8 x;
    if(l_u8Humi > 99) l_u8Humi = 99;
    if(l_u8Humi < 10)
    {
        for(x = 0; x < C_FONT_WIDTH; x++)
            F_TMS_Mapping_Write(&g_Area_Number, (C_FONT_WIDTH + 1) + x, t_u16PixelFont_4x9[l_u8Humi][x]);
    }
    else
    {
        F_TMS_ShowNum(&g_Area_Number, l_u8Humi);
    }
}

static void F_TMS_ShowTimerUI(U8 u8Hour, U8 u8Bright)
{
    U8 u8Col = 0;
    U8 u8Digit = 0; 
    
    if (u8Hour < 10) 
    {
        for(u8Col = 0; u8Col < 4; u8Col++)
            F_TMS_Mapping_Write_Ex(&g_Area_Number, 1 + u8Col, t_u16PixelFont_4x9[u8Hour][u8Col], u8Bright);
            
        for(u8Col = 0; u8Col < 3; u8Col++)
            F_TMS_Mapping_Write_Ex(&g_Area_Number, 6 + u8Col, t_Font_h_3x6[u8Col], u8Bright);
    }
    else 
    {
        F_TMS_Mapping_Write_Ex(&g_Area_Number, 0, 0x01FF, u8Bright);
        
        u8Digit = u8Hour % 10; 
        for(u8Col = 0; u8Col < 3; u8Col++)
            F_TMS_Mapping_Write_Ex(&g_Area_Number, 2 + u8Col, t_Font_Timer_3x9[u8Digit][u8Col], u8Bright);
            
        for(u8Col = 0; u8Col < 3; u8Col++)
            F_TMS_Mapping_Write_Ex(&g_Area_Number, 6 + u8Col, t_Font_h_3x6[u8Col], u8Bright);
    }
}

void F_TMS_ShowErrChars(U8 u8Char1, U8 u8Char2)
{
    U8 x, u8LogicCol = 0;
    
    for (x = 0; x < C_FONT_WIDTH; x++)
        F_TMS_Mapping_Write(&g_Area_Number, u8LogicCol + x, t_u16ErrFont_4x9[u8Char1][x]);
        
    u8LogicCol += (C_FONT_WIDTH + 1); 
    
    for (x = 0; x < C_FONT_WIDTH; x++)
        F_TMS_Mapping_Write(&g_Area_Number, u8LogicCol + x, t_u16ErrFont_4x9[u8Char2][x]);
}


// =================================================================================
// ZONE 6: 业务显示任务 (App Layer)
// =================================================================================
void F_DispResetAnim(void)
{
    s_u8NormalState = NSTATE_CONFIRM; 
    s_u16NormalTimer = 0;             
}

static void F_DispPowerUp(void)
{
    U8 i, j, pwm = F_GetPWMValue();
    for(i=0; i<9; i++)
        for(j=0; j<16; j++) g_t_u8TMS6644_DispBuf[i][j] = pwm;
}

static void F_DispNormal(void)
{
    U8 u8FrameIdx = 0;
    U8 u8Col = 0;
    U8 u8TotalFrames = 0; 
    
    U8 u8Bright = 0;
    U8 u8Dim1 = 0;
    U8 u8Dim2 = 0;
    
    // 关机半亮显示开关键
    if(!g_stSys.bTurnOn) 
    {
        F_TMS_SetLed_Ex(L_POWER_KEY, (g_stSys.u8DispBright * 6) / 10);
        return;
    }

    // 公共图标常亮
    F_TMS_SetLed(L_LIGHT_KEY, 1);
    F_TMS_SetLed(L_TIME_KEY, 1);
    F_TMS_SetLed(L_MODE_KEY, 1);
    F_TMS_SetLed(L_POWER_KEY, 1);
    
    if(g_stSys.u8TimeSetData > 0) F_TMS_SetLed(L_TIME, 1);
    if(g_stSys.bWaterLack) F_TMS_SetLed(L_WATER_LACK, 1);
    if(g_stSys.bCleanRemind) F_TMS_SetLed(L_CLEAN_REMIND, 1); 

    // 拦截高优先级状态
    if(g_stSys.bIsAirDrying)
    {
        F_TMS_SetLed(L_AIR_DRY, 1);
        F_DispResetAnim();
        return;
    }

    /* V2.6: normal display = humidity only (Pic3); animation moved to DIPS_MODE_SET */
    F_TMS_ShowHumi(g_stSys.u8DispHumi);
    F_TMS_SetLed(L_HUMI_PERCENT, 1);
    return;
}

// ===== mode/gear setting display: hold frame 3s, then (if switched) play anim three loops =====
#define MSTATE_HOLD  0
#define MSTATE_ANIM  1
#define MODE_ANIM_LOOP_CNT  3
static U8  xdata s_u8MSState   = MSTATE_HOLD;
static U16 xdata s_u16MSTimer  = 0;
static bit s_bMSSwitched       = 0;

// mode key first press: enter setting state, no switch yet
void F_DispEnterModeSet(void)
{
    s_u8MSState   = MSTATE_HOLD;
    s_u16MSTimer  = 0;
    s_bMSSwitched = 0;
    F_DispTskChange(DIPS_MODE_SET);
}

// mode key cycle inside setting: restart 3s hold, mark a real switch
void F_DispModeSwitched(void)
{
    s_u8MSState   = MSTATE_HOLD;
    s_u16MSTimer  = 0;
    s_bMSSwitched = 1;
    F_DispTskChange(DIPS_MODE_SET);
}

static void F_DispModeSet(void)
{
    U8 u8FrameIdx = 0, u8Col = 0, u8TotalFrames = 10;
    U8 u8Bright = g_stSys.u8DispBright;
    U8 u8Dim1 = 0, u8Dim2 = 0;
    U16 code *pFrame = t_Frame_L4;

    // common keys + status LEDs (same as normal display)
    F_TMS_SetLed(L_LIGHT_KEY, 1);
    F_TMS_SetLed(L_TIME_KEY, 1);
    F_TMS_SetLed(L_MODE_KEY, 1);
    F_TMS_SetLed(L_POWER_KEY, 1);
    if(g_stSys.u8TimeSetData > 0) F_TMS_SetLed(L_TIME, 1);
    if(g_stSys.bWaterLack)        F_TMS_SetLed(L_WATER_LACK, 1);
    if(g_stSys.bCleanRemind)      F_TMS_SetLed(L_CLEAN_REMIND, 1);

    switch(g_stSys.u8WorkMode)
    {
        case MODE_SMART: u8TotalFrames = 8;  break;
        case MODE_SLEEP: u8TotalFrames = 8;  break;
        default:         u8TotalFrames = 10; break;   // manual L1..L4
    }

    if(s_u8MSState == MSTATE_HOLD)
    {
        // Pic1: static hold frame of the current mode
        switch(g_stSys.u8WorkMode)
        {
            case MODE_MANUAL_1: pFrame = t_Frame_L1;    break;
            case MODE_MANUAL_2: pFrame = t_Frame_L2;    break;
            case MODE_MANUAL_3: pFrame = t_Frame_L3;    break;
            case MODE_MANUAL_4: pFrame = t_Frame_L4;    break;
            case MODE_SLEEP:    pFrame = t_Frame_Sleep; break;
            case MODE_SMART:    pFrame = t_Frame_Smart; break;
            default:            pFrame = t_Frame_L4;    break;
        }
        for(u8Col = 0; u8Col < 9; u8Col++)
            F_TMS_Mapping_Write_Ex(&g_Area_Icon9x9, u8Col, pFrame[u8Col], u8Bright);

        if(++s_u16MSTimer >= 30)        // 3s hold
        {
            if(s_bMSSwitched)           // real switch happened -> play anim three loops
            {
                s_u8MSState  = MSTATE_ANIM;
                s_u16MSTimer = 0;
            }
            else                        // just entered, no switch -> back to humidity
            {
                F_DispTskChange(DIPS_NORMAL);
            }
        }
    }
    else   // MSTATE_ANIM: play the mode animation three loops
    {
        u8FrameIdx = (s_u16MSTimer / 3) % u8TotalFrames;  // 3 ticks per frame = 300ms

        switch(g_stSys.u8WorkMode)
        {
            case MODE_MANUAL_1:
                for(u8Col = 0; u8Col < 9; u8Col++)
                    F_TMS_Mapping_Write_Ex(&g_Area_Icon9x9, u8Col, t_Anim_L1_Bright[u8FrameIdx * 9 + u8Col], u8Bright);
                break;
            case MODE_MANUAL_2:
                for(u8Col = 0; u8Col < 9; u8Col++)
                    F_TMS_Mapping_Write_Ex(&g_Area_Icon9x9, u8Col, t_Anim_L2_Bright[u8FrameIdx * 9 + u8Col], u8Bright);
                break;
            case MODE_MANUAL_3:
                for(u8Col = 0; u8Col < 9; u8Col++)
                    F_TMS_Mapping_Write_Ex(&g_Area_Icon9x9, u8Col, t_Anim_L3_Bright[u8FrameIdx * 9 + u8Col], u8Bright);
                break;
            case MODE_MANUAL_4:
                for(u8Col = 0; u8Col < 9; u8Col++)
                    F_TMS_Mapping_Write_Ex(&g_Area_Icon9x9, u8Col, t_Anim_L4_Bright[u8FrameIdx * 9 + u8Col], u8Bright);
                break;
            case MODE_SMART:
                u8Dim1 = (u8Bright * 6) / 10;
                u8Dim2 = (u8Bright * 2) / 10;
                if(u8Bright > 0)
                {
                    if(u8Dim1 == 0) u8Dim1 = 1;
                    if(u8Dim2 == 0) u8Dim2 = 1;
                }
                F_TMS_DrawSparse(&g_Area_Icon9x9, t_Anim_Smart_Dim2_Sparse[u8FrameIdx], u8Dim2);
                F_TMS_DrawSparse(&g_Area_Icon9x9, t_Anim_Smart_Dim1_Sparse[u8FrameIdx], u8Dim1);
                for(u8Col = 0; u8Col < 9; u8Col++)
                    F_TMS_Mapping_Write_Ex(&g_Area_Icon9x9, u8Col, t_Anim_Smart_Bright[u8FrameIdx * 9 + u8Col], u8Bright);
                break;
            case MODE_SLEEP:
                for(u8Col = 0; u8Col < 9; u8Col++)
                    F_TMS_Mapping_Write_Ex(&g_Area_Icon9x9, u8Col, t_Anim_Sleep_Bright[u8FrameIdx * 9 + u8Col], u8Bright);
                break;
        }

        if(++s_u16MSTimer >= (u8TotalFrames * 3 * MODE_ANIM_LOOP_CNT))
            F_DispTskChange(DIPS_NORMAL);   // anim done -> humidity
    }
}

static void F_DispTimeSet(void)
{
    U8 bTimeShow = ((s_u8DispLastT % 10) >= 5) ? 1 : 0;

    if(bTimeShow)
    {
        F_TMS_ShowTimerUI(g_stSys.u8TimeSetData, g_stSys.u8DispBright);
        F_TMS_SetLed(L_TIME, 1); 
    }

    F_TMS_SetLed(L_LIGHT_KEY, 1);
    F_TMS_SetLed(L_TIME_KEY, 1);
    F_TMS_SetLed(L_MODE_KEY, 1);
    F_TMS_SetLed(L_POWER_KEY, 1);
    
    if(g_stSys.bWaterLack) F_TMS_SetLed(L_WATER_LACK, 1);
}

static void F_DispWaterLack(void)
{
    U8 u8Step, u8FrameIdx, u8Col;

    /* point5: play [frame3, frame2] (idx2, idx1) x 3 loops = 6 frames x 300ms = 1.8s,
       then enter air-dry. Each frame dwells 3 ticks (100ms each). */
    u8Step     = s_u8WLackTick / 3;          /* 0..5 */
    u8FrameIdx = (u8Step & 1) ? 1 : 2;       /* even step -> idx2(frame3), odd -> idx1(frame2) */

    for(u8Col = 0; u8Col < 9; u8Col++)
        F_TMS_Mapping_Write_Ex(&g_Area_Icon9x9, u8Col, t_Anim_WaterLack_Bright[u8FrameIdx * 9 + u8Col], g_stSys.u8DispBright);

    F_TMS_SetLed(L_WATER_LACK, 1);   /* spec: water-lack LED solid on */
    F_TMS_SetLed(L_LIGHT_KEY, 1);
    F_TMS_SetLed(L_TIME_KEY, 1);
    F_TMS_SetLed(L_MODE_KEY, 1);
    F_TMS_SetLed(L_POWER_KEY, 1);

    /* air-dry trigger moved to F_DisplayUpData always-run (works while screen off) */
}

static void F_DispWaterFull(void)
{
    U8 u8PassTicks = 0, u8FrameIdx = 0, u8Col = 0;
    U8 u8Bright = 0, u8Dim1 = 0;

    u8Bright = g_stSys.u8DispBright;
    u8Dim1 = (u8Bright * 6) / 10; 
    if(u8Bright > 0 && u8Dim1 == 0) u8Dim1 = 1; 

    u8PassTicks = 100 - s_u8DispLastT;
    u8FrameIdx = (u8PassTicks / 3) % 3; 

    F_TMS_DrawSparse(&g_Area_Icon9x9, t_Anim_WaterFull_Dim1_Sparse[u8FrameIdx], u8Dim1);
    
    for(u8Col = 0; u8Col < 9; u8Col++)
    {
        F_TMS_Mapping_Write_Ex(&g_Area_Icon9x9, u8Col, t_Anim_WaterFull_Bright[u8FrameIdx * 9 + u8Col], u8Bright);
    }

    F_TMS_SetLed(L_LIGHT_KEY, 1);
    F_TMS_SetLed(L_TIME_KEY, 1);
    F_TMS_SetLed(L_MODE_KEY, 1);
    F_TMS_SetLed(L_POWER_KEY, 1);
}

static void F_DispAirDry(void)
{
    static U8 s_u8Tick = 0;
    U8 u8FrameIdx = 0, u8Col = 0;
    U8 u8Bright = 0, u8Dim1 = 0, u8Dim2 = 0;
    U8 bBlinkOn = 0;
    
    u8Bright = g_stSys.u8DispBright;
    u8Dim1 = (u8Bright * 6) / 10; 
    u8Dim2 = (u8Bright * 2) / 10; 
    if(u8Bright > 0)
    {
        if(u8Dim1 == 0) u8Dim1 = 1;
        if(u8Dim2 == 0) u8Dim2 = 1;
    }

    if(++s_u8Tick >= (AIR_DRY_ANIM_FRAME_CNT * AIR_DRY_ANIM_TICKS_PER_FRAME)) s_u8Tick = 0; 
    u8FrameIdx = s_u8Tick / AIR_DRY_ANIM_TICKS_PER_FRAME;

    F_TMS_DrawSparse(&g_Area_Icon9x9, t_Anim_AirDry_Dim2_Sparse[u8FrameIdx], u8Dim2);
    F_TMS_DrawSparse(&g_Area_Icon9x9, t_Anim_AirDry_Dim1_Sparse[u8FrameIdx], u8Dim1);

    for(u8Col = 0; u8Col < 9; u8Col++)
    {
        F_TMS_Mapping_Write_Ex(&g_Area_Icon9x9, u8Col, t_Anim_AirDry_Bright[u8FrameIdx * 9 + u8Col], u8Bright);
    }
    
    if(g_stSys.u8AirDryWarnTick > 0)
    {
        g_stSys.u8AirDryWarnTick--; 
        bBlinkOn = (((g_stSys.u8AirDryWarnTick - 1) / 5) % 2 != 0);
        F_TMS_SetLed(L_AIR_DRY, bBlinkOn ? 1 : 0);
    }
    else
    {
        F_TMS_SetLed(L_AIR_DRY, 1); 
    }
    
    if(g_stSys.bWaterLack) F_TMS_SetLed(L_WATER_LACK, 1); 
    F_TMS_SetLed(L_LIGHT_KEY, 1);
    F_TMS_SetLed(L_TIME_KEY, 1);
    F_TMS_SetLed(L_MODE_KEY, 1);
    F_TMS_SetLed(L_POWER_KEY, 1);
}

static void F_DispCleanRemind(void)
{
    static U8 s_u8Tick = 0;
    U8 u8FrameIdx = 0, u8Col = 0;

    if(++s_u8Tick >= 10) s_u8Tick = 0; 
    u8FrameIdx = s_u8Tick / 5;

    for(u8Col = 0; u8Col < 9; u8Col++)
    {
        F_TMS_Mapping_Write_Ex(&g_Area_Icon9x9, u8Col, t_Anim_Clean_Bright[u8FrameIdx * 9 + u8Col], g_stSys.u8DispBright);
    }

    F_TMS_SetLed(L_CLEAN_REMIND, 1); 
    F_TMS_SetLed(L_LIGHT_KEY, 1);
    F_TMS_SetLed(L_TIME_KEY, 1);
    F_TMS_SetLed(L_MODE_KEY, 1);
    F_TMS_SetLed(L_POWER_KEY, 1);
}

static void F_DispTankRemoved(void)
{
    static U8 s_u8Tick = 0;
    U8 u8FrameIdx = 0;
    U8 u8Col = 0;
    U8 u8Bright = g_stSys.u8DispBright;
    U8 u8Dim1 = (u8Bright * 4) / 10;
    
    if(u8Bright > 0 && u8Dim1 == 0) u8Dim1 = 1;

    if(++s_u8Tick >= 30) s_u8Tick = 0;
    u8FrameIdx = s_u8Tick / 3;

    switch(u8FrameIdx)
    {
        case 0: 
            for(u8Col = 0; u8Col < 9; u8Col++) 
                F_TMS_Mapping_Write_Ex(&g_Area_Icon9x9, u8Col, t_Anim_Tank_F1[u8Col], u8Bright);
            break;
        case 1: 
            for(u8Col = 0; u8Col < 9; u8Col++) 
                F_TMS_Mapping_Write_Ex(&g_Area_Icon9x9, u8Col, t_Anim_Tank_F2[u8Col], u8Bright);
            break;
        case 2: 
            for(u8Col = 0; u8Col < 9; u8Col++) 
                F_TMS_Mapping_Write_Ex(&g_Area_Icon9x9, u8Col, t_Anim_Tank_F3[u8Col], u8Bright);
            break;
        case 3: 
            for(u8Col = 0; u8Col < 9; u8Col++) 
                F_TMS_Mapping_Write_Ex(&g_Area_Icon9x9, u8Col, t_Anim_Tank_F4[u8Col], u8Bright);
            break;
        case 4: 
        case 6: 
        case 8: 
            for(u8Col = 0; u8Col < 9; u8Col++) 
                F_TMS_Mapping_Write_Ex(&g_Area_Icon9x9, u8Col, t_Anim_Tank_Shared_A[u8Col], u8Bright);
            break;
        case 5: 
        case 7: 
        case 9: 
            for(u8Col = 0; u8Col < 9; u8Col++) 
            {
                F_TMS_Mapping_Write_Ex(&g_Area_Icon9x9, u8Col, t_Anim_Tank_Shared_B_Dim1[u8Col], u8Dim1);
                F_TMS_Mapping_Write_Ex(&g_Area_Icon9x9, u8Col, t_Anim_Tank_Shared_B_Bright[u8Col], u8Bright);
            }
            break;
    }

    F_TMS_SetLed(L_LIGHT_KEY, 1);
    F_TMS_SetLed(L_TIME_KEY, 1);
    F_TMS_SetLed(L_MODE_KEY, 1);
    F_TMS_SetLed(L_POWER_KEY, 1);
}

void F_DispQuickTest()
{
    U8 i, j, pwm = F_GetPWMValue();
    if(s_u8DispLastT > 15)
    {   
        for(i=0; i<9; i++)
        for(j=0; j<16; j++) g_t_u8TMS6644_DispBuf[i][j] = pwm;
    }
    else
    {
        F_TMS_ShowNum(&g_Area_Number, 0x05); 
    }
}

void F_DispErrState()
{
    if(g_bSnCodeError)
    {   
        F_TMS_ShowErrChars(CH_S, CH_n); 
    }
    else if(g_stSys.bQuickTest && (g_stSys.bHumiErr || fgBlueError || fgWifiError_AP))
    {
        if(fgBlueError)          F_TMS_ShowErrChars(CH_F, CH_L); 
        else if(fgWifiError_AP)  F_TMS_ShowErrChars(CH_F, CH_P); 
        else                     F_TMS_ShowErrChars(CH_F, CH_H); 
    }
}

U8 F_ReadErrState()
{
    if(g_bSnCodeError) return 1;
    else if(g_stSys.bQuickTest && (g_stSys.bHumiErr || fgBlueError || fgWifiError_AP)) return 1;   
    return 0;
}


// =================================================================================
// ZONE 7: 核心调度系统 (Core System)
// =================================================================================
void F_mDispInital(void)
{
    F_DispTskChange(DIPS_POWERUP);
}

void F_DispTskChange(unsigned char u8TaskIndex)
{
    if(u8TaskIndex < DISP_TOTAL_TASK)
    {
        if(u8TaskIndex == DIPS_TIME_SET)
        {
            if(s_u8DispIndex == DIPS_TIME_SET) s_bTimeModified = 1; 
            else s_bTimeModified = 0; 
        }
        s_u8DispIndex = u8TaskIndex;
        s_u8DispLastT = DispTskTbl[u8TaskIndex].LastTime;
        s_u8DispBlink = DispTskTbl[u8TaskIndex].Blink;
        s_bDispMask = 0;
        if(u8TaskIndex == DIPS_WATER_LACK) s_u8WLackTick = 0;   /* point5: restart water-lack anim on entry */
    }
    if(u8TaskIndex == DIPS_NORMAL)
    {
        F_DispResetAnim(); 
    }
}

U8 F_GetDispType(void)
{
    return DispTskTbl[s_u8DispIndex].Index;
}

void F_DispSetTimeModified(void)
{
    s_bTimeModified = 1;
}

static void F_DisplayUpData(void)
{
    U8 bShouldScreenOff = 0;
    U8 bIsErrState = 0;
    if(g_stSys.u16Cnt_WakeDisp) g_stSys.u16Cnt_WakeDisp--;
    if(g_stSys.u8Cnt_TimeSet)  g_stSys.u8Cnt_TimeSet--;

    /* always-run (screen on or off): water-lack timing -> air-dry */
    if(g_stSys.bTurnOn && s_u8DispIndex == DIPS_WATER_LACK && g_stSys.bWaterLack && !g_stSys.bIsAirDrying)
    {
        if(++s_u8WLackTick >= 18)
        {
            s_u8WLackTick = 0;
            F_EnterAirDry();
            F_DispTskChange(DIPS_AIR_DRY);
        }
    }

    if(s_u8DispLastT)
    {
        s_u8DispLastT--;
        if(s_u8DispLastT == 0) 
        {
            if(s_u8DispIndex == DIPS_TIME_SET)
            {
                if(s_bTimeModified && g_stSys.u16TimeCnt > 0) BUZ_TIMESET(); 
            }
            if(g_stSys.bTurnOn)
            {
                if(g_stSys.bTankRemoved)      F_DispTskChange(DIPS_TANK_REMOVED); 
                else if(g_stSys.bIsAirDrying) F_DispTskChange(DIPS_AIR_DRY);      
                else if(g_stSys.bWaterLack)   F_DispTskChange(DIPS_WATER_LACK); 
                else                          F_DispTskChange(DIPS_NORMAL);      
            }
            else
            {
                F_DispTskChange(DIPS_NORMAL);
            }
        }
    }

    F_DispClean(); 
    bIsErrState = F_ReadErrState();

    if(g_stSys.bScreenAlwaysOn == 0 && g_stSys.u16Cnt_WakeDisp == 0)
    {
        bShouldScreenOff = 1;
        if(s_u8DispIndex == DIPS_CLEAN_REMIND || 
           s_u8DispIndex == DIPS_WATER_FULL   || 
           s_u8DispIndex == DIPS_TIME_SET     || 
           s_u8DispIndex == DIPS_TANK_REMOVED || 
           s_u8DispIndex == DIPS_QUICK_TEST)     
        {
            bShouldScreenOff = 0; 
        }
    }

    if(bShouldScreenOff && g_stSys.bTurnOn)
    {
        if(g_stSys.u8WorkMode == MODE_SLEEP) 
        {
            // Sleep mode logic...
        }
        F_TMS_SetLed_Ex(L_POWER_KEY, (g_stSys.u8DispBright * 6) / 10);
        
        if(g_stSys.bWaterLack)   F_TMS_SetLed_Ex(L_WATER_LACK, (g_stSys.u8DispBright * 3) / 10);
        if(g_stSys.bIsAirDrying) F_TMS_SetLed_Ex(L_AIR_DRY, (g_stSys.u8DispBright * 3) / 10);
    }
    else
    {
        if(S_API_WifiDisp() && !bIsErrState) F_TMS_SetLed_Ex(L_WIFI, g_stSys.u8DispBright);
        
        if(s_u8DispIndex == DIPS_QUICK_TEST)
        {
            F_DispQuickTest();
        }
        else if(bIsErrState && (s_u8DispIndex != DIPS_POWERUP))
        {   
            F_DispErrState();
        }
        else if(s_bDispMask == 0) 
        {
            switch(s_u8DispIndex)
            {
                case DIPS_NORMAL:       F_DispNormal();         break;
                case DIPS_POWERUP:      F_DispPowerUp();        break;
                case DIPS_TIME_SET:     F_DispTimeSet();        break;
                case DIPS_WATER_LACK:   F_DispWaterLack();      break;
                case DIPS_WATER_FULL:   F_DispWaterFull();      break;
                case DIPS_AIR_DRY:      F_DispAirDry();         break;
                case DIPS_CLEAN_REMIND: F_DispCleanRemind();    break;
                case DIPS_TANK_REMOVED: F_DispTankRemoved();    break;
                case DIPS_MODE_SET:     F_DispModeSet();        break;
                default:                F_DispNormal();         break;
            }
        } 
    }
}

void F_mDisplayControl(void)
{
    if(F_GetTimeTick(C_TIME_100ms))
    {
        if(g_u8TestState)
        {
            F_SelfTestDislayUpData();
        }
        else
        {
            F_DisplayUpData();
        }
        F_TMS6644_Display_PWM(&g_t_u8TMS6644_DispBuf[0][0], 144);
    }
}