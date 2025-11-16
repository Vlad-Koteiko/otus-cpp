#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main(int argc, char** argv) {
  std::string line;
  std::vector<int> numbers;

  while (std::getline(std::cin, line)) {
    numbers.push_back(std::stoi(line));
  }

  double sum     = std::accumulate(numbers.begin(), numbers.end(), 0.0);
  double average = sum / numbers.size();

  std::cout << average << std::endl;

  return 0;
}
