#pragma once

#include <array>
#include <utility>

#include "../../common.h"

/**
 * @file snake.h
 * @brief Snake model types and movement logic.
 *
 * This file declares the `Position` structure and the `Snake` class which
 * stores the snake body, movement direction and provides methods for
 * moving, growing and collision detection.
 */

/**
 * @struct Position
 * @brief Simple 2D integer coordinate used for the field and snake body.
 */
struct Position {
  int x = 0;
  int y = 0;

  Position() = default;
  Position(int x_val, int y_val) : x(x_val), y(y_val) {}
};

/**
 * @class Snake
 * @brief Represents the snake body, movement and collision checks.
 *
 * The `Snake` class stores the ordered list of body positions, the current
 * movement direction and provides operations to move the snake, grow it
 * when a fruit is eaten and check for self or boundary collisions.
 */
namespace s21 {
class Snake {
 public:
  /**
   * @enum Direction
   * @brief Cardinal movement directions used by the snake.
   */
  enum class Direction { kDown = 0, kLeft = 1, kUp = 2, kRight = 3 };

  Snake();

  void AddPart() noexcept;
  void Move() noexcept;
  void SetDirection(Direction new_dir) noexcept;
  void Clear();

  bool IsColliding() const;
  std::array<Position, SCALE> GetBody() const;
  int GetLength() const;
  Position GetHead() const;

 private:
  static constexpr std::array<std::pair<int, int>, 4> kDirectionOffsets = {
      std::make_pair(0, 1),   // Down
      std::make_pair(-1, 0),  // Left
      std::make_pair(0, -1),  // Up
      std::make_pair(1, 0)    // Right
  };

  std::array<Position, SCALE> body_;
  Direction direction_{Direction::kRight};
  int length_ = 4;
};

}  // namespace s21
