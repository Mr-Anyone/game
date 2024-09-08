#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>

struct Square {
  glm::vec2 top_left;
  float width;

  explicit Square(glm::vec2 top_left, float width)
      : top_left{top_left}, width{width} {}
};

bool isPointInSquare(const Square &square, const glm::vec2 &vec);
#endif
