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

int NcInit(St_t* St){

	static short int state = 0;

		// 終了時の関数を登録
		if(state == 0){
			atexit(NcExit);
			state = 1;
		}

		setlocale(LC_ALL, "");

		// 初期化
		initscr();

		getmaxyx(stdscr, St->Pos.MaxY, St->Pos.MaxX);

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
// キー取得
int Nckey(void){
	return getch();
}
// クリア
void NcClear(void){
		clear();
}
// カーソル位置を変更
void NcCur(int y, int x){
		move(y, x);
}
// スペースで上書き
void NcPrintOw(int y, int x, int len){

	int i;

			move(y, x);

			//  出力されている文字をスペースで上書き
			for(i = x; i < x + len; i++){
				NcPrintStr(y, i, (char*)" ", 0);
			}

}
// 文字列を出力
void NcPrintStr(int y, int x, char* str, int fmt){

		NcCur(y, x);

		if(fmt == 0){
			addstr(str);
		}
		else{
			printw("%.*s", fmt, str);
		}

}
// 数字を出力
void NcPrintInt(int y, int x, int i, int fmt){

		NcCur(y, x);

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

