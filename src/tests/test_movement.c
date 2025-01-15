#include "tests.h"

START_TEST(movement_test_0){
    Game game = {0};
    init_game(&game);
    for(int i = 0; i < HEIGHT; ++i) {
        for(int j = 0; j < WIDTH; ++j) {
            ck_assert_int_eq(0, game.game_info.field[i][j]);
        }
    }
    ck_assert_int_eq(0, game.game_info.score);
    ck_assert_int_eq(0, game.game_info.pause);
    ck_assert_int_eq(1, game.game_info.level);
}
END_TEST

Suite *movement_suite(void) {
  Suite *s = suite_create("\033[45m-=S21_CALC_COMPLEMENTS=-\033[0m");
  TCase *tc_core = tcase_create("core");
  tcase_add_test(tc_core, movement_test_0);
  suite_add_tcase(s, tc_core);
  return s;
}
