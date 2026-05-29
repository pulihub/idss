
if [ "$#" -ne "1" ]
then
   echo
   echo ERROR command $0 requires one, and only one, parameter: the name of the input file
   echo
   exit -1
fi

if [ -f $1.stats.txt ]
then
   echo
   echo ERROR file $1.stats.txt already exists
   echo
   exit -1
fi

./D1HFileC_stats $1

if [ "$?" -ne "0" ]
then
   echo
   echo ERROR running command $0 $@
   echo
   exit -1
fi

