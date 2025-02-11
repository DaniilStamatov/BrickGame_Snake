#include "controller.h"

Game& GetCurrentGameState() {
  static Game game;
  return game;
}

void userInput(UserAction_t action, bool hold) {
  (void)hold;
  Game& s_game = GetCurrentGameState();
  s_game.HandleInput(action);
}

GameInfo_t updateCurrentState(float dt) {
  Game& s_game = GetCurrentGameState();
  s_game.ProcessStateMachine(dt);
  return s_game.GetCurrentGameInfo();
}
