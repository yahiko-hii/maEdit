﻿#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "./Main.h"

int main(int argc, char* argv[]){

	St_t* St;
	int r = 0;

		St = (St_t*)malloc(sizeof(St_t) * 1);
		if(St == NULL){
			return -1;
		}

		if(MainInit(St, argc, argv) != 0){
			free(St);
			return 1;
		}

		while(1){

			MainPrint(St);
			r = MainKey(St);
			if(r != 0){
				break;
			}

		}

		free(St);

	return 0;
}

// 初期化
int MainInit(St_t* St, int argc, char** argv){

	int i;
	int r;
	char* nf = (char*)"untitled";

		NcInit();

		St->Dat.Cd = CurDir(argc, argv);
		if(St->Dat.Cd == NULL){
			return -1;
		}

		St->Dat.IntDig = 2;

		St->C.Cur[0] = '|';
		St->C.Cur[1] = '\0';

		St->C.Eol[0] = '\0';
		St->C.Eol[1] = '\0';

		strncpy(St->C.Dm, "| ", 2);
		St->C.Dm[2]= '\0';

		St->C.Gs[0]= '-';
		St->C.Gs[1]= '\0';

		St->Key.CurLen = (int)strlen(St->C.Cur);
		St->Key.EolLen = (int)strlen(St->C.Eol);

		St->File.Dat = NULL;

		r = 0;
		// 引数で渡されたファイルを開く
		for(i = 1; i < argc; i++){

			r = MainReedFile(St, argv[i]);
			if(r < 0){
				break;
			}

		}
		ReedfileExit();
		// 0より小さい戻り値はメモリ確保失敗
		if(r < 0){
			return -1;
		}

		// ファイルが開けなかった場合、ファイル名のz0と空行z1のメモリを確保しておく
		if(St->File.Dat == NULL){

			i = (int)(strlen(St->Dat.Cd) + strlen(nf));
			St->File.Dat = CpppAlloc(0, 0, i);
			if(St->File.Dat == NULL){
				return -1;
			}
			sprintf(St->File.Dat[0][0], "%s%s", St->Dat.Cd, nf);
			St->File.Dat[0][0][i] = '\0';

			St->File.Dat = CpppAlloc(0, 1, 1);
			if(St->File.Dat == NULL){
				return -1;
			}
			St->File.Dat[0][1][0] = '\0';

			St->Pos.FileNum = 0;
			St->Pos.DatZ = 1;
			St->Pos.DatX = 0;

		}

	return 0;
}

// キーの処理
int MainKey(St_t* St){

	int i;
	int c;
	int z = 0;

		i = Nckey();
		c = toupper(i);

		// EXIT
		if(c == DF_EXIT){
				return 1;
		}
		// UP
		else if((c == DF_UP || c == DF_ARR_UP) && St->Pos.DatZ > 1){
			St->Pos.DatZ = St->Pos.DatZ - 1;
			z = 1;
		}
		// DOWN
		else if(
			(c == DF_DOWN || c == DF_ARR_DOWN)
			&&
			St->File.Dat[St->Pos.FileNum][St->Pos.DatZ] != NULL
			&& St->File.Dat[St->Pos.FileNum][St->Pos.DatZ + 1] != NULL
		){
			St->Pos.DatZ = St->Pos.DatZ + 1;
			z = 1;
		}
		// LEFT
		else if(c == DF_LEFT || c == DF_ARR_LEFT){

			if(St->Pos.DatX > 0){
				i = UTF8_ByteFl(St->File.Dat[St->Pos.FileNum][St->Pos.DatZ], St->Pos.DatX - 1, -1);
				St->Pos.DatX = i;
			}
			else if(St->Pos.DatZ >= 2){
				St->Pos.DatZ = St->Pos.DatZ - 1;
				St->Pos.DatX = (int)strlen(St->File.Dat[St->Pos.FileNum][St->Pos.DatZ]);
			}

		}
		// RIGHT
		else if(c == DF_RIGHT || c == DF_ARR_RIGHT){

			if(St->Pos.DatX < (int)strlen(St->File.Dat[St->Pos.FileNum][St->Pos.DatZ])){
				i = UTF8_ByteFl(St->File.Dat[St->Pos.FileNum][St->Pos.DatZ], St->Pos.DatX + 1, 1);
				St->Pos.DatX = i;
			}
			else if(St->File.Dat[St->Pos.FileNum][St->Pos.DatZ + 1] != NULL){
				St->Pos.DatZ = St->Pos.DatZ + 1;
				St->Pos.DatX = 0;
			}

		}
		//  文字削除
		else if(c == DF_DEL){
			if(MainCharDel(St) < 0){
				return -1;
			}
		}
		// ファイルオープン
		else if(c == DF_OPEN){

			i = MainFileOpen(St);
			if(i < 0){
				return -1;
			}
			else if(i != 0){
				return 0;
			}

			St->Pos.DatZ = 1;
			St->Pos.DatX = 0;

		}
		// 開いてるファイルリストの出力と切り替え
		else if(c == DF_LS){

			if(MainFileList(St) < 0){
				return -1;
			}

			St->Pos.DatZ = 1;
			St->Pos.DatX = 0;

		}
		// ファイルの切り替え
		else if(c == DF_FILE_UP && St->Pos.FileNum >= 1){
			St->Pos.FileNum = St->Pos.FileNum - 1;
			St->Pos.DatZ = 1;
			St->Pos.DatX = 0;
		}
		else if(c == DF_FILE_DOWND){

			for(i = 0; St->File.Dat[i] != NULL; i++);
			if(St->Pos.FileNum < i - 1){
				St->Pos.FileNum = St->Pos.FileNum + 1;
				St->Pos.DatZ = 1;
				St->Pos.DatX = 0;
			}

		}
		// Put 現在のファイルを出力
		else if(c == DF_PUT){
			if(MainFilePut(St) < 0){
				return -1;
			}
		}
		// 現在のファイルをクローズ
		else if(c == DF_CLOSE){

			St->File.Dat = CpppAllocDelPp(St->Pos.FileNum);
			if(St->File.Dat == NULL){
				return -1;
			}
			else if(St->Pos.FileNum > 0){
				St->Pos.FileNum = St->Pos.FileNum - 1;
			}

			St->Pos.DatZ = 1;
			St->Pos.DatX = 0;

		}
		//  検索
		else if(c == DF_FIND){
			if(MainFind(St) < 0){
				return -1;
			}
		}
		// 置き換え
		else if(c == DF_RE){
			if(MainAs(St) < 0){
				return -1;
			}
		}
		// ソート
		else if(c == DF_SORT){
			St->File.Dat[St->Pos.FileNum] = SortPp(St->File.Dat[St->Pos.FileNum], 1);
			if(St->File.Dat[St->Pos.FileNum] == NULL){
				return -1;
			}
			z = 1;
		}
		// キー入力
		else if(c == DF_IP){
			// 出力されている文字をスペースで上書き
			NcPrintOw(0, 0, (int)strlen(St->File.Dat[St->Pos.FileNum][0]));
			if(MainAdd(St) < 0){
				return -1;
			}
		}

		// Z軸移動の場合、配列の長さとマルチバイトの先頭位置を調べる
		if(z != 0){

			i = (int)strlen(St->File.Dat[St->Pos.FileNum][St->Pos.DatZ]);
			// カーソルXの位置が配列のサイズより大きい場合、配列の最後の位置から探す
			if(St->Pos.DatX > i){
				i = UTF8_ByteFl(St->File.Dat[St->Pos.FileNum][St->Pos.DatZ], i, -1);
			}
			else{
				i = UTF8_ByteFl(St->File.Dat[St->Pos.FileNum][St->Pos.DatZ], St->Pos.DatX, -1);
			}
			St->Pos.DatX = i;

		}

	return 0;
}

// ファイル名の入力
int MainFileOpen(St_t* St){

	char* ptr;
	int r;

		NcClear();

		NcPrintStr(0, 0, (char*)"Open File Name\n>", 0);

		ptr = MainGetStr(St, 1, 1, (char*)"\0", 1);
		if(ptr == NULL){
			return -1;
		}
		else if(ptr[0] == '\0' || ptr[0] == '\n'){
			return 0;
		}

		//ファイルを開く
		r = MainReedFile(St, ptr);
		ReedfileExit();
		if(r < 0){
			return -1;
		}

	return 0;
}

// ファイルから読み込み
int MainReedFile(St_t* St, char* fname){

	char* ptr;
	int line;
	int pos;

		if(St->File.Dat == NULL){
			pos = 0;
		}
		else{

			// 既に開いているファイルのリストと比較
			for(pos = 0; St->File.Dat[pos] != NULL; pos++){

				if(strcmp(fname, St->File.Dat[pos][0]) == 0){
					pos = -1;
					break;
				}

			}
			// 既に開いてるファイルなら戻る
			if(pos < 0){
				return 1;
			}

		}

		ptr = Reedfile(fname, INT_MAX);
		if(ptr == NULL || ptr[0] == '\0'){
			return 1;
		}

		St->Pos.FileNum = pos;

		// St->File.Dat[St->Pos.FileNum]のline 0にファイル名を収納
		pos = (int)strlen(fname);
		St->File.Dat = CpppAlloc(St->Pos.FileNum, 0, pos);
		if(St->File.Dat == NULL){
			return -1;
		}
		strncpy(St->File.Dat[St->Pos.FileNum][0], fname, pos);
		St->File.Dat[St->Pos.FileNum][0][pos] = '\0';

		// 最低1ライン、空行のメモリは確保しておく
		St->File.Dat = CpppAlloc(St->Pos.FileNum, 1, 1);
		if(St->File.Dat == NULL){
			return -1;
		}
		St->File.Dat[St->Pos.FileNum][1][0] = '\0';

		// lineは1から
		line = 1;

		pos = 0;
		while(1){

			// 改行もしくは最後の位置
			if(ptr[pos] == '\r' || ptr[pos] == '\n' || (ptr[pos] == '\0' && pos > 0)){

				// メモリを確保
				St->File.Dat = CpppAlloc(St->Pos.FileNum, line, pos);
				if(St->File.Dat == NULL){
					pos = -1;
					break;
				}
				strncpy(St->File.Dat[St->Pos.FileNum][line], ptr, pos);
				St->File.Dat[St->Pos.FileNum][line][pos] = '\0';

				if(ptr[pos] == '\0'){
					break;
				}

				ptr = &ptr[pos + 1];

				line = line + 1;
				pos = 0;

			}
			else if(ptr[pos] == '\0'){
				break;
			}
			else{
				pos = pos + 1;
			}

		}

		if(pos < 0){
			return -1;
		}

		St->Pos.DatZ = 1;
		St->Pos.DatX = 0;

	return 0;
}

// ファイルへ出力確認
int MainFilePut(St_t* St){

	int put_z;

	char* ptr;
	int i;
	char c;

		NcClear();

		put_z = 0;
		NcPrintStr(put_z, 0, (char*)"Put File Name", 0);
		put_z = put_z + 1;
		NcPrintStr(put_z, 0, (char*)">", 0);

		ptr = MainGetStr(St, put_z, 1, St->File.Dat[St->Pos.FileNum][0], 1);
		if(ptr == NULL){
			return -1;
		}
		else if(ptr[0] == '\0' || ptr[0] == '\n'){
			return 0;
		}

		put_z = put_z + 1;
		NcPrintStr(put_z, 0, (char*)"File Put? (Y)/n", 0);
		put_z = put_z + 1;
		NcPrintStr(put_z, 0, (char*)">", 0);

		// Yesでファイルへの書き込みへ
		i = Nckey();
		c = (char)toupper(i);
		if(c == '\0' || c == '\n' || c == 'Y'){
			if(MainWriteFile(St, ptr) != 0){
				return 1;
			}
		}

		// ファイル名の更新
		i = (int)strlen(ptr);
		St->File.Dat = CpppAlloc(St->Pos.FileNum, 0, i);
		if(St->File.Dat == NULL){
			return -1;
		}
		strncpy(St->File.Dat[St->Pos.FileNum][0], ptr, i);
		St->File.Dat[St->Pos.FileNum][0][i] = '\0';

	return 0;
}

// ファイルへ書き込み
int MainWriteFile(St_t* St, char* fname){

	FILE* fp;
	int i;
	int len;

		fp = fopen(fname, "wb");
		if(fp == NULL){
			return -1;
		}

		for(i = 1; St->File.Dat[St->Pos.FileNum][i] != NULL; i++){

			len = (int)strlen(St->File.Dat[St->Pos.FileNum][i]);
			if((int)fwrite(St->File.Dat[St->Pos.FileNum][i], sizeof(char), len, fp) != len){
				break;
			}
			else if(St->File.Dat[St->Pos.FileNum][i + 1] != NULL){
				fputc('\n', fp);
			}

		}

		fclose(fp);

	return 0;
}

// ファイルリスト
int MainFileList(St_t* St){

	int put_x;
	int fls;
	int i;

	char* ptr;

		NcClear();

		// ターミナルの出力位置
		put_x = 0;

		// 開かれているファイルをリスト
		for(fls = 0; St->File.Dat[fls] != NULL; fls++){

			put_x = 0;

			// ファイル番号
			NcPrintInt(fls + 3, put_x, fls, St->Dat.IntDig);
			// 区切り
			put_x = St->Dat.IntDig;
			NcPrintStr(fls + 3, put_x, (char*)": ", 0);
			// ファイル名
			put_x = put_x + 2;
			NcPrintStr(fls + 3, put_x, St->File.Dat[fls][0], 0);

		}

		NcPrintStr(0, 0, (char*)"Input Fine number", 0);
		NcPrintStr(1, 0, (char*)">", 0);

		ptr = MainGetStr(St, 1, 1, (char*)"\0", 1);
		if(ptr == NULL){
			return -1;
		}
		else if(ptr[0] == '\0' || ptr[0] == '\n'){
			return 1;
		}

		i = (int)strtol(ptr, NULL, 10);
		if(i >= 0 && i < fls){
			// ファイルを切り替え
			St->Pos.FileNum = i;
		}

		// 使用する数字の最大桁数を更新
		fls = NumbDgt((unsigned int)fls);
		if(fls > St->Dat.IntDig){
			St->Dat.IntDig = fls;
		}

	return 0;
}

// 文字列の挿入
int MainAdd(St_t* St){

	char* str;
	int str_len;
	int dat_len;
	int all_len;
	char* buf;

			NcPrintStr(0, 0, (char*)">", 0);

			// 文字列の取得
			str = MainGetStr(St, 0, 1, (char*)"\0", 0);
			CppAllocExit();
			if(str == NULL){
				return -1;
			}
			//  改行
			else if(str[0] == '\n'){
				if(MainLF(St) < 0){
					return -1;
				}
				return 0;
			}
			else if(str[0] == '\0'){
				return 0;
			}

			str_len = (int)strlen(str);

			if(St->File.Dat[St->Pos.FileNum][St->Pos.DatZ] != NULL){
				dat_len = (int)strlen(St->File.Dat[St->Pos.FileNum][St->Pos.DatZ]);
			}
			else{
				dat_len = 0;
			}

			if(INT_MAX - dat_len <= str_len){
				return -1;
			}
			all_len = str_len + dat_len;

			buf = (char*)malloc(sizeof(char) * all_len + 1);
			if(buf == NULL){
				return -1;
			}

			if(dat_len > 0){
				// DatXの位置まで、取得したstr、残りの位置から代入
				sprintf(buf, "%.*s%s%s",
					St->Pos.DatX, St->File.Dat[St->Pos.FileNum][St->Pos.DatZ],
					str,
					&St->File.Dat[St->Pos.FileNum][St->Pos.DatZ][St->Pos.DatX]
				);
			}
			else{
				strncpy(buf, str, str_len);
				buf[str_len] = '\0';
			}

			// 現在の位置にメモリを確保して、連結したbufを代入
			St->File.Dat = CpppAlloc(St->Pos.FileNum, St->Pos.DatZ, all_len);
			if(St->File.Dat == NULL){
				free(buf);
				return -1;
			}
			strncpy(St->File.Dat[St->Pos.FileNum][St->Pos.DatZ], buf, all_len);
			St->File.Dat[St->Pos.FileNum][St->Pos.DatZ][all_len] = '\0';

			free(buf);

			// カーソルの位置を変更
			St->Pos.DatX = St->Pos.DatX + str_len;

	return 0;
}

// 改行
int MainLF(St_t* St){

	int len;

		len = (int)strlen(St->File.Dat[St->Pos.FileNum][St->Pos.DatZ]) - St->Pos.DatX;

		St->File.Dat = CpppAllocAddPpp(St->Pos.FileNum, St->Pos.DatZ + 1, len);
		if(St->File.Dat == NULL){
			return -1;
		}

		strncpy(St->File.Dat[St->Pos.FileNum][St->Pos.DatZ + 1], &St->File.Dat[St->Pos.FileNum][St->Pos.DatZ][St->Pos.DatX], len);
		St->File.Dat[St->Pos.FileNum][St->Pos.DatZ + 1][len] = '\0';

		St->File.Dat[St->Pos.FileNum][St->Pos.DatZ][St->Pos.DatX] = '\0';

		St->Pos.DatZ = St->Pos.DatZ + 1;
		St->Pos.DatX = 0;

	return 0;
}

// 1文字削除
int MainCharDel(St_t* St){

	int len0;
	int len1;

	int pos0;
	int pos1;
	int diff;

		if(St->Pos.DatX > 0){

			len0 = (int)strlen(St->File.Dat[St->Pos.FileNum][St->Pos.DatZ]);

			// マルチバイトの-1文字の先頭位置
			pos0 = UTF8_ByteFl(St->File.Dat[St->Pos.FileNum][St->Pos.DatZ], St->Pos.DatX - 1, -1);
			// 元の位置との差分
			diff = St->Pos.DatX - pos0;
			// 元の位置
			pos1 = St->Pos.DatX;

			// カーソルの位置を移動
			St->Pos.DatX = pos0;

			// 位置のスライド
			while(St->File.Dat[St->Pos.FileNum][St->Pos.DatZ][pos1] != '\0'){
				St->File.Dat[St->Pos.FileNum][St->Pos.DatZ][pos0] = St->File.Dat[St->Pos.FileNum][St->Pos.DatZ][pos1];
				pos0 = pos0 + 1;
				pos1 = pos1 + 1;
			}
			// 差分の後ろの位置に\0
			St->File.Dat[St->Pos.FileNum][St->Pos.DatZ][len0 - diff] = '\0';

			return 0;
		}
		// FileNumの0はファイル名なのでZ位置は後ろに移動できない
		else if(St->Pos.DatZ <= 1){
			return 0;
		}

		len0 = (int)strlen(St->File.Dat[St->Pos.FileNum][St->Pos.DatZ - 1]);
		len1 = (int)strlen(St->File.Dat[St->Pos.FileNum][St->Pos.DatZ]);

		// 上の行の後ろに連結
		St->File.Dat = CpppAlloc(St->Pos.FileNum, St->Pos.DatZ - 1, len0 + len1);
		if(St->File.Dat == NULL){
			return -1;
		}
		strncat(
			St->File.Dat[St->Pos.FileNum][St->Pos.DatZ - 1],
			St->File.Dat[St->Pos.FileNum][St->Pos.DatZ],
			len1
		);
		St->File.Dat[St->Pos.FileNum][St->Pos.DatZ - 1][len0 + len1] = '\0';

		// 現在の場所を開放して後ろを連結
		St->File.Dat = CpppAllocDelPpp(St->Pos.FileNum, St->Pos.DatZ);
		if(St->File.Dat == NULL){
			return -1;
		}

		St->Pos.DatZ = St->Pos.DatZ - 1;
		St->Pos.DatX = len0;

	return 0;
}

// 検索
int MainFind(St_t* St){

	char* ptr0;
	char* ptr1;
	int z;
	int x;
	int i;

		NcPrintOw(0, 0, St->Pos.MaxX);
		NcPrintStr(0, 0, (char*)">", 1);

		ptr0 = MainGetStr(St, 0, 1, (char*)"\0", 1);
		if(ptr0 == NULL){
			return -1;
		}
		else if(ptr0[0] == '\0' || ptr0[0] == '\n'){
			return 1;
		}

		i = 0;
		z = St->Pos.DatZ;
		x = St->Pos.DatX;
		while(1){

			while(St->File.Dat[St->Pos.FileNum][z] != NULL){

				// 最初の位置の1つ前の行まで来たら抜ける
				if(z == St->Pos.DatZ - 1){
					i = 2;
					break;
				}

				ptr1 = ArrSrch(&St->File.Dat[St->Pos.FileNum][z][x], ptr0, 0);
				if(ptr1 != NULL){
					St->Pos.DatZ = z;
					St->Pos.DatX = (int)(strlen(St->File.Dat[St->Pos.FileNum][z]) - strlen(ptr1));
					St->Pos.DatX = St->Pos.DatX + (int)strlen(ptr0);
					i = 2;
					break;
				}

				z = z + 1;
				x = 0;

			}

			i = i + 1;
			if(i >= 2){
				break;
			}

			z = 1;
			x = 0;

		}

	return 0;
}

// 置き換え
int MainAs(St_t* St){

	int put_z;

	char* ptr;

	char** ls;
	int ls0_len;
	int ls1_len;
	int all_len;
	int pos;

	short int al;

	int z;

	int i = 0;

		put_z = 0;
		NcClear();

		// 置き換える対象の文字列の取得
		NcPrintStr(put_z, 0, (char*)"Src", 0);
		put_z = put_z + 1;
		NcPrintStr(put_z, 0, (char*)">", 0);

		ptr = MainGetStr(St, put_z, 1, (char*)"\0", 1);
		if(ptr == NULL){
			return -1;
		}
		else if(ptr[0] == '\0' || ptr[0] == '\n'){
			return 0;
		}
		ls0_len = (int)strlen(ptr);
		ls = CppAlloc(0, ls0_len);
		if(ls == NULL){
			return -1;
		}
		strncpy(ls[0], ptr, ls0_len);
		ls[0][ls0_len] = '\0';

		put_z = put_z + 1;

		// 置き換える文字列の取得
		NcPrintStr(put_z, 0, (char*)"Dst", 0);
		put_z = put_z + 1;
		NcPrintStr(put_z, 0, (char*)">", 0);

		ptr = MainGetStr(St, put_z, 1, (char*)"\0", 1);
		if(ptr == NULL){
			CppAllocExit();
			return -1;
		}
		else if(ptr[0] == '\0' || ptr[0] == '\n'){
			ptr[0] = '\0';
			ls1_len = 0;
		}
		else{
			ls1_len = (int)strlen(ptr);
		}
		ls = CppAlloc(1, ls1_len);
		if(ls == NULL){
			return -1;
		}
		strncpy(ls[1], ptr, ls1_len);
		ls[1][ls1_len] = '\0';

		put_z = put_z + 1;

		// 大文字小文字を区別するかどうか
		NcPrintStr(put_z, 0, (char*)"case-insensitive? (Y)/n", 0);
		put_z = put_z + 1;
		NcPrintStr(put_z, 0, (char*)">", 0);

		ptr = MainGetStr(St, put_z, 1, (char*)"\0", 1);
		if(ptr == NULL){
			CppAllocExit();
			return -1;
		}
		// yなら区別しない
		if(ptr[0] == '\0' || ptr[0] == '\n' || ArrCmp(ptr, (char*)"Y", 0) == 0){
			al = 0;
		}
		else{
			al = 1;
		}

		// 置き換え対象と置き換える文字が同じ
		if(ArrCmp(ls[0], ls[1], al) == 0){
			CppAllocExit();
			return 0;
		}

		for(z = 1; St->File.Dat[St->Pos.FileNum][z] != NULL; z++){

			// 文字を探す位置
			i = 0;
			while(1){

				// 文字検索
				ptr = ArrSrch(&St->File.Dat[St->Pos.FileNum][z][i], ls[0], al);
				if(ptr == NULL){
					break;
				}
				pos = (int)(strlen(St->File.Dat[St->Pos.FileNum][z]) - strlen(ptr));

				all_len = (int)strlen(St->File.Dat[St->Pos.FileNum][z]) - ls0_len + ls1_len;
				ptr = CpAlloc(all_len);
				if(ptr == NULL){
					i = -1;
					break;
				}
				// 見つかった位置まで、置き換える文字、置き換える文字の後から最後まで
				sprintf(ptr, "%.*s%s%s",
					pos, St->File.Dat[St->Pos.FileNum][z],
					ls[1],
					&St->File.Dat[St->Pos.FileNum][z][pos + ls0_len]
				);

				St->File.Dat = CpppAlloc(St->Pos.FileNum, z, all_len);
				if(St->File.Dat == NULL){
					i = -1;
					break;
				}
				strncpy(St->File.Dat[St->Pos.FileNum][z], ptr, all_len);
				St->File.Dat[St->Pos.FileNum][z][all_len] = '\0';

				// 次の探す位置
				i = pos + ls1_len;

			}

			if(i < 0){
				break;
			}

		}

		CppAllocExit();

		St->Pos.DatX = 0;

	return i;
}

// 文字列の取得
char* MainGetStr(St_t* St, int z, int x, char* arg, int f){

	static char* ptr;
	int pos = 0;
	int c;

	char** ls = NULL;
	int ls_num;

	int len_ptr;
	int len_ls;
	int len_all;

		ls_num = -1;

		if(arg == NULL){
			pos = 0;
			arg = (char*)"\0";
		}
		else{
			pos = (int)strlen(arg);
		}

		// 確保されているメモリ < 指定された長さならメモリが再確保される。
		ptr = CpAlloc(pos);
		if(ptr == NULL){
			return NULL;
		}
		strncpy(ptr, arg, pos);
		ptr[pos] = '\0';

		while(1){

			//  出力されている文字をスペースで上書き
			NcPrintOw(z, x, pos + 1);
			NcPrintStr(z, x, ptr, 0);

			c = (char)Nckey();

			if(c == EOF){
				break;
			}
			else if(c == '\n'){

				/* トークンリストから文字列の利用を確定 */
				if(ls != NULL && ls_num >= 0){

					len_ptr = (int)strlen(ptr);
					len_ls = (int)strlen(ls[ls_num]);
					len_all = len_ptr + len_ls;

					ptr = CpAlloc(len_all);
					if(ptr == NULL){
						break;
					}

					memmove(ptr + len_ptr, ls[ls_num], len_ls);
					ptr[len_all] = '\0';

				}
				else if(pos == 0){
					ptr[0] = '\n';
					ptr[1] = '\0';
				}

				break;
			}
			//  文字削除
			else if(c == DF_DEL && pos > 0){
				pos = UTF8_ByteFl(ptr, pos - 1, -1);
				ptr[pos] = '\0';
			}
			else if(c == '\t' || (unsigned char)c >= 32 && (unsigned char)c != 127){

				ptr = CpAlloc(pos);
				if(ptr == NULL){
					break;
				}

				ptr[pos] = (char)c;
				pos = pos + 1;
				ptr[pos] = '\0';

			}

			/* トークンを利用しない場合コンテニュー */
			if(f != 0){
				continue;
			}
			/* トークンを探す */
			ls = Tok(St->Dat.Cd, ptr);
			if(ls == NULL){
				continue;
			}
			/* Ctrl+Nでトークンリストの位置を移動 */
			if(c == DF_CTRL_N){
				ls_num = ls_num + 1;
			}
			ls_num = MainTok(St, z, x, ls, ls_num);

		}
		if(ptr == NULL){
			return NULL;
		}

	return ptr;
}

int MainTok(St_t* St, int z, int x, char** ls, int ls_num){

	int i;
	int len = 0;
	int dgt;

		z = z + 1;
		NcPrintOw(z, 0, St->Pos.MaxX);
		NcPrintStr(z, x + 1, (char*)"Next: Ctrl+N", 0);

		z = z + 1;
		for(i = 0; ls[i] != NULL; i++){

			len = (int)strlen(ls[i]);
			NcPrintOw(z, 0, len + 8);

			if(i == ls_num){
				NcPrintStr(z, x, (char*)">", 0);
				NcPrintStr(z, x + 1, ls[i], 0);
			}
			else{
				NcPrintStr(z, x + 1, ls[i], 0);
			}

			z = z + 1;
		}
		NcPrintOw(z, 0, len + 4);

		// 使用する数字の最大桁数を更新
		dgt = NumbDgt((unsigned int)i);
		if(dgt > St->Dat.IntDig){
			St->Dat.IntDig = dgt;
		}

		if(ls_num >= 0 && ls_num < i){
			return ls_num;
		}

	return -1;
}

// 出力
int MainPrint(St_t* St){

	char* text[] = {
		(char*)"Move:W or S or A or D, Exit:Ctrl+E",
		(char*)"AddStr:Enter, Del:Ctrl+X, Find:F, Repl:R, Sort:T,",
		(char*)"Open:O, Put:P, Close:E, List:L or C or Space,",
		NULL
	};
	int i;

	int put_z;
	int put_x;

	int dat_top;
	int dat_bottom;

	char* ptr;
	int len;
	int all_len;
	int cr = 0;

		NcClear();

		// 配列の参照開始位置
		dat_top = St->Pos.DatZ + DF_TOP;
		if(dat_top < 1){
			dat_top = 1;
		}
		dat_bottom = St->Pos.DatZ + DF_BOTTOM;

		// ターミナルのサイズを取得
		NcGetmax(St);
		if(St->Pos.MaxX > 8){
			St->Pos.MaxX = (int)(St->Pos.MaxX * 0.9);
		}
		// ターミナルの出力位置
		put_z = 0;
		put_x = 0;

		NcPrintStr(put_z, put_x, St->File.Dat[St->Pos.FileNum][0], 0);
		put_z = put_z + 1;
		for(i = 0; i < St->Pos.MaxX; i++){
			NcPrintStr(put_z, i, (char*)St->C.Gs, 0);
		}
		put_z = put_z + 1;

		for(i = dat_top; St->File.Dat[St->Pos.FileNum][i] != NULL; i++){

			if(i > dat_bottom){
				break;
			}

			put_x = 0;

			// 行番号
			NcPrintInt(put_z, put_x, i, St->Dat.IntDig);
			put_x = St->Dat.IntDig;
			NcPrintStr(put_z, put_x, (char*)St->C.Dm, 0);
			put_x = (int)(put_x + strlen(St->C.Dm));

			len = (int)strlen(St->File.Dat[St->Pos.FileNum][i]);

			if((INT_MAX - len) <= (St->Key.CurLen + St->Key.EolLen)){
				break;
			}
			all_len = len + St->Key.CurLen + St->Key.EolLen;
			// 確保されているメモリ < 指定された長さならメモリが再確保される。
			ptr = CpAlloc(all_len);
			if(ptr == NULL){
				break;
			}

			// 出力位置が現在の位置と同じ場合、カーソルも含める
			if(i == St->Pos.DatZ){

				sprintf(ptr, "%.*s%s%s%s",
					St->Pos.DatX, St->File.Dat[St->Pos.FileNum][i],
					St->C.Cur,
					&St->File.Dat[St->Pos.FileNum][i][St->Pos.DatX],
					St->C.Eol
				);

				len = len + St->Key.CurLen + St->Key.EolLen;

			}
			else{
				sprintf(ptr, "%s%s", St->File.Dat[St->Pos.FileNum][i], St->C.Eol);
				len = len + St->Key.EolLen;
			}

			// CRのポイントで改行しつつ出力
			while(len > St->Pos.MaxX){

				// マルチバイトでの現在の文字の先頭位置
				cr = UTF8_ByteFl(ptr, St->Pos.MaxX, -1);
				if(cr < 1){
					break;
				}

				// 出力する最後の位置を指定して出力する
				NcPrintStr(put_z, put_x, ptr, cr);

				ptr = &ptr[cr];
				len = len - cr;

				put_z = put_z + 1;

			}
			if(len > 0){
				NcPrintStr(put_z, put_x, ptr, 0);
			}

			put_z = put_z + 1;

		}

		// 使用する数字の最大桁数を更新
		i = NumbDgt((unsigned int)i);
		if(i > St->Dat.IntDig){
			St->Dat.IntDig = i;
		}

		put_x = 0;
		// 区切りと説明テキストを出力
		for(i = 0; i < St->Pos.MaxX; i++){
			NcPrintStr(put_z, i, (char*)St->C.Gs, 0);
		}
		for(i = 0; text[i] != NULL; i++){
			put_z = put_z + 1;
			NcPrintStr(put_z, put_x, text[i], 0);
		}

	return 0;
}

