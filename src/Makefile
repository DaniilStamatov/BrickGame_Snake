CC = gcc
CFLAGS = -Wall -Wextra  -Werror -std=c11 -g
GCOV_FLAGS = --coverage -fprofile-arcs -ftest-coverage
CHECK_LIBS = -lcheck -lsubunit -lrt -lpthread -lm

TARGET = ./tetris.a
OBJ_DIR = ./build/obj
GAME = ./game.c
FRONTEND = ./gui/cli/renderer.c
TETRIS_SOURCES = $(wildcard ./brick_game/tetris/*.c)
OBJS = $(patsubst ./%.c, $(OBJ_DIR)/%.o, $(TETRIS_SOURCES))
all: clean install

install:$(TARGET)
	$(CC) $(CFLAGS) -o tetris $(GAME) $(FRONTEND) $(TARGET) -lncurses
	make clean

$(TARGET): $(OBJS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR) 
	$(CC) $(CFLAGS) -c $< -o $@

clang_format:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i *.c
	clang-format -i brick_game/tetris/*.c brick_game/tetris/*.h
	clang-format -i gui/cli/*.c gui/cli/*.h
	rm -f .clang-format

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean