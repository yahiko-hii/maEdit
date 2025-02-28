#ifndef IG_ST
#define IG_ST

struct ST_POS{
	int FileNum;

	int DatY;
	int DatX;

	int MaxY;
	int MaxX;
};
struct ST_C{
	char* Cur;
	char* Eol;
	char* Dm;
	char* Gs;
};
struct ST_KEY{
	int CurLen;
	int EolLen;
};
struct ST_FILE{
	char*** Dat;
};
struct ST_DAT{
	int IntDig;
	char* Cd;
};
struct ST{
	struct ST_POS Pos;
	struct ST_C C;
	struct ST_KEY Key;
	struct ST_FILE File;
	struct ST_DAT Dat;
};
typedef struct ST St_t;

#endif

