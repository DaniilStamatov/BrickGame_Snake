#pragma once
#define HEIGHT 20
#define WIDTH 10
#define FIGURE_HEIGHT 4
#define SCALE (HEIGHT * WIDTH)

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>

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

EXPORT void userInput(UserAction_t action, bool hold);
EXPORT GameInfo_t updateCurrentState(float dt);

#ifdef __cplusplus
}
#endif