export DIR=$PWD

echo
echo `date`: Benchmark $DIR - gathering stats

export FILES=$DIR/Totstat.dump

rm -f $FILES

rm -f stats_D1HStats_2*.txt

rm -f e_D1HStats_2*.txt

rm -f stats_D1HWrap*.dump.txt

#ls -1 stats_D1HWrap*.dump | while read file
#do
#  D1HStats $file >/dev/null 2>/dev/null
#  if [ $? -eq 0 ]
#  then
#     rm -f e_D1HStats_2*.txt
#     cat $file >> $FILES
#  fi
#  rm -f stats_D1HStats_2*.txt
#done

cat stats_D1HWrap*.dump > $FILES

D1HStats $FILES
if [ $? -eq 0 ]
then
    echo `date`: Benchmark $DIR - stats generated
else
    echo `date`: Benchmark $DIR - failed to generated stats
fi

rm -f $FILES >/dev/null 2>/dev/null

