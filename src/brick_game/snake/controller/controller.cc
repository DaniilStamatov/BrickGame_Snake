#include "controller.h"

#include "../../common.h"

s21::Game& GetCurrentGameState() {
  static s21::Game game;
  return game;
}

extern "C" {
EXPORT void userInput(UserAction_t action, bool hold) {
  (void)hold;
  s21::Game& game = GetCurrentGameState();
  game.HandleInput(action);
}

EXPORT GameInfo_t updateCurrentState(float dt) {
  s21::Game& game = GetCurrentGameState();
  game.ProcessStateMachine(dt);
  return game.GetCurrentGameInfo();
}
}  // extern "C"
