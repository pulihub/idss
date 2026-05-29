#!/bin/bash
# This file is part of D1HT. Please see its license terms in D1HT.license.txt
#
# This script prints a summary of errors and warnings of all D1HT and D1HWrap sysouts in the current working directory 
#
# This script takes no parameters
#

echo

if [ "$#" -ne "0" ]
then
     echo ERROR: This script takes no parameters
     exit -1
fi

echo "D1HT peers Started:" `ls -1 | grep "e_D1HT:" | grep txt | grep -Ev "stats.txt|dump.txt|statsN.txt|D1HWrap" | wc -l`

rm -f lixo1_D1HT.txt errlist.txt

ls -1 | grep -E "e_D1H|E_|ERROR.txt" | grep txt | grep -Ev "stats.txt|dump.txt|statsN.txt" | xargs grep -E "match | Exiting|Broken pipe|cancelled|Wrong|Secondary|Exiting|ERROR|failed|Failed|Assert|ASSERT|Join finished OK|RETURN|WARNING|ATTENTION" | grep -v "Dumping status of threads and mutexes" | grep -vi "lookup failed" | grep -v lookups_failed | grep -v "Probe failed" | grep -v "Signal 15 caught"  > lixo1_D1HT.txt 2>/dev/null

sort -u lixo1_D1HT.txt > lixo_all.txt

rm -f lixo1_D1HT.txt errlist.txt

echo "D1HT peers joined OK:" `grep "e_D1HT:" lixo_all.txt | grep "Join finished OK" | wc -l`

echo "Dumps:" `ls -1 | grep "D1HT" | grep dump | grep -v txt  2>/dev/null | wc -l`  | grep -v ": 0"
 
echo "cores:" `ls -al core.* 2>/dev/null | wc -l` | grep -v ": 0"

echo D1HWrap ERRORS: `grep D1HWrap lixo_all.txt | grep Cause | grep -Evi "RETURN|WARNING|ATTENTION|Secondary ERROR" | tee -a errlist.txt | awk -F txt '{print $1}' | sort -u | wc -l` | grep -v ": 0"

grep -E "RETURN|WARNING|ATTENTION" lixo_all.txt | grep Cause | sort -u > lixo3.txt

grep D1HT lixo_all.txt | grep -Ev "D1HWrap|D1HLookup" | grep -Ev "RETURN|WARNING|ATTENTION" > lixo_D1HT.txt

echo "D1HT peers cancelled:" `grep " cancelled" lixo_D1HT.txt | awk -F txt '{print $1}' | sort -u | wc -l` | grep -v ": 0"

echo "D1HT Secondary ERROR:" `grep "Secondary ERROR" lixo_D1HT.txt | tee errlist.txt | awk '{print $1}' | sort -u | wc -l` | grep -v ": 0"

grep -Evi "cancelled|Secondary ERROR" lixo_D1HT.txt > lixo1_D1HT.txt

echo "D1HT ERRORS:" `grep -E "match | Exiting|cancelled|Wrong|Secondary|Exiting|ERROR|failed|Failed|Assert|ASSERT" lixo1_D1HT.txt | awk -F txt '{print $1}' | sort -u | wc -l` | grep -v ": 0"

echo "D1HT Exceptions:" `grep -E "match |Wrong|ERROR|failed|Failed" lixo1_D1HT.txt | grep -Ev "ret = bind|Join" | awk -F txt '{print $1}' | sort -u | awk -F : '{print " " $2 " " host}' | tee xeq.exceptions | wc -l` | grep -v ": 0"

echo "D1HT Assert:" `grep Assert lixo1_D1HT.txt | tee -a errlist.txt | awk -F txt '{print $1}' | sort -u | wc -l` | grep -v ": 0"

echo "D1HT ASSERT:" `grep ASSERT lixo1_D1HT.txt | tee -a errlist.txt | awk -F txt '{print $1}' | sort -u | wc -l` | grep -v ": 0"

echo "D1HT Join ERRORS:" `grep ERROR lixo1_D1HT.txt | grep Cause | grep Join | tee -a errlist.txt | awk -F txt '{print $1}' | sort -u | wc -l`  | grep -v ": 0"

echo D1HT lack of memory ERRORS: `grep ERROR lixo1_D1HT.txt | grep "ERROR Message: Cannot allocate memory" | tee -a errlist.txt | awk -F txt '{print $1}' | sort -u | wc -l` | grep -v ": 0"

echo D1HT memory problems: `grep ERROR lixo1_D1HT.txt | grep "malloced" | grep Exiting | tee -a errlist.txt | awk -F txt '{print $1}' | sort -u | wc -l` | grep -v ": 0"

echo D1HT SIG faults: `grep ERROR lixo1_D1HT.txt | grep SIG | grep Cause |  tee -a errlist.txt | awk -F txt '{print $1}' | sort -u | wc -l` | grep -v ": 0"

echo D1HT bind ERRORs: `grep ERROR lixo1_D1HT.txt | grep Exiting | grep "ret = bind" | tee -a errlist.txt | awk -F txt '{print $1}' | sort -u | wc -l`  | grep -v ": 0"

echo D1HT other errors: `grep ERROR lixo1_D1HT.txt | grep Exiting | grep -vE "Join|Secondary|ASSERT|cancelled|Signal|SIG|Assert|memory|ret = bind" | tee -a errlist.txt | awk -F txt '{print $1}' | sort -u | wc -l` | grep -v ": 0"

echo "Broken pipe:" `grep -i "broken pipe" lixo_all.txt | tee -a errlist.txt | wc -l` | grep -v ": 0"

echo "peer failed:" `grep "peer failed" lixo_all.txt | grep D1HWrap | tee -a errlist.txt | wc -l` | grep -v ": 0"

echo "Failed to get peer stats:" `grep "Failed to get " lixo_all.txt | grep D1HWrap | wc -l`  | grep -v ": 0"

echo "stats did not match:" `grep "did not match" lixo_all.txt | grep D1HWrap | wc -l`  | grep -v ": 0"

echo problem: `Dgrep e_D1HW -i problem | grep -v "Failed to get " | tee -a errlist.txt | wc -l` | grep -v ": 0"

echo Wrong: `grep Wrong lixo_all.txt | tee -a errlist.txt | wc -l` | grep -v ": 0"

echo problem with local UDP port: `grep "reopening local UDP port" lixo3.txt | grep Cause | tee -a errlistx.txt | wc -l`  | grep -v ": 0"

echo RETURN: `grep RETURN lixo3.txt | tee -a errlistx.txt | wc -l` | grep -v ": 0"

echo WARNING: `grep WARNING lixo3.txt | tee -a errlistx.txt | wc -l` | grep -v ": 0"

echo ATTENTION: `grep ATTENTION lixo3.txt | tee -a errlistx.txt | wc -l` | grep -v ": 0"

grep Cause lixo_all.txt | grep -Evi "secondary error|warning" | grep -E "ERROR|ASSERT|Assert|ASSERT" | sort -bfiu > lixo_cause.txt 

export XCOUNT=`cat lixo_cause.txt | wc -l`

if [ "$XCOUNT" -gt "0" ] 
then
   echo
   echo ERROR count by cause - total of $XCOUNT occurrences
   echo

   cat lixo_cause.txt | awk -F Cause '{print $2}' | awk -F Detail '{print $1}' | sort -bfi | uniq -c | sort -rn
fi

grep Cause lixo_D1HT.txt | grep -i secondary | grep -i error | sort -bfiu > lixo_cause.txt 

export XCOUNT=`cat lixo_cause.txt | wc -l`

if [ "$XCOUNT" -gt "0" ] 
then
   echo
   echo Secondary ERROR count by cause - total of $XCOUNT occurrences
   echo

   cat lixo_cause.txt | awk -F Cause '{print $2}' | awk -F Detail '{print $1}' | sort -bfi | uniq -c | sort -rn
fi

grep Cause lixo_all.txt | grep -Evi "cancelled|secondary error" | grep -Ev "ERROR|ASSERT|Assert|ASSERT" | sort -bfiu > lixo_cause.txt 

export XCOUNT=`cat lixo_cause.txt | wc -l`

if [ "$XCOUNT" -gt "0" ] 
then
   echo
   echo Warning/etc count by cause - total of $XCOUNT occurrences
   echo

   cat lixo_cause.txt | awk -F Cause '{print $2}' | awk -F Detail '{print $1}' | sort -bfi | uniq -c | sort -rn
fi

grep Cause lixo_all.txt | grep FILE= | grep -Evi "cancelled|secondary error" | sort -bfiu > lixo_cause.txt 

export XCOUNT=`cat lixo_cause.txt | wc -l`

if [ "$XCOUNT" -gt "0" ] 
then
   echo
   echo Problem/warning count by file line - total of $XCOUNT occurrences
   echo

   cat lixo_cause.txt | grep FILE= | awk -F FILE= '{print $2}' | awk -F Detail '{print $1}' | sort -bfi | uniq -c | sort -rn
fi

echo

rm -f lixo*.txt errlist*.txt

#head -100 errlistx.txt > errlist.txt
#rm -f errlistx.txt

#echo errlogs 
#cat E*.txt | grep -v "Thread signal_handler_thread" 2>/dev/null

