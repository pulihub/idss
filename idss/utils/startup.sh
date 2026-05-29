#!/usr/bin/env bash

sed -i "s/<Hostname>.*<\/Hostname>/<Hostname>$HOSTNAME<\/Hostname>/g" /etc/idss_conf.xml

python /datagen.py $IDSS_NR;

P2P_PORT=`expr $IDSS_PORT + 1`

cd /usr/local/server;

if [[ ! -v IDSS_BOOTSTRAP_IP ]]; then
    /usr/local/server/idss -p $IDSS_PORT  \
    -a `hostname -I` \
    -f /etc/idss_conf.xml \
    -d 2 \
    -e /usr/local/server/stderr \
    -l /usr/local/server/log \
    -o  $P2P_PORT \
    -r 0.75;
    echo "Launched idss bootstrap node";
else
    /usr/local/server/idss -p $IDSS_PORT  \
    -a `hostname -I` \
    -f /etc/idss_conf.xml \
    -d 2 \
    -e /usr/local/server/stderr \
    -l /usr/local/server/log \
    -o $P2P_PORT \
    -r 0.75 \
    -j $IDSS_BOOTSTRAP_IP:$P2P_PORT;
    echo "Launched idss node";
fi

hostname;
hostname -I;
tail -f /dev/null;
