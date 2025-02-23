#!/bin/sh

V_CUR_DIR=$(dirname "$(readlink -f "$0")");
cd "${V_CUR_DIR}";

# シンボリックリンクが切れている場合unlink
find /usr/local/bin/ -xtype l -exec unlink "{}" \;

for V_A in `find ${V_CUR_DIR}/ -type f -name "*.out" -print0 | xargs -0`
do

	# ディレクトリを除いたファイル名を取得
	V_TMP="${V_A##*/}";
	# 更に拡張子を除いたファイル名を取得
	V_FN="${V_TMP%.*}";

	# パスが通ったデレクトリに拡張子無しの小文字でリンク
	echo "ln -sf ${V_A} /usr/local/bin/$(printf ${V_FN} | tr [A-Z] [a-z]);";
	ln -sf "${V_A}" /usr/local/bin/"$(printf "${V_FN}" | tr [A-Z] [a-z])";

done;

echo "exit: ${0};";
exit;

