#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <vector>

#include "MockReader.hpp"
#include "TestHelper.hpp"
#include "core/StatProcessor.h"

// NOLINTBEGIN(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)

TEST_CASE( "check-init" ) {
  REQUIRE( true );
}

/* === Stat Processor === */

TEST_CASE( "baseline-on-given-data" ) {
  const std::vector<double> input = { 1.0, 2.0, 5.0, 3.0, 4.0, 2.0 };
  constexpr int WINDOW_SIZE = 3;

  MockReader mock_reader{ input };
  StatProcessor stat_processor;
  auto output = drainAndProcessAllSamples( mock_reader, stat_processor );

  REQUIRE( output.size() == input.size() );
  for ( std::size_t i = 0; i < output.size(); ++i ) {
    CAPTURE( i );
    checkRowApprox( output.at( i ), calculateExpectedRow( input, i, WINDOW_SIZE ) );
  }
}

TEST_CASE( "baseline-window-size=1" ) {
  const std::vector<double> input = { 1.0, 2.0, 5.0, 3.0, 4.0, 2.0 };
  constexpr int WINDOW_SIZE = 1;

  MockReader mock_reader{ input };
  StatProcessor stat_processor;
  auto output = drainAndProcessAllSamples( mock_reader, stat_processor );

  REQUIRE( output.size() == input.size() );
  for ( std::size_t i = 0; i < output.size(); ++i ) {
    CAPTURE( i );
    checkRowApprox( output.at( i ), calculateExpectedRow( input, i, WINDOW_SIZE ) );
  }
}

// NOLINTEND(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)