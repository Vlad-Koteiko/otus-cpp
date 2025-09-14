#include "lib.hpp"
#include <cassert>
#include <iostream>

int main() {

  Matrix<int, 0> matrix;

  for (std::size_t i = 0; i < 10; ++i) {

    matrix[i][i] = i;
  }

  for (std::size_t i = 9; i > 0; --i) {

    matrix[9 - i][i] = i;
  }

  for (std::size_t i = 1; i < 9; ++i) {

    for (std::size_t j = 1; j < 9; ++j) {
      std::cout << matrix[i][j] << " ";
    }

    std::cout << std::endl;
  }

  std::cout << "size matrix " << matrix.size() << std::endl;

  for (auto [r, c, v] : matrix) {
    std::cout << " r " << r << " c " << c << " v " << v << std::endl;
  }

  ((matrix[100][100] = 314) = 0) = 217;

  std::cout << matrix[100][100] << std::endl;
  return 0;
}
