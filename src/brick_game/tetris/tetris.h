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

/**
 * @file tetris_game.h
 * @brief Tetris Game Documentation
 *
 * This project implements a classic Tetris game using C and the NCurses
 * library. The game allows players to control falling tetrominoes, clearing
 * lines and scoring points. The project includes functionality for game
 * initialization, user input handling, collision detection, and score
 * management.
 */

/**
 * @mainpage Tetris Game Documentation
 *
 * ## Overview
 * This project implements a classic Tetris game using C and the NCurses
 * library. The game allows players to control falling tetrominoes, clearing
 * lines and scoring points. The project includes functionality for game
 * initialization, user input handling, collision detection, and score
 * management.
 *
 * ## Table of Contents
 * - [Features](#features)
 * - [Installation](#installation)
 * - [Usage](#usage)
 * - [Game Controls](#game-controls)
 * - [Function Descriptions](#function-descriptions)
 * - [Testing](#testing)
 */

/**
 * @section features Features
 * - Classic Tetris gameplay
 * - Various tetromino shapes
 * - High score tracking
 * - User controls for piece movement and rotation
 * - Line clearing and score management
 */

/**
 * @section installation Installation
 *
 * ### Prerequisites
 * - GCC (GNU Compiler Collection)
 * - NCurses library
 * - Check library (for unit testing)
 * - Make
 *
 * ### Steps
 * 1. Install the NCurses, GCC compiler, Make and Check libraries (if not
 * already installed).
 * 2. Compile the game using makefile (src folder):
 *    ```
 *    make install
 *    ```
 */

/**
 * @section usage Usage
 * Run the game using the following command:
 * ```
 * ./build/tetris
 * ```
 */

/**
 * @section game-controls Game Controls
 * - **Arrow Left:** Move piece left
 * - **Arrow Right:** Move piece right
 * - **Arrow Down:** Drop piece
 * - **Space:** Rotate piece
 * - **P:** Pause/Resume the game
 * - **Q:** Quit the game
 */

/**
 * @section function-descriptions Function Descriptions
 */

/**
 * @enum state
 * @brief Represents the different states of the game.
 */
typedef enum state {
  START,      ///< The initial state of the game.
  SPAWN,      ///< The state when a new tetromino is spawned.
  MOVING,     ///< The state when the tetromino is moving.
  SHIFTING,   ///< The state when the tetromino is shifting.
  GAME_OVER,  ///< The state when the game is over.
  PAUSE       ///< The state when the game is paused.
} state;

/**
 * @struct position
 * @brief Represents a position on the game field.
 */
typedef struct position {
  int x;  ///< The x-coordinate of the position.
  int y;  ///< The y-coordinate of the position.
} position;

/**
 * @enum UserAction_t
 * @brief Represents user actions in the game.
 */
typedef enum {
  Start,      ///< Action to start the game.
  Pause,      ///< Action to pause the game.
  Terminate,  ///< Action to terminate the game.
  Left,       ///< Action to move the tetromino left.
  Right,      ///< Action to move the tetromino right.
  Up,         ///< Action to move the tetromino up.
  Down,       ///< Action to move the tetromino down.
  Action      ///< Action to perform a specific game action (e.g., rotate).
} UserAction_t;

/**
 * @enum TetrisType
 * @brief Represents the different types of tetrominoes.
 */
typedef enum TetrisType {
  I,  ///< I-shaped tetromino.
  L,  ///< L-shaped tetromino.
  J,  ///< J-shaped tetromino.
  O,  ///< O-shaped tetromino.
  S,  ///< S-shaped tetromino.
  T,  ///< T-shaped tetromino.
  Z   ///< Z-shaped tetromino.
} TetrisType;

/**
 * @struct TetrominoMap
 * @brief Represents the mapping of a tetromino type to its rotation states.
 */
typedef struct {
  TetrisType type;        ///< The type of the tetromino.
  position states[4][4];  ///< The rotation states of the tetromino.
} TetrominoMap;

/**
 * @struct Tetromino
 * @brief Represents a tetromino piece in the game.
 */
typedef struct {
  TetrisType type;  ///< The type of the tetromino.
  int orientation;  ///< The current orientation of the tetromino (0-3 for 0°,
                    ///< 90°, 180°, 270°).
  position
      location;  ///< The current location of the tetromino on the game field.
  position state[4];  ///< The positions of the tetromino's blocks in its
                      ///< current orientation.
} Tetromino;

/**
 * @struct GameInfo_t
 * @brief Contains information about the current game state.
 */
typedef struct {
  int **field;     ///< The game field represented as a 2D array of integers.
  int **next;      ///< The next tetromino to be spawned.
  int high_score;  ///< The highest score achieved in the game.
  int score;       ///< The current score of the player.
  int level;       ///< The current level of the game.
  int pause;  ///< Flag indicating whether the game is paused (1 for paused, 0
              ///< for not).
  int speed;  ///< The speed of the game (e.g., how fast tetrominoes fall).
} GameInfo_t;

/**
 * @struct Game
 * @brief Represents the current state of the game.
 */
typedef struct {
  int is_playing;  ///< Flag indicating whether the game is currently being
                   ///< played (1 for playing, 0 for not).
  int cleared;     ///< The number of lines cleared in the current game.
  int new_input;   ///< Flag indicating whether new user input has been received
                   ///< (1 for yes, 0 for no).
  Tetromino *next;       ///< Pointer to the next tetromino to be spawned.
  Tetromino *current;    ///< Pointer to the currently active tetromino.
  state state;           ///< The current state of the game.
  long long time;        ///< The elapsed time since the game started.
  GameInfo_t game_info;  ///< The game information structure containing various
                         ///< game stats.
  UserAction_t action;   ///< The current user action being processed.
  TetrominoMap *blocks;  ///< Pointer to the tetromino map
} Game;

/// @file
/// @brief Implementation of functions from brickgame.h
/// @details This file contains realization of user actions, initialization of
/**
 * @brief Initializes the game state, including the game field and the first
 * tetromino pieces.
 */
void init_game(Game *game);
/**
 * @brief Handles user input to control the game actions based on the specified
 * action.
 * @param action The user action to be processed.
 * @param hold Indicates whether the action is a hold action.
 */
void userInput(UserAction_t action, bool hold);

/**
 * @brief Updates the current game state, including moving the tetromino down
 * automatically based on the game speed.
 * @return The updated game information.
 */
GameInfo_t updateCurrentState();

/**
 * @brief Processes user input from the keyboard, translating key presses into
 * game actions.
 * @param c The character input from the user.
 */
void process_input(int c);

/**
 * @brief Generates a new tetromino piece and checks for game over conditions.
 * @param game Pointer to the current game instance.
 */
void create_new_falling(Game *game);

/**
 * @brief Fills the next tetromino piece to be displayed on the game interface.
 * @param game_info Pointer to the game information structure.
 * @param next Pointer to the next tetromino piece.
 */
void fill_next_func(GameInfo_t *game_info, Tetromino *next);

/**
 * @brief Retrieves the current game information, including the state of the
 * game field and the active tetromino.
 * @return Pointer to the current game information.
 */
Game *get_current_game_info();

/**
 * @brief Initializes the game information structure, setting up the initial
 * state of the game.
 * @return The initialized game information.
 */
GameInfo_t init_game_info();

/**
 * @brief Checks for any completed lines in the game field and removes them.
 * @param game Pointer to the current game instance.
 */
void check_lines_full(Game *game);

/**
 * @brief Handles the rotation of the current tetromino piece.
 * @param game Pointer to the current game instance.
 */
void process_rotation(Game *game);
/**
 * @brief Moves the current tetromino piece to the right or left based on the
 * specified direction.
 * @param game Pointer to the current game instance.
 * @param direction Direction to move (1 for right, -1 for left).
 */
void move_right_or_left(Game *game, int direction);
/**
 * @brief Moves the current tetromino piece down by one row in the game field.
 * @param game Pointer to the current game instance.
 */
void move_block_down(Game *game);

/**
 * @brief Transfers the next tetromino piece into the current active piece.
 * @param game Pointer to the current game instance.
 */
void fill_current_from_next(Game *game);
/**
 * @brief Creates a 2D matrix of integers with the specified number of rows and
 * columns.
 * @param rows Number of rows in the matrix.
 * @param columns Number of columns in the matrix.
 * @return Pointer to the created matrix.
 */
int **create_matrix(int rows, int columns);

/**
 * @brief Places the specified tetromino block onto the game field.
 * @param game Pointer to the current game instance.
 * @param block Pointer to the tetromino block to place.
 */
void put_block(Game *game, Tetromino *block);

/**
 * @brief Removes the specified tetromino block from the game field.
 * @param game Pointer to the current game instance.
 * @param block Pointer to the tetromino block to remove.
 */
void remove_block(Game *game, Tetromino *block);

/**
 * @brief Implements the game state machine, managing transitions between
 * different game states.
 * @param game Pointer to the current game instance.
 */
void state_machine(Game *game);

/**
 * @brief Handles the movement of the current tetromino piece based on user
 * input and game logic.
 * @param game Pointer to the current game instance.
 */
void move_figure(Game *game);

/**
 * @brief Finalizes the game state, performing any necessary cleanup and
 * displaying the final score.
 * @param game Pointer to the current game instance.
 */
void finish_game(Game *game);

/**
 * @brief Moves the current tetromino piece down to the lowest possible position
 * in the game field.
 * @param game Pointer to the current game instance.
 */
void move_to_bottom(Game *game);

/**
 * @brief Implements a timer function that manages the game speed.
 * @param gs Pointer to the current game instance.
 * @param delay Delay in milliseconds for the timer.
 * @return The elapsed time.
 */
int timer(Game *gs, int delay);

/**
 * @brief Frees the memory allocated for a 2D matrix of integers.
 * @param matrix Pointer to the matrix to free.
 * @param rows Number of rows in the matrix.
 */
void free_matrix(int **matrix, int rows);

/**
 * @brief Retrieves the current time in milliseconds.
 * @return The current time in milliseconds.
 */
long long get_time();

/**
 * @brief Rotates the specified tetromino block based on the shift parameter.
 * @param block Pointer to the tetromino block to rotate.
 * @param blocks Pointer to the tetromino map.
 * @param shift Parameter for rotation (3 for right, 1 for left).
 * @return Pointer to the rotated tetromino block.
 */
Tetromino *rotate_block(const Tetromino *block, TetrominoMap *blocks,
                        int shift);

/**
 * @brief Checks if the current piece collides with the game field or
 * boundaries.
 * @param game Pointer to the current game instance.
 * @param block Pointer to the tetromino block to check.
 * @return 1 if the block fits, 0 otherwise.
 */
int check_block_fits(const Game *game, Tetromino *block);

/**
 * @brief Initializes a new block in the tetromino map based on the specified
 * type.
 * @param blocks Pointer to the tetromino map.
 * @param type The type of the block to initialize.
 */
void init_new_map_block(TetrominoMap *blocks, int type);

/**
 * @brief Initializes all types of tetromino blocks.
 * @return Pointer to the initialized tetromino map.
 */
TetrominoMap *init_all_block_types();

/**
 * @brief Initializes a new tetromino block of the specified type.
 * @param blocks Pointer to the tetromino map.
 * @param type The type of the block to initialize.
 * @param game Pointer to the current game instance.
 */
void init_new_block(TetrominoMap *blocks, int type, Game *game);

/**
 * @brief Initializes the I-shaped tetromino block.
 * @param tetris Pointer to the tetromino map.
 */
void init_block_I(TetrominoMap *tetris);

/**
 * @brief Initializes the L-shaped tetromino block.
 * @param tetris Pointer to the tetromino map.
 */
void init_block_L(TetrominoMap *tetris);

/**
 * @brief Initializes the J-shaped tetromino block.
 * @param tetris Pointer to the tetromino map.
 */
void init_block_J(TetrominoMap *tetris);

/**
 * @brief Initializes the O-shaped tetromino block.
 * @param tetris Pointer to the tetromino map.
 */
void init_block_O(TetrominoMap *tetris);

/**
 * @brief Initializes the S-shaped tetromino block.
 * @param tetris Pointer to the tetromino map.
 */
void init_block_S(TetrominoMap *tetris);

/**
 * @brief Initializes the T-shaped tetromino block.
 * @param tetris Pointer to the tetromino map.
 */
void init_block_T(TetrominoMap *tetris);

/**
 * @brief Initializes the Z-shaped tetromino block.
 * @param tetris Pointer to the tetromino map.
 */
void init_block_Z(TetrominoMap *tetris);

/**
 * @brief Fills the block matrix for a given tetromino type with its rotation
 * states.
 * @param tetris Pointer to the tetromino map.
 * @param states The rotation states of the tetromino.
 */
void fill_block_matrix(TetrominoMap *tetris, position states[4][4]);

#endif