#include "game.h"
Game::Game() {
  m_state = State::START;
  m_gameInfo.field = nullptr;
}

void Game::InitGame()
{
  if(!m_gameInfo.field){
    m_gameInfo.field = new int *[HEIGHT];
    for (int i = 0; i < HEIGHT; ++i) {
      m_gameInfo.field[i] = new int[WIDTH];
    }
  }
  ClearField();
  m_gameInfo.next = nullptr;
  m_gameInfo.score = 0;
  m_gameInfo.level = 1;
  m_gameInfo.speed = s_speedFactor * pow(0.8, m_gameInfo.level);
  m_gameInfo.pause = PauseState::PLAYING;
  FILE *file = fopen("high_score.txt", "r");
  if (file) {
    fscanf(file, "%d", &m_gameInfo.high_score);
    fclose(file);
  }
  m_state = State::MOVING;
  m_snake.ClearSnake();
  SpawnFruit();
}

Game::~Game() {
  if (m_gameInfo.field) {
    for (int i = 0; i < HEIGHT; ++i) {
        delete[] m_gameInfo.field[i];
    }
    delete[] m_gameInfo.field;
    m_gameInfo.field = nullptr;
  }
 
}

void Game::ProcessStateMachine(float dt) {
  if(m_state != State::START && m_gameInfo.pause != TERMINATE) {
    if(m_state == State::MOVING) {
      MoveSnake(dt);  
    } else if(m_state == State::SPAWNING) {
      SpawnFruit();
    } else if(m_state == State::EATING) {
      m_snake.AddPart();
      m_gameInfo.score++;
      if(m_gameInfo.high_score < m_gameInfo.score) {
        m_gameInfo.high_score = m_gameInfo.score;
      }
      m_gameInfo.level = m_gameInfo.score / 5 + 1;
      m_gameInfo.speed = s_speedFactor * pow(0.8, m_gameInfo.level);
      m_state = State::SPAWNING;
    }
    if(m_state != State::GAME_LOST) {
      ClearField();
      UpdateSnakePosition();
      UpdateFruitPosition();
    }
    if(m_state == State::GAME_LOST) {
      m_gameInfo.pause = PauseState::GAME_OVER;
       FILE *file = fopen("high_score.txt", "w");
        if (file) {
          fprintf(file, "%d", m_gameInfo.score);
          fclose(file);
        }
    }
  }

}

void Game::HandleInput(UserAction_t action) {
    switch (action)
    {
    case UserAction_t::Up:
        SetSnakeDireciton(Snake::Direction::Up);
        break;
    case UserAction_t::Down:
        SetSnakeDireciton(Snake::Direction::Down);
        break;
    case UserAction_t::Left:
        SetSnakeDireciton(Snake::Direction::Left);
        break;
    case UserAction_t::Right:
        SetSnakeDireciton(Snake::Direction::Right);
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
        break;
    default:
        break;
    }
}

void Game::MoveSnake(float dt) {
  m_updateTime += dt;
  if(m_updateTime >= m_gameInfo.speed) {
    m_snake.Move();
    m_updateTime = 0;
  }
  if(m_snake.IsColliding()) m_state = State::GAME_LOST;
  if(IsCollidingFruit()) {
    m_state = State::EATING;
  }
}

void Game::SpawnFruit() {
  Position fruitPosition;
  do {
    fruitPosition.x = rand() % WIDTH;
    fruitPosition.y = rand() % HEIGHT;
  } while (!IsPositionValid(fruitPosition));
  m_fruitLocation = fruitPosition;
  m_state = State::MOVING;
}

bool Game::IsPositionValid(const Position &pos) const {
  std::array<Position, SCALE> snakeBody = m_snake.GetSnakeBody();
  for (int i = 0; i < m_snake.GetLength(); ++i) {
    if (snakeBody[i].x == pos.x && snakeBody[i].y == pos.y) {
      return false;
    }
  }
  return true;
}

void Game::UpdateFruitPosition() {
  m_gameInfo.field[m_fruitLocation.y][m_fruitLocation.x] = FRUIT_COLOR;
}

void Game::UpdateSnakePosition() {
  std::array<Position, SCALE> snakeBody = m_snake.GetSnakeBody();
  m_gameInfo.field[m_snake.GetHead().y][m_snake.GetHead().x] = HEAD_COLOR;
  for (int i = 1; i < m_snake.GetLength(); ++i) {
    int x = snakeBody[i].x;
    int y = snakeBody[i].y;
    m_gameInfo.field[y][x] = BODY_COLOR;
  }
}

void Game::ClearField() {
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      m_gameInfo.field[i][j] = 0;
    }
  }
}

bool Game::IsCollidingFruit() const {
  return m_snake.GetHead().x == m_fruitLocation.x &&
         m_snake.GetHead().y == m_fruitLocation.y;
}

void Game::SetSnakeDireciton(Snake::Direction new_dir)
{
  m_snake.SetDirection(new_dir);
}

void Game::TogglePause()
{
  if(m_state!=State::START && m_state != State::GAME_LOST) {
    if(m_gameInfo.pause == PAUSE) {
    m_gameInfo.pause = PLAYING;
    m_state = State::MOVING;
  } else {
    m_gameInfo.pause = PAUSE;
    m_state = State::PAUSED;
  }
  }
}

void Game::QuitGame()
{
  m_gameInfo.pause = PauseState::TERMINATE;
  FILE *file = fopen("high_score.txt", "w");
  if (file) {
    fprintf(file, "%d", m_gameInfo.score);
    fclose(file);
  }
}

void Game::ProcessRestart()
{
  if(m_state == State::GAME_LOST || m_state == State::START) {
    InitGame();
  }
}

GameInfo_t Game::GetCurrentGameInfo() const
{
    return m_gameInfo;
}

Position Game::GetSnakeHeadPosition() const {
  return m_snake.GetHead();
}
