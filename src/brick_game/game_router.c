/* Router that exposes the generic `userInput` and `updateCurrentState`
   symbols expected by frontends and delegates calls to either the Snake or
   Tetris backend selected at runtime. */

#include "common.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
// Declarations for the renamed/back-end specific entry points that exist
// in the wrapper translation units we add to the desktop build.
extern void snake_userInput(UserAction_t action, bool hold);
extern GameInfo_t snake_updateCurrentState(float dt);

extern void tetris_userInput(UserAction_t action, bool hold);
extern GameInfo_t tetris_updateCurrentState(float dt);
#ifdef __cplusplus
}
#endif

static void (*current_userInput)(UserAction_t, bool) = NULL;
static GameInfo_t (*current_update)(float) = NULL;

EXPORT void SelectGame(const char *name) {
  if (name == NULL) name = "snake";
  if (strcmp(name, "tetris") == 0) {
    current_userInput = tetris_userInput;
    current_update = tetris_updateCurrentState;
  } else {
    // default to snake
    current_userInput = snake_userInput;
    current_update = snake_updateCurrentState;
  }
}

EXPORT void userInput(UserAction_t action, bool hold) {
  if (!current_userInput) SelectGame(NULL);
  current_userInput(action, hold);
}

EXPORT GameInfo_t updateCurrentState(float dt) {
  if (!current_update) SelectGame(NULL);
  return current_update(dt);
}
