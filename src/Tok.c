#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <threads.h>

#include "./Tok.h"

thread_local static char** Sg_TokLs;
thread_local static int Sg_TokLs_Pos;

char** Tok(char* cd, char* src){

	/* src配列の場所 */
	int src_pos = 0;

	char ch[5];
	/* 1文字分の長さ */
	short int byte_len;
	/* 前回の1文字分の長さ */
	short int ord_byte_len = 0;

	char str[DF_TOK_MAX + 1];
	char* path;

	char* dir = (char*)"dat/str/";

	int r = 0;

		Sg_TokLs = NULL;
		Sg_TokLs_Pos = 0;

		/* カレントディレクトリ + ディレクトリ + 最大strlen + 区切り分と\0 */
		path = (char*)malloc(sizeof(char*) * (strlen(cd) + DF_TOK_MAX + strlen(dir) + 4));
		if(path == NULL){
			return NULL;
		}

		path[0] = '\0';
		str[0] = '\0';

		while(1){

			if(src[src_pos] == '\0'){
				break;
			}

			/* 1文字のバイト数が返ってくるのでそのバイト分を代入 */
			byte_len = UTF8_Byte(src, src_pos);
			if(byte_len < 1 || byte_len > 4){
				r = -1;
				break;
			}
			strncpy(ch, &src[src_pos], byte_len);
			ch[byte_len] = '\0';

			/* 文字処理 */
			if(TokStr(str, DF_TOK_MAX, ch, byte_len, ord_byte_len) == NULL){
				r = -1;
				break;
			}

			/* strのファイルが開けたら文字列をlsに代入 */
			if(str[0] != '\0'){
				sprintf(path, "%s%s%s", cd, dir, str);
				r = TokDir(path);
				if(r < 0){
					break;
				}
			}

			/* srcの参照位置と前回のバイトの長さを更新 */
			src_pos = src_pos + byte_len;
			ord_byte_len = byte_len;

		}
		free(path);
		ReedfileExit();
		if(r < 0){
			return NULL;
		}

	return Sg_TokLs;
}

static char* TokStr(char* str, int tok_max, char* ch, short int byte_len, short int ord_byte_len){

	int len_str;
	int len_ch;
	int len_all;

			len_str = (int)strlen(str);
			len_ch = (int)strlen(ch);
			len_all = (int)(len_str + len_ch);

			/* strのサイズをオーバーもしくはアスキーコードで使用できる文字以外 */
			if(
				len_all >= tok_max
				||
				(byte_len == 1 && isalnum(ch[0]) == 0 && ch[0] != '.' && ch[0] != '-' && ch[0] != '_' && ch[0] != '#')
			){
				str[0] = '\0';
				return str;
			}
			/* strの後ろにchを移動 */
			else if(byte_len == ord_byte_len){
				memmove(str + len_str, ch, len_ch);
				str[len_all] = '\0';
			}
			/* 文字種が変わったのでchを最初にコピー */
			else{
				strncpy(str, ch, len_ch);
				str[len_ch] = '\0';
			}

	return str;
}

static int TokDir(char* path){

	char* ptr;
	char* sc;
	int len;
	int r = 0;

		ptr = Reedfile(path, INT_MAX);
		if(ptr == NULL){
			return 1;
		}
		else if(ptr[0] == '\0'){
			return 0;
		}

		while(1){

			sc = strchr(ptr, '\n');
			if(sc == NULL){
				break;
			}

			len = (int)(strlen(ptr) - strlen(sc));
			if(len < 1){
				break;
			}
			Sg_TokLs = CppAlloc(Sg_TokLs_Pos, len);
			if(Sg_TokLs == NULL){
				r = -1;
				break;
			}
			strncpy(Sg_TokLs[Sg_TokLs_Pos], ptr, len);
			if(Sg_TokLs[Sg_TokLs_Pos][len - 1] == '\r'){
				Sg_TokLs[Sg_TokLs_Pos][len - 1] = '\0';
			}
			else{
				Sg_TokLs[Sg_TokLs_Pos][len] = '\0';
			}

			Sg_TokLs_Pos = Sg_TokLs_Pos + 1;
			ptr = &ptr[len + 1];

		}

	return r;
}

