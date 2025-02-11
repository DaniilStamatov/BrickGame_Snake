#pragma once

#include <random>
#include "snake.h"
#define FRUIT_COLOR 2
#define HEAD_COLOR 4
#define BODY_COLOR 1
class Game {
public:
    Game();
    void InitGame();
    ~Game();
    void ProcessStateMachine(float dt);
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
    void HandleInput(UserAction_t action);
    GameInfo_t GetCurrentGameInfo() const;
    Position GetSnakeHeadPosition() const;
private:
    enum class State {
        MOVING, 
        EATING,
        PAUSED,
        SPAWNING,
        GAME_LOST,
        START,
    };
    State m_state;
    const static int s_speedFactor = 1000;
    GameInfo_t m_gameInfo;
    Snake m_snake;
    Position m_fruitLocation;
    float m_updateTime = 0;
};


