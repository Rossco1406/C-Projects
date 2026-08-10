CC = cc
CFLAGS = -Wall -Wextra -std=c99
LDLIBS = -lm

TARGET = calculator

OBJS = main.o calculator.o input.o stack.o opstack.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

main.o: main.c calculator.h
calculator.o: calculator.c calculator.h stack.h opstack.h input.h
input.o: input.c input.h
stack.o: stack.c stack.h
opstack.o: opstack.c opstack.h

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean


