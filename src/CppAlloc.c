#include <stdlib.h>
#include <limits.h>
#include <threads.h>

#include "./CppAlloc.h"

#define DF_OFFSET 2

thread_local static char** Sg_CppAlloc = NULL;

char** CppAlloc(int pos, int len){

	static short int state = 0;

		// 終了時の関数を登録
		if(state == 0){
			atexit(CppAllocExit);
			state = 1;
		}

		if(pos < 0 || pos > INT_MAX - DF_OFFSET){
			return NULL;
		}
		else if(len < 0 || len > INT_MAX - DF_OFFSET){
			return NULL;
		}

		if(CppAlloc0(pos) != 0){
			return NULL;
		}
		else if(CppAlloc1(pos, len) != 0){
			return NULL;
		}

	return Sg_CppAlloc;
}

static short int CppAlloc0(int pos){

	int alloc_size;
	char** ptr;

		if(Sg_CppAlloc == NULL){

			// Sg_CppAllocがNULLの場合でメモリを確保する位置が0じゃない場合
			if(pos != 0){
				return 1;
			}

			Sg_CppAlloc = (char**)malloc(sizeof(char*) * (pos + DF_OFFSET));
			if(Sg_CppAlloc == NULL){
				return 1;
			}
			Sg_CppAlloc[0] = NULL;
			Sg_CppAlloc[1] = NULL;

			return 0;
		}

		// 最後の位置より前か後か
		for(alloc_size = 0; Sg_CppAlloc[alloc_size] != NULL; alloc_size++);
		if(pos < alloc_size){
			return 0;
		}
		else if(pos > alloc_size){
			return 1;
		}

		ptr = (char**)realloc(Sg_CppAlloc, sizeof(char*) * (pos + DF_OFFSET));
		if(ptr == NULL){
			CppAllocExit();
			return 1;
		}
		Sg_CppAlloc = ptr;

		if(pos == alloc_size){
			Sg_CppAlloc[pos + 1] = NULL;
		}

	return 0;
}

static short int CppAlloc1(int pos, int len){

	char* ptr;

		if(Sg_CppAlloc[pos] == NULL){

			Sg_CppAlloc[pos] = (char*)malloc(sizeof(char) * (len + DF_OFFSET));
			if(Sg_CppAlloc[pos] == NULL){
				CppAllocExit();
				return 1;
			}
			Sg_CppAlloc[pos][0] = '\0';

			return 0;
		}

		ptr = (char*)realloc(Sg_CppAlloc[pos], sizeof(char) * (len + DF_OFFSET));
		if(ptr == NULL){
			CppAllocExit();
			return 1;
		}
		Sg_CppAlloc[pos] = ptr;

	return 0;
}

void CppAllocExit(void){

	int pos;

		if(Sg_CppAlloc != NULL){

			for(pos = 0; Sg_CppAlloc[pos] != NULL; pos++){
				free(Sg_CppAlloc[pos]);
				Sg_CppAlloc[pos] = NULL;
			}

			free(Sg_CppAlloc);
			Sg_CppAlloc = NULL;

		}

}

