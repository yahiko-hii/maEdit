#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <threads.h>

#include "./Reedfile.h"

#define DE_REEDFILE_OFFSET 2

thread_local static char* Sg_Reedfile = NULL;
// 一回で確保するメモリ量
thread_local static int Sg_ReedfileAlloc = 1024;

char* Reedfile(char* fn, int max){

	static short int state = 0;

	FILE* fp;
	char c[1];
	int len;

		if(fn == NULL || max < 1){
			return NULL;
		}
		else if(max > INT_MAX - DE_REEDFILE_OFFSET){
			max = INT_MAX - DE_REEDFILE_OFFSET;
		}

		// 終了時の関数を登録
		if(state == 0){
			atexit(ReedfileExit);
			state = 1;
		}

		fp = fopen(fn, "rb");
		if(fp == NULL){
			return NULL;
		}

		if(ReedfileAlloc(1) != 0){
			fclose(fp);
			return NULL;
		}

		for(len = 0; fread(c, sizeof(char), 1, fp) == 1; len++){

			if(len >= max){
				break;
			}

			//  \nだった場合のために改行と\0を入れれるように余分にメモリを確保
			if(ReedfileAlloc(len + DE_REEDFILE_OFFSET) != 0){
				break;
			}

			// \nだった場合
			if(c[0] == '\n'){

				// 次がなければ抜ける
				if(fread(c, sizeof(char), 1, fp) != 1){
					break;
				}

				// \nと次に取得した値を入れる
				Sg_Reedfile[len] = '\n';
				len = len + 1;
				Sg_Reedfile[len] = c[0];

			}
			else{
				Sg_Reedfile[len] = c[0];
			}

		}

		fclose(fp);

		if(Sg_Reedfile == NULL){
			return NULL;
		}

		Sg_Reedfile[len] = '\0';

	return Sg_Reedfile;
}

static int ReedfileAlloc(int len){

	static int allocsize;
	char* ptr;

	// メモリの確保
	if(Sg_Reedfile == NULL){

		allocsize = len + Sg_ReedfileAlloc;
		Sg_Reedfile =(char*)malloc(sizeof(char) * allocsize);
		if(Sg_Reedfile == NULL){
			return -1;
		}

	}
	// 必要なサイズの再確保
	else if(len >= allocsize){

		if(allocsize >= INT_MAX){
			ReedfileExit();
			return -1;
		}
		else if(INT_MAX / 2 > Sg_ReedfileAlloc){
			Sg_ReedfileAlloc = Sg_ReedfileAlloc * 2;
		}

		if(INT_MAX - len - Sg_ReedfileAlloc < 1){
			allocsize = INT_MAX;
		}
		else{
			allocsize = len + Sg_ReedfileAlloc;
		}

		ptr =(char*)realloc(Sg_Reedfile, sizeof(char) * allocsize);
		if(ptr == NULL){
			ReedfileExit();
			return -1;
		}
		Sg_Reedfile = ptr;

	}

	return 0;
}

void ReedfileExit(void){

		if(Sg_Reedfile != NULL){
			free(Sg_Reedfile);
			Sg_Reedfile = NULL;
		}

}

