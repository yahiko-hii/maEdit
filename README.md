----

This program uses the ncurses library, which is licensed under the X11 License.

Copyright 2025 yahiko hi

The ncurses library is provided under the X11 License (MIT License). For more details, see the LICENSE file in this distribution.

----

maEdit
TUI(テキストユーザインタフェース)のエディタです

----

# compiler

# Debian
apt install -y gcc g++ clang libncurses-dev;

# Red Hat
yum -y install gcc;
yum -y install ncurses-devel;

# compile
sh ./comp.sh

./comp.shのディレクトリに.outファイルを作成されます

----

# rm

# シンボリックリンクを作成していたら削除
unlink /usr/local/bin/maedit

# ディレクトリを削除
rm -r -f ./maEdit

----

メール
contact.em@mainaddr.com

----

2025/02/03
公開

2025/02/04
Ctrl + K 等で文字入力している最中も Ctrl + X で一文字削除できるように変更

2025/02/05
処理に対するキーを変更(Ctrl + K ではなくKだけで文字列の入力開始)
細かい修正

2025/02/07
size_tとintみたいな異なる符号の比較となるような部分の修正

2025/02/09
行頭で文字を削除した際、ライン移動のメモリの扱いを変更

2025/02/11
[ と ] のキー入力でもファイルの番号を切り替えられるように変更

2025/02/12
MainInitの変数rが未定義の状態で比較していたので修正

2025/02/13
置き換えを追加

2025/02/14
出力の時、ファイル名を変更出来るように変更

2025/02/15
細かい変更

2025/02/19
カーソルと改行の文字を変更 / 出力するよう変更

2025/02/20
細かい変更

2025/02/20
ファイルの読み込みの際のメモリの確保方法を少し変更

2025/02/22
出力範囲を変更

2025/02/22
出力範囲を変更

2025/02/24
ncursesのライセンスに合わせて変更

2025/02/26
細かい変更

2025/03/03
引数で渡された中に読み込めないファイル名があった場合、それ以降もファイルも読み込めなかったので修正

----
