CC = cc
CFLAGS = -Wall -Wextra -std=c99

TARGET = filestats

all: $(TARGET)

$(TARGET): main.o filestats.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o filestats.o

main.o: main.c filestats.h
	$(CC) $(CFLAGS) -c main.c

filestats.o: filestats.c filestats.h
	$(CC) $(CFLAGS) -c filestats.c

clean:
	rm -f $(TARGET) *.o