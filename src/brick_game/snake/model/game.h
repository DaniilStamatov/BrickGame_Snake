#pragma once

#include <random>
#include "snake.h"
#define TERMINATE 3
#define GAME_OVER 2
#define PAUSE 1
#define PLAYING 0
#define FRUIT_COLOR 2
#define HEAD_COLOR 4
#define BODY_COLOR 1
class Game {
    
public:
struct Position {
    int x, y;
};
    Game();
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
    GameInfo_t GetCurrentGameInfo() const;
private:
    enum class State {
        MOVING, 
        EATING,
        PAUSED,
        SPAWNING
    };
    State m_state;
    const static int s_speedFactor = 100;
    GameInfo_t m_gameInfo;
    Snake m_snake;
    Position m_fruitLocation;
    float m_updateTime = 0;
};


