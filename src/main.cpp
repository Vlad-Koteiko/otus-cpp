#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

#include "../libs/classifier.h"
#include "../libs/logreg_classifier.h"

using namespace ::kdd99;

bool read_features(std::istream& stream,
                   kdd99::BinaryClassifier::features_t& features) {
  std::string line;
  std::getline(stream, line);

  features.clear();
  std::istringstream linestream {line};
  double value;
  while (linestream >> value) {
    features.push_back(value);
  }
  return stream.good();
}

std::vector<LogregClassifier> read_vector(std::istream& stream) {
  std::vector<LogregClassifier> classifiers;
  std::string line;

  while (std::getline(stream, line)) {
    if (line.empty())
      continue;

    std::istringstream iss(line);
    std::vector<double> coef((std::istream_iterator<double>(iss)),
                             std::istream_iterator<double>());
    classifiers.emplace_back(coef);
  }

  return classifiers;
}

int main([[maybe_unused]] int argc, char* argv[]) {
  std::ifstream istream {argv[2]};
  auto predictor = read_vector(istream);
  istream.close();

  std::ifstream test_data {argv[1]};

  kdd99::BinaryClassifier::features_t features;
  int correct = 0;
  int total   = 0;

  double y_true = 0.0;

  while (true) {
    if (!(test_data >> y_true))
      break;

    if (!read_features(test_data, features))
      break;

    int predicted_class = 0;
    double best_prob    = -1.0;

    std::cout << "-------------------  ------------------------" << std::endl;
    for (size_t i = 0; i < 10; ++i) {
      double p = predictor[i].predict_proba(features);
      std::cout << i << " conf-> " << p << std::endl;
      if (p > best_prob) {
        best_prob       = p;
        predicted_class = i;
      }
    }

    std::cout << "test number-> " << y_true << " calculater number-> "
              << predicted_class << std::endl;

    if (predicted_class == static_cast<int>(y_true))
      correct++;

    total++;
  }

  std::cout << (double)correct / total << std::endl;

  return 0;
}
