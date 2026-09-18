#ifndef __FUNCTION_H_
#define __FUNCTION_H_

#define     __DEBUG__           0



#ifdef _Native_FUN_H
   #define	EXT_FUN
#else
   #define	EXT_FUN extern	
#endif

#define	POW_ON_TIME				20//20 //*100MS 上电全亮时间





//过去10S 温度曲线变化形式
typedef enum
{
	TEMP_INC,			
	TEMP_ZERO_INC,
	TEMP_DEC,
	TEMP_ZERO_DEC
}TempChange_Enum;


//
typedef enum
{
	T_G_INC,			
	T_G_DEC,
}TempNext_Enum;

typedef enum
{
	STEP_INC,			
	STEP_DEC,
}Deal_Enum;


typedef enum
{
	HEAT_NORMAL,			//正常加热
//	HEAT_HOT_PROTECT,		//干烧保护
	HEAT_PRO_COOK,			//干烧保护进入煎烤间断模式
}HeatState_Enum;

typedef enum
{
	FORE_NON,			//不强制
//	FORE_HEAT,			//强制热
	FORE_STOP,			//强制停
}ForeHeat_Enum;

typedef enum
{
	GOAL_NORMAL_HEAT,	//不强制
	GOAL_STOP_HEAT,		//停加热
}HeatGoal_Enum;




typedef enum
{
	DISP_HEAT_NON,
	DISP_HEAT_AD,		//显示最高最低温度
	DISP_RECOVER		//恢复斜率显示
}Disp_Enum;
/*	
typedef enum
{
	TEMP_NON,				//起始未定义温度判定区间
	TEM_HOT,			
	TEM_COLD,
}FstState_Enum;
*/
#define	TEM_MID_VAL			100		//判定HOT与COLD
#define	CHK_DIFF_HOT		20		//判定多大差值才认为干烧
#define	C_SLOPE1S			2
#define	STORE_LENGTH 		20
#define	ADD_AD				2	//过高温判断阈值
#define	DEC_AD				5	//过低温判断阈值

//#define	DOWN_AD_LENGTH		20	//记录下降斜率
#define	HEAT_KEEP_TIME		10	//最小判断斜率时间，变更功率反应时间

#define CHK_POT_TIME		200//	*10MS 检测是否有锅的时间


typedef struct
{
	u8 HeatStep;
	u8 TempDat[STORE_LENGTH];
	u8 Inter10ms;
	u16 KeepPowSec;
	s8 s8TempSlope;				//温度时间斜率
	u16 ShowLevel;
	u8 IncSec;
	u8 DecSec;
	u8 RecordTempDat;
	u8 HeatStop_Sec;
	u8 BHeatFlag;
	HeatState_Enum State;

	u8 Heat500MS;
//	u8 HotHeatAD;
//	u8 ColdHeatAD;

	u8 HeatSec;
	u8 HeatCycSec;
//	u8 CycLoopCnt;
	HeatGoal_Enum mHeatGoal;
//	ForeHeat_Enum mForeState;
}OVEN_Str;

#define	TEMP_SLOPE_RD_SEC			25*2	//*500MS 时间后记录温度AD
#define	HEAT_RD_SEC			40*2	//*500MS 时间后记录温度AD
#define	COLD_RD_SEC			55*2	//*500MS 时间后冷却后温度AD


enum{
	H_NONE, 				//初始
	H_PRE_FST,				//1 // 默认预热 
	H_PRE_WAIT, 			//2 // 预热时间较长、小暂停一会
	H_PRE_SECOND_ONE,		//3 // 追了一段预热，小暂停下
	H_PRE_SECOND_TWO,			//4 // 预热刚结束
	
	
	H_HOT_WAIT, 			//5 //过温,停加热
	H_COLD_START,			//6 //温度过低,停加热时间已到，开始加热 	
	H_START_BEFORE, 		//7 //在范围内提前加热
	H_STOP_BEFORE,			//8 //在范围内提前停止加热,
	H_LONG_FORCE_STOP,		//9 //加热时间过长，强制先停

	H_PRE_SECOND,			//10	// 预热刚结束
};





//烹饪模式
enum{
	MENU_MENZHU_COOK_M,		//0  
	MENU_BAOTANG_COOK_M,	//1 
	MENU_MANDUN_COOK_M		//2 
};


#ifdef _Native_FUN_H
	EXT_FUN OVEN_Str xdata sHeat;

#else


	EXT_FUN OVEN_Str xdata sHeat;

#endif


#define	TE_01	8 
#define	TE_02	8 
#define	TE_03	9 
#define	TE_04	9 
#define	TE_05	10 
#define	TE_06	10 
#define	TE_07	11 
#define	TE_08	11 
#define	TE_09	12 
#define	TE_10	12 
#define	TE_11	13 
#define	TE_12	13 
#define	TE_13	14 
#define	TE_14	15 
#define	TE_15	15 
#define	TE_16	16 
#define	TE_17	17 
#define	TE_18	17 
#define	TE_19	18 
#define	TE_20	19 
#define	TE_21	20 
#define	TE_22	21 
#define	TE_23	21 
#define	TE_24	22 
#define	TE_25	23 
#define	TE_26	24 
#define	TE_27	25 
#define	TE_28	26 
#define	TE_29	27 
#define	TE_30	28 
#define	TE_31	29 
#define	TE_32	30 
#define	TE_33	32 
#define	TE_34	33 
#define	TE_35	34 
#define	TE_36	35 
#define	TE_37	36 
#define	TE_38	38 
#define	TE_39	39 
#define	TE_40	40 
#define	TE_41	42 
#define	TE_42	43 
#define	TE_43	45 
#define	TE_44	46 
#define	TE_45	48 
#define	TE_46	49 
#define	TE_47	51 
#define	TE_48	52 
#define	TE_49	54 
#define	TE_50	56 
#define	TE_51	57 
#define	TE_52	59 
#define	TE_53	61 
#define	TE_54	62 
#define	TE_55	64 
#define	TE_56	66 
#define	TE_57	68 
#define	TE_58	70 
#define	TE_59	72 
#define	TE_60	73 
#define	TE_61	75 
#define	TE_62	77 
#define	TE_63	79 
#define	TE_64	81 
#define	TE_65	83 
#define	TE_66	85 
#define	TE_67	87 
#define	TE_68	89 
#define	TE_69	91 
#define	TE_70	93 
#define	TE_71	95 
#define	TE_72	97 
#define	TE_73	99 
#define	TE_74	101 
#define	TE_75	104 
#define	TE_76	106 
#define	TE_77	108 
#define	TE_78	110 
#define	TE_79	112 
#define	TE_80	114 
#define	TE_81	116 
#define	TE_82	118 
#define	TE_83	120 
#define	TE_84	122 
#define	TE_85	124 
#define	TE_86	126 
#define	TE_87	128 
#define	TE_88	130 
#define	TE_89	132 
#define	TE_90	134 	//TE_88
#define	TE_91	136 
#define	TE_92	138 
#define	TE_93	140 
#define	TE_94	142 
#define	TE_95	144 
#define	TE_96	146 
#define	TE_97	148 
#define	TE_98	150 
#define	TE_99	152 
#define	TE_100	154 
#define	TE_101	155 
#define	TE_102	157 
#define	TE_103	159 
#define	TE_104	161 
#define	TE_105	162 
#define	TE_106	164 
#define	TE_107	166 
#define	TE_108	167 
#define	TE_109	169 
#define	TE_110	171 
#define	TE_111	172 
#define	TE_112	174 
#define	TE_113	176 
#define	TE_114	177 
#define	TE_115	179 
#define	TE_116	180 
#define	TE_117	182 
#define	TE_118	183 
#define	TE_119	184 
#define	TE_120	186 
#define	TE_121	187 
#define	TE_122	188 
#define	TE_123	190 
#define	TE_124	191 
#define	TE_125	192 
#define	TE_126	194 
#define	TE_127	195 
#define	TE_128	196 
#define	TE_129	197 
#define	TE_130	198 
#define	TE_131	200 
#define	TE_132	201 
#define	TE_133	202 
#define	TE_134	203 
#define	TE_135	204 
#define	TE_136	205 
#define	TE_137	206 
#define	TE_138	207 
#define	TE_139	208 
#define	TE_140	209 
#define	TE_141	210 
#define	TE_142	211 
#define	TE_143	212 
#define	TE_144	213 
#define	TE_145	213 
#define	TE_146	214 
#define	TE_147	215 
#define	TE_148	216 
#define	TE_149	217 
#define	TE_150	218 
#define	TE_151	218 
#define	TE_152	219 
#define	TE_153	220 
#define	TE_154	221 
#define	TE_155	221 
#define	TE_156	222 
#define	TE_157	223 
#define	TE_158	223 
#define	TE_159	224 
#define	TE_160	225 
#define	TE_161	225 
#define	TE_162	226 
#define	TE_163	226 
#define	TE_164	227 
#define	TE_165	227 
#define	TE_166	228 
#define	TE_167	229 
#define	TE_168	229 
#define	TE_169	230 
#define	TE_170	230 
#define	TE_171	231 
#define	TE_172	231 
#define	TE_173	232 
#define	TE_174	232 
#define	TE_175	232 
#define	TE_176	233 
#define	TE_177	233 
#define	TE_178	234 
#define	TE_179	234 
#define	TE_180	235 
#define	TE_181	235 
#define	TE_182	235 
#define	TE_183	236 
#define	TE_184	236 
#define	TE_185	237 
#define	TE_186	237 
#define	TE_187	237 
#define	TE_188	238 
#define	TE_189	238 
#define	TE_190	238 
#define	TE_191	239 
#define	TE_192	239 
#define	TE_193	239 
#define	TE_194	239 
#define	TE_195	240 
#define	TE_196	240 
#define	TE_197	240 
#define	TE_198	241 
#define	TE_199	241 
#define	TE_200	241 
#define	TE_201	241 
#define	TE_202	242 
#define	TE_203	242 
#define	TE_204	242 
#define	TE_205	242 
#define	TE_206	243 
#define	TE_207	243 
#define	TE_208	243 
#define	TE_209	243 
#define	TE_210	244 
#define	TE_211	244 
#define	TE_212	244 
#define	TE_213	244 
#define	TE_214	244 
#define	TE_215	245 
#define	TE_216	245 
#define	TE_217	245 
#define	TE_218	245 
#define	TE_219	245 
#define	TE_220	245 
#define	TE_221	246 
#define	TE_222	246 
#define	TE_223	246 
#define	TE_224	246 
#define	TE_225	246 
#define	TE_226	246 
#define	TE_227	247 
#define	TE_228	247 
#define	TE_229	247 
#define	TE_230	247 
#define	TE_231	247 
#define	TE_232	247 
#define	TE_233	247 
#define	TE_234	247 
#define	TE_235	248 
#define	TE_236	248 
#define	TE_237	248 
#define	TE_238	248 
#define	TE_239	248 
#define	TE_240	248 
#define	TE_241	248 
#define	TE_242	248 
#define	TE_243	249 
#define	TE_244	249 
#define	TE_245	249 
#define	TE_246	249 
#define	TE_247	249 
#define	TE_248	249 
#define	TE_249	249 
#define	TE_250	249 
#define	TE_251	249 
#define	TE_252	249 
#define	TE_253	250 
#define	TE_254	250 
#define	TE_255	250 
#define	TE_256	250 
#define	TE_257	250 
#define	TE_258	250 
#define	TE_259	250 
#define	TE_260	250 
#define	TE_261	250 
#define	TE_262	250 
#define	TE_263	250 
#define	TE_264	250 
#define	TE_265	250 
#define	TE_266	251 
#define	TE_267	251 
#define	TE_268	251 
#define	TE_269	251 
#define	TE_270	251 
#define	TE_271	251 
#define	TE_272	251 
#define	TE_273	251 
#define	TE_274	251 
#define	TE_275	251 
#define	TE_276	251 
#define	TE_277	251 
#define	TE_278	251 
#define	TE_279	251 
#define	TE_280	251 
#define	TE_281	251 
#define	TE_282	252 
#define	TE_283	252 
#define	TE_284	252 
#define	TE_285	252 
#define	TE_286	252 
#define	TE_287	252 
#define	TE_288	252 
#define	TE_289	252 
#define	TE_290	252 
#define	TE_291	252 
#define	TE_292	252 
#define	TE_293	252 
#define	TE_294	252 
#define	TE_295	252 
#define	TE_296	252 
#define	TE_297	252 
#define	TE_298	252 
#define	TE_299	252 
#define	TE_300	252 



extern void F_TimeEvent(void);
extern u8 DriveControDeal(u8 NowTempDat);
extern void TimeCountFrq();


#endif



