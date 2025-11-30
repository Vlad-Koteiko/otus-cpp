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
    std::vector<float> coef((std::istream_iterator<float>(iss)),
                            std::istream_iterator<float>());

    classifiers.emplace_back(coef);
  }

  return classifiers;
}

int main([[maybe_unused]] int argc, char* argv[]) {
  std::ifstream istream {argv[2]};
  if (istream.is_open()) {
  }
  auto predictor = read_vector(istream);
  istream.close();

  auto features = LogregClassifier::features_t {};

  double y_pred_expected = 0.0;

  std::ifstream test_data {argv[1]};
  if (test_data.is_open()) {
  }

  std::size_t count = 0;
  double sum        = 0.0;

  for (;;) {
    test_data >> y_pred_expected;
    if (!read_features(test_data, features)) {
      break;
    }
    auto y_pred = predictor[y_pred_expected].predict_proba(features);
    sum += y_pred;
    ++count;
    // std::cout << "class-> " << y_pred_expected << " coefficient " << y_pred
    //           << std::endl;
  }

  std::cout << sum / count << std::endl;

  return 0;
}
