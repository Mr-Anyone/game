#ifndef DS_H
#define DS_H

#include "ecs.h"
#include <array>
#include <vector>

struct MeshVertices {
  glm::vec3 pos;
  glm::vec3 normal;

  explicit MeshVertices(const glm::vec3 &pos, const glm::vec3 &normal)
      : pos{pos}, normal{normal} {}
};

class DiamondSquareMesh {
public:
  DiamondSquareMesh(unsigned int size);

  void renderMesh() const;
  void generateMesh(const int size, float hieght_one, float height_two,
                    float height_three, float height_four);
  void setRange(float range);

  std::vector<TriangleMesh> makeTriangleMesh();

private:
  void diamondSquare(int top_left_x, int top_left_y, int bottom_right_x,
                     int bottom_right_y, float **array, int array_size,
                     float range);
  void deleteMesh();

  std::vector<MeshVertices> m_vertices;
  std::vector<unsigned int> m_indicies;

  std::size_t m_indices_size, m_vertices_size;
  unsigned int m_ebo, m_vao, m_vbo;

  float m_range = 0.1;
};

#endif
