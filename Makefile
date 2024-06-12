TARGET = sdvmodman
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))
CC = cc
CFLAGS=-Iinclude

all: clean $(TARGET)

clean:
	rm -f *.o
	rm -f sdvmodman

$(TARGET): $(OBJ)
	$(CC) -o $@ $? $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

install:
	mkdir -p /usr/local/bin
	cp $(TARGET) /usr/local/bin/$(TARGET)

uninstall:
	rm -f /usr/local/bin/$(TARGET)
