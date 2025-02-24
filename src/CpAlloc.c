#include <stdlib.h>
#include <stdint.h>

#include "./CpAlloc.h"

// 一回のメモリ確保分
#define DF_CP_ALLOC 32
// \0の分
#define DF_OFFSET 1

static char* Sg_CpAlloc = NULL;

char* CpAlloc(size_t len){

	static size_t alloc_len = 0;
	static short int state = 0;

	char* ptr;

		// 終了時の関数を登録
		if(state == 0){
			atexit(CpAllocExit);
			state = 1;
		}

		if(len > SIZE_MAX - DF_CP_ALLOC - DF_OFFSET){
			CpAllocExit();
			return NULL;
		}
		else if(Sg_CpAlloc == NULL){

			alloc_len = len + DF_CP_ALLOC;
			Sg_CpAlloc = (char*)malloc(sizeof(char) * (alloc_len + DF_OFFSET));
			if(Sg_CpAlloc == NULL){
				return NULL;
			}

			Sg_CpAlloc[0] = '\0';

		}
		else if(len < alloc_len){
			return Sg_CpAlloc;
		}
		else{

			alloc_len = len + DF_CP_ALLOC;
			ptr = (char*)realloc(Sg_CpAlloc, sizeof(char) * (alloc_len + DF_OFFSET));
			if(ptr == NULL){
				CpAllocExit();
				return NULL;
			}
			Sg_CpAlloc = ptr;

		}

	return Sg_CpAlloc;
}

void CpAllocExit(void){

		if(Sg_CpAlloc != NULL){
			free(Sg_CpAlloc);
			Sg_CpAlloc = NULL;
		}

}

