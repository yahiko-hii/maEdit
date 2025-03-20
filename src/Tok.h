#ifndef RG_TOK
#define RG_TOK

#include "./UTF8_Byte.h"
#include "./Reedfile.h"
#include "./CppAlloc.h"

#define DF_TOK_MAX 1024

char** Tok(char*, char*);
static char* TokStr(char*, int, char*, short int, short int);
static int TokDir(char*);

#endif

