# syntax=docker/dockerfile:1
FROM ubuntu:noble AS build

# install app dependencies
RUN apt-get update && apt-get install -y build-essential \
    cmake libxml2-dev libssl-dev libsqlite3-dev zlib1g-dev \
    lzma-dev libdb-dev libnsl-dev sqlite3 clang nano python3.12-venv unzip autoconf yacc flex libbison-dev

# compile gsoap and integrate gsoap files in IDSS src code
COPY gsoap-2.8 /gsoap-2.8

WORKDIR /gsoap-2.8
RUN <<EOF
./configure
make
make install exec_prefix=/gsoap_bin
EOF

COPY idss /idss

RUN <<EOF
mkdir -p /idss/bin && cp /gsoap_bin/bin/soapcpp2 /idss/bin/soapcpp2
chmod +x /idss/bin/soapcpp2
cp /gsoap-2.8/gsoap/stdsoap2.h /idss/include
cp /gsoap-2.8/gsoap/stdsoap2.c /idss/src/gsoap
cp /gsoap-2.8/gsoap/src/soapcpp2.c /idss/src/gsoap
EOF

# compile and install IDSS
WORKDIR /idss
RUN <<EOF
cmake -S /idss -B /idss/build -DCMAKE_BUILD_TYPE="Release" -D CMAKE_C_COMPILER=clang -D CMAKE_CXX_COMPILER=clang++;
cmake --build build -v;
mkdir -p info.d/inodeinfo_sd;
mkdir -p info.d/query_results;
mkdir -p info.d/query_tmpfiles;
mkdir -p info.d/server;
mkdir -p info.d/systeminfo_sd;
cmake --install build -v;
sqlite3 /usr/local/server/idss_db < /idss/schema/create_schema_p2p.sql
EOF

FROM ubuntu:noble AS idss
RUN apt-get update && apt-get install -y libxml2 libnsl2 libsqlite3-0 python3 python3.12-venv

# transfer install files from build image
COPY --from=build /usr/local/server /usr/local/server
COPY --from=build /usr/local/bin /usr/local/bin
COPY --from=build /usr/local/info.d /usr/local/info.d
COPY --from=build /usr/local/lib /usr/local/lib
COPY --from=build /etc/idss_conf.xml /etc/idss_conf.xml
COPY --from=build /usr/local/server/idss_db /usr/local/server/idss_db
COPY --from=build /idss/utils/startup.sh /
COPY --from=build /idss/utils/datagen.py /

ENV LD_LIBRARY_PATH="/usr/local/lib:$LD_LIBRARY_PATH"
ENV IDSS_PORT=19000
ENV IDSS_NR=10

RUN <<EOF
mkdir /pyenv;
python3 -m venv /pyenv;
EOF

ENV PATH="/pyenv/bin:$PATH"

RUN pip3 install faker

ENTRYPOINT [ "/startup.sh" ]
