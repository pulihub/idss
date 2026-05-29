############################################################
# Makefile per la compilazione del wrapper a D1HT 
#
# il target "d1ht_wrapper.o" va linkato al demo 
#
############################################################

d1ht_wrapper.o : d1ht_wrapper.cpp d1ht_wrapper.h
	g++ -w -fPIE -c d1ht_wrapper.cpp -o d1ht_wrapper.o -I.. -L.. -ld1ht

clean:
	rm *.o
############################################################	
