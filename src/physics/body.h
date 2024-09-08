#pragma once

#include <glm/glm.hpp>

struct RigidBody {
  float mass;

  glm::vec3 net_force;
  glm::vec3 net_torque;

  glm::vec3 pos;
  glm::vec3 velocity;
  glm::vec3 angular_momentum;

  glm::mat3 orientation;
  glm::mat3 inverse_inertia_tensor;
};

struct Sphere {
  float radius;
  RigidBody *body;
};

struct TriangleMesh {
  //     -a-
  //   -     -
  // c---------c
  glm::vec3 normal; // should this be a pointer instead?
  glm::vec3 center;
  glm::vec3 indicies; // (a, b, c)
};
