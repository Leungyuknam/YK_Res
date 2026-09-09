#ifndef OTA_h	 
#define OTA_h		
                                        			
#ifdef	EXTR                
#undef	EXTR  
#endif

#ifdef	OTA	
#define EXTR                            			                                                                             
#else						            			                                                                             
#define EXTR	extern                  			                                                                             
#endif 

//#define OTA_WEIGAO    //制作为高版本
#define OTA_FUNCTION		1
#define U8_OTA_ORDER	0x90   //OTA相关命令

enum 
{
	U8_OTA_9000_VERSIONNUM = 0, //9000 查询版本号数量
	U8_OTA_9001_VERSION = 1, //9001 查询版本号
	U8_OTA_9002_DOWNLOAD = 2, //9002 模块通知设备下载
	U8_OTA_9003_DOWNLOADING = 3, //9003 下载每一帧数据
	U8_OTA_9004_DOWNLOAD = 4, //9004 校验正确告知模块下载完成
	U8_OTA_9005_CHANGEHARDWARE = 5, //9005 通知电控切换分区
	U8_OTA_9006_UPLOAD = 6, //9006 电控上报结果给模块
	U8_OTA_9007_DOWNLOAD = 7, //9007 电控触发升级
};

#define CBYTE ((unsigned char volatile code  *) 0)
#define MakeWord(high,low)  (((uint)(high) << 8) | (uint)(low))
#define HiByte(u16Data)     	  ((uchar)((u16Data) >> 8))
#define LoByte(u16Data)     	  ((uchar)((u16Data) & 0xff))
#define HiNibble(u8Data)    	  ((uchar)(((u8Data) >> 4) & 0xf))
#define LoNibble(u8Data)    	  ((uchar)(( u8Data) & 0xf))

#define MAKEWORD	MakeWord
#define CheckAddr      	(512*7)   //E方扇区7
#define Cheak_Block      (CheckAddr/512)   //E方扇区7

#define U16_OTAUpgFlg_addr1   (CheckAddr+0)  //升级标志位AA
#define U16_OTAUpgFlg_addr2   (CheckAddr+1)  //升级标志位55
#define U16_OTAVer_addr           (CheckAddr+2)  //升级地址
#define U16_OTASize_addr1        (CheckAddr+30) //长度地址
#define U16_OTASize_addr2        (CheckAddr+31)  //长度地址
#define U16_OTASize_addr3        (CheckAddr+32) //长度地址
#define U16_OTASize_addr4        (CheckAddr+33)  //长度地址
#define U16_OTACrc_addr            (CheckAddr+34)  //CRC地址
#define U16_OTA9006Flg_addr    (CheckAddr+35)  //9006发送标志位

#define U16_OTAUpgFlg1      	0xAA  //升级标志位AA
#define U16_OTAUpgFlg2      	0x55  //升级标志位55
#define U16_OTA9006Flg      	0x96  //升级标志位96

EXTR	ram_field  unOTAorder ;  
#define fgSend9007En		unOTAorder.ram_bit.bit_1 
#define fgEnableDownload  	unOTAorder.ram_bit.bit_2
#define fgSend9006En		unOTAorder.ram_bit.bit_3
#define fgOtaSuccess   		unOTAorder.ram_bit.bit_4

EXTR void OTAMessageRecPro(uchar ucL_pt[]);
EXTR void OTAMessageSendPro(uchar ucL_pt[]);
EXTR void OTASwitchBootloader(void);
EXTR void OTAProcesingTimeDeal(void);
EXTR void OTAOkCheck(void);
#endif
/****************************************************************
					END	
****************************************************************/
