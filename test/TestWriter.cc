#include <catch2/catch_test_macros.hpp>
#include <sstream>

#include "StatRow.h"
#include "io/CsvWriter.h"
#include "io/IoLib.hpp"

// NOLINTBEGIN

TEST_CASE( "write-simple-sample" ) {
  std::stringstream string_stream{};
  CsvWriter writer{ string_stream };
  REQUIRE(
      writer.writeStatRow( StatRow{ OrgInputVal{ 1.0 }, MeanRolVal{ 1.0 }, MaxRolVal{ 1.0 }, MinRolVal{ 1.0 } } ) );
  REQUIRE( string_stream.str() == "Wejście; Avg; Max; Min;\n1.00; 1.00; 1.00; 1.00; \n" );
}

TEST_CASE( "write-too-long-double" ) {
  std::stringstream string_stream{};
  CsvWriter writer{ string_stream };
  double org_val = [] {
    double base = 1.0;
    for ( int i = 0; i < io_lib::DOUBLE_STR_BUFFER_SIZE + 5; ++i ) {
      base *= 10;
    }
    return base;
  }();
  REQUIRE( !writer.writeStatRow(
      StatRow{ OrgInputVal{ org_val }, MeanRolVal{ 1.0 }, MaxRolVal{ 1.0 }, MinRolVal{ 1.0 } } ) );
  REQUIRE( string_stream.str() == "Wejście; Avg; Max; Min;\n" );
}

// NOLINTEND