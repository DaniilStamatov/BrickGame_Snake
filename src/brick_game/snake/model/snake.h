#pragma once
#include "../../common.h"
#include <array>
#include <vector>
class Snake {
public:
  struct SnakePart {
    int x, y;
    SnakePart() : x(0), y(0) {}
    SnakePart(int x, int y) : x(x), y(y) {}
  };
  enum class Direction { Down = 0, Left = 1, Up = 2, Right = 3 };
  Snake();
  void AddPart() noexcept;
  void Move() noexcept;
  bool IsColliding() const;
  void SetDirection(Direction new_dir) noexcept;
  std::array<SnakePart, SCALE> GetSnakeBody() const;
  int GetLength() const;
  SnakePart GetHead() const;

private:
  std::array<SnakePart, SCALE> m_parts;
  Direction m_direction;
  int m_length;

  const std::array<std::pair<int, int>, 4> c_directionOffsets = {
      std::make_pair(0, 1), std::make_pair(-1, 0), std::make_pair(0, -1),
      std::make_pair(1, 0)};
};