#pragma once

#include "IStatProcessor.h"
#include "StatRow.h"
#include "core/RollingMax.h"
#include "core/RollingMean.h"
#include "core/RollingMin.h"

template <int WindowSize>
requires( WindowSize > 0 )
class StatProcessor : public IStatProcessor {
 private:
  RollingMean<double, WindowSize> rolling_mean_calculator_{};
  RollingMax<double, WindowSize> rolling_max_calculator_{};
  RollingMin<double, WindowSize> rolling_min_calculator_{};

 public:
  StatProcessor() = default;

  StatRow process( double sample ) override;
};

#include "core/StatProcessor.tpp"