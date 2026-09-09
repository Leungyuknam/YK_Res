#ifndef UARTTRANS_H	 
#define UARTTRANS_H	
#ifdef	EXTR
#undef	EXTR
#endif

#ifdef	UARTTRANS 
#define EXTR 
#else
#define EXTR extern
#endif 

#define DEBUG  						0

#define U8_04COM_SINGLE				1 //单命令
#define U8_04COM_DOUBLE				2 //双命令

#define WIFILED_LINKFAIL				5 //wifi无法发现路由器下的闪烁状态
#define WIFILED_PASSWORDERROR		4 //wifi密码错误下的闪烁状态
#define WIFILED_FASTFLASH			3 //wifi灯快闪
#define WIFILED_SLOWFLASH			2 //wifi灯慢闪
#define WIFILED_ON					1 //wifi灯常亮
#define WIFILED_OFF					0 //wifi灯熄灭

#define U8_AUTHORITY_CONFIRMED		0 //确权成功
#define U8_AUTHORITY_CONFIRMING	1 //进入待确权
#define U8_AUTHORITY_CONFIRMEOUT	2 //待确权超时

EXTR	 ram_field  unFlagWifiUart;  
#define fgSendMarstOrder04		unFlagWifiUart.ram_bit.bit_0
#define fgSendMarstOrder		unFlagWifiUart.ram_bit.bit_1 //主动上传配置命令标识
#define fg_buz_enFirst			unFlagWifiUart.ram_bit.bit_2 //wifi蜂鸣器初次接收的标识
#define fgSendQuickLink		unFlagWifiUart.ram_bit.bit_3 //wifi蜂鸣器初次接收的标识
#define fgReceWifiOrder		unFlagWifiUart.ram_bit.bit_4 //接收到02命令，回复04命令
#define fgErrReport			unFlagWifiUart.ram_bit.bit_5

EXTR	 ram_field  unFlagWifiUart1;  
#define fgReceWifiData			unFlagWifiUart1.ram_bit.bit_0 //上电后能够接收到Wifi信号标识 
#define fgSendMarstCheckOrder	unFlagWifiUart1.ram_bit.bit_1
#define fgWifiLinkState			unFlagWifiUart1.ram_bit.bit_2 //触发配网
#define fgWifiReset				unFlagWifiUart1.ram_bit.bit_3 //模组重置命令
#define fgWifi64OrderTrigger	unFlagWifiUart1.ram_bit.bit_4 //64触发命令
#define fgEscApState			unFlagWifiUart1.ram_bit.bit_5 //ap 10分钟超时灭灯

EXTR	 ram_field  unFlagWifiUart2;  
#define fgSnCodeReady			unFlagWifiUart2.ram_bit.bit_0
#define fgSendBarCode14Order	unFlagWifiUart2.ram_bit.bit_1
#define fgSendWifi14Order		unFlagWifiUart2.ram_bit.bit_2
#define fgReceWifi14Order		unFlagWifiUart2.ram_bit.bit_3

EXTR RAM_REGION uchar ucSendBufWifi[U8_UART_LENGTH];//wifi发送数据缓冲区
EXTR RAM_REGION uchar ucRecBufWifi[U8_UART_LENGTH];//wifi接收数据缓冲区
EXTR RAM_REGION uchar ucSendCountWifi;//发送字节计数变量
EXTR RAM_REGION uchar ucRecCountWifi;//发送字节计数变量
EXTR RAM_REGION uchar ucSendTotalCnt;//发送总的字节长度
EXTR RAM_REGION uchar ucWifiFlashMarstCnt;//上电WIFI未启热点闪烁时间变量 

EXTR RAM_REGION uchar ucReceState;//接收的状态
EXTR RAM_REGION uchar ucSendState;//发送的状态
EXTR RAM_REGION uchar ucReceEndToSend;
EXTR RAM_REGION uchar ucWifiRecFinishTime;			
EXTR RAM_REGION uchar ucWifiRecDataTime;//接收到一个字节的时间计时变量
EXTR RAM_REGION uchar ucWifiLedSlowFlashtime;//wifi慢闪时间

EXTR RAM_REGION uchar ucWifiNoRecDataInt;//连续一段时间没有接收到串口数据就信息接收初始化处理 
EXTR RAM_REGION uchar ucT10msSendTimeOut;//增加一段时间没有发送完成的时候，转为发送空闲状态

EXTR RAM_REGION uchar ucWifiLinkState;
EXTR RAM_REGION uchar ucWifiLinkMode;
EXTR RAM_REGION uchar ucInDoorSerialNumBuf[32];//集团条码缓存

EXTR RAM_REGION uchar ucWifiLedDisState;//WiFi图标上电主动闪烁提示配网
EXTR RAM_REGION uchar ucWifiReceAuthorityState;//WiFi 确权状态量
EXTR RAM_REGION uchar uc04ComSendState;//有按键操作时，先上传状态 再上传按键埋点(上传命令类型为双状态)；
                                       //反之上传命令类型为单状态
typedef	struct WIFI_COMMAND
{
	uchar byte1;
	uchar byte8;
	uchar byte9;
	uchar byte10;
	uchar byte12;
	uchar byte13;
	uchar byte14;
	uchar byte15;
	uchar byte16;
	uchar byte17;
}WIFI_COMMAND;

EXTR WIFI_COMMAND WIFI_0D_COMMAND;     

EXTR void Wifi64OrderClean(void);
EXTR void WifiUartMarstOrder(void);
/*************************************************
	用户调用的函数接口
*************************************************/
EXTR void S_API_WifiCommInit(void);
EXTR void S_API_WifiUartDeal(void);
#endif
/*************************************************
               	 END
*************************************************/
