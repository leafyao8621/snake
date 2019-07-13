CC = gcc
AR = ar
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
LIB = snake.a

%.o: %.c
	$(CC) -g -c $< -o $@

$(LIB): $(OBJ)
	@cp src/snake.h .
	$(AR) rcs $@ $^

all: clean
clean: $(LIB)
	@rm $(OBJ) $(LIB) snake.h
