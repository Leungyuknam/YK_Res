#ifndef WifiInterFace_h
#define WifiInterFace_h

#ifdef	EXTR 
#undef	EXTR 
#endif 
 
#ifdef	WifiInterFace
#define EXTR
#else
#define EXTR extern
#endif     

extern bit g_bSnCodeError;             //SNπ ’œ

EXTR void UserInterFace(void);
EXTR void S_API_MachineStateChange(void);
EXTR void S_API_Buz(uchar cnt,uint on_time,uint off_time);
#endif
/*******************************************************************
						END	
*******************************************************************/
