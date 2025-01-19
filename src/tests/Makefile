CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
GCOV_FLAGS= -fprofile-arcs -ftest-coverage
LDFLAGS = -lncurses -lm
CHECK_LIBS = -lcheck -lm
TETRIS_LIB = ./tetris.a
OBJ_DIR = ./build
GAME = ./game.c
TEST_DIR = ./tests
FRONTEND = ./gui/cli/renderer.c
TEST_SRC=$(wildcard $(TEST_DIR)/test_*.c) ./tests/tests.c
TETRIS_SOURCES = $(wildcard ./brick_game/tetris/*.c)
OBJS = $(patsubst ./brick_game/tetris/%.c, $(OBJ_DIR)/brick_game/tetris/%.o, $(TETRIS_SOURCES))
DIST_DIR=archive

ifeq ($(OS_NAME),Linux)
OPEN = xdg-open
endif

ifeq ($(OS_NAME),Darwin)
OPEN = open
endif


all: $(TETRIS_LIB) install 
	$(OBJ_DIR)/tetris

install: tetris

uninstall: 
	rm $(OBJ_DIR)/tetris

tetris: $(GAME) $(FRONTEND) $(TETRIS_LIB)
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/$@ $^ $(LDFLAGS)

$(TETRIS_LIB): $(OBJS)
	ar rc $@ $^
	ranlib $@

$(OBJ_DIR)/brick_game/tetris/%.o: ./brick_game/tetris/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

dist: clean_dist
	cd ../ && mkdir -p ${DIST_DIR}
	cd ../ && cp -rf src/brick_game $(DIST_DIR)/brick_game
	cd ../ && cp -rf src/gui $(DIST_DIR)/gui
	cd ../ && cp -rf src/Makefile $(DIST_DIR)/
	cd ../ && tar -czvf archive.tar.gz $(DIST_DIR)
	cd ../ && rm -rf $(DIST_DIR)

test: clean $(TETRIS_LIB)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(TETRIS_SOURCES) $(TEST_SRC) -o $(OBJ_DIR)/unit_test $(CHECK_LIBS)
	$(OBJ_DIR)/unit_test

valgrind_tests: $(TEST_EXECUTABLES)
	@echo "Running tests with Valgrind..."
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --trace-children=yes -s $(OBJ_DIR)/unit_test 2>&1 || true;
	@echo "end"

dvi: clean
	doxygen doc/Doxyfile
	xdg-open html/html/index.html
	
gcov_report: test
	@echo "Generating coverage report..."
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory ./coverage_report
	@xdg-open ./coverage_report/index.html || echo "Could not open report"

clang_format:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i *.c
	clang-format -i brick_game/tetris/*.c brick_game/tetris/*.h
	clang-format -i gui/cli/*.c gui/cli/*.h
	clang-format -i tests/*.c tests/*.h
	rm -f .clang-format

clean:
	rm -rf $(OBJ_DIR) $(TETRIS_LIB) tetris coverage.info unit_test
	rm -f ./*.gcno ./*.gcda 
	rm -rf $(OBJ_DIR) coverage_report
	rm -rf html

clean_dist:
	@cd ../ && rm -rf archive
	@cd ../ && rm -rf archive.tar.gz

.PHONY: all clean clang_format dvi