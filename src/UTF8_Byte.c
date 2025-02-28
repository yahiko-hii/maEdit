#include <stdio.h>

#include "./UTF8_Byte.h"

short int UTF8_Byte(char* str, int pos){

	short int r;

		if(pos < 0){
			return 0;
		}

		r = UTF8_Byte0(str, pos);
		if(r > 0){
			return r;
		}
		r = UTF8_Byte194(str, pos);
		if(r > 0){
			return r;
		}
		r = UTF8_Byte224(str, pos);
		if(r > 0){
			return r;
		}
		r = UTF8_Byte225(str, pos);
		if(r > 0){
			return r;
		}
		r = UTF8_Byte237(str, pos);
		if(r > 0){
			return r;
		}
		r = UTF8_Byte238(str, pos);
		if(r > 0){
			return r;
		}
		r = UTF8_Byte240(str, pos);
		if(r > 0){
			return r;
		}
		r = UTF8_Byte241(str, pos);
		if(r > 0){
			return r;
		}
		r = UTF8_Byte244(str, pos);
		if(r > 0){
			return r;
		}

	return 0;
}

// 1バイト
static short int UTF8_Byte0(char* str, int pos){

		if((unsigned char)str[pos] != 0 && (unsigned char)str[pos] <= 127){
			return 1;
		}

	return 0;
}

// 2バイト
static short int UTF8_Byte194(char* str, int pos){

		if((unsigned char)str[pos] >= 194 && (unsigned char)str[pos] <= 223){

			if((unsigned char)str[pos + 1] >= 128 && (unsigned char)str[pos + 1] <= 191){
				return 2;
			}

		}

	return 0;
}

// 3バイト
static short int UTF8_Byte224(char* str, int pos){

		if((unsigned char)str[pos] == 224){

			if((unsigned char)str[pos + 1] >= 160 && (unsigned char)str[pos + 1] <= 191){

				if((unsigned char)str[pos + 2] >= 128 && (unsigned char)str[pos + 2] <= 191){
					return 3;
				}

			}

		}

	return 0;
}

static short int UTF8_Byte225(char* str, int pos){

		if((unsigned char)str[pos] >= 225 && (unsigned char)str[pos] <= 236){

			if((unsigned char)str[pos + 1] >= 128 && (unsigned char)str[pos + 1] <= 191){

				if((unsigned char)str[pos + 2] >= 128 && (unsigned char)str[pos + 2] <= 191){
					return 3;
				}

			}

		}

	return 0;
}

static short int UTF8_Byte237(char* str, int pos){

		if((unsigned char)str[pos] == 237){

			if((unsigned char)str[pos + 1] >= 128 && (unsigned char)str[pos + 1] <= 159){

				if((unsigned char)str[pos + 2] >= 128 && (unsigned char)str[pos + 2] <= 191){
					return 3;
				}

			}

		}

	return 0;
}

static short int UTF8_Byte238(char* str, int pos){

		if((unsigned char)str[pos] >= 238 && (unsigned char)str[pos] <= 239){

			if((unsigned char)str[pos + 1] >= 128 && (unsigned char)str[pos + 1] <= 191){

				if((unsigned char)str[pos + 2] >= 128 && (unsigned char)str[pos + 2] <= 191){
					return 3;
				}

			}

		}

	return 0;
}

// 4バイト
static short int UTF8_Byte240(char* str, int pos){

		if((unsigned char)str[pos] == 240){

			if((unsigned char)str[pos + 1] >= 144 && (unsigned char)str[pos + 1] <= 191){

				if((unsigned char)str[pos + 2] >= 128 && (unsigned char)str[pos + 2] <= 191){

					if((unsigned char)str[pos + 3] >= 128 && (unsigned char)str[pos + 3] <= 191){
						return 4;
					}

				}

			}

		}

	return 0;
}

static short int UTF8_Byte241(char* str, int pos){

		if((unsigned char)str[pos] >= 241 && (unsigned char)str[pos] <= 243){

			if((unsigned char)str[pos + 1] >= 128 && (unsigned char)str[pos + 1] <= 191){

				if((unsigned char)str[pos + 2] >= 128 && (unsigned char)str[pos + 2] <= 191){

					if((unsigned char)str[pos + 3] >= 128 && (unsigned char)str[pos + 3] <= 191){
						return 4;
					}

				}

			}

		}

	return 0;
}

static short int UTF8_Byte244(char* str, int pos){

		if((unsigned char)str[pos] == 244){

			if((unsigned char)str[pos + 1] >= 128 && (unsigned char)str[pos + 1] <= 143){

				if((unsigned char)str[pos + 2] >= 128 && (unsigned char)str[pos + 2] <= 191){

					if((unsigned char)str[pos + 3] >= 128 && (unsigned char)str[pos + 3] <= 191){
						return 4;
					}

				}

			}

		}

	return 0;
}

// 配列の位置を移動し多バイト文字の先頭の位置を探す
int UTF8_ByteFl(char* str, int pos, short int v){

		if(pos < 1){
			return 0;
		}

		while(1){

			if((unsigned char)str[pos] <= 127){
				break;
			}
			else if((unsigned char)str[pos] >= 194 && (unsigned char)str[pos] <= 223){
				break;
			}
			else if((unsigned char)str[pos] == 224){
				break;
			}
			else if((unsigned char)str[pos] >= 225 && (unsigned char)str[pos] <= 236){
				break;
			}
			else if((unsigned char)str[pos] == 237){
				break;
			}
			else if((unsigned char)str[pos] >= 238 && (unsigned char)str[pos] <= 239){
				break;
			}
			else if((unsigned char)str[pos] == 240){
				break;
			}
			else if((unsigned char)str[pos] >= 241 && (unsigned char)str[pos] <= 243){
				break;
			}
			else if((unsigned char)str[pos] == 244){
				break;
			}

			// マイナス方向、0まで
			if(v == -1){

				if(pos < 1){
					break;
				}

				pos = pos - 1;

			}
			// プラス方向
			else if(v == 1){
				pos = pos + 1;
			}
			// フラグが不明
			else{
				break;
			}

		}

	return pos;
}

