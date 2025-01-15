CC = gcc
CFLAGS = -Wall -Wextra  -Werror -std=c11 -g
GCOV_FLAGS = --coverage -fprofile-arcs -ftest-coverage
CHECK_LIBS = -lcheck -lsubunit -lrt -lpthread -lm

TARGET = /tetris.a

SRCS = game.c brick_game/tetris/tetris.c gui/cli/renderer.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ -lncurses -lm

%.o: %.c
	$(CC) $(CFLAGS)   -c $< -o $@

clang_format:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i *.c
	clang-format -i brick_game/tetris/*.c brick_game/tetris/*.h
	clang-format -i gui/cli/*.c gui/cli/*.h
	rm -f .clang-format

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean