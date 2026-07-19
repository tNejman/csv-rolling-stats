#include "core/StatProcessor.h"

#include "StatRow.h"

StatRow StatProcessor::process( double sample ) {
  return StatRow{ OrgInputVal{ sample }, MeanRolVal{ 0.0 }, MaxRolVal{ 0.0 }, MinRolVal{ 0.0 } };
}