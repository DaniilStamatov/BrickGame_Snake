#include "renderer.h"

void renderer_main(GameInfo_t game_info) {
  clear();
   if (game_info.pause == PLAYING) {
    print_border();
  }
  if (game_info.field == NULL) {
    print_main_menu();
  } else {
    print_field(game_info);
    display_score(game_info);
  }
  
 
  if (game_info.pause == PAUSE) {
    print_pause_menu();
  }
  if (game_info.pause == GAME_OVER) {
    print_game_over_field(game_info);
  }
  refresh();
}

void print_border() {
  const char border = '|';
  const char border1 = '-';
  int row = HEIGHT + 2, col = WIDTH * 2 + 2;
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
  mvaddstr(10, 0, "Press 'Enter' to start");
  attroff(COLOR_PAIR(3));
}
void print_pause_menu() {
  attron(COLOR_PAIR(3));
  mvaddstr(10, 0, "Paused! Press Enter to start");
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
  int shift_x = WIDTH * 2 + 4;
  mvaddstr(0, shift_x, "HIGH SCORE:");
  mvprintw(1, shift_x, "%d", game.high_score);
  mvprintw(3, shift_x, "SCORE:");
  mvprintw(4, shift_x, "%d", game.score);
  mvprintw(6, shift_x, "LEVEL:");
  mvprintw(7, shift_x, "%d", game.level);
  if(game.next) {
    mvaddstr(9, shift_x, "NEXT:");
    for (int i = 0; i < FIGURE_HEIGHT; ++i) {
      for (int j = 0; j < FIGURE_HEIGHT; ++j) {
        if (game.next[i][j] != 0) {
          ADD_BLOCK(14 + i, 24 + j * 2 + 1, game.next[i][j]);
        }
      }
    }
  }
}

void print_game_over_field(GameInfo_t game) {
  clear();
  attron(COLOR_PAIR(2));
  mvprintw(10, 2, "GAME OVER!! YOUR SCORE: %d", game.score);
  mvaddstr(12, 2, "Press 'Enter' to restart");
  attroff(COLOR_PAIR(2));
  refresh();
}

void init_ncurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  halfdelay(1);
  curs_set(0);
  init_colors();
}

void process_input(int c) {
  if (c == '\n') {
    userInput(Start, false);
  } else if (c == ' ') {
    userInput(Pause, false);
  } else if (c == 'q') {
    userInput(Terminate, false);
  } else if (c == KEY_LEFT) {
    userInput(Left, false);
  } else if (c == KEY_RIGHT) {
    userInput(Right, false);
  } else if (c == KEY_UP) {
    userInput(Up, false);
  } else if (c == KEY_DOWN) {
    userInput(Down, false);
  } else if (c == 'r') {
    userInput(Action, false);
  }
}

void clear_ncurses() {
  wclear(stdscr);
  endwin();
}

void GameLoop() {
  init_ncurses();
  srand(time(NULL));
  clock_t prev_time = clock();
  GameInfo_t game_info;
  while (game_info.pause != TERMINATE) {
    clock_t current = clock();
    float deltatime = (float)(current - prev_time);
    prev_time = current;
    game_info = updateCurrentState(deltatime);
    renderer_main(game_info);
    process_input(getch());
  }
  clear_ncurses();
}