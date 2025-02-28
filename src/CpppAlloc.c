#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <threads.h>

#include "./CpppAlloc.h"

#define DF_OFFSET 2

thread_local static char*** Sg_CpppAlloc = NULL;

char*** CpppAlloc(int pos0, int pos1, int len){

	static short int state = 0;

		// 終了時の関数を登録
		if(state == 0){
			atexit(CpppAllocExit);
			state = 1;
		}

		if(pos0 < 0 || pos0 > INT_MAX - DF_OFFSET){
			return NULL;
		}
		else if(pos1 < 0 || pos1 > INT_MAX - DF_OFFSET){
			return NULL;
		}
		else if(len < 0 || len > INT_MAX - DF_OFFSET){
			return NULL;
		}

		if(CpppAlloc0(pos0) != 0){
			return NULL;
		}
		else if(CpppAlloc1(pos0, pos1) != 0){
			return NULL;
		}
		else if(CpppAlloc2(pos0, pos1, len) != 0){
			return NULL;
		}

	return Sg_CpppAlloc;
}

static short int CpppAlloc0(int pos0){

	int alloc_size;
	char*** ptr;

		if(Sg_CpppAlloc == NULL){

			// Sg_CpppAllocがNULLの場合でメモリを確保する位置が0じゃない場合
			if(pos0 != 0){
				return 1;
			}

			Sg_CpppAlloc = (char***)malloc(sizeof(char*) * (pos0 + DF_OFFSET));
			if(Sg_CpppAlloc == NULL){
				return 1;
			}
			Sg_CpppAlloc[0] = NULL;
			Sg_CpppAlloc[1] = NULL;

			return 0;
		}

		// 最後の位置より前か後か
		for(alloc_size = 0; Sg_CpppAlloc[alloc_size] != NULL; alloc_size++);
		if(pos0 < alloc_size){
			return 0;
		}
		else if(pos0 > alloc_size){
			return 1;
		}

		ptr = (char***)realloc(Sg_CpppAlloc, sizeof(char*) * (pos0 + DF_OFFSET));
		if(ptr == NULL){
			CpppAllocExit();
			return 1;
		}
		Sg_CpppAlloc = ptr;

		if(pos0 == alloc_size){
			Sg_CpppAlloc[pos0 + 1] = NULL;
		}

	return 0;
}

static short int CpppAlloc1(int pos0, int pos1){

	int alloc_size;
	char** ptr;

		if(Sg_CpppAlloc[pos0] == NULL){

			// Sg_CpppAlloc[pos0]がNULLの場合でメモリを確保する位置が0じゃない場合
			if(pos1 != 0){
				return 1;
			}

			Sg_CpppAlloc[pos0] = (char**)malloc(sizeof(char*) * (pos1 + DF_OFFSET));
			if(Sg_CpppAlloc[pos0] == NULL){
				CpppAllocExit();
				return 1;
			}
			Sg_CpppAlloc[pos0][0] = NULL;
			Sg_CpppAlloc[pos0][1] = NULL;

			return 0;
		}

		// 最後の位置より前か後か
		for(alloc_size = 0; Sg_CpppAlloc[pos0][alloc_size] != NULL; alloc_size++);
		if(pos1 < alloc_size){
			return 0;
		}
		else if(pos1 > alloc_size){
			return 1;
		}

		ptr = (char**)realloc(Sg_CpppAlloc[pos0], sizeof(char*) * (pos1 + DF_OFFSET));
		if(ptr == NULL){
			CpppAllocExit();
			return 1;
		}
		Sg_CpppAlloc[pos0] = ptr;

		if(pos1 == alloc_size){
			Sg_CpppAlloc[pos0][pos1 + 1] = NULL;
		}

	return 0;
}

static short int CpppAlloc2(int pos0, int pos1, int len){

	char* ptr;

		if(Sg_CpppAlloc[pos0][pos1] == NULL){

			Sg_CpppAlloc[pos0][pos1] = (char*)malloc(sizeof(char) * (len + DF_OFFSET));
			if(Sg_CpppAlloc[pos0][pos1] == NULL){
				CpppAllocExit();
				return 1;
			}
			Sg_CpppAlloc[pos0][pos1][0] = '\0';

			return 0;
		}

		ptr = (char*)realloc(Sg_CpppAlloc[pos0][pos1], sizeof(char) * (len + DF_OFFSET));
		if(ptr == NULL){
			CpppAllocExit();
			return 1;
		}
		Sg_CpppAlloc[pos0][pos1] = ptr;

	return 0;
}

char*** CpppAllocAddPp(int pos, int add_pos, int len){

	int alloc_size;

		if(Sg_CpppAlloc == NULL){
			return NULL;
		}
		else if(pos < 0 || add_pos < 0 || len < 0){
			return Sg_CpppAlloc;
		}

		// サイズのチェック。ポインタのポインタの最後の位置はNULLを付けている
		for(alloc_size = 0; Sg_CpppAlloc[alloc_size] != NULL; alloc_size++);
		if(pos >= alloc_size){
			return Sg_CpppAlloc;
		}
		for(alloc_size = 0; Sg_CpppAlloc[pos][alloc_size] != NULL; alloc_size++);
		if(add_pos > alloc_size){
			return NULL;
		}

		// ポインタのポインタのサイズを+1
		if(CpppAlloc1(pos, alloc_size) < 0){
			return NULL;
		}

		// 指している場所をスライド
		while(alloc_size > add_pos){
			Sg_CpppAlloc[pos][alloc_size] = Sg_CpppAlloc[pos][alloc_size - 1];
			alloc_size = alloc_size - 1;
		}

		// add_posにメモリを
		Sg_CpppAlloc[pos][add_pos] = (char*)malloc(sizeof(char*) * (len + DF_OFFSET));
		if(Sg_CpppAlloc[pos][add_pos] == NULL){
			return NULL;
		}
		Sg_CpppAlloc[pos][add_pos][0] = '\0';

	return Sg_CpppAlloc;
}

char*** CpppAllocDelPp(int pos, int line){

	int alloc_size;
	int len;
	char* ptr;

		if(Sg_CpppAlloc == NULL){
			return NULL;
		}
		else if(pos < 0 || line < 0){
			return Sg_CpppAlloc;
		}

		// サイズのチェック。ポインタのポインタの最後の位置はNULLを付けている
		for(alloc_size = 0; Sg_CpppAlloc[alloc_size] != NULL; alloc_size++);
		if(pos >= alloc_size){
			return NULL;
		}
		for(alloc_size = 0; Sg_CpppAlloc[pos][alloc_size] != NULL; alloc_size++);
		if(line >= alloc_size){
			return NULL;
		}

		while(1){

			// 次が最後の位置なら、現在の位置を開放して最後の位置とする
			if(Sg_CpppAlloc[pos][line + 1] == NULL){
				free(Sg_CpppAlloc[pos][line]);
				Sg_CpppAlloc[pos][line] = NULL;
				break;
			}

			// 現在の位置に次の位置の長さ分のメモリを確保しコピー
			len = (int)strlen(Sg_CpppAlloc[pos][line + 1]);
			ptr = (char*)realloc(Sg_CpppAlloc[pos][line], sizeof(char*) * (len + 1));
			if(ptr == NULL){
				CpppAllocExit();
				break;
			}
			Sg_CpppAlloc[pos][line] = ptr;

			strncpy(Sg_CpppAlloc[pos][line], Sg_CpppAlloc[pos][line + 1], len);
			Sg_CpppAlloc[pos][line][len] = '\0';

			line = line + 1;
		}
		if(Sg_CpppAlloc == NULL){
			return NULL;
		}

	return Sg_CpppAlloc;
}

void CpppAllocExit(void){

	int pos0;
	int pos1;

		if(Sg_CpppAlloc != NULL){

			for(pos0 = 0; Sg_CpppAlloc[pos0] != NULL; pos0++){

				for(pos1 = 0; Sg_CpppAlloc[pos0][pos1] != NULL; pos1++){
					free(Sg_CpppAlloc[pos0][pos1]);
					Sg_CpppAlloc[pos0][pos1] = NULL;
				}

				free(Sg_CpppAlloc[pos0]);
				Sg_CpppAlloc[pos0] = NULL;
			}

			free(Sg_CpppAlloc);
			Sg_CpppAlloc = NULL;
		}

}

