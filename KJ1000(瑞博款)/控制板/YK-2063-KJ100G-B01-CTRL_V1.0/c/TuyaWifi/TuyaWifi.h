#ifndef TUYA_WIFI_H
#define TUYA_WIFI_H

#ifdef	EXTR
#undef	EXTR                                                                        
#endif
						                                                                                         
#ifdef	TUYA_WIFI_C
#define EXTR                                                                                                         
#else						                                                                                         
#define EXTR	extern                                                                                               
#endif 

//=============================================================================
//帧的字节顺序
//=============================================================================
#define         HEAD_FIRST                      0
#define         HEAD_SECOND                     1        
#define         PROTOCOL_VERSION                2
#define         FRAME_TYPE                      3
#define         LENGTH_HIGH                     4
#define         LENGTH_LOW                      5
#define         DATA_START                      6
//=============================================================================
//数据帧类型
//=============================================================================
#define         HEAT_BEAT_CMD                   0                               //心跳包
#define         PRODUCT_INFO_CMD                1                               //产品信息
#define         WORK_MODE_CMD                   2                               //查询MCU 设定的模块工作模式	
#define         WIFI_STATE_CMD                  3                               //wifi工作状态	
#define         WIFI_RESET_CMD                  4                               //重置wifi
#define         WIFI_MODE_CMD                   5                               //选择smartconfig/AP模式	
#define         DATA_QUERT_CMD                  6                               //命令下发
#define         STATE_UPLOAD_CMD                7                               //状态上报	 
#define         STATE_QUERY_CMD                 8                               //状态查询   
#define         UPDATE_QUERY_CMD                9                               //升级查询
#define         UPDATE_START_CMD                0x0a                            //升级开始
#define         UPDATE_TRANS_CMD                0x0b                            //升级传输
#define         GET_ONLINE_TIME_CMD             0x0c                            //获取系统时间(格林威治时间)
#define         FACTORY_MODE_CMD                0x0d                            //进入产测模式    
#define         WIFI_TEST_CMD                   0x0e                            //wifi功能测试
#define         GET_LOCAL_TIME_CMD              0x1c                            //获取本地时间
#define         WEATHER_OPEN_CMD                0x20                            //打开天气          
#define         WEATHER_DATA_CMD                0x21                            //天气数据
#define         HEAT_BEAT_STOP                  0x25                            //关闭WIFI模组心跳
#define         STREAM_OPEN_CMD                 0x26                            //开启流服务功能
#define         STREAM_START_CMD                0x27                            //开启流数据传输
#define         STREAM_TRANS_CMD                0x28                            //流数据传输
#define         STREAM_STOP_CMD                 0x29                            //结束流数据传输
//=============================================================================
#define VERSION                 0x03                                            //协议版本号
#define PROTOCOL_HEAD           0x07                                            //固定协议头长度
#define FIRM_UPDATA_SIZE        256                                            //升级包大小
#define FRAME_FIRST             0x55
#define FRAME_SECOND            0xaa
//============================================================================= 
//enum WIFI_TASK_NUM
//{
//	WIFI_CMD_TURN_ON = 0,
//	WIFI_CMD_PM25_DATA,	    //范围1-999
//	WIFI_CMD_WORK_MODE,	    //0:Manual 1:Auot 2:Sleep 3:Pet
//	WIFI_CMD_FAN,           //0:1 1:2 2:3
//	WIFI_CMD_FLITER_TIME,   //滤芯寿命 范围0~100
// 	WIFI_CMD_LOCK,	
// 	WIFI_CMD_RGB_DISP_EN,	
// 	WIFI_CMD_FLITER_INIT,	
//	WIFI_CMD_PM25,	        //0:优 1:良 2:一般 3:差
//	WIFI_CMD_ERR,	        //BIT0:倾倒 BIT1:滤网
//	WIFI_CMD_TYPEF,	
//	WIFI_CMD_VOC,	        //0:优 1:良 2:一般 3:差
//    
//	WIFI_CMD_ALL,

//	WIFI_CMD_RESET,
// 	WIFI_CMD_TEST,
//   
//	WIFI_TOTAL_TASK,
//};

enum WIFI_TASK_NUM
{
	WIFI_CMD_TURN_ON = 0,
	WIFI_CMD_CH2O_DATA,			//范围0-200    
	WIFI_CMD_WORK_MODE,	    //0:Auto 1:Disinfect 2:Sleep
	WIFI_CMD_FAN,           //0:1 1:2 2:3 3:自动
	WIFI_CMD_FLITER_TIME,   //滤芯寿命 范围0~100
 	WIFI_CMD_LOCK,	
 	WIFI_CMD_FLITER_INIT,
	WIFI_CMD_TVOC_DATA,			//VOC 范围0~500
	WIFI_CMD_PM25_DATA,			
	WIFI_CMD_TEMP_DATA,			//室内温度 范围0~60
	WIFI_CMD_AIRQUALITY,
	WIFI_CMD_ERR,	          //BIT0:门盖 BIT1:倾倒  BIT2:滤芯
	WIFI_CMD_TYPEF,
	WIFI_CMD_HUMI_DATA,	//室内湿度 范围5~99
	WIFI_CMD_YUYIN,
    
	WIFI_CMD_ALL,

	WIFI_CMD_RESET,
 	WIFI_CMD_TEST,
 	WIFI_CMD_LOCALTIME,
   
	WIFI_TOTAL_TASK,
};


typedef struct{
	unsigned char DataLength;
	unsigned char dpID;
	unsigned char DataType;
	unsigned char FuncLength;
}TYPE_WIFICMD;

extern  unsigned char LONG_REG WifiWorkState;
#define	WIFI_WORK_SMART_CONFIG			0
#define	WIFI_WORK_AP					1
#define	WIFI_WORK_LINKING				2
#define	WIFI_WORK_LINK_OK				3
#define	WIFI_WORK_LINK_CLOUD			4
#define	WIFI_WORK_LOW_POWER				5
#define	WIFI_WORK_ERR					0xFF

extern  unsigned char LONG_REG g_u8WifiTestState;
#define WIFI_TEST_None	        0   //等待
#define WIFI_TEST_On	        1   //启动产测
#define WIFI_TEST_NoRoute	    2   //没有路由
#define WIFI_TEST_NoAuthor	    3   //没有授权
#define WIFI_TEST_Ok	        4   //ok

#define C_WIFI_DISP_TIME        60 //WIFI按键配网最长显示时间 单位s
#define C_WIFI_GETLOCALTIME_CNT		1800		//获取当地时间的间隔 单位s

EXTR bit g_bWifiWorkStart;

EXTR void TuyaWifiRecProc(void);
EXTR void TuyaWifiTask(unsigned char Command);
EXTR void TuyaWifiProc(void);
EXTR void F_WifiLedProc();
EXTR void F_WifiLedOn();

#endif

