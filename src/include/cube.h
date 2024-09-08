#pragma once

#include "ecs.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <math.h>
#include <glm/glm.hpp>
#include <vector>
#include <shader.h>

extern glm::mat4 projection;
extern glm::mat4 view;

class Cube {
public:
  Cube()
      : m_shader{"./../shader/sphere_vertex.glsl",
                 "./../shader/sphere_fragment_shader.glsl"} {

    const float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f,
        0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f,
        0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,
        0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f};

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0],
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3,
                          (void *)0);
    glEnableVertexAttribArray(0);
  }

  void renderCube(const RigidBody &body) {
    glm::mat4 model = make4xRotationMatrx(body.orientation);
    // model = glm::scale(glm::mat4(1.0f), glm::vec3(5.0, 5.0, 5.0)) * model;
    // model = glm::translate(glm::mat4(1.0), body.pos) * model;
    // printMat(model, 4);
    // printMat(body.orientation, 3);

    m_shader.use();
    m_shader.setMat4("model", model);
    m_shader.setMat4("view", view);
    m_shader.setMat4("projection", projection);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

private:
  glm::mat4 make4xRotationMatrx(const glm::mat4 &rotation_matrix) {
    return glm::mat4{
        {rotation_matrix[0][0], rotation_matrix[0][1], rotation_matrix[0][2],
         0},
        {rotation_matrix[1][0], rotation_matrix[1][1], rotation_matrix[1][2],
         0},
        {rotation_matrix[2][0], rotation_matrix[2][1], rotation_matrix[2][2],
         0},
        {0, 0, 0, 1},
    };
  }
  unsigned int m_vao, m_vbo;
  Shader m_shader;
};
