#include "lib.hpp"
#include "version.h"

int version() { return PROJECT_VERSION_PATCH; }

void printPool(Pool &pool, std::ostream &file) {

  std::string str;
  str = "bulk: ";

  for (auto p : pool) {
    str += p;
    str += ", ";
    pool.pop_back();
  }

  str.resize(str.size() - 2);
  std::cout << str << std::endl;
  file << str << std::endl;
}

void dinamikBlok(Pool &pool, std::ostream &file) {

  printPool(pool, file);
  std::string line;
  std::size_t count = 1;

  while ((line != "}") && (count != 0)) {
    std::getline(std::cin, line);
    if (line == "{") {
      ++count;
    } else if (line == "}") {
      --count;
    } else {
      pool.push_back(line);
    }
  }
}
