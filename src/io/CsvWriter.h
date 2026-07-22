#pragma once

#include "IStatWriter.h"
#include "StatRow.h"
#include "io/IoLib.hpp"

class CsvWriter : public IStatWriter {
  static std::optional<std::array<char, io_lib::DOUBLE_STR_BUFFER_SIZE>> doubleToCharArr(
      double to_be_processed ) noexcept;
  static void truncateDoubleBuffer( std::array<char, io_lib::DOUBLE_STR_BUFFER_SIZE>& to_be_prepared ) noexcept;

 public:
  CsvWriter( std::ostream& stream );
  bool writeStatRow( StatRow row_to_write ) noexcept override;
};