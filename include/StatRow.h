#pragma once

#include "NamedVal.hpp"

using OrgInputVal = NamedVal<double, struct OrgInputValTag>;
using MeanRolVal = NamedVal<double, struct MeanRolValTag>;
using MaxRolVal = NamedVal<double, struct MaxRolValTag>;
using MinRolVal = NamedVal<double, struct MinRolValTag>;

struct StatRow {
 public:
  OrgInputVal org_input_;
  MeanRolVal mean_;
  MaxRolVal max_;
  MinRolVal min_;

  explicit StatRow( OrgInputVal org_input, MeanRolVal mean, MaxRolVal max, MinRolVal min )
      : org_input_( org_input ), mean_( mean ), max_( max ), min_( min ) {
  }

  bool operator==( const StatRow& other ) const noexcept = default;
};