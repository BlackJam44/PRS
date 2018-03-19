CC = gcc
LD = gcc
CFLAGS += -Wall -g -c
LDFLAGS += -Wall -g

all: tpserveur tpclient

tpserveur: tpserveur.o functions.o
	${LD} ${LDFLAGS} tpserveur.o functions.o -o tpserveur

tpclient: tpclient.o functions.o
	${LD} ${LDFLAGS} tpclient.o functions.o -o tpclient

tpserveur.o: tpserveur.c
	${CC} ${CFLAGS} tpserveur.c -o tpserveur.o

tpclient.o: tpclient.c
	${CC} ${CFLAGS} tpclient.c -o tpclient.o

functions.o: functions.c functions.h
	${CC} ${CFLAGS} functions.c -o functions.o

clean:
	rm -f *~ *.o tpserveur tpclient  image.jpg
