CC = gcc
LIBS = -lgthread-2.0 `pkg-config gtk+-2.0 --cflags --libs`

all:	client	server

client:	client.c
	${CC} -w -g -pthread client.c $(LIBS) -o client  

server:	server.c
	${CC} -lm -pthread server.c  -o server  

clean:
	rm -f *.o *~ client server