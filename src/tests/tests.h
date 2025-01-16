#ifndef UNIT_TESTS_S21_BRICK_GAME_H_
#define UNIT_TESTS_S21_BRICK_GAME_H_

#include <check.h>
#include <time.h>
#include <unistd.h>

#include "../brick_game/tetris/tetris.h"
Suite *movement_suite(void);
Suite *machine_suite(void);

void run_tests(void);
void run_testcase(Suite *testcase);

#endif
