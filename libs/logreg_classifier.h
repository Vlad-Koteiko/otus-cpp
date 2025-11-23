#pragma once

#include <vector>

#include "classifier.h"

namespace kdd99 {

  class LogregClassifier : public BinaryClassifier {
   public:
    using coef_t = features_t;

    LogregClassifier(const coef_t& coef);

    float predict_proba(const features_t& feat) const override;

   protected:
    std::vector<float> coef_;
  };

}  // namespace kdd99
