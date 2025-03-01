#ifndef IG_MAIN
#define IG_MAIN

#include "./St.h"
#include "./CurDir.h"
#include "./Reedfile.h"
#include "./CpAlloc.h"
#include "./CppAlloc.h"
#include "./CpppAlloc.h"
#include "./UTF8_Byte.h"
#include "./Arr.h"
#include "./NumbDgt.h"

#include "./Nc.h"

// 出力開始位置
#define DF_TOP -4
#define DF_BOTTOM 12

// Linux ターミナル、Windows コマンドプロンプト 両方で取得出来るキー
#define DF_CTRL_B 2
#define DF_CTRL_E 5
#define DF_CTRL_G 7
#define DF_CTRL_K 11
#define DF_CTRL_L 12
#define DF_CTRL_N 14
#define DF_CTRL_O 15
#define DF_CTRL_P 16
#define DF_CTRL_T 20
#define DF_CTRL_X 24
#define DF_CTRL_Y 25
// CTRL + [
#define DF_CTRL_0X5B 27
// CTRL + ]
#define DF_CTRL_0X5D 29

// ライブラリ依存 キー
// ↑
#define DF_ARR_UP 259
// ↓
#define DF_ARR_DOWN 258
// ←
#define DF_ARR_LEFT 260
// →
#define DF_ARR_RIGHT 261

// キー設定

// Exit
#define DF_EXIT 'E'

// ↑
#define DF_UP 'W'

// ↓
#define DF_DOWN 'S'

// ←
#define DF_LEFT 'A'

// →
#define DF_RIGHT 'D'

// Key Input
#define DF_IP '\n'

// 文字削除
#define DF_DEL DF_CTRL_X

// Replace
#define DF_RE 'R'

// List
#define DF_LS 'L'
// File Open
#define DF_OPEN 'O'
// File Put
#define DF_PUT 'P'

//ファイルの切り替え
#define DF_FILE_UP ' '
#define DF_FILE_DOWND 'C'

int main(int, char**);
int MainInit(St_t* St, int, char**);
int MainKey(St_t* St);

int MainFileOpen(St_t* St);
int MainReedFile(St_t* St, char*, int);
int MainFilePut(St_t* St);
int MainWriteFile(St_t* St, char*);
int MainFileList(St_t* St);

int MainAdd(St_t* St);
int MainLF(St_t* St);
int MainCharDel(St_t* St);
int MainAs(St_t* St);
char* MainGetStr(int, int, char*);

int MainPrint(St_t* St);

#endif

