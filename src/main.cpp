#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "async.hpp"

int main([[maybe_unused]] int argc, char* argv[]) {
  std::string line = " ";
  Context* ctx     = connect(std::stol(argv[1]));

  while (line != "\0") {
    std::getline(std::cin, line);
    receive(ctx, line);
  }

  disconnect(ctx);

  return 0;
}
