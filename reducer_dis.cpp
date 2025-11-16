#include <iostream>
#include <numeric>
#include <string>
#include <vector>

double calculate(const std::vector<int>& prices) {
  if (prices.empty() || prices.size() == 1) {
    return 0.0;
  }

  double mean =
      std::accumulate(prices.begin(), prices.end(), 0.0) / prices.size();

  double sumSquaredDifferences = 0.0;

  for (int price : prices) {
    double difference = price - mean;
    sumSquaredDifferences += difference * difference;
  }

  return sumSquaredDifferences / (prices.size() - 1);
}

int main(int argc, char** argv) {
  std::string line;
  std::vector<int> numbers;

  while (std::getline(std::cin, line)) {
    numbers.push_back(std::stoi(line));
  }

  std::cout << calculate(numbers) << std::endl;

  return 0;
}
