#pragma once

#include <memory>
#include <random>

#include "snake.h"

/**
 * @file game.h
 * @brief Main game controller for the Snake game.
 *
 * The `Game` class implements the high-level state machine for the Snake
 * game: initialization, the main update loop, input handling, score and
 * level management, fruit spawning and high-score persistence.
 */

/**
 * @class Game
 * @brief Encapsulates game state and behavior for a single Snake game session.
 *
 * Public methods allow starting/restarting the game, processing time steps,
 * and handling user actions. Internally the class owns a `Snake` instance,
 * the play field, current score/level and the fruit location.
 */
namespace s21 {
class Game {
 public:
  Game();
  ~Game();

  void InitGame();
  void ProcessStateMachine(float dt);
  void HandleInput(UserAction_t action);
  GameInfo_t GetCurrentGameInfo() const;
  Position GetSnakeHeadPosition() const;
  int GetSnakeLength() const;
  void DebugGrowSnake(int times);

 private:
  void SpawnFruit();
  bool IsPositionValid(const Position &pos) const;
  void MoveSnake(float dt);
  void UpdateFruitPosition();
  void UpdateSnakePosition();
  void ClearField();
  bool IsCollidingFruit() const;
  void SetSnakeDireciton(Snake::Direction new_dir);
  void TogglePause();
  void QuitGame();
  void ProcessRestart();

  enum class State {
    MOVING,
    EATING,
    PAUSED,
    SPAWNING,
    GAME_LOST,
    GAME_WON,
    START,
  };

  const static int kSpeedFactor = 1000;

  State current_state_;
  GameInfo_t game_info_;
  Snake snake_;
  Position fruit_location_;
  float update_time_ = 0.0f;
  int normal_speed_ = 0;
  bool fast_mode_ = false;
};

}  // namespace s21
