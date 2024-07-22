CC=gcc
CCFLAGS=-g -std=c99 -Wall -I./include/
LDFLAGS=-lcglm -lglfw -lGL -lm
BIN=mine

SRC=$(shell find . -name "*.c")
OBJ=$(SRC:.c=.o)

all: $(BIN)

%.o: %.c
	$(CC) $(CCFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(BIN)
