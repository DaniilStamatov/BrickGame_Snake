#include <gtest/gtest.h>

#include "../../brick_game/snake/controller/controller.h"

TEST(snake_test, simple) {
  s21::Game &game = GetCurrentGameState();
  game.InitGame();

  EXPECT_EQ(game.GetSnakeHeadPosition().x, WIDTH / 2 - 1);
  EXPECT_EQ(game.GetSnakeHeadPosition().y, HEIGHT / 2 - 1);

  EXPECT_EQ(game.GetSnakeLength(), 4);

  game.HandleInput(Down);
  game.ProcessStateMachine(1100);
  EXPECT_EQ(game.GetSnakeHeadPosition().x, WIDTH / 2 - 1);
  EXPECT_EQ(game.GetSnakeHeadPosition().y, HEIGHT / 2);

  game.HandleInput(Left);
  game.ProcessStateMachine(1100);
  EXPECT_EQ(game.GetSnakeHeadPosition().x, WIDTH / 2 - 2);
  EXPECT_EQ(game.GetSnakeHeadPosition().y, HEIGHT / 2);

  game.HandleInput(Right);
  game.ProcessStateMachine(1100);
  EXPECT_EQ(game.GetSnakeHeadPosition().x, WIDTH / 2 - 3);

  game.HandleInput(Pause);
  game.ProcessStateMachine(1100);
  GameInfo_t gameInfo = game.GetCurrentGameInfo();
  EXPECT_EQ(gameInfo.pause, PAUSE);
  game.HandleInput(Pause);
  game.ProcessStateMachine(1100);
  gameInfo = game.GetCurrentGameInfo();
  EXPECT_EQ(gameInfo.pause, PLAYING);

  int before_speed = game.GetCurrentGameInfo().speed;
  game.HandleInput(Action);
  int after_speed = game.GetCurrentGameInfo().speed;
  EXPECT_LT(after_speed, before_speed);

  GameInfo_t gameInfo2 = updateCurrentState(0);
  EXPECT_EQ(gameInfo2.pause, PLAYING);
  userInput(Terminate, 0);
  game.HandleInput(Terminate);
  game.ProcessStateMachine(1100);
  gameInfo = game.GetCurrentGameInfo();
  EXPECT_EQ(gameInfo.pause, TERMINATE);
}

TEST(snake_test, win_condition_and_grow) {
  s21::Game &game = GetCurrentGameState();
  game.InitGame();
  int to_grow = SCALE - game.GetSnakeLength();
  for (int i = 0; i < to_grow; ++i) {
    game.DebugGrowSnake(1);
  }
  EXPECT_EQ(game.GetSnakeLength(), SCALE);
  game.ProcessStateMachine(0);
  GameInfo_t gi = game.GetCurrentGameInfo();
  EXPECT_EQ(gi.pause, GAME_OVER);
}
