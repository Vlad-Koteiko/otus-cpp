#define BOOST_TEST_MODULE test_version

#include <boost/test/unit_test.hpp>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>

#include "libs/classifier.h"
#include "libs/logreg_classifier.h"

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

BOOST_AUTO_TEST_SUITE(test_version)

BOOST_AUTO_TEST_CASE(TestBD) {
  std::ifstream istream {"../logreg_coef.txt"};
  BOOST_REQUIRE(istream.is_open());
  auto coef = read_vector(istream);
  istream.close();

  auto predictor = LogregClassifier {coef};

  auto features = LogregClassifier::features_t {};

  double y_pred_expected = 0.0;

  std::ifstream test_data {"../test_data_logreg.txt"};
  BOOST_REQUIRE(test_data.is_open());
  for (;;) {
    test_data >> y_pred_expected;
    if (!read_features(test_data, features)) {
      break;
    }
    auto y_pred = predictor.predict_proba(features);
    std::cout << "y_pred " << y_pred << " y_pred_expected " << y_pred_expected << " size " << features.size() << " size coef " << coef.size() << std::endl;
    // BOOST_TEST(y_pred_expected == y_pred, boost::test_tools::tolerance(1e-5));
  }
}

BOOST_AUTO_TEST_SUITE_END()
