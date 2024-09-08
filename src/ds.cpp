#include <cassert>
#include <cstdlib>
#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <glm/glm.hpp>

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
  m_indices = nullptr;
  m_vertices = nullptr;
  m_range = 10.0;

  generateMesh(size, 10, 1.0, 5.0, 0.0);
}

DiamondSquareMesh::DiamondSquareMesh(DiamondSquareMesh &mesh) {
  mesh.m_vertices = new float[m_vertices_size];
  mesh.m_indices = new unsigned int[m_indices_size];

  for (int i = 0; i < m_vertices_size; ++i) {
    mesh.m_vertices[i] = m_vertices[i];
  }

  for (int i = 0; i < m_indices_size; ++i) {
    mesh.m_indices[i] = m_indices[i];
  }
}

DiamondSquareMesh::~DiamondSquareMesh() {
  if (m_indices != nullptr)
    delete[] m_indices;

  if (m_vertices != nullptr)
    delete[] m_vertices;
}

void DiamondSquareMesh::renderMesh() const {
  glBindVertexArray(m_vao);
  glDrawElements(GL_TRIANGLES, m_indices_size, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void DiamondSquareMesh::deleteMesh() {
  // free opengl buffers
  // glDeleteBuffers(1, &m_ebo);
  // glDeleteBuffers(1, &m_vao);
  // glDeleteVertexArrays(1, &m_vao);

  // free memory
  if (m_indices != nullptr)
    delete[] m_indices;

  if (m_vertices != nullptr)
    delete[] m_vertices;
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
  // save_to_file("grid.txt", temp_array, array_width);
  // for(int i = 0; i<array_width; ++i){
  //    for(int j = 0;j<array_width; ++j){
  //        std::cout << temp_array[i][j] << "|";
  //    }
  //    std::cout << "\n";
  //}

  // turning 2d array into a single 1d array
  m_vertices = new float[array_width * array_width * 6];
  int count = 0;
  for (int i = 0; i < array_width; ++i) {
    for (int j = 0; j < array_width; ++j) {
      // adding data into vertices
      m_vertices[count] = j;
      m_vertices[count + 1] = temp_array[i][j];
      m_vertices[count + 2] = i;

      // do this if this is not the last
      if (i < array_width - 1 && j < array_width - 1) {
        glm::vec3 a{j, temp_array[i][j], i};
        glm::vec3 b{j, temp_array[i][j], i + 1};
        glm::vec3 c{j + 1, temp_array[i][j], i};
        glm::vec3 d{j + 1, temp_array[i][j], i};

        glm::vec3 normal = 0.5f * (glm::cross((b - a), (c - a)) +
                                   glm::cross((c - a), (d - a)));
        m_vertices[count + 3] = normal.x;
        m_vertices[count + 4] = normal.y;
        m_vertices[count + 5] = normal.z;
      } else {
        m_vertices[count + 3] = 0;
        m_vertices[count + 4] = 1;
        m_vertices[count + 5] = 0;
      }
      count += 6;
      // std::cout << temp_array[2][3] << std::endl;
    }
  }
  m_vertices_size = count;

  // std::cout << "There are: " << count << " vertices " << std::endl;

  m_indices_size = (array_width - 1) * (array_width - 1) * 6;
  m_indices = new unsigned int[m_indices_size];
  int indicies_index = 0, verticies_index = 0;
  while (indicies_index < m_indices_size) {
    if ((verticies_index + 1) % array_width == 0 && verticies_index != 0) {
      verticies_index += 1;
    }

    assert(indicies_index < m_indices_size &&
           "invalid memeory access in indicies");
    assert(verticies_index + array_width + 1 < m_vertices_size &&
           "invalid memory access in vertices");

    m_indices[indicies_index] = verticies_index;
    m_indices[indicies_index + 1] = verticies_index + array_width;
    m_indices[indicies_index + 2] = verticies_index + array_width + 1;
    m_indices[indicies_index + 3] = verticies_index + 1;
    m_indices[indicies_index + 4] = verticies_index + array_width + 1;
    m_indices[indicies_index + 5] = verticies_index;

    indicies_index += 6;
    ++verticies_index;
  }

  // for(int i =0; i<m_vertices_size; ++i){
  //    std::cout << m_vertices[i] << ",";
  //}
  // std::cout << "\n";

  // for(int i =0; i<m_indices_size; ++i){
  //    std::cout << m_indices[i] << ",";
  //}
  // std::cout << "\n";

  glGenBuffers(1, &m_vbo);
  glGenBuffers(1, &m_ebo);
  glGenVertexArrays(1, &m_vao);

  glBindVertexArray(m_vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_vertices_size, m_vertices,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices_size,
               m_indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // std::cout << "vertices" << std::endl;
  // for(int i = 0; i<m_vertices_size; i +=3){
  //    std::cout << m_vertices[i] << "|" << m_vertices[i + 1] << "|" <<
  //    m_vertices[i + 2] << "|" << std::endl;
  //}
  // std::cout << "vertices" << std::endl;

  // std::cout << "indicies" << std::endl;
  // for(int i = 0; i<6; ++i){
  //    std::cout << m_indices[i] << std::endl;
  //}
  // std::cout << "indices" << std::endl;

  for (int i = 0; i < array_width; ++i) {
    delete[] temp_array[i];
  }

  delete[] temp_array;

  glBindVertexArray(0);
}

void DiamondSquareMesh::setRange(const float range) { m_range = range; }
