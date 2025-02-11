#include "snake.h"

Snake::Snake() : m_direction(Direction::Left), m_length(1) {
  m_parts[0] = Position(WIDTH / 2 - 1, HEIGHT / 2 - 1);
}

void Snake::AddPart() noexcept { ++m_length; }

void Snake::Move() noexcept {
  auto offset = c_directionOffsets[static_cast<int>(m_direction)];
  int old_x = m_parts.front().x;
  int old_y = m_parts.front().y;
  m_parts.front().x += offset.first;
  m_parts.front().y += offset.second;

  for (size_t i = 1; i < m_parts.size(); ++i) {
    std::swap(m_parts[i].x, old_x);
    std::swap(m_parts[i].y, old_y);
  }
}

bool Snake::IsColliding() const {
  if (GetHead().x < 0 || GetHead().x > WIDTH - 1 || GetHead().y < 0 ||
      GetHead().y > HEIGHT - 1) {
    return true;
  }

  for (int i = 1; i < m_length; i++) {
    if (GetHead().x == m_parts[i].x && GetHead().y == m_parts[i].y) {
      return true;
    }
  }

  return false;
}

void Snake::SetDirection(Direction new_dir) noexcept { m_direction = new_dir; }

std::array<Position, SCALE> Snake::GetSnakeBody() const { return m_parts; }

int Snake::GetLength() const { return m_length; }

Position Snake::GetHead() const { return *m_parts.begin(); }

void Snake::ClearSnake() {
  m_length = 1;
  m_parts[0] = Position(WIDTH / 2 - 1, HEIGHT / 2 - 1);
}
