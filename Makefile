CC = gcc
CFLAGS = -Wall

all: server client

server: server.c user.c
	$(CC) $(CFLAGS) server.c user.c -o server

client: client.c user.c
	$(CC) $(CFLAGS) client.c user.c -o client

clean:
	rm -f server client
