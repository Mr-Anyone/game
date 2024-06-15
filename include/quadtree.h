#ifndef QUADTREE_H
#define QUADTREE_H

#include "collision.h"
#include <memory>
#include <vector>
#include "ecs.h"

template <typename T>
struct Node {
  glm::vec2 point;
  T obj;

  explicit Node(const glm::vec2 &point, const T &obj)
      : obj{obj}, point{point} {}
};

// assumes the following coordinate systmes
// ---------> x
// |
// |
// |
// Z
template <typename T>
class QuadTree {
public:
  explicit QuadTree(const Square &rectangle);

  void query(const glm::vec2 &point, std::vector<T> &result);
  void insert(const glm::vec2 &point, const T &obj);

private:
  void initAllFourCorner();

  static constexpr int THRESHOLD = 4;
  Square m_square;
  std::vector<Node<T>> m_array;
  std::unique_ptr<QuadTree> m_top_left, m_top_right, m_bottom_left,
      m_bottom_right;
};

template class QuadTree<double>;
template class QuadTree<TriangleMesh>;
#endif
