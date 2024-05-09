#ifndef ECS_H
#define ECS_H

#include <iostream>
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

using Entity = std::uint32_t;

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

std::ostream &operator<<(std::ostream &os, const RigidBody &body);
RigidBody makeBoxRigidBody(float width, float mass);

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

template <typename T>
class ComponentArray {
public:
  ComponentArray();

  void appendComponent(Entity entity, T component);
  T &getComponent(Entity entity);
  void removeComponent(Entity entity);

private:
  std::vector<T> m_components;
  std::unordered_map<Entity, std::size_t> m_entity_to_index;
  std::unordered_map<std::size_t, Entity> m_index_to_entity;

  friend void testComponentArray();
};

template class ComponentArray<RigidBody>;
#endif
