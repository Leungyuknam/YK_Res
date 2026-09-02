#ifndef _BLUE_H_
#define _BLUE_H_

#define c_STIVE_BUF_SIZE            8//对8s的数据做平均
#define c_STIVE_PREHEAT_TIME        90//粉尘预热时间 单位s
#define c_STIVE_LINK_ERR_TIME       10//通信错误时间 单位s

extern U16 xdata g_u16Pm25Data;     //PM25数值 单位ug/m3
extern U16 xdata g_u16Pm10Data;     //PM10数值 单位ug/m3
extern U16 xdata g_u16Pm1Data;      //PM1.0数值 单位ug/m3 
#define c_STIVE_DATA_MAX            999

extern  U8 xdata g_u8StiveState;
#define STIVE_STATE_ERR             0
#define STIVE_STATE_OFF             1
#define STIVE_STATE_ON              2

typedef struct
{
    U8      bGetSumFlg : 1; //已获取总和标记
    U8      u8Index;        //缓存索引
    U8      u8DelayTime;
	U16		u16DispData;
	U16		u16DispDataBak;
    U16     u16DataSum;
    U16     u16DataBuf[c_STIVE_BUF_SIZE];
}StiveData_t;

extern StiveData_t xdata g_sPm25;
extern StiveData_t xdata g_sPm10;
extern StiveData_t xdata g_sPm1;
//=============================================================================
//帧的字节顺序
//=============================================================================
#define HEAD_FIRST              0
#define FRAME_TYPE_H            1
#define FRAME_TYPE_L            2
#define FRAME_DATA              3
//=============================================================================
//数据帧类型
//=============================================================================
#define	STIVE_CMD_READ          1	//读取粉尘数据指令	
#define	STIVE_CMD_ON            2	//开启粉尘测量指令
#define	STIVE_CMD_OFF           3	//关闭粉尘测量指令
#define	STIVE_CMD_QUERY         4	//查询粉尘状态指令	回复是否开启粉尘测量

extern U8 F_StiveSendCode();
extern void F_StiveRecProc(U8 length);
extern void F_StiveCommucate(void);

#endif

