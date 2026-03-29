GXX=gcc
FLAGS=-Wall -Werror -pedantic -g
SOURCES=server.c parser.c
TARGET=server.out

all:
	$(GXX) $(FLAGS) $(SOURCES) -o $(TARGET)

run:
	./$(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET)
