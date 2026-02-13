#include "snake.h"

namespace s21 {

Snake::Snake() {
  int cx = WIDTH / 2 - 1;
  int cy = HEIGHT / 2 - 1;
  for (int i = 0; i < length_; ++i) {
    body_[i] = Position(cx - i, cy);
  }
}

void Snake::AddPart() noexcept {
  if (length_ < SCALE) {
    Position tail = body_[length_ - 1];
    body_[length_] = tail;
    ++length_;
  }
}

void Snake::Move() noexcept {
  const auto& offset = kDirectionOffsets[static_cast<int>(direction_)];

  Position new_head = body_[0];
  new_head.x += offset.first;
  new_head.y += offset.second;

  for (int i = length_ - 1; i >= 1; --i) {
    body_[i] = body_[i - 1];
  }

  body_[0] = new_head;
}

bool Snake::IsColliding() const {
  const Position& head = GetHead();

  if (head.x < 0 || head.x >= WIDTH || head.y < 0 || head.y >= HEIGHT) {
    return true;
  }

  for (int i = 1; i < length_; ++i) {
    if (head.x == body_[i].x && head.y == body_[i].y) {
      return true;
    }
  }

  return false;
}

void Snake::SetDirection(Direction new_dir) noexcept {
  if ((direction_ == Direction::kLeft && new_dir == Direction::kRight) ||
      (direction_ == Direction::kRight && new_dir == Direction::kLeft) ||
      (direction_ == Direction::kUp && new_dir == Direction::kDown) ||
      (direction_ == Direction::kDown && new_dir == Direction::kUp)) {
    return;
  }
  direction_ = new_dir;
}

std::array<Position, SCALE> Snake::GetBody() const { return body_; }

int Snake::GetLength() const { return length_; }

Position Snake::GetHead() const { return body_.front(); }

void Snake::Clear() {
  length_ = 4;
  int cx = WIDTH / 2 - 1;
  int cy = HEIGHT / 2 - 1;
  for (int i = 0; i < length_; ++i) {
    body_[i] = Position(cx - i, cy);
  }
}

}  // namespace s21
