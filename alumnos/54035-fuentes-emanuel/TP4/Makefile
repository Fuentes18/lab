CC = gcc
CFLAGS=-Wall

LDLIBS=-pthread

server: 
	$(CC) $(LDLIBS) -o server server.c
cliente:
	$(CC) $(LDLIBS) -o client client.c

clean:
	rm server client

