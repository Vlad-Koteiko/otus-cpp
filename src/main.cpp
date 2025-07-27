#include "lib.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>

int main(int, char **) {
  std::cout << "Version: " << version() << std::endl;

  try {

    std::map<int, std::uint64_t, std::less<int>,
             PoolAllocator<std::pair<const int, std::uint64_t>, 10>>
        foo;

    for (int i = 0; i < 10; ++i) {
      foo.insert({i, factorial(i)});
    }

    // foo.insert({11,11}); // bad alloc

    for (auto &&v : foo) {
      std::cout << " first " << v.first << " second " << v.second << std::endl;
    }

  } catch (...) {

    std::cerr << " Exception !" << std::endl;
  }

  return 0;
}
