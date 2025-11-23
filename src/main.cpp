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

std::vector<float> read_vector(std::istream& stream) {
  std::vector<float> result;

  std::copy(std::istream_iterator<float>(stream),
            std::istream_iterator<float>(),
            std::back_inserter(result));
  return result;
}

int main([[maybe_unused]] int argc, char* argv[]) {
  
  std::ifstream istream {argv[1]};
  if (istream.is_open()) {
  }
  auto coef = read_vector(istream);
  istream.close();

  auto predictor = LogregClassifier {coef};

  auto features = LogregClassifier::features_t {};

  double y_pred_expected = 0.0;

  std::ifstream test_data {argv[2]};
  if (test_data.is_open()) {
  }

  for (;;) {
    test_data >> y_pred_expected;
    if (!read_features(test_data, features)) {
      break;
    }
    auto y_pred = predictor.predict_proba(features);
    std::cout << "class-> " << y_pred_expected << " coefficient " << y_pred
              << std::endl;
  }

  return 0;
}
