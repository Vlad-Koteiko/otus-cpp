#define BOOST_TEST_MODULE test_version

#include "libs/lib.hpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_version)

BOOST_AUTO_TEST_CASE(test_valid_version) { BOOST_CHECK(version() > 0); }

BOOST_AUTO_TEST_CASE(TestMap) {

  std::map<int, std::uint64_t, std::less<int>,
           PoolAllocator<std::pair<const int, std::uint64_t>, 10>>
      foo;

  for (int i = 0; i < 10; ++i) {
    foo.insert({i, factorial(i)});
  }
}

BOOST_AUTO_TEST_CASE(TestFactorial) {

  std::uint64_t val = factorial(9);

  BOOST_CHECK(val == 362880);
}
BOOST_AUTO_TEST_SUITE_END()
