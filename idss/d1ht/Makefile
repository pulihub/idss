## Compiler
CC					= g++

# C source files: original D1HT sources (modified)
OSOURCES = stats.cpp mtime.cpp exit.cpp xeqpoint.cpp messages.cpp threads.cpp common.cpp TCP_server.cpp UDP_server.cpp join.cpp lock.cpp RT_other.cpp RT_add_del.cpp RT_pred_succ.cpp test.cpp extern.cpp parameters.cpp events.cpp events_D1HT.cpp interval.cpp close_interval.cpp probe.cpp stab.cpp lookup.cpp memory.cpp rtt.cpp

#HPC Lab ex-novo files to provide IDSS extension
EXTENSION_SOURCES= QueryManager.c Query.c

SOURCES=$(OSOURCES) $(EXTENSION_SOURCES)

# Object files corresponding to source files.
OBJECTS_COMMON	= $(SOURCES:.c=.o)
OBJECTS_D1HT	= main.o signal.o user_call.o d1ht_main_thread.o d1ht.o
OBJECTS_D1HWrap	= D1HWrap.o D1HWrap_signal.o D1HWrap_lookup.o D1HWrap_routines.o
OBJECTS_D1HLookup= D1HLookup.o D1HWrap_signal.o D1HWrap_lookup.o D1HWrap_routines.o

LIBD1HT= libd1ht.a

KERVER = $(shell uname -r)
CPUARC = $(shell uname -p)

WARNINGFLAGS	=-Wall -Wextra -Wundef -Wsign-compare -Wno-write-strings -Wcast-align -Wconversion -Wdeprecated -Wunused -Wno-unused-parameter -fno-strict-aliasing

#BASEFLAGS 		=$(WARNINGFLAGS) -D_REENTRANT -D_THREAD_SAFE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_GNU_SOURCE -I./include -pthread -fPIC
BASEFLAGS 		=-D_REENTRANT -D_THREAD_SAFE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_GNU_SOURCE -I./include -pthread -fPIC


# Compiler Flags.
CFLAGS = $(BASEFLAGS) -O3
#CFLAGS	= -std=gnu++98 $(BASEFLAGS) -O3 -DD1HT_PASS_COUNT
#CFLAGS	= -std=gnu++98 $(BASEFLAGS) -O3 -DXEQP_LEVEL3 -DXEQP_THREAD
#CFLAGS	= -std=gnu++98 $(BASEFLAGS) -O0 -fno-inline -DDEBUG -DD1HT_PASS_COUNT
#CFLAGS	= -std=gnu++98 $(BASEFLAGS) -O0 -fno-inline -DDEBUG -DNOPROBE -ggdb

LBASEFLAGS =
LFLAGS = $(OTHERLDFLAGS) -lpthread -export-dynamic

### Rules.

.PHONY: all clean

all:	D1HT D1HLookup D1HWrap $(LIBD1HT) D1HStats D1HStat_compare

clean:
	rm -f *.a $(LIBD1HT) $(OBJECTS_COMMON) $(OBJECTS_D1HT) $(OBJECTS_D1HWrap) D1HT D1HWrap D1HStats D1HStat_compare D1HLookup *.o table*.txt o*.txt e*.txt dump*txt stat*.txt lixo*.txt *.dump xeq.exceptions file_fill *.so.* *.so *~ *_original *.results *.results.txt

$(LIBD1HT): $(OBJECTS_COMMON) signal.o
	ar rcs libd1ht.a $^

user_application:   	signal.o user_application.o
	$(CC) $(CFLAGS) -o $@ $^ -ldl -lcrypto -lc -lm $(LFLAGS)

D1HT:  	$(OBJECTS_D1HT) $(OBJECTS_COMMON)
	$(CC) $(CFLAGS) -o $@ $^ -ldl -lcrypto -lc -lm $(LFLAGS)

D1HStat_compare: 	$(OBJECTS_COMMON) D1HStat_compare.o signal.o
	$(CC) $(CFLAGS) -o $@ $^ -ldl -lcrypto -lc -lm $(LFLAGS)

D1HStats: 	$(OBJECTS_COMMON) D1HStats.o signal.o
	$(CC) $(CFLAGS) -o $@ $^ -ldl -lcrypto -lc -lm $(LFLAGS)

D1HWrap: 	$(OBJECTS_COMMON) $(OBJECTS_D1HWrap)
	$(CC) $(CFLAGS) -o $@ $^ -ldl -lcrypto -lc -lm $(LFLAGS)

D1HLookup: 	$(OBJECTS_COMMON) $(OBJECTS_D1HLookup)
	$(CC) $(CFLAGS) -o $@ $^ -ldl -lcrypto -lc -lm $(LFLAGS)

D1HStats.o: 		*.h Makefile
signal.o:			*.h Makefile
$(OBJECTS_COMMON):  *.h Makefile
$(OBJECTS_D1HT):    *.h Makefile
$(OBJECTS_D1HWrap): *.h Makefile
$(OBJECTS_D1HLookup):*.h Makefile
