killall D1HT 2>/dev/null
rm -f e_D1H*.txt *.dump routing_thrash.txt 2>/dev/null
echo `date` - Running make D1HT
make D1HT >/dev/null
if [ $? -eq 0 ] 
then 
	export MYSELF=`uname -n`
	echo "npeers=2" > ./routing_thrash.txt
	echo "$MYSELF:9100" >> ./routing_thrash.txt
	echo "$MYSELF:9200" >> ./routing_thrash.txt
	echo `date` - Running D1HT
	./D1HT -C -V -o --routing_table=routing_thrash.txt -N 128 -s 10000 -a $MYSELF:9100 $@  &  
	./D1HT -C -V -o --routing_table=routing_thrash.txt -N 128 -s 10000 -a $MYSELF:9200 $@  &  
	sleep 2
	killall -SIGUSR1 D1HT 2>/dev/null
	sleep 1  
	killall -SIGHUP  D1HT 2>/dev/null
	sleep 1  
	killall -SIGCONT D1HT 2>/dev/null
	sleep 2
	./D1HT -V -o -p 9300 -s 10000 -A $MYSELF:9100 $@  &  
	./D1HT -V -o -p 9400 -s 10000 -A $MYSELF:9100 $@  &  
        sleep 5
	killall D1HT 2>/dev/null
	sleep 10
	killall -9 D1HT 2>/dev/null
	./D1HT_xeq.sh
else
	echo
	echo `date`- make error
fi
rm -f routing_thrash.txt 2>/dev/null
echo `date` - Ending $0

