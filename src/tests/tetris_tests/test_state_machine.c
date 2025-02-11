#include "tests.h"

START_TEST(test_process_input_pause) {
  Game *game = get_current_game_info();
  init_game(game);
  game->action = Start;
  game->new_input = 0;

  game->action = Pause;
  ck_assert_int_eq(game->action, Pause);

  finish_game(game);
}
END_TEST

START_TEST(test_process_input_terminate) {
  Game *game = get_current_game_info();
  init_game(game);
  game->action = Start;
  game->new_input = 0;

  game->action = Terminate;
  ck_assert_int_eq(game->action, Terminate);
  finish_game(game);
}
END_TEST

START_TEST(test_process_input) {
    Game *game = get_current_game_info();
    init_game(game);
    game->new_input = 0;
    userInput(Start, 0);
    updateCurrentState(0);
    ck_assert_int_eq(game->game_info.pause, 0);
    create_new_falling(game);
    game->new_input = 0;

    userInput(Right, 0);
    updateCurrentState(0);
    ck_assert_int_eq(game->current_state, MOVING);
    ck_assert_ptr_nonnull(game->current);
    move_figure(game, 1.0f);
    ck_assert_int_eq(game->current->location.x, 4);
    game->new_input = 0;
    userInput(Left, 0);
    move_figure(game, 1.0f);
    updateCurrentState(0);
    ck_assert_int_eq(game->current->location.x, 3);
    game->new_input = 0;
    userInput(Action, 0);
    updateCurrentState(0);
    ck_assert_int_eq(game->current->location.x, 3);
    game->new_input = 0;

    userInput(Terminate, 0);

    ck_assert_int_eq(game->action, Terminate);
    finish_game(game);
}
END_TEST


Suite *machine_suite(void) {
  Suite *s = suite_create("\033[45m-=S21_CALC_COMPLEMENTS=-\033[0m");
  TCase *tc_core = tcase_create("core");

  tcase_add_test(tc_core, test_process_input_pause);
  tcase_add_test(tc_core, test_process_input_terminate);
  tcase_add_test(tc_core, test_process_input);
  suite_add_tcase(s, tc_core);
  return s;
}
