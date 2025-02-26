#!/bin/sh

V_CUR_DIR=$(dirname "$(readlink -f "$0")");
cd "${V_CUR_DIR}";

# ソースディレクトリ
if [ -d "${V_CUR_DIR}/src/" ]
then
	V_SRC_DIR="${V_CUR_DIR}/src";
else
	V_SRC_DIR="${V_CUR_DIR}";
fi;

# 最適化
V_OPT="-Ofast";

ls ${V_SRC_DIR}/*.c > /dev/null 2>&1;
if [ $? -eq 0 ];
then

	echo '*.c';
	V_SRC_EXT='.c';

	if command -v clang > /dev/null 2>&1;
	then
		V_COMP='clang';
		V_COMP_LIB='-std=c23';
		V_FLAGS="${V_COMP_LIB} -Weverything -Wno-padded -Werror ${V_OPT}";
	elif command -v gcc > /dev/null 2>&1;
	then
		V_COMP='gcc';
		V_COMP_LIB='-std=c2x';
		V_FLAGS="${V_COMP_LIB} -Wextra -Werror ${V_OPT}";
	fi;

fi;

ls ${V_SRC_DIR}/*.cpp > /dev/null 2>&1;
if [ $? -eq 0 ];
then

	echo '*.cpp';
	V_SRC_EXT='.cpp';

	if command -v clang++ > /dev/null 2>&1;
	then
		V_COMP='clang++';
		V_COMP_LIB='-std=c23';
		V_FLAGS="${V_COMP_LIB} -Weverything -Wno-padded -Werror ${V_OPT}";
	elif command -v g++ > /dev/null 2>&1;
	then
		V_COMP='g++';
		V_COMP_LIB='-std=c2x';
		V_FLAGS="${V_COMP_LIB} -Wextra -Werror ${V_OPT}";
	fi;

fi;

if [ -z "${V_COMP}" ]
then
	echo 'exit';
	exit;
fi;

# 作成先ディレクトリ
V_OP_DIR="${V_CUR_DIR}/";

# 現在のディレクトリの最後の部分を出力時のファイル名とする
V_OP_FN="${V_CUR_DIR##*/}";

# extension 出力する拡張子
V_OP_ETS='.out';

# アセンブリコードで出力するかのフラグ -asmを付けてこのスクリプトを実行する
V_OP_ASM='0';

mkdir -p "${V_CUR_DIR}/tmp/o";

####
# チェック
####
# ライブラリが使われてるかチェックする為にソースを一つにまとめる
mkdir -p /tmp/$$/;
cat "${V_SRC_DIR}/"*"${V_SRC_EXT}" > /tmp/$$/srcall.txt;

# 引数2の.hが存在したら変更
FuncCheck(){

	grep -i "${1}" /tmp/$$/srcall.txt > /tmp/$$/grep.txt;
	if [ -s /tmp/$$/grep.txt ]
	then
		V_COMP="${2}";
		V_SRC_EXT="${3}";
		V_FLAGS="${4}";
		V_OP_ETS="${5}";
		echo "${1}";
	fi;

	return 0;
};

# ncurses?
FuncCheck 'ncurses.h' "${V_COMP}" "${V_SRC_EXT}" \
"${V_FLAGS} -lncursesw" \
"${V_OP_ETS}";

rm -r -f /tmp/$$*;

# 引数をチェック
for V_A in $@
do

	# windows用のコンパイル?
	if [ "${V_A}" = '-windows' ]
	then

		if command -v x86_64-w64-mingw32-g++ > /dev/null 2>&1;
		then
			echo 'windows';
			V_COMP='x86_64-w64-mingw32-g++';
			V_FLAGS='-lgdi32';
			V_OP_ETS='.exe';
		fi;

	# アセンブリコードの出力?
	elif [ "${V_A}" = '-asm' ]
	then
		V_OP_ASM='1';
		mkdir -p "${V_CUR_DIR}/tmp/s";
	fi;

done;

echo "\n----\ncompile\n";
for V_A in `find ${V_CUR_DIR}/ -type f -name "*${V_SRC_EXT}"`
do

	# ディレクトリを除いたファイル名を取得
	V_TMP="${V_A##*/}";
	# 更に拡張子を除いたファイル名を取得
	V_FN="${V_TMP%.*}";

	if [ "${V_OP_ASM}" = '0' ]
	then

		# .oファイルが存在しないもしくはタイムスタンプが違う
		if [ ! -e ${V_CUR_DIR}/tmp/o/${V_FN}.o ] ||
			[ ${V_A} -nt ${V_CUR_DIR}/tmp/o/${V_FN}.o ] ||
			[ ${V_SRC_DIR}/${V_FN}.h -nt ${V_CUR_DIR}/tmp/o/${V_FN}.o ]
		then

			echo "${V_CUR_DIR}/tmp/o/${V_FN}.o";
			${V_COMP} -c "${V_A}" -o "${V_CUR_DIR}/tmp/o/${V_FN}.o";

			touch ${V_A} ${V_SRC_DIR}/${V_FN}.h "${V_CUR_DIR}/tmp/o/${V_FN}.o";

		fi;

	else
		echo "${V_CUR_DIR}/tmp/s/${V_FN}.s";
		${V_COMP} -S "${V_A}" -o "${V_CUR_DIR}/tmp/s/${V_FN}.s";
	fi;

done;

if [ ${V_OP_ASM} = '0' ]
then
	echo "\n----\nlink\n";
	echo "${V_COMP} ${V_CUR_DIR}/tmp/o/*.o -o ${V_OP_DIR}/${V_OP_FN}${V_OP_ETS} ${V_FLAGS}";
	${V_COMP} "${V_CUR_DIR}/tmp/o/"*".o" -o "${V_OP_DIR}/${V_OP_FN}${V_OP_ETS}" ${V_FLAGS};
fi;

#####
# 後処理
####
if [ -f "${V_OP_DIR}/${V_OP_FN}${V_OP_ETS}" ]
then
	echo "\n----\npost-processing\n";
	# パーミッション変更
	echo "chmod -R 550 \"${V_OP_DIR}/${V_OP_FN}${V_OP_ETS}\"";
	chmod -R 550 "${V_OP_DIR}/${V_OP_FN}${V_OP_ETS}";
fi;

echo "\n----\n";
echo "exit: ${0};";
exit;

# ヘッダ ファイルの場所
# /usr/include/
