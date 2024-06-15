#include <cassert>
#include <cstdlib>
#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <glm/glm.hpp>
#include <vector>

#include "ds.h"

static float randomRange(float start, float range) {
  float r2 =
      static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / range));

  // std::cout << start + r2 << std::endl;
  return start + r2;
}

static void save_to_file(const char *path_to_file, float **array, int size) {
  std::ofstream file;
  file.open(path_to_file, std::ofstream::out | std::ofstream::app);

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      file << array[i][j] << "|";
    }
    file << "\n";
  }
}

static float average(float one, float two, float three, float four) {
  // std::cout << "one: " << one << " two: " << two << " three "  << three <<
  // "four " << four << std::endl;
  float average = (one + two + three + four) / 4.0;
  // std::cout << average << std::endl;
  return average;
}

static float average(float one, float two, float three) {
  return (one + two + three) / 3.0f;
}

static void print_array(float **array, int array_size) {
  for (int i = 0; i < array_size; ++i) {
    for (int j = 0; j < array_size; ++j) {
      std::cout << array[i][j] << "|";
    }

    std::cout << "\n";
  }
}

void DiamondSquareMesh::diamondSquare(int top_left_x, int top_left_y,
                                      int bottom_right_x, int bottom_right_y,
                                      float **array, int array_size,
                                      float range) {
  if (bottom_right_x - top_left_x < 2) {
    return;
  }

  // square step
  int mid_x = (top_left_x + bottom_right_x) / 2;
  int mid_y = (top_left_y + bottom_right_y) / 2;

  float average_corner = average(
      array[top_left_x][top_left_y], array[top_left_x][bottom_right_y],
      array[bottom_right_x][top_left_y], array[bottom_right_x][bottom_right_y]);

  // square step
  array[mid_x][mid_y] = average_corner + randomRange(0.0, 0.1);

  // diamond step
  float top_left = array[top_left_x][top_left_y];
  float top_rigth = array[top_left_x][bottom_right_y];
  float bottom_left = array[bottom_right_x][top_left_y];
  float bottom_right = array[bottom_right_x][bottom_right_y];
  float mid = array[mid_x][mid_y];

  array[top_left_x][mid_y] =
      average(top_left, mid, top_rigth) + randomRange(-range / 2, range);
  array[mid_x][top_left_y] =
      average(top_left, mid, bottom_left) + randomRange(-range / 2, range);
  array[bottom_right_x][mid_y] =
      average(mid, top_rigth, bottom_right) + randomRange(-range / 2, range);
  array[mid_x][bottom_right_y] =
      average(mid, bottom_left, bottom_right) + randomRange(-range / 2, range);

  // print_array(array, array_size);

  // std::cout << array[top_left_x][top_left_y] << std::endl;
  // recursively call this function
  diamondSquare(top_left_x, top_left_y, mid_x, mid_y, array, array_size,
                range / 2);
  diamondSquare(mid_x, top_left_y, bottom_right_x, mid_y, array, array_size,
                range / 2);

  diamondSquare(top_left_x, mid_y, mid_x, bottom_right_y, array, array_size,
                range / 2);
  diamondSquare(mid_x, mid_y, bottom_right_x, bottom_right_y, array, array_size,
                range / 2);
}

DiamondSquareMesh::DiamondSquareMesh(unsigned int size) {
  m_range = 10.0;

  generateMesh(size, 100, 10, 50, -20);
}

void DiamondSquareMesh::renderMesh() const {
  glBindVertexArray(m_vao);
  glDrawElements(GL_TRIANGLES, m_indicies.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void DiamondSquareMesh::deleteMesh() {
  m_indicies.clear();
  m_vertices.clear();

  // free opengl buffers
  glDeleteBuffers(1, &m_ebo);
  glDeleteBuffers(1, &m_vao);
  glDeleteVertexArrays(1, &m_vao);
}

void DiamondSquareMesh::generateMesh(const int size, float hieght_one,
                                     float height_two, float height_three,
                                     float height_four) {
  deleteMesh();

  int array_width = std::pow(2, size) + 1;

  float **temp_array = new float *[array_width];
  for (int i = 0; i < array_width; ++i) {
    temp_array[i] = new float[array_width];
  }

  temp_array[0][0] = hieght_one;
  temp_array[array_width - 1][0] = height_two;
  temp_array[array_width - 1][array_width - 1] = height_three;
  temp_array[0][array_width - 1] = height_four;

  diamondSquare(0, 0, array_width - 1, array_width - 1, temp_array, array_width,
                m_range);

  // turning 2d array into a single 1d array
  int count = 0;
  for (int i = 0; i < array_width; ++i) {
    for (int j = 0; j < array_width; ++j) {
      // adding data into vertices
      glm::vec3 pos = {j, temp_array[i][j], i};
      glm::vec3 normal = {0, 1, 0};

      // do this if this is not the last
      if (i < array_width - 1 && j < array_width - 1) {
        glm::vec3 a{j, temp_array[i][j], i};
        glm::vec3 b{j, temp_array[i][j], i + 1};
        glm::vec3 c{j + 1, temp_array[i][j], i};
        glm::vec3 d{j + 1, temp_array[i][j], i};

        normal = 0.5f *
                 (glm::cross((b - a), (c - a)) + glm::cross((c - a), (d - a)));
      }

      MeshVertices vertex(pos, normal);
      m_vertices.push_back(vertex);
    }
  }

  for (int i = 0; i < m_vertices.size() - array_width; ++i) {

    if ((i + 1) % array_width == 0 && i != 0) {
      continue;
    }

    m_indicies.push_back(i);
    m_indicies.push_back(i + array_width);
    m_indicies.push_back(i + array_width + 1);

    m_indicies.push_back(i + 1);
    m_indicies.push_back(i);
    m_indicies.push_back(i + array_width + 1);
  }

  glGenBuffers(1, &m_vbo);
  glGenBuffers(1, &m_ebo);
  glGenVertexArrays(1, &m_vao);

  glBindVertexArray(m_vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(MeshVertices) * m_vertices.size(),
               &m_vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(unsigned int) * m_indicies.size(), &m_indicies[0],
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  for (int i = 0; i < array_width; ++i) {
    delete[] temp_array[i];
  }

  delete[] temp_array;

  glBindVertexArray(0);
}

glm::vec3 computeCenter(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
  return (a + b + c) / 3.0f;
}

std::vector<TriangleMesh> DiamondSquareMesh::makeTriangleMesh() {
  std::vector<TriangleMesh> triangle_meshes = {};

  assert(m_indicies.size() % 3 == 0);
  for (int i = 0; i < m_indicies.size(); i += 6) {
    // triangle meshes
    glm::vec3 a = m_vertices[m_indicies[i]].pos;
    glm::vec3 b = m_vertices[m_indicies[i + 1]].pos;
    glm::vec3 c = m_vertices[m_indicies[i + 2]].pos;

    glm::vec3 center1 = computeCenter(a, b, c);
    glm::vec3 normal1 = glm::cross((b - a), (c - a));

    glm::vec3 d = m_vertices[m_indicies[i + 3]].pos;
    glm::vec3 e = m_vertices[m_indicies[i + 4]].pos;
    glm::vec3 f = m_vertices[m_indicies[i + 5]].pos;

    glm::vec3 center2 = computeCenter(d, e, f);
    glm::vec3 normal2 = glm::cross((e - d), (f - d));

    TriangleMesh mesh1(center1, normal1);
    TriangleMesh mesh2(center2, normal2);
    triangle_meshes.push_back(mesh1);
    triangle_meshes.push_back(mesh2);
  }

  return triangle_meshes;
}

void DiamondSquareMesh::setRange(const float range) { m_range = range; }
