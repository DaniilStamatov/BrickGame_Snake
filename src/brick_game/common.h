#pragma once
#define HEIGHT 20
#define WIDTH 10
#define SCALE (HEIGHT * WIDTH)

typedef enum {
    PLAYING = 0,
    PAUSE = 1,
    GAME_OVER = 2,
    TERMINATE = 3
} PauseState;

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

typedef struct {
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
} GameInfo_t;
