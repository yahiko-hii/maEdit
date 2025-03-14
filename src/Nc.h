#ifndef IG_NC
#define IG_NC

#include "./St.h"

int NcInit(void);
void NcGetmax(St_t*);
int Nckey(void);
void NcClear(void);
void NcCur(int, int);
void NcPrintOw(int, int, int);
void NcPrintStr(int, int, char*, int);
void NcPrintInt(int, int, int, int);
void NcExit(void);

#endif

