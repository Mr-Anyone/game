#include "solve.h"
#include "printing.h"
#include "ecs.h"
#include "physics.h"

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

extern glm::vec3 camPos;
extern glm::vec3 front;

void applyGravity(ComponentArray<RigidBody> &body_array) {
  // we always assume that the player has the entity id of 0
  body_array.getComponent(0);
}

glm::mat3 createCorssMatrix(const glm::vec3 &vec3) {
  return glm::mat3{
      {0, -vec3.z, vec3.y}, {vec3.z, 0, -vec3.x}, {-vec3.y, vec3.x, 0}};
}

static glm::mat3 orthoNormalizeOrientation(glm::mat3 &mat3) {
  glm::mat3 result = glm::mat3();
  glm::vec3 x = {mat3[0][0], mat3[1][0], mat3[2][0]};
  glm::vec3 y = {mat3[0][1], mat3[1][1], mat3[2][1]};
  glm::vec3 z = {mat3[0][2], mat3[1][2], mat3[2][2]};

  x = glm::normalize(x);
  z = glm::normalize(glm::cross(x, y));
  y = glm::normalize(glm::cross(z, x));

  return glm::mat3{
      {x[0], y[0], z[0]},
      {x[1], y[1], z[1]},
      {x[2], y[2], z[2]},
  };
}

static void integrateOneBody(RigidBody &body, float time_delta) {
  // updating center of mass, the easy part
  glm::vec3 acceleration = 1 / body.mass * body.net_force; // a = 1/m *F
  body.velocity += acceleration * time_delta;              // v = vo + a dt
  body.pos += body.velocity + time_delta;

  // now this is the hard part, how to deal with net torque
  body.angular_momentum =
      body.angular_momentum +
      time_delta * body.net_torque; // updating angular momentum

  glm::vec3 omega = body.inverse_inertia_tensor * body.angular_momentum;

  // compute the new orientation
  body.orientation = body.orientation +
                     time_delta * createCorssMatrix(omega) * body.orientation;
  body.orientation = orthoNormalizeOrientation(body.orientation);

  body.inverse_inertia_tensor = body.orientation * body.inverse_inertia_tensor *
                                glm::transpose(body.orientation);

  body.net_force = glm::vec3(0.0f);
  body.net_torque = glm::vec3(0.0f);
}

void integrate(ComponentArray<RigidBody> &bodies, float time_delta) {
  integrateOneBody(bodies.getComponent(0), time_delta);
}

glm::vec3
calcualteIntersectionPointOfSphereAndTriangle(const RigidBody &body,
                                              const TriangleMesh &mesh) {

  // define some useful variables
  glm::vec3 normal = mesh.normal;
  glm::vec3 point_in_triangle = mesh.center;
  glm::vec3 center_of_sphere = body.pos; // the position is the center of mass

  glm::mat3 a = {{0, -normal.y, -normal.x},
                 {-normal.z, normal.x, -normal.y},
                 {normal.y, 0, -normal.z}};

  float u = solveZ(a, center_of_sphere - point_in_triangle);
  glm::vec3 intersection = u * normal + center_of_sphere;
  return intersection;
}
// this assumes that the rigid body is indeed a sphere

void resolveTriangleSphereCollision(const RigidBody &body,
                                    const TriangleMesh &mesh) {
  glm::vec3 intersection =
      calcualteIntersectionPointOfSphereAndTriangle(body, mesh);
}
