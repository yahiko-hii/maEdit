#ifndef IG_UTF8_BYTE
#define IG_UTF8_BYTE

short int UTF8_Byte(char*, int);
// 1バイト
static short int UTF8_Byte0(char*, int);
// 2バイト
static short int UTF8_Byte194(char*, int);
// 3バイト
static short int UTF8_Byte224(char*, int);
static short int UTF8_Byte225(char*, int);
static short int UTF8_Byte237(char*, int);
static short int UTF8_Byte238(char*, int);
// 4バイト
static short int UTF8_Byte240(char*, int);
static short int UTF8_Byte241(char*, int);
static short int UTF8_Byte244(char*, int);

int UTF8_ByteFl(char*, int, short int);

#endif

