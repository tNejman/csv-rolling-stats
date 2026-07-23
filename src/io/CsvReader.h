#pragma once

#include <array>
#include <expected>
#include <istream>
#include <optional>

#include "ISampleReader.h"
#include "io/IoLib.hpp"

class CsvReader : public ISampleReader {
  [[nodiscard]] static std::optional<double> toDouble(
      const std::array<char, io_lib::DOUBLE_STR_BUFFER_SIZE>& char_array ) noexcept;

 public:
  CsvReader( std::istream& stream ) noexcept;

  [[nodiscard]] bool validateColumnName() noexcept override;
  [[nodiscard]] std::expected<double, ReadError> nextSample() noexcept override;
};