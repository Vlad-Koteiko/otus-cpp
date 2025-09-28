#include "lib.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

int main([[maybe_unused]] int argc, char *argv[]) {

  std::string line;
  Pool pool;
  std::ofstream file("bulk.log");
  auto time = std::chrono::system_clock::now();

  if (file.is_open()) {

    for (int i = 0; i < std::stol(argv[1]); ++i) {
      std::getline(std::cin, line);
      time = std::chrono::system_clock::now();
      if (line == "\0") {
        break;
      } else if (line == "{") {
        dinamikBlok(pool, file);
        --i;
      } else {
        pool.push_back(line);
      }
    }

    printPool(pool, file);
    file.close();
  }

  std::time_t seconds = std::chrono::system_clock::to_time_t(time);
  std::string newFileName = "bulk" + std::to_string(seconds) + ".log";
  std::filesystem::rename("bulk.log", newFileName);

  return 0;
}
