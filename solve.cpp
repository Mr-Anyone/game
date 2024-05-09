#include "solve.h"

float solveZ(const glm::mat3 &mat3, const glm::vec3 vec3) {
  float d = glm::determinant(mat3);
  if (d == 0) {
    std::cout << "cannot solve equation" << std::endl;
    assert(false);
  }

  glm::mat3 detmat = glm::mat3{{mat3[0][0], mat3[0][1], vec3[0]},
                               {mat3[1][0], mat3[1][1], vec3[1]},
                               {mat3[2][0], mat3[2][1], vec3[2]}};
  return glm::determinant(detmat) / d;
}
