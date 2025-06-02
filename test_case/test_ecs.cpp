#include "ecs.hpp"
#include <iostream>

RigidBody makeTestRigidBody(glm::vec3 some_data) {
  glm::mat4 identity = glm::mat3(1.0f);

  return {100.0f,    some_data, some_data, some_data,
          some_data, some_data, identity,  identity};
}

void testComponentArray() {
  ComponentArray<RigidBody> array;

  array.appendComponent(0, makeTestRigidBody(glm::vec3{1, 1, 1}));
  array.appendComponent(1, makeTestRigidBody(glm::vec3{1, 1, 1}));
  array.appendComponent(2, makeTestRigidBody(glm::vec3{1, 3, 1}));

  // testing for basic size!
  assert(array.m_components.size() == 3);
  std::cout << "Correct Array Size" << std::endl;

  assert(array.getComponent(2).pos == glm::vec3(1, 3, 1));
  assert(array.getComponent(0).pos == glm::vec3(1, 1, 1));
  std::cout << "Can get data" << std::endl;

  // testing removing
  array.removeComponent(1);
  assert(array.m_components.size() == 2);

  // testing getting stuff
  assert(array.getComponent(2).pos == glm::vec3(1, 3, 1));
  assert(array.getComponent(0).pos == glm::vec3(1, 1, 1));
  std::cout << "I can still get the components" << std::endl;

  array.removeComponent(2);
  assert(array.getComponent(0).pos == glm::vec3(1, 1, 1));
  assert(array.m_components.size() == 1);

  array.removeComponent(0);
  assert(array.m_components.size() == 0);
}

int main() {
  testComponentArray();
  assert(false);

  return 0;


}
