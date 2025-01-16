#include "renderer.h"

void renderer_main(GameInfo_t game_info) {
  clear();
  if (game_info.field == NULL) {
    print_main_menu();
  } else {
    print_field(game_info);
    display_score(game_info);
  }
  if (game_info.pause == 1) {
    print_pause_menu();
  }
  if (game_info.pause == 2) {
    print_game_over_field(game_info);
  }
  if (game_info.pause == 0) {
    print_border();
  }
  refresh();
}

void print_border() {
  const char border = '|';
  const char border1 = '-';
  int row = 22, col = 22;
  for (int i = 0; i < row; i++)
    for (int j = 0; j < col; j++) {
      if (i == 0) {
        mvaddch(i, j, border1);
      }
      if (i == row - 1) {
        mvaddch(i, j, border1);
      }
      if (j == 0 && i > 0) {
        mvaddch(i, j, border);
      }
      if (j == col - 1 && i > 0) {
        mvaddch(i, j, border);
      }
    }
}

void print_main_menu() {
  attron(COLOR_PAIR(3));
  mvaddstr(10, 2, "Press 'r' to start");
  attroff(COLOR_PAIR(3));
}
void print_pause_menu() {
  attron(COLOR_PAIR(3));
  mvaddstr(10, 2, "Paused! Press 'r' to start");
  attroff(COLOR_PAIR(3));
}

void init_colors() {
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_CYAN);
  init_pair(2, COLOR_WHITE, COLOR_GREEN);
  init_pair(3, COLOR_WHITE, COLOR_YELLOW);
  init_pair(4, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(5, COLOR_WHITE, COLOR_GREEN);
  init_pair(6, COLOR_WHITE, COLOR_RED);
  init_pair(7, COLOR_WHITE, COLOR_BLUE);
  init_pair(8, COLOR_WHITE, COLOR_WHITE);
}

void print_field(GameInfo_t g) {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (g.field[i][j] == 0) {
        ADD_EMPTY(i + 1, j * 2 + 1);
      } else {
        ADD_BLOCK(i + 1, j * 2 + 1, g.field[i][j]);
      }
    }
  }
}

void display_score(GameInfo_t game) {
  mvaddstr(0, 24, "HIGH SCORE:");
  mvprintw(1, 24, "%d", game.high_score);
  mvprintw(3, 24, "SCORE:");
  mvprintw(4, 24, "%d", game.score);
  mvprintw(6, 24, "LEVEL:");
  mvprintw(7, 24, "%d", game.level);
  mvaddstr(9, 24, "NEXT:");
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (game.next[i][j] != 0) {
        ADD_BLOCK(14 + i, 24 + j * 2 + 1, game.next[i][j]);
      }
    }
  }
}

void print_game_over_field(GameInfo_t game) {
  clear();
  attron(COLOR_PAIR(2));
  mvprintw(10, 2, "GAME OVER!! YOUR SCORE: %d", game.score);
  mvaddstr(12, 2, "Press 'r' to restart");
  attroff(COLOR_PAIR(2));
  refresh();
}

void init_ncurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, 1);
  halfdelay(1);
  scrollok(stdscr, 1);
  curs_set(0);
  mouseinterval(1);
  keypad(stdscr, TRUE);
  init_colors();
}

void clear_ncurses() {
  wclear(stdscr);
  endwin();
}
