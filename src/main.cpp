#include "lib.hpp"
#include <iostream>

int main(int argc, char *argv[]) {

  auto config = cmpRead(argc, argv);

  auto resuld = run(config);

  for (const auto &s : resuld) {
    std::cout << s << std::endl;
  }

  return 0;
}
