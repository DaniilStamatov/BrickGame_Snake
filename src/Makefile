CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lncurses -lm
TARGET = ./tetris.a
OBJ_DIR = ./build/obj
GAME = ./game.c
FRONTEND = ./gui/cli/renderer.c
TETRIS_SOURCES = $(wildcard ./brick_game/tetris/*.c)
OBJS = $(patsubst ./brick_game/tetris/%.c, $(OBJ_DIR)/brick_game/tetris/%.o, $(TETRIS_SOURCES))

all: $(TARGET) tetris

tetris: $(GAME) $(FRONTEND) $(TARGET)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET): $(OBJS)
	ar rc $@ $^
	ranlib $@

$(OBJ_DIR)/brick_game/tetris/%.o: ./brick_game/tetris/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clang_format:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i *.c
	clang-format -i brick_game/tetris/*.c brick_game/tetris/*.h
	clang-format -i gui/cli/*.c gui/cli/*.h
	rm -f .clang-format

clean:
	rm -rf $(OBJ_DIR) $(TARGET) tetris

.PHONY: all clean clang_format