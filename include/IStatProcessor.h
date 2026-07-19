#pragma once

#include "StatRow.h"

class IStatProcessor {  // NOLINT(cppcoreguidelines-special-member-functions)
 private:
 public:
  virtual ~IStatProcessor() = default;

  virtual StatRow process( double sample ) = 0;
};