#include "tests.h"

START_TEST(init_test_0) {
  Game game = {0};
  init_game(&game);
  for (int i = 0; i < HEIGHT; ++i)
    for (int j = 0; j < WIDTH; ++j)
      ck_assert_int_eq(0, game.game_info.field[i][j]);

  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) ck_assert_int_eq(0, game.game_info.next[i][j]);
  ck_assert_int_eq(0, game.game_info.score);
  ck_assert_int_eq(0, game.game_info.pause);
  ck_assert_int_eq(1, game.game_info.level);
  finish_game(&game);
}
END_TEST

START_TEST(test_move_block_down) {
  Game game;
  init_game(&game);
  create_new_falling(&game);
  move_block_down(&game);
  ck_assert_int_eq(game.current->location.y, 1);
  finish_game(&game);
}
END_TEST

START_TEST(test_process_rotation) {
  Game game;
  init_game(&game);
  create_new_falling(&game);
  game.current->orientation = 0;
  process_rotation(&game);
  ck_assert_int_eq(game.current->orientation, 3);
  finish_game(&game);
}
END_TEST

START_TEST(test_move_right) {
  Game game;
  init_game(&game);
  create_new_falling(&game);
  move_right_or_left(&game, 1);
  ck_assert_int_eq(game.current->location.x, 4);
  finish_game(&game);
}
END_TEST

START_TEST(test_move_left) {
  Game game;
  init_game(&game);
  create_new_falling(&game);
  move_right_or_left(&game, -1);
  ck_assert_int_eq(game.current->location.x, 2);
  finish_game(&game);
}
END_TEST

START_TEST(test_move_right_collision) {
  Game game;
  init_game(&game);
  create_new_falling(&game);
  for (int i = 0; i < 6; ++i) {
    move_right_or_left(&game, 1);
  }
  ck_assert_int_eq(0, check_block_fits(&game, game.current));
  ck_assert_int_eq(game.current->location.x, 8);
  finish_game(&game);
}
END_TEST

START_TEST(test_move_to_bottom) {
  Game game;
  init_game(&game);
  create_new_falling(&game);
  move_to_bottom(&game);
  ck_assert_int_ge(game.current->location.y, HEIGHT - 4);
  finish_game(&game);
}
END_TEST

START_TEST(test_check_lines_full_1) {
  Game game;
  init_game(&game);
  for (int j = 0; j < WIDTH; j++) {
    game.game_info.field[HEIGHT - 1][j] = 1;
  }

  int initial_score = game.game_info.score;
  check_lines_full(&game);

  ck_assert_int_eq(game.game_info.score, initial_score + 100);
  for (int j = 0; j < WIDTH; j++) {
    ck_assert_int_eq(game.game_info.field[HEIGHT - 1][j], 0);
  }

  finish_game(&game);
}
END_TEST

START_TEST(test_check_lines_full_2) {
  Game game;
  init_game(&game);
  for (int i = HEIGHT - 1; i >= HEIGHT - 3; i--) {
    for (int j = 0; j < WIDTH; j++) {
      game.game_info.field[i][j] = 1;
    }
  }
  int initial_score = game.game_info.score;
  check_lines_full(&game);

  ck_assert_int_eq(game.game_info.score, initial_score + 500);
  for (int i = HEIGHT - 1; i >= HEIGHT - 3; i--) {
    for (int j = 0; j < WIDTH; j++) {
      ck_assert_int_eq(game.game_info.field[i][j], 0);
    }
  }
  finish_game(&game);
}
END_TEST

START_TEST(test_check_lines_full_3) {
  Game game;
  init_game(&game);
  for (int i = HEIGHT - 1; i >= HEIGHT - 2; i--) {
    for (int j = 0; j < WIDTH; j++) {
      game.game_info.field[i][j] = 1;
    }
  }
  int initial_score = game.game_info.score;
  check_lines_full(&game);

  ck_assert_int_eq(game.game_info.score, initial_score + 300);
  for (int i = HEIGHT - 1; i >= HEIGHT - 2; i--) {
    for (int j = 0; j < WIDTH; j++) {
      ck_assert_int_eq(game.game_info.field[i][j], 0);
    }
  }
  finish_game(&game);
}
END_TEST

START_TEST(test_check_lines_full_4) {
  Game game;
  init_game(&game);
  for (int i = HEIGHT - 1; i >= HEIGHT - 4; i--) {
    for (int j = 0; j < WIDTH; j++) {
      game.game_info.field[i][j] = 1;
    }
  }
  int initial_score = game.game_info.score;
  check_lines_full(&game);

  ck_assert_int_eq(game.game_info.score, initial_score + 700);
  for (int i = HEIGHT - 1; i >= HEIGHT - 4; i--) {
    for (int j = 0; j < WIDTH; j++) {
      ck_assert_int_eq(game.game_info.field[i][j], 0);
    }
  }
  finish_game(&game);
}
END_TEST

Suite *movement_suite(void) {
  Suite *s = suite_create("\033[45m-=S21_CALC_COMPLEMENTS=-\033[0m");
  TCase *tc_core = tcase_create("core");
  tcase_add_test(tc_core, init_test_0);
  tcase_add_test(tc_core, test_move_block_down);
  tcase_add_test(tc_core, test_process_rotation);
  tcase_add_test(tc_core, test_move_right);
  tcase_add_test(tc_core, test_move_left);
  tcase_add_test(tc_core, test_move_right_collision);
  tcase_add_test(tc_core, test_move_to_bottom);
  tcase_add_test(tc_core, test_check_lines_full_1);
  tcase_add_test(tc_core, test_check_lines_full_2);
  tcase_add_test(tc_core, test_check_lines_full_3);
  tcase_add_test(tc_core, test_check_lines_full_4);
  suite_add_tcase(s, tc_core);
  return s;
}
