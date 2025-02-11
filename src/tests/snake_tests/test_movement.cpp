#include <gtest/gtest.h>
#include "../../brick_game/snake/controller/controller.h"

TEST(snake_test, simple) {
    Game game = GetCurrentGameState();
    game.InitGame();

    EXPECT_EQ(game.GetSnakeHeadPosition().x, WIDTH / 2 -1);
    EXPECT_EQ(game.GetSnakeHeadPosition().y, HEIGHT / 2 -1);
    game.SetSnakeDireciton(Snake::Direction::Down);
    game.ProcessStateMachine(1100);
    EXPECT_EQ(game.GetSnakeHeadPosition().x, WIDTH / 2 -1);
    EXPECT_EQ(game.GetSnakeHeadPosition().y, HEIGHT / 2);
    game.SetSnakeDireciton(Snake::Direction::Left);
    game.ProcessStateMachine(1100);
    EXPECT_EQ(game.GetSnakeHeadPosition().x, WIDTH / 2 - 2);
    EXPECT_EQ(game.GetSnakeHeadPosition().y, HEIGHT / 2);
    game.SetSnakeDireciton(Snake::Direction::Right);
    game.ProcessStateMachine(1100);
    EXPECT_EQ(game.GetSnakeHeadPosition().x, WIDTH / 2 - 1);
    EXPECT_EQ(game.GetSnakeHeadPosition().y, HEIGHT / 2);
     game.SetSnakeDireciton(Snake::Direction::Up);
    game.ProcessStateMachine(1100);
    EXPECT_EQ(game.GetSnakeHeadPosition().x, WIDTH / 2 - 1);
    EXPECT_EQ(game.GetSnakeHeadPosition().y, HEIGHT / 2 - 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
