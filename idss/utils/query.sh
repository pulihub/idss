#!/usr/bin/env bash
CPWD=`pwd`
cd /usr/local/bin;
./idss_test_client -h $BOOTSTRAP_NODE_IP -p $IDSS_PORT -v -t $1 -q "SELECT * FROM tb_user;";
cd $CPWD;
