#ifndef DS_H
#define DS_H

#include <array>

class DiamondSquareMesh {
public:
  DiamondSquareMesh(unsigned int size);
  DiamondSquareMesh(DiamondSquareMesh &mesh);
  ~DiamondSquareMesh();

  void renderMesh() const;
  void generateMesh(const int size, float hieght_one, float height_two,
                    float height_three, float height_four);
  void setRange(float range);

private:
  void diamondSquare(int top_left_x, int top_left_y, int bottom_right_x,
                     int bottom_right_y, float **array, int array_size,
                     float range);
  void deleteMesh();

  float *m_vertices;
  unsigned int *m_indices;

  std::size_t m_indices_size, m_vertices_size;
  unsigned int m_ebo, m_vao, m_vbo;

  float m_range = 0.1;
};

#endif
