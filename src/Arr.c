#include <string.h>
#include <ctype.h>

#include "./Arr.h"

char* ArrSrch(char* str0, char* str1, short int al){

	size_t str0_len;
	size_t str1_len;

	size_t pos;
	size_t find0;
	size_t find1 = 0;

		if(str0 == NULL || str1 == NULL){
			return NULL;
		}

		str0_len = strlen(str0);
		str1_len = strlen(str1);
		// 探す対象の文字列が探す文字列より少ない、もしくは探す文字列の長さが0
		if(str0_len < str1_len || str1_len == 0){
			return NULL;
		}

		for(pos = 0; str0[pos]; pos++){

			// str0[find0]の比較開始位置
			find0 = pos;
			// str1の位置
			find1 = 0;

			// 探す対象の文字の位置をスライドさせていく
			while(str0[find0] != '\0' && str1[find1] != '\0'){

				// 比較
				if(ArrChar(str0[find0], str1[find1], al) != 0){
					break;
				}

				find0 = find0 + 1;
				find1 = find1 + 1;

			}

			if(str1[find1] == '\0'){
				break;
			}

		}

		// 検索開始位置を返す
		if(str1[find1] == '\0'){
			return &str0[pos];
		}

	return NULL;
}

// 一致しないなら1を返す
short int ArrCmp(char* str0, char* str1, short int al){

	size_t str0_len;
	size_t str1_len;

	size_t pos;

	short int r = 0;

		str0_len = strlen(str0);
		str1_len = strlen(str1);
		// 長さが違うから一致ではない
		if(str0_len < str1_len){
			return -1;
		}
		else if(str0_len > str1_len){
			return 1;
		}

		for(pos = 0; str0[pos]; pos++){

			// 比較
			r = ArrChar(str0[pos], str1[pos], al);
			if(r != 0){
				break;
			}

		}

	return r;
}

static short int ArrChar(char c0, char c1, short int al){

	int i0;
	int i1;

		// alが0なら大文字に変換して比較。違う文字なら1か-1を返す
		if(al == 0){

			i0 =  toupper((unsigned char)c0);
			i1 =  toupper((unsigned char)c1);

			if(i0 < i1){
				return -1;
			}
			else if(i0 > i1){
				return 1;
			}

		}
		else{

			if((unsigned char)c0 < (unsigned char)c1){
				return -1;
			}
			else if((unsigned char)c0 > (unsigned char)c1){
				return 1;
			}

		}

	return 0;
}

