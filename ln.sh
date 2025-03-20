#!/bin/sh

V_CUR_DIR=$(dirname "$(readlink -f "$0")");
cd "${V_CUR_DIR}";

# シンボリックリンクが切れている場合unlink
find /usr/local/bin/ -xtype l -exec unlink "{}" \;

# ディレクトリを除いたファイル名を取得
V_TMP="${V_CUR_DIR##*/}";
# 更に拡張子を除いたファイル名を取得
V_FN="${V_TMP%.*}";

echo "ln -sf ${V_CUR_DIR}/${V_FN}.sh /usr/local/bin/"$(printf "${V_FN}" | tr [A-Z] [a-z])";";
ln -sf ${V_CUR_DIR}/${V_FN}.sh /usr/local/bin/"$(printf "${V_FN}" | tr [A-Z] [a-z])";

echo "exit: ${0};";
exit;

