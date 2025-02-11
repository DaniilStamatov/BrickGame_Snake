#include <gtest/gtest.h>

#include "../../brick_game/snake/controller/controller.h"

TEST(snake_test, simple) {
  Game game = GetCurrentGameState();
  game.InitGame();

  EXPECT_EQ(game.GetSnakeHeadPosition().x, WIDTH / 2 - 1);
  EXPECT_EQ(game.GetSnakeHeadPosition().y, HEIGHT / 2 - 1);
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
  EXPECT_EQ(game.GetSnakeHeadPosition().x, WIDTH / 2 - 1);
  EXPECT_EQ(game.GetSnakeHeadPosition().y, HEIGHT / 2);
  game.HandleInput(Up);
  game.ProcessStateMachine(1100);
  EXPECT_EQ(game.GetSnakeHeadPosition().x, WIDTH / 2 - 1);
  EXPECT_EQ(game.GetSnakeHeadPosition().y, HEIGHT / 2 - 1);
  game.HandleInput(Pause);
  game.ProcessStateMachine(1100);
  GameInfo_t gameInfo = game.GetCurrentGameInfo();
  EXPECT_EQ(gameInfo.pause, PAUSE);
  game.HandleInput(Pause);
  game.ProcessStateMachine(1100);
  gameInfo = game.GetCurrentGameInfo();
  EXPECT_EQ(gameInfo.pause, PLAYING);
  GameInfo_t gameInfo2 = updateCurrentState(0);

  EXPECT_EQ(gameInfo2.pause, PLAYING);
  userInput(Terminate, 0);
  game.HandleInput(Terminate);
  game.ProcessStateMachine(1100);
  gameInfo = game.GetCurrentGameInfo();
  EXPECT_EQ(gameInfo.pause, TERMINATE);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
