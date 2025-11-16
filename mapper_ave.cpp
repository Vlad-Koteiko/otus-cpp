#include <iostream>
#include <string>
#include <vector>

int getPrice(std::string& csvLine) {
  std::vector<std::string> tokens;
  std::string token;
  bool inQuotes = false;

  for (char c : csvLine) {
    if (c == '"') {
      inQuotes = !inQuotes;
    } else if (c == ',' && !inQuotes) {
      tokens.push_back(token);
      token.clear();
    } else {
      token += c;
    }
  }
  tokens.push_back(token);
  std::string priceStr = tokens[9];

  try {
    return std::stoi(priceStr);
  } catch (const std::exception& e) {
    return -1;
  }
}

int main(int argc, char** argv) {
  std::string line;
  while (std::getline(std::cin, line)) {
    if (auto price = getPrice(line); price >= 0) {
      std::cout << price << std::endl;
    }
  }

  return 0;
}
