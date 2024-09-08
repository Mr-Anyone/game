#pragma once

#include "shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <math.h>
#include <vector>

constexpr float PI = 3.1416926;

class LightingSphere {
public:
  LightingSphere(float radius, int resolution)
      : m_shader("./../shader/sphere_vertex.glsl",
                 "./../shader/sphere_fragment_shader.glsl") {
    makeMesh(radius, resolution);
  }

  void makeMesh(float radius, int resolution) {
    m_vertices.clear();
    m_indicies.clear();

    float delta_y = (radius * 2) / resolution;
    float delta_theta = 2 * PI / resolution;

    // two times resolution since we want a sphere and not a semi sphere
    for (int i = 1; i <= resolution; ++i) {
      float y = radius - i * delta_y;
      float r = std::sqrt(radius * radius - y * y);

      for (int j = 0; j < resolution; ++j) {
        float theta = delta_theta * j;

        float x = r * std::cos(theta);
        float z = r * std::sin(theta);

        // pushing back x, y, z
        m_vertices.push_back(x);
        m_vertices.push_back(y);
        m_vertices.push_back(z);
      }
    }
    m_vertices.push_back(0);
    m_vertices.push_back(radius);
    m_vertices.push_back(0);

    m_vertices.push_back(0);
    m_vertices.push_back(-radius);
    m_vertices.push_back(0);

    for (int i = 0; i < resolution * (resolution - 2); ++i) {
      if ((i + 1) % (resolution) == 0 && i != 0) {
        m_indicies.push_back(i);
        m_indicies.push_back(i + resolution);
        m_indicies.push_back(i + 1);

        m_indicies.push_back(i);
        m_indicies.push_back(i - resolution + 1);
        m_indicies.push_back(i + 1);
        continue;
      }
      m_indicies.push_back(i);
      m_indicies.push_back(i + resolution);
      m_indicies.push_back(i + resolution + 1);

      m_indicies.push_back(i);
      m_indicies.push_back(i + 1);
      m_indicies.push_back(i + resolution + 1);
    }

    unsigned int indicie_top = resolution * (resolution);
    unsigned int indicies_bottom = resolution * (resolution) + 1;
    for (int i = 0; i < resolution; ++i) {
      if (i == resolution - 1) {
        m_indicies.push_back(i);
        m_indicies.push_back(indicie_top);
        m_indicies.push_back(0);

        m_indicies.push_back(i + resolution * (resolution - 2));
        m_indicies.push_back(indicies_bottom);
        m_indicies.push_back(i + resolution * (resolution - 2) - resolution +
                             1);
      } else {
        m_indicies.push_back(i);
        m_indicies.push_back(indicie_top);
        m_indicies.push_back(i + 1);

        m_indicies.push_back(i + resolution * (resolution - 2));
        m_indicies.push_back(indicies_bottom);
        m_indicies.push_back(i + resolution * (resolution - 2) + 1);
      }
    }

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_vertices.size(),
                 m_vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(unsigned int) * m_indicies.size(), m_indicies.data(),
                 GL_STATIC_DRAW);
  }

  void render(const glm::mat4 &projection, const glm::mat4 &view,
              const glm::vec3 &pos, const glm::vec3 &lightColor) {
    m_shader.use();
    m_shader.setMat4("projection", projection);
    m_shader.setMat4("view", view);
    m_shader.setMat4("model", glm::translate(glm::mat4(1.0f), pos));
    m_shader.setVec3("lightColor", lightColor.x, lightColor.y, lightColor.z);

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indicies.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

private:
  int m_resolution;
  std::vector<float> m_vertices{};
  std::vector<unsigned int> m_indicies{};

  unsigned int m_vao, m_vbo, m_ebo;
  Shader m_shader;
};
