#include "gui/cli/renderer.h"

int main() {
  init_ncurses();
  srand(time(NULL));
  clock_t prev_time = clock();
  GameInfo_t game_info  ; 

  game_info.field = NULL; 
  game_info.next = NULL;
  game_info.score = 0;
  game_info.high_score = 0;
  game_info.level = 0;
  game_info.speed = 0;
  game_info.pause = 0;
  while (game_info.pause != 2) {
    clock_t current = clock();
    float deltatime = (float)(current - prev_time);
    prev_time = current;
    game_info = updateCurrentState(deltatime);
    renderer_main(game_info);
    process_input(getch());
  }
  clear_ncurses();
  return 0;
}