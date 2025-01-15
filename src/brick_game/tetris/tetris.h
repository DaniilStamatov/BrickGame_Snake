#ifndef TETRIS_H
#define TETRIS_H

#define WIDTH 10
#define HEIGHT 20
#define MAX_LEVEL 10
#define GAME_SPEED 500
#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef enum state {
  START,
  SPAWN,
  MOVING,
  SHIFTING,
  ATTACHING,
  GAME_OVER,
  PAUSE
} state;

typedef struct position {
  int x, y;
} position;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef enum TetrisType { I, L, J, O, S, T, Z } TetrisType;

typedef struct {
  TetrisType type;
  position states[4][4];
} TetrominoMap;

typedef struct {
  TetrisType type;
  int orientation;
  position location;
  position state[4];
} Tetromino;

typedef struct {
  int **field;
  int **next;
  int high_score;
  int score;
  int level;
  int pause;
  int speed;
} GameInfo_t;

typedef struct {
  int is_playing;
  int cleared;
  int new_input;
  Tetromino *next;
  Tetromino *current;
  state state;
  long long time;
  GameInfo_t game_info;
  UserAction_t action;
  TetrominoMap *blocks;
} Game;

void fill_next_func(GameInfo_t *game_info, Tetromino *next);
void userInput(UserAction_t user_input, bool hold);
GameInfo_t updateCurrentState();
Game *get_current_game_info();
void init_game(Game *game);
GameInfo_t init_game_info();
void check_lines_full(Game *game);
void process_rotation(Game *game);
void move_right_or_left(Game *game, int direction);
void move_block_down(Game *game);
void create_new_falling(Game *game);
void fill_current_from_next(Game *game);
int **create_matrix(int rows, int columns);
void put_block(Game *game, Tetromino *block);
void remove_block(Game *game, Tetromino *block);
void state_machine(Game *game);
void move_figure(Game *game);
void finish_game(Game *game);
void move_to_bottom(Game *game);
void process_input(int c);
int timer(Game *gs, int delay);
long long get_time();

Tetromino *rotate_block(
    const Tetromino *block, TetrominoMap *blocks,
    int shift);  // shift param is for rotation(3-right, 1-left)
int check_block_fits(const Game *game, Tetromino *block);
void init_new_map_block(TetrominoMap *blocks, int type);
TetrominoMap *init_all_block_types();
void init_new_block(TetrominoMap *blocks, int type, Game *game);
void init_block_I(TetrominoMap *tetris);
void init_block_L(TetrominoMap *tetris);
void init_block_J(TetrominoMap *tetris);
void init_block_O(TetrominoMap *tetris);
void init_block_S(TetrominoMap *tetris);
void init_block_T(TetrominoMap *tetris);
void init_block_Z(TetrominoMap *tetris);
void fill_block_matrix(TetrominoMap *tetris, position states[4][4]);

#endif