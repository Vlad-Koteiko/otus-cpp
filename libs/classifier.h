#pragma once

#include <vector>

namespace kdd99 {

  class BinaryClassifier {
   public:
    using features_t = std::vector<double>;

    virtual ~BinaryClassifier() {}

    virtual double predict_proba(const features_t&) const = 0;
  };

}  // namespace kdd99
