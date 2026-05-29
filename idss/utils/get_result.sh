#!/usr/bin/env bash

CPWD=`pwd`
cd /usr/local/bin;
./idss_get_file_client -h 127.0.0.1 -q $1;
cd $CPWD;
