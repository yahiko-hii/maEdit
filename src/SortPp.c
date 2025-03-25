#include <stdlib.h>
#include <string.h>

#include "./SortPp.h"

char** SortPp(char** ls, int start){

		if(ls == NULL){
			return NULL;
		}

	return SortPpInsertion(ls, start);
}


char** SortPpInsertion(char** ls, int start){

	int i;
	int j;

		if(ls == NULL){
			return NULL;
		}

		for(i = 0; ls[i] != NULL; i++);
		i = i - 1;
		if(i < start){
			return NULL;
		}

		for(i = start + 1; ls[i] != NULL; i++){

			for(j = start; j <= i; j++){

				if(strcmp(ls[j], ls[i]) > 0){
					ls = SortPpInsertionPr(ls, j, i);
				}

			}

		}

	return ls;
}

// pointer reassignment
static char** SortPpInsertionPr(char** ls, int pos_j, int pos_i){

	char* ptr;
	int i;

		ptr = ls[pos_i];

		for(i = pos_i; i > pos_j; i--){
			ls[i] = ls[i - 1];
		}

		ls[pos_j] = ptr;

	return ls;
}

