// 数字の桁数を調べる
int NumbDgt(unsigned int Num){

	int dgt = 0;

		if(Num == 0){
			return 1;
		}

		while(Num > 0){
			Num = Num / 10;
			dgt = dgt + 1;
		}

	return dgt;
}

