#!/usr/bin/env bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BDIR=$HOME/idss_builds

mkdir -p "$BDIR/gsoap-2.8"
cp -r "$SCRIPT_DIR"/gsoap-2.8 "$BDIR"

mkdir -p "$BDIR/idss"
cp -r "$SCRIPT_DIR"/idss "$BDIR"

cd "$BDIR/gsoap-2.8" || exit
./configure
make
mkdir -p "$BDIR/gsoap_bin"
sudo make install exec_prefix="$BDIR/gsoap_bin"
sudo chown -R $USER:$USER "$BDIR/gsoap_bin"

mkdir -p "$BDIR/idss/bin"
cp "$BDIR/gsoap_bin/bin/soapcpp2" "$BDIR/idss/bin/soapcpp2"
chmod +x "$BDIR/idss/bin/soapcpp2"
cp "$BDIR/gsoap-2.8/gsoap/stdsoap2.h" "$BDIR/idss/include"
cp "$BDIR/gsoap-2.8/gsoap/stdsoap2.c" "$BDIR/idss/src/gsoap"
cp "$BDIR/gsoap-2.8/gsoap/src/soapcpp2.c" "$BDIR/idss/src/gsoap"

cd "$BDIR"
mkdir -p "$BDIR/idss/build"
cmake -S idss -B "$BDIR/idss/build" --install-prefix "$BDIR/idss-bin" -DCMAKE_BUILD_TYPE="Release" -D CMAKE_C_COMPILER=clang -D CMAKE_CXX_COMPILER=clang++;
cmake --build "$BDIR/idss/build" -v;
mkdir -p "$BDIR"/idss/info.d/inodeinfo_sd;
mkdir -p "$BDIR"/idss/info.d/query_results;
mkdir -p "$BDIR"/idss/info.d/query_tmpfiles;
mkdir -p "$BDIR"/idss/info.d/server;
mkdir -p "$BDIR"/idss/info.d/systeminfo_sd;
cmake --install "$BDIR/idss/build" -v;

sqlite3 "$BDIR"/idss-bin/server/idss_db < "$BDIR"/idss/schema/create_schema_p2p.sql

mkdir "$BDIR"/pyenv;
python3 -m venv "$BDIR"/pyenv;

export PATH="$BDIR/pyenv/bin:$PATH"

pip3 install faker
