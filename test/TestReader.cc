#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <ios>
#include <sstream>

#include "ISampleReader.h"
#include "TestHelper.hpp"
#include "io/CsvReader.h"
#include "io/IoLib.hpp"

// NOLINTBEGIN

TEST_CASE( "empty-stream" ) {
  std::stringstream string_stream{};
  CsvReader reader{ string_stream };
  auto maybe_line = reader.nextSample();
  REQUIRE( maybe_line.error() == ReadError::END_OF_FILE );
}

TEST_CASE( "broken-stream" ) {
  std::stringstream string_stream{};
  string_stream.setstate( std::ios::badbit );
  CsvReader reader{ string_stream };
  auto maybe_line = reader.nextSample();
  REQUIRE( maybe_line.error() == ReadError::HARDWARE_ISUE );
}

TEST_CASE( "single-sample" ) {
  std::stringstream string_stream{ "1.5;\n" };
  CsvReader reader{ string_stream };
  auto maybe_line = reader.nextSample();
  REQUIRE( maybe_line.value() == Catch::Approx( 1.5 ) );
}

TEST_CASE( "single-sample-no-newline-at-the-end" ) {
  std::stringstream string_stream{ "1.5;" };
  CsvReader reader{ string_stream };
  auto maybe_line = reader.nextSample();
  REQUIRE( maybe_line.value() == Catch::Approx( 1.5 ) );
}

TEST_CASE( "two-samples-no-return-duplicate" ) {
  std::stringstream string_stream{ "1.5;\n2.0\n" };
  CsvReader reader{ string_stream };
  auto maybe_line = reader.nextSample();
  REQUIRE( maybe_line.value() == Catch::Approx( 1.5 ) );
  auto maybe_line2 = reader.nextSample();
  REQUIRE( maybe_line2.value() == Catch::Approx( 2.0 ) );
}

TEST_CASE( "read-after-sample" ) {
  std::stringstream string_stream{ "1.5;\n" };
  CsvReader reader{ string_stream };
  auto maybe_line = reader.nextSample();
  REQUIRE( maybe_line.value() == Catch::Approx( 1.5 ) );
  auto maybe_line2 = reader.nextSample();
  REQUIRE( maybe_line2.error() == ReadError::END_OF_FILE );
}

TEST_CASE( "line-exactly-(N-1)-sized" ) {
  std::stringstream string_stream{};
  for ( int i = 0; i < io_lib::DOUBLE_STR_BUFFER_SIZE - 4; ++i ) {
    string_stream << '1';
  }
  string_stream << ".00\n";  // newline doesn't go in the buffer
  CsvReader reader{ string_stream };
  auto maybe_line = reader.nextSample();

  double expected_val = [&] {
    double base = 1.0;
    for ( int i = 0; i < io_lib::DOUBLE_STR_BUFFER_SIZE - 5; ++i ) {
      base *= 10;
      base += 1;
    }
    return base;
  }();

  REQUIRE( maybe_line.value() == Catch::Approx( expected_val ) );
}

TEST_CASE( "line-exactly-(N)-sized" ) {
  std::stringstream string_stream{};
  for ( int i = 0; i < io_lib::DOUBLE_STR_BUFFER_SIZE - 3; ++i ) {
    string_stream << '1';
  }
  string_stream << ".00\n";  // newline doesn't go in the buffer
  CsvReader reader{ string_stream };
  auto maybe_line = reader.nextSample();
  REQUIRE( maybe_line.error() == ReadError::PARSE_ERROR );
}

TEST_CASE( "line-longer-than-buffer" ) {
  std::stringstream string_stream{};
  for ( int i = 0; i < io_lib::DOUBLE_STR_BUFFER_SIZE + 1; ++i ) {
    string_stream << '1';
  }
  CsvReader reader{ string_stream };
  auto maybe_line = reader.nextSample();
  REQUIRE( maybe_line.error() == ReadError::PARSE_ERROR );
}

TEST_CASE( "empty-line" ) {
  std::stringstream string_stream{ "\n" };
  CsvReader reader{ string_stream };
  auto maybe_line = reader.nextSample();
  REQUIRE( maybe_line.error() == ReadError::PARSE_ERROR );
}

// NOLINTEND