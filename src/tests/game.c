#include "brick_game/tetris/tetris.h"
#include "gui/cli/renderer.h"

int main() {
  init_ncurses();
  srand(get_time());
  GameInfo_t game_info = {0};
  while (game_info.pause != 3) {
    game_info = updateCurrentState();
    renderer_main(game_info);
    process_input(getch());
  }
  clear_ncurses();
  return 0;
}