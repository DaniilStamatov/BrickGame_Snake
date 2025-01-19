#ifndef RENDERER_H
#define RENDERER_H

#include <ncurses.h>

#include "../../brick_game/tetris/tetris.h"
#define ADD_BLOCK(y, x, c)            \
  mvaddch(y, x, ' ' | COLOR_PAIR(c)); \
  mvaddch(y, x + 1, ' ' | COLOR_PAIR(c))

#define ADD_EMPTY(y, x)               \
  mvaddch(y, x, ' ' | COLOR_PAIR(0)); \
  mvaddch(y, x + 1, ' ' | COLOR_PAIR(0))
void print_main_menu();
void print_pause_menu();
void print_game_over_field(GameInfo_t game);
void renderer_main(GameInfo_t game_info);
void init_colors();
void print_field(GameInfo_t g);
void display_score(GameInfo_t game);
void init_ncurses();
void clear_ncurses();
void print_border();
#endif