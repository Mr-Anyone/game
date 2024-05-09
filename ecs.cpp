#include "ecs.h"
#include <iostream>

template <typename T>
ComponentArray<T>::ComponentArray() {}

template <typename T>
void ComponentArray<T>::appendComponent(Entity entity, T component) {
  if (m_entity_to_index.find(entity) != m_entity_to_index.end()) {
    std::cout << "You are going to replace stuff" << std::endl;
  }

  m_components.push_back(component);
  // inserting stuff to make sure things work
  m_entity_to_index[entity] = m_components.size() - 1;
  m_index_to_entity[m_components.size() - 1] = entity;
}

template <typename T>
void ComponentArray<T>::removeComponent(Entity to_be_replace_entity) {
  if (m_entity_to_index.find(to_be_replace_entity) == m_entity_to_index.end()) {
    std::cout << "This is game over" << std::endl;
    return;
  }

  if (m_components.size() == 1) {
    m_components.clear();
    m_entity_to_index.clear();
    m_index_to_entity.clear();
    return;
  }

  // an edge case where we are removing the last index!
  if (m_components.size() - 1 == m_entity_to_index[to_be_replace_entity]) {
    m_components.pop_back();
    m_index_to_entity.erase(m_components.size() - 1);
    m_entity_to_index.erase(to_be_replace_entity);
    return;
  }

  // we would remove stuff by replacing stuff
  std::size_t last_index = m_components.size() - 1;
  std::size_t to_be_replace_index = m_entity_to_index[to_be_replace_entity];
  Entity last_entity = m_index_to_entity[last_index];

  m_components[to_be_replace_index] = m_components[last_index];

  // removing and replacing some stuff
  m_entity_to_index.erase(to_be_replace_entity);
  m_index_to_entity.erase(to_be_replace_index);

  m_entity_to_index.erase(last_entity);
  m_index_to_entity.erase(last_index);

  m_index_to_entity[to_be_replace_index] = last_entity;
  m_entity_to_index[last_entity] = to_be_replace_index;

  m_components.pop_back();
}

template <typename T>
T &ComponentArray<T>::getComponent(Entity entity) {
  if (m_entity_to_index.find(entity) == m_entity_to_index.end()) {
    std::cout << "could not find component!" << std::endl;
    assert(false);
  }

  return m_components[m_entity_to_index[entity]];
}

RigidBody makeBoxRigidBody(float width, float mass) {
  glm::vec3 zero_vector = glm::vec3(0.0, 0.0, 0.0);
  glm::mat3 identity = glm::mat3(1.0f);

  // see here for more: https://hepweb.ucsd.edu/ph110b/110b_notes/node26.html
  glm::mat3 inertia_tensor = mass * width * width / 6 * glm::mat3(1.0f);
  glm::mat3 inverse_inertia_tensor = glm::inverse(inertia_tensor);

  return RigidBody{mass,        zero_vector,           zero_vector,
                   zero_vector, zero_vector,           zero_vector,
                   identity,    inverse_inertia_tensor};
}

static void printVec3(std::ostream &os, const glm::vec3 &vec3) {
  os << vec3.x << "," << vec3.y << "," << vec3.z;
}

std::ostream &operator<<(std::ostream &os, const RigidBody &body) {
  // print out net force and net torque
  os << "torque";
  printVec3(os, body.net_torque);

  os << " net force ";
  printVec3(os, body.net_force);

  return os;
}
