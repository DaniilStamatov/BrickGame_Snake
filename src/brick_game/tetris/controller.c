#include "tetris.h"

Game *get_current_game_info() {
  static Game game_info_ex = {0};
  return &game_info_ex;
}

void userInput(UserAction_t user_input, bool hold) {
  (void)hold;
  Game *game_info_ex = get_current_game_info();
  if (!game_info_ex->new_input) {
    game_info_ex->action = user_input;
    game_info_ex->new_input = true;
  }
}

GameInfo_t updateCurrentState() {
  Game *game_info_ex = get_current_game_info();
  state_machine(game_info_ex);

  return game_info_ex->game_info;
}