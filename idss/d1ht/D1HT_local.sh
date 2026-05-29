#!/bin/bash
# This file is part of D1HT. Please see its license terms in D1HT.license.txt
#
# This script runs an experiment with all peers in the local machine, and generates its statistics and a summary of errors
# 
# This script takes at least 4 parameters: Routing table size, number of peers, Average session length and duration of the measuring phase
# All other parameters are passed to the D1HT peers
#

killall D1HT D1HWrap 2>/dev/null >/dev/null

export DIR=/tmp/benchmarks/D1HT-$2peers.Savg$3-`date +%F-%H:%M`

export PATH=$PWD:$PATH

export BDIR=$PWD

rm -fr $DIR 2>/dev/null >/dev/null

mkdir -p $DIR

if [ $? -ne 0 ] 
then
   echo 
   echo ERROR Cause: Unable to create directory $DIR
   exit -1
fi

cd $DIR

export D1HTSYSID=`date +%N`

echo
rm -f benchdata.txt 2>/dev/null
date | tee -a benchdata.txt
echo Running command $0 $@ | tee -a benchdata.txt
echo bench dir is $DIR | tee -a benchdata.txt
echo Routing table size is $1 | tee -a benchdata.txt
echo Number of peers is $2 | tee -a benchdata.txt
echo Average session length is $3 seconds | tee -a benchdata.txt
echo Mesuring phase duration is $4 seconds | tee -a benchdata.txt

if [ "$#" -gt "4" ]
then
   echo D1HT parameters are $5 $6 $7 $8 $9 | tee -a benchdata.txt
else
   if [ "$#" -lt "4" ]
   then
     echo
     echo ERROR Cause: There should be at least 4 parameters: Routing table size, number of peers, average session length in minutes, and duration of the measuring phase in seconds
     exit -1
   fi
   echo There will be no D1HT parameters | tee -a benchdata.txt
fi
echo

if [ "$1" -le "$2" ]
then
     echo
     echo ERROR Cause: Routing table size $1 should be bigger than the number of peers $2
     exit -1
fi
let FIRSTPHASE=$2

if [ "$FIRSTPHASE" -lt "30" ]
then
    export FIRSTPHASE=30    
fi

export MEASPHASE=$4

if [ "$MEASPHASE" -lt "30" ]
then
   echo
   echo ERROR Cause: The measuring phase duration must be at least 30 seconds
   exit -1
fi

killall D1HT D1HWrap 2>/dev/null >/dev/null

echo `date`: Benchmark $DIR - building binaries - running make all

cd $BDIR

make all > /dev/null

if [ $? -ne 0 ] 
then
   echo 
   echo ERROR Cause: make error
   exit -1
fi

cd $DIR

killall -9 D1HT D1HWrap 2>/dev/null >/dev/null

export PORTNO=5000

D1HT -N $1 -a 127.0.0.1:$PORTNO -v -o $5 $6 $7 $8 $9 &

export FIRSTPID=$!

echo "phase0.duration=$FIRSTPHASE" > $DIR/phases.in
echo "phase0.lookup_rate=0" >> $DIR/phases.in
echo "phase0.Savg=0" >> $DIR/phases.in

echo "phase1.duration=$MEASPHASE" >> $DIR/phases.in
echo "phase1.lookup_rate=1" >> $DIR/phases.in
echo "phase1.Savg=$3" >> $DIR/phases.in

let EVENTSEXPECTED=2*$2*$MEASPHASE/$3

echo `date`: Benchmark $DIR - creating peers

export PORTNO1=$PORTNO
let lastport=$PORTNO1+9

for i in $(seq 2 $2)
do
    let PORTNO1=$PORTNO1+3
    let nextport=$PORTNO1+3
    D1HWrap $DIR/phases.in D1HT -o -v -A 127.0.0.1:$lastport -A 127.0.0.1:$PORTNO -A 127.0.0.1:$lastport -A 127.0.0.1:$nextport -p $PORTNO1 $5 $6 $7 $8 $9 &
    export lastport=$PORTNO1
done

sleep 30

let TIMETOT=$MEASPHASE+$FIRSTPHASE+120+300

echo `date`: Benchmark $DIR - should finish in about $TIMETOT seconds
echo `date`: Benchmark $DIR - starting first benchmark phase 

killall -SIGHUP D1HWrap

sleep 20

killall -SIGHUP D1HWrap

sleep $FIRSTPHASE

echo `date`: Benchmark $DIR - Running benchmark measuring phase 

sleep 20

kill $FIRSTPID 2>/dev/null >/dev/null

sleep 40

kill -9 $FIRSTPID 2>/dev/null >/dev/null

echo `date`: Benchmark $DIR - waiting for tasks to finish

wait

echo `date`: Benchmark $DIR - all tasks finished

echo `date`: Benchmark $DIR - verifying errors

killall D1HT D1HWrap 2>/dev/null >/dev/null

sleep 10

D1HT_xeq.sh | tee $DIR/xeq.txt 2>&1

D1HT_GetStats.sh

killall -9 D1HT D1HWrap 2>/dev/null >/dev/null

if [ ! -f Totstat.dump.stats.txt ]
then
   echo
   echo ERROR Cause: Problem gathering stats
   exit -1
fi

export EVENTSDETECTED=`grep "phase1.events_detected" Totstat.dump.stats.txt  | awk -F = '{print $2}' | awk -F . '{print $1}' | awk '{print $1}' `
echo EVENTS EXPECTED=$EVENTSEXPECTED DETECTED=$EVENTSDETECTED

export SAVG=`grep "phase1.Savg" Totstat.dump.stats.txt  | wc -l `

if [ "$SAVG" -eq "0" ] 
then
   echo
   echo WARNING Cause: Statistics about the Average Session Length were not generated
   exit -1
fi

export SAVG=`grep "phase1.Savg" Totstat.dump.stats.txt  | awk -F = '{print $2}' | awk -F . '{print $1}' | awk '{print $1}' `
echo Measured Savg=$SAVG

if [ "$EVENTSDETECTED" -gt "$EVENTSEXPECTED" ] 
then
     echo
     echo !!!! ATTENTION !!!! Your intended Savg was $3 but the measured Savg was $SAVG
     echo !!!! ATTENTION !!!! If the difference between the intended and the measured Savgs is above 10%, you should discard this experiment
     echo !!!! ATTENTION !!!! If you have doubts about this issue, please contact support.d1ht@gmail.com
     echo
fi     
