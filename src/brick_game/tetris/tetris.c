#include "tetris.h"

void process_input(int c) {
  if (c == 'r') {
    userInput(Start, false);
  } else if (c == 'p') {
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
  } else if (c == ' ') {
    userInput(Action, false);
  }
}

void userInput(UserAction_t user_input, bool hold) {
  (void)hold;
  Game *game_info_ex = get_current_game_info();
  if (!game_info_ex->new_input) {
    game_info_ex->action = user_input;
    game_info_ex->new_input = true;
  }
}

Game *get_current_game_info() {
  static Game game_info_ex = {0};
  return &game_info_ex;
}

GameInfo_t updateCurrentState() {
  Game *game_info_ex = get_current_game_info();
  state_machine(game_info_ex);

  return game_info_ex->game_info;
}

void init_game(Game *game) {
  game->state = SPAWN;
  game->cleared = 0;
  game->is_playing = 1;
  game->blocks = init_all_block_types();
  game->time = get_time();
  game->current = malloc(sizeof(Tetromino));
  game->next = malloc(sizeof(Tetromino));
  init_new_block(game->blocks, rand() % 7, game);
  game->game_info = init_game_info();
}

GameInfo_t init_game_info() {
  GameInfo_t game = {0};
  game.score = 0;
  game.level = 1;
  game.pause = 0;
  game.speed = GAME_SPEED;
  game.next = create_matrix(4, 4);
  game.field = create_matrix(HEIGHT, WIDTH);
  FILE *file = fopen("high_score.txt", "r");
  if (file) {
    fscanf(file, "%d", &game.high_score);
    fclose(file);
  }
  return game;
}

TetrominoMap *init_all_block_types() {
  TetrominoMap *blocks = malloc(7 * sizeof(TetrominoMap));
  for (int i = 0; i < 7; i++) {
    init_new_map_block(&blocks[i], i);
  }
  return blocks;
}

void init_new_block(TetrominoMap *blocks, int type, Game *game) {
  game->next->type = type;
  int rotation = rand() % 4;
  game->next->orientation = rotation;
  for (int i = 0; i < 4; i++) {
    game->next->state[i] = blocks[type].states[rotation][i];
  }
}

Tetromino *rotate_block(const Tetromino *block, TetrominoMap *blocks,
                        int shift) {
  Tetromino *new_block = malloc(sizeof(Tetromino));
  new_block->type = block->type;
  int new_orientation = (block->orientation + shift) % 4;
  for (int i = 0; i < 4; i++) {
    new_block->state[i] = blocks[block->type].states[new_orientation][i];
  }
  new_block->orientation = new_orientation;
  new_block->location = block->location;
  return new_block;
}

void init_new_map_block(TetrominoMap *blocks, int type) {
  if (type == I) {
    init_block_I(blocks);
  } else if (type == L) {
    init_block_L(blocks);
  } else if (type == J) {
    init_block_J(blocks);
  } else if (type == O) {
    init_block_O(blocks);
  } else if (type == S) {
    init_block_S(blocks);
  } else if (type == T) {
    init_block_T(blocks);
  } else if (type == Z) {
    init_block_Z(blocks);
  }
}

void init_block_I(TetrominoMap *tetris) {
  position states[4][4] = {{{1, 0}, {1, 1}, {1, 2}, {1, 3}},
                           {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
                           {{3, 0}, {3, 1}, {3, 2}, {3, 3}},
                           {{0, 1}, {1, 1}, {2, 1}, {3, 1}}};
  fill_block_matrix(tetris, states);
}

void init_block_L(TetrominoMap *tetris) {
  position states[4][4] = {{{0, 2}, {1, 0}, {1, 1}, {1, 2}},
                           {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
                           {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
                           {{0, 0}, {0, 1}, {1, 1}, {2, 1}}};
  fill_block_matrix(tetris, states);
}

void init_block_J(TetrominoMap *tetris) {
  position states[4][4] = {{{0, 0}, {1, 0}, {1, 1}, {1, 2}},
                           {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
                           {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
                           {{0, 1}, {1, 1}, {2, 0}, {2, 1}}};
  fill_block_matrix(tetris, states);
}

void init_block_O(TetrominoMap *tetris) {
  position states[4][4] = {{{0, 1}, {0, 2}, {1, 1}, {1, 2}},
                           {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
                           {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
                           {{0, 1}, {0, 2}, {1, 1}, {1, 2}}};
  fill_block_matrix(tetris, states);
}

void init_block_S(TetrominoMap *tetris) {
  position states[4][4] = {{{0, 1}, {0, 2}, {1, 0}, {1, 1}},
                           {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
                           {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
                           {{0, 0}, {1, 0}, {1, 1}, {2, 1}}};
  fill_block_matrix(tetris, states);
}

void init_block_T(TetrominoMap *tetris) {
  position states[4][4] = {{{0, 1}, {1, 0}, {1, 1}, {1, 2}},
                           {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
                           {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
                           {{0, 1}, {1, 0}, {1, 1}, {2, 1}}};
  fill_block_matrix(tetris, states);
}

void init_block_Z(TetrominoMap *tetris) {
  position states[4][4] = {{{0, 0}, {0, 1}, {1, 1}, {1, 2}},
                           {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
                           {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
                           {{0, 1}, {1, 0}, {1, 1}, {2, 0}}};
  fill_block_matrix(tetris, states);
}

void set_cell(Game *game, int row, int col, int value) {
  game->game_info.field[row][col] = value;
}

void remove_block(Game *game, Tetromino *block) {
  for (int i = 0; i < 4; i++) {
    int row = block->location.y + block->state[i].y;
    int col = block->location.x + block->state[i].x;

    set_cell(game, row, col, 0);
  }
}

int is_in_range(int row, int column) {
  return 0 <= row && row < HEIGHT && 0 <= column && column < WIDTH;
}

int check_block_fits(const Game *game, Tetromino *block) {
  int result = 1;
  for (int i = 0; i < 4; ++i) {
    int column = block->location.x + block->state[i].x;
    int row = block->location.y + block->state[i].y;
    if (!is_in_range(row, column) || game->game_info.field[row][column] != 0) {
      result = 0;
    }
  }
  return result;
}

void fill_block_matrix(TetrominoMap *tetris, position states[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      tetris->states[i][j] = states[i][j];
    }
  }
}

void put_block(Game *game, Tetromino *block) {
  for (int i = 0; i < 4; ++i) {
    int x = block->location.x + block->state[i].x;
    int y = block->location.y + block->state[i].y;
    game->game_info.field[y][x] = block->type + 1;
  }
}

int **create_matrix(int rows, int columns) {
  int **matrix = (int **)calloc(rows, sizeof(int *));
  for (int i = 0; i < rows; i++) {
    matrix[i] = (int *)calloc(columns, sizeof(int));
  }
  return matrix;
}

void fill_current_from_next(Game *game) {
  Tetromino *temp = game->next;
  game->current->orientation = temp->orientation;
  game->current->type = temp->type;
  for (int i = 0; i < 4; ++i) {
    game->current->state[i] = temp->state[i];
  }
}

void create_new_falling(Game *game) {
  if (game->current == NULL) {
    game->current = malloc(sizeof(Tetromino));
  }
  fill_current_from_next(game);
  game->current->location = (position){3, 0};
  init_new_block(game->blocks, rand() % 7, game);
  fill_next_func(&game->game_info, game->next);
  game->time = get_time();
  game->state = check_block_fits(game, game->current) ? MOVING : GAME_OVER;
}

void fill_next_func(GameInfo_t *game_info, Tetromino *next) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      game_info->next[i][j] = 0;
    }
  }
  for (int i = 0; i < 4; i++) {
    int x = next->state[i].x;
    int y = next->state[i].y;
    if (y >= 0 && y < 4 && x >= 0 && x < 4) {
      game_info->next[y][x] = next->type + 1;
    }
  }
}

void move_block_down(Game *game) {
  remove_block(game, game->current);
  game->current->location.y++;
  if (check_block_fits(game, game->current)) {
    game->state = MOVING;
  } else {
    game->current->location.y--;
    check_lines_full(game);
    game->state = SPAWN;
  }
  put_block(game, game->current);
}

void move_right_or_left(Game *game, int direction) {
  remove_block(game, game->current);
  game->current->location.x += direction;
  if (!check_block_fits(game, game->current)) {
    game->current->location.x -= direction;
  }
  put_block(game, game->current);
  game->state = MOVING;
}

void move_to_bottom(Game *game) {
  remove_block(game, game->current);
  while (check_block_fits(game, game->current)) {
    game->current->location.y++;
  }
  game->current->location.y--;
  put_block(game, game->current);
  check_lines_full(game);
  game->state = SPAWN;
}

void process_rotation(Game *game) {
  remove_block(game, game->current);
  Tetromino *temp = rotate_block(game->current, game->blocks, 3);
  if (check_block_fits(game, temp)) {
    free(game->current);
    game->current = temp;
  } else {
    free(temp);
  }
  put_block(game, game->current);
}

void check_lines_full(Game *game) {
  int full_line_count = 0;
  for (int i = 0; i < HEIGHT; ++i) {
    int full_line = 1;
    for (int j = 0; j < WIDTH; ++j) {
      if (game->game_info.field[i][j] == 0) {
        full_line = 0;
      }
    }

    if (full_line) {
      full_line_count += 1;
      for (int k = i; k > 0; k--) {
        for (int j = 0; j < WIDTH; j++) {
          game->game_info.field[k][j] = game->game_info.field[k - 1][j];
        }
      }
      for (int j = 0; j < WIDTH; j++) {
        game->game_info.field[0][j] = 0;
      }
      i--;
    }
  }
  if (full_line_count == 1) {
    game->game_info.score += 100;
  } else if (full_line_count == 2) {
    game->game_info.score += 300;
  } else if (full_line_count == 3) {
    game->game_info.score += 500;
  } else if (full_line_count == 4) {
    game->game_info.score += 700;
  }
  game->game_info.level = game->game_info.score / 600 + 1;
  game->game_info.speed = GAME_SPEED * pow(0.8, game->game_info.level);
}

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

void move_figure(Game *game) {
  if (game->new_input) {
    if (game->action == Terminate) {
      game->game_info.pause = 3;
      finish_game(game);
    } else if (game->action == Pause) {
      game->state = PAUSE;
      game->game_info.pause = 1;
    } else if (game->action == Left)
      move_right_or_left(game, -1);
    else if (game->action == Right)
      move_right_or_left(game, 1);
    else if (game->action == Down)
      move_to_bottom(game);
    else if ((game->action == Action || game->action == Up))
      process_rotation(game);
  }
  if (timer(game, game->game_info.speed) && game->game_info.pause != 1)
    game->state = SHIFTING;
  game->new_input = 0;
}

void pause_state(Game *game) {
  if (game->new_input) {
    if (game->action == Pause) {
      game->state = MOVING;
      game->game_info.pause = 0;
    } else if (game->action == Terminate) {
      game->game_info.pause = 3;
      finish_game(game);
    }
  }
  game->new_input = 0;
}

void state_machine(Game *game) {
  if (game->state == START) {
    if (game->new_input) {
      if (game->action == Start) {
        game->game_info.pause = 0;
        init_game(game);
      } else if (game->action == Terminate) {
        game->game_info.pause = 3;
        finish_game(game);
      }
    }
    game->new_input = 0;
  }
  if (game->state == SPAWN) {
    create_new_falling(game);
  } else if (game->state == SHIFTING) {
    move_block_down(game);
  } else if (game->state == MOVING) {
    move_figure(game);
  } else if (game->state == GAME_OVER) {
    game->game_info.pause = 2;
    game->state = START;
  } else if (game->state == PAUSE) {
    pause_state(game);
  }
}

void finish_game(Game *game) {
  if (game->game_info.score > game->game_info.high_score) {
    FILE *file = fopen("high_score.txt", "w");
    if (file) {
      fprintf(file, "%d", game->game_info.score);
      fclose(file);
    }
  }
  for (int i = 0; i < HEIGHT; ++i) {
    free(game->game_info.field[i]);
  }
  free(game->game_info.field);
  for (int i = 0; i < 4; ++i) {
    free(game->game_info.next[i]);
  }
  free(game->game_info.next);
  game->game_info.field = NULL;
  free(game->next);
  free(game->current);
  free(game->blocks);
  game->is_playing = 0;
}