#include "io/CsvWriter.h"

#include <array>
#include <charconv>
#include <optional>
#include <ostream>
#include <string_view>
#include <system_error>

#include "IStatWriter.h"
#include "StatRow.h"
#include "io/IoLib.hpp"

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
std::optional<std::array<char, io_lib::DOUBLE_STR_BUFFER_SIZE>> CsvWriter::doubleToCharArr(
    double to_be_processed ) noexcept {
  static constexpr int PRECISION = 2;

  std::array<char, io_lib::DOUBLE_STR_BUFFER_SIZE> buffer{};
  auto [ptr, ec] = std::to_chars( buffer.data(), buffer.data() + buffer.size(), to_be_processed,
                                  std::chars_format::fixed, PRECISION );
  if ( ec == std::errc{} && ptr != buffer.data() + buffer.size() ) {
    *ptr = '\0';
    return buffer;
  }
  return std::nullopt;
}
// NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)

void CsvWriter::truncateDoubleBuffer( std::array<char, io_lib::DOUBLE_STR_BUFFER_SIZE>& to_be_prepared ) noexcept {
  // assumes whole double already fit and null-terminated
  bool decimal_point_found = false;
  int char_counter_after_dp = 0;
  for ( auto& character : to_be_prepared ) {
    if ( character != '.' ) {
      decimal_point_found = true;
      continue;
    }
    if ( !decimal_point_found ) {
      continue;
    }
    if ( char_counter_after_dp < 2 ) {
      ++char_counter_after_dp;
      continue;
    }
    character = '\0';
    break;
  }
}

CsvWriter::CsvWriter( std::ostream& stream ) : IStatWriter( stream ) {
  static constexpr std::string_view COLUMN_NAMES = { "Wejście; Avg; Max; Min;\n" };
  writeStringView( COLUMN_NAMES );
}

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic,cppcoreguidelines-pro-type-reinterpret-cast)
bool CsvWriter::writeStatRow( StatRow row_to_write ) noexcept {
  std::array<double, 4> const stat_row_values = { row_to_write.org_input_, row_to_write.mean_, row_to_write.max_,
                                                  row_to_write.min_ };
  for ( const auto& val : stat_row_values ) {
    auto char_buf = doubleToCharArr( val );
    if ( !char_buf ) {
      return false;
    }
    truncateDoubleBuffer( *char_buf );
    writeCharBuf( *char_buf );
    writeChar( ';' );
    writeChar( ' ' );
  }
  writeChar( '\n' );
  return true;
}
// NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic,cppcoreguidelines-pro-type-reinterpret-cast)