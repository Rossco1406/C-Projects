CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = minishell
SOURCES = main.c shell.c parser.c
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clean
