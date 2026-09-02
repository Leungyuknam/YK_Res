#include "h\Allinclude.h"

// 【注意】这里已经修改为对接 Uart1TXDBuf 和 Uart1RXDBuf
#define VoiceSendBuf  Uart1TXDBuf
#define VoiceRecBuf   Uart1RXDBuf

bit g_bVoiceEn = 1;
bit g_bUartErr = 0;

// 业务数据实体定义 (如果有初始值需求可以修改)
unsigned char g_u8TouchData = 0;       
unsigned char g_u8SensorData = 0;      
unsigned char g_u8VoiceCommTimeOut = 30; // 新增：通讯超时计数器
// 语音模块原始校验和
uchar VoiceCheckSum(uchar *ucPoint, uchar uiCount)
{
    uchar i;
    uchar j = 0;
    for(i=0; i<uiCount; i++)
    {
        j += ucPoint[i];
    }
    return j;
}

// 主控通信整帧累加校验和
uchar F_Calc_MCU_CheckSum(uchar *buf, uchar len)
{
    uchar i;
    uchar sum = 0;
    for(i = 0; i < len; i++)
    {
        sum += buf[i];
    }
    return sum;
}

// ==========================================
// 发送函数 1：打包含有【语音指令】的 23 字节帧
// ==========================================
unsigned char F_Send_MCU_Cmd(unsigned int CommandData)
{       
    unsigned char i;
    
    // 1. 填充语音指令 (前8字节)
    VoiceSendBuf[BYTE_HEAD_0] = HEARDER_0;
    VoiceSendBuf[BYTE_HEAD_1] = HEARDER_1;
    VoiceSendBuf[BYTE_ID] = ID;
    VoiceSendBuf[BYTE_CMD] = CMD_SEND;
    VoiceSendBuf[BYTE_DATA0] = (unsigned char)((CommandData >> 8) & 0xFF);
    VoiceSendBuf[BYTE_DATA1] = (unsigned char)( CommandData       & 0xFF);
    VoiceSendBuf[BYTE_CHECKSUM] = VoiceCheckSum(VoiceSendBuf, BYTE_CHECKSUM);
    VoiceSendBuf[BYTE_TAIL] = MSG_TAIL;
    
    // 2. 填充显示数据 (中间12字节)
    for(i = 0; i < 12; i++)
    {
        VoiceSendBuf[BYTE_TX_DISP_START + i] = DispBuf[i];
    }
    
    // 3. 填充亮度数据 (第21字节)
    VoiceSendBuf[BYTE_TX_BRIGHTNESS] = g_u8DisBright;
	
	VoiceSendBuf[BYTE_TX_RGB_INDEX] = g_u8RgbColorIndex;
    
    // 4. 填充整帧校验和 (最后1字节，校验前22字节)
    VoiceSendBuf[BYTE_TX_MCU_CHECKSUM] = F_Calc_MCU_CheckSum(VoiceSendBuf, 22);
    
    return TXD1_BUF_LENGTH; // 返回 23
}

// ==========================================
// 发送函数 2：打包【没有语音指令】的 23 字节空闲帧
// ==========================================
unsigned char F_Send_MCU_Idle_Cmd(void)
{
    unsigned char i;
    
    // 1. 前 8 字节全部填充 0 (表示无语音指令)
    for(i = 0; i < 8; i++)
    {
        VoiceSendBuf[i] = 0x00;
    }
    
    // 2. 填充显示数据 (中间12字节)
    for(i = 0; i < 12; i++)
    {
        VoiceSendBuf[BYTE_TX_DISP_START + i] = DispBuf[i];
    }
    
    // 3. 填充亮度数据 (第21字节)
    VoiceSendBuf[BYTE_TX_BRIGHTNESS] = g_u8DisBright;
	
	VoiceSendBuf[BYTE_TX_RGB_INDEX] = g_u8RgbColorIndex;
    
    // 4. 填充整帧校验和 (最后1字节，校验前22字节)
    VoiceSendBuf[BYTE_TX_MCU_CHECKSUM] = F_Calc_MCU_CheckSum(VoiceSendBuf, 22);
    
    return TXD1_BUF_LENGTH; // 返回 23
}

void F_VoiceFliter(void)
{
    if(g_bVoiceEn)
    {
        AddFuncB(&F_Send_MCU_Cmd, 0x0202, 1);
    }
}

void F_VoiceChange(unsigned int Command)
{
    if(g_bVoiceEn)
    {
        AddFuncB(&F_Send_MCU_Cmd, Command, 1);
    }
    else
    {
        F_BuzzerSet(BUZ_SHORT);
    }
}

// 语音业务处理逻辑 (保持原有逻辑)
void F_VOICE2MCU(unsigned char Command, unsigned char Data)
{
    U8 temp = 0;
    U16 l_u16CommandBack = ((Command << 8) | Data);
    
    if(g_bQDErr || g_bDoorOpenLeft || g_bDoorOpenRight)
    {   
        return;
    }
    
    if(VOICE_CMD_WAKE == Command)
    {   
        if(Data == c_ON) { F_VoiceChange(l_u16CommandBack); return; }
        else return;
    }
    else if(VOICE_CMD_POWER == Command)
    {
        if(Data == c_ON) { g_bTurnOn = c_ON; }
        else if(Data == c_OFF) { g_bTurnOn = c_OFF; }
        F_VoiceChange(l_u16CommandBack);
        F_DispTskChange(DIPS_NORMAL);                   
    }
    else if((VOICE_CMD_FAN_WORD == Command) || (VOICE_CMD_FAN_NUM == Command))
    {
        if(g_bTurnOn)
        {
            switch(Data)
            {
                case 0: case 1: case 2: g_u8WorkMode = Data + 1; break;
                default: g_u8WorkMode = MODE_LOW; break;
            }
            F_VoiceChange(l_u16CommandBack);                
        }
    }
    else if(VOICE_CMD_ION == Command)
    {
        if(g_bTurnOn)
        {
            if(Data && (MODE_SLEEP != g_u8WorkMode)) { g_bDLZEn = 1; }
            else { g_bDLZEn = 0; }
            F_VoiceChange(l_u16CommandBack);
        }
    }
    else if(VOICE_CMD_MODE == Command)
    {
        if(g_bTurnOn)
        {
            if(Data) { g_u8WorkMode = MODE_SLEEP; }
            else { g_u8WorkMode = MODE_AI; }
            F_VoiceChange(l_u16CommandBack);                
        }
    }
    else if(VOICE_CMD_TIME_ON == Command)
    {
        if(!g_bTurnOn)
        {
            g_u8TimeHBlinkTime = C_TIMEH_BLINK_TIME;
            F_TimeType(Data);
            F_VoiceChange(l_u16CommandBack);                
        }
    }
    else if(VOICE_CMD_TIME_OFF == Command)
    {
        if(g_bTurnOn)
        {
            g_u8TimeHBlinkTime = C_TIMEH_BLINK_TIME;
            F_TimeType(Data);
            F_VoiceChange(l_u16CommandBack);                
        }
    }
    else if(VOICE_CMD_TIME_CANCEL == Command)
    {
        g_u8TimeHBlinkTime = C_TIMEH_BLINK_TIME;
        F_TimeType(0);
        F_VoiceChange(l_u16CommandBack);                
    }
    else if(VOICE_CMD_ONOFF == Command)
    {
        g_bVoiceEn = 1;
        F_VoiceChange(l_u16CommandBack);            
        if(Data) { g_bVoiceEn = 1; }
        else { g_bVoiceEn = 0; }                
    }
}

void F_VoiceCommMonitor(void)
{
    if(g_u8VoiceCommTimeOut > 0)
    {
        g_u8VoiceCommTimeOut--;
        if(g_u8VoiceCommTimeOut == 0)
        {
            // 发生超时，清空数据
            g_u8TouchData = 0;
            g_u8SensorData = 0;
			g_bUartErr = 1;
			F_BuzzerSet(BUZ_SHORT_3);
        }
		else
		{
			g_bUartErr = 0;
		}
    }
}


// 接收解析逻辑：解析 11 字节
void F_VoiceRecProc()
{       
    // 1. 首先校验整帧数据的校验和 (前10个字节相加，对比第11个字节)
    if( F_Calc_MCU_CheckSum(VoiceRecBuf, 10) == VoiceRecBuf[BYTE_RX_MCU_CHECKSUM] )
    {
		g_u8VoiceCommTimeOut = 30;
		
        // 2. 提取触摸和传感器数据
        g_u8TouchData = VoiceRecBuf[BYTE_RX_TOUCH];
        g_u8SensorData = VoiceRecBuf[BYTE_RX_SENSOR];
        
        // 3. 校验里面是否包含有效的语音指令数据
        if( (HEARDER_0 == VoiceRecBuf[BYTE_HEAD_0]) 
         && (HEARDER_1 == VoiceRecBuf[BYTE_HEAD_1]) 
         && (ID == VoiceRecBuf[BYTE_ID]) 
         && (CMD_REC == VoiceRecBuf[BYTE_CMD])
         && (VoiceCheckSum(VoiceRecBuf, BYTE_CHECKSUM) == VoiceRecBuf[BYTE_CHECKSUM])
         && (MSG_TAIL == VoiceRecBuf[BYTE_TAIL]) )
        {
            // 如果校验通过，说明收到了语音指令，执行你的业务逻辑
            F_VOICE2MCU(VoiceRecBuf[BYTE_DATA0], VoiceRecBuf[BYTE_DATA1]);
        }
    }
	else
	{
		g_u8TouchData = 0;
		g_u8SensorData = 0;
	}
}