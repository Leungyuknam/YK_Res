#ifndef _TYPEDEF_H_
#define	_TYPEDEF_H_

typedef unsigned char   U8 ;
typedef unsigned int    U16	;
typedef unsigned long   U32	 ;
typedef          bit    Bit;
typedef          bit    Bool;
typedef void (*voidFan)(void);
typedef void (*U8Fan)(U8 DA);

#define  S8     signed char              
#define  S16    signed int               
#define  S32    signed long 
#define  s8     signed char              
#define  s16    signed int               
#define  s32    signed long 

#define	S_REG  		data
#define	SHORT_REG	idata
#define	LONG_REG	xdata

#define	CtoF(x)		(((((int)(x)) * 90) / 5 + 325) / 10)
#define	FtoC(x)		(((((int)(x)) - 32) * 50 / 9 + 5) / 10)

#define   	NOP()				{_nop_();}

//BIT¶¨Òå¡¡¡¡
#define  BIT0   0x01
#define  BIT1   0x02
#define  BIT2   0x04
#define  BIT3   0x08
#define  BIT4   0x10
#define  BIT5   0x20
#define  BIT6   0x40
#define  BIT7   0x80
//#define	 BIT8   0x0100
//#define	 BIT9   0x0200
//#define	 BIT10  0x0400
//#define	 BIT11  0x0800

typedef union{
     unsigned char Byte;
	 struct{
		  unsigned char b0 : 1;
		  unsigned char b1 : 1;
		  unsigned char b2 : 1;
		  unsigned char b3 : 1;
		  unsigned char b4 : 1;
		  unsigned char b5 : 1;
		  unsigned char b6 : 1;
		  unsigned char b7 : 1;
		}BBit;
}StrByte;             

#define B00000000 0x00
#define B00000001 0x01
#define B00000010 0x02
#define B00000011 0x03
#define B00000100 0x04
#define B00000101 0x05
#define B00000110 0x06
#define B00000111 0x07
#define B00001000 0x08
#define B00001001 0x09
#define B00001010 0x0A
#define B00001011 0x0B
#define B00001100 0x0C
#define B00001101 0x0D
#define B00001110 0x0E
#define B00001111 0x0F	


#define B00010000 0x10
#define B00010001 0x11
#define B00010010 0x12
#define B00010011 0x13
#define B00010100 0x14
#define B00010101 0x15
#define B00010110 0x16
#define B00010111 0x17
#define B00011000 0x18
#define B00011001 0x19
#define B00011010 0x1A
#define B00011011 0x1B
#define B00011100 0x1C
#define B00011101 0x1D
#define B00011110 0x1E
#define B00011111 0x1F

#define B00100000 0x20
#define B00100001 0x21
#define B00100010 0x22
#define B00100011 0x23
#define B00100100 0x24
#define B00100101 0x25
#define B00100110 0x26
#define B00100111 0x27
#define B00101000 0x28
#define B00101001 0x29
#define B00101010 0x2A
#define B00101011 0x2B
#define B00101100 0x2C
#define B00101101 0x2D
#define B00101110 0x2E
#define B00101111 0x2F

#define B00110000 0x30
#define B00110001 0x31
#define B00110010 0x32
#define B00110011 0x33
#define B00110100 0x34
#define B00110101 0x35
#define B00110110 0x36
#define B00110111 0x37
#define B00111000 0x38
#define B00111001 0x39
#define B00111010 0x3A
#define B00111011 0x3B
#define B00111100 0x3C
#define B00111101 0x3D
#define B00111110 0x3E
#define B00111111 0x3F

#define B01000000 0x40
#define B01000001 0x41
#define B01000010 0x42
#define B01000011 0x43
#define B01000100 0x44
#define B01000101 0x45
#define B01000110 0x46
#define B01000111 0x47
#define B01001000 0x48
#define B01001001 0x49
#define B01001010 0x4A
#define B01001011 0x4B
#define B01001100 0x4C
#define B01001101 0x4D
#define B01001110 0x4E
#define B01001111 0x4F

#define B01010000 0x50
#define B01010001 0x51
#define B01010010 0x52
#define B01010011 0x53
#define B01010100 0x54
#define B01010101 0x55
#define B01010110 0x56
#define B01010111 0x57
#define B01011000 0x58
#define B01011001 0x59
#define B01011010 0x5A
#define B01011011 0x5B
#define B01011100 0x5C
#define B01011101 0x5D
#define B01011110 0x5E
#define B01011111 0x5F

#define B01100000 0x60
#define B01100001 0x61
#define B01100010 0x62
#define B01100011 0x63
#define B01100100 0x64
#define B01100101 0x65
#define B01100110 0x66
#define B01100111 0x67
#define B01101000 0x68
#define B01101001 0x69
#define B01101010 0x6A
#define B01101011 0x6B
#define B01101100 0x6C
#define B01101101 0x6D
#define B01101110 0x6E
#define B01101111 0x6F

#define B01110000 0x70
#define B01110001 0x71
#define B01110010 0x72
#define B01110011 0x73
#define B01110100 0x74
#define B01110101 0x75
#define B01110110 0x76
#define B01110111 0x77
#define B01111000 0x78
#define B01111001 0x79
#define B01111010 0x7A
#define B01111011 0x7B
#define B01111100 0x7C
#define B01111101 0x7D
#define B01111110 0x7E
#define B01111111 0x7F

#define B10000000 0x80
#define B10000001 0x81
#define B10000010 0x82
#define B10000011 0x83
#define B10000100 0x84
#define B10000101 0x85
#define B10000110 0x86
#define B10000111 0x87
#define B10001000 0x88
#define B10001001 0x89
#define B10001010 0x8A
#define B10001011 0x8B
#define B10001100 0x8C
#define B10001101 0x8D
#define B10001110 0x8E
#define B10001111 0x8F

#define B10010000 0x90
#define B10010001 0x91
#define B10010010 0x92
#define B10010011 0x93
#define B10010100 0x94
#define B10010101 0x95
#define B10010110 0x96
#define B10010111 0x97
#define B10011000 0x98
#define B10011001 0x99
#define B10011010 0x9A
#define B10011011 0x9B
#define B10011100 0x9C
#define B10011101 0x9D
#define B10011110 0x9E
#define B10011111 0x9F

#define B10100000 0xA0
#define B10100001 0xA1
#define B10100010 0xA2
#define B10100011 0xA3
#define B10100100 0xA4
#define B10100101 0xA5
#define B10100110 0xA6
#define B10100111 0xA7
#define B10101000 0xA8
#define B10101001 0xA9
#define B10101010 0xAA
#define B10101011 0xAB
#define B10101100 0xAC
#define B10101101 0xAD
#define B10101110 0xAE
#define B10101111 0xAF

#define B10110000 0xB0
#define B10110001 0xB1
#define B10110010 0xB2
#define B10110011 0xB3
#define B10110100 0xB4
#define B10110101 0xB5
#define B10110110 0xB6
#define B10110111 0xB7
#define B10111000 0xB8
#define B10111001 0xB9
#define B10111010 0xBA
#define B10111011 0xBB
#define B10111100 0xBC
#define B10111101 0xBD
#define B10111110 0xBE
#define B10111111 0xBF

#define B11000000 0xC0
#define B11000001 0xC1
#define B11000010 0xC2
#define B11000011 0xC3
#define B11000100 0xC4
#define B11000101 0xC5
#define B11000110 0xC6
#define B11000111 0xC7
#define B11001000 0xC8
#define B11001001 0xC9
#define B11001010 0xCA
#define B11001011 0xCB
#define B11001100 0xCC
#define B11001101 0xCD
#define B11001110 0xCE
#define B11001111 0xCF

#define B11010000 0xD0
#define B11010001 0xD1
#define B11010010 0xD2
#define B11010011 0xD3
#define B11010100 0xD4
#define B11010101 0xD5
#define B11010110 0xD6
#define B11010111 0xD7
#define B11011000 0xD8
#define B11011001 0xD9
#define B11011010 0xDA
#define B11011011 0xDB
#define B11011100 0xDC
#define B11011101 0xDD
#define B11011110 0xDE
#define B11011111 0xDF

#define B11100000 0xE0
#define B11100001 0xE1
#define B11100010 0xE2
#define B11100011 0xE3
#define B11100100 0xE4
#define B11100101 0xE5
#define B11100110 0xE6
#define B11100111 0xE7
#define B11101000 0xE8
#define B11101001 0xE9
#define B11101010 0xEA
#define B11101011 0xEB
#define B11101100 0xEC
#define B11101101 0xED
#define B11101110 0xEE
#define B11101111 0xEF

#define B11110000 0xF0
#define B11110001 0xF1
#define B11110010 0xF2
#define B11110011 0xF3
#define B11110100 0xF4
#define B11110101 0xF5
#define B11110110 0xF6
#define B11110111 0xF7
#define B11111000 0xF8
#define B11111001 0xF9
#define B11111010 0xFA
#define B11111011 0xFB
#define B11111100 0xFC
#define B11111101 0xFD
#define B11111110 0xFE
#define B11111111 0xFF

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

#endif