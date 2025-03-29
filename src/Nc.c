//標準以外のライブラリの使用ファイル

/*
// マクロ
__unix__
__linux__
_WIN32
_WIN64
__MINGW32__
__MINGW64__
*/

#ifdef _WIN64
	#include <curses.h>
#elif __MINGW64_
	#include <curses.h>
#else
	#include <ncurses.h>
#endif

#include <stdlib.h>
#include <locale.h>

#include "./Nc.h"

int NcInit(void){

	static short int state = 0;

		// 終了時の関数を登録
		if(state == 0){
			atexit(NcExit);
			state = 1;
		}

		setlocale(LC_ALL, "");

		// 初期化
		initscr();

		// 入力バッファを使用しない
		cbreak();

		// 入力文字を表示しない
		noecho();

		// カーソルを表示しない
		curs_set(0);

		// カーソルキーを有効
		keypad(stdscr, TRUE);

	return 0;
}
// Max サイズ
void NcGetmax(St_t* St){
	getmaxyx(stdscr, St->Pos.MaxZ, St->Pos.MaxX);
}
// キー取得
int Nckey(void){
	return getch();
}
// クリア
void NcClear(void){
		clear();
}
// カーソル位置を変更
void NcCur(int z, int x){
		move(z, x);
}
// スペースで上書き
void NcPrintOw(int z, int x, int len){

	int i;

		if(x < 0){
			x = 0;
		}

		//  出力されている文字をスペースで上書き
		for(i = x; i < x + len; i++){
			move(z, i);
			addstr(" ");
		}

}
// 文字列を出力
void NcPrintStr(int z, int x, char* str, int fmt){

		NcCur(z, x);

		if(fmt == 0){
			addstr(str);
		}
		else{
			printw("%.*s", fmt, str);
		}

}
// 数字を出力
void NcPrintInt(int z, int x, int i, int fmt){

		NcCur(z, x);

		if(fmt == 0){
			printw("%d", i);
		}
		else{
			printw("%.*d", fmt, i);
		}

}
// 終了時に呼び出す
void NcExit(void){
		endwin();
}

