#pragma once

#include "IStatProcessor.h"
#include "StatRow.h"

class StatProcessor : public IStatProcessor {
 public:
  StatRow process( double sample ) override;
};