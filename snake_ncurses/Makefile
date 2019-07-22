CC = gcc
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
LIB =  ../snake/snake.a
BIN = snake

%.o: %.c
	$(CC) -c $< -o $@

$(BIN): $(OBJ)
	$(CC) $(OBJ) $(LIB) -o $(BIN) -lncurses

.PHONY: clean
clean: $(BIN)
	@rm $(OBJ) $(BIN)
