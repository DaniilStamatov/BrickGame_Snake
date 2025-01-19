#include "tetris.h"

long long get_time() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

int timer(Game *game, int delay) {
  int result = 0;
  long long time = get_time();

  if (time - game->time >= delay) {
    game->time = time;
    result = 1;
  }

  return result;
}
