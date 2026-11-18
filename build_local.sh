#!/usr/bin/env bash

BDIR=$HOME/idss_builds

cp -r gsoap-2.8 "$BDIR"

cd "$BDIR/gsoap-2.8" || exit
./configure
make
mkdir -p "$BDIR/gsoap-bin"
make install exec_prefix="$BDIR/gsoap-bin"

cp "$BDIR/gsoap-bin/bin/soapcpp2" "idss/bin/soapcpp2"
chmod +x "idss/bin/soapcpp2"
cp gsoap-2.8/gsoap/stdsoap2.h idss/include
cp gsoap-2.8/gsoap/stdsoap2.c idss/src/gsoap
cp gsoap-2.8/gsoap/src/soapcpp2.c idss/src/gsoap

mkdir -p "$BDIR/idss"
cmake -S idss -B "$BDIR/idss" -DCMAKE_BUILD_TYPE="Release" -D CMAKE_C_COMPILER=clang -D CMAKE_CXX_COMPILER=clang++;
cmake --build "$BDIR/idss" -v;
mkdir -p info.d/inodeinfo_sd;
mkdir -p info.d/query_results;
mkdir -p info.d/query_tmpfiles;
mkdir -p info.d/server;
mkdir -p info.d/systeminfo_sd;
cmake --install "$BDIR/idss" --install-prefix "$BDIR/idss-bin" -v;

# sqlite3 /usr/local/server/idss_db < idss/schema/create_schema_p2p.sql

# mkdir /pyenv;
# python3 -m venv /pyenv;

# export PATH="/pyenv/bin:$PATH"

# pip3 install faker
