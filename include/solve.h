#ifndef SOLVE_H
#define SOLVE_H

#include <iostream>
#include <glm/glm.hpp>

// Cramer's Rules, since I am too lazy to implement Gaussian eliminination
float solveZ(const glm::mat3 &mat3, const glm::vec3 vec3);
#endif
