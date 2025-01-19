#include "tests.h"

START_TEST(test_process_input_start) {
  Game *game = get_current_game_info();
  init_game(game);

  process_input('r');
  ck_assert_int_eq(game->action, Start);
  game->new_input = 0;

  process_input(KEY_LEFT);
  ck_assert_int_eq(game->action, Left);
  game->new_input = 0;

  process_input(KEY_RIGHT);
  ck_assert_int_eq(game->action, Right);
  game->new_input = 0;

  process_input(KEY_DOWN);
  ck_assert_int_eq(game->action, Down);
  game->new_input = 0;

  process_input(' ');
  ck_assert_int_eq(game->action, Action);
  finish_game(game);
}
END_TEST

START_TEST(test_process_input_pause) {
  Game *game = get_current_game_info();
  init_game(game);
  process_input('r');
  game->new_input = 0;

  process_input('p');
  ck_assert_int_eq(game->action, Pause);

  finish_game(game);
}
END_TEST

START_TEST(test_process_input_terminate) {
  Game *game = get_current_game_info();
  init_game(game);
  process_input('r');
  game->new_input = 0;

  process_input('q');
  ck_assert_int_eq(game->action, Terminate);
  finish_game(game);
}
END_TEST

START_TEST(test_machine) {
  Game *game = get_current_game_info();
  init_game(game);
  process_input('r');
  game->new_input = 0;
  updateCurrentState();

  process_input(KEY_LEFT);
  updateCurrentState();

  ck_assert_int_eq(game->current->location.x, 2);
  process_input(KEY_RIGHT);
  updateCurrentState();
  ck_assert_int_eq(game->current->location.x, 3);
  process_input('p');
  updateCurrentState();
  ck_assert_int_eq(game->state, PAUSE);
  process_input('p');
  updateCurrentState();
  ck_assert_int_eq(game->state, MOVING);
  process_input(KEY_UP);
  updateCurrentState();
  ck_assert_int_eq(game->action, Up);
  process_input(KEY_DOWN);
  updateCurrentState();
  ck_assert_int_ge(game->current->location.y, HEIGHT - 4);
  process_input('q');
  updateCurrentState();
  ck_assert_int_eq(game->action, Terminate);
}
END_TEST

START_TEST(test_machine_1) {
  Game *game = get_current_game_info();
  process_input('r');
  game->new_input = 0;
  updateCurrentState();

  process_input(KEY_LEFT);
  updateCurrentState();

  ck_assert_int_eq(game->current->location.x, 2);
  process_input(KEY_RIGHT);
  updateCurrentState();
  ck_assert_int_eq(game->current->location.x, 3);
  process_input('p');
  updateCurrentState();
  ck_assert_int_eq(game->state, PAUSE);
  process_input('q');
  updateCurrentState();
  ck_assert_int_eq(game->action, Terminate);
}
END_TEST

Suite *machine_suite(void) {
  Suite *s = suite_create("\033[45m-=S21_CALC_COMPLEMENTS=-\033[0m");
  TCase *tc_core = tcase_create("core");

  tcase_add_test(tc_core, test_process_input_start);
  tcase_add_test(tc_core, test_process_input_pause);
  tcase_add_test(tc_core, test_process_input_terminate);
  tcase_add_test(tc_core, test_machine);
  tcase_add_test(tc_core, test_machine_1);
  suite_add_tcase(s, tc_core);
  return s;
}
