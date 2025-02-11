CFLAGS = gcc -Wall -Wextra -Werror -std=c11 -g
CPPFLAGS = g++ -Wall -Wextra -Werror -std=c++17 -g
GCOV_FLAGS= -fprofile-arcs -ftest-coverage
LDFLAGS = -lncurses -lm
CHECK_LIBS = -lcheck -lm
TETRIS_LIB = ./tetris.a
OBJ_DIR = ./build
GAME = ./game.c
SNAKE_GAME = ./cli_main.c
TEST_DIR = ./tests
FRONTEND = ./gui/cli/renderer.c
TEST_SRC =$(wildcard $(TEST_DIR)/tetris_tests/test_*.c) ./tests/tetris_tests/tests.c
SNAKE_TEST_SRC = $(wildcard $(TEST_DIR)/snake_tests/test_*.cpp)
TETRIS_SOURCES = $(wildcard ./brick_game/tetris/*.c ./brick_game/tetris/*.cpp)
OBJS = $(patsubst ./brick_game/tetris/%.c, $(OBJ_DIR)/brick_game/tetris/%.o, $(TETRIS_SOURCES))
SNAKE_DIRS = brick_game/snake/controller brick_game/snake/model

SNAKE_SOURCES = $(wildcard $(addsuffix /*.cpp, $(SNAKE_DIRS)))
SNAKE_OBJS = $(patsubst ./brick_game/snake/%.cpp, $(OBJ_DIR)/snake/%.o, $(SNAKE_SOURCES))

DIST_DIR=archive

all: install 

install: tetris snake
	rm -rf snake_game && mkdir snake_game && cd snake_game && cmake ../SnakeCmakeFile && make && cp Snake.exe ../build 
	rm -rf tetris_game && mkdir tetris_game && cd tetris_game && cmake ../TetrisCmakeFile && make && cp Tetris.exe ../build 

uninstall: 
	rm -rf ./build



tetris: $(SNAKE_GAME) $(FRONTEND) $(TETRIS_LIB)
	$(CFLAGS) -o $(OBJ_DIR)/$@ $^ $(LDFLAGS)  -DTETRIS

snake: snake.a
	$(CPPFLAGS) -o $(OBJ_DIR)/snake.o $(SNAKE_OBJS)  $(SNAKE_GAME) $(FRONTEND) $(LDFLAGS) -DSNAKE

snake.a: $(SNAKE_OBJS)
	mkdir -p $(OBJ_DIR)/snake
	ar rc $@ $^
	ranlib $@

# Rules for compiling Snake C++ files
$(OBJ_DIR)/snake/controller/%.o: ./brick_game/snake/controller/%.cpp
	@mkdir -p $(dir $@)
	$(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/snake/model/%.o: ./brick_game/snake/model/%.cpp
	@mkdir -p $(dir $@)
	$(CPPFLAGS) -c $< -o $@

# Rules for compiling Tetris C files
$(OBJ_DIR)/brick_game/tetris/%.o: ./brick_game/tetris/%.c
	@mkdir -p $(dir $@)
	$(CFLAGS) -c $< -o $@

$(TETRIS_LIB): $(OBJS)
	ar rc $@ $^
	ranlib $@

dist: clean_dist
	cd ../ && mkdir -p ${DIST_DIR}
	cd ../ && cp -rf src/brick_game $(DIST_DIR)/brick_game
	cd ../ && cp -rf src/gui $(DIST_DIR)/gui
	cd ../ && cp -rf src/Makefile $(DIST_DIR)/
	cd ../ && tar -czvf archive.tar.gz $(DIST_DIR)
	cd ../ && rm -rf $(DIST_DIR)

test: clean $(TETRIS_LIB)
	$(CFLAGS) $(GCOV_FLAGS) $(TETRIS_SOURCES) $(TEST_SRC) -o $(OBJ_DIR)/unit_test $(CHECK_LIBS)
	$(OBJ_DIR)/unit_test
	$(CPPFLAGS) $(GCOV_FLAGS) $(SNAKE_SOURCES) $(SNAKE_TEST_SRC) -o $(OBJ_DIR)/snake_unit_test -lgtest -lpthread -lm
	$(OBJ_DIR)/snake_unit_test


valgrind_tests: $(TEST_EXECUTABLES)
	@echo "Running tests with Valgrind..."
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --trace-children=yes -s $(OBJ_DIR)/unit_test 2>&1 || true;
	@echo "end"

dvi: clean
	doxygen doc/Doxyfile
	xdg-open html/html/index.html
	
clang-format:
	cp ../materials/linters/.clang-format .
	find . -type f \( -name '*.cpp' -o -name '*.h' -o -name '*.c' \) -exec clang-format -i {} \; -exec clang-format -n {} \;


gcov_report: test
	@echo "Generating coverage report..."
	lcov --capture --directory . --output-file coverage.info --exclude '/usr/include/*' --ignore-errors inconsistent
	genhtml coverage.info --output-directory ./coverage_report
	@xdg-open ./coverage_report/index.html || echo "Could not open report"

clean:
	rm -rf $(OBJ_DIR) $(TETRIS_LIB) tetris coverage.info unit_test
	rm -f ./*.gcno ./*.gcda  snake.a tetris.a high_score.txt snake_high_score.txt
	rm -rf $(OBJ_DIR) coverage_report
	rm -rf html
	rm -rf snake_game
	rm -rf tetris_game

clean_dist:
	@cd ../ && rm -rf archive
	@cd ../ && rm -rf archive.tar.gz

.PHONY: all clean clang_format dvi