#pragma once
#include "../model/game.h"

inline void userInput(UserAction_t action, bool hold);
inline GameInfo_t updateCurrentState(float dt);
inline Game& GetCurrentGameState();

