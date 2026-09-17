CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

minishell: src/main.o src/shell.o src/parser.o
	$(CC) src/main.o src/shell.o src/parser.o -o minishell

src/main.o: src/main.c include/shell.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/shell.o: src/shell.c include/shell.h include/parser.h
	$(CC) $(CFLAGS) -c src/shell.c -o src/shell.o

src/parser.o: src/parser.c include/parser.h include/shell.h
	$(CC) $(CFLAGS) -c src/parser.c -o src/parser.o

clean:
	rm -f src/*.o minishell
