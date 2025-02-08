#pragma once
#include "../model/game.h"

void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState(float dt);
Game& GetCurrentGameState();