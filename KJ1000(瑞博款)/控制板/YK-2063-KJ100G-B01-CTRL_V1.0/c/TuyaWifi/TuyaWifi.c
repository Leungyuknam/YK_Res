#define TUYA_WIFI_C

#include "h\Allinclude.h"

#define TuYaSendBuf 	g_u8WifiUartTXDBuf
#define TuYaRecBuf 	    g_u8WifiUartRXDBuf

unsigned char LONG_REG fgHuarBeat = 0;
unsigned char LONG_REG WifiLinkTime = 10;
unsigned char LONG_REG WifiWorkState = WIFI_WORK_ERR;

unsigned char LONG_REG g_u8WifiTestState = 0;

unsigned int LONG_REG g_u16WifiDispTime = 0;
unsigned int LONG_REG g_u16WifiRunTime = 0;
//unsigned char code strProduct_ID[42] = 
//"{\"p\":\"rpkpwv9rert2onwp\",\"v\":\"1.0.0\",\"m\":1}";

unsigned char code strProduct_ID[42] = 
"{\"p\":\"3OtYSSmEFjT69fFz\",\"v\":\"1.0.0\",\"m\":1}";

unsigned int LONG_REG g_u16SysStateBak = 0;
//unsigned char code strProduct_ID[42] = 
//"{\"p\":\"hzmcuxba1lwoxvit\",\"v\":\"1.0.0\",\"m\":1}";
unsigned char LONG_REG WorkStateBuf[13] = {0};

bit g_bRecTypef = 0;
bit g_bWifiWorkStart = 0;
bit g_bWifiFirstLink = 0;

//TYPE_WIFICMD code WifiCmdTbl[WIFI_CMD_ALL] = 
//{
//	{0x05,0x01,0x01,0x01},
//	{0x08,0x02,0x02,0x04},
//	{0x05,0x03,0x04,0x01},
//	{0x05,0x04,0x04,0x01},
//	{0x08,0x05,0x02,0x04},
//	{0x05,0x07,0x01,0x01},
//	{0x05,0x08,0x01,0x01},
//	{0x05,0x0b,0x01,0x01},
//	{0x05,0x15,0x04,0x01},
//	{0x05,0x16,0x05,0x01},
//	{0x05,0x17,0x04,0x01},
//	{0x05,0x65,0x04,0x01},
//};

TYPE_WIFICMD code WifiCmdTbl[WIFI_CMD_ALL] = 
{
	{0x05,0x01,0x01,0x01},
	{0x08,0x02,0x02,0x04},
	{0x05,0x03,0x04,0x01},
	{0x05,0x04,0x04,0x01},
	{0x08,0x05,0x02,0x04},
	{0x05,0x07,0x01,0x01},
	{0x05,0x0b,0x01,0x01},
	{0x08,0x0c,0x02,0x04},	
	{0x08,0x0d,0x02,0x04},
	{0x08,0x0e,0x02,0x04},
	{0x05,0x15,0x04,0x01},
	{0x05,0x16,0x05,0x01},
	{0x05,0x17,0x04,0x01},
	{0x08,0x1B,0x02,0x04},	
	{0x05,0x1C,0x01,0x01},
};

uchar TuyaCheckSum(uchar *ucPoint,uint uiCount)
{
	uint i;
    uchar j = 0;
	for(i=0; i<uiCount; i++)
	{
		j += ucPoint[i];
	}
	return	j;
}

void CopyStr(unsigned char *SourceStr,unsigned char *AimStr,unsigned char Count)
{
	while(Count)
	{
		Count --;
		*AimStr = *SourceStr;
		AimStr ++;
		SourceStr++;
	}
}

unsigned char Send_HEAT_BEAT(void)
{
	TuYaSendBuf[HEAD_FIRST] = FRAME_FIRST;
 	TuYaSendBuf[HEAD_SECOND] = FRAME_SECOND;
  	TuYaSendBuf[PROTOCOL_VERSION] = VERSION;
  	TuYaSendBuf[FRAME_TYPE] = HEAT_BEAT_CMD;
  	TuYaSendBuf[LENGTH_HIGH] = 0;
  	TuYaSendBuf[LENGTH_LOW] = 1;
	TuYaSendBuf[6] = fgHuarBeat;
	TuYaSendBuf[7] = TuyaCheckSum(TuYaSendBuf,7);
	fgHuarBeat = 1;
	return 8;
}

void Rec_HEAT_BEAT(void)
{
	AddFunc(&Send_HEAT_BEAT,0,1);	
}

unsigned char Send_PRODUCT_INFO(void)
{
	TuYaSendBuf[HEAD_FIRST] = FRAME_FIRST;
	TuYaSendBuf[HEAD_SECOND] = FRAME_SECOND;
	TuYaSendBuf[PROTOCOL_VERSION] = VERSION;
	TuYaSendBuf[FRAME_TYPE] = PRODUCT_INFO_CMD;
	TuYaSendBuf[LENGTH_HIGH] = 0;
	TuYaSendBuf[LENGTH_LOW] = 0x2a;
    
	CopyStr(&strProduct_ID,&TuYaSendBuf[6],42);
    	
	TuYaSendBuf[48] = TuyaCheckSum(TuYaSendBuf,48);
	return 49;
}

void Rec_PRODUCT_INFO(void)
{
	InsertFunc(&Send_PRODUCT_INFO,0,1);
}

unsigned char Send_WORK_MODE(void)
{
	TuYaSendBuf[HEAD_FIRST] = FRAME_FIRST;
 	TuYaSendBuf[HEAD_SECOND] = FRAME_SECOND;
  	TuYaSendBuf[PROTOCOL_VERSION] = VERSION;
  	TuYaSendBuf[FRAME_TYPE] = WORK_MODE_CMD;
	TuYaSendBuf[LENGTH_HIGH] = 0;
  	TuYaSendBuf[LENGTH_LOW] = 0;
	TuYaSendBuf[6] = TuyaCheckSum(TuYaSendBuf,6);
	return 7;
}

void Rec_WORK_MODE(void)
{
	InsertFunc(&Send_WORK_MODE,0,1);
}

unsigned char Send_WIFI_STATE(void)
{
	TuYaSendBuf[HEAD_FIRST] = FRAME_FIRST;
 	TuYaSendBuf[HEAD_SECOND] = FRAME_SECOND;
  	TuYaSendBuf[PROTOCOL_VERSION] = VERSION;
  	TuYaSendBuf[FRAME_TYPE] = WIFI_STATE_CMD;
	TuYaSendBuf[LENGTH_HIGH] = 0;
  	TuYaSendBuf[LENGTH_LOW] = 0;
	TuYaSendBuf[6] = TuyaCheckSum(TuYaSendBuf,6);
	return 7;
}

void Rec_WIFI_STATE(void)
{
	if(WifiWorkState != TuYaRecBuf[6])
	{
		WifiWorkState = TuYaRecBuf[6];
		if((WifiWorkState >= WIFI_WORK_LINKING) && (WifiWorkState != WIFI_WORK_LOW_POWER))
		{
			TuyaWifiTask(WIFI_CMD_ALL);
		}
	}
	InsertFunc(&Send_WIFI_STATE,0,1);
}

unsigned char Send_WIFI_RESET(void)
{
	TuYaSendBuf[HEAD_FIRST] = FRAME_FIRST;
	TuYaSendBuf[HEAD_SECOND] = FRAME_SECOND;
	TuYaSendBuf[PROTOCOL_VERSION] = VERSION;
	TuYaSendBuf[FRAME_TYPE] = WIFI_RESET_CMD;
	TuYaSendBuf[LENGTH_HIGH] = 0;
	TuYaSendBuf[LENGTH_LOW] = 0;
	TuYaSendBuf[6] = TuyaCheckSum(TuYaSendBuf,6);
	return 7;
}

void Rec_WIFI_RESET(void)
{
	ClearFunc(&Send_WIFI_RESET);	
}

void Rec_Factroy_Test(void)
{
	if(TuYaRecBuf[6])
	{   g_u8WifiTestState = WIFI_TEST_Ok;}
	else
	{
		if(TuYaRecBuf[7] == 0x00)
		{
            //未扫描到名称为tuya_mdev_test路由器,请检查
            g_u8WifiTestState = WIFI_TEST_NoRoute;
		}
		else if(TuYaRecBuf[7] == 0x01)
		{
            //模块未授权
            g_u8WifiTestState = WIFI_TEST_NoAuthor;
		}
	}
}

unsigned char Send_Factroy_Test(void)
{
	TuYaSendBuf[HEAD_FIRST] = FRAME_FIRST;
 	TuYaSendBuf[HEAD_SECOND] = FRAME_SECOND;
  	TuYaSendBuf[PROTOCOL_VERSION] = VERSION;
  	TuYaSendBuf[FRAME_TYPE] = WIFI_TEST_CMD;
  	TuYaSendBuf[LENGTH_HIGH] = 0;
  	TuYaSendBuf[LENGTH_LOW] = 0;
	TuYaSendBuf[6] = TuyaCheckSum(TuYaSendBuf,6);
	return 7;
}

unsigned char Send_WIFI_ResetMode(void)
{
	TuYaSendBuf[HEAD_FIRST] = FRAME_FIRST;
	TuYaSendBuf[HEAD_SECOND] = FRAME_SECOND;
	TuYaSendBuf[PROTOCOL_VERSION] = VERSION;
	TuYaSendBuf[FRAME_TYPE] = WIFI_MODE_CMD;
	TuYaSendBuf[LENGTH_HIGH] = 0;
	TuYaSendBuf[LENGTH_LOW] = 1;
	TuYaSendBuf[6] = 0x01;// 0x00：smartconifg  0x01:AP 配置模式
	TuYaSendBuf[7] = TuyaCheckSum(TuYaSendBuf,7);
	WifiWorkState = WIFI_WORK_AP;
	return 8;
}

void Rec_WIFI_ResetMode(void)
{
	ClearFunc(&Send_WIFI_ResetMode);	
}


unsigned char Send_WIFI_LocalTime(void)
{
	TuYaSendBuf[HEAD_FIRST] = FRAME_FIRST;
	TuYaSendBuf[HEAD_SECOND] = FRAME_SECOND;
	TuYaSendBuf[PROTOCOL_VERSION] = VERSION;
	TuYaSendBuf[FRAME_TYPE] = GET_LOCAL_TIME_CMD;
	TuYaSendBuf[LENGTH_HIGH] = 0;
	TuYaSendBuf[LENGTH_LOW] = 0;
	TuYaSendBuf[6] = TuyaCheckSum(TuYaSendBuf,6);
	return 7;
}

void Rec_WIFI_LocalTime(void)
{
	if(1 == TuYaRecBuf[6])	//成功获取时间
	{
		if((TuYaRecBuf[10] > 18) || (TuYaRecBuf[10] < 8))		//19~7
		{	g_u8LocalTimeIsNight = 1;}
		else
		{	g_u8LocalTimeIsNight = 0;}	
	}
	else
	{
		g_u8LocalTimeIsNight = 0;	//否则默认为白天
	}

	ClearFunc(&Send_WIFI_LocalTime);	
}


s32 MCU2WIFI(unsigned char Command)
{
	s32 temp = 0;
	
	if(Command == WIFI_CMD_TURN_ON)
	{
		temp = g_bTurnOn;
	}
	else if (Command == WIFI_CMD_CH2O_DATA)
	{
        temp = g_u16DispCH2OData;
	}
	else if (Command == WIFI_CMD_PM25_DATA)
	{
        temp = g_sPm25.u16DispData;
	}
	else if (Command == WIFI_CMD_TVOC_DATA)
	{
        temp = g_u16DispVocData;
	}
	
	else if(Command == WIFI_CMD_WORK_MODE)
  {
			if(MODE_SLEEP == g_u8WorkMode)
			{   temp = 2;}
			else if(g_bDLZEn)
			{   temp = 1;}
			else
			{		temp = 0;}
	}
	else if(Command == WIFI_CMD_FAN)
	{
        if(MODE_SLEEP == g_u8WorkMode)
        {   temp = 0;}
        else if(MODE_AI == g_u8WorkMode)
        {   
						temp = 3;
        }
        else if(MODE_LOW == g_u8WorkMode)
        {   
						temp = 0;
        }
				else if(MODE_MID == g_u8WorkMode)
        {   
						temp = 1;
        }
				else if(MODE_HIGH == g_u8WorkMode)
        {   
						temp = 2;
        }
	}
	else if (Command == WIFI_CMD_FLITER_TIME)
	{
		temp = g_u8RunPercent;
	}
	else if (Command == WIFI_CMD_LOCK)
	{
		temp = g_bLockEn;
	}
	else if (Command == WIFI_CMD_FLITER_INIT)
	{
		temp = 0;
	}
	else if (Command == WIFI_CMD_TEMP_DATA)
	{
		temp = g_s8NowTemp;
	}
	else if (Command == WIFI_CMD_HUMI_DATA)
	{
		temp = g_u8NowHumi;
	}
	else if (Command == WIFI_CMD_ERR)
	{
        if(g_bQDErr)
		{   temp |= BIT1;}
		if(g_bDoorOpenLeft || g_bDoorOpenRight)
		{   temp |= BIT0;}
        if(g_bScreenInitFlg)
		{   temp |= BIT2;}
		if(g_bWaterBox && g_bWaterShortage)
		{	temp |= BIT3;}
	}
	else if (Command == WIFI_CMD_TYPEF)
	{
		temp = g_bRecTypef;
	}
	else if (Command == WIFI_CMD_AIRQUALITY)
	{
		temp = g_u8AirQuality;
	}
	else if (Command == WIFI_CMD_YUYIN)
	{
		temp = g_bVoiceEn;
	}
        
	return temp;
}

void WIFI2MCU(unsigned char Command,unsigned int Par)
{	
	if(Command == WIFI_CMD_TURN_ON)
	{
		if(g_bTurnOn != Par)
		{
			g_bTurnOn = Par;
            if(g_bTurnOn)
            {   F_BuzzerSet(BUZ_UP);}
            else
            {   F_BuzzerSet(BUZ_DOWN);}
            g_bLockEn = 0;
            F_DispTskChange(DIPS_NORMAL);
		}
	}
	else if(Command == WIFI_CMD_WORK_MODE)
	{
        if(g_bQDErr || g_bLockEn || g_bDoorOpenLeft || g_bDoorOpenRight)
        {   
            if(g_bLockEn)
            {
                g_u8LockBlinkTime = C_LOCK_BLINK_TIME;
                F_BuzzerSet(BUZ_LOCK);
            }  
            return;
        }
        if(2 == Par)
        {
			g_u8WorkMode = MODE_SLEEP;
			g_bDLZEn = 0;
        }
        else if(1 == Par)
        {
			if(MODE_SLEEP == g_u8WorkMode)
			{
				g_u8WorkMode = MODE_LOW;
			}	
			g_bDLZEn = 1;
		}
		else 
		{
			if(MODE_SLEEP == g_u8WorkMode)
			{
				g_u8WorkMode = MODE_LOW;
			}
			g_bDLZEn = 0;
		}
        F_BuzzerSet(BUZ_SHORT);
	}
	else if(Command == WIFI_CMD_FAN)
	{
        if(g_bQDErr || g_bLockEn || g_bDoorOpenLeft || g_bDoorOpenRight)
        {   
            if(g_bLockEn)
            {
                g_u8LockBlinkTime = C_LOCK_BLINK_TIME;
                F_BuzzerSet(BUZ_LOCK);
            }  
            return;
        }
		if(0 == Par)
		{
			g_u8WorkMode = MODE_LOW;
		}
		else if(1 == Par)
		{
			g_u8WorkMode = MODE_MID;
		}
		else if(2 == Par)
		{
			g_u8WorkMode = MODE_HIGH;
		}
		else if(3 == Par)
		{
			g_u8WorkMode = MODE_AI;
		}
        F_BuzzerSet(BUZ_SHORT);
	}
	else if(Command == WIFI_CMD_LOCK)
	{
        if(g_bQDErr|| g_bDoorOpenLeft || g_bDoorOpenRight)
        {   return;}
        g_bLockEn = Par;
        F_BuzzerSet(BUZ_SHORT);
	}
	else if(Command == WIFI_CMD_FLITER_INIT)
	{
        if(g_bQDErr || g_bLockEn || g_bDoorOpenLeft || g_bDoorOpenRight)
        {   
            if(g_bLockEn)
            {
                g_u8LockBlinkTime = C_LOCK_BLINK_TIME;
                F_BuzzerSet(BUZ_LOCK);
            }  
            return;
        }
        if(Par)
        {
            g_bScreenInitFlg = 0;
            g_u8RunPercent = 0;
            g_u8FhFilterPer = 100;
            g_u16ScreenRunTime = 0;
            
            F_BuzzerSet(BUZ_SHORT_3);
        }
	}
	else if(Command == WIFI_CMD_TYPEF)
	{
        g_bRecTypef = Par;
	}
	else if(Command == WIFI_CMD_YUYIN)
	{
        if(g_bQDErr|| g_bDoorOpenLeft || g_bDoorOpenRight)
        {   return;}
        g_bVoiceEn = Par;
        F_BuzzerSet(BUZ_SHORT);
	}
	
}

unsigned char Send_MCU_Cmd(unsigned char Command)
{
	s32 Temp = 0;
	
	TuYaSendBuf[HEAD_FIRST] = FRAME_FIRST;
	TuYaSendBuf[HEAD_SECOND] = FRAME_SECOND;
	TuYaSendBuf[PROTOCOL_VERSION] = VERSION;
	TuYaSendBuf[FRAME_TYPE] = STATE_UPLOAD_CMD;
	TuYaSendBuf[LENGTH_HIGH] = 0;
	TuYaSendBuf[LENGTH_LOW] = WifiCmdTbl[Command].DataLength;
	TuYaSendBuf[6] = WifiCmdTbl[Command].dpID;
	TuYaSendBuf[7] = WifiCmdTbl[Command].DataType;;
	TuYaSendBuf[8] = 0;
	TuYaSendBuf[9] = WifiCmdTbl[Command].FuncLength;

	Temp = MCU2WIFI(Command);
	if(WifiCmdTbl[Command].FuncLength == 4)
	{
		TuYaSendBuf[10] = (Temp >> 24)& 0x00ff;
		TuYaSendBuf[11] = (Temp >> 16)& 0x00ff;
		TuYaSendBuf[12] = (Temp >> 8)& 0x00ff;
		TuYaSendBuf[13] = Temp & 0x00ff;
	}
	else if(WifiCmdTbl[Command].FuncLength == 2)
	{
		TuYaSendBuf[10] = (Temp >> 8)& 0x00ff;
		TuYaSendBuf[11] = Temp & 0x00ff;
	}
	else
	{
		TuYaSendBuf[10] = Temp & 0x00ff;
	}
	Temp = TuYaSendBuf[LENGTH_LOW] + 6;
	TuYaSendBuf[Temp] = TuyaCheckSum(TuYaSendBuf,Temp);
	return (TuYaSendBuf[LENGTH_LOW] + 7);
}

void Rec_Wifi_Cmd(void)
{
	unsigned char temp;

	for(temp = 0; temp < WIFI_CMD_ALL; temp ++)
	{
		if(TuYaRecBuf[6] == WifiCmdTbl[temp].dpID)
		{
			break;
		}
	}
	if(temp >= WIFI_CMD_ALL)return;

	if(WifiCmdTbl[temp].FuncLength == 4)
		WIFI2MCU(temp,TuYaRecBuf[13]);
	else
		WIFI2MCU(temp,TuYaRecBuf[10]);
}

void TuyaWifiRecProc(void)
{
	unsigned char temp;
	
	temp = TuYaRecBuf[LENGTH_LOW] + 6;
	
	if((TuYaRecBuf[0] == FRAME_FIRST)
		&&(TuYaRecBuf[1] == FRAME_SECOND)
		&&(TuYaRecBuf[temp] == TuyaCheckSum(TuYaRecBuf,temp)))
	{
		WifiLinkTime = 30;
		switch(TuYaRecBuf[FRAME_TYPE])
		{
			case HEAT_BEAT_CMD:
				Rec_HEAT_BEAT();
				break;

			case PRODUCT_INFO_CMD:
				Rec_PRODUCT_INFO();
				break;

			case WORK_MODE_CMD:
				Rec_WORK_MODE();
				break;

			case WIFI_STATE_CMD:
				Rec_WIFI_STATE();
				break;

			case WIFI_RESET_CMD:
				Rec_WIFI_RESET();
				break;

			case WIFI_MODE_CMD:
				Rec_WIFI_ResetMode();
				break;

			case DATA_QUERT_CMD:
				Rec_Wifi_Cmd();
				//TuyaWifiTask(WIFI_CMD_ALL);
				break;

			case STATE_QUERY_CMD:
				TuyaWifiTask(WIFI_CMD_ALL);
				break;

			case WIFI_TEST_CMD:
				Rec_Factroy_Test();
				break;

			case GET_LOCAL_TIME_CMD:
				Rec_WIFI_LocalTime();
				break;

			default:
				break;
		}
	}
	else
	{

	}
}

void TuyaWifiTask(unsigned char Command)
{
	if(WIFI_CMD_RESET == Command)
	{
		WifiLinkTime = 10;
		g_u16WifiDispTime = C_WIFI_DISP_TIME;
		WifiWorkState = WIFI_WORK_AP;
		g_bWifiFirstLink = 0;
		AddFunc(&Send_WIFI_ResetMode,0,20);
	}
	else if(WIFI_CMD_TEST == Command)
	{
        WifiLinkTime = 10;
        g_u16WifiDispTime = C_WIFI_DISP_TIME;
        g_u8WifiTestState = WIFI_TEST_On;
		WifiWorkState = WIFI_WORK_AP;
		g_bWifiFirstLink = 0;
		AddFunc(&Send_Factroy_Test,0,10);
	}
	else if(WIFI_CMD_LOCALTIME == Command)
	{
		AddFunc(&Send_WIFI_LocalTime,0,5);
	}
	else if((WifiWorkState < WIFI_WORK_LOW_POWER) 
    && (WifiWorkState >= WIFI_WORK_LINKING))
    {
        if(WIFI_CMD_ALL == Command)
        {
            while(Command)
            {
                Command --;
                AddFunc(&Send_MCU_Cmd,Command,1);
            }
        }
        else
        {   AddFunc(&Send_MCU_Cmd,Command,1);}
    }
}

void F_WorkStateChengeSend()
{
    U16 temp = 0;
    
    temp = 0;
    if(g_bTurnOn)
    {   temp |= BIT0;}
    if(g_bLockEn)
    {   temp |= BIT1;}
    if(g_bScreenInitFlg)
    {   temp |= BIT2;}
    if(g_bRecTypef)
    {   temp |= BIT3;}
    if(g_bQDErr)
    {   temp |= BIT4;}
    if(g_bDoorOpenLeft)
    {   temp |= BIT5;}
	if(g_bVoiceEn)
    {   temp |= BIT6;}
	if(g_bDoorOpenRight)
	{	temp |= BIT7;}
	if(g_bWaterShortage)
	{	temp |= BIT8;}
	if(g_bWaterBox)
	{	temp |= BIT9;}
	if(g_u16SysStateBak != temp)
	{
        g_u16SysStateBak = temp;
        TuyaWifiTask(WIFI_CMD_ALL); 
	}
//    if(WorkStateBuf[0] != temp)
//    {
//        WorkStateBuf[0] = temp;
//        TuyaWifiTask(WIFI_CMD_ALL);        
//    }

    temp = WorkStateBuf[1] << 8;
    temp |= WorkStateBuf[2];
    if(temp != g_sPm25.u16DispData)
    {
        WorkStateBuf[1] = g_sPm25.u16DispData >> 8;
        WorkStateBuf[2] = g_sPm25.u16DispData;
        TuyaWifiTask(WIFI_CMD_PM25_DATA);
    }
    
    if((WorkStateBuf[3] != g_u8WorkMode) || WorkStateBuf[12] != g_bDLZEn)
    {
        WorkStateBuf[3] = g_u8WorkMode;
		WorkStateBuf[12] = g_bDLZEn;
        TuyaWifiTask(WIFI_CMD_WORK_MODE);
        TuyaWifiTask(WIFI_CMD_FAN);
    }

    if(WorkStateBuf[4] != g_u8RunPercent)
    {
        WorkStateBuf[4] = g_u8RunPercent;
        TuyaWifiTask(WIFI_CMD_FLITER_TIME);
    }
    
    if(WorkStateBuf[5] != g_u8AirQuality)
    {
        WorkStateBuf[5] = g_u8AirQuality;
        TuyaWifiTask(WIFI_CMD_AIRQUALITY);
    }
    
    if(WorkStateBuf[6] != g_s8NowTemp)
    {
        WorkStateBuf[6] = g_s8NowTemp;
        TuyaWifiTask(WIFI_CMD_TEMP_DATA);
    }
    
    if(WorkStateBuf[7] != g_u8NowHumi)
    {
        WorkStateBuf[7] = g_u8NowHumi;
        TuyaWifiTask(WIFI_CMD_HUMI_DATA);
    }
		
	temp = WorkStateBuf[8] << 8;
    temp |= WorkStateBuf[9];
    if(temp != g_u16DispVocData)
    {
        WorkStateBuf[8] = g_u16DispVocData >> 8;
        WorkStateBuf[9] = g_u16DispVocData;
        TuyaWifiTask(WIFI_CMD_TVOC_DATA);
    }
		
	temp = WorkStateBuf[10] << 8;
    temp |= WorkStateBuf[11];
    if(temp != g_u16DispCH2OData)
    {
        WorkStateBuf[10] = g_u16DispCH2OData >> 8;
        WorkStateBuf[11] = g_u16DispCH2OData;
        TuyaWifiTask(WIFI_CMD_CH2O_DATA);
    }
		
}

void TuyaWifiProc(void)
{        
	if(F_GetTimeTick(C_TIME_100ms))
	{        
        F_WorkStateChengeSend();
    }
    
	if(F_GetTimeTick(C_TIME_1s))
	{
		if(WifiLinkTime)
		{   WifiLinkTime --;}
		else
		{   WifiWorkState = WIFI_WORK_ERR;}
		if(g_u8TestState)
		{
			if(WIFI_TEST_On == g_u8WifiTestState)
			{   TuyaWifiTask(WIFI_CMD_TEST);}
		}
		else if(++g_u16WifiRunTime > C_WIFI_GETLOCALTIME_CNT)
        {
			g_u16WifiRunTime = 0;
			TuyaWifiTask(WIFI_CMD_LOCALTIME);
		}
		else
		{
			if(WIFI_TEST_On == g_u8WifiTestState)
			{   g_u8WifiTestState = WIFI_TEST_None;}
			if(WIFI_WORK_LINK_CLOUD == WifiWorkState)
			{
				g_bWifiWorkStart = 1;
				if(!g_bWifiFirstLink)
				{
					g_bWifiFirstLink = 1;
					TuyaWifiTask(WIFI_CMD_LOCALTIME);		//连上云端，获取当地时间
				}
			}
			else if((!g_bWifiWorkStart) && (g_bTurnOn))	//上电30秒后，未启动配网、测试模式、联网，则自动配网
			{
				g_bWifiWorkStart = 1;
				TuyaWifiTask(WIFI_CMD_RESET);
			}
		}
		
		if(g_u16WifiDispTime)g_u16WifiDispTime--;
	}
}

void F_WifiLedProc()
{
	static uchar Blink = 0;

    Blink ++;
    if(WifiWorkState == WIFI_WORK_LINK_CLOUD)
    {
        DIS_WIFI();
    }
    else if(g_u16WifiDispTime)
    {
        if((WifiWorkState == WIFI_WORK_LINK_OK)
        ||(WifiWorkState == WIFI_WORK_LINKING))
        {
            if(Blink & BIT2)DIS_WIFI();
        }
        else if(WifiWorkState == WIFI_WORK_AP)
        {
            if(Blink & BIT2)DIS_WIFI();
        }
        else if(WifiWorkState == WIFI_WORK_SMART_CONFIG)
        {
            if(Blink & BIT2)DIS_WIFI();
        }
    }
}
