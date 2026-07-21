#pragma once

#include "StatRow.h"
#include "core/StatProcessor.h"

template <int WindowSize>
requires( WindowSize > 0 )
StatRow StatProcessor<WindowSize>::process( double sample ) {
  if constexpr ( WindowSize == 1 ) {
    return StatRow{ OrgInputVal{ sample }, MeanRolVal{ sample }, MaxRolVal{ sample }, MinRolVal{ sample } };
  } else {
    return StatRow{ OrgInputVal{ sample }, MeanRolVal{ rolling_mean_calculator_.getMean() },
                    MaxRolVal{ rolling_max_calculator_.getOptimum() },
                    MinRolVal{ rolling_min_calculator_.getOptimum() } };
  }
}