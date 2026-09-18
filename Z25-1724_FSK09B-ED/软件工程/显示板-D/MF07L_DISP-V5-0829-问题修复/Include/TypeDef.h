						
#ifndef	_TypeDef_H
#define	_TypeDef_H

//------------------------------类型别名----------------------------------------------------------
typedef unsigned char   U8;
typedef unsigned int    U16;
typedef unsigned long   U32;

typedef unsigned char   u8;
typedef unsigned int    u16;
typedef unsigned long   u32;

typedef unsigned char 	uchar;
typedef unsigned int 	uint;  
typedef unsigned long   ulong;

typedef unsigned char 	uint8;
typedef unsigned int  	uint16;  
typedef unsigned long 	uint32;

typedef unsigned char 	uint8_t;
typedef unsigned int  	uint16_t;  
typedef unsigned long 	uint32_t;



typedef   signed            char int8_t;
typedef   signed short      int int16_t;
typedef   signed            long  int32_t;

typedef int32_t  s32;
typedef int16_t  s16;
typedef int8_t   s8;



typedef  unsigned char 	Byte;
typedef  unsigned int 	Word;
//typedef  unsigned long 	Dword;

typedef          bit    Bit;
typedef          bit    Bool;
typedef void (*voidFan)(void);
typedef void (*U8Fan)(U8 DA);
#define  S8     signed char              //有符号字符型变量  8位数据：   -128~127
#define  S16    signed int               //有符号整型变量    16位数据：  -32768~32767 
#define  S32    signed long              //有符号长整型变量  32位数据    -2147483648---+2147483647

#define FALSE               0
#define TRUE                1

#define	REG  			data
#define	B_REG  			bdata
#define	X_REG			xdata
#define	I_REG			idata

#define MAX_U8			255
//-------------------------------------------------------------------------------------------------
    //二进制宏定义
#define LongToBin(n) \
(                    \
((n >> 21) & 0x80) | \
((n >> 18) & 0x40) | \
((n >> 15) & 0x20) | \
((n >> 12) & 0x10) | \
((n >>  9) & 0x08) | \
((n >>  6) & 0x04) | \
((n >>  3) & 0x02) | \
((n      ) & 0x01)   \
)
#define _BIN(n) LongToBin(0x##n##l) //write binary charactor set,exsample : Bin(11111111) = 0xff

//------------------------------关于位操作---------------------------------------------------------
#define BitSet(_sfr,_bit) (_sfr)|=(_bit)
#define BitClr(_sfr,_bit) (_sfr)&=(~(_bit))
#define BitTst(_sfr,_bit) ((_sfr)&(_bit))
#define BitXor(_sfr,_bit) (_sfr)^=(_bit)

#define clrb(x,y) (x&=~(1<<y))    	//位清0
#define setb(x,y) (x|= (1<<y))     	//位置1
#define getb(x,y) (x&(1<<y))       	//位判断
#define xorb(x,y) (x^=(1<<y))    	//位取反


//BIT定义　　
#define  BIT0 0x01
#define  BIT1 0x02
#define  BIT2 0x04
#define  BIT3 0x08
#define  BIT4 0x10
#define  BIT5 0x20
#define  BIT6 0x40
#define  BIT7 0x80
#define  BIT8 0x100
#define  BIT9 0x200
#define  BIT10 0x400
#define  BIT11 0x800
#define  BIT12 0x1000
#define  BIT13 0x2000
#define  BIT14 0x4000
#define  BIT15 0x8000

#define  BIT16 (1L<<16)
#define  BIT17 (1L<<17)
#define  BIT18 (1L<<18)
#define  BIT19 (1L<<19)
#define  BIT20 (1L<<20)
#define  BIT21 (1L<<21)
#define  BIT22 (1L<<22)
#define  BIT23 (1L<<23)
#define  BIT24 (1L<<24)
#define  BIT25 (1L<<25)
#define  BIT26 (1L<<26)
#define  BIT27 (1L<<27)
#define  BIT28 (1L<<28)
#define  BIT29 (1L<<29)
#define  BIT30 (1L<<30)
#define  BIT31 (1L<<31)


#define	b0000_0000	0x00	//	0
#define	b0000_0001	0x01	//	1
#define	b0000_0010	0x02	//	2
#define	b0000_0011	0x03	//	3
#define	b0000_0100	0x04	//	4
#define	b0000_0101	0x05	//	5
#define	b0000_0110	0x06	//	6
#define	b0000_0111	0x07	//	7
#define	b0000_1000	0x08	//	8
#define	b0000_1001	0x09	//	9
#define	b0000_1010	0x0A	//	10
#define	b0000_1011	0x0B	//	11
#define	b0000_1100	0x0C	//	12
#define	b0000_1101	0x0D	//	13
#define	b0000_1110	0x0E	//	14
#define	b0000_1111	0x0F	//	15
#define	b0001_0000	0x10	//	16
#define	b0001_0001	0x11	//	17
#define	b0001_0010	0x12	//	18
#define	b0001_0011	0x13	//	19
#define	b0001_0100	0x14	//	20
#define	b0001_0101	0x15	//	21
#define	b0001_0110	0x16	//	22
#define	b0001_0111	0x17	//	23
#define	b0001_1000	0x18	//	24
#define	b0001_1001	0x19	//	25
#define	b0001_1010	0x1A	//	26
#define	b0001_1011	0x1B	//	27
#define	b0001_1100	0x1C	//	28
#define	b0001_1101	0x1D	//	29
#define	b0001_1110	0x1E	//	30
#define	b0001_1111	0x1F	//	31
#define	b0010_0000	0x20	//	32
#define	b0010_0001	0x21	//	33
#define	b0010_0010	0x22	//	34
#define	b0010_0011	0x23	//	35
#define	b0010_0100	0x24	//	36
#define	b0010_0101	0x25	//	37
#define	b0010_0110	0x26	//	38
#define	b0010_0111	0x27	//	39
#define	b0010_1000	0x28	//	40
#define	b0010_1001	0x29	//	41
#define	b0010_1010	0x2A	//	42
#define	b0010_1011	0x2B	//	43
#define	b0010_1100	0x2C	//	44
#define	b0010_1101	0x2D	//	45
#define	b0010_1110	0x2E	//	46
#define	b0010_1111	0x2F	//	47
#define	b0011_0000	0x30	//	48
#define	b0011_0001	0x31	//	49
#define	b0011_0010	0x32	//	50
#define	b0011_0011	0x33	//	51
#define	b0011_0100	0x34	//	52
#define	b0011_0101	0x35	//	53
#define	b0011_0110	0x36	//	54
#define	b0011_0111	0x37	//	55
#define	b0011_1000	0x38	//	56
#define	b0011_1001	0x39	//	57
#define	b0011_1010	0x3A	//	58
#define	b0011_1011	0x3B	//	59
#define	b0011_1100	0x3C	//	60
#define	b0011_1101	0x3D	//	61
#define	b0011_1110	0x3E	//	62
#define	b0011_1111	0x3F	//	63
#define	b0100_0000	0x40	//	64
#define	b0100_0001	0x41	//	65
#define	b0100_0010	0x42	//	66
#define	b0100_0011	0x43	//	67
#define	b0100_0100	0x44	//	68
#define	b0100_0101	0x45	//	69
#define	b0100_0110	0x46	//	70
#define	b0100_0111	0x47	//	71
#define	b0100_1000	0x48	//	72
#define	b0100_1001	0x49	//	73
#define	b0100_1010	0x4A	//	74
#define	b0100_1011	0x4B	//	75
#define	b0100_1100	0x4C	//	76
#define	b0100_1101	0x4D	//	77
#define	b0100_1110	0x4E	//	78
#define	b0100_1111	0x4F	//	79
#define	b0101_0000	0x50	//	80
#define	b0101_0001	0x51	//	81
#define	b0101_0010	0x52	//	82
#define	b0101_0011	0x53	//	83
#define	b0101_0100	0x54	//	84
#define	b0101_0101	0x55	//	85
#define	b0101_0110	0x56	//	86
#define	b0101_0111	0x57	//	87
#define	b0101_1000	0x58	//	88
#define	b0101_1001	0x59	//	89
#define	b0101_1010	0x5A	//	90
#define	b0101_1011	0x5B	//	91
#define	b0101_1100	0x5C	//	92
#define	b0101_1101	0x5D	//	93
#define	b0101_1110	0x5E	//	94
#define	b0101_1111	0x5F	//	95
#define	b0110_0000	0x60	//	96
#define	b0110_0001	0x61	//	97
#define	b0110_0010	0x62	//	98
#define	b0110_0011	0x63	//	99
#define	b0110_0100	0x64	//	100
#define	b0110_0101	0x65	//	101
#define	b0110_0110	0x66	//	102
#define	b0110_0111	0x67	//	103
#define	b0110_1000	0x68	//	104
#define	b0110_1001	0x69	//	105
#define	b0110_1010	0x6A	//	106
#define	b0110_1011	0x6B	//	107
#define	b0110_1100	0x6C	//	108
#define	b0110_1101	0x6D	//	109
#define	b0110_1110	0x6E	//	110
#define	b0110_1111	0x6F	//	111
#define	b0111_0000	0x70	//	112
#define	b0111_0001	0x71	//	113
#define	b0111_0010	0x72	//	114
#define	b0111_0011	0x73	//	115
#define	b0111_0100	0x74	//	116
#define	b0111_0101	0x75	//	117
#define	b0111_0110	0x76	//	118
#define	b0111_0111	0x77	//	119
#define	b0111_1000	0x78	//	120
#define	b0111_1001	0x79	//	121
#define	b0111_1010	0x7A	//	122
#define	b0111_1011	0x7B	//	123
#define	b0111_1100	0x7C	//	124
#define	b0111_1101	0x7D	//	125
#define	b0111_1110	0x7E	//	126
#define	b0111_1111	0x7F	//	127
#define	b1000_0000	0x80	//	128
#define	b1000_0001	0x81	//	129
#define	b1000_0010	0x82	//	130
#define	b1000_0011	0x83	//	131
#define	b1000_0100	0x84	//	132
#define	b1000_0101	0x85	//	133
#define	b1000_0110	0x86	//	134
#define	b1000_0111	0x87	//	135
#define	b1000_1000	0x88	//	136
#define	b1000_1001	0x89	//	137
#define	b1000_1010	0x8A	//	138
#define	b1000_1011	0x8B	//	139
#define	b1000_1100	0x8C	//	140
#define	b1000_1101	0x8D	//	141
#define	b1000_1110	0x8E	//	142
#define	b1000_1111	0x8F	//	143
#define	b1001_0000	0x90	//	144
#define	b1001_0001	0x91	//	145
#define	b1001_0010	0x92	//	146
#define	b1001_0011	0x93	//	147
#define	b1001_0100	0x94	//	148
#define	b1001_0101	0x95	//	149
#define	b1001_0110	0x96	//	150
#define	b1001_0111	0x97	//	151
#define	b1001_1000	0x98	//	152
#define	b1001_1001	0x99	//	153
#define	b1001_1010	0x9A	//	154
#define	b1001_1011	0x9B	//	155
#define	b1001_1100	0x9C	//	156
#define	b1001_1101	0x9D	//	157
#define	b1001_1110	0x9E	//	158
#define	b1001_1111	0x9F	//	159
#define	b1010_0000	0xA0	//	160
#define	b1010_0001	0xA1	//	161
#define	b1010_0010	0xA2	//	162
#define	b1010_0011	0xA3	//	163
#define	b1010_0100	0xA4	//	164
#define	b1010_0101	0xA5	//	165
#define	b1010_0110	0xA6	//	166
#define	b1010_0111	0xA7	//	167
#define	b1010_1000	0xA8	//	168
#define	b1010_1001	0xA9	//	169
#define	b1010_1010	0xAA	//	170
#define	b1010_1011	0xAB	//	171
#define	b1010_1100	0xAC	//	172
#define	b1010_1101	0xAD	//	173
#define	b1010_1110	0xAE	//	174
#define	b1010_1111	0xAF	//	175
#define	b1011_0000	0xB0	//	176
#define	b1011_0001	0xB1	//	177
#define	b1011_0010	0xB2	//	178
#define	b1011_0011	0xB3	//	179
#define	b1011_0100	0xB4	//	180
#define	b1011_0101	0xB5	//	181
#define	b1011_0110	0xB6	//	182
#define	b1011_0111	0xB7	//	183
#define	b1011_1000	0xB8	//	184
#define	b1011_1001	0xB9	//	185
#define	b1011_1010	0xBA	//	186
#define	b1011_1011	0xBB	//	187
#define	b1011_1100	0xBC	//	188
#define	b1011_1101	0xBD	//	189
#define	b1011_1110	0xBE	//	190
#define	b1011_1111	0xBF	//	191
#define	b1100_0000	0xC0	//	192
#define	b1100_0001	0xC1	//	193
#define	b1100_0010	0xC2	//	194
#define	b1100_0011	0xC3	//	195
#define	b1100_0100	0xC4	//	196
#define	b1100_0101	0xC5	//	197
#define	b1100_0110	0xC6	//	198
#define	b1100_0111	0xC7	//	199
#define	b1100_1000	0xC8	//	200
#define	b1100_1001	0xC9	//	201
#define	b1100_1010	0xCA	//	202
#define	b1100_1011	0xCB	//	203
#define	b1100_1100	0xCC	//	204
#define	b1100_1101	0xCD	//	205
#define	b1100_1110	0xCE	//	206
#define	b1100_1111	0xCF	//	207
#define	b1101_0000	0xD0	//	208
#define	b1101_0001	0xD1	//	209
#define	b1101_0010	0xD2	//	210
#define	b1101_0011	0xD3	//	211
#define	b1101_0100	0xD4	//	212
#define	b1101_0101	0xD5	//	213
#define	b1101_0110	0xD6	//	214
#define	b1101_0111	0xD7	//	215
#define	b1101_1000	0xD8	//	216
#define	b1101_1001	0xD9	//	217
#define	b1101_1010	0xDA	//	218
#define	b1101_1011	0xDB	//	219
#define	b1101_1100	0xDC	//	220
#define	b1101_1101	0xDD	//	221
#define	b1101_1110	0xDE	//	222
#define	b1101_1111	0xDF	//	223
#define	b1110_0000	0xE0	//	224
#define	b1110_0001	0xE1	//	225
#define	b1110_0010	0xE2	//	226
#define	b1110_0011	0xE3	//	227
#define	b1110_0100	0xE4	//	228
#define	b1110_0101	0xE5	//	229
#define	b1110_0110	0xE6	//	230
#define	b1110_0111	0xE7	//	231
#define	b1110_1000	0xE8	//	232
#define	b1110_1001	0xE9	//	233
#define	b1110_1010	0xEA	//	234
#define	b1110_1011	0xEB	//	235
#define	b1110_1100	0xEC	//	236
#define	b1110_1101	0xED	//	237
#define	b1110_1110	0xEE	//	238
#define	b1110_1111	0xEF	//	239
#define	b1111_0000	0xF0	//	240
#define	b1111_0001	0xF1	//	241
#define	b1111_0010	0xF2	//	242
#define	b1111_0011	0xF3	//	243
#define	b1111_0100	0xF4	//	244
#define	b1111_0101	0xF5	//	245
#define	b1111_0110	0xF6	//	246
#define	b1111_0111	0xF7	//	247
#define	b1111_1000	0xF8	//	248
#define	b1111_1001	0xF9	//	249
#define	b1111_1010	0xFA	//	250
#define	b1111_1011	0xFB	//	251
#define	b1111_1100	0xFC	//	252
#define	b1111_1101	0xFD	//	253
#define	b1111_1110	0xFE	//	254
#define	b1111_1111	0xFF	//	255
//--------------------------------------------------------------------------------------------------

//------------------------------新数据类型--------------------------------------------------------


typedef struct _TYPE_BIT
{
	unsigned char	Bt0:1;
	unsigned char	Bt1:1;
	unsigned char	Bt2:1;
	unsigned char	Bt3:1;
	unsigned char	Bt4:1;
	unsigned char	Bt5:1;
	unsigned char	Bt6:1;
	unsigned char	Bt7:1;
} TYPE_BIT;

typedef union {
    unsigned char byte;
    struct {        
        unsigned char high : 4;
        unsigned char low  : 4;
    }half;
}StrChar;


typedef union {
    unsigned int word;
    struct {
        unsigned char high;
        unsigned char low;
    }byte;
}StrInt;
//typedef union {
//    unsigned long dword;
//    struct {
//        unsigned int high;
//        unsigned int low; 
//    }word;
//    struct {
//        unsigned char m_high;
//        unsigned char m_low;
//        unsigned char l_high;
//        unsigned char l_low; 
//    }byte;
//}StrLong;


typedef union {
	struct {
		unsigned char Bit0: 1;
		unsigned char Bit1: 1;
		unsigned char Bit2: 1;
		unsigned char Bit3: 1;
		unsigned char Bit4: 1;
		unsigned char Bit5: 1;
		unsigned char Bit6: 1;
		unsigned char Bit7: 1;
	} Bits;
	unsigned char Byte;
} Flag8;
//-------------------------------------------------------------------------------------------

//----------------------------辅助宏定义-----------------------------------------------------
#define	MAKEWORD(v1,v2)	(((Word)(v1)<<8) + (Word)(v2))
#define	HIBYTE(v1)		((Byte)((v1)>>8))
#define	LOBYTE(v1)		((Byte)((v1)&0xff))
#define LongToBin(n) \
(                    \
((n >> 21) & 0x80) | \
((n >> 18) & 0x40) | \
((n >> 15) & 0x20) | \
((n >> 12) & 0x10) | \
((n >>  9) & 0x08) | \
((n >>  6) & 0x04) | \
((n >>  3) & 0x02) | \
((n      ) & 0x01)   \
)
#define Bin(n) LongToBin(0x##n##l)	//write binary charactor set,exsample : Bin(11111111) = 0xff
//--------------------------------------------------------------------------------------------


//----------------------------不推荐使用--------------------------------------------------
//----------------------------不推荐使用--------------------------------------------------




#define bin(n) LongToBin(0x##n##l)	//write binary charactor set,exsample : Bin(11111111) = 0xff
//#define   bit(n) (1<<n)
//#define   bit0    0x01
//#define   bit1    0x02
//#define   bit2    0x04
//#define   bit3    0x08
//#define   bit4    0x10
//#define   bit5    0x20
//#define   bit6    0x40
//#define   bit7    0x80  



//union byte_type_t{
// uint8 byte_8;
// struct p8_bit{
//    uint8 bit0:1 ; // 1
//    uint8 bit1:1 ; // 2
//    uint8 bit2:1 ; // 3
//    uint8 bit3:1 ; // 4
//    uint8 bit4:1 ; // 5
//    uint8 bit5:1 ; // 6
//    uint8 bit6:1 ; // 7
//    uint8 bit7:1 ; // 8
// }Bits;
//};

typedef struct bit_def
{
 u8 b7:1;
 u8 b6:1;
 u8 b5:1;
 u8 b4:1;
 u8 b3:1;
 u8 b2:1;
 u8 b1:1;
 u8 b0:1;
}BYTE_BIT_DEF;
////////////////////////////////////////////////////////////////////
//个人喜好定义部分 
#define	Nop()		{_nop_();}

#define enableInterrupts()    EA=1	 /** 开启总中断	 **/
#define disableInterrupts()   EA=0	 /** 关闭总中断	 **/
#define Di()        EA=0
#define Ei()        EA=1                  

//typedef enum {FALSE = 0, TRUE = !FALSE} bool;


//typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

//typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

//typedef enum {LOW = 0, HIGH = !LOW} PriorityStatus;

#define	S_REG  		idata
#define	LONG_REG	xdata
#define	SHORT_REG	idata

#endif
