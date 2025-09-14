#define BOOST_TEST_MODULE test_version

#include "libs/lib.hpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_version)

BOOST_AUTO_TEST_CASE(test_valid_version) { BOOST_CHECK(version() > 0); }

BOOST_AUTO_TEST_CASE(TestMap) {

  Matrix<int, -1> matrix;
  assert(matrix.size() == 0); // все ячейки свободны
  auto a = matrix[0][0];
  assert(a == -1);
  assert(matrix.size() == 0);
  matrix[100][100] = 314;
  assert(matrix[100][100] == 314);
  assert(matrix.size() == 1);
  // // // выведется одна строка
  // // 100100314
  for (auto [x, y, v] : matrix) {
    std::cout << x << y << v << std::endl;
  }
}

BOOST_AUTO_TEST_CASE(TestFactorial) {


}
BOOST_AUTO_TEST_SUITE_END()
