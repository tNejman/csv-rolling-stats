#pragma once

#include <expected>
#include "ISampleReader.h"

class CsvReader : public ISampleReader {
  std::expected<double, ReadError> nextSample() override;
};