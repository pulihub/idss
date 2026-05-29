############################################################
# Makefile per la compilazione del demo test
#
# il target "demo" simula l'interazione di un processo 
# esterno con D1HT
#
############################################################

demo : demo.o 
	g++ demo.o ../d1ht_main_thread.o ../d1ht.o ../wrapper/d1ht_wrapper.o -o demo -L.. -ld1ht -lpthread -lssl -lcrypto

demo.o : demo.c
	gcc -c demo.c -o demo.o -I.. -I../wrapper 

clean:
	rm *.o demo *.dump
############################################################
