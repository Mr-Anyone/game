#include "collision.h"

bool isPointInSquare(const Square &square, const glm::vec2 &point) {
  float x = square.top_left.x;
  float y = square.top_left.y;
  float width = square.width;

  return x + width >= point.x && point.x >= x && y + width >= point.y &&
         point.y >= y;
}
