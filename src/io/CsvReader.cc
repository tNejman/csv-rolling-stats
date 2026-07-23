#include "io/CsvReader.h"

#include <array>
#include <charconv>
#include <expected>
#include <istream>
#include <optional>
#include <string_view>
#include <system_error>

#include "ISampleReader.h"
#include "io/IoLib.hpp"

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
[[nodiscard]] std::optional<double> CsvReader::toDouble(
    const std::array<char, io_lib::DOUBLE_STR_BUFFER_SIZE>& char_array ) noexcept {
  double constructed_value{};
  auto [ptr, ec] = std::from_chars( char_array.data(), char_array.data() + char_array.size(), constructed_value );
  if ( ec != std::errc{} ) {
    return std::nullopt;
  }
  return constructed_value;
}
// NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)

CsvReader::CsvReader( std::istream& stream ) noexcept : ISampleReader( stream ) {
}

[[nodiscard]] bool CsvReader::validateColumnName() noexcept {
  static constexpr std::string_view COLUMN_NAMES = "Wejście;";
  const auto first_line_buf = nextLine();
  return first_line_buf.has_value() && std::string_view( first_line_buf->data() ) == COLUMN_NAMES;
}

[[nodiscard]] std::expected<double, ReadError> CsvReader::nextSample() noexcept {
  auto line = nextLine();
  if ( !line ) {
    switch ( line.error() ) {
      case ReadLineError::END_OF_FILE: return std::unexpected{ ReadError::END_OF_FILE };
      case ReadLineError::HARDWARE_ISSUE: return std::unexpected{ ReadError::HARDWARE_ISUE };
      case ReadLineError::TOO_LONG_LINE: return std::unexpected{ ReadError::PARSE_ERROR };
    }
  }
  auto maybe_double = toDouble( *line );
  if ( !maybe_double ) {
    return std::unexpected{ ReadError::PARSE_ERROR };
  }
  return *maybe_double;
}