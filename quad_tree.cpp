#include <iostream>
#include "quadtree.h"
#include <cassert>

template <typename T>
QuadTree<T>::QuadTree(const Square &square)
    : m_square{square}, m_top_left{nullptr}, m_top_right{nullptr},
      m_bottom_left{nullptr}, m_bottom_right{nullptr} {}

template <typename T>
void QuadTree<T>::insert(const glm::vec2 &point, const T &obj) {
  assert(isPointInSquare(m_square, point) &&
         "Your point is outside of the rectangle of the quadtree!");
  if (m_array.size() < THRESHOLD) {
    m_array.emplace_back(Node(point, obj));
    return;
  }

  // initialize all four corner
  initAllFourCorner();

  if (isPointInSquare(m_bottom_left->m_square, point)) {
    m_bottom_left->insert(point, obj);
  } else if (isPointInSquare(m_bottom_right->m_square, point)) {
    m_bottom_right->insert(point, obj);
  } else if (isPointInSquare(m_top_right->m_square, point)) {
    m_top_right->insert(point, obj);
  } else if (isPointInSquare(m_top_left->m_square, point)) {
    m_top_left->insert(point, obj);
  } else {
    std::cout << "what is going on? You should maybe consider increasing the "
                 "size of the tree"
              << std::endl;
    assert(false);
  }
}

template <typename T>
void QuadTree<T>::initAllFourCorner() {
  float new_square_width = m_square.width / 2.f;

  if (m_top_left == nullptr) {
    m_top_left = std::make_unique<QuadTree<T>>(
        Square(m_square.top_left, new_square_width));
  }

  if (m_top_right == nullptr) {
    m_top_right = std::make_unique<QuadTree<T>>(
        Square({m_square.top_left.x + new_square_width, m_square.top_left.y},
               new_square_width));
  }

  if (m_bottom_left == nullptr) {
    m_bottom_left = std::make_unique<QuadTree<T>>(
        Square({m_square.top_left.x, m_square.top_left.y + new_square_width},
               new_square_width));
  }

  if (m_bottom_right == nullptr) {
    m_bottom_right = std::make_unique<QuadTree<T>>(
        Square({m_square.top_left.x + new_square_width,
                m_square.top_left.y + new_square_width},
               new_square_width));
  }
}

// @todo: this is some really unsafe code!
// you have to not either make a copy, or think of ways to share a pointer!
template <typename T>
void QuadTree<T>::query(const glm::vec2 &point, std::vector<T> &result) {
  assert(isPointInSquare(m_square, point) &&
         "Your point is not in the square. GG");
  for (auto &node : m_array) {
    result.push_back(node.obj);
  }

  if (m_top_left) {
    if (isPointInSquare(m_top_left->m_square, point)) {
      m_top_left->query(point, result);
    }
  }

  if (m_top_right) {
    if (isPointInSquare(m_top_right->m_square, point)) {
      m_top_right->query(point, result);
    }
  }

  if (m_bottom_right) {
    if (isPointInSquare(m_bottom_right->m_square, point)) {
      m_bottom_right->query(point, result);
    }
  }

  if (m_bottom_left) {
    if (isPointInSquare(m_bottom_left->m_square, point)) {
      m_bottom_left->query(point, result);
    }
  }
}
