#ifndef _SENSE_DEAL_H_
#define _SENSE_DEAL_H_

extern unsigned char g_u8SensorData;

extern StrByte SENSE;
#define g_u8SenseState      SENSE.dchar
#define g_bQDErr            SENSE.BBit.b0//倾倒故障               P4
#define g_bQDErrBak         SENSE.BBit.b1//倾倒故障
#define g_bWaterShortage    SENSE.BBit.b2//缺水                   P4
#define g_bWaterShortageBak SENSE.BBit.b3//缺水
#define g_bDoorOpenLeft    				SENSE.BBit.b4//左门盖检测               P4
#define g_bDoorOpenLeftBak 				SENSE.BBit.b5//左门盖检测
#define g_bDoorOpenRight    			SENSE.BBit.b6//右门盖检测               P4
#define g_bDoorOpenRightBak 			SENSE.BBit.b7//右门盖检测

extern bit g_bWaterBox;		//水箱在位
extern bit g_bWaterBoxBak;	//水箱在位

extern void F_mAllSenseDeal(void);

#endif 