#ifndef PRINTING_H
#define PRINTING_H

#include <iostream>
#include <stdio.h>

template <typename T>
static void printMat(const T &mat, const int size) {
  std::cout << "==Some Mat" << size << " =" << std::endl;
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      printf("%5.2f,", mat[i][j]);
    }

    std::cout << "\n";
  }

  std::cout << "===" << std::endl;
}

#endif
