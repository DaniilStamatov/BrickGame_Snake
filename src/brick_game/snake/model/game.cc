#include "game.h"

#include <algorithm>

constexpr int kFruitColor = 2;
constexpr int kHeadColor = 4;
constexpr int kBodyColor = 1;

namespace s21 {

Game::Game() {
  current_state_ = State::START;
  game_info_.field = nullptr;
  normal_speed_ = 0;
  fast_mode_ = false;
}

void Game::InitGame() {
  if (!game_info_.field) {
    game_info_.field = new int *[HEIGHT];
    for (int i = 0; i < HEIGHT; ++i) {
      game_info_.field[i] = new int[WIDTH];
    }
  }
  ClearField();
  game_info_.next = nullptr;
  game_info_.score = 0;
  game_info_.level = 1;
  game_info_.speed = kSpeedFactor * pow(0.8, game_info_.level);
  // store normal speed for action toggle
  normal_speed_ = game_info_.speed;
  fast_mode_ = false;
  game_info_.pause = PauseState::PLAYING;
  FILE *file = fopen("snake_high_score.txt", "r");
  if (file) {
    fscanf(file, "%d", &game_info_.high_score);
    fclose(file);
  }
  current_state_ = State::MOVING;
  snake_.Clear();
  update_time_ = 0.0f;
  SpawnFruit();
}

Game::~Game() {
  if (game_info_.field) {
    for (int i = 0; i < HEIGHT; ++i) {
      delete[] game_info_.field[i];
    }
    delete[] game_info_.field;
    game_info_.field = nullptr;
  }
}

void Game::ProcessStateMachine(float dt) {
  if (current_state_ != State::START && game_info_.pause != TERMINATE) {
    if (snake_.GetLength() >= SCALE) {
      current_state_ = State::GAME_WON;
    }
    if (current_state_ == State::MOVING) {
      MoveSnake(dt);
    } else if (current_state_ == State::SPAWNING) {
      SpawnFruit();
    } else if (current_state_ == State::EATING) {
      snake_.AddPart();
      game_info_.score++;
      if (game_info_.high_score < game_info_.score) {
        game_info_.high_score = game_info_.score;
      }
      if (game_info_.level < 10) {
        game_info_.level = game_info_.score / 5 + 1;
      } else {
        game_info_.level = 10;
      }
      game_info_.speed = kSpeedFactor * pow(0.8, game_info_.level);
      current_state_ = State::SPAWNING;
      if (snake_.GetLength() >= SCALE) {
        current_state_ = State::GAME_WON;
      }
    }
    if (current_state_ != State::GAME_LOST &&
        current_state_ != State::GAME_WON) {
      ClearField();
      UpdateSnakePosition();
      UpdateFruitPosition();
    }
    if (current_state_ == State::GAME_LOST ||
        current_state_ == State::GAME_WON) {
      game_info_.pause = PauseState::GAME_OVER;
      FILE *file = fopen("snake_high_score.txt", "w");
      if (file) {
        fprintf(file, "%d", game_info_.score);
        fclose(file);
      }
    }
  }
}

void Game::HandleInput(UserAction_t action) {
  switch (action) {
    case UserAction_t::Up:
      SetSnakeDireciton(Snake::Direction::kUp);
      break;
    case UserAction_t::Down:
      SetSnakeDireciton(Snake::Direction::kDown);
      break;
    case UserAction_t::Left:
      SetSnakeDireciton(Snake::Direction::kLeft);
      break;
    case UserAction_t::Right:
      SetSnakeDireciton(Snake::Direction::kRight);
      break;
    case UserAction_t::Pause:
      TogglePause();
      break;
    case UserAction_t::Start:
      ProcessRestart();
      break;
    case UserAction_t::Terminate:
      QuitGame();
      break;
    case UserAction_t::Action:
      if (!fast_mode_) {
        normal_speed_ = game_info_.speed;
        game_info_.speed = std::max(1, game_info_.speed / 2);
        fast_mode_ = true;
      } else {
        // restore
        game_info_.speed = normal_speed_ ? normal_speed_ : game_info_.speed;
        fast_mode_ = false;
      }
      break;
    default:
      break;
  }
}

void Game::MoveSnake(float dt) {
  update_time_ += dt;
  bool moved = false;
  if (update_time_ >= game_info_.speed) {
    snake_.Move();
    moved = true;
    update_time_ = 0;
  }
  if (moved && snake_.IsColliding()) current_state_ = State::GAME_LOST;
  if (IsCollidingFruit()) {
    current_state_ = State::EATING;
  }
  if (snake_.GetLength() >= SCALE) {
    current_state_ = State::GAME_WON;
  }
}

void Game::SpawnFruit() {
  Position fruitPosition;
  do {
    fruitPosition.x = rand() % WIDTH;
    fruitPosition.y = rand() % HEIGHT;
  } while (!IsPositionValid(fruitPosition));
  fruit_location_ = fruitPosition;
  current_state_ = State::MOVING;
}

bool Game::IsPositionValid(const Position &pos) const {
  std::array<Position, SCALE> snakeBody = snake_.GetBody();
  for (int i = 0; i < snake_.GetLength(); ++i) {
    if (snakeBody[i].x == pos.x && snakeBody[i].y == pos.y) {
      return false;
    }
  }
  return true;
}

void Game::UpdateFruitPosition() {
  game_info_.field[fruit_location_.y][fruit_location_.x] = kFruitColor;
}

void Game::UpdateSnakePosition() {
  std::array<Position, SCALE> snakeBody = snake_.GetBody();
  game_info_.field[snake_.GetHead().y][snake_.GetHead().x] = kHeadColor;
  for (int i = 1; i < snake_.GetLength(); ++i) {
    int x = snakeBody[i].x;
    int y = snakeBody[i].y;
    game_info_.field[y][x] = kBodyColor;
  }
}

void Game::ClearField() {
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      game_info_.field[i][j] = 0;
    }
  }
}

bool Game::IsCollidingFruit() const {
  return snake_.GetHead().x == fruit_location_.x &&
         snake_.GetHead().y == fruit_location_.y;
}

void Game::SetSnakeDireciton(Snake::Direction new_dir) {
  snake_.SetDirection(new_dir);
}

void Game::TogglePause() {
  if (current_state_ != State::START && current_state_ != State::GAME_LOST) {
    if (game_info_.pause == PAUSE) {
      game_info_.pause = PLAYING;
      current_state_ = State::MOVING;
    } else {
      game_info_.pause = PAUSE;
      current_state_ = State::PAUSED;
    }
  }
}

void Game::QuitGame() {
  game_info_.pause = PauseState::TERMINATE;
  FILE *file = fopen("snake_high_score.txt", "w");
  if (file) {
    fprintf(file, "%d", game_info_.score);
    fclose(file);
  }
}

void Game::ProcessRestart() {
  if (current_state_ == State::GAME_LOST || current_state_ == State::START) {
    InitGame();
  }
}

GameInfo_t Game::GetCurrentGameInfo() const { return game_info_; }

Position Game::GetSnakeHeadPosition() const { return snake_.GetHead(); }

int Game::GetSnakeLength() const { return snake_.GetLength(); }

void Game::DebugGrowSnake(int times) {
  for (int i = 0; i < times; ++i) {
    snake_.AddPart();
  }
}

}  // namespace s21
