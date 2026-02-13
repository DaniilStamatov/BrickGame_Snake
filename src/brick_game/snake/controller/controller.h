#pragma once
#include "../model/game.h"

/**
 * @file controller.h
 * @brief Controller API for accessing the current Snake game instance.
 *
 * The controller exposes a very small interface used by frontends to obtain
 * a reference to the current `Game` instance for rendering or input mapping.
 */

/**
 * @brief Returns the global/current `Game` instance used by the controller.
 * @return Reference to the active `Game` object.
 */
namespace s21 {
class Game;
}

s21::Game& GetCurrentGameState();