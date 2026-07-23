#include <array>
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <cstdlib>
#include <utility>
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
  StatProcessor<WINDOW_SIZE> stat_processor;
  auto output = test_helper::drainAndProcessAllSamples( mock_reader, stat_processor );

  REQUIRE( output.size() == input.size() );
  for ( std::size_t i = 0; i < output.size(); ++i ) {
    CAPTURE( i );
    test_helper::checkRowApprox( output.at( i ), test_helper::calculateExpectedRow( input, i, WINDOW_SIZE ) );
  }
}

TEST_CASE( "baseline-different-windows" ) {
  test_helper::testBodyStatProcessor( { 1.0, 2.0, 5.0, 3.0, 4.0, 2.0 } );
}

TEST_CASE( "monotonic-non-decreasing-different-windows" ) {
  test_helper::testBodyStatProcessor( { 1.0, 1.0, 2.0, 2.0, 4.0, 1000.0 } );
}

TEST_CASE( "monotonic-non-increasing-different-windows" ) {
  test_helper::testBodyStatProcessor( { 420.0, 67.0, 5.0, 5.0, 0.0, 0.0 } );
}

TEST_CASE( "all-samples-equal-and-zero-different-windows" ) {
  test_helper::testBodyStatProcessor( { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } );
}

TEST_CASE( "all-samples-equal-non-zero-different-windows" ) {
  test_helper::testBodyStatProcessor( { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 } );
}

TEST_CASE( "two-maximums-overlapping-window-different-windows" ) {
  test_helper::testBodyStatProcessor( { 5.0, 1000.0, 5.0, 5.0, 1000.0, 5.0 } );
}

TEST_CASE( "two-minimums-overlapping-window-different-windows" ) {
  test_helper::testBodyStatProcessor( { 5.0, 0.0, 5.0, 5.0, 0.0, 5.0 } );
}

TEST_CASE( "no-samples-different-windows" ) {
  test_helper::testBodyStatProcessor( { 1000.0, 5.0, 5.0, 5.0, 5.0, 1000.0 } );
}

// NOLINTEND(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)