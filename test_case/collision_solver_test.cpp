#include <iostream>
#include <glm/glm.hpp>

#include "ecs.hpp"
#include "solve.h"
#include "physics.h"

constexpr float tolerance = 0.001;

void test_solve_z() {
  glm::mat3 mat = {{1, 2, 3}, {2, 3, 4}, {123, 12, 2}};
  glm::vec3 b = {1, 2, 3};

  // note that the result is negative os it is plut instead of minus
  float z = solveZ(mat, b);
  assert(fabs(z + 1.18812) <= tolerance);

  // testing for another case
  b[0] = 100;
  z = solveZ(mat, b);
  assert(fabs(z - 336.980198) <= tolerance);

  // testing for another case
  mat[0][0] = -100;
  z = solveZ(mat, b);
  assert(fabs(z + 7.8531755615) <= tolerance);

  std::cout << "wow, I've passed test cases" << std::endl;
}

void test_intersection() {
  RigidBody body;
  TriangleMesh mesh;

  mesh.normal = glm::vec3(1, 1, 1);
  mesh.center = glm::vec3(3, 1, 1);

  body.pos = glm::vec3(0, 0, 0);

  // https://www.desmos.com/3d/cl4ztm4jnt see above
  glm::vec3 pos = calcualteIntersectionPointOfSphereAndTriangle(body, mesh);
  assert(fabs(pos.x - 5 / 3.f) < tolerance);
  assert(fabs(pos.y - 5 / 3.f) < tolerance);
  assert(fabs(pos.z - 5 / 3.f) < tolerance);
}

int main() {
  test_solve_z();
  test_intersection();
  return 0;
}
