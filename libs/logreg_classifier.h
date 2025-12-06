#pragma once

#include <vector>

#include "classifier.h"

namespace kdd99 {

  class LogregClassifier : public BinaryClassifier {
   public:
    using coef_t = features_t;

    LogregClassifier(const coef_t& coef);

    double predict_proba(const features_t& feat) const override;

   protected:
    std::vector<double> coef_;
  };

}  // namespace kdd99
