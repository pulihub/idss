
export FILES=/tmp/tot.dump.$PPID

rm -f $FILES 2>/dev/null

for i in $1*.dump
do
 ./D1HStats $i

  if [ $? -eq 0 ]
  then
     cat $i >> $FILES
  else
     mv e_D1HStats_*.txt $i.ERROR
  fi

  rm -f  e_D1HStats_*.txt

done
./D1HStats $FILES

   grep -v "filesystem " $FILES.stats.txt | grep -v "FILE=" > $1.txt
   echo >> stats_application.txt

   grep -v "filesystem " $FILES.stats.txt | grep "FILE=" | sort -rn -t , -k 2 | sort -t % -k 2 >> $1.txt
   echo >> stats_application.txt

   grep "filesystem " $FILES.stats.txt | grep -v "FILE=" >> $1.txt

   rm -f  $FILES.stats.txt > /dev/null 2>/dev/null

rm -f $FILES

echo `date` consolidated statistics generated on $1.txt
