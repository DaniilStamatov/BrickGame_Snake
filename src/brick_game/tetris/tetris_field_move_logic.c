#include "tetris.h"

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
  if (game->game_info.level < 10)
    game->game_info.level = game->game_info.score / 600 + 1;
  game->game_info.speed = GAME_SPEED * pow(0.8, game->game_info.level);
}