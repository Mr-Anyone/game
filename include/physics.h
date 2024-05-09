#ifndef PHYSICS_H
#define PHYSICS_H

#include "ecs.h"

// applying gravity and stuff like that
void applyGravity(ComponentArray<RigidBody> &body_array);

// physics engien basic stuff
void resolveTriangleSphereCollision();
void integrate(ComponentArray<RigidBody> &bodies, float time_delta);

// this really shouldn't be public since it is internal function, but I need
// this for testing
glm::vec3
calcualteIntersectionPointOfSphereAndTriangle(const RigidBody &body,
                                              const TriangleMesh &mesh);
#endif
