#include "controller.h"

Game& GetCurrentGameState() {
    static Game game;
    return game;
}

void userInput(UserAction_t action, bool hold)
{
    (void)hold;
    Game& s_game = GetCurrentGameState();    
    switch (action)
    {
    case UserAction_t::Up:
        s_game.SetSnakeDireciton(Snake::Direction::Up);
        break;  
    case UserAction_t::Down:
        s_game.SetSnakeDireciton(Snake::Direction::Down);
        break;
    case UserAction_t::Left:
        s_game.SetSnakeDireciton(Snake::Direction::Left);
        break;
    case UserAction_t::Right:
        s_game.SetSnakeDireciton(Snake::Direction::Right);
        break;
    case UserAction_t::Pause:
        s_game.TogglePause();
        break;
    case UserAction_t::Start:
            // Обработка действия Start
        break;
    case UserAction_t::Terminate:
        s_game.QuitGame();
        // Обработка действия Terminate
        break;
    case UserAction_t::Action:
        // Обработка действия Action
        break;
    default:
        // Обработка неизвестного действия (если необходимо)
        break;
    }
}

// GameInfo_t HandleInput(UserAction_t action, bool hold) {
//     userInput(action, hold);
//     return updateCurrentState();
// }

GameInfo_t updateCurrentState(float dt)
{
    Game& s_game = GetCurrentGameState();
    s_game.ProcessStateMachine(dt);

    return s_game.GetCurrentGameInfo();
}
