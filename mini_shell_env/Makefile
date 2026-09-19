CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

all: minishell server client

minishell: src/main.o src/shell.o src/parser.o
	$(CC) src/main.o src/shell.o src/parser.o -o minishell

server: src/server.o src/shell.o src/parser.o
	$(CC) src/server.o src/shell.o src/parser.o -o server

client: src/client.o
	$(CC) src/client.o -o client

src/main.o: src/main.c include/shell.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/shell.o: src/shell.c include/shell.h include/parser.h
	$(CC) $(CFLAGS) -c src/shell.c -o src/shell.o

src/parser.o: src/parser.c include/parser.h include/shell.h
	$(CC) $(CFLAGS) -c src/parser.c -o src/parser.o

src/server.o: src/server.c include/shell.h include/parser.h
	$(CC) $(CFLAGS) -c src/server.c -o src/server.o

src/client.o: src/client.c
	$(CC) $(CFLAGS) -c src/client.c -o src/client.o

clean:
	rm -f src/*.o minishell server client
