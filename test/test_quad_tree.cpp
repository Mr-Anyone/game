#include <iostream>
#include "glm/fwd.hpp"
#include "quadtree.h"

void quad_tree_test() {
  Square square({10, 10}, 20);
  QuadTree<double> tree(square);

  for (int i = 0; i < 100; ++i) {
    tree.insert(glm::vec2(15, 16), 10);
    tree.insert(glm::vec2(15, 16), 10);
    tree.insert(glm::vec2(15, 16), 10);
    tree.insert(glm::vec2(15, 16), 10);
  }

  std::vector<double> result;
  tree.query(glm::vec2(15, 16), result);

  assert(result.size() == 400);
  result.clear();

  tree.insert(glm::vec2(30, 30), 123.1);
  tree.query(glm::vec2(30, 30), result);
  std::cout << "the size of the array is: " << result.size() << std::endl;

  bool found = false;
  for (double data : result) {
    if (data == 123.1) {
      found = true;
    }
  }
  assert(found);
}

void quad_tree_test_2() {
  QuadTree<double> tree(Square(glm::vec2(0, 0), 10));
  glm::vec2 on_edge(5, 5);
  for (int i = 0; i < 100; ++i) {
    tree.insert(on_edge, 123.1);
  }

  std::vector<double> result;
  tree.query(on_edge, result);
  assert(result.size() == 100);

  // assering that each element is that right number
  for (int i = 0; i < result.size(); ++i) {
    assert(result[i] == 123.1);
  }

  std::cout << "I passed second test. Yay" << std::endl;
}
int main() {
  quad_tree_test();
  quad_tree_test_2();

  // struct Stuff {
  //   glm::vec2 x;
  //   glm::vec3 y;
  // };

  // Stuff a{glm::vec2(1, 1), glm::vec3(12, 1, 12)};
  // Stuff b{glm::vec2(1123, 1), glm::vec3(12, 1, 12)};
  // std::vector<Stuff> d = {a, b};
  // float *c = reinterpret_cast<float *>(&d[0]);

  // assert(*c == 1);
  // assert(*(c + 1) == 1);
  // assert(*(c + 2) == 12);
  // assert(*(c + 3) == 1);
  // assert(*(c + 4) == 12);
  // assert(*(c + 5) == 1123);
  // std::cout << "some testing done" << std::endl;
  return 0;
}
