#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>

#include "./CurDir.h"

thread_local static char* Sg_CurDir = NULL;
thread_local static unsigned short int Sg_CurDirState = 0;

char* CurDir(int argc, char** argv){

	size_t pos;

		if(Sg_CurDirState == 0){
			atexit(CurDirExit);
			Sg_CurDirState = 1;
		}

		// argvが空か長さが0
		if(argv[0] == NULL || argv[0][0] == '\0'){

			if(CurDirAlloc(3) != 0){
				return NULL;
			}

			sprintf(Sg_CurDir, "./");
			return Sg_CurDir;
		}

		pos = strlen(argv[0]) - 1;
		while (1){

			if(pos < 1){
				break;
			}
			else if(argv[0][pos] == '/' || argv[0][pos] == '\\'){
				pos = pos + 1;
				break;
			}

			pos = pos - 1;

		};

		// 長さが0だった場合./と\0を入れれるようにメモリを+3確保
		if(CurDirAlloc(pos + 3) != 0){
			return NULL;
		}

		if(pos < 1){
			sprintf(Sg_CurDir, "./");
		}
		else {
			strncpy(Sg_CurDir, argv[0], pos);
			Sg_CurDir[pos] = '\0';
		}

	return Sg_CurDir;
}

static int CurDirAlloc(size_t len){

	char* ptr;

		if(Sg_CurDir == NULL){

			Sg_CurDir = (char*)malloc(sizeof(char) * len);
			if(Sg_CurDir == NULL){
				return -1;
			}

		}
		else {

			ptr = (char*)realloc(Sg_CurDir, sizeof(char) * len);
			if(ptr == NULL){
				return -1;
			}
			Sg_CurDir = ptr;

		}

	return 0;
}

void CurDirExit(void){

		if(Sg_CurDir != NULL){
			free(Sg_CurDir);
			Sg_CurDir = NULL;
		}

}

